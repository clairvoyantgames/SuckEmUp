

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
	// Collision Component of Ball
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	TSubobjectPtr<class UBoxComponent> CollisionComp;


	// Ball mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TSubobjectPtr<UStaticMeshComponent> Mesh;
	
	bool bPlayerHas;
	ASuckEmUpCharacter* character;

	// Override Tick
	virtual void Tick(float DeltaSeconds) OVERRIDE;

	// Begin Play function
	virtual void BeginPlay() OVERRIDE;
	
	void PickUp(ASuckEmUpCharacter* thisCharacter, float followRate);

	float RateOfMovement;
	bool bPlayerThrow;
	float FollowRateOffset;
	FVector ToHitLocation;
	void ThrowMe(FVector hitLocation);
	UFUNCTION()
	void OnBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & hit);
};
