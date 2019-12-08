#pragma once

#include "CoreMinimal.h"
#include "InteractionTrace.h"

class FPawnTrace : public FInteractionTrace
{
public:
	FPawnTrace(const class UInteractionRuntimeSettings* InteractionSettings);
	virtual bool Perform(const class AActor* ForActor, struct FHitResult& HitResult) override;

private:
	float MaximumPawnDistance;
};
