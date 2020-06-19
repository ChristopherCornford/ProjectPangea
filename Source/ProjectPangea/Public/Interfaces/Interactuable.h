// Author: Pablo Bengoa Nagy <p.bengoanagy@gmail.com>

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactuable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractuable : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTPANGEA_API IInteractuable {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactuable")
    bool Interact();
};
