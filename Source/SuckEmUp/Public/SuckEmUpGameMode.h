// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SuckEmUpGameMode.generated.h"

// The GameMode defines the game being played. It governs the game rules, scoring, what actors
// are allowed to exist in this game type, and who may enter the game.
//
// This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of SuckEmUpCharacter

UCLASS(minimalapi)
class ASuckEmUpGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	int32 winCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	bool gameStarted;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void SpawnSecondPlayer();
};
