#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "InteractionTrace.h"
#include "InteractionTypes.h"
#include "InteractionSensingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionUpdatedDelegate, FInteractionStimulus, Stimulus);

UCLASS(ClassGroup=Interaction, HideCategories=(Activation, Collision), meta=(BlueprintSpawnableComponent, ShortTooltip = "An InteractionComponent is meant to be added to the HUD to detect interactable items.") )
class INTERACTIVEITEMS_API UInteractionSensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionSensingComponent();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
	TWeakObjectPtr<class AActor> PreviousInteractionTarget;

	bool CallStartHighlightOn(TWeakObjectPtr<class AActor> InteractionTarget) const;
	bool CallStopHighlightOn(TWeakObjectPtr<class AActor> InteractionTarget) const;

	TUniquePtr<FInteractionTrace> InteractionTrace;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableHighlighting;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableContinuousUpdates;

	UPROPERTY(BlueprintAssignable)
	FInteractionUpdatedDelegate OnTargetInteractionUpdated;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetInteractionProgress() const;
};
