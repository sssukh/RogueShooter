// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/FloatingTextActor.h"

#include <string>

#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "RogueShooter/AssetPath.h"
#include "UI/UW_FloatingTextWidget.h"


// Sets default values
AFloatingTextActor::AFloatingTextActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Widget = CreateDefaultSubobject<UWidgetComponent>("Widget");

	Widget->SetupAttachment(GetRootComponent());

	Widget->SetDrawSize(FVector2d(150.0f,50.0f));

	Widget->SetPivot(FVector2d(0.5f,0.5f));

	Widget->SetWidgetSpace(EWidgetSpace::Screen);

	Widget->SetTickMode(ETickMode::Enabled);
	
	// 위젯 클래스 설정
	
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(*AssetPath::Blueprint::WBP_FloatingTextWidget_C);
	if(WidgetClassFinder.Succeeded())
	{
		Widget->SetWidgetClass(WidgetClassFinder.Class);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFloatFinder(*AssetPath::Curve::Curve_Fade);
	if(CurveFloatFinder.Succeeded())
	{
		FadeCurve = CurveFloatFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveVectorFinder(*AssetPath::Curve::Curve_Movement);
	if(CurveVectorFinder.Succeeded())
	{
		MovementCurve = CurveVectorFinder.Object;
	}
}

// Called when the game starts or when spawned
void AFloatingTextActor::BeginPlay()
{
	Super::BeginPlay();

	// Fade Out

	// 위젯 컴포넌트에서 보일 위젯을 floating text widget으로 변경 및 값 적용
	UUW_FloatingTextWidget* FloatingText = Cast<UUW_FloatingTextWidget>(Widget->GetUserWidgetObject());

	FloatingText->TextBlock_Damage->SetText(FText::FromString(FString::SanitizeFloat(Damage)));

	TextBlock = FloatingText->TextBlock_Damage;

	// fade curve 값에 따라 fadeout됨.
	FOnTimelineFloat FadeFunction;

	FadeFunction.BindUFunction(this,FName("FadeWidget"));

	Fade.AddInterpFloat(FadeCurve,FadeFunction);

	Fade.PlayFromStart();
	// Move Up

	InitialLocation =  GetActorLocation();

	Destination = InitialLocation + FVector(0.0f,0.0f,150.0f);
	
	FOnTimelineFloat MovementFunction;

	MovementFunction.BindUFunction(this,FName("MoveWidget"));

	FOnTimelineEvent FinishedFunction;

	FinishedFunction.BindUFunction(this,FName("DestroyActor"));

	Movement.AddInterpFloat(MovementCurve,MovementFunction);

	Movement.SetTimelineFinishedFunc(FinishedFunction);

	Movement.PlayFromStart();
}

// Called every frame
void AFloatingTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Fade.TickTimeline(DeltaTime);

	Movement.TickTimeline(DeltaTime);
}

void AFloatingTextActor::FadeWidget(float alpha)
{
	TextBlock->SetRenderOpacity(alpha);
}

void AFloatingTextActor::MoveWidget(float alpha)
{
	SetActorLocation(FVector::SlerpVectorToDirection(InitialLocation,Destination,alpha));
}

void AFloatingTextActor::DestroyActor()
{
	Destroy();
}



