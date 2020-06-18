// Author: Gabriel Rodríguez Viana <rodriguezvi2esat-alumni.com>

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemLogic.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemLogic : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTPANGEA_API IItemLogic {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactuable")
    bool Use();

  //More added later for extra functionality
};
