// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "AnimNode_ManualComponentPose.h"
#include "AnimGraphNode_ManualComponentPose.generated.h"

/**
 * 
 */
UCLASS()
class MMISCENEUNCOOKED_API UAnimGraphNode_ManualComponentPose : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()

public:

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface

protected:

	virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }

private:

	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_ManualComponentPose Node;
};
