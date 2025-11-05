// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/Stash.h"

#include "FbxMeshUtils.h"
#include "Components/SphereComponent.h"
#include "RogueShooter/AssetPath.h"
#include "GameplayActors/Base_Pickup.h"
#include "GameplayActors/Gold_Pickup.h"
#include "GameplayActors/Health_Pickup.h"
#include "Utility/RSCollisionChannel.h"


// Sets default values
AStash::AStash()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	StaticMesh->SetRelativeLocation(FVector(0.0f,0.0f,25.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshFinder(*AssetPath::Mesh::SM_Cube);

	if(StaticMeshFinder.Succeeded())
		StaticMesh->SetStaticMesh( StaticMeshFinder.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder(*AssetPath::Material::CubeYellow);

	if(MatFinder.Succeeded())
		StaticMesh->SetMaterial(0,MatFinder.Object);
	
	StaticMesh->SetupAttachment(RootComponent);
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	
	Sphere->SetSphereRadius(96.0f);

	Sphere->SetLineThickness(0.0f);

	Sphere->SetupAttachment(StaticMesh);
	
	DoOnce.Reset();

	static ConstructorHelpers::FClassFinder<AHealth_Pickup> HealthPickupFinder(*AssetPath::Blueprint::BP_HealthPickup_C);
	if(HealthPickupFinder.Succeeded())
	{
		PossibleItemsToSpawn.Add(HealthPickupFinder.Class);
	}

	static ConstructorHelpers::FClassFinder<AGold_Pickup> GoldPickupFinder(*AssetPath::Blueprint::BP_GoldPickup_C);
	if(GoldPickupFinder.Succeeded())
	{
		PossibleItemsToSpawn.Add(GoldPickupFinder.Class);
	}
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
	TSubclassOf<ABase_Pickup> pickupclass = PossibleItemsToSpawn[FMath::RandRange(0,PossibleItemsToSpawn.Num()-1)];

	FVector SpawnLocation = GetActorLocation();

	FRotator SpawnRotator = FRotator::ZeroRotator;
	
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	GetWorld()->SpawnActor(pickupclass,&SpawnLocation,&SpawnRotator,ActorSpawnParameters);
}

void AStash::ResetActivation(bool bReset)
{
	if(bReset)
		DoOnce.Reset();

	if(!DoOnce.Execute())
		return;
	
	SpawnItem();

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer,FTimerDelegate::CreateUObject(this,&AStash::ResetStash),StashResetTime,false);
}

void AStash::ResetStash()
{
	if(CheckForItems())
	{
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer,FTimerDelegate::CreateUObject(this,&AStash::ResetStash),StashResetTime,false);
	}
	else
	{
		HideStash(false);

		ResetActivation(true);
	}
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
	// 자기 자신을 제외하고 트레이스를 한다.
	FHitResult HitResult;

	FVector ActorLocation = GetActorLocation();	
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_COLLISION_PICKUPITEM);

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	
	bool result = GetWorld()->SweepSingleByObjectType(HitResult,ActorLocation,ActorLocation,FQuat::Identity,ObjectQueryParams,FCollisionShape::MakeSphere(32.0f));

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(),ActorLocation,32.0f,32,FColor::Blue,false,2.0f);
#endif

	return result;
}

float AStash::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                         class AController* EventInstigator, AActor* DamageCauser)
{
	HideStash(true);

	if(HasAuthority())
	{
		ResetActivation(false);
	}

		
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
