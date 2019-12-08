#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"

class FInteractionTrace
{
public:
	FInteractionTrace(const class UInteractionRuntimeSettings* InteractionSettings);
	virtual bool Perform(const class AActor* ForActor, struct FHitResult& HitResult) = 0;
	bool IsInteractable(const TWeakObjectPtr<class AActor>& TargetActor);

protected:
	void DrawDebugTrace(class UWorld* world, const FVector& start, const FVector& end, const FColor color);
	bool GetCameraLocationAndDirection(const class AActor* ForActor, FVector& CameraLocation, FVector& CameraDirection);
	bool Trace(const class AActor* ForActor, struct FHitResult& HitResult, const FVector& start, const FVector& end);

private:
	EDrawDebugTrace::Type DrawDebugType;

public:
	static TUniquePtr<FInteractionTrace> CreateTrace(const class UInteractionRuntimeSettings* InteractionSettings);
};
