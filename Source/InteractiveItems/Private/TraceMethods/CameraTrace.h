#pragma once

#include "CoreMinimal.h"
#include "InteractionTrace.h"

class FCameraTrace : public FInteractionTrace
{
public:
	FCameraTrace(const class UInteractionRuntimeSettings* interactionSettings);
	virtual bool Perform(const class AActor* ForActor, struct FHitResult& HitResult) override;

private:
	float MaximumCameraDistance;
};
