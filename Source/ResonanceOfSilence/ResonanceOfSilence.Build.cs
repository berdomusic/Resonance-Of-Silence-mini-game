// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ResonanceOfSilence : ModuleRules
{
	public ResonanceOfSilence(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara" });
        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags" });

        PrivatePCHHeaderFile = "RoSPCH.h";
    }
}
