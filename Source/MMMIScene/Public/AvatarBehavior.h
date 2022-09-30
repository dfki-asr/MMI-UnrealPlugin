// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// Helper class for executing hard coded behaviors (series of MInstructions) without applying Ajan
// Only for testing purposes, unrequired as soon as Ajan is working

#pragma once

#include <vector>
#include <map>

#include "Windows/AllowWindowsPlatformTypes.h"
#include "gen-cpp/mmu_types.h"
#include "gen-cpp/mmi_constants.h"
#include "InstructionWrapper.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "BlueprintTools.h"
#include "Containers/Map.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"


#include "AvatarBehavior.generated.h"

class AMMIAvatar;
class UMMISceneObject;
class ASimulationController;

UCLASS()
class MMISCENE_API AAvatarBehavior : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AAvatarBehavior();

#pragma region Blueprint functions
	/*** Schedule an idle instruction
	* @param avatar - An avatar to run the instruction
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param delay - The delay between current and previous instructions in seconds
	* @param duration - The duration of idle instruction in seconds. Idle indefinitely if set to zero or less.
	* @return The instruction data. Can be chained to next instructions.*/
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Instruction")
	UInstructionWrapper* IdleInstruction(AMMIAvatar* avatar,
		 UInstructionWrapper* previousInstruction = nullptr, float duration = 0, float delay = 0.01f);

	/*** Schedule a walk instruction
	* @param avatar - An avatar to run the instruction
	* @param walkTarget - The walk destination as a reference to an AActor with a MMISceneObject component
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param duration - The duration of instruction in seconds. Forces the end condition for instruction if set to value greater than zero.
	* @param delay - The delay between current and previous instructions in seconds
	* @param velocity - The walking speed of the character
	* @param reuseEnvironment - If set to false, rebuild the scene in path planner
	* @param areaCosts - The map of navigation area names with overriden costs.
	* If reuseEnvironment parameter is set to false, overriden values will reset to defaults.
	* @return The instruction data. Can be chained to next instructions.*/
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm="areaCosts"))
	UPARAM(DisplayName = "Instruction")
	UInstructionWrapper* WalkInstruction(AMMIAvatar* avatar, AActor* walkTarget,
		const TMap<TEnumAsByte<NavMeshArea>, float>& areaCosts,
		UInstructionWrapper* previousInstruction = nullptr,
		float duration = 0, float delay = 0.01f, float velocity = 1.0f,
		bool reuseEnvironment = true);

    UFUNCTION( BlueprintCallable )
	UPARAM( DisplayName = "Instruction" )
	UInstructionWrapper* ResponsiveWalkInstruction(
            AMMIAvatar* avatar, FVector walkDirection,
            UInstructionWrapper* previousInstruction = nullptr, float duration = 0,
            float delay = 0.01f, float velocity = 1.0f, bool reuseEnvironment = true,
            FString instrID = "" );

	/*** Schedule a reach instruction
	* @param avatar - An avatar to run the instruction
	* @param reachTarget - The object to reach as a reference to an AActor with a MMISceneObject component
	* @param hand - The hand to use to reach the object
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param duration - The duration of instruction in seconds. Forces the end condition for instruction if set to value greater than zero.
	* @param delay - The delay between current and previous instructions in seconds
	* @return The instruction data. Can be chained to next instructions.*/
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Instruction")
	UInstructionWrapper* ReachInstruction(AMMIAvatar* avatar, AActor* reachTarget, Hand hand,
		UInstructionWrapper* previousInstruction = nullptr, float duration = 0, float delay = 0.01f);

	//ToDo: perhaps there is no need in grasp instruction without the preceding reach instruction
	// May add reach instruction functionality inside grasp instruction

	/*** Schedule a grasp instruction
	* @param avatar - An avatar to run the instruction
	* @param graspTarget - The object to grasp as a reference to an AActor with a MMISceneObject component
	* @param hand - The hand to use to grasp the object
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param duration - The duration of instruction in seconds. Forces the end condition for instruction if set to value greater than zero.
	* @param delay - The delay between current and previous instructions in seconds
	* @return The instruction data. Can be chained to next instructions.*/
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Instruction")
	UInstructionWrapper* GraspInstruction(AMMIAvatar* avatar, AActor* graspTarget, Hand hand,
		UInstructionWrapper* previousInstruction = nullptr, float duration = 0, float delay = 0.01f);

	/*** Schedule a release instruction
	* @param avatar - An avatar to run the instruction
	* @param hand - The hand to free
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param duration - The duration of instruction in seconds. Forces the end condition for instruction if set to value greater than zero.
	* @param delay - The delay between current and previous instructions in seconds
	* @return The instruction data. Can be chained to next instructions.*/
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Instruction")
	UInstructionWrapper* ReleaseInstruction(AMMIAvatar* avatar, Hand hand, 
		UInstructionWrapper* previousInstruction = nullptr, float duration = 0, float delay = 0.01f);

	/*** Schedule a carry instruction
	* @param avatar - An avatar to run the instruction
	* @param carryTarget - The object to carry as a reference to an AActor with a MMISceneObject component
	* @param hand - The hand to use to carry a target object
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param duration - The duration of instruction in seconds. Forces the end condition for instruction if set to value greater than zero.
	* @param delay - The delay between current and previous instructions in seconds
	* @return The instruction data. Can be chained to next instructions.*/
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Instruction")
	UInstructionWrapper* CarryInstruction(AMMIAvatar* avatar, AActor* carryTarget, Hand hand,
		UInstructionWrapper* previousInstruction, float duration = 0, float delay = 0.01f);

	/*** Schedule a move instruction
	* @param avatar - An avatar to run the instruction
	* @param targetObject - The object to carry as a reference to an AActor with a MMISceneObject component
	* @param targetDestination - The final location as a reference to an AActor with a MMISceneObject component
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param duration - The duration of instruction in seconds. Forces the end condition for instruction if set to value greater than zero.
	* @param delay - The delay between current and previous instructions in seconds
	* @return The instruction data. Can be chained to next instructions.*/
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Instruction")
	UInstructionWrapper* MoveInstruction(AMMIAvatar* avatar, AActor* targetObject, AActor* targetDestination, Hand hand,
		UInstructionWrapper* previousInstruction = nullptr, float duration = 0, float delay = 0.01f);

	/*** Execute all scheduled instructions for specific avatar */
	UFUNCTION(BlueprintCallable)
	void ExecuteInstructions(AMMIAvatar* avatar);

	/*** Execute all scheduled instructions for all avatars */
	UFUNCTION(BlueprintCallable)
	void ExecuteInstructionsForAll();

	/*** Clean instructions list for specific avatar */
	UFUNCTION(BlueprintCallable)
	void CleanInstructions(AMMIAvatar* avatar);

	/*** Clean instructions list for all avatars */
	UFUNCTION(BlueprintCallable)
	void CleanInstructionsForAll();

