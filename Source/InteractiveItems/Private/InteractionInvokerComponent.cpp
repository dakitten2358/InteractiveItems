#include "InteractionInvokerComponent.h"
#include "InteractionInterface.h"
#include "InteractionRuntimeSettings.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UInteractionInvokerComponent::UInteractionInvokerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InteractionTimerInterval = (1.0f / 20.0f);
}

void UInteractionInvokerComponent::BeginPlay()
{
	Super::BeginPlay();
	const UInteractionRuntimeSettings* interactionSettings = GetDefault<UInteractionRuntimeSettings>();
	if (interactionSettings)
	{
		InteractionTrace = FInteractionTrace::CreateTrace(interactionSettings);
	}
}

bool UInteractionInvokerComponent::BeginInteraction()
{
	FHitResult hitResult(ForceInit);
	if (InteractionTrace.IsValid() &&
		InteractionTrace->Perform(GetOwner(), hitResult) &&
		InteractionTrace->IsInteractable(hitResult.Actor))
	{
		float Duration = 0.0f;
		TWeakObjectPtr<AActor> interactionTarget = hitResult.Actor;
		EInteractionType interactionType = GetInteractionType(interactionTarget, Duration);

		switch (interactionType)
		{
		case EInteractionType::Duration:
			PerformInteractionTypeDuration(Duration, interactionTarget);
			return true;
		case EInteractionType::Instant:
		default:
			PerformInteractionTypeInstant(interactionTarget);
			return true;
		}
	}
	return false;
}

bool UInteractionInvokerComponent::CancelInteraction()
{
	// cancel any pending interactions
	if (InProgressTarget.Get() != nullptr)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(InProgressTimerHandle);

		FireInteractionCancelled();

		InProgressTarget.Reset();
		InProgressElapsed = 0.0;
		InProgressDuration = 0.0f;
		return true;
	}	

	return false;
}

void UInteractionInvokerComponent::Interact(AActor* InteractionTarget)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		InteractOnServer(InteractionTarget);
		return;
	}

	IInteractionInterface::Execute_Interact(InteractionTarget, GetOwner());
}

void UInteractionInvokerComponent::InteractOnServer_Implementation(AActor* InteractionTarget)
{
	Interact(InteractionTarget);
}

void UInteractionInvokerComponent::PerformInteractionTypeInstant(TWeakObjectPtr<AActor> OnActor)
{
	if (OnActor.IsValid() && OnActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		Interact(OnActor.Get());
	}
}

void UInteractionInvokerComponent::PerformInteractionTypeDuration(float Duration, TWeakObjectPtr<AActor> OnActor)
{
	InProgressTarget = OnActor;
	InProgressDuration = Duration;
	InProgressElapsed = 0.0f;

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(InProgressTimerHandle, [this] { InteractionTimerCallback(); }, InteractionTimerInterval, true);
}

void UInteractionInvokerComponent::InteractionTimerCallback()
{
	InProgressElapsed += InteractionTimerInterval;

	// Are we done?
	if (InProgressElapsed >= InProgressDuration)
	{
		// interaction has been completed
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(InProgressTimerHandle);

		if (InProgressTarget.IsValid() && InProgressTarget->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			Interact(InProgressTarget.Get());
		}

		// fire an event 
		FireInteractionProgress();
		FireInteractionCompleted();

		// clear out parameters
		InProgressTarget.Reset();
		InProgressElapsed = 0.0f;
		InProgressDuration = 0.0f;
		
		return;
	}

	// We're not done, but have we strayed off target?
	FHitResult hitResult(ForceInit);
	if (InteractionTrace.IsValid() &&
		InteractionTrace->Perform(GetOwner(), hitResult) &&
		InteractionTrace->IsInteractable(hitResult.Actor) &&
		hitResult.Actor == InProgressTarget)
	{
		// still on target, send out an interaction updated event
		FireInteractionProgress();
	}
	else
	{
		// no longer on target, cancel this interaction
		CancelInteraction();
	}
}

float UInteractionInvokerComponent::GetInteractionProgress() const
{
	return InProgressDuration > 0.0f ? FMath::Clamp((InProgressElapsed / InProgressDuration), 0.0f, 1.0f) : 0.0f;
}

EInteractionType UInteractionInvokerComponent::GetInteractionType(TWeakObjectPtr<AActor> ForActor, float& Duration)
{
	if (ForActor.IsValid() && ForActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		return IInteractionInterface::Execute_GetInteractionType(ForActor.Get(), Duration);
	}
	return EInteractionType::Instant;
}

void UInteractionInvokerComponent::FireInteractionProgress()
{
	OnInteractionProgress.Broadcast(FInteractionStatus(InProgressTarget, InProgressElapsed, InProgressDuration));
}

void UInteractionInvokerComponent::FireInteractionCompleted()
{
	OnInteractionCompleted.Broadcast(FInteractionStatus(InProgressTarget, InProgressElapsed, InProgressDuration));
}

void UInteractionInvokerComponent::FireInteractionCancelled()
{
	OnInteractionCancelled.Broadcast(FInteractionStatus(InProgressTarget, InProgressElapsed, InProgressDuration));
}
