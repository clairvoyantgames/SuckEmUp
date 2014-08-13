

#include "SuckEmUpPrivatePCH.h"
#include "SuckUmms.h"


ASuckUmms::ASuckUmms(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CollisionComp = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComp"));
	CollisionComp->InitBoxExtent(FVector(100));
	RootComponent = CollisionComp;

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);

	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	PrimaryActorTick.bCanEverTick = true;

	CollisionComp->bGenerateOverlapEvents = true;
	bPlayerHas = false;
}


void ASuckUmms::BeginPlay()
{
	RateOfMovement = 0;
}

void ASuckUmms::Tick(float DeltaSeconds)
{
	if (bPlayerHas)
	{
		RateOfMovement += DeltaSeconds * 1/(500 + FollowRateOffset);
		this->SetActorLocation(FMath::Lerp(GetActorLocation(), (character->GetActorLocation() + (character->GetActorForwardVector()*-FollowRateOffset)), FMath::Clamp(RateOfMovement, 0.0f, 1.0f)));
	}
}

void ASuckUmms::PickUp(ASuckEmUpCharacter* thisCharacter, float followRate)
{
	RateOfMovement = 0;
	character = thisCharacter;
	bPlayerHas = true;
	FollowRateOffset = followRate;
}