// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DoubleHeroes : ModuleRules
{
	public DoubleHeroes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemSteam", "GameplayAbilities", "UMG", "ModularGameplay" });

		PrivateDependencyModuleNames.AddRange(new string[] {"GameplayTags", "GameplayTasks", "NavigationSystem", "Niagara", "NetCore", "AIModule", "SlateCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		
		//SetupIrisSupport(Target);
	}
}
