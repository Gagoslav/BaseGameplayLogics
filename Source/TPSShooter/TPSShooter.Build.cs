// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class TPSShooter : ModuleRules
{
	public TPSShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });


		// Make UBT to look for appropriate headers in Module's directory to avoid full relative paths' indication 
		PrivateIncludePaths.AddRange(new string[] { Name/*"TPSShooter"*/ });
	}
}
