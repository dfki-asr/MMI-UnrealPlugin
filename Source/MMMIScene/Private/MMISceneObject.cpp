// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// This class is the representation of the MSceneObject in the Unreal Engine.
// Allows the user to mark AActors as MSceneObject.
// Makes the required properties of the parent AActor (Root Transform, Colliders, Physics) known
// to the MOSIM framework and transforms them to the format required by MOSIM.

#include "MMISceneObject.h"
#include "MOSIM.h"
#include "UnrealSceneAccess.h"
#include "MMUAccess.h"
#include <regex>
#include <cmath>
#include <algorithm>
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "Utils/Logger.h"
#include "EngineUtils.h"
#include "CommonTools.h"
//Used for culture invariant float to string conversions.
#include<boost/lexical_cast.hpp>

using namespace std;
using namespace MMIStandard;

// Sets default values for this component's properties
UMMISceneObject::UMMISceneObject()
    : UpdatePhysicsCurrentFrame( false ),
      taskEditorID( 0 ),
      localID( 0 ),
      ParentActor( nullptr ),
      rootComp( nullptr ),
      SceneAccess( nullptr ),
      BodyInstance( nullptr ),
      RootPrimitive( nullptr ),
      scene( nullptr ),
      mass( 0.f ),
      colliderSet( false ),
      lastGlobalPosition(),
      lastGlobalRotation(),
      rootThreshold( 0.1f ),
      physicsUpdateCounter( 0 ),
      lastVelocity(),
      lastAngularVelocity(),
      lastCenterOfMass(),
      lastInertia(),
      lastMass( 0.f ),
      physicsThreshold( 0.1f ),
      isInitialized( false ),
	  skeletalMesh(nullptr),
      hasParentMMISceneObject( false ),
      rootIsPrimitive( false )
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You
    // can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // update the component during every tick
    PrimaryComponentTick.SetTickFunctionEnable( true );

    // get the root component of the top level actor
    ParentActor = GetAttachmentRootActor();
    if( ParentActor != nullptr )
    {
		skeletalMesh = ParentActor->FindComponentByClass<USkeletalMeshComponent>();

        // use the parents root component for updating the physics, colliders, etc.
        rootComp = ParentActor->GetRootComponent();
        if( rootComp == nullptr )
        {
            UE_LOG( LogMOSIM, Warning, TEXT( "Owner has no root component." ) );
        }

        // check, whether the owner has a primitive component
        ParentActor->GetComponents<UPrimitiveComponent>( PrimitiveComponents );
        // TODO: refering to the first found primitive component as the primitive component of the
        // root component is prone to errors, however, it seems to work so far
        // TODO: attempt to exclude by refering to the wrong primitive component as the root
        // components primitive component by throwing an error if the AActor has more than one
        // primitive component
        // if (PrimitiveComponents.Num() > 1) { UE_LOG(LogMOSIM, Warning, TEXT("Marked SceneObject
        // has more than one primitive component. The UE-MOSIM plugin is currently not able to
        // handle this.")); }

		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(this->rootComp);
		if (PrimitiveComponent)
		{
			rootIsPrimitive = true;
			// TODO: actually, the BodyInstance should be derived from the Primitive Component from the
			// following line and not from PrimitiveComponents[0], but unfortunately, when queries are
			// made to the BodyInstance, only nonsense is returned e.g. for the mass, and the other
			// properties.
			// FName ComponentTypeName = PrimitiveComponent->GetFName();
			// RootPrimitive = PrimitiveComponent;
			// BodyInstance = PrimitiveComponent->GetBodyInstance();
			// UE_LOG(LogMOSIM, Warning, TEXT("Result Cast %s"), *ComponentTypeName.ToString());
			RootPrimitive = PrimitiveComponents[0];
			BodyInstance = PrimitiveComponents[0]->GetBodyInstance();
			// ComponentTypeName = PrimitiveComponents[0]->GetFName();
			// UE_LOG(LogMOSIM, Warning, TEXT("Ergebnis Array %s"), *ComponentTypeName.ToString());
		}
		else
			UE_LOG(LogMOSIM, Warning,
				TEXT("Root Component of the MMISceneObject has to be a UPrimitiveComponent."));
    }
    else
    {
        UE_LOG( LogMOSIM, Warning, TEXT( "MMISceneObject component is not set, has no owner." ) );
        // runtime_error("Component is not set, has no owner.");
    }

    // set the collison for the whole actor
    if( ParentActor != nullptr )
    {
        ParentActor->SetActorEnableCollision( true );
    }

    // set the MMISceneObject Name (has to be unique, therefore actor Name)
    // and the MSceneObject Name (not necessarily unique), has to be equal to the actor label
    if( ParentActor != nullptr )
    {
        this->SetupBase();
    }

    // Create a new UUID for the MSceneObject
    this->MSceneObject.__set_ID( MMUAccess::GetNewGuid() );
    this->MSceneObject.Properties.insert_or_assign( "type", this->GetSceneObjectType() );
}

