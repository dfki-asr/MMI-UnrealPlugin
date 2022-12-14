// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam, Janis Sprenger

// This class is the representation of the MOSIM Avatar in the Unreal Engine. It is derivated from
// APawn.
// APawn was favored compared to ACharacter, as APawn allows a simpler C++ based access to the
// skeleton.
// Defines methods for accessing the Avatars skeleton, including coordinate transformations.
// Connects via MMUAccess to the MOSIM framework, loads and initializes the CoSimulationMMU
// Connects to the required services (SkeletonAccess and Retargeting)

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "MMIAvatar.h"
#include "MOSIM.h"
#include "Utils/Logger.h"
#include "MotionModelUnitAccess.h"
#include "SimulationController.h"
#include "UnrealSceneAccess.h"
#include "MMUAccess.h"
#include "RetargetingAccess.h"
#include "SkeletonAccess.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include <map>
#include <regex>
#include "EngineUtils.h"
#include "Engine.h"
#include "CommonTools.h"

#include "avatar_types.h"

// initialize static variables
int AMMIAvatar::RemoteCoSimulationAccessPortIncremented =
    MMISettings::GetInstance()->RemoteCoSimulationAccessPort;

// Sets default values
AMMIAvatar::AMMIAvatar(const FObjectInitializer& ObjectInitializer)
    : Super( ObjectInitializer.DoNotCreateDefaultSubobject( ACharacter::MeshComponentName ) ), 
    AddBoneSceneObjects( false ),
      Timeout( 1 ),
      RemoteCoSimulationAccessPort( 0 ),
      RemoteCoSimulationAccessAddress( "127.0.0.1" ),
      AvatarID( string( "" ) ),
      sessionID( string( "" ) ),
      baseName( string( "" ) ),
      statusText( string( "" ) ),
      MMUAccessPtr( nullptr ),
      SceneAccess( nullptr ),
      SimController( nullptr ),
      skeletonAccessPtr( nullptr ),
      running( true ),
      retargetingAccessPtr( nullptr ),
      isInitialized( false )
{

    for(int i = 0; i < this->nJoints; i++)
    {
        this->_MJointType_VALUES_TO_NAMES.insert_or_assign( i, _kMJointTypeNames[i]);
    }
    // set the base name (name of the Avatar without automatic extensions by Unreal fro making the
    // Name unique (e.g "_2"))
    // applied in the AvatarBehavior class for looking for the behavior
    this->UpdateBaseName();

    //this->MOSIMMesh =
    //    CreateDefaultSubobject<UPoseableMeshComponent>( TEXT( "CharacterPoseableMesh" ) );
    //this->MOSIMMesh->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    // Initialize MAvatar
    // Create a new UUID for the avatar ID, this prevents issues in the retargeting and MMU usage
    // later on.
    this->AvatarID = MMUAccess::GetNewGuid();
    this->MAvatar.ID = this->AvatarID;
    this->MAvatar.Name = this->baseName;
    this->MAvatar.Description = MAvatarDescription();
    this->MAvatar.Description.AvatarID = this->AvatarID;

    this->AvatarIDVis = this->AvatarID.c_str();

    // Set this character to call Tick() every frame.  You can turn this off to improve performance
    // if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Add the poseable mesh component. This will appear as an empty component, which has to be
    // configured by the user when making a new blueprint.
    this->MOSIMMesh = CreateDefaultSubobject<UPoseableMeshComponent>( TEXT( "Mesh" ) );
    this->MOSIMMesh->SetHiddenInGame( false, true );

    // set the collison for the whole actor
    this->SetActorEnableCollision( true );

}

void AMMIAvatar::OnConstruction( const FTransform& Transform )
{
    Super::OnConstruction( Transform );
    auto height = this->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    if( this->MOSIMMesh != nullptr )
    {
        this->MOSIMMesh->SetRelativeLocation( FVector( 0, 0, -height ) );
        this->MOSIMMesh->SetRelativeRotation( FQuat( 0, 0, 0, 1 ) );
    }
    
}


void AMMIAvatar::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    this->AvatarIDVis = this->AvatarID.c_str();

}

FString AMMIAvatar::GetAvatarID()
{
    return this->AvatarID.c_str();
}

void AMMIAvatar::SetAvatarID( FString id )
{
    this->AvatarID = std::string(TCHAR_TO_UTF8(*id));
    this->MAvatar.ID = this->AvatarID;
    this->AvatarIDVis = this->AvatarID.c_str();
    this->MAvatar.Description.AvatarID = this->AvatarID;
    this->MAvatar.Description.ZeroPosture.AvatarID = this->AvatarID;
}

