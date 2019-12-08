#include "InteractionTrace.h"
#include "InteractionInterface.h"
#include "InteractionRuntimeSettings.h"
#include "TraceMethods/CameraTrace.h"
#include "TraceMethods/CameraAndPawnTrace.h"
#include "TraceMethods/PawnTrace.h"

#include "Camera/CameraComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

FInteractionTrace::FInteractionTrace(const UInteractionRuntimeSettings* InteractionSettings)
{
	check(InteractionSettings);
	DrawDebugType = InteractionSettings->GetDrawDebugType();
}

TUniquePtr<FInteractionTrace> FInteractionTrace::CreateTrace(const UInteractionRuntimeSettings* InteractionSettings)
{
	check(InteractionSettings);

	TUniquePtr<FInteractionTrace> trace;
	switch (InteractionSettings->TraceMethod)
	{
	case EInteractionTraceMethod::CameraAndPawn:
		trace = MakeUnique<FCameraAndPawnTrace>(InteractionSettings);
		break;
	case EInteractionTraceMethod::Camera:
		trace = MakeUnique<FCameraTrace>(InteractionSettings);
		break;
	case EInteractionTraceMethod::Pawn:
	default:
		trace = MakeUnique<FPawnTrace>(InteractionSettings);
		break;		
	}
	return trace;
}

void FInteractionTrace::DrawDebugTrace(UWorld* World, const FVector& Start, const FVector& End, const FColor Color)
{
	switch (DrawDebugType)
	{
	case EDrawDebugTrace::ForOneFrame:
		DrawDebugLine(World, Start, End, Color, false, -1.0f, ECC_WorldStatic, 1.f);
		break;
	case EDrawDebugTrace::ForDuration:
		DrawDebugLine(World, Start, End, Color, false, 10.0f, ECC_WorldStatic, 1.f);
		break;
	case EDrawDebugTrace::Persistent:
		DrawDebugLine(World, Start, End, Color, true, -1.0f, ECC_WorldStatic, 1.f);
		break;
	default:
		break;
	}
}

bool FInteractionTrace::GetCameraLocationAndDirection(const AActor* ForActor, FVector& CameraLocation, FVector& CameraDirection)
{
	CameraLocation = FVector::ZeroVector;
	CameraDirection = FVector::ForwardVector;

	// can we find a camera component on it?
	const UCameraComponent* cameraComponent = ForActor ? ForActor->FindComponentByClass<UCameraComponent>() : nullptr;
	if (!cameraComponent)
	{
		return false;
	}

	CameraLocation = cameraComponent->GetComponentLocation();
	CameraDirection = cameraComponent->GetForwardVector();
	return true;
}

bool FInteractionTrace::Trace(const AActor* ForActor, FHitResult& HitResult, const FVector& Start, const FVector& End)
{
	ECollisionChannel collisionChannel = ECollisionChannel::ECC_Visibility;
	bool traceComplex = true;
	FCollisionQueryParams queryParams(TEXT("InteractionTrace"), traceComplex, ForActor);
	ForActor->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, collisionChannel, queryParams);
	return HitResult.bBlockingHit;
}

bool FInteractionTrace::IsInteractable(const TWeakObjectPtr<AActor>& TargetActor)
{
	if (TargetActor.IsValid() && TargetActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		return IInteractionInterface::Execute_IsInteractionTarget(TargetActor.Get());
	}
	return false;
}