UMMISceneObject::~UMMISceneObject()
{
    // unregister the SceneObject at the UnrealSceneAccess
    if( this->SceneAccess )
    {
        this->SceneAccess->RemoveSceneObject_SceneUpdate( this->MSceneObject );
        this->SceneAccess->RemoveMMISceneObject( this->MSceneObject.ID );
    }
}

// Called when the game starts
void UMMISceneObject::BeginPlay()
{
    Super::BeginPlay();
}

void UMMISceneObject::Setup( UnrealSceneAccess* _SceneAccess )
{
    // do the base settings for the setup
    this->SetupBase();

    switch( this->SceneObjectType )
    {
        case SceneObjectType::Tool:
            // switch out unreal physics interactions
            for( UPrimitiveComponent* primiComp : this->PrimitiveComponents )
            {
                primiComp->SetSimulatePhysics( true );
                primiComp->SetEnableGravity( false );
            }
            break;
        case SceneObjectType::WalkTarget:
            break;
        default:
            this->SetupCollider();
            break;
    }
    this->MSceneObject.Properties.insert_or_assign( "type", this->GetSceneObjectType ());

    // get access to the UnrealSceneAccess
    this->SceneAccess = _SceneAccess;

    // Add the scene object to the scene access
    this->SceneAccess->AddSceneObject_SceneUpdate( this->MSceneObject );

    this->isInitialized = true;
}

void UMMISceneObject::SetupBase()
{
    // set the transform of the MSceneObject
    this->SetupTransform();

    // update the names of the object
    this->updateNames();

    // set the physics, update the root transform history and the physics history
    if( this->ParentActor )
    {
        this->SetupPhysics();
        this->UpdateRootHistory();
        this->UpdatePhysicsHistory();
    }
}

// Called every frame
void UMMISceneObject::TickComponent( float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction )
{
    if( !this->isInitialized )
        return;

    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    if( this->HasRootChanged() )
    {
        // sequence of the root comp update
        // UnrealSceneAccess writes into the MSceneObject.Transform and passes the values to the
        // root component of the owner AActor
        // via calling MMISceneObject::UpdateRootComponent()
        // Here, the MSceneObject.Transform is updated according to the owner AActors root
        // component, when something has changed.
        // No matter, whether the engine or MOSIM changed the owners root transform

        // update the MSceneObject
        this->UpdateMSceneObjTransform( this->MSceneObject.Transform );

        // notify unreal scene access about the changed transformation
        if( this->SceneAccess )
            this->SceneAccess->TransformationChanged_SceneUpdate(
                this->MSceneObject, this->MSceneObject.Transform.Position,
                this->MSceneObject.Transform.Rotation, this->MSceneObject.Transform.Parent );
        else
            UE_LOG( LogMOSIM, Warning,
                    TEXT( "Scene Access in %s is not initialized. Simulation is not executable." ),
                    UTF8_TO_TCHAR( this->Name.c_str() ) );

        // update the root component history
        this->UpdateRootHistory();
    }

    // sequence of the physics update
    // UnrealSceneAccess writes into the MSceneObject.PhysicsProperties and passes the values to the
    // owner AActor via calling MMISceneObject::UpdatePhysics(physicsInteraction)
    // Here, the MSceneObject.PhysicsProperties is updated acording to the owner AActors physics,
    // when something has changed
    // No matter, whether the engine or MOSIM changed the owners physics

    if( this->HasPhysicsChanged() )
    {
        // update the MSceneObjects physics according to the owner AActors physics
        this->SetupPhysics();

        // update the physics history
        this->UpdatePhysicsHistory();

        // notify unreal scene access about the changed transformation
        this->SceneAccess->PhysicsPropertiesChanged_SceneUpdate(
            this->MSceneObject, this->MSceneObject.PhysicsProperties );
    }
}

