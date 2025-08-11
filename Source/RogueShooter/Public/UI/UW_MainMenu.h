// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MainMenu.generated.h"

class UUW_ServerBrowser;
class UUW_HostMenu;
class UButton;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_MainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_MainMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonHostClicked();

	UFUNCTION()
	void OnButtonExitClicked();

	UFUNCTION()
	void OnButtonJoinClicked();
public:
	UPROPERTY(BlueprintReadWrite,Category="UW_MainMenu | Designer",meta = (BindWidget))
	TObjectPtr<UButton> Button_Exit;

	UPROPERTY(BlueprintReadWrite,Category="UW_MainMenu | Designer",meta = (BindWidget))
	TObjectPtr<UButton> Button_Host;

	UPROPERTY(BlueprintReadWrite,Category="UW_MainMenu | Designer",meta = (BindWidget))
	TObjectPtr<UButton> Button_Join;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_MainMenu | Set up")
	TSubclassOf<UUW_HostMenu> HostMenuClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_MainMenu | Set up")
	TSubclassOf<UUW_ServerBrowser> ServerBrowserClass;
};
