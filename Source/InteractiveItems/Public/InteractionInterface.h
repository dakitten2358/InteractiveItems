#pragma once

#include "CoreMinimal.h"
#include "InteractionTypes.h"
#include "InteractionInterface.generated.h"

UINTERFACE(BlueprintType)
class INTERACTIVEITEMS_API UInteractionInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class INTERACTIVEITEMS_API IInteractionInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Interaction)
	void Interact(const class AActor* InteractingActor);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Interaction)
	bool IsInteractionTarget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Interaction)
	EInteractionType GetInteractionType(float& Duration);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Interaction)
	bool GetInteractionDetails(FInteractionDetails& Detail);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Interaction)
	bool GetInteractionWorldLocation(FVector& WorldLocation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Interaction)
	void StartInteractionHighlighting();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=Interaction)
	void StopInteractionHighlighting();
};
