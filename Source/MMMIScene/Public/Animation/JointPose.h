// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "JointPose.generated.h"

/**
 * The location and rotation of a joint
 */
USTRUCT(BlueprintType)
struct MMISCENE_API FJointPose
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName JointName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector JointPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuat JointRotation;

public:
	FJointPose()
	{
	}

	FJointPose(const FName& jointName, const FVector& jointPosition, const FQuat& jointRotation)
		: JointName(jointName),
		JointPosition(jointPosition),
		JointRotation(jointRotation)
	{
	}
};
