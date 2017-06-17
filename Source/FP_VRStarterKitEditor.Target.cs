// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class FP_VRStarterKitEditorTarget : TargetRules
{
	public FP_VRStarterKitEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "FP_VRStarterKit" } );
	}
}
