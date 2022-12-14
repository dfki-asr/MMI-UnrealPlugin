// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// Central instance for exectuing the simulations. Collects all MAvatars and MScene Objects
// and passes them to the UnrealSceneAccess. Initializes all MAvatars and MSceneObjects during
// BeginPlay() and executes the Simulation in MOSIM during Tick()
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "SimulationController.h"
#include "UnrealSceneAccess.h"
#include "MMISettings.h"
#include "MMUAccess.h"
#include "AvatarBehavior.h"
#include "SkeletonAccess.h"
//#include "AjanAgent.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "Utils/Logger.h"
#include "CommonTools.h"

#include <thread>
#include <Concurrent_vector.h>

#include "Kismet/GameplayStatics.h"

class MMISettings;

// Sets default values
ASimulationController::ASimulationController()
    : RealTime( true ),
      FixedStepTime( 0.01f ),
      PhysicsUpdatesPerFrame( true ),
      AutoStart( true ),
      UESceneAccess( nullptr ),
      Behavior( nullptr ),
	  DisplayDrawcalls(true),
	  LineColor(FColor::Red),
	  PointColor(FColor::Yellow),
	  PointSize(5.0f),
      initialized( false ),
      frameNumber( 0 ),
      currentUpdateFPS( 30.f )
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if
    // you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Instance of the UnrealSceneAccess
    UESceneAccess = new UnrealSceneAccess();

//#if WITH_EDITOR
	USelection::SelectionChangedEvent.AddUObject(this, &ASimulationController::OnObjectSelected);
//#endif
}

ASimulationController::~ASimulationController()
{
    // set the SceneAccess pointer in all registered avatars and mmisceneobjects to nullptr
    for( AMMIAvatar* avatarInUEAccess : this->UESceneAccess->GetMMIAvatarsVector() )
    {
        avatarInUEAccess->SceneAccess = nullptr;
        avatarInUEAccess->SimController = nullptr;
    }
    // set the Simulation Controller pointer in all registered avatars to nullptr
    for( UMMISceneObject* objectInUEAccess : this->UESceneAccess->GetMMISceneObjectsVector() )
    {
        objectInUEAccess->SceneAccess = nullptr;
    }

    if( this->UESceneAccess )
        delete this->UESceneAccess;
}

// Called when the game starts or when spawned
void ASimulationController::BeginPlay()
{
    Super::BeginPlay();

	if (this->AutoStart)
	{
		/*/try
		    {*/
			this->Setup();
        /*}
		catch (exception e)
		{
			UE_LOG(LogMOSIM, Error, TEXT("Setup failed. Perhaps the MOSIM framework is not "
				"started. Simulation will be finished."));
			FString message(
				"Setup failed. Perhaps the MOSIM framework is not started. Simulation will be "
				"finished.");
			GEngine->AddOnScreenDebugMessage(static_cast<uint64>(this->GetUniqueID()), 5.0f,
				FColor::Red, message);
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			PlayerController->ConsoleCommand("quit");

			this->initialized = false;
			return;
		}*/
	}
}

