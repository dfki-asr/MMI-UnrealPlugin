// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Ulan Akmatbekov

#include "CommonTools.h"

using namespace std;
using namespace MMIStandard;

#pragma region Vector conversion methods

// If Unreal uses c++ optimizition (\O2, \Ox), the rvalues should prevent unnecessary copies
// even if the value created and modified in method, but it requires Release mode
// In Debug mode, it is better to return values directly from constructor call
// (ie. return FVector3(1.0f, 2.0f, 3.0f)), so the RVO would be applied even there

MVector3 ToMVec3(const FVector& fVec, double multiplier /* = 0.01 */)
{
	MVector3 res;
	res.__set_X(- multiplier * fVec.X);
	res.__set_Y(multiplier * fVec.Z);
	res.__set_Z(multiplier * fVec.Y);
	return res;
}

std::vector<double> ToDVec(const FVector& fVec, double multiplier /* = 0.01 */)
{
	return
	{
		-multiplier * fVec.X,
		multiplier * fVec.Z,
		multiplier * fVec.Y
	};
}

FVector ToFVec3(const MVector3& mVec, double multiplier /* = 100 */)
{
	return FVector(
		-multiplier * mVec.X,
		multiplier * mVec.Z,
		multiplier * mVec.Y);
}

FVector ToFVec3(const vector<double>& dVec, double multiplier /* = 100 */)
{
	if (dVec.size() != 3)
		throw runtime_error("Cannot use double vector: Input needs to have exactly 3 values");

	return FVector(
		-multiplier * dVec[0],
		multiplier * dVec[2],
		multiplier * dVec[1]);
}

MQuaternion ToMQuat(const FQuat& fquat)
{
	MQuaternion res;
	res.__set_X(-fquat.X);
	res.__set_Y(fquat.Z);
	res.__set_Z(fquat.Y);
	res.__set_W(fquat.W);
	return res;
}

FQuat ToFQuat(const MQuaternion& mquat)
{
	return FQuat(-mquat.X, mquat.Z, mquat.Y, mquat.W);
}

FVector ToFVec2(const std::vector<double>& dVec, int startIndex, double multiplier/* = 100*/)
{
	if (startIndex > dVec.size() - 2)
		throw runtime_error("Not enough elements left in array to construct a 2D vector");

	return FVector(
		-multiplier * dVec[startIndex],
		multiplier * dVec[startIndex + 1],
		0);
}

FVector ToFVec3(const std::vector<double>& dVec, int startIndex, double multiplier/* = 100*/)
{
	if (startIndex > dVec.size() - 3)
		throw runtime_error("Not enough elements left in array to construct a 3D vector");

	return FVector(
		-multiplier * dVec[startIndex],
		multiplier * dVec[startIndex + 2],
		multiplier * dVec[startIndex + 1]);
}

#pragma endregion
