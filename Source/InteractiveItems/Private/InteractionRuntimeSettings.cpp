#include "InteractionRuntimeSettings.h"

const FName UInteractionRuntimeSettings::DefaultInteractionInputActionName = FName("Interact");
const EInteractionTraceMethod UInteractionRuntimeSettings::DefaultTraceMethod = EInteractionTraceMethod::CameraAndPawn;
const float UInteractionRuntimeSettings::DefaultInteractionCameraDistance = 800;
const float UInteractionRuntimeSettings::DefaultInteractionDistance = 200;
const bool UInteractionRuntimeSettings::DefaultShowTraces = false;

UInteractionRuntimeSettings::UInteractionRuntimeSettings()
	: InteractionInputActionName(DefaultInteractionInputActionName)
	, TraceMethod(DefaultTraceMethod)
	, InteractionCameraDistance(DefaultInteractionCameraDistance)
	, InteractionDistance(DefaultInteractionDistance)
	, bShowTraces(DefaultShowTraces)
{
}
