// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/Base_Pickup.h"

#include "Components/SphereComponent.h"
#include "RogueShooter/AssetPath.h"


// Sets default values
ABase_Pickup::ABase_Pickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	StaticMesh->SetRelativeLocation(FVector(0.0f,0.0f,20.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PotionBottleFinder(*AssetPath::Mesh::Potion);
	if(PotionBottleFinder.Succeeded())
		StaticMesh->SetStaticMesh(PotionBottleFinder.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> PotionMatFinder(*AssetPath::Material::PotionMaterial);
	if(PotionMatFinder.Succeeded())
		StaticMesh->SetMaterial(0,PotionMatFinder.Object);
}

// Called when the game starts or when spawned
void ABase_Pickup::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ABase_Pickup::OnSphereComponentBeginOverlap);
}

// Called every frame
void ABase_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_Pickup::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!HasAuthority())
		return;

	DoPickupAction(OtherActor);

	Destroy();
}

void ABase_Pickup::DoPickupAction(AActor* Character)
{
}

