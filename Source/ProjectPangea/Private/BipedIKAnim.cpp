// Fill out your copyright notice in the Description page of Project Settings.


#include "BipedIKAnim.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

#define RADtoDEG 57.295779513

void UBipedIKAnim::NativeUpdateAnimation(float DeltaSeconds) {
  if (owner_ == nullptr) {
    owner_ = Cast<ACharacter>(GetOwningActor());
  }
  if (owner_ != nullptr) {
    actor_mesh_ = Cast<USkeletalMeshComponent>(owner_->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
    FootIK(DeltaSeconds);
  }
}

FHitResult UBipedIKAnim::CheckTrace(FVector basepos, FVector endpos) {
  FCollisionQueryParams IK_TraceParams =
    FCollisionQueryParams(FName(TEXT("Trace")), true, owner_);
  IK_TraceParams.bTraceComplex = true;
  IK_TraceParams.bReturnPhysicalMaterial = false;
  IK_TraceParams.TraceTag = FName("Trace");


  FHitResult Trace_Hit(ForceInit);

  bool validHit = GetWorld()->LineTraceSingleByChannel(
    Trace_Hit,         //result
    basepos,        //start
    endpos,         //end
    ECC_Visibility, //collision channel
    IK_TraceParams
  );

  if (draw_debug_) {
    DrawDebugLine(GetWorld(), basepos, endpos,
      FColor::Green, false, 0.0f);

    DrawDebugSphere(GetWorld(), Trace_Hit.ImpactPoint, 5.0f, 8, FColor::Red);
  }

  return Trace_Hit;
}


void UBipedIKAnim::FootIK(float deltatime){

  FVector actor_loc = owner_->GetActorLocation();
  FVector left_foot_loc = actor_mesh_->GetSocketLocation(left_socket_);
  left_foot_loc.Z = actor_loc.Z;

  FHitResult left_trace = CheckTrace(left_foot_loc,
    left_foot_loc - FVector(0.0f, 0.0f, hip_height_ + max_height_check_));

  if (left_trace.IsValidBlockingHit()) {
    float hit_Z = left_trace.ImpactPoint.Z;
    float foot_Z = actor_loc.Z - hip_height_;

    float Z_offset = hit_Z - foot_Z;
    //Check if over or under capsule bottom
    if (Z_offset < 0.1f && Z_offset > -0.1f) {
      left_foot_offset_ = FMath::FInterpTo(left_foot_offset_, 0.0f, deltatime, interp_speed_foot);
      left_hip_offset_ = FMath::FInterpTo(left_hip_offset_, 0.0f, deltatime, interp_speed_foot);
    } else if (Z_offset > 0.0f) {
      left_foot_offset_ = FMath::FInterpTo(left_foot_offset_, Z_offset, deltatime, interp_speed_foot);
    } else {
      left_hip_offset_ = FMath::FInterpTo(left_hip_offset_, Z_offset, deltatime, interp_speed_hip);
    }

    FVector normal = left_trace.ImpactNormal;

    float pitch = FMath::Atan2(normal.X, normal.Z)*RADtoDEG * -1.0f;
    float roll = FMath::Atan2(normal.Y, normal.Z)*RADtoDEG;

    left_rotation = FVector(roll,pitch,0.0f);

  }

  FVector right_foot_loc = actor_mesh_->GetSocketLocation(right_socket_);
  right_foot_loc.Z = actor_loc.Z;
  FHitResult right_trace = CheckTrace(right_foot_loc,
    right_foot_loc - FVector(0.0f, 0.0f, hip_height_ + max_height_check_));

  if (right_trace.IsValidBlockingHit()) {
    float hit_Z = right_trace.ImpactPoint.Z;
    float foot_Z = actor_loc.Z - hip_height_;

    float Z_offset = hit_Z - foot_Z;

    //Check if over or under capsule bottom
    if (Z_offset < 0.1f && Z_offset > -0.1f) {
      right_foot_offset_ = FMath::FInterpTo(right_foot_offset_, 0.0f, deltatime, interp_speed_foot);
      right_hip_offset_ = FMath::FInterpTo(right_hip_offset_, 0.0f, deltatime, interp_speed_foot);
    }
    else if (Z_offset > 0.0f) {
      //Set foot offset
      right_foot_offset_ = FMath::FInterpTo(right_foot_offset_, Z_offset, deltatime, interp_speed_foot);
    }
    else {
      //Set hip offset
      right_hip_offset_ = FMath::FInterpTo(right_hip_offset_, Z_offset, deltatime, interp_speed_hip);
    }

    FVector normal = right_trace.ImpactNormal;

    float pitch = FMath::Atan2(normal.X, normal.Z)*RADtoDEG * -1.0f;
    float roll = FMath::Atan2(normal.Y, normal.Z)*RADtoDEG;

    right_rotation = FVector(roll, pitch, 0.0f);
  }

  UCharacterMovementComponent *charmov = owner_->GetCharacterMovement();
  if (charmov != nullptr) {
    if (charmov->IsFalling()) {
      ik_alpha_ = 0.0f;
      GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, "yee");
    } else ik_alpha_ = 1.0f;
  }


  if (draw_debug_) {
    DrawDebugCapsule(GetWorld(),owner_->GetActorLocation(),96.0f,30.f,
      FQuat::Identity,FColor::Green);

    GEngine->AddOnScreenDebugMessage(6, 0.1f, FColor::Red, FString("Left offset: ") + FString::SanitizeFloat(left_foot_offset_));
    GEngine->AddOnScreenDebugMessage(7, 0.1f, FColor::Red, FString("Right offset: ") + FString::SanitizeFloat(right_foot_offset_));
    GEngine->AddOnScreenDebugMessage(8, 0.1f, FColor::Red, FString("Roll: ") + FString::SanitizeFloat(left_rotation.X));
    GEngine->AddOnScreenDebugMessage(9, 0.1f, FColor::Red, FString("Pitch: ") + FString::SanitizeFloat(left_rotation.Y));

  }


  right_foot_offset_ = left_hip_offset_ - FMath::Clamp(right_foot_offset_,0.0f, max_height_check_);
  left_foot_offset_ = right_hip_offset_ - FMath::Clamp(left_foot_offset_, 0.0f, max_height_check_);
  //Add both offsets for when both legs are under capsule
  hip_offset_ = left_hip_offset_ + right_hip_offset_;
}

float UBipedIKAnim::Attack(int attack_index) {
  float montage_duration = 0.0f;
  if (attack_index < attack_montages_.Num()) {
    montage_duration = Montage_Play(attack_montages_[attack_index]);
  }
  return montage_duration;
}
