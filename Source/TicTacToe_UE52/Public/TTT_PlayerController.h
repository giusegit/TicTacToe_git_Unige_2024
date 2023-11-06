// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TTT_HumanPlayer.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TTT_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTOE_UE52_API ATTT_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATTT_PlayerController();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* TTTContext;
	 

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ClickAction;

	void ClickOnGrid();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
