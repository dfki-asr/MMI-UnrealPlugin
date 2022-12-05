// SPDX-License-Identifier: MIT
// The content of this file has been developed in the context of the MOSIM research project.
// Original author(s): Stephan Adam

// This class is the representation of the MSceneObject in the Unreal Engine.
// Allows the user to mark AActors as MSceneObject.
// Makes the required properties of the parent AActor (Root Transform, Colliders, Physics) known
// to the MOSIM framework and transforms them to the format required by MOSIM.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintTools.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/BodyInstance.h"

#include "Windows\AllowWindowsPlatformTypes.h"
#include "scene_types.h"
#include "Windows\HideWindowsPlatformTypes.h"

#include "MMISceneObject.generated.h"

class UnrealSceneAccess;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class MMISCENE_API UMMISceneObject : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UMMISceneObject();

    // destructor
    ~UMMISceneObject();

    bool UpdatePhysicsCurrentFrame;

	MMIStandard::MSceneObject MSceneObject;

    // IDs task List Editor, will be handeled by Task List Editor
    unsigned long taskEditorID;  // can repeat
    unsigned long localID;       // should not repeat

    // Name of the MMISceneObject as string
    std::string Name;
    // Get the GetActorLabel that the user can see in the interface
    std::string getNameStr();
    // update the names
    void updateNames();

    // Update the MSceneObject Name and set the Name of the MMISceneObject accordingly
    void setMSceneObjectName( std::string _Name );

    // get the root component
    AActor* ParentActor;
    USceneComponent* rootComp;

    // pointer to the unreal scene access
    UnrealSceneAccess* SceneAccess;

    // primitive components
    TArray<UPrimitiveComponent*> PrimitiveComponents;

    // Class to modify the physics
    FBodyInstance* BodyInstance;

    // primitive component of the root component
    UPrimitiveComponent* RootPrimitive;

    // Triggered at the simulation start by the SimulationController
    void Setup( UnrealSceneAccess* _SceneAccess );

    // Base setup method, required for triggering the setup without the simulation controller
    void SetupBase();

    // set the parent MMISceneObject, if available
    void SetParent();

    // change the parent object to the new object by name
    void ChangeParent( std::string newParentName );

    // dropdown list for MMISceneObjectTypes
    UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = SceneObjectProperties )
    TEnumAsByte<SceneObjectType> SceneObjectType = SceneObjectType::SceneObject;

	/* Navmesh area type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SceneObjectProperties)
	TEnumAsByte<NavMeshArea> AreaType;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

private:

    // access to the scene
    UWorld* scene;

    // Sets up the transform of the MSceneObject
    void SetupTransform();

    // get the current position of the AActor owning the MMISceneObject component
    void UpdateMSceneObjTransform(MMIStandard::MTransform& transform );

    // set up the colliders in the MSceneObject
    void SetupCollider();
	bool setupBoxCollider(MMIStandard::MCollider& mCollider);
	bool setupSphereCollider(MMIStandard::MCollider& mCollider);
	bool setupCapsuleCollider(MMIStandard::MCollider& mCollider);
	bool setupStaticMeshCollider(MMIStandard::MCollider& mCollider);
	bool setupSkeletalMeshCollider(MMIStandard::MCollider& mCollider);

    // set up the physics
    void SetupPhysics();

    // mass of the object
    float mass;

    // flag indicating whether a collider was found
    bool colliderSet;

    std::string GetSceneObjectType();

    /////////////////////////////////////////////////////////////////////////////////////
    ///////////// fields to supervise changes of the root component /////////////////////

    // has the root component changed
    bool HasRootChanged();
    // Update the root component history
    void UpdateRootHistory();
    // history of the root component
    FVector lastGlobalPosition;
    FQuat lastGlobalRotation;

    // threshold value above which changes are recognized
    float rootThreshold;

    /////////////////////////////////////////////////////////////////////////////////////
    ///////////// fields to supervise changes of the physics ////////////////////////////

    int physicsUpdateCounter;
    // has the root component changed
    bool HasPhysicsChanged();
    // Update the root component history
    void UpdatePhysicsHistory();
    // history of the physics
    FVector lastVelocity;
    FVector lastAngularVelocity;
    FVector lastCenterOfMass;
    FVector lastInertia;
    float lastMass;

    // threshold value above which changes are recognized
    float physicsThreshold;

    bool isInitialized;

    USkeletalMeshComponent* skeletalMesh;

    ////////////////////////////////////////////////////////////////////////////////////

public:
    // Called every frame
    virtual void TickComponent( float DeltaTime, ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction ) override;

    // check, if the scene component has a parent MMISceneObject
    bool hasParentMMISceneObject;

    // check, whether the root component is a primitive component
    bool rootIsPrimitive;

    // update root component (location of the Actor/SceneObject)
    void UpdateRootComponent();

    // update the physics of the object
    void UpdatePhysics( const MMIStandard::MPhysicsInteraction& physicsInteraction );
};
