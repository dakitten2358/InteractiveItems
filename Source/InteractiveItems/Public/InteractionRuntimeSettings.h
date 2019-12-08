#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/KismetSystemLibrary.h"

#include "InteractionTypes.h"
#include "InteractionRuntimeSettings.generated.h"

UCLASS(config=Engine, defaultconfig)
class INTERACTIVEITEMS_API UInteractionRuntimeSettings : public UObject
{
	GENERATED_BODY()

public:
	UInteractionRuntimeSettings();
	
	UPROPERTY(EditAnywhere, config, Category="Input")
	FName InteractionInputActionName;

	UPROPERTY(EditAnywhere, config, Category="Trace")
	EInteractionTraceMethod TraceMethod; 

	UPROPERTY(EditAnywhere, config, Category="Trace", meta=(EditCondition="bUseCameraTarget", UIMin="0.0", ClampMin="0.0"))
	float InteractionCameraDistance;

	UPROPERTY(EditAnywhere, config, Category="Trace", meta=(UIMin="0.0", ClampMin="0.0"))
	float InteractionDistance;

	UPROPERTY(EditAnywhere, config, Category="Debug")
	bool bShowTraces;

	EDrawDebugTrace::Type GetDrawDebugType() const { return bShowTraces ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None; }

public:
	static const FName DefaultInteractionInputActionName;
	static const EInteractionTraceMethod DefaultTraceMethod;
	static const float DefaultInteractionCameraDistance;
	static const float DefaultInteractionDistance;
	static const bool DefaultShowTraces;
};