void AMMIAvatar::SetBehavior( UAvatarBehavior* b )
{
    this->behavior = b;
}




AMMIAvatar::~AMMIAvatar()
{
    if( this->MMUAccessPtr )
        delete MMUAccessPtr;

    if( this->retargetingAccessPtr )
        delete retargetingAccessPtr;

    if( this->skeletonAccessPtr )
        delete skeletonAccessPtr;

    // unregister the Avatar at the UnrealSceneAccess
    if( this->SceneAccess )
    {
        this->SceneAccess->RemoveAvatar_SceneUpdate( this->MAvatar );
        this->SceneAccess->RemoveMMIAvatar( this->MAvatar.ID );
    }

    // unregister the Avatar at the Simulation Controller
    if( this->SimController )
        this->SimController->Avatars.erase( find( this->SimController->Avatars.begin(),
                                                  this->SimController->Avatars.end(), this ) );
}

// Called when the game starts or when spawned
void AMMIAvatar::BeginPlay()
{
    Super::BeginPlay();
}

bool AMMIAvatar::Setup( MIPAddress registerAddress, string _sessionID,
                        UnrealSceneAccess* _SceneAccess, ASimulationController* _SimContr,
                        mutex& mtx, mutex& mtxReta )
{
    if( this->isInitialized )
        return true;
    // set the unreal scene access
    this->SceneAccess = _SceneAccess;

    // set the simulation controller access
    this->SimController = _SimContr;

    // set the RemoteCoSimulationAccessPort
    this->RemoteCoSimulationAccessPort = RemoteCoSimulationAccessPortIncremented++;

    // load the reference posture
    try
    {
        // Loading the reference zero posture.
        MAvatarPosture zeroP = this->LoadAvatarPosture( FPaths::ProjectContentDir() + ReferencePostureFile );
        this->GlobalReferencePosture = zeroP;
        UE_LOG( LogMOSIM, Display, TEXT( " Successfully loaded AvatarDescription %s" ),
                *ReferencePostureFile );
    }
    catch( exception e )
    {
        // In case of an error, we should actually stop the program and notify the user, otherwise
        // the software will exit with a access violation exception and no information.
        string message = "Failed to load the reference posture " +
                         string( TCHAR_TO_UTF8( *ReferencePostureFile ) ) + "!";
        runtime_error( message.c_str() );
        UE_LOG( LogMOSIM, Error, TEXT( "%s" ), *FString( message.c_str() ) );
        this->isInitialized = false;
        return false;
    }

    // Connection to the remote retargeting.
    try
    {
        this->retargetingAccessPtr =
            new RetargetingAccess( _sessionID, registerAddress, this->Timeout, this->AvatarID );
    }
    catch( exception e )
    {
        string message = "Failed to access the retargeting service!";
        runtime_error( message.c_str() );
        UE_LOG( LogMOSIM, Error, TEXT( "%s" ), *FString( message.c_str() ) );
        this->isInitialized = false;
        return false;
    }
    // Retarget configuration and store in avatar.
    mtxReta.lock();
    this->MAvatar.Description =
        this->retargetingAccessPtr->SetupRetargeting( this->GlobalReferencePosture );
    mtxReta.unlock();

    // Connection to the remote skeleton access.
    try
    {
        this->skeletonAccessPtr =
            new SkeletonAccess( _sessionID, registerAddress, this->Timeout, this->AvatarID );
    }
    catch( exception e )
    {
        string message = "AMMIAvatar: Failed to access the skeleton service!";
        runtime_error( message.c_str() );
        UE_LOG( LogMOSIM, Error, TEXT( "%s" ), *FString( message.c_str() ) );
        this->isInitialized = false;
        return false;
    }

    // Initialize the anthropometry
    mtx.lock();
    this->skeletonAccessPtr->InitializeAnthropometry( this->MAvatar.Description );
    mtx.unlock();

    // pass the zero Posture
    mtxReta.lock();
    this->MAvatar.PostureValues = this->ReadCurrentPosture();
    mtxReta.unlock();

    // add the MAvatar to the UnrealSceneAccess->SceneUpdate
    mtx.lock();
    this->SceneAccess->AddAvatar_SceneUpdate( this->MAvatar );
    mtx.unlock();

    // Set up the MMUAccess and connect to the adapters
    this->MMUAccessPtr = new MMUAccess( _sessionID );
    this->MMUAccessPtr->AvatarID = this->AvatarID;
    this->MMUAccessPtr->SceneAccess = this->SceneAccess;

    try
    {
        this->MMUAccessPtr->Connect( registerAddress, this->Timeout, this->AvatarID,
                                     this->MAvatar.Description );
    }
    catch( exception e )
    {
        string message = "AMMIAvatar: Failed to connect to MMUs!";
        runtime_error( message.c_str() );
        UE_LOG( LogMOSIM, Error, TEXT( "%s" ), *FString( message.c_str() ) );
        this->isInitialized = false;
        return false;
    }

    // get the loadable MMUs
    vector<MMUDescription> mmuDescs = this->MMUAccessPtr->GetLoadableMMUsClient();

    // load the MMUs
    MMUDescription mmuDescToLoad;
    for( MMUDescription mmuDesc : mmuDescs )
    {
        if( mmuDesc.Name.find( "CoSimulation" ) != mmuDesc.Name.npos )
        {
            mmuDescToLoad = mmuDesc;
            break;
        }
    }

    bool loadedSuccess = this->MMUAccessPtr->LoadSpecificMMU( mmuDescToLoad, this->Timeout );
    if( !loadedSuccess )
    {
        string message = "Failed to load MMUs!";
        runtime_error( message.c_str() );
        UE_LOG( LogMOSIM, Error, TEXT( "%s" ), *FString( message.c_str() ) );
    }
    else
    {
        try
        {
            // TODO_IMPORTANT: perhaps create the initialization properties here --> e.g. disposing
            // the co-simulation mmu
            map<string, string> initializationProperties = map<string, string>();
            initializationProperties.insert( pair<string, string>{
                string( "AccessPort" ), to_string( this->RemoteCoSimulationAccessPort )} );

            bool initSuccess = this->MMUAccessPtr->InitializeSpecificMMU(
                this->Timeout, this->MAvatar.Description, this->MMUAccessPtr->MotionModelUnits[0],
                initializationProperties );
        }
        catch( const exception& e )
        {
            string message = string( e.what() );
            FString messageFStr =
                FString::Printf( TEXT( "%s  Please restart MOSIM framework and the simulation." ),
                                 UTF8_TO_TCHAR( message.c_str() ) );
            UE_LOG( LogMOSIM, Error, TEXT( "%s" ), *messageFStr );
            GEngine->AddOnScreenDebugMessage( static_cast<uint64>( this->GetUniqueID() ), 5.0f,
                                              FColor::Red, messageFStr );
            this->isInitialized = false;
            return false;
        }
    }

    this->isInitialized = true;
    return true;
}

