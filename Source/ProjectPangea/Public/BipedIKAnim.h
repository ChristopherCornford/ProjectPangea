// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BipedIKAnim.generated.h"

/**
 * 
 */

class AProjectPangeaCharacter;

UCLASS()
class PROJECTPANGEA_API UBipedIKAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
  //Left foot bone socket
  UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AnimVars | Skeleton")
    FName left_socket_;

  //Left foot bone socket
  UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AnimVars | Skeleton")
    FName right_socket_;

  //Hip bone name
  UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AnimVars | Skeleton")
    FName hip_bone_;

  //Distance from actor pivot point and flat theoretical floor
  UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AnimVars | Skeleton")
    float base_hip_height_;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AnimVars | Skeleton")
    float base_hip_height_crouched_;

  //Dont calculate over that distance (for ledges)
  UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AnimVars | IK Config")
    float max_height_check_;

  //interpolation speed for foot and hip height andjustments
  UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AnimVars | IK Config")
    float interp_speed_hip;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AnimVars | IK Config")
    float interp_speed_foot;

  //Draw debug capsule and rays
  UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AnimVars")
    bool draw_debug_;

  //offset for iknode
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AnimVars | IK Result")
    float left_foot_offset_;

  //offset for iknode
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AnimVars | IK Result")
    float right_foot_offset_;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AnimVars | IK Result")
    FVector left_rotation;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AnimVars | IK Result")
    FVector right_rotation;

  //hip transform
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AnimVars | IK Result")
    float hip_offset_;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AnimVars | IK Result")
    float ik_alpha_;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AnimVars | General Results")
    float input_angle_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "AnimVars | ActionAnims")
    TArray<UAnimMontage*> attack_montages_;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "AnimVars | ActionAnims")
    UAnimMontage* switch_180_;

  UFUNCTION()
    float Attack(int attack_index); //To do: change parameter to enum
  
protected:
  float left_hip_offset_;
  float right_hip_offset_;
  float hip_height_;

  AProjectPangeaCharacter *owner_;
  USkeletalMeshComponent *actor_mesh_;

  virtual void NativeUpdateAnimation(float DeltaSeconds) override;

  FHitResult CheckTrace(FVector basepos, FVector endpos);

  void FootIK(float deltatime);

  void AnimActions();
};