// update root component (location of the Actor/SceneObject)
void UMMISceneObject::UpdateRootComponent()
{
    // the virtual root maps to the actor location / rotation. +scale
    this->ParentActor->SetActorLocation(ToFVec3(MSceneObject.Transform.Position));
    this->ParentActor->SetActorRotation(ToFQuat(MSceneObject.Transform.Rotation));
    this->ParentActor->SetActorScale3D(ToFVec3(MSceneObject.Transform.Scale));
}

// update the physics of the object
void UMMISceneObject::UpdatePhysics( const MPhysicsInteraction& physicsInteraction )
{
    if( !this->rootIsPrimitive || !this->BodyInstance )
        return;

    this->UpdatePhysicsCurrentFrame = true;

    // Apply the manipulation
    switch( physicsInteraction.Type )
    {
        case MPhysicsInteractionType::AddForce:
        {
            BodyInstance->AddForce(
				ToFVec3(MSceneObject.PhysicsProperties.NetForce),
				true, true );
            break;
        }
        case MPhysicsInteractionType::AddTorque:
        {
			//ToDo: why torque is scaled? Should be sent as it is (with multiplier = 1).
            BodyInstance->AddTorqueInRadians(
				ToFVec3(MSceneObject.PhysicsProperties.NetTorque),
				true, true );
            break;
        }
        case MPhysicsInteractionType::ChangeAngularVelocity:
        {
			//ToDo: why torque is scaled? Should be sent as it is (with multiplier = 1).
            BodyInstance->AddTorqueInRadians(
				ToFVec3(MSceneObject.PhysicsProperties.AngularVelocity),
				true, true );
            break;
        }
        case MPhysicsInteractionType::ChangeCenterOfMass:
        {
            FVector comNew = ToFVec3(MSceneObject.PhysicsProperties.CenterOfMass);
            FVector comOld = BodyInstance->GetCOMPosition();
            RootPrimitive->SetCenterOfMass(comOld - comNew, FName());
            break;
        }
        case MPhysicsInteractionType::ChangeInertia:
        {
            // change of the inertia tensor is actually unnecessary
            // should be done either by the bodys shape or by the mass distribution, not directly

			/*this->BodyInstance->SomethingInertiaMagic(
				ToFVec3(MSceneObject.PhysicsProperties.Inertia),
				something, something);*/
            UE_LOG( LogMOSIM, Warning,
                    TEXT( "Change of the inertia tensor is not implemented so far." ) );
            break;
        }
        case MPhysicsInteractionType::ChangeMass:
        {
            // TODO: check, if the commented lines are necessary
            // this->BodyInstance->bOverrideMass = true;
            this->BodyInstance->SetMassOverride( this->MSceneObject.PhysicsProperties.Mass );
            // this->BodyInstance->UpdateMassProperties();
            break;
        }
        case MPhysicsInteractionType::ChangeVelocity:
        {
            RootPrimitive->AddImpulse(
				ToFVec3(MSceneObject.PhysicsProperties.Velocity),
				FName(), true );
            break;
        }
    }
}