void ASimulationController::Setup()
{
    // get the world
    scene = this->GetWorld();

    // start the scene access server
    this->UESceneAccess->InitializeServers();


    // collect all avatars and objects
    this->RegisterAllAvatarsAndObjects();

	CurrentSceneID = MMUAccess::GetNewGuid();

    // assign the instructions for the avatars
    this->Avatars = this->UESceneAccess->GetMMIAvatarsVector();

    // register, whether all avatars are ajan agents
    //concurrent_vector<AMMIAvatar*> AreAjanAgents = concurrent_vector<AMMIAvatar*>();
    //concurrent_vector<AMMIAvatar*> AreNotAjanAgents = concurrent_vector<AMMIAvatar*>();

    // generate mutex for controlling the write access to the vectors above
    // generate mutex for controlling the skeleton access
    mutex mtx;
    mutex mtxReta;
    mutex mtxSetup;

    int ajanClPortCounter = 8083;

    bool avatarLoadSuccess = false;
    for( AMMIAvatar* avatar : this->Avatars )
    {
        UE_LOG( LogMOSIM, Display, TEXT( "AjanCLPort: %i" ), ajanClPortCounter );

        /*
        // Create a new session id
        string avatarSessionID = MMUAccess::GetNewGuid();
		AvatarSessionIDs[avatar] = avatarSessionID;

        // setup the avatar
        mtxSetup.lock();
        avatarLoadSuccess = avatar->Setup( MMISettings::GetInstance()->MIPRegisterAddress, avatarSessionID,
                                           this->UESceneAccess, this, mtx, mtxReta );
        mtxSetup.unlock();
        */
        avatarLoadSuccess = avatar->IsInitialized();
        if( !avatarLoadSuccess )
        {
            FString messageFStr = "Avatar setup failed.";
            UE_LOG( LogMOSIM, Error, TEXT( "%s" ), *messageFStr );
            GEngine->AddOnScreenDebugMessage( static_cast<uint64>( this->GetUniqueID() ), 5.0f,
                                              FColor::Red, messageFStr );
            this->initialized = false;
            return;
        }

        // check, whether Ajan is running
        /*
        TArray<UAjanAgent*> AjanComponents;
        avatar->GetComponents<UAjanAgent>( AjanComponents );
        if( AjanComponents.Num() > 0 && AjanComponents[0]->isAjanRunning )
        {
            AjanComponents[0]->SetClPort( ajanClPortCounter );
            avatar->running = true;
            AreAjanAgents.push_back( avatar );
        }
        else
        {
            AreNotAjanAgents.push_back( avatar );
        }

        // each Ajan agent requires 3 subsequent ports
        ajanClPortCounter += 3;
        */
    }

    // look for instructions in AAvatarBehavior Class in case Avatars are found that are not
    // controlled by Ajan
   // if( !AreNotAjanAgents.empty() )
    //{
        // get the avatarBehavior
    /*
		AActor* simActor = UGameplayStatics::GetActorOfClass(scene, AAvatarBehavior::StaticClass());
		Behavior = Cast<AAvatarBehavior>(simActor);

        if( !this->Behavior )
        { 
			// continue, in case no AvatarBehavior class is registered
            FString message =
                FString::Printf( TEXT( "No AAvatarBehavior class found. All Avatars not marked as "
                                       "AjanAgents will not execute any instructions." ) );
            UE_LOG( LogMOSIM, Warning, TEXT("%s"), *message);
            GEngine->AddOnScreenDebugMessage( static_cast<uint64>( this->GetUniqueID() ), 5.0f,
                                              FColor::Red, message );
            //return;
        }
		else
		{
			UE_LOG(LogMOSIM, Display,
				TEXT("ASimulationController(): AAvatarBehavior class found."));
		}
   // }
   */

    this->initialized = true;
}

void ASimulationController::ExecuteInstructions(AMMIAvatar* avatar)
{
    if( !avatar->behavior)
    {
        return;
    }
    auto& instructions = avatar->behavior->GetInstructions();

	MBoolResponse retBool;

	MSimulationState simstate = MSimulationState();
	simstate.Current = avatar->ReadCurrentPosture();
	simstate.Initial = avatar->ReadCurrentPosture();

	for (MInstruction& instruction : instructions)
	{
		for (int i = 0; i < avatar->MMUAccessPtr->MotionModelUnits.size(); i++)
		{
			try
			{
				avatar->MMUAccessPtr->MotionModelUnits[i]->AssignInstruction(
					retBool, instruction, simstate);
				avatar->running = true;
				this->currentInstructionsID[avatar] = instruction.ID;
			}
			catch (const exception& e)
			{
				string message = string(e.what());
				Logger::printLog(L_ERROR, message);
				FString messageFStr = FString::Printf(
					TEXT("%s. Please restart MOSIM framework and the simulation."),
					UTF8_TO_TCHAR(message.c_str()));
				UE_LOG(LogMOSIM, Warning, TEXT("%s"), *messageFStr);
				GEngine->AddOnScreenDebugMessage(static_cast<uint64>(this->GetUniqueID()), 5.0f,
					FColor::Red, messageFStr);
			}
		}
	}
}

void ASimulationController::ExecuteInstructionsForAll()
{
    for( auto it = Avatars.begin(); it != Avatars.end(); ++it )
    {
        ExecuteInstructions(*it);
    }
}

