// Fill out your copyright notice in the Description page of Project Settings.

// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Ulan Akmatbekov

// This file is used for common functional in scope of Unreal Editor
// Anything related with visual representation of structures in editor, inspector, bluepring nodes, etc. can be stored here
// Do not mix up with CommonTools.h, which is used for common functional in scope of c++ code

//If necessary, BlueprintTools.cpp file can be added to project later

#pragma once

#include "CoreMinimal.h"
#include "BlueprintTools.generated.h"

UENUM(BlueprintType)
enum Hand
{
	Right,
	Left
};

UENUM(BlueprintType)
enum NavMeshArea
{
	None,
	Walkable,
	NotWalkable,
	Jump,
	Crosswalk,
	Road,
	Sidewalk
};

UENUM()
enum SceneObjectType
{
	SceneObject = 1,
	InitialLocation = 2,
	FinalLocation = 3,
	WalkTarget = 4,
	Area = 5,
	Part = 6,
	Tool = 7,
	Group = 8
};