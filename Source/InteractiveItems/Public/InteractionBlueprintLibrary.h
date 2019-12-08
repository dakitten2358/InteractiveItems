#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionBlueprintLibrary.generated.h"

UCLASS()
class INTERACTIVEITEMS_API UInteractionBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Interaction")
	static bool GetBindingForInteractionAction(FKey& Binding);
};
