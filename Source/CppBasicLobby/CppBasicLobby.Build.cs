// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CppBasicLobby : ModuleRules
{
	public CppBasicLobby(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","OnlineSubsystem","OnlineSubsystemUtils" });
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
