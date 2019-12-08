#include "InteractionSensingComponent.h"
#include "InteractionInterface.h"
#include "InteractionInvokerComponent.h"
#include "InteractionRuntimeSettings.h"
#include "InteractionTrace.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UInteractionSensingComponent::UInteractionSensingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	EnableHighlighting = true;
	EnableContinuousUpdates = true;
}

void UInteractionSensingComponent::BeginPlay()
{
	Super::BeginPlay();
	const UInteractionRuntimeSettings* interactionSettings = GetDefault<UInteractionRuntimeSettings>();
	if (interactionSettings)
	{
		InteractionTrace = FInteractionTrace::CreateTrace(interactionSettings);
	}
}

void UInteractionSensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	const APawn* controllerPawn = playerController ? playerController->GetPawn() : nullptr;

	FHitResult HitResult(ForceInit);
	FInteractionStimulus interactionStore;
	if (InteractionTrace.IsValid() &&
		InteractionTrace->Perform(controllerPawn, HitResult) &&
		InteractionTrace->IsInteractable(HitResult.Actor))
	{
		EInteractionStimulusType type = (PreviousInteractionTarget == HitResult.Actor) ? EInteractionStimulusType::Update : EInteractionStimulusType::Show;
		float progress = GetInteractionProgress();
		interactionStore = FInteractionStimulus(HitResult.Actor, type, progress, FInteractionStimulus::SensingSucceeded);
	}
	
	// broadcast only if it's different than the previous
	if (interactionStore.InteractionTarget != PreviousInteractionTarget)
	{
		CallStopHighlightOn(PreviousInteractionTarget);
	}

	if (interactionStore.InteractionTarget != PreviousInteractionTarget || (EnableContinuousUpdates && PreviousInteractionTarget != nullptr && interactionStore.InteractionTarget != nullptr))
	{
		OnTargetInteractionUpdated.Broadcast(interactionStore);
	}

	if (interactionStore.InteractionTarget != PreviousInteractionTarget)
	{
		PreviousInteractionTarget = interactionStore.InteractionTarget;
		CallStartHighlightOn(interactionStore.InteractionTarget);
	}

	return;
}

bool UInteractionSensingComponent::CallStartHighlightOn(TWeakObjectPtr<AActor> InteractionTarget) const
{
	if (EnableHighlighting && InteractionTarget.IsValid() && InteractionTarget->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		IInteractionInterface::Execute_StartInteractionHighlighting(InteractionTarget.Get());
		return true;
	}
	return false;
}

bool UInteractionSensingComponent::CallStopHighlightOn(TWeakObjectPtr<AActor> InteractionTarget) const
{
	if (EnableHighlighting && InteractionTarget.IsValid() && InteractionTarget->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		IInteractionInterface::Execute_StopInteractionHighlighting(InteractionTarget.Get());
		return true;
	}
	return false;
}

float UInteractionSensingComponent::GetInteractionProgress() const
{
	const APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	const APawn* controllerPawn = playerController ? playerController->GetPawn() : nullptr;
	const UInteractionInvokerComponent* interactionInvoker = controllerPawn ? controllerPawn->FindComponentByClass<UInteractionInvokerComponent>() : nullptr;
	if (interactionInvoker)
	{
		return interactionInvoker->GetInteractionProgress();
	}
	return 0.0f;
}
