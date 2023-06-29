#pragma once

#include <MMIAvatarComponent.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
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

#include "Engine/SkeletalMesh.h"

#include "avatar_types.h"

// initialize static variables
int UMosimAvatar::RemoteCoSimulationAccessPortIncremented =
    MMISettings::GetInstance()->RemoteCoSimulationAccessPort;

FQuat UMosimAvatar::UE2MOSIM =
    FRotator( 0, 90, 0 ).Quaternion() * FRotator( 0, 0, -90 ).Quaternion();
FQuat UMosimAvatar::MOSIM2UE =
    FRotator( 0, -90, 0 ).Quaternion() * FRotator( 0, 0, 90 ).Quaternion();




UMosimAvatar::UMosimAvatar()
    : Super(),
    Timeout( 1 ),
    RemoteCoSimulationAccessPort( 0 ),
    AvatarID( string( "" ) ), sessionID( string( "" ) ), baseName( string( "" ) ),
    statusText( string( "" ) ), MMUAccessPtr( nullptr ), SceneAccess( nullptr ),
    SimController( nullptr ), skeletonAccessPtr( nullptr ), running( true ),
    retargetingAccessPtr( nullptr ), isInitialized( false )
{
    for( int i = 0; i < this->nJoints; i++ )
    {
        this->_MJointType_VALUES_TO_NAMES.insert_or_assign( i, _kMJointTypeNames[i] );
    }
    // set the base name (name of the Avatar without automatic extensions by Unreal fro making the
    // Name unique (e.g "_2"))
    // applied in the AvatarBehavior class for looking for the behavior
    this->UpdateBaseName();

    // this->MOSIMMesh =
    //    CreateDefaultSubobject<UPoseableMeshComponent>( TEXT( "CharacterPoseableMesh" ) );
    // this->MOSIMMesh->AttachToComponent(GetCapsuleComponent(),
    // FAttachmentTransformRules::KeepRelativeTransform);

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
    PrimaryComponentTick.bCanEverTick = true;

    // Add the poseable mesh component. This will appear as an empty component, which has to be
    // configured by the user when making a new blueprint.
    // this->MOSIMMesh = CreateDefaultSubobject<UPoseableMeshComponent>( TEXT( "Mesh" ) );
    // this->MOSIMMesh->SetHiddenInGame( false, true );

    // set the collison for the whole actor
    // this->SetActorEnableCollision( true );
}

void UMosimAvatar::BeginPlay()
{
    Super::BeginPlay();
}

void UMosimAvatar::EndPlay( const EEndPlayReason::Type EndPlayReason )
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

void UMosimAvatar::OnComponentDestroyed( bool bDestroyingHierarchy )
{
    Super::OnComponentDestroyed( bDestroyingHierarchy );
}

bool UMosimAvatar::Setup( MIPAddress registerAddress, string _sessionID,
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
    this->RemoteCoSimulationAccessPort = UMosimAvatar::RemoteCoSimulationAccessPortIncremented++;

    // load the reference posture
    try
    {
        // Loading the reference zero posture.
        MAvatarPosture zeroP =
            this->LoadAvatarPosture( FPaths::ProjectContentDir() + ReferencePostureFile );
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
        string message = "UMosimAvatar: Failed to access the skeleton service!";
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
        string message = "UMosimAvatar: Failed to connect to MMUs!";
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
                string( "AccessPort" ), to_string( this->RemoteCoSimulationAccessPort ) } );

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

