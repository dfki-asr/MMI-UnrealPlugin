// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// Central instance for exectuing the simulations. Collects all MAvatars and MScene Objects
// and passes them to the UnrealSceneAccess. Initializes all MAvatars and MSceneObjects during
// BeginPlay() and executes the Simulation in MOSIM during Tick()

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include <string>
#include <map>
#include <vector>
#include <concurrent_unordered_map.h>

#include "Windows\AllowWindowsPlatformTypes.h"
#include "mmu_types.h"
#include "Windows\HideWindowsPlatformTypes.h"
#include "MOSIM.h"
#include "MMISceneObject.h"
#include "MMIAvatar.h"

#include "SimulationController.generated.h"

using namespace std;
using namespace MMIStandard;
using namespace Concurrency;

// forward declaration
class AMMIAvatar;
class UnrealSceneAccess;
class MMISettings;
class UAvatarBehavior;

UCLASS( HideCategories = ( Input, Actor, LOD ) )
class MMISCENE_API ASimulationController : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASimulationController();
    // destructor
    ~ASimulationController();

    UPROPERTY( EditAnywhere, Category = "MOSIM" )
    // Specifies whether the co simulator works in realtime
    bool RealTime;
    UPROPERTY( EditAnywhere, Category = "MOSIM" )
    // Specifies the fixed frame time (if non realtime mode)
    float FixedStepTime;
    // The amount of physics updates within each frame
    int PhysicsUpdatesPerFrame;

    UPROPERTY( EditAnywhere, Category = "MOSIM" )
    int RediscoverRate = 10;


	// The global ID for the current environment
	string CurrentSceneID;
    // All assigned avatars
    vector<AMMIAvatar*> Avatars;

    vector<UMMISceneObject*> sceneObjects;
	// The unique session id for each avatar
	concurrent_unordered_map<AMMIAvatar*, string> AvatarSessionIDs;

    // start the simulation controller automatically
    bool AutoStart;

    // Instance of the UnrealSceneAccess
    UnrealSceneAccess* UESceneAccess;

    // Access to the Instructions, in case ajan is not coupled
    UAvatarBehavior* Behavior;

	UPROPERTY(EditAnywhere, Category = "Debugging")
	bool DisplayDrawcalls;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	FColor LineColor;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	FColor PointColor;
	UPROPERTY(EditAnywhere, Category = "Debugging")
	float PointSize;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Flag which inicates whether the Co Simulator is initialized
    bool initialized;
    // The delta time for the current frame
    float frameDeltaTime;
    // The serialized co-simulation checkpoints -> to be extended inn future
    vector<string> coSimulationCheckpoints;
    // The checkoints of the scenes
    map<string, string> sceneCheckpoints;
    // The present frame number
    int frameNumber;
    // The current fps
    float currentUpdateFPS;

    // Performs a simulation cycle for a single frame
    void DoStep( float time );

    // Pushes the scene to each adapter/MMU
    void PushScene();
    // apply the updates of the scene
    void ApplySceneUpdate();

public:
    // Called every frame
    virtual void Tick( float DeltaTime ) override;

    // setup the simulation controller
    void Setup();

    // update all MMIAvatars and MMISceneObjects in the scene
    void RegisterAllAvatarsAndObjects();

    void RegisterNewAvatar( AMMIAvatar* avatar );
    void RegisterNewObject(UMMISceneObject* mmiSceneObject);

    // exectuted at the end of the game
    virtual void EndPlay( EEndPlayReason::Type EndPlayReason ) override;

    // load and execute the instructions
    void ExecuteInstructions(AMMIAvatar* avatar);

	// load and execute the instructions
	void ExecuteInstructionsForAll();

	// abort the current instruction during the game for specific avatar
	void StopCurrentInstruction(AMMIAvatar* avatar);

	// abort the current instruction during the game for all agents
	void StopAllCurrentInstructions();

private:

	void RenderDrawcallsForSelectedAvatars();

//#if WITH_EDITORONLY_DATA
	void OnObjectSelected(UObject* Object);
//#endif

    // access to the scene
    UWorld* scene;

    // results of the simulation step
    concurrent_unordered_map<AMMIAvatar*, MSimulationResult> resultsMap;

    // current instruction ID
    map<AMMIAvatar*, string> currentInstructionsID;

	// Drawcalls from the last executed DoStep for each avatar
	map<AMMIAvatar*, std::vector<MMIStandard::MDrawingCall>> lastDrawcalls;

    int rediscoverCounter;
};
