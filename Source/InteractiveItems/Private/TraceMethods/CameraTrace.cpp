#include "CameraTrace.h"
#include "InteractionRuntimeSettings.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"

FCameraTrace::FCameraTrace(const UInteractionRuntimeSettings* InteractionSettings) : FInteractionTrace(InteractionSettings)
{
	MaximumCameraDistance = InteractionSettings->InteractionCameraDistance;
}

bool FCameraTrace::Perform(const AActor* ForActor, FHitResult& HitResult)
{
	FVector cameraLocation, cameraDirection;
	if (!GetCameraLocationAndDirection(ForActor, cameraLocation, cameraDirection))
		return false;

	FVector traceEnd = cameraLocation + (cameraDirection * MaximumCameraDistance);
	if (!Trace(ForActor, HitResult, cameraLocation, traceEnd))
	{
		DrawDebugTrace(ForActor->GetWorld(), cameraLocation, traceEnd, FColor::Green);
		return false;
	}

	DrawDebugTrace(ForActor->GetWorld(), cameraLocation, HitResult.ImpactPoint, FColor::Red);
	DrawDebugTrace(ForActor->GetWorld(), HitResult.ImpactPoint, traceEnd, FColor::Green);

	return true;
}
