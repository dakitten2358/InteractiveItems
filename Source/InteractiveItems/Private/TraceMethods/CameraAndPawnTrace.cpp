#include "CameraAndPawnTrace.h"
#include "InteractionRuntimeSettings.h"

#include "GameFramework/Actor.h"

FCameraAndPawnTrace::FCameraAndPawnTrace(const UInteractionRuntimeSettings* InteractionSettings) : FInteractionTrace(InteractionSettings)
{
	MaximumCameraDistance = InteractionSettings->InteractionCameraDistance;
	MaximumPawnDistance = InteractionSettings->InteractionDistance;
}

bool FCameraAndPawnTrace::Perform(const AActor* ForActor, FHitResult& HitResult)
{
	FVector cameraLocation, cameraDirection;
	if (!GetCameraLocationAndDirection(ForActor, cameraLocation, cameraDirection))
		return false;

	// first check to see if the camera is over anything interesting
	FVector traceEnd = cameraLocation + (cameraDirection * MaximumCameraDistance);
	if (!Trace(ForActor, HitResult, cameraLocation, traceEnd))
	{
		DrawDebugTrace(ForActor->GetWorld(), cameraLocation, traceEnd, FColor::Green);
		return false;
	}

	// we did hit something, so let's try again from the actor location
	FVector actorLocation = ForActor->GetActorLocation();
	FVector actorToTargetDirection = (HitResult.ImpactPoint - actorLocation).GetSafeNormal();
	FVector actorTraceEnd = actorLocation + (actorToTargetDirection * MaximumPawnDistance);
	if (!Trace(ForActor, HitResult, actorLocation, actorTraceEnd))
	{
		DrawDebugTrace(ForActor->GetWorld(), actorLocation, traceEnd, FColor::Green);
		return false;
	}

	DrawDebugTrace(ForActor->GetWorld(), actorLocation, HitResult.ImpactPoint, FColor::Red);
	DrawDebugTrace(ForActor->GetWorld(), HitResult.ImpactPoint, traceEnd, FColor::Green);

	return true;
}
