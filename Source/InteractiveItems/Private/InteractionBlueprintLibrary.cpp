#include "InteractionBlueprintLibrary.h"
#include "InteractionRuntimeSettings.h"

#include "GameFramework/InputSettings.h"

bool UInteractionBlueprintLibrary::GetBindingForInteractionAction(FKey& Binding)
{
	const UInteractionRuntimeSettings* runtimeSettings = GetDefault<UInteractionRuntimeSettings>();
	if (runtimeSettings)
	{
		const FName& interactionActionName = runtimeSettings->InteractionInputActionName;
		const UInputSettings* inputSettings = GetDefault<UInputSettings>();
		TArray<FInputActionKeyMapping> mappings;
		inputSettings->GetActionMappingByName(interactionActionName, mappings);
		if (mappings.Num() > 0)
		{
			Binding = mappings[0].Key;
			return true;
		}
	}	
	return false;
}
