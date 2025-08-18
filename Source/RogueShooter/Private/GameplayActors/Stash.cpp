// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/Stash.h"

#include "FbxMeshUtils.h"
#include "Components/SphereComponent.h"
#include "RogueShooter/AssetPath.h"
#include "GameplayActors/Base_Pickup.h"
#include "Utility/RSCollisionChannel.h"


// Sets default values
AStash::AStash()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	StaticMesh->SetRelativeLocation(FVector(0.0f,0.0f,25.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshFinder(*AssetPath::Mesh::SM_Cube);

	if(StaticMeshFinder.Succeeded())
		StaticMesh->SetStaticMesh( StaticMeshFinder.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder(*AssetPath::Material::CubeYellow);

	if(MatFinder.Succeeded())
		StaticMesh->SetMaterial(0,MatFinder.Object);
	
	Sphere->SetSphereRadius(96.0f);

	Sphere->SetLineThickness(0.0f);
}

// Called when the game starts or when spawned
void AStash::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStash::SpawnItem()
{
	TSubclassOf<ABase_Pickup> pickupclass = PossibleItemsToSpawn[FMath::RandRange(0,PossibleItemsToSpawn.Num())];

	FVector SpawnLocation = GetActorLocation();

	FRotator SpawnRotator = FRotator::ZeroRotator;
	
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	GetWorld()->SpawnActor(pickupclass,&SpawnLocation,&SpawnRotator,ActorSpawnParameters);
}

void AStash::ResetStash()
{
	
}

void AStash::HideStash_Implementation(bool Hide)
{
	SetActorHiddenInGame(Hide);

	// Hide가 true이면 world static 으로 설정 -> ovelap안됨
	// false 이면 enemy로 설정 -> overlap됨
	ECollisionChannel CollisionChannel = Hide?ECollisionChannel::ECC_WorldStatic:ECC_COLLISION_ENEMY;

	Sphere->SetCollisionObjectType(CollisionChannel);
}

bool AStash::CheckForItems()
{
	TArray<FHitResult> HitResults;

	FVector ActorLocation = GetActorLocation();	
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_COLLISION_PICKUPITEM);

	FCollisionQueryParams params;
	params.
	GetWorld()->SweepMultiByObjectType(HitResults,ActorLocation,ActorLocation,FQuat::Identity,ObjectQueryParams,FCollisionShape::MakeSphere(32.0f));

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(),ActorLocation,32.0f,32,FColor::Blue,false,2.0f);
#endif
}

float AStash::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                         class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
