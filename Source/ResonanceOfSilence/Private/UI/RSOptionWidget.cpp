#include "UI/RSOptionWidget.h"

void URSOptionWidget::ApplySettings_Implementation()
{
	UpdateInternals();
}

void URSOptionWidget::UpdateInternals()
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
	UpdateInternals();
}