void UMMISceneObject::UpdateMSceneObjTransform( MTransform& transform )
{
	transform.Position = ToMVec3(ParentActor->GetActorLocation());
	transform.Rotation = ToMQuat(ParentActor->GetActorRotation().Quaternion());
	//ToDo: Add Scale MVector in MTransform and remove "ScaleXYZ" below in SetupCollider
	transform.Scale =  ToMVec3(ParentActor->GetActorScale(), 1);
}

void UMMISceneObject::SetupTransform()
{
    // Create MTransform stub based on the global reference posture
    MTransform transform;
    transform.ID = this->MSceneObject.ID;

    // set the transform according to the current position
    this->UpdateMSceneObjTransform( transform );
    this->MSceneObject.__set_Transform( transform );

    // set the parent, if available
    this->SetParent();
}

void UMMISceneObject::SetParent()
{
    // set the parent if available
    USceneComponent* ParentComponent = this->GetAttachParent();
    int counter = 0;

    while( ParentComponent != nullptr )
    {
        if( counter > 0 )
            ParentComponent = ParentComponent->GetAttachParent();

        UMMISceneObject* parentMMISceneObject = Cast<UMMISceneObject>( ParentComponent );
        if( parentMMISceneObject != nullptr )
        {
            this->MSceneObject.Transform.__set_Parent( parentMMISceneObject->MSceneObject.ID );
            this->MSceneObject.Transform.__isset.Parent = true;
            this->hasParentMMISceneObject = true;
            break;
        }
        else
        {
            counter++;
            continue;
        }
    }
}

void UMMISceneObject::ChangeParent( string newParentID )
{
    vector<UMMISceneObject*> mmiSceneObjects = this->SceneAccess->GetMMISceneObjectsVector();
    bool found = false;
    for( UMMISceneObject* mmiSceneObject : mmiSceneObjects )
    {
        if( mmiSceneObject->MSceneObject.ID == newParentID )
        {
            this->MSceneObject.Transform.__set_Parent( newParentID );
            found = true;
        }
    }
    if( !found )
    {
        string message = "New Parent UMMISceneObject with name " + newParentID + " not found.";
        UE_LOG( LogMOSIM, Warning, TEXT( "%s" ), *FString( message.c_str() ) );
        Logger::printLog( L_ERROR, message );
    }
}

bool UMMISceneObject::setupBoxCollider(MCollider& mCollider)
{
	UBoxComponent* boxCollider = ParentActor->FindComponentByClass<UBoxComponent>();
	
	if (boxCollider != nullptr)
	{
		mCollider.__set_Type(MColliderType::Box);

		MBoxColliderProperties boxCollProps;
		boxCollProps.__set_Size(ToMVec3(2 * boxCollider->GetUnscaledBoxExtent()));
		mCollider.__set_BoxColliderProperties(boxCollProps);
		mCollider.__isset.BoxColliderProperties = true;

		mCollider.__set_PositionOffset(ToMVec3(boxCollider->GetRelativeLocation()));
		mCollider.__isset.PositionOffset = true;

		return true;
	}
	return false;
}

bool UMMISceneObject::setupSphereCollider(MCollider& mCollider)
{
	USphereComponent* sphereCollider = ParentActor->FindComponentByClass<USphereComponent>();
	if (sphereCollider != nullptr)
	{
		mCollider.__set_Type(MColliderType::Sphere);

		MSphereColliderProperties sphereCollProps;
		// Unreal multiplies sphere radius with the smallest scale component
		// Thus to sent the actual value, need to scale the radius down
		sphereCollProps.__set_Radius(0.01 * ParentActor->GetActorScale().GetAbsMin()
			* sphereCollider->GetUnscaledSphereRadius());
		mCollider.__set_SphereColliderProperties(sphereCollProps);
		mCollider.__isset.SphereColliderProperties = true;

		mCollider.__set_PositionOffset(ToMVec3(sphereCollider->GetRelativeLocation()));
		mCollider.__isset.PositionOffset = true;

		return true;
	}
	return false;
}

