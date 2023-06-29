// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Janis Sprenger, Niklas Braun
#pragma once

#include "Windows\AllowWindowsPlatformTypes.h"
#include "avatar_types.h"
#include "MCoSimulationAccess.h"
#include "MSkeletonAccess.h"
#include "Windows\HideWindowsPlatformTypes.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMesh.h"
#include "Components/ActorComponent.h"

#include "MMISettings.h"

// load JSON:
#include "CoreMinimal.h"
#include "Misc/FileHelper.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "UObject/UObjectGlobals.h"

#include "AvatarBehavior.h"

#include "MMIAvatarComponent.generated.h"

// forward declarations
class ASimulationController;
class UnrealSceneAccess;
class MMUAccess;
class RetargetingAccess;
class SkeletonAccess;
class MMISettings;
class UAvatarBehavior;

using namespace std;
using namespace MMIStandard;

UCLASS( Blueprintable, BlueprintType, ClassGroup = ( MOSIM ),
        meta = ( BlueprintSpawnableComponent ) )
class MMISCENE_API UMosimAvatar : public UActorComponent
{
    GENERATED_BODY()

public: 
    UMosimAvatar();
    // destructor
    void BeginPlay() override;
    void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

    void OnComponentDestroyed( bool bDestroyingHierarchy ) override;


    UFUNCTION( BlueprintCallable, BlueprintImplementableEvent,
               meta = ( DisplayName = "SetSnapshot" ) )
    void SetSnapshot( FPoseSnapshot snap );

    UFUNCTION( BlueprintCallable, BlueprintImplementableEvent,
               meta = ( DisplayName = "GetSnapshot" ) )
    FPoseSnapshot GetSnapshot();

        // Relative path of the reference posture file for MOSIM retargeting service.
    UPROPERTY( VisibleAnywhere, BlueprintReadWrite,
               Category = "MOSIM Properties" )  //, Meta = ( FilePathFilter = "mos" ) )
    FString ReferencePostureFile;


        // Relative path of the reference posture file for MOSIM retargeting service.
    UAvatarBehavior* behavior;

    // add bones of the avatar as scene objects
    bool AddBoneSceneObjects;

    // timeout for establishing the connection to the MMI framework and the remote MMUs in seconds
    int Timeout;

    // static storage of the Port Number
    // is incremented by 1 for every MMIAvatar instance and passed to the CoSimulationMMU
    // each CoSimulationMMU starts a remote scene access server that listen on the respective port
    // port is passed to Ajan and Ajan registers at the respective port at the
    // RemoteCoSimulationAccess
    static int RemoteCoSimulationAccessPortIncremented;

    // the port for the external write access
    int RemoteCoSimulationAccessPort;

    // the address for the external write access
    string RemoteCoSimulationAccessAddress;

    // ID of the avatar, ideally automatically generated with a UUID.
    string AvatarID;

    UPROPERTY( VisibleAnywhere, Category = "MOSIM Properties" )
    FString AvatarIDVis;

    // ID of the session
    string sessionID;

    // basename of the avatar
    string baseName;
    void UpdateBaseName();

    string statusText;

    // Representation of the MAvatar
    MAvatar MAvatar;

    // class providing the access to the MMUs
    MMUAccess* MMUAccessPtr;

    // pointer to the unreal scene access
    UnrealSceneAccess* SceneAccess;

    // access to the simulation controller
    ASimulationController* SimController;

    // the remote skeleton access
    SkeletonAccess* skeletonAccessPtr;

    // Function to read out the current MAvatarPostureValues of the Avatar. Utilizes the Retargeting
    // Service.
    MAvatarPostureValues ReadCurrentPosture();

    // current helper variable to check, if everything is set up and tick can be used.
    UPROPERTY( EditAnywhere, Category = "MOSIM" )
    bool running;

    bool IsInitialized();
    bool InitialSceneSynch = false;

    // Function to apply Posture Values to the Avatar. Utilizes the Retargeting Service.
    void ApplyPostureValues( MAvatarPostureValues vals );

    // setup method for the avatar, called by the simulation controller at begin play
    bool Setup( MIPAddress registerAddress, string _sessionID, UnrealSceneAccess* _SceneAccess,
                ASimulationController* _SimContr, mutex& mtx, mutex& mtxReta );

    // Called every frame
    virtual void TickComponent( float DeltaTime, enum ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction ) override;

    protected:

    // the retrageting access
    RetargetingAccess* retargetingAccessPtr;

    // Loads Avatar Posture from File
    MAvatarPosture LoadAvatarPosture( FString filePath );
    // Reference Posture for Retargeting
    MAvatarPosture GlobalReferencePosture;

        UFUNCTION( BlueprintCallable, Category = "MOSIM|Functions" )
    void SetAvatarID( FString id );

            UFUNCTION( BlueprintCallable, Category = "MOSIM|Functions" )
    FString GetAvatarID();


private:
    bool isInitialized;
    FString actualConfigFilePath;

    std::map<int, const char*> _MJointType_VALUES_TO_NAMES;
    int nJoints = 66;

    static FQuat UE2MOSIM;
    static FQuat MOSIM2UE;

    const char* _kMJointTypeNames[66] = { "Undefined",
                                          "LeftBallTip",
                                          "LeftBall",
                                          "LeftAnkle",
                                          "LeftKnee",
                                          "LeftHip",
                                          "RightBallTip",
                                          "RightBall",
                                          "RightAnkle",
                                          "RightKnee",
                                          "RightHip",
                                          "PelvisCentre",
                                          "S1L5Joint",
                                          "T12L1Joint",
                                          "T1T2Joint",
                                          "C4C5Joint",
                                          "HeadJoint",
                                          "HeadTip",
                                          "MidEye",
                                          "LeftShoulder",
                                          "LeftElbow",
                                          "LeftWrist",
                                          "RightShoulder",
                                          "RightElbow",
                                          "RightWrist",
                                          "LeftThumbMid",
                                          "LeftThumbMeta",
                                          "LeftThumbCarpal",
                                          "LeftThumbTip",
                                          "LeftIndexMeta",
                                          "LeftIndexProximal",
                                          "LeftIndexDistal",
                                          "LeftIndexTip",
                                          "LeftMiddleMeta",
                                          "LeftMiddleProximal",
                                          "LeftMiddleDistal",
                                          "LeftMiddleTip",
                                          "LeftRingMeta",
                                          "LeftRingProximal",
                                          "LeftRingDistal",
                                          "LeftRingTip",
                                          "LeftLittleMeta",
                                          "LeftLittleProximal",
                                          "LeftLittleDistal",
                                          "LeftLittleTip",
                                          "RightThumbMid",
                                          "RightThumbMeta",
                                          "RightThumbCarpal",
                                          "RightThumbTip",
                                          "RightIndexMeta",
                                          "RightIndexProximal",
                                          "RightIndexDistal",
                                          "RightIndexTip",
                                          "RightMiddleMeta",
                                          "RightMiddleProximal",
                                          "RightMiddleDistal",
                                          "RightMiddleTip",
                                          "RightRingMeta",
                                          "RightRingProximal",
                                          "RightRingDistal",
                                          "RightRingTip",
                                          "RightLittleMeta",
                                          "RightLittleProximal",
                                          "RightLittleDistal",
                                          "RightLittleTip",
                                          "Root" };
};