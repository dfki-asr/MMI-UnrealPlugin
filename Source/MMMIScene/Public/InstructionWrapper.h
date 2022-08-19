// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "gen-cpp/mmu_types.h"
#include "InstructionWrapper.generated.h"

UCLASS(BlueprintType)
class MMISCENE_API UInstructionWrapper : public UObject
{
public:
	GENERATED_BODY()

	UInstructionWrapper()
		: Instruction()
	{

	}

	static UInstructionWrapper* Create(const MMIStandard::MInstruction& instruction)
	{
		UInstructionWrapper* result = NewObject<UInstructionWrapper>();
		result->Instruction = instruction;
		return result;
	}

	MMIStandard::MInstruction Instruction;
};
