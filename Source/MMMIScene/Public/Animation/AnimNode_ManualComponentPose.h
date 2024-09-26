// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <JointPose.h>
#include <BoneControllers/AnimNode_SkeletalControlBase.h>

#include "AnimNode_ManualComponentPose.generated.h"

/**
 * The custom animation node for setting the avatar pose manually in the component space
 */
USTRUCT(BlueprintInternalUseOnly)
struct MMISCENE_API FAnimNode_ManualComponentPose : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (PinShownByDefault))
	TArray<FJointPose> JointPoses;

public:
	FAnimNode_ManualComponentPose() {}

   // FAnimNode_SkeletalControlBase interface
	virtual bool IsValidToEvaluate(const USkeleton* skeleton, const FBoneContainer& requiredBones) override;
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& output,
		TArray<FBoneTransform>& outBoneTransforms) override;
	// End of FAnimNode_SkeletalControlBase interface
};
