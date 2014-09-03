

#pragma once

#include "GameFramework/Actor.h"
#include "SuckEmUpCharacter.h"
#include "SuckUmms.generated.h"

/**
 * 
 */
UCLASS()
class SUCKEMUP_API ASuckUmms : public AActor
{
	GENERATED_UCLASS_BODY()

	// Sprite
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite)
	TSubobjectPtr<UPaperFlipbookComponent> Sprite;
	
	bool bPlayerHas;

	ASuckEmUpCharacter* OwningCharacter;
		
	void PickUp(ASuckEmUpCharacter* thisCharacter, float followRate);

	float RateOfMovement;
	bool bPlayerThrow;
	float FollowRateOffset;
	FVector ToHitLocation;
	void ThrowMe(FVector hitLocation);
	UFUNCTION()
	void OnBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & hit);
};
