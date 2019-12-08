#include "PawnTrace.h"
#include "InteractionRuntimeSettings.h"

#include "GameFramework/Actor.h"

FPawnTrace::FPawnTrace(const UInteractionRuntimeSettings* InteractionSettings) : FInteractionTrace(InteractionSettings)
{
	MaximumPawnDistance = InteractionSettings->InteractionDistance;
}

bool FPawnTrace::Perform(const AActor* ForActor, FHitResult& HitResult)
{
	FVector actorLocation = ForActor->GetActorLocation();
	FVector actorDirection = ForActor->GetActorForwardVector();

	FVector traceEnd = actorLocation + (actorDirection * MaximumPawnDistance);
	if (!Trace(ForActor, HitResult, actorLocation, traceEnd))
	{
		DrawDebugTrace(ForActor->GetWorld(), actorLocation, traceEnd, FColor::Green);
		return false;
	}

	DrawDebugTrace(ForActor->GetWorld(), actorLocation, HitResult.ImpactPoint, FColor::Red);
	DrawDebugTrace(ForActor->GetWorld(), HitResult.ImpactPoint, traceEnd, FColor::Green);

	return true;
}
