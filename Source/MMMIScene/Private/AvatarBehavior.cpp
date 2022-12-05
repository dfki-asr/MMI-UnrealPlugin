// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// Helper class for executing hard coded behaviors (series of MInstructions) without applying Ajan
// Only for testing purposes, unrequired as soon as Ajan is working

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "AvatarBehavior.h"
#include "MOSIM.h"
#include "SimulationController.h"
#include "UnrealSceneAccess.h"
#include "MMISceneObject.h"
#include "MMIAvatar.h"
#include "mmi_constants.h"

#include <algorithm>


#include "MMUAccess.h"

#include "Engine.h"
#include <format>
#include "CommonTools.h"

//used for culture invariant float to string conversions.
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace MMIStandard;




// Sets default values
UAvatarBehavior::UAvatarBehavior()
	: LogOnScreen(true),
	  StackLog(true),
	  LogDisplayTime(5.0f),
	  SimContr( nullptr),
	  mmiConstants()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if
    // you don't need it.
    //PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UAvatarBehavior::BeginPlay()
{
	AActor* simActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASimulationController::StaticClass());
	SimContr = Cast<ASimulationController>(simActor);
}


UInstructionWrapper* UAvatarBehavior::IdleInstruction(AMMIAvatar* avatar,
	UInstructionWrapper* previousInstruction, float duration, float delay)
{
	// It is probably not possible to stop the execution before runtime errors. Can't think of more elegant solution.
	if (checkNullArgument(avatar, "Avatar", "Idle Instruction"))
		return nullptr;

	MInstruction idleInstruction;
	idleInstruction.__set_ID(MMUAccess::GetNewGuid());
	idleInstruction.__set_Name("Idle");
	idleInstruction.__set_MotionType("Pose/Idle");
    idleInstruction.__set_AvatarID( avatar->AvatarID );

	scheduleNewInstruction(avatar, idleInstruction, previousInstruction, duration, delay);

	if(duration > 0)
		log(string_format("Scheduled Idle Instruction for %s for %.2f seconds",
			avatar->baseName.c_str(), duration));
	else
		log("Scheduled Idle Instruction for " + avatar->baseName);

	return UInstructionWrapper::Create(idleInstruction);
}



void UAvatarBehavior::ExecuteInstructions(AMMIAvatar* avatar)
{
	if (checkNullArgument(avatar, "Avatar", "Execute Instructions"))
		return;

	SimContr->ExecuteInstructions(avatar);
	//log("Executed instructions for an avatar with name " + avatar->baseName);
}

void UAvatarBehavior::CleanInstructions(AMMIAvatar* avatar)
{
	if (checkNullArgument(avatar, "Avatar", "Clean Instructions"))
		return;

	instructionList.clear();
	log("Cleaned instructions for an avatar with name " + avatar->baseName);
}


bool UAvatarBehavior::checkFailure(bool failureRule, const std::string& logMessage)
{
	if (failureRule)
	{
		log(logMessage, Verbosity::Error);
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
	}
	return failureRule;
}

bool UAvatarBehavior::checkNullArgument(void* ptr, const std::string& argument, const std::string& function)
{
	return checkFailure(ptr == nullptr,
		string_format("Missing %s in %s",
			argument.c_str(), function.c_str()));
}

bool UAvatarBehavior::checkNullComponent(void* ptr, const std::string& actor, const std::string& function)
{
	return checkFailure(ptr == nullptr,
		string_format("Missing a UMMISceneObject component in %s in function %s",
			actor.c_str(), function.c_str()));
}


void UAvatarBehavior::log(const char* messagePtr, Verbosity verbosity) const
{
	log(FString(messagePtr), verbosity);
}

void UAvatarBehavior::log(const string& messageStr, Verbosity verbosity) const
{
	log(UTF8_TO_TCHAR(messageStr.c_str()), verbosity);
}

void UAvatarBehavior::log(const FString& message, Verbosity verbosity) const
{
	FColor color;
	switch (verbosity)
	{
	case Verbosity::Display:
		color = FColor::Green;
		UE_LOG(LogMOSIM, Display, TEXT("%s"), *message);
		break;
	case Verbosity::Error:
		color = FColor::Red;
		UE_LOG(LogMOSIM, Error, TEXT("%s"), *message);
		break;
	}
	if (LogOnScreen)
	{
		GEngine->AddOnScreenDebugMessage(
			StackLog ? INDEX_NONE : static_cast<uint64>(GetUniqueID()),
			LogDisplayTime, color, message);
	}
}

void UAvatarBehavior::scheduleNewInstruction(AMMIAvatar* avatar, MInstruction& newInstruction,
	UInstructionWrapper* previousInstruction, float duration, float delay)
{
    auto& instructions = instructionList;
	
	string prevId;
	if(previousInstruction != nullptr)
		prevId = previousInstruction->Instruction.ID;
	else if (!instructions.empty())
		prevId = instructions.back().ID;

	if (!prevId.empty())
	{
		string startCondition = delay > 0
			? string_format("%s:%s + %.4f",
				prevId.c_str(), mmiConstants.MSimulationEvent_End.c_str(), delay)
			: string_format("%s:%s",
				prevId.c_str(), mmiConstants.MSimulationEvent_End.c_str());
		newInstruction.__set_StartCondition(startCondition);
	}

	if (duration > 0)
	{
		newInstruction.__set_EndCondition(string_format("%s:%s + %.4f",
			newInstruction.ID.c_str(), mmiConstants.MSimulationEvent_Start.c_str(), duration)); // or maybe I should put prevId.ID + (delay + duration)
	}

	bool update = false;
	for( int i = 0; i < instructions.size(); i++ )
    {
        if( instructions[i].ID == newInstruction.ID )
        {
            update = true;
            instructions[i] = newInstruction;
		}
	}
    if( !update )
    {
        // insert new instruction for the chosen avatar in the end of a queue
        instructions.push_back( newInstruction );
    }
}

std::vector<MMIStandard::MInstruction>& UAvatarBehavior::GetInstructions()
{
    return this->instructionList;
}
