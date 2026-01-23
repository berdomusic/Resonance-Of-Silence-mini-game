#include "UI/RSOptionWidget.h"

void URSOptionWidget::ApplyInternalSettings_Implementation()
{
	UpdateInternalVariables();
}

void URSOptionWidget::UpdateInternalVariables()
{
	InternalTemp_GameplayUserSettings = URSGameUserSettingsFunctionLibrary::GetGameplayUserSettings();
	InternalTemp_ControlsUserSettings = URSGameUserSettingsFunctionLibrary::GetControlsUserSettings();
	InternalTemp_PerformanceUserSettings = URSGameUserSettingsFunctionLibrary::GetPerformanceUserSettings();
	InternalTemp_GraphicsUserSettings = URSGameUserSettingsFunctionLibrary::GetGraphicsUserSettings();
	InternalTemp_SoundUserSettings = URSGameUserSettingsFunctionLibrary::GetSoundUserSettings();
}

void URSOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateInternalVariables();
}
