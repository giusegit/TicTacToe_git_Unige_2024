// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSign.h"
#include "GameField.h"
#include "TTT_GameMode.h"

// Sets default values
ABaseSign::ABaseSign()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseSign::BeginPlay()
{
	Super::BeginPlay();
	ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->GField->OnResetEvent.AddDynamic(this, &ABaseSign::SelfDestroy);
}

// Called every frame
void ABaseSign::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSign::SelfDestroy()
{
	Destroy();
}

