// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// Helper class for executing hard coded behaviors (series of MInstructions) without applying Ajan
// Only for testing purposes, unrequired as soon as Ajan is working

#include "AvatarBehavior.h"
#include "MOSIM.h"
#include "SimulationController.h"
#include "UnrealSceneAccess.h"
#include "MMISceneObject.h"
#include "MMIAvatar.h"
#include "gen-cpp/mmi_constants.h"

#include <algorithm>

#include "MMUAccess.h"

#include "Engine.h"
//used for culture invariant float to string conversions.
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace MMIStandard;

template<typename ... Args> //Copypasted from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	auto buf = std::make_unique<char[]>(size);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}


// Sets default values
AAvatarBehavior::AAvatarBehavior()
	: LogOnScreen(true),
	  StackLog(true),
	  LogDisplayTime(5.0f),
	  SimContr( nullptr),
	  mmiConstants()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if
    // you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAvatarBehavior::BeginPlay()
{
    Super::BeginPlay();

	AActor* simActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASimulationController::StaticClass());
	SimContr = Cast<ASimulationController>(simActor);
}

// Called every frame
void AAvatarBehavior::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

UInstructionWrapper* AAvatarBehavior::IdleInstruction(AMMIAvatar* avatar,
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

UInstructionWrapper* AAvatarBehavior::WalkInstruction(
	AMMIAvatar* avatar, AActor* walkTarget,
	const TMap<TEnumAsByte<NavMeshArea>, float>& areaCosts,
	UInstructionWrapper* previousInstruction /*= nullptr*/,
	float duration /*= 0*/, float delay /*= 0.01f*/, float velocity /*= 1.0f*/,
	bool reuseEnvironment /*= true*/)
{
	if (checkNullArgument(avatar, "Avatar", "Walk Instruction") ||
		checkNullArgument(walkTarget, "Walk Target", "Walk Instruction"))
		return nullptr;

	UMMISceneObject* walkSceneObject = Cast<UMMISceneObject>(
		walkTarget->GetComponentByClass(UMMISceneObject::StaticClass()));

	if (checkNullComponent(walkSceneObject,
		TCHAR_TO_UTF8(*walkTarget->GetName()),
		"Walk Instruction"))
		return nullptr;

	MInstruction walkInstruction;
	walkInstruction.__set_ID(MMUAccess::GetNewGuid());
	walkInstruction.__set_MotionType("Locomotion/Walk");
	walkInstruction.__set_Name("Walking to target location");
    walkInstruction.__set_AvatarID(avatar->AvatarID);

	map<string, string> walkProperties =
	{
		{"TargetID", walkSceneObject->MSceneObject.ID},
		{"UseTargetOrientation", "false"},
        { "Velocity", boost::lexical_cast<std::string>( velocity ) },
		//Default to_string convert bool to 0 or 1
		{"ReuseEnvironment", reuseEnvironment ? "true" : "false"},
		{"PathPlanningID", SimContr->CurrentSceneID},
		{"OffsetY", "0.03"} // Hack, remove once model root is fixed
	};
	if (areaCosts.Num() > 0)
	{
		walkProperties["AreaCosts"] = TCHAR_TO_UTF8(
			*FString::JoinBy(areaCosts, TEXT(","),
				[](auto& areaCost)
			{
				FString areaNameStr = UEnum::GetValueAsString<NavMeshArea>(areaCost.Key.GetValue());
				return areaNameStr + ":" + FString::SanitizeFloat(areaCost.Value);
			}));
	}
	walkInstruction.__set_Properties(walkProperties);

	scheduleNewInstruction(avatar, walkInstruction, previousInstruction, duration, delay);

	if (duration > 0)
		log(string_format("Scheduled Walk Instruction for %s to target object %s for %.2f seconds",
			avatar->baseName.c_str(), walkSceneObject->MSceneObject.Name.c_str(), duration));
	else
		log(string_format("Scheduled Walk Instruction for %s to target object %s",
			avatar->baseName.c_str(), walkSceneObject->MSceneObject.Name.c_str()));


	return UInstructionWrapper::Create(walkInstruction);
}

UInstructionWrapper* AAvatarBehavior::ReachInstruction(AMMIAvatar* avatar, AActor* reachTarget, Hand hand,
	UInstructionWrapper* previousInstruction, float duration, float delay)
{
	if (checkNullArgument(avatar, "Avatar", "Reach Instruction") ||
		checkNullArgument(reachTarget, "Reach Target", "Reach Instruction"))
		return nullptr;

	UMMISceneObject* reachSceneObject = Cast<UMMISceneObject>(
		reachTarget->GetComponentByClass(UMMISceneObject::StaticClass()));

	if (checkNullComponent(reachSceneObject,
		TCHAR_TO_UTF8(*reachTarget->GetName()),
		"Reach Instruction"))
		return nullptr;

	MInstruction reachInstruction;
	reachInstruction.__set_ID(MMUAccess::GetNewGuid());
	reachInstruction.__set_MotionType("Pose/Reach");
	reachInstruction.__set_Name("Reaching target object.");
    reachInstruction.__set_AvatarID(avatar->AvatarID);

	string handStr = TCHAR_TO_UTF8(*UEnum::GetValueAsString<Hand>(hand));

	map<string, string> reachProperties =
	{
		{"TargetID", reachSceneObject->MSceneObject.ID},
		{"Hand", handStr}
	};
	reachInstruction.__set_Properties(reachProperties);

	scheduleNewInstruction(avatar, reachInstruction, previousInstruction, duration, delay);

	if (duration > 0)
		log(string_format("Scheduled Reach Instruction for %s with target object %s and %s hand for %.2f seconds",
			avatar->baseName.c_str(), reachSceneObject->MSceneObject.Name.c_str(), handStr.c_str(), duration));
	else
		log(string_format("Scheduled Reach Instruction for %s with target object %s and %s hand",
			avatar->baseName.c_str(), reachSceneObject->MSceneObject.Name.c_str(), handStr.c_str()));

	return UInstructionWrapper::Create(reachInstruction);
}

UInstructionWrapper* AAvatarBehavior::GraspInstruction(AMMIAvatar* avatar, AActor* graspTarget, Hand hand,
	UInstructionWrapper* previousInstruction, float duration, float delay)
{
	if (checkNullArgument(avatar, "Avatar", "Grasp Instruction") ||
		checkNullArgument(graspTarget, "Grasp Target", "Grasp Instruction"))
		return nullptr;

	UMMISceneObject* graspSceneObject = Cast<UMMISceneObject>(
		graspTarget->GetComponentByClass(UMMISceneObject::StaticClass()));

	if (checkNullComponent(graspSceneObject,
		TCHAR_TO_UTF8(*graspTarget->GetName()),
		"Grasp Instruction"))
		return nullptr;

	MInstruction graspInstruction;
	graspInstruction.__set_ID(MMUAccess::GetNewGuid());
	graspInstruction.__set_MotionType("Pose/Grasp");
	graspInstruction.__set_Name("Grasping target object.");
    graspInstruction.__set_AvatarID(avatar->AvatarID);

	string handStr = TCHAR_TO_UTF8(*UEnum::GetValueAsString<Hand>(hand));

	map<string, string> graspProperties =
	{
		{"TargetID", graspSceneObject->MSceneObject.ID},
		{"Hand", handStr}
	};
	graspInstruction.__set_Properties(graspProperties);

	scheduleNewInstruction(avatar, graspInstruction, previousInstruction, duration, delay);

	if (duration > 0)
		log(string_format("Scheduled Grasp Instruction for %s with target object %s and %s hand for %.2f seconds",
			avatar->baseName.c_str(), graspSceneObject->MSceneObject.Name.c_str(), handStr.c_str(), duration));
	else
		log(string_format("Scheduled Grasp Instruction for %s with target object %s and %s hand",
			avatar->baseName.c_str(), graspSceneObject->MSceneObject.Name.c_str(), handStr.c_str()));

	return UInstructionWrapper::Create(graspInstruction);
}

UInstructionWrapper* AAvatarBehavior::ReleaseInstruction(AMMIAvatar* avatar, Hand hand,
	UInstructionWrapper* previousInstruction, float duration, float delay)
{
	if (checkNullArgument(avatar, "Avatar", "Release Instruction"))
		return nullptr;

	MInstruction releaseInstruction;
	releaseInstruction.__set_ID(MMUAccess::GetNewGuid());
	releaseInstruction.__set_MotionType("Object/Release");
	releaseInstruction.__set_Name("Releasing object.");
    releaseInstruction.__set_AvatarID( avatar->AvatarID );

	string handStr = TCHAR_TO_UTF8(*UEnum::GetValueAsString<Hand>(hand));
	map<string, string> reachProperties = { {"Hand", handStr} };
	releaseInstruction.__set_Properties(reachProperties);

	scheduleNewInstruction(avatar, releaseInstruction, previousInstruction, duration, delay);

	if (duration > 0)
		log(string_format("Scheduled Release Instruction for %s with %s hand for %.2f seconds",
			avatar->baseName.c_str(), handStr.c_str(), duration));
	else
		log(string_format("Scheduled Release Instruction for %s with %s hand",
			avatar->baseName.c_str(), handStr.c_str()));

	return UInstructionWrapper::Create(releaseInstruction);
}

UInstructionWrapper* AAvatarBehavior::CarryInstruction(AMMIAvatar* avatar, AActor* carryTarget, Hand hand,
	UInstructionWrapper* previousInstruction, float duration, float delay)
{
	if (checkNullArgument(avatar, "Avatar", "Carry Instruction") ||
		checkNullArgument(carryTarget, "Carry Target", "Carry Instruction"))
		return nullptr;

	UMMISceneObject* carrySceneObject = Cast<UMMISceneObject>(
		carryTarget->GetComponentByClass(UMMISceneObject::StaticClass()));

	if (checkNullComponent(carrySceneObject,
		TCHAR_TO_UTF8(*carryTarget->GetName()),
		"Carry Instruction"))
		return nullptr;

	MInstruction carryInstruction;
	carryInstruction.__set_ID(MMUAccess::GetNewGuid());
	carryInstruction.__set_MotionType("Object/Carry");
	carryInstruction.__set_Name("Carrying target object");
    carryInstruction.__set_AvatarID( avatar->AvatarID );

	string handStr = TCHAR_TO_UTF8(*UEnum::GetValueAsString<Hand>(hand));
	map<string, string> carryProperties =
	{
		{"TargetID", carrySceneObject->MSceneObject.ID},
		{"Hand", handStr}
	};
	carryInstruction.__set_Properties(carryProperties);

	scheduleNewInstruction(avatar, carryInstruction, previousInstruction, duration, delay);

	if (duration > 0)
		log(string_format("Scheduled Carry Instruction for %s with target object %s and %s hand for %.2f seconds",
			avatar->baseName.c_str(), carrySceneObject->MSceneObject.Name.c_str(), handStr.c_str(), duration));
	else
		log(string_format("Scheduled Carry Instruction for %s with target object %s and %s hand",
			avatar->baseName.c_str(), carrySceneObject->MSceneObject.Name.c_str(), handStr.c_str()));

	return UInstructionWrapper::Create(carryInstruction);
}

UInstructionWrapper* AAvatarBehavior::MoveInstruction(AMMIAvatar* avatar, AActor* targetObject,
	AActor* targetDestination, Hand hand,
	UInstructionWrapper* previousInstruction, float duration, float delay)
{
	if (checkNullArgument(avatar, "Avatar", "Move Instruction") ||
		checkNullArgument(targetObject, "Target Object", "Move Instruction") ||
		checkNullArgument(targetDestination, "Target Destination", "Move Instruction"))
		return nullptr;

	UMMISceneObject* carrySceneObject = Cast<UMMISceneObject>(
		targetObject->GetComponentByClass(UMMISceneObject::StaticClass()));
	UMMISceneObject* destinationSceneObject = Cast<UMMISceneObject>(
		targetDestination->GetComponentByClass(UMMISceneObject::StaticClass()));

	if (checkNullComponent(carrySceneObject,
			TCHAR_TO_UTF8(*targetObject->GetName()),
			"Move Instruction") ||
		checkNullComponent(destinationSceneObject,
			TCHAR_TO_UTF8(*targetDestination->GetName()),
			"Move Instruction"))
		return nullptr;

	MInstruction moveInstruction;
	moveInstruction.__set_ID(MMUAccess::GetNewGuid());
	moveInstruction.__set_MotionType("Object/Move");
	moveInstruction.__set_Name("Moving object");
    moveInstruction.__set_AvatarID(avatar->AvatarID);

	string handStr = TCHAR_TO_UTF8(*UEnum::GetValueAsString<Hand>(hand));
	map<string, string> moveProperties =
	{
		{"TargetID", destinationSceneObject->MSceneObject.ID},
		{"SubjectID", carrySceneObject->MSceneObject.ID},
		{"Hand", handStr}
	};
	moveInstruction.__set_Properties(moveProperties);

	scheduleNewInstruction(avatar, moveInstruction, previousInstruction, duration, delay);

	if (duration > 0)
		log(string_format(
			"Scheduled Move Instruction for %s to target object %s with target destination %s and %s hand for %.2f seconds",
			avatar->baseName.c_str(), carrySceneObject->MSceneObject.Name.c_str(),
			destinationSceneObject->MSceneObject.Name.c_str(), handStr.c_str(), duration));
	else
		log(string_format(
			"Scheduled Move Instruction for %s to target object %s with target destination %s and %s hand",
			avatar->baseName.c_str(), carrySceneObject->MSceneObject.Name.c_str(),
			destinationSceneObject->MSceneObject.Name.c_str(), handStr.c_str()));

	return UInstructionWrapper::Create(moveInstruction);
}


void AAvatarBehavior::ExecuteInstructions(AMMIAvatar* avatar)
{
	if (checkNullArgument(avatar, "Avatar", "Execute Instructions"))
		return;

	SimContr->ExecuteInstructions(avatar);
	log("Executed instructions for an avatar with name " + avatar->baseName);
}

void AAvatarBehavior::ExecuteInstructionsForAll()
{
	SimContr->ExecuteInstructionsForAll();
	log("Executed instructions for all avatars");
}

void AAvatarBehavior::CleanInstructions(AMMIAvatar* avatar)
{
	if (checkNullArgument(avatar, "Avatar", "Clean Instructions"))
		return;

	InstructionMap[avatar].clear();
	log("Cleaned instructions for an avatar with name " + avatar->baseName);
}

void AAvatarBehavior::CleanInstructionsForAll()
{
	InstructionMap.clear();
	log("Cleaned instructions for all avatars");
}


bool AAvatarBehavior::checkFailure(bool failureRule, const std::string& logMessage)
{
	if (failureRule)
	{
		log(logMessage, Verbosity::Error);
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
	}
	return failureRule;
}

bool AAvatarBehavior::checkNullArgument(void* ptr, const std::string& argument, const std::string& function)
{
	return checkFailure(ptr == nullptr,
		string_format("Missing %s in %s",
			argument.c_str(), function.c_str()));
}

bool AAvatarBehavior::checkNullComponent(void* ptr, const std::string& actor, const std::string& function)
{
	return checkFailure(ptr == nullptr,
		string_format("Missing a UMMISceneObject component in %s in function %s",
			actor.c_str(), function.c_str()));
}


void AAvatarBehavior::log(const char* messagePtr, Verbosity verbosity) const
{
	log(FString(messagePtr), verbosity);
}

void AAvatarBehavior::log(const string& messageStr, Verbosity verbosity) const
{
	log(UTF8_TO_TCHAR(messageStr.c_str()), verbosity);
}

void AAvatarBehavior::log(const FString& message, Verbosity verbosity) const
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

void AAvatarBehavior::scheduleNewInstruction(AMMIAvatar* avatar, MInstruction& newInstruction,
	UInstructionWrapper* previousInstruction, float duration, float delay)
{
	auto& instructions = InstructionMap[avatar];
	
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

	// insert new instruction for the chosen avatar in the end of a queue
	instructions.push_back(newInstruction);
}