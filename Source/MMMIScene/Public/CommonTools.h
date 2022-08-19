// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// This file is used for common functional in scope of Unreal Editor
// Anything related with visual representation of structures in editor can be stored here
// Do not mix up with BlueprintTools.h, which is used for common functional in scope of Unreal Editor (visual changes)

#pragma once

#include "gen-cpp/scene_types.h"
#include "Math/Vector.h"
#include "Math/Quat.h"
#include <vector>

#pragma region Vector conversion methods

// This is the standard transfer between UE4 -> MOSIM coordinate system.
// UE4: (Left, Forward, Up), MOSIM: (Right, Up, Forward), both Left-Handed Coordinate Systems.

//Convert Unreal vector to MOSIM vector
//By default, multiplier is set to 0.01 (cm -> m, cm/s^2 -> m/s^2) for coordinates, lengths, forces,
//but can take other values for other units conversion.
//Should be 1 for dimensionless units i.e. scale or angular velocity.
MMIStandard::MVector3 ToMVec3(const FVector& fVec, double multiplier = 0.01);

//Convert Unreal vector to MOSIM std::vector<double>
//By default, multiplier is set to 0.01 (cm -> m, cm/s^2 -> m/s^2) for coordinates, lengths, forces,
//but can take other values for other units conversion.
//Should be 1 for dimensionless units i.e. scale or angular velocity.
std::vector<double> ToDVec(const FVector& fVec, double multiplier = 0.01);

//Convert MOSIM vector to Unreal vector
//By default, multiplier is set to 100 (m -> cm, m/s^2 -> cm/s^2) for coordinates, lengths, forces,
//but can take other values for other units conversion.
//Should be 1 for dimensionless units i.e. scale or angular velocity.
FVector ToFVec3(const MMIStandard::MVector3& mVec, double multiplier = 100);

//Convert MOSIM vector to Unreal vector
//By default, multiplier is set to 100 (m -> cm, m/s^2 -> cm/s^2) for coordinates, lengths, forces,
//but can take other values for other units conversion.
//Should be 1 for dimensionless units i.e. scale or angular velocity.
FVector ToFVec3(const std::vector<double>& dVec, double multiplier = 100);

MMIStandard::MQuaternion ToMQuat(const FQuat& fquat);
FQuat ToFQuat(const MMIStandard::MQuaternion& mquat);



//Convert vector elements to Unreal 2D vector (XY plane) starting from specified index
//By default, multiplier is set to 100 (m -> cm, m/s^2 -> cm/s^2) for coordinates, lengths, forces,
//but can take other values for other units conversion.
//Should be 1 for dimensionless units i.e. scale or angular velocity.
FVector ToFVec2(const std::vector<double>& dVec, int startIndex, double multiplier = 100);

//Convert vector elements to Unreal 3D vector starting from specified index
//By default, multiplier is set to 100 (m -> cm, m/s^2 -> cm/s^2) for coordinates, lengths, forces,
//but can take other values for other units conversion.
//Should be 1 for dimensionless units i.e. scale or angular velocity.
FVector ToFVec3(const std::vector<double>& dVec, int startIndex, double multiplier = 100);

#pragma endregion