// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "SuckEmUpPrivatePCH.h"
#include "SuckEmUpGameMode.h"

ASuckEmUpGameMode::ASuckEmUpGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("Class'/Game/Blueprints/MyCharacter.MyCharacter_C'"));

	winCount = 5;

	gameStarted = false;

	if (PlayerPawnBPClass.Object != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Object;
		//FString thisString;
		//GEngine->GameViewport->CreatePlayer(1, thisString, true);
	}	
}

void ASuckEmUpGameMode::SpawnSecondPlayer(){
	UGameplayStatics::CreatePlayer(this,-1,true);
}