bool AMMIAvatar::IsInitialized()
{
    return this->isInitialized;
}

// Called every frame
void AMMIAvatar::Tick( float DeltaTime )
{
    if( !this->isInitialized )
    {
        auto scene = this->GetWorld();
        for( TActorIterator<ASimulationController> SimContr( scene ); SimContr; ++SimContr )
        {
            SimContr->RegisterNewAvatar( this );
        }
        return;
    }
        

    Super::Tick( DeltaTime );

    // Get the posture values of the current posture
    this->MAvatar.PostureValues = this->ReadCurrentPosture();

    // Notify the Unreal Scene Access scene updates about the update
    this->SceneAccess->PostureValuesChanged_SceneUpdate( this->MAvatar,
                                                         this->MAvatar.PostureValues );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Functions

// At first, we need to load the avatar configuration. Using BPs could help with setting up the
// correct file path, but could be solved differently as well.
FString AMMIAvatar::BP_LoadAvatarConfig( FString filePath )
{
    try
    {
        // Loading the reference zero posture.
        MAvatarPosture zeroP = this->LoadAvatarPosture( filePath );
        this->GlobalReferencePosture = zeroP;
        return TEXT( "Successfully loaded AvatarDescription" );
    }
    catch( exception e )
    {
        // In case of an error, we should actually stop the program and notify the user, otherwise
        // the software will exit with a access violation exception and no information.
        return TEXT( "Error during loading" );
    }
}

// Second we need to connect to the services and in this demo to the idle mmu directly.
FString AMMIAvatar::BP_LoadingCallback()
{
    UE_LOG( LogMOSIM, Warning, TEXT( "Janis: BP Loading Callback" ) );

    // register at the launcher and get the adapters
    MIPAddress mmiRegisterAddress;
    mmiRegisterAddress.Address = "127.0.0.1";
    mmiRegisterAddress.Port = 9009;

    // Connection to the remote retargeting.
    this->retargetingAccessPtr = new RetargetingAccess(
        this->MMUAccessPtr->SessionId, mmiRegisterAddress, this->Timeout, this->AvatarID );
    // Retarget configuration and store in avatar.
    this->MAvatar.Description =
        this->retargetingAccessPtr->SetupRetargeting( this->GlobalReferencePosture );

    // get the loadable MMUs
    vector<MMUDescription> mmuDescs = this->MMUAccessPtr->GetLoadableMMUsClient();

    // load the MMUs
    MMUDescription mmuDescToLoad;
    for( MMUDescription mmuDesc : mmuDescs )
    {
        if( mmuDesc.Name.find( "UnityIdleMMU" ) != mmuDesc.Name.npos )
        {
            mmuDescToLoad = mmuDesc;
            break;
        }
    }

    bool loadedSuccess = this->MMUAccessPtr->LoadSpecificMMU( mmuDescToLoad, this->Timeout );
    if( !loadedSuccess )
    {
        string logString2 = "Problems loading MMUs!";
        FString returnString( logString2.c_str() );
        // In this case we should stop the further execution and notify the user, to prevent access
        // violation errors.
        return returnString;
    }
    else
    {
        // print loaded MMUs in the logger
        string numMMUs = to_string( this->MMUAccessPtr->MotionModelUnits.size() );
        string logString = numMMUs + " MMUs loaded:";
        for( MotionModelUnitAccess* loadedMMUs : this->MMUAccessPtr->MotionModelUnits )
        {
            const MMUDescription* mmuDesc = loadedMMUs->GetMMUDescription();
            logString = logString + " " + mmuDesc->Name;
        }
        UE_LOG( LogMOSIM, Display, TEXT( "%s" ), *logString.c_str() );
        MBoolResponse resp;
        map<string, string> empty = map<string, string>();

        // intialize the MMU (here only the co-simulation MMU)
        this->MMUAccessPtr->InitializeSpecificMMU( this->Timeout, this->MAvatar.Description,
                                                   this->MMUAccessPtr->MotionModelUnits[0], empty );
        // convert output string to FString
        FString returnString( logString.c_str() );
        return returnString;
    }
}

// This function has to be called afterwards, to set the motion instruction to idle
FString AMMIAvatar::BP_Idle()
{
    MBoolResponse ret;
    MInstruction minstr = MInstruction();
    minstr.ID = this->MMUAccessPtr->GetNewGuid();
    minstr.Name = "Idle";
    minstr.MotionType = "Pose/Idle";

    // Ideally, we do not want to manage the simulation state here, but in the Co-Simulation or
    // Simulation Controller.
    MSimulationState simstate = MSimulationState();
    simstate.Current = this->ReadCurrentPosture();
    simstate.Initial = this->ReadCurrentPosture();

    // This has to be replaced with an MInstruction to the Co-Simulator, which
    // then in turn should distribute this to the Idle mmu.
    for( int i = 0; i < this->MMUAccessPtr->MotionModelUnits.size(); i++ )
    {
        if( this->MMUAccessPtr->MotionModelUnits[i]->getMotionType() == minstr.MotionType )
        {
            this->MMUAccessPtr->MotionModelUnits[i]->AssignInstruction( ret, minstr, simstate );
            this->running = true;
            return FString( "Successfully Assigned Idle Instruction to Idle MMU" );
        }
    }
    return FString( "There was no Idle MMU in your Environment!" );
}

// This is a placeholder update function.
void AMMIAvatar::BP_Update( float dt )
{
    // This is placeholder code. Here, we should connect to the Co-Simulator and query the new
    // frame. A similar function should be called from the Simulation Controller.
    if( this->running )
    {
        MSimulationResult ret;

        // The simulation state is not something, that we want to manage here.
        // Ideally, this is managed by the co-simulator / simulation controller and has not to be
        // considered here.
        MSimulationState simstate = MSimulationState();
        simstate.Current = this->ReadCurrentPosture();
        simstate.Initial = this->ReadCurrentPosture();

        // This has to be replaced with the Co-Simulator call.
        for( int i = 0; i < this->MMUAccessPtr->MotionModelUnits.size(); i++ )
        {
            if( this->MMUAccessPtr->MotionModelUnits[i]->getMotionType() == "Pose/Idle" )
            {
                this->MMUAccessPtr->MotionModelUnits[i]->DoStep( ret, dt, simstate );
                break;
            }
        }

        // Apply the returned posturevalues to the avatar.
        this->ApplyPostureValues( ret.Posture );
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Posture Helper Functions.

/**
 *
 * LoadAvatarPosture loads the reference posture in the *.mos format for retargeting.
 * The returned MAvatarPosture can be used to setup the retargeting service.
 * The MAvatarPosture is in the MOSIM Coordinate system and not in the UE4 coordinate system.
 *
 * @param: file path to the *.mos file in the project
 * @return: MAvatarPosture containing global MJoint Positions and Rotations
 **/
MAvatarPosture AMMIAvatar::LoadAvatarPosture( FString filePath )
{
    MAvatarPosture posture;
    posture.AvatarID = this->AvatarID;
    posture.Joints = vector<MJoint>();

    FString jsonString;
    FFileHelper::LoadFileToString( jsonString, *filePath );

    TSharedPtr<FJsonObject> JsonObject = MakeShareable( new FJsonObject() );
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create( jsonString );

    std::map<std::string, int> MJointType_STRING2INT;
    std::map<int, const char*> vals = _MJointType_VALUES_TO_NAMES;

    for( int i = 0; i != MJointType::type::Root + 1; i++ )
    {
        string t = vals[i];
        MJointType_STRING2INT.insert( std::make_pair( t, i ) );
    }

    if( FJsonSerializer::Deserialize( JsonReader, JsonObject ) && JsonObject.IsValid() )
    {
        TArray<TSharedPtr<FJsonValue>> joints = JsonObject->GetArrayField( "Joints" );

        for( int32 i = 0; i < joints.Num(); i++ )
        {
            TSharedPtr<FJsonObject> jointObj = joints[i]->AsObject();
            FString id = jointObj->GetStringField( "ID" );
            // int type = jointObj->GetIntegerField( "Type" );
            string stype = string( TCHAR_TO_UTF8( *( jointObj->GetStringField( "Type" ) ) ) );
            int type = MJointType_STRING2INT[stype];

            TSharedPtr<FJsonObject> pos = jointObj->GetObjectField( "Position" );
            TSharedPtr<FJsonObject> rot = jointObj->GetObjectField( "Rotation" );

            MJoint mjoint = MJoint();
            mjoint.ID = string( TCHAR_TO_UTF8( *id ) );
            mjoint.Type = static_cast<MJointType::type>( type );

            MQuaternion q = MQuaternion();
            q.X = rot->GetNumberField( "X" ) ;
            q.Y = rot->GetNumberField( "Y" );
            q.Z = rot->GetNumberField( "Z" );
            q.W = rot->GetNumberField( "W" );

            MVector3 v = MVector3();
            v.X = pos->GetNumberField( "X" );
            v.Y = pos->GetNumberField( "Y" );
            v.Z = pos->GetNumberField( "Z" );

            mjoint.Rotation = q;
            mjoint.Position = v;

            posture.Joints.push_back( mjoint );
        }
    }

    return posture;
}

/**
 *  ReadCurrentPosture converts the current avatar posture in UE4
 *  to the MAvatarPostureValues using the Retargeting Service.
 *  This includes the transfer of coordinate systems.
 *
 *  @params: none
 *  @return: MAvatarPostureValues which can be transferred to the mosim system.
 **/
MAvatarPostureValues AMMIAvatar::ReadCurrentPosture()
{
    MAvatarPosture globalPosture = MAvatarPosture();
    globalPosture.AvatarID = this->MAvatar.ID;
    globalPosture.Joints = vector<MJoint>();
    //UE_LOG( LogTemp, Warning, TEXT( "reading" ) );
    for( int32 i = 0; i < this->GlobalReferencePosture.Joints.size(); i++ )
    {
        MJoint j = this->GlobalReferencePosture.Joints[i];

        // Create MJoint stub based on the global reference posture
        MJoint cj;
        cj.ID = j.ID;
        cj.Type = j.Type;  // This can contain Undefined joints, which are not retargeted by the
                           // retargeting service.

        if( j.ID == "_VirtualRoot" )
        {
            auto actorPos = GetActorLocation();
            actorPos.Z -= this->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
            cj.Position = ToMVec3(actorPos);
			cj.Rotation = ToMQuat(GetActorRotation().Quaternion());
        }
        else
        {
            // For every other coordinate system, we can take the joint ID and select it from the
            // avatar,
            // as the *.mos file is designed specifically for this avatar.
			FVector loc = MOSIMMesh->GetBoneLocationByName(
				FName( UTF8_TO_TCHAR( j.ID.c_str() ) ),
				EBoneSpaces::WorldSpace );
			FQuat rot = MOSIMMesh->GetBoneRotationByName(
				FName( UTF8_TO_TCHAR( j.ID.c_str() ) ),
				EBoneSpaces::WorldSpace )
				.Quaternion();

            // scaling see above.
            loc = loc / 100;

            cj.Position.X = loc.Y;
            cj.Position.Y = loc.Z;
            cj.Position.Z = loc.X;

            rot = FRotator(0, 90, 0).Quaternion() * FRotator( 0, 0, -90 ).Quaternion() * rot;

            cj.Rotation.X = -rot.X;
            cj.Rotation.Y = -rot.Y;
            cj.Rotation.Z = rot.Z;
            cj.Rotation.W = rot.W;
        }
        globalPosture.Joints.push_back( cj );
    }

    // Retarget to MOSIM skeleton. This requires that there was a SetupRetargeting before.
    return retargetingAccessPtr->RetargetToIntermediate(globalPosture);
}

/**
 *  ApplyPostureValues converts the avatarposturevalues
 *  using the Retargeting Service and applys the result to the UE4 avatar.
 *  This includes the transfer of coordinate systems.
 *
 *  @params: MAvatarPostureValues which are in the intermediate mosim format.
 *  @return: void
 **/
void AMMIAvatar::ApplyPostureValues( MAvatarPostureValues vals )
{
    if( vals.PostureData.size() == 0 )
        return;
    vals.AvatarID = this->AvatarID;
    vals.PartialJointList = vector<MJointType::type>();

    // update the posture values in the MAvatar
    this->MAvatar.__set_PostureValues( vals );

    // Retarget to global posture from MOSIM skeleton. This requires that there was a
    // SetupRetargeting before.
    MAvatarPosture globalPosture = this->retargetingAccessPtr->RetargetFromIntermediate( vals );
    //UE_LOG( LogTemp, Warning, TEXT( "applying" ) );
    for( int32 i = 0; i < globalPosture.Joints.size(); i++ )
    {
        MJoint j = globalPosture.Joints[i];
        if( j.Type != MJointType::type::Undefined )
        {
            // Only retarget joints, which are mapped by the retargeting.
            // ToDo: Non-Mapped joints should probably be interpolated.

            if( j.ID == "_VirtualRoot" )
            {
                auto actorPos = ToFVec3( j.Position );
                actorPos.Z += this->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
                FHitResult hitres;
                this->SetActorLocationAndRotation( actorPos, ToFQuat( j.Rotation ), false, &hitres,
                                                    ETeleportType::TeleportPhysics );

            }
            else
            {
				FVector loc;
                loc.X = j.Position.Z;
                loc.Y = j.Position.X;
                loc.Z = j.Position.Y;
                loc = loc * 100;

				FQuat rot;
                rot.X = -j.Rotation.X;
                rot.Y = -j.Rotation.Y;
                rot.Z = j.Rotation.Z;
                rot.W = j.Rotation.W;
                // The additional rotation is relevant! For some reason, this seems to be invariant
                // to the global orientation of the character.
                rot = FRotator( 0, -90, 0 ).Quaternion() * FRotator( 0, 0, 90 ).Quaternion() * rot;

                this->MOSIMMesh->SetBoneRotationByName( FName( UTF8_TO_TCHAR( j.ID.c_str() ) ),
                                                   rot.Rotator(), EBoneSpaces::WorldSpace );
                this->MOSIMMesh->SetBoneLocationByName( FName( UTF8_TO_TCHAR( j.ID.c_str() ) ), loc,
                                                   EBoneSpaces::WorldSpace );
            }
        }
    }
    

}

void AMMIAvatar::UpdateBaseName()
{
    string NameStr = string( TCHAR_TO_UTF8( *this->GetName() ) );

    if( regex_match( NameStr, regex( "^(.+_)[0-9]*$" ) ) )
    {
        regex regexp( "_[0-9]*$" );
        this->baseName = regex_replace( NameStr, regexp, "" );
    }
    else
        this->baseName = NameStr;
}