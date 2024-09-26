// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimGraphNode_ManualComponentPose.h"

#define LOCTEXT_NAMESPACE "A3Nodes"

FText UAnimGraphNode_ManualComponentPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("NodeTitle", "Absolute Manual Pose");
}

FText UAnimGraphNode_ManualComponentPose::GetTooltipText() const
{
	return LOCTEXT("NodeTooltip", "Generates the avatar pose from supplied joints' positions and rotations in component space");
}

#undef LOCTEXT_NAMESPACE