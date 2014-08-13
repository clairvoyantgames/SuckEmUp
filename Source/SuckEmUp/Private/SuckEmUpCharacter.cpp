// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "SuckEmUpPrivatePCH.h"
#include "SuckEmUpCharacter.h"
#include "PaperFlipbookComponent.h"

ASuckEmUpCharacter::ASuckEmUpCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUseControllerViewRotation = false;
	CharacterMovement->bOrientRotationToMovement = false;

	// Configure character movement
	CharacterMovement->GravityScale = 2.0f;
	CharacterMovement->AirControl = 0.80f;
	CharacterMovement->JumpZVelocity = 1000.f;
	CharacterMovement->GroundFriction = 3.0f;
	CharacterMovement->MaxWalkSpeed = 600.0f;
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
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ASuckEmUpCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeed > 0.0f) ? RunningAnimation : IdleAnimation;

	Sprite->SetFlipbook(DesiredAnimation);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASuckEmUpCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAxis("MoveRight", this, &ASuckEmUpCharacter::MoveRight);

	InputComponent->BindTouch(IE_Pressed, this, &ASuckEmUpCharacter::TouchStarted);
}

void ASuckEmUpCharacter::MoveRight(float Value)
{
	// Update animation to match the motion
	UpdateAnimation();

	// Set the rotation so that the character faces his direction of travel.
	if (Value < 0.0f)
	{
		Sprite->SetWorldRotation(FRotator(0.0, 180.0f, 0.0f));
	}
	else if (Value > 0.0f)
	{
		Sprite->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	}

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ASuckEmUpCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}