bool UMMISceneObject::setupCapsuleCollider(MCollider& mCollider)
{
	UCapsuleComponent* capsuleCollider = ParentActor->FindComponentByClass<UCapsuleComponent>();
	if (capsuleCollider != nullptr)
	{
		mCollider.__set_Type(MColliderType::Capsule);

		MCapsuleColliderProperties capsuleCollProps;
		capsuleCollProps.__set_Radius(0.01 * capsuleCollider->GetUnscaledCapsuleRadius());
		capsuleCollProps.__set_Height(0.02 * capsuleCollider->GetUnscaledCapsuleHalfHeight());
		// TODO: check, if there is a main axis
		//capsuleCollProps.__set_MainAxis(boxExtentMVect);
		mCollider.__set_CapsuleColliderProperties(capsuleCollProps);
		mCollider.__isset.CapsuleColliderProperties = true;

		mCollider.__set_PositionOffset(ToMVec3(capsuleCollider->GetRelativeLocation()));
		mCollider.__isset.PositionOffset = true;

		return true;
	}
	return false;
}

bool UMMISceneObject::setupStaticMeshCollider(MCollider& mCollider)
{
	UStaticMeshComponent* meshCollider = ParentActor->FindComponentByClass<UStaticMeshComponent>();
	if (meshCollider != nullptr)
	{
		mCollider.__set_Type(MColliderType::Mesh);
		// get the colliders properties
		FTriMeshCollisionData triMeshData;
		bool hasPhysicsData = meshCollider->GetStaticMesh()->GetPhysicsTriMeshData(&triMeshData, true);
		if (hasPhysicsData)
		{
			vector<MVector3> vertices;
			for (FVector& v : triMeshData.Vertices)
				vertices.push_back(ToMVec3(v));

			vector<int> triangles;
			// For some reason, the inverse triangle order is neccessary
			for (FTriIndices& t : triMeshData.Indices)
				triangles.insert(triangles.end(), { t.v0, t.v2 , t.v1 });

			MMeshColliderProperties meshCollProps;
			meshCollProps.__set_Vertices(vertices);
			meshCollProps.__set_Triangles(triangles);
			mCollider.__set_MeshColliderProperties(meshCollProps);
			mCollider.__isset.MeshColliderProperties = true;

			mCollider.__set_PositionOffset(ToMVec3(meshCollider->GetRelativeLocation()));
			mCollider.__isset.PositionOffset = true;

			return true;
		}
	}
	return false;
}

bool UMMISceneObject::setupSkeletalMeshCollider(MCollider& mCollider)
{
	if (skeletalMesh != nullptr)
	{
		//Currently, only box extent is transfered. There is no opportunity nor need to send the sphere data.
		auto boxSphereBounds = skeletalMesh->SkeletalMesh->GetImportedBounds();

		mCollider.__set_Type(MColliderType::Box);

		MBoxColliderProperties boxCollProps;
		boxCollProps.__set_Size(ToMVec3(2 * boxSphereBounds.BoxExtent));
		mCollider.__set_BoxColliderProperties(boxCollProps);
		mCollider.__isset.BoxColliderProperties = true;

		mCollider.__set_PositionOffset(ToMVec3(boxSphereBounds.Origin));
		mCollider.__isset.PositionOffset = true;
		return true;

	}
	return false;
}

