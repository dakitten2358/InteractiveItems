#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "InteractionTypes.h"
#include "InteractionWidgetInterface.generated.h"

UINTERFACE(BlueprintType)
class INTERACTIVEITEMS_API UInteractionWidgetInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class INTERACTIVEITEMS_API IInteractionWidgetInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Interaction)
	void ShowInteractionWidget(FVector2D Position, float Progress, AActor* InteractionTarget);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Interaction)
	void HideInteractionWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Interaction)
	void UpdateInteractionWidget(FVector2D Position, float Progress, AActor* InteractionTarget);
};
