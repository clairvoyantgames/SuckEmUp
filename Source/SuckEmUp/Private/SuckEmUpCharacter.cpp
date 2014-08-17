// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "SuckEmUpPrivatePCH.h"
#include "SuckEmUpCharacter.h"
#include "SuckUmms.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"

ASuckEmUpCharacter::ASuckEmUpCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	ConeMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ConeMesh"));
	ConeMesh->AttachTo(RootComponent);
	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CharacterMovement->bOrientRotationToMovement = false;

	// Configure character movement
	CharacterMovement->GravityScale = 2.0f;
	CharacterMovement->AirControl = 0.80f;
	CharacterMovement->JumpZVelocity = 1000.f;
	CharacterMovement->GroundFriction = 3.0f;
	CharacterMovement->MaxWalkSpeed = 1800;
	CharacterMovement->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	CharacterMovement->bConstrainToPlane = true;
	CharacterMovement->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	CharacterMovement->bUseFlatBaseForFloorChecks = true;

	// Note: The reference to the RunningAnimation and IdleAnimation flipbooks to play on the Sprite component
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	//CapsuleComponent->bGenerateOverlapEvents = true;
	//CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ASuckEmUpCharacter::OnBeginOverlap);
	FollowersOffset = 100;
	relativeScale = baseSuckerScale;

	CollisionComp = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComp"));
	CollisionComp->InitBoxExtent(FVector(150));
	CollisionComp->AttachTo(ConeMesh);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComp->bGenerateOverlapEvents = true;

	CanWalk = true;

	relativeBoxScale = 1;
	CapsuleComponent->SetCapsuleSize(60, 60);

	
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ASuckEmUpCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeed > 0.0f) ? RunningAnimation : IdleAnimation;
	if (this->GetMovementComponent()->IsFalling())
	{
		DesiredAnimation = JumpAnimation;
	}
	if (relativeScale != baseSuckerScale)
	{
		DesiredAnimation = SuckDownAnimation;
	}

	Sprite->SetFlipbook(DesiredAnimation);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASuckEmUpCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	InputComponent->BindAction("Jump", IE_Pressed, this, &ASuckEmUpCharacter::MyJump);
	InputComponent->BindAxis("MoveRight", this, &ASuckEmUpCharacter::MoveRight);
	InputComponent->BindAction("Suck", IE_Pressed, this, &ASuckEmUpCharacter::SuckEm);
	InputComponent->BindAction("Up", IE_Pressed, this, &ASuckEmUpCharacter::Up);
}