void UMMISceneObject::SetupCollider()
{
    MCollider collider;
    collider.__set_ID( this->MSceneObject.ID );

	// skeletalMesh is already inspected, so try setupping it first
	if (setupSkeletalMeshCollider(collider) ||
		// go down to other collider types, if previous did not succeed...
		setupBoxCollider(collider) ||
		setupSphereCollider(collider) ||
		setupCapsuleCollider(collider) ||
		// ...and use the mesh collider as last resort
		setupStaticMeshCollider(collider))
	{
		this->MSceneObject.__set_Collider(collider);
		this->MSceneObject.__isset.Collider = true;
		this->colliderSet = true;
	}

	if (this->colliderSet && AreaType != NavMeshArea::None)
	{
		MSceneObject.Collider.Properties["Area"] =
			AreaType == NavMeshArea::NotWalkable
			? "Not Walkable" // The only case with separated words in area name
			: TCHAR_TO_UTF8(*UEnum::GetValueAsString<NavMeshArea>(
				AreaType.GetValue()));
		this->MSceneObject.Collider.__isset.Properties = true;
	}

	//ToDo: Remove once Scale is added to MTransform
	//ToDo: Change the sign for actorScale.X once coordinate system is fixed
        
	MVector3 actorScale = ToMVec3(ParentActor->GetActorScale(), 1);
	MSceneObject.Collider.Properties["ScaleX"] = boost::lexical_cast<std::string>(-actorScale.X);
    MSceneObject.Collider.Properties["ScaleY"] = boost::lexical_cast<std::string>( (double)actorScale.Y );
    MSceneObject.Collider.Properties["ScaleZ"] = boost::lexical_cast<std::string>( (double)actorScale.Z );
    
	this->MSceneObject.Collider.__isset.Properties = true;
}

void UMMISceneObject::SetupPhysics()
{
    MPhysicsProperties physProps = MPhysicsProperties();

    if( this->rootIsPrimitive && this->BodyInstance )
    {
        this->mass = this->BodyInstance->GetBodyMass();

        // set the massoverride according to the total mass in the first call of the method
        if( this->physicsUpdateCounter == 0 )
            this->BodyInstance->SetMassOverride( mass, true );

        physProps.__set_Mass( this->mass );

        physProps.__set_CenterOfMass(ToDVec(BodyInstance->GetCOMPosition()));

        //ToDo: is in local mass space --> whatever that is, clarify, what MOSIM expects
        //kgcm^2 -> kgm^2
		//ToDo: check if conversion of inertia is right
        physProps.__set_Inertia(ToDVec(BodyInstance->GetBodyInertiaTensor(), 0.0001));

        //ToDo: is in global coordinates, clarify, what MOSIM expects
		//ToDo: why angular velocity is scaled? Should be sent as it is (with multiplier = 1)
        physProps.__set_AngularVelocity(
			ToDVec(BodyInstance->GetUnrealWorldAngularVelocityInRadians()));
    }

    // TODO: unclear, if this code is really neccessary, returns currently the same value as above
    if(skeletalMesh != nullptr)
    {
		physProps.__set_CenterOfMass(
			ToDVec(skeletalMesh->GetSkeletalCenterOfMass()));
    }

    physProps.__set_Velocity(
		ToDVec(ParentActor->GetVelocity()));

    // add the physics properties to the MSceneObject
    this->MSceneObject.__set_PhysicsProperties( physProps );
    this->MSceneObject.__isset.PhysicsProperties = true;
}

