#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/WeakObjectPtr.h"

#include "InteractionTrace.h"
#include "InteractionTypes.h"
#include "InteractionInvokerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionProgressDelegate, FInteractionStatus, Status);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionCompletedDelegate, FInteractionStatus, Status);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionCancelledDelegate, FInteractionStatus, Status);

UCLASS(ClassGroup=Interaction, meta=(BlueprintSpawnableComponent))
class INTERACTIVEITEMS_API UInteractionInvokerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionInvokerComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool BeginInteraction();

	UFUNCTION(BlueprintCallable)
	bool CancelInteraction();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Interaction)
	float InteractionTimerInterval;

	UPROPERTY(BlueprintAssignable)
	FInteractionProgressDelegate OnInteractionProgress;

	UPROPERTY(BlueprintAssignable)
	FInteractionCompletedDelegate OnInteractionCompleted;

	UPROPERTY(BlueprintAssignable)
	FInteractionCancelledDelegate OnInteractionCancelled;

	UFUNCTION(BlueprintCallable)
	float GetInteractionProgress() const;

private:
	UFUNCTION(Server, Reliable)
	void InteractOnServer(class AActor* InteractionTarget);
	void InteractOnServer_Implementation(class AActor* InteractionTarget);

	void Interact(class AActor* InteractionTarget);

	TUniquePtr<FInteractionTrace> InteractionTrace;

private:
	EInteractionType GetInteractionType(const TWeakObjectPtr<class AActor> ForActor, float& Duration);
	void PerformInteractionTypeInstant(const TWeakObjectPtr<class AActor> ForActor);
	void PerformInteractionTypeDuration(float Duration, const TWeakObjectPtr<class AActor> ForActor);

	void InteractionTimerCallback();

	void FireInteractionProgress();
	void FireInteractionCompleted();
	void FireInteractionCancelled();	

private:
	TWeakObjectPtr<class AActor> InProgressTarget;
	float InProgressDuration;
	float InProgressElapsed;
	FTimerHandle InProgressTimerHandle;
};
