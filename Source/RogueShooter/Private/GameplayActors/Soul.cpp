// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/Soul.h"

#include "FrameTypes.h"
#include "PhysicsReplication.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Interface/Interface_GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "RogueShooter/AssetPath.h"
#include "RogueShooter/FlowControlLIbrary.h"


// Sets default values
ASoul::ASoul()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Sphere 설정
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	
	Sphere->SetSphereRadius(240.0f);

	Sphere->SetLineThickness(0.0f);



	// Inner 설정
	Inner = CreateDefaultSubobject<USphereComponent>("Inner");

	Inner->SetSphereRadius(64.0f);

	Inner->SetLineThickness(0.0f);




	// Particle System Component 설정
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");

	ParticleSystemComponent->bEditableWhenInherited = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleFinder(*AssetPath::EffectTemplate::BaseSoul);
	if(ParticleFinder.Succeeded())
	{
		ParticleSystemComponent->SetTemplate(ParticleFinder.Object);
	}

	ParticleSystemComponent->SetAutoActivate(true);
	
	// 구조 설정 
	SetRootComponent(Sphere);

	Inner->SetupAttachment(Sphere);
	
	ParticleSystemComponent->SetupAttachment(Sphere);

	DoOnce = FDoOnce();

	SphereDoOnce = FDoOnce();
}

// Called when the game starts or when spawned
void ASoul::BeginPlay()
{
	Super::BeginPlay();

	TimelineUpdate.BindUFunction(this,FName("SoulLocationUpdate"));

	// sphere component에 바인드
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ASoul::OuterSphereBeginOverlap);

	// inner sphere component에 바인드
	Inner->OnComponentBeginOverlap.AddDynamic(this,&ASoul::InnerSphereBeginOverlap);
}

// Called every frame
void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ASoul::AddXP()
{
	// Interface 상속 받았는지 체크 후 do once 
	if(GM_Interface && GM_Interface->GetClass()->ImplementsInterface(UInterface_GameManager::StaticClass()) && DoOnce.Execute())
	{
		IInterface_GameManager::Execute_TransferXP(GM_Interface,XPAmount);

		Destroy();
	}
}

void ASoul::InnerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* pOtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),EmitterTemplate,Inner->GetComponentLocation());

	if(HasAuthority())
		AddXP();
}

void ASoul::OuterSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* pOtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!HasAuthority())
		return;

	// Do Once 실행 
	if(!SphereDoOnce.Execute())
		return;

	this->OtherActor = pOtherActor;

	StartingLocation = GetActorLocation();
	
	Timeline.AddInterpFloat(TimeCurve,TimelineUpdate);
}

void ASoul::SoulLocationUpdate(float Alpha)
{
	FVector Destination = OtherActor->GetActorLocation();
	
	this->SetActorLocation(FVector::SlerpVectorToDirection(StartingLocation,Destination,Alpha));
}

