// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// Helper class for executing hard coded behaviors (series of MInstructions) without applying Ajan
// Only for testing purposes, unrequired as soon as Ajan is working

#pragma once

#include <vector>
#include <map>

#include "Windows/AllowWindowsPlatformTypes.h"
#include "mmu_types.h"
#include "mmi_constants.h"
#include "InstructionWrapper.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "BlueprintTools.h"
#include "Containers/Map.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Vector.h"


#include "AvatarBehavior.generated.h"

class UActorComponent;
class UMMISceneObject;
class ASimulationController;

UCLASS( ClassGroup = ( MOSIM ), meta = ( BlueprintSpawnableComponent ) )
class MMISCENE_API UAvatarBehavior : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    UAvatarBehavior();

#pragma region Blueprint functions
	/*** Schedule an idle instruction
	* @param avatar - An avatar to run the instruction
	* @param previousInstruction - The reference to previous instruction. If not passed, this instruction will be chained to the last scheduled instruction.
	* @param delay - The delay between current and previous instructions in seconds
	* @param duration - The duration of idle instruction in seconds. Idle indefinitely if set to zero or less.
	* @return The instruction data. Can be chained to next instructions.*/
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Instruction")
	UInstructionWrapper* IdleInstruction(UMosimAvatar* avatar,
		 UInstructionWrapper* previousInstruction = nullptr, float duration = 0, float delay = 0.01f);
    

	UFUNCTION( BlueprintCallable )
    UPARAM( DisplayName = "Instruction" )
    UInstructionWrapper* GenericUpdateInstruction( UMosimAvatar* avatar,											
                                            FString updateID, FString Name, FString MotionType,
											TMap<FString, FString> parameters,
                                           UInstructionWrapper* previousInstruction = nullptr,		
                                            float duration = 0, float delay = 0.01f );
    UFUNCTION( BlueprintCallable )
	FString SerializeFVector( FVector v );

	/*** Execute all scheduled instructions for specific avatar */
	UFUNCTION(BlueprintCallable)
	void ExecuteInstructions(UMosimAvatar* avatar);

	/*** Execute all scheduled instructions for all avatars */
	//UFUNCTION(BlueprintCallable)
	//void ExecuteInstructionsForAll();

	/*** Clean instructions list for specific avatar */
	UFUNCTION(BlueprintCallable)
	void CleanInstructions(UMosimAvatar* avatar);

	/*** Clean instructions list for all avatars */
	//UFUNCTION(BlueprintCallable)
	//void CleanInstructionsForAll();
        

#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Logging")
	bool LogOnScreen;
	/** True if stacking messages on screen is necessary. Otherwise, will be replacing with new logs.*/
	UPROPERTY(EditAnywhere, Category = "Logging")
	bool StackLog;
	UPROPERTY(EditAnywhere, Category = "Logging")
	float LogDisplayTime;

	// Get the instruction list
	std::vector<MMIStandard::MInstruction>& GetInstructions();

	// storage for the list of instructions for each avatar
	// std::map<UMosimAvatar*, std::vector<MMIStandard::MInstruction>> InstructionMap;

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

	// Called every frame
    //virtual void TickComponent( float DeltaTime, ELevelTick TickType,
    //                            FActorComponentTickFunction* ThisTickFunction ) override;

	std::vector<MMIStandard::MInstruction> instructionList;

protected:

	enum Verbosity
	{
		Display, Error
	};

	// Log message on screen and console with Display verbosity
	void log(const char* messagePtr, Verbosity verbosity = Verbosity::Display) const;
	void log(const std::string& messageStr, Verbosity verbosity = Verbosity::Display) const;
	void log(const FString& message, Verbosity verbosity = Verbosity::Display) const;

	template <typename... Args>
        std::string string_format( const std::string& format, Args... args )
        {
            //// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
            int size_s =
                std::snprintf( nullptr, 0, format.c_str(), args... ) + 1;  // Extra space for '\0'
            if( size_s <= 0 )
            {
                throw std::runtime_error( "Error during formatting." );
            }
            auto size = static_cast<size_t>( size_s );
            auto buf = std::make_unique<char[]>( size );
            std::snprintf( buf.get(), size, format.c_str(), args... );
            return std::string( buf.get(),
                                buf.get() + size - 1 );  // We don't want the '\0' inside
        }

	/** Check the rule, log and stop execution if true*/
	bool checkFailure(bool failureRule, const std::string& logMessage);
	/** Check the pointer argument in function, log and stop execution if nullptr passed*/
	bool checkNullArgument(void* ptr, const std::string& argument, const std::string& function);
	/** Check the component from actor, log and stop execution if nullptr passed*/
	bool checkNullComponent(void* ptr, const std::string& actor, const std::string& function);

	/** Schedule new instruction after the end of previous instruction with delay*/
	void scheduleNewInstruction(UMosimAvatar* avatar, MMIStandard::MInstruction& newInstruction,
		UInstructionWrapper* previousInstruction, float duration, float delay);


    // simulation controller
    ASimulationController* SimContr;

	MMIStandard::mmiConstants mmiConstants;
};

/*template<typename ... Args> //Copypasted from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	auto buf = std::make_unique<char[]>(size);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}*/