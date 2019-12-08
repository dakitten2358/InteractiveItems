#include "InteractionTypes.h"
#include "InteractionInterface.h"

#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetLayoutLibrary.h"

FInteractionStimulus::FInteractionStimulus()
	: InteractionTarget(nullptr)
	, InteractionType(EInteractionStimulusType::Hide)
	, InteractionProgress(0.0f)
	, bIsInteractionAvailable(0)
{
}

FInteractionStimulus::FInteractionStimulus(TWeakObjectPtr<AActor> InteractionTargetActor, EInteractionStimulusType InteractionType, float InteractionProgress, FInteractionStimulus::FResult Result)
	: InteractionTarget(InteractionTargetActor)
	, InteractionType(InteractionType)
	, InteractionProgress(InteractionProgress)
	, bIsInteractionAvailable(Result == SensingSucceeded && InteractionTargetActor.IsValid())
{
	if (bIsInteractionAvailable)
	{
		InteractionLocation = InteractionTarget->GetActorLocation();

		// check for an overridden location
		GetInteractionLocation(InteractionTarget, InteractionLocation);
		
		// ui related, so we can assume it's player 0
		auto playerController = UGameplayStatics::GetPlayerController(InteractionTarget.Get(), 0);
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(playerController, InteractionLocation, InteractionPosition);
	}
}

void FInteractionStimulus::GetInteractionLocation(TWeakObjectPtr<AActor> ForActor, FVector& WorldLocation)
{
	if (ForActor.IsValid() && ForActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		FVector NewWorldLocation;
		if (IInteractionInterface::Execute_GetInteractionWorldLocation(ForActor.Get(), NewWorldLocation))
		{
			WorldLocation = NewWorldLocation;
		}
	}
}

FInteractionStatus::FInteractionStatus() : InteractionTarget(nullptr), InteractionPercentComplete(0.0f)
{
}

FInteractionStatus::FInteractionStatus(TWeakObjectPtr<class AActor> Target, float Elapsed, float Duration)
	: InteractionTarget(Target)
{
	InteractionPercentComplete = (Duration > 0.0f) ? FMath::Clamp<float>((Elapsed / Duration), 0.0f, 1.0f) : 0.0f;
}