// Called every frame
void UMosimAvatar::TickComponent( float DeltaTime, enum ELevelTick TickType,
                                  FActorComponentTickFunction* ThisTickFunction )
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

    Super::TickComponent( DeltaTime, TickType, ThisTickFunction);

    // Get the posture values of the current posture
    this->MAvatar.PostureValues = this->ReadCurrentPosture();

    // Notify the Unreal Scene Access scene updates about the update
    this->SceneAccess->PostureValuesChanged_SceneUpdate( this->MAvatar,
                                                         this->MAvatar.PostureValues );
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
MAvatarPosture UMosimAvatar::LoadAvatarPosture( FString filePath )
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
            q.X = rot->GetNumberField( "X" );
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
MAvatarPostureValues UMosimAvatar::ReadCurrentPosture()
{
    MAvatarPosture globalPosture = MAvatarPosture();
    globalPosture.AvatarID = this->MAvatar.ID;
    globalPosture.Joints = vector<MJoint>();

    FPoseSnapshot snap = this->GetSnapshot();
    ACharacter* owner = (ACharacter*)GetOwner();

    // make snap global
    USkeleton* skel = owner->GetMesh()->SkeletalMesh->Skeleton;
    for( int i = 0; i < snap.LocalTransforms.Num(); i++ )
    {
        TArray<int32> children;
        skel->GetChildBones( i, children );
        for( int c = 0; c < children.Num(); c++ )
        {
            FTransform out; 
            FTransform::Multiply( &out, &snap.LocalTransforms[i], &snap.LocalTransforms[c] );
            snap.LocalTransforms[c] = out;
        }
    }


    // UE_LOG( LogTemp, Warning, TEXT( "reading" ) );
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
            auto actorPos = owner->GetActorLocation();
            actorPos.Z -= owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
            cj.Position = ToMVec3( actorPos );
            cj.Rotation = ToMQuat( owner->GetActorRotation().Quaternion() );
        }
        else
        {
            // For every other coordinate system, we can take the joint ID and select it from the
            // avatar,
            // as the *.mos file is designed specifically for this avatar.
            FVector loc;
            FQuat rot;
            int ueJID = owner->GetMesh()->GetBoneIndex(FName( UTF8_TO_TCHAR( j.ID.c_str() )));
            loc = snap.LocalTransforms[i].GetLocation();
            rot = snap.LocalTransforms[i].GetRotation();
            /*
            FVector loc = MOSIMMesh->GetBoneLocationByName( FName( UTF8_TO_TCHAR( j.ID.c_str() ) ),
                                                            EBoneSpaces::WorldSpace );
            FQuat rot = MOSIMMesh
                            ->GetBoneRotationByName( FName( UTF8_TO_TCHAR( j.ID.c_str() ) ),
                                                     EBoneSpaces::WorldSpace )
                            .Quaternion();
            */
            // scaling see above.
            loc = loc / 100;

            cj.Position.X = loc.Y;
            cj.Position.Y = loc.Z;
            cj.Position.Z = loc.X;

            rot = UMosimAvatar::UE2MOSIM * rot;

            cj.Rotation.X = -rot.X;
            cj.Rotation.Y = -rot.Y;
            cj.Rotation.Z = rot.Z;
            cj.Rotation.W = rot.W;
        }
        globalPosture.Joints.push_back( cj );
    }

    // Retarget to MOSIM skeleton. This requires that there was a SetupRetargeting before.
    return retargetingAccessPtr->RetargetToIntermediate( globalPosture );
}

/**
 *  ApplyPostureValues converts the avatarposturevalues
 *  using the Retargeting Service and applys the result to the UE4 avatar.
 *  This includes the transfer of coordinate systems.
 *
 *  @params: MAvatarPostureValues which are in the intermediate mosim format.
 *  @return: void
 **/
void UMosimAvatar::ApplyPostureValues( MAvatarPostureValues vals )
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
    // UE_LOG( LogTemp, Warning, TEXT( "applying" ) );

    FPoseSnapshot snap = this->GetSnapshot();
    ACharacter* owner = (ACharacter*)GetOwner();

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
                actorPos.Z += owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
                FHitResult hitres;
                owner->SetActorLocationAndRotation( actorPos, ToFQuat( j.Rotation ), false, &hitres,
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
                rot = UMosimAvatar::MOSIM2UE * rot;
                int ueJID = owner->GetMesh()->GetBoneIndex( FName( UTF8_TO_TCHAR( j.ID.c_str() ) ) );
                snap.LocalTransforms[ueJID].SetLocation( loc );
                snap.LocalTransforms[ueJID].SetRotation( rot );
                /*
                this->MOSIMMesh->SetBoneRotationByName( FName( UTF8_TO_TCHAR( j.ID.c_str() ) ),
                                                        rot.Rotator(), EBoneSpaces::WorldSpace );
                this->MOSIMMesh->SetBoneLocationByName( FName( UTF8_TO_TCHAR( j.ID.c_str() ) ), loc,
                                                        EBoneSpaces::WorldSpace );
                                                        */
            }

        }
    }
    // TODO: Make Snap joint local by traversing from child to parent bones.
    this->SetSnapshot( snap );
}

bool UMosimAvatar::IsInitialized()
{
    return this->isInitialized;
}


FString UMosimAvatar::GetAvatarID()
{
    return this->AvatarID.c_str();
}

void UMosimAvatar::SetAvatarID( FString id )
{
    this->AvatarID = std::string( TCHAR_TO_UTF8( *id ) );
    this->MAvatar.ID = this->AvatarID;
    this->AvatarIDVis = this->AvatarID.c_str();
    this->MAvatar.Description.AvatarID = this->AvatarID;
    this->MAvatar.Description.ZeroPosture.AvatarID = this->AvatarID;
}

/*
void UMosimAvatar::SetBehavior( UAvatarBehavior* b )
{
    this->behavior = b;
}
*/


void UMosimAvatar::UpdateBaseName()
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