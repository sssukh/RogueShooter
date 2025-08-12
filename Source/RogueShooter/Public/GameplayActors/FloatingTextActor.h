// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "FloatingTextActor.generated.h"

class UUW_FloatingTextWidget;
class UTextBlock;
class UWidgetComponent;

UCLASS()
class ROGUESHOOTER_API AFloatingTextActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloatingTextActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void FadeWidget(float alpha);

	UFUNCTION()
	void MoveWidget(float alpha);

	UFUNCTION()
	void DestroyActor();

public:
	UPROPERTY()
	float Damage;
	
private:
	UPROPERTY()
	TObjectPtr<class UWidgetComponent> Widget;

	UPROPERTY()
	FTimeline Movement;

	UPROPERTY()
	FTimeline Fade;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> MovementCurve;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> FadeCurve;

	UPROPERTY()
	FVector InitialLocation;

	UPROPERTY()
	FVector Destination;

	UPROPERTY()
	TObjectPtr<UTextBlock> TextBlock;

	TSubclassOf<UUW_FloatingTextWidget> FloatingTextClass;
};