#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Logging")
	bool LogOnScreen;
	/** True if stacking messages on screen is necessary. Otherwise, will be replacing with new logs.*/
	UPROPERTY(EditAnywhere, Category = "Logging")
	bool StackLog;
	UPROPERTY(EditAnywhere, Category = "Logging")
	float LogDisplayTime;

	// storage for the list of instructions for each avatar
	std::map<AMMIAvatar*, std::vector<MMIStandard::MInstruction>> InstructionMap;

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	enum Verbosity
	{
		Display, Error
	};

	// Log message on screen and console with Display verbosity
	void log(const char* messagePtr, Verbosity verbosity = Verbosity::Display) const;
	void log(const std::string& messageStr, Verbosity verbosity = Verbosity::Display) const;
	void log(const FString& message, Verbosity verbosity = Verbosity::Display) const;

	/** Check the rule, log and stop execution if true*/
	bool checkFailure(bool failureRule, const std::string& logMessage);
	/** Check the pointer argument in function, log and stop execution if nullptr passed*/
	bool checkNullArgument(void* ptr, const std::string& argument, const std::string& function);
	/** Check the component from actor, log and stop execution if nullptr passed*/
	bool checkNullComponent(void* ptr, const std::string& actor, const std::string& function);

	/** Schedule new instruction after the end of previous instruction with delay*/
	void scheduleNewInstruction(AMMIAvatar* avatar, MMIStandard::MInstruction& newInstruction,
		UInstructionWrapper* previousInstruction, float duration, float delay);


    // simulation controller
    ASimulationController* SimContr;

	MMIStandard::mmiConstants mmiConstants;
};
