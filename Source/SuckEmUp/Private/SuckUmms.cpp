

#include "SuckEmUpPrivatePCH.h"
#include "SuckUmms.h"


ASuckUmms::ASuckUmms(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CollisionComp = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComp"));
	CollisionComp->InitBoxExtent(FVector(25));
	RootComponent = CollisionComp;

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);

	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	PrimaryActorTick.bCanEverTick = true;

	CollisionComp->bGenerateOverlapEvents = true;
	bPlayerHas = false;
	bPlayerThrow = false;
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASuckUmms::OnBeginOverlap);
}


void ASuckUmms::BeginPlay()
{
	RateOfMovement = 0;
}

void ASuckUmms::Tick(float DeltaSeconds)
{
	if (bPlayerHas)
	{
		if (bPlayerThrow)
		{
			RateOfMovement += DeltaSeconds;
			this->SetActorLocation(FMath::Lerp(GetActorLocation(), ToHitLocation, FMath::Clamp(RateOfMovement, 0.0f, 1.0f)));
			if (RateOfMovement >= 1)
			{
				bPlayerThrow = false;
				RateOfMovement = 0;
				bPlayerHas = false;
			}
		}
		else
		{
			RateOfMovement += DeltaSeconds * 1 / FollowRateOffset;
			this->SetActorLocation(FMath::Lerp(GetActorLocation(), (character->GetActorLocation() + (character->GetActorForwardVector()*-FollowRateOffset)), FMath::Clamp(RateOfMovement, 0.0f, 1.0f)));
		}
	}
}

void ASuckUmms::PickUp(ASuckEmUpCharacter* thisCharacter, float followRate)
{
	RateOfMovement = 0;
	character = thisCharacter;
	bPlayerThrow = false;
	bPlayerHas = true;
	FollowRateOffset = followRate;
}

void ASuckUmms::ThrowMe(FVector hitLocation)
{
	bPlayerThrow = true;
	ToHitLocation = hitLocation;
	RateOfMovement = 0;
}
void ASuckUmms::OnBeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & hit)
{
	if (bPlayerThrow)
	{
		ASuckEmUpCharacter* thisCharacter = Cast<ASuckEmUpCharacter>(OtherActor);
		if (thisCharacter && thisCharacter != character)
		{
			thisCharacter->StunMe(2.0f);
		}
	}
}