void ASimulationController::StopCurrentInstruction(AMMIAvatar* avatar)
{
    MBoolResponse retBool;
    for( int i = 0; i < avatar->MMUAccessPtr->MotionModelUnits.size(); i++ )
    {
		avatar->MMUAccessPtr->MotionModelUnits[i]->Abort( retBool, currentInstructionsID[avatar] );
        if( !retBool.Successful )
        {
            string message = "Error while aborting instruction for " + avatar->baseName;
            Logger::printLog( L_ERROR, message );
            FString messageFStr =
                FString::Printf( TEXT( "%s. Please restart MOSIM framework and the simulation." ),
                                 UTF8_TO_TCHAR( message.c_str() ) );
            UE_LOG( LogMOSIM, Warning, TEXT( "%s" ), *messageFStr );
            GEngine->AddOnScreenDebugMessage( static_cast<uint64>( this->GetUniqueID() ), 5.0f,
                                              FColor::Red, messageFStr );
        }
    }
}

void ASimulationController::StopAllCurrentInstructions()
{
    for( auto it = Avatars.begin(); it != Avatars.end(); ++it )
    {
        StopCurrentInstruction( *it );
    }
}

// Called every frame
void ASimulationController::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );

    // estimate updates in FPS
    this->currentUpdateFPS = 1.0f / DeltaTime;

    // Get the desired delta time for the current frame
    this->frameDeltaTime = this->RealTime ? DeltaTime : this->FixedStepTime;

    // skip rest, if not initialized
    if( !this->initialized )
    {
        FString message(
            "Setup failed in Tick. Perhaps the MOSIM framework is not started. Simulation will be "
            "terminated." );
        UE_LOG( LogMOSIM, Error, TEXT( "%s" ), *message );
        GEngine->AddOnScreenDebugMessage( static_cast<uint64>( this->GetUniqueID() ), 5.0f,
                                          FColor::Red, message );
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        PlayerController->ConsoleCommand( "quit" );
        return;
    }

    if (rediscoverCounter % RediscoverRate == 0) {
        //this->RegisterAllAvatarsAndObjects();
    }

    this->DoStep( this->frameDeltaTime );

    // count the frames
    this->frameNumber++;
}

void ASimulationController::DoStep( float time )
{
    clock_t begin, begin_, scene_, doStep, doStep_, apply_;
    begin = std::clock();

    // Scene synchronization of each MMU Access
    this->PushScene();
    scene_ = std::clock();

	bool invalidateDebug = false;

    // Do the Co Simulation for each Avatar
    // Create a map which contains the avatar states for each MMU
    for( AMMIAvatar* avatar : this->Avatars )  // could be parallelized in multiple threads
    {
        if( avatar->running )
        {
            MSimulationState simstate = MSimulationState();
            simstate.Current = avatar->ReadCurrentPosture();
            simstate.Initial = simstate.Current;

            MSimulationResult simRes = MSimulationResult();

            for( int i = 0; i < avatar->MMUAccessPtr->MotionModelUnits.size(); i++ )
            {
                doStep = std::clock();
                avatar->MMUAccessPtr->MotionModelUnits[i]->DoStep( simRes, time, simstate );
                doStep_ = std::clock();
                break;
            }
            apply_ = std::clock();
            // pass channel data to the remote skeleton access
            avatar->skeletonAccessPtr->SetChannelData( simstate.Current );

            // Add the results
            this->resultsMap.insert( pair<AMMIAvatar*, MSimulationResult>{avatar, simRes} );

            for( string st : simRes.LogData )
            {
                FString fstr( st.c_str() );
                UE_LOG( LogMOSIM, Display, TEXT( "%s" ), *fstr );
            }

			if (simRes.DrawingCalls.size() > 0)
			{
				lastDrawcalls[avatar] = simRes.DrawingCalls;
				invalidateDebug = true;
			}

            // apply the results
            this->ApplySceneUpdate();


            // clear all results, as this is no archive
            this->resultsMap.clear();
        }
    }
    begin_ = std::clock();
    float t1 = float( begin_ - begin ) / CLOCKS_PER_SEC;    // total
    float t2 = float( scene_ - begin ) / CLOCKS_PER_SEC;    // scene
    float t3 = float( doStep - scene_ ) / CLOCKS_PER_SEC;   // read
    float t4 = float( doStep_ - doStep ) / CLOCKS_PER_SEC;  // dostep
    float t5 = float( begin_ - apply_ ) / CLOCKS_PER_SEC;   // apply
    UE_LOG( LogMOSIM, Display,
            TEXT( "Timings: %.3f, scene: %.3f, read: %.3f dostep: %.3f, apply: %.3f" ),
            t1, t2, t3, t4, t5
             );

    #if WITH_EDITOR
	if (invalidateDebug && DisplayDrawcalls)
		RenderDrawcallsForSelectedAvatars();
    #endif
    
}


