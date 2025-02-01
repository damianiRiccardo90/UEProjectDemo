// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UEProject : ModuleRules
{
	public UEProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{
            "AIModule",
            "Core",
			"CoreUObject",
			"Engine",
            "EnhancedInput",
            "InputCore",
            "GameplayAbilities",
            "GameplayCameras",
            "GameplayStateTreeModule",
            "GameplayTags",
            "GameplayTasks",
            "StateTreeModule"
        });
    }
}
