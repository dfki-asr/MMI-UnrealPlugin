// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNode_ManualComponentPose.h"

bool FAnimNode_ManualComponentPose::IsValidToEvaluate(const USkeleton* skeleton, const FBoneContainer& requiredBones)
{
    return JointPoses.Num() != 0;
}

void FAnimNode_ManualComponentPose::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& output, TArray<FBoneTransform>& outBoneTransforms)
{
    DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateSkeletalControl_AnyThread) // to debug performance

    check(outBoneTransforms.Num() == 0);

    const FBoneContainer& boneContainer = output.Pose.GetPose().GetBoneContainer();

    for (auto& jointPose : JointPoses)
    {
        const FCompactPoseBoneIndex boneCPB = (FCompactPoseBoneIndex)boneContainer
            .GetPoseBoneIndexForBoneName(jointPose.JointName);
        
        if (boneCPB == INDEX_NONE) // ignore LODed bones
            continue;

        FTransform boneTransform = output.Pose.GetComponentSpaceTransform(boneCPB);

        boneTransform.SetLocation(jointPose.JointPosition);
        boneTransform.SetRotation(jointPose.JointRotation);

        outBoneTransforms.Add(FBoneTransform(boneCPB, boneTransform));
    }

    outBoneTransforms.Sort(FCompareBoneTransformIndex());
}