void ASimulationController::RenderDrawcallsForSelectedAvatars()
{
#if WITH_EDITOR
	FlushPersistentDebugLines(scene);

	for (auto& avatarDrawcalls: lastDrawcalls)
        {
		if (avatarDrawcalls.first->IsSelectedInEditor())
		{
			for (const MDrawingCall& drawcall : avatarDrawcalls.second)
			{
				int n = drawcall.Data.size();
				switch (drawcall.Type)
				{
				case MDrawingCallType::DrawLine2D:
				{
					for (int i = 2; i < n; i += 2)
						DrawDebugLine(scene, ToFVec2(drawcall.Data, i - 2), ToFVec2(drawcall.Data, i), LineColor, true);
					break;
				}

				case MDrawingCallType::DrawLine3D:
				{
					for (int i = 3; i < n; i += 3)
						DrawDebugLine(scene, ToFVec3(drawcall.Data, i - 3), ToFVec3(drawcall.Data, i), LineColor, true);
					break;
				}

				case MDrawingCallType::DrawPoint2D:
				{
					for (int i = 0; i < n; i += 2)
						DrawDebugPoint(scene, ToFVec2(drawcall.Data, i), PointSize, PointColor, true);
					break;
				}

				case MDrawingCallType::DrawPoint3D:
				{
					for (int i = 0; i < n; i += 3)
						DrawDebugPoint(scene, ToFVec3(drawcall.Data, i), PointSize, PointColor, true);
					break;
				}

				default:
					break;
				}
			}
		}
    }
#endif
}

void ASimulationController::PushScene()
{
    // Synchronizes the scene in before each update

    auto lambdaExp = [this]( const int& i ) {
        //if( this->frameNumber == 0 )
        if( !this->Avatars[i]->InitialSceneSynch)
        {
            this->Avatars[i]->InitialSceneSynch = true;
            this->Avatars[i]->MMUAccessPtr->PushScene( true );
        }
        else
        {
            this->Avatars[i]->MMUAccessPtr->PushScene( false );
        }
    };

    vector<thread*> ThreadVector = vector<thread*>();

    for( int i = 0; i < this->Avatars.size(); i++ )
    {
        ThreadVector.push_back( new thread( lambdaExp, i ) );
    }
    MMUAccess::StopThreads( ThreadVector );

    // Clear the events since the current events have been already synchronized
    this->UESceneAccess->Clear_SceneUpdate();
}

void ASimulationController::ApplySceneUpdate()
{
    vector<MSceneManipulation> sceneManipulations = vector<MSceneManipulation>();

    for( pair<AMMIAvatar*, MSimulationResult> coSimulationResult : this->resultsMap )
    {
        // Assign the posture of the avatar
        coSimulationResult.first->ApplyPostureValues( coSimulationResult.second.Posture );

        // Add the scene manipulations
        for( MSceneManipulation sceneManipuation : coSimulationResult.second.SceneManipulations )
            sceneManipulations.push_back( sceneManipuation );
    }

    // Apply the manipulations of the scene
    MBoolResponse boolResp;
    this->UESceneAccess->ApplyManipulations( boolResp, sceneManipulations );
    if( !boolResp.Successful )
    {
        string message = "Problems assigning scene manipulations!";
        UE_LOG( LogMOSIM, Error, TEXT( "%s" ), *FString( message.c_str() ) );
        Logger::printLog( L_ERROR, message );
    }

    // TODO: Apply the remote scene updates (not implemented so far)
    // this->ApplyRemoteSceneUpdates();
}

