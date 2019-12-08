#include "InteractiveItems.h"
#include "InteractionRuntimeSettings.h"

#include "ISettingsModule.h"
#include "ISettingsSection.h"

#define LOCTEXT_NAMESPACE "FInteractiveItemsModule"

void FInteractiveItemsModule::StartupModule()
{
	RegisterSettings();
	OnSettingsChanged();
}

void FInteractiveItemsModule::ShutdownModule()
{
	UnregisterSettings();
}

void FInteractiveItemsModule::RegisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "InteractiveItems",
			LOCTEXT("InteractiveItemsSettingsName", "Interactive Items"),
			LOCTEXT("InteractiveItemsSettingsDescription", "Configure the Interactive Items plugin"),
			GetMutableDefault<UInteractionRuntimeSettings>()
		);

		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FInteractiveItemsModule::OnSettingsChanged);
		}
	}
#endif
}

void FInteractiveItemsModule::UnregisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "InteractiveItems");
	}
#endif
}

bool FInteractiveItemsModule::OnSettingsChanged()
{
	return true;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInteractiveItemsModule, InteractiveItems)
