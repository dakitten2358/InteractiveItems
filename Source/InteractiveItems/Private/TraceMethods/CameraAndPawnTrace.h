#pragma once

#include "CoreMinimal.h"
#include "InteractionTrace.h"

class FCameraAndPawnTrace : public FInteractionTrace
{
public:
	FCameraAndPawnTrace(const class UInteractionRuntimeSettings* InteractionSettings);
	virtual bool Perform(const class AActor* ForActor, struct FHitResult& HitResult) override;

private:
	float MaximumCameraDistance;
	float MaximumPawnDistance;
	
};
