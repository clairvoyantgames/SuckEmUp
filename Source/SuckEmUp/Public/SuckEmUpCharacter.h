// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperCharacter.h"
// #include "SuckUmms.h"
#include "SuckEmUpCharacter.generated.h"

// This class is the default character for SuckEmUp, and it is responsible for all
// physical interaction between the player and the world.
//
//   The capsule component (inherited from ACharacter) handles collision with the world
//   The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
//   The Sprite component (inherited from APaperCharacter) handles the visuals

UCLASS(config=Game)
class ASuckEmUpCharacter : public APaperCharacter
{
	GENERATED_UCLASS_BODY()

	// Cone mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ConeMesh)
	TSubobjectPtr<UStaticMeshComponent> ConeMesh;

	// Override Tick
	virtual void Tick(float DeltaSeconds) OVERRIDE;

	float relativeScale;

	float relativeBoxScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	TSubobjectPtr<class UBoxComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool CanWalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float baseSuckerScale;
//		TSubobjectPtr<class U> CollisionCom2;

protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* SuckAnimation;

	void SuckEm();

	float FollowersOffset;

	UFUNCTION()
	void OnBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & hit);

protected:

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
};