void ASimulationController::RegisterNewAvatar( AMMIAvatar* avatar )
{
    if (this->UESceneAccess == nullptr) {
        return;
    }
    bool check = true;
    for( int i = 0; i < this->Avatars.size(); i++ )
    {
        if( this->Avatars[i]->AvatarID == avatar->AvatarID )
        {
            check = false;
            break;
        }
    }
    if( check )
    {
        mutex mtx;
        mutex mtxReta;
        mutex mtxSetup;
        this->UESceneAccess->AddMMIAvatar( avatar->MAvatar.ID, avatar );
        this->Avatars.push_back(avatar);
        if (!avatar->IsInitialized()) {
            // Create a new session id
            string avatarSessionID = MMUAccess::GetNewGuid();
		    AvatarSessionIDs[avatar] = avatarSessionID;

            mtxSetup.lock();
            avatar->Setup( MMISettings::GetInstance()->MIPRegisterAddress, avatarSessionID,
                                               this->UESceneAccess, this, mtx, mtxReta );
            mtxSetup.unlock();
        }

    }
}

void ASimulationController::RegisterNewObject(UMMISceneObject* mmiSceneObject)
{
    if (this->UESceneAccess == nullptr) {
        return;
    }
    bool check = true;
    for( int i = 0; i < this->sceneObjects.size(); i++ )
    {
        if( this->sceneObjects[i] == mmiSceneObject)
        {
            check = false;
            break;
        }
    }
    if( check )
    {
                // update the names of the Object
				mmiSceneObject->Setup(UESceneAccess);
                UE_LOG( LogMOSIM, Display, TEXT( "UMMISceneObject with name %s found." ),
                        *FString( mmiSceneObject->Name.c_str() ) );
                // Register the scene object at the Unreal Scene Access
                this->UESceneAccess->AddMMISceneObject( mmiSceneObject->MSceneObject.ID,
                                                        mmiSceneObject );
                this->sceneObjects.push_back(mmiSceneObject);
    }

}

void ASimulationController::RegisterAllAvatarsAndObjects()
{
    // collect all avatars
    scene = this->GetWorld();
    for( TActorIterator<AMMIAvatar> ActorItr( scene ); ActorItr; ++ActorItr )
    {
        UE_LOG( LogMOSIM, Display, TEXT( "AMMIAvatar with name %s found." ),
                *FString( ActorItr->MAvatar.Name.c_str() ) );
        // Register Avatar at the Unreal Scene Access
        RegisterNewAvatar(*ActorItr);
        //this->UESceneAccess->AddMMIAvatar( ActorItr->MAvatar.ID, *ActorItr );
    }

    // collect all MMISceneObjects
    for( TActorIterator<AActor> USceneObjItr( scene ); USceneObjItr; ++USceneObjItr )
    {
        TArray<USceneComponent*> ComponentsArray;
        USceneObjItr->GetComponents<USceneComponent>( ComponentsArray );
        for( USceneComponent* sceneComponent : ComponentsArray )
        {
            UMMISceneObject* mmiSceneObject = Cast<UMMISceneObject>( sceneComponent );
            if( mmiSceneObject )
            {
                RegisterNewObject(mmiSceneObject);
                /*
                // update the names of the Object
				mmiSceneObject->Setup(UESceneAccess);
                UE_LOG( LogMOSIM, Display, TEXT( "UMMISceneObject with name %s found." ),
                        *FString( mmiSceneObject->Name.c_str() ) );
                // Register the scene object at the Unreal Scene Access
                //this->UESceneAccess->AddMMISceneObject( mmiSceneObject->MSceneObject.ID,
                //mmiSceneObject );
                */
            }
        }
    }
}

// necessary, as the thrift servers are othwerise not terminated at the end of the simulation
void ASimulationController::EndPlay( EEndPlayReason::Type EndPlayReason )
{

    if( this->UESceneAccess )
        this->UESceneAccess->UESceneAccessServerCore->~UnrealSceneAccessServer();

    // set back the port counter to the original value
    AMMIAvatar::RemoteCoSimulationAccessPortIncremented = 9011;
}



void ASimulationController::OnObjectSelected(UObject* selectedObject)
{
#if WITH_EDITOR
	if(DisplayDrawcalls)
		RenderDrawcallsForSelectedAvatars();
#endif
}