// Update the MSceneObject Name and set the Name of the MMISceneObject accordingly
void UMMISceneObject::setMSceneObjectName( string _Name )
{
    this->MSceneObject.__set_Name( _Name );

    // convert the name to an FString
    FString NameFStr( _Name.c_str() );
    FString NameFStrAdapted( "" );
    FString NameFStrAdaptedUnderscore( "" );
    bool nameAdapted = false;

    // find a unique name
    this->scene = this->GetWorld();

    // get a list of all names of actors in the scene
    vector<FString> actorNames = vector<FString>();
    for( TActorIterator<AActor> AActorItr( this->scene ); AActorItr; ++AActorItr )
    {
        actorNames.push_back( AActorItr->GetName() );
    }

    // check, whether the base name is already unique
    if( find( actorNames.begin(), actorNames.end(), NameFStr ) != actorNames.end() )
    {
        int index = 2;
        for( int i = 0; actorNames.size(); i++ )
        {
            NameFStrAdapted = NameFStr + FString::FromInt( index );
            NameFStrAdaptedUnderscore = NameFStr + "_" + FString::FromInt( index );
            if( find( actorNames.begin(), actorNames.end(), NameFStrAdapted ) != actorNames.end() ||
                find( actorNames.begin(), actorNames.end(), NameFStrAdaptedUnderscore ) !=
                    actorNames.end() )
                index++;
            else
                break;
        }
        this->ParentActor->Rename( *NameFStrAdapted );

        #if WITH_EDITOR
        this->ParentActor->SetActorLabel( *NameFStr );  // defines label presented in the engine
        #endif
        
    }
    else
    {
        this->ParentActor->Rename( *NameFStr );

        #if WITH_EDITOR
        this->ParentActor->SetActorLabel( *NameFStr );
        #endif
        
    }

    // set the name field according to the FString name of the object (to get the unique name)
    FString NameFStrReturn = ParentActor->GetName();
    // set the string version of the name according to the name of the parent object
    this->Name = string( TCHAR_TO_UTF8( *NameFStrReturn ) );
}

string UMMISceneObject::getNameStr()
{
    // if the MMISceneObject is the topmost object in the hierarchy,
    // give it the parents component name
    // otherwise give it the SceneComponents name
    if( !this->hasParentMMISceneObject )
    {
        // set the Name of the MMISceneObject according to the name of the parent object
        // TODO: ActorLabel returns strange values
        // FString LabelNameFStr = ParentActor->GetActorLabel();
        FString NameFStr = ParentActor->GetName();
        // set the string version of the name according to the name of the parent object
        // string _LabelName = string(TCHAR_TO_UTF8(*LabelNameFStr));
        string _Name = string( TCHAR_TO_UTF8( *NameFStr ) );
        return _Name;
    }
    else
    {
        FString NameFStr = this->GetName();
        string _Name = string( TCHAR_TO_UTF8( *NameFStr ) );
        return _Name;
    }
}