void ASuckEmUpCharacter::MoveRight(float Value)
{
	// Update animation to match the motion
	UpdateAnimation();

	if (CanWalk)
	{
		// Set the rotation so that the character faces his direction of travel.
		if (Value < 0.0f)
		{
			CapsuleComponent->SetWorldRotation(FRotator(0.0, 180.0f, 0.0f));
			//Sprite->SetWorldRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (Value > 0.0f)
		{
			CapsuleComponent->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
			//Sprite->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
		}

		// Apply the input to the character motion
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
	else
	{
		// Set the rotation so that the character faces his direction of travel.
		if (Value < 0.0f)
		{
			CapsuleComponent->SetWorldRotation(FRotator(0.0, 180.0f, 0.0f));
			//Sprite->SetWorldRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (Value > 0.0f)
		{
			CapsuleComponent->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
			//Sprite->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}
void ASuckEmUpCharacter::Up()
{
	FString thisString;

	GEngine->GameViewport->CreatePlayer(1, thisString, true);
	for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ACameraActor* thisCamera;
		thisCamera = *ActorItr;
		UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(thisCamera);
		UGameplayStatics::GetPlayerController(this, 1)->SetViewTargetWithBlend(thisCamera);
		break;
	}
	
	
	
}
void ASuckEmUpCharacter::MyJump()
{
	if (CanWalk)
	{
		UPaperFlipbook* DesiredAnimation = JumpAnimation;

		Sprite->SetFlipbook(DesiredAnimation);
		Super::Jump();
		
	}
}

void ASuckEmUpCharacter::SuckEm()
{
	UPaperFlipbook* DesiredAnimation = SuckAnimation;

	Sprite->SetFlipbook(DesiredAnimation);

	relativeScale += .05;
	relativeBoxScale += .025;
	relativeScale = FMath::Clamp(relativeScale, baseSuckerScale, 3.0f);
	ConeMesh->SetRelativeScale3D(FVector(relativeScale));
	CollisionComp->SetRelativeScale3D(FVector(relativeBoxScale));
	ConeMesh->SetWorldLocation(GetActorLocation() + GetActorForwardVector() * (50 + (relativeScale * 300)) );

	FRotator newRotation = this->CapsuleComponent->RelativeRotation;

	if (newRotation.Yaw != 0)
	{
		CollisionComp->SetWorldLocation(ConeMesh->GetComponentLocation() + FVector(75, 0, 0));
	}
	else
	{
		CollisionComp->SetWorldLocation(ConeMesh->GetComponentLocation() - FVector(75, 0, 0));
	}

	TArray<AActor*> OverlappingActors;
	CollisionComp->GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Num() > 0)
	{
		for (int32 x = 0; x < OverlappingActors.Num(); x++)
		{
			ASuckUmms* thisSuckumms = Cast<ASuckUmms>(OverlappingActors[x]);
			if (thisSuckumms)
			{
				if (thisSuckumms->bPlayerHas == true)
				{
					if (thisSuckumms->character != this)
					{
						thisSuckumms->PickUp(this, FollowersOffset);
						FollowersOffset += 50;

					}
				}
				else
				{
					thisSuckumms->PickUp(this, FollowersOffset);
					FollowersOffset += 50;
				}
			}
		}
	}
}

void ASuckEmUpCharacter::OnBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & hit)
{
	/*
	ASuckUmms* thisSuckumms = Cast<ASuckUmms>(OtherActor);
	if (thisSuckumms)
	{
		if (thisSuckumms->bPlayerHas == true)
		{
			if (thisSuckumms->character != this)
			{
				thisSuckumms->PickUp(this, FollowersOffset);
				FollowersOffset += 100;

			}
		}
		else
		{
			thisSuckumms->PickUp(this, FollowersOffset);
			FollowersOffset += 100;
		}
	}*/
}

void ASuckEmUpCharacter::Tick(float DeltaSeconds)
{
	if (relativeScale != baseSuckerScale)
	{
		CanWalk = false;
		relativeScale -= .0020;
		relativeBoxScale -= .001;
		relativeScale = FMath::Clamp(relativeScale, baseSuckerScale, 2.5f);
		ConeMesh->SetRelativeScale3D(FVector(relativeScale));
		CollisionComp->SetRelativeScale3D(FVector(relativeBoxScale));
		ConeMesh->SetWorldLocation(GetActorLocation() + GetActorForwardVector() * (50 + (relativeScale * 300)));

		FRotator newRotation = this->CapsuleComponent->RelativeRotation;

		if (newRotation.Yaw != 0)
		{
			CollisionComp->SetWorldLocation(ConeMesh->GetComponentLocation() + FVector(75,0,0));
		}
		else
		{
			CollisionComp->SetWorldLocation(ConeMesh->GetComponentLocation() - FVector(75, 0, 0));
		}
		ConeMesh->SetVisibility(true);

		TArray<AActor*> OverlappingActors;
		CollisionComp->GetOverlappingActors(OverlappingActors);
		if (OverlappingActors.Num() > 0)
		{
			for (int32 x = 0; x < OverlappingActors.Num(); x++)
			{
				ASuckUmms* thisSuckumms = Cast<ASuckUmms>(OverlappingActors[x]);
				if (thisSuckumms)
				{
					if (thisSuckumms->bPlayerHas == true)
					{
						if (thisSuckumms->character != this)
						{
							thisSuckumms->PickUp(this, FollowersOffset);
							FollowersOffset += 50;

						}
					}
					else
					{
						thisSuckumms->PickUp(this, FollowersOffset);
						FollowersOffset += 50;
					}
				}
			}
		}
	}
	else
	{
		CanWalk = true;
		ConeMesh->SetVisibility(false);
	}
}

void ASuckEmUpCharacter::PossessedBy(class AController* InController)
{

	Super::PossessedBy(InController);

	// [server] as soon as PlayerState is assigned, set team colors of this pawn for local player
	//UpdateTeamColorsAllMIDs();
}