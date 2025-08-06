// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_GameMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuClosed);

class UButton;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_GameMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_GameMenu(const FObjectInitializer& ObjectInitializer);

	 void NativeConstruct() override;

	UFUNCTION()
	void OnClicked_Continue();

	UFUNCTION()
	void OnClicked_QuitDesktop();

	UFUNCTION()
	void OnClicked_QuitMenu();
public:
	UPROPERTY(BlueprintReadWrite, Category="UW_GameMenu|Designer", meta=(BindWidget))
	TObjectPtr<UButton> Button_Continue;

	UPROPERTY(BlueprintReadWrite, Category="UW_GameMenu|Designer", meta=(BindWidget))
	TObjectPtr<UButton> Button_Quit_Desktop;
	
	UPROPERTY(BlueprintReadWrite, Category="UW_GameMenu|Designer", meta=(BindWidget))
	TObjectPtr<UButton> Button_Quit_Menu;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnMenuClosed OnMenuClosed;
};
