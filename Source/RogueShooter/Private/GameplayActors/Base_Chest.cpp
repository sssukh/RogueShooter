// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/Base_Chest.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Interface_CharacterManager.h"
#include "Interface/Interface_ControllerManager.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"


// Sets default values
ABase_Chest::ABase_Chest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// StaticMesh 초기 설정
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	StaticMesh->SetWorldScale3D(FVector(0.75f));

	StaticMesh->SetWorldLocation(FVector(0.0f,0.0f,35.0f));
 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChestMeshFinder(*AssetPath::Mesh::ChamferCube);

	if(ChestMeshFinder.Succeeded())
	{
		StaticMesh->SetStaticMesh(ChestMeshFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> ChestMatFinder(*AssetPath::Material::BaseChestMat);

	if(ChestMatFinder.Succeeded())
	{
		UMaterialInterface* ChestMaterial = ChestMatFinder.Object;
		
		StaticMesh->SetMaterial(0,ChestMaterial);
	}
	
	StaticMesh->SetupAttachment(GetRootComponent());

	// Sphere 초기 설정 
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");

	Sphere->SetSphereRadius(125.0f);

	Sphere->SetLineThickness(0.0f);
	
	Sphere->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void ABase_Chest::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ABase_Chest::SphereBeginOverlap);
}

// Called every frame
void ABase_Chest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_Chest::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		ChestAction();

		Destroy();
	}
}

void ABase_Chest::ChestAction()
{
	
	TArray<APlayerState*> PlayerStates  = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;

	for(const APlayerState* ps : PlayerStates)
	{
		APawn* PlayerPawn = ps->GetPlayerController()->GetPawn();

		if(!PlayerPawn->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			if(IInterface_CharacterManager::Execute_IsAlive(PlayerPawn))
			{
				if(ps->GetPlayerController()->GetClass()->ImplementsInterface(UInterface_ControllerManager::StaticClass()))
				{
					IInterface_ControllerManager::Execute_OnChestFound(ps->GetPlayerController());
				}
			}
		}
	}

	for(const APlayerState* ps : PlayerStates)
	{
		if(ps->GetPawn()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			IInterface_CharacterManager::Execute_Pause(ps->GetPawn(),true,false);
		}
	}
}