void UMMISceneObject::updateNames()
{
    // first set the MMISceneObject name
    this->Name = this->getNameStr();
    // set the name according to the Parents Name and the MMISceneObject Name (without the digits at
    // the end identifying copies of actors in Unreal)
    // check if the name contains an underscore followed by digits

    if( regex_match( this->Name, regex( "^(.+_)[0-9]*$" ) ) )
    {
        regex regexp( "_[0-9]*$" );
        string mSceneObjName = regex_replace( this->Name, regexp, "" );
        this->MSceneObject.__set_Name( mSceneObjName );
    }
    else
        this->MSceneObject.__set_Name( this->Name );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// methods to track the transform of the root component

// Indicates whether changes occured since the last frame
bool UMMISceneObject::HasRootChanged()
{
    if(FVector::Dist(lastGlobalPosition, ParentActor->GetActorLocation()) > rootThreshold )
        return true;

    float globalRotationAngle = ParentActor->GetActorRotation().Quaternion().GetAngle();
    if( abs((lastGlobalRotation.GetAngle() - globalRotationAngle) ) > rootThreshold )
		return true;

    // check, if the parent has changed
    if( this->hasParentMMISceneObject )
    {
        vector<UMMISceneObject*> MMISceneObjects = SceneAccess->GetMMISceneObjectsVector();
        vector<UMMISceneObject*>::iterator it = find_if(
            MMISceneObjects.begin(), MMISceneObjects.end(), [this]( UMMISceneObject* object ) {
                return object->MSceneObject.ID == this->MSceneObject.Transform.Parent;
            } );
        if( it != MMISceneObjects.end())
        {
            UMMISceneObject* parentMMISceneObject = *it;

            if(FVector::Dist(
				parentMMISceneObject->lastGlobalPosition,
                parentMMISceneObject->ParentActor->GetActorLocation()) > rootThreshold )
                return true;

            float globalParentRotationAngle =
                parentMMISceneObject->ParentActor->GetActorRotation().Quaternion().GetAngle() ;
            if( abs((parentMMISceneObject->lastGlobalRotation.GetAngle() -
				globalParentRotationAngle) ) > rootThreshold )
				return true;
        }
        else
        {
            string message =
                "Could not find Parent MMISceneObject of Object " + this->MSceneObject.Name + "!";
            UE_LOG( LogMOSIM, Warning, TEXT( "%s" ), *FString( message.c_str() ) );
            Logger::printLog( L_INFO, message );
        }
    }

    return false;
}

// Updates the internal state according to the specified transform
void UMMISceneObject::UpdateRootHistory()
{
    // Update the transform in each frame
    this->lastGlobalPosition = this->ParentActor->GetActorLocation();
    this->lastGlobalRotation = this->ParentActor->GetActorRotation().Quaternion();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// methods to track the changes of the physical properties

// updates the physics history
void UMMISceneObject::UpdatePhysicsHistory()
{
    if( this->rootIsPrimitive && this->BodyInstance )
    {
        this->lastAngularVelocity = this->BodyInstance->GetUnrealWorldAngularVelocityInRadians();
        this->lastCenterOfMass = this->BodyInstance->GetCOMPosition();
        this->lastInertia = this->BodyInstance->GetBodyInertiaTensor();

        if( this->physicsUpdateCounter == 0 )
        {
            this->lastMass = this->BodyInstance->GetBodyMass();
            float massCompare = this->BodyInstance->GetMassOverride();
        }
        else
        {
            //ToDo: check, which mass is the one, that gets actually changed
            this->lastMass = this->BodyInstance->GetMassOverride();
            // this->lastMass = this->BodyInstance->GetBodyMass();
        }
    }

    this->lastVelocity = this->ParentActor->GetVelocity();

    this->physicsUpdateCounter++;
}

bool UMMISceneObject::HasPhysicsChanged()
{
    if( this->rootIsPrimitive && this->BodyInstance )
    {
		if (FVector::Dist(
			lastAngularVelocity,
			BodyInstance->GetUnrealWorldAngularVelocityInRadians()) > physicsThreshold)
			return true;

		if (FVector::Dist(
			lastCenterOfMass,
			BodyInstance->GetCOMPosition()) > physicsThreshold)
			return true;

		if(FVector::Dist(
			lastInertia,
			BodyInstance->GetBodyInertiaTensor()) > physicsThreshold)
			return true;

        if( this->physicsUpdateCounter == 0 )
        {
            if( abs(lastMass - BodyInstance->GetBodyMass() ) > physicsThreshold )
				return true;
        }
        else
        {
			//ToDo: Why newMass is not used?
            //float newMass = this->BodyInstance->GetMassOverride();
            float diffMass = abs( this->lastMass - this->BodyInstance->GetBodyMass() );
            if( diffMass > physicsThreshold)
				return true;
        }
    }

    if(FVector::Dist(lastVelocity, ParentActor->GetVelocity()) > physicsThreshold)
		return true;

    return false;
}

string UMMISceneObject::GetSceneObjectType()
{
    switch( this->SceneObjectType.GetValue() )
    {
        case SceneObjectType::Area:
            return "Area";
        case SceneObjectType::FinalLocation:
            return "FinalLocation";
        case SceneObjectType::Group:
            return "Group";
        case SceneObjectType::InitialLocation:
            return "InitialLocation";
        case SceneObjectType::Part:
            return "Part";
        case SceneObjectType::SceneObject:
            return "SceneObject";
        case SceneObjectType::Tool:
            return "Tool";
        case SceneObjectType::WalkTarget:
            return "WalkTarget";
        default:
            return "None";
    }
}
