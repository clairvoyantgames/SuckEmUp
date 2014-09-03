

#include "SuckEmUpPrivatePCH.h"
#include "SuckUmms.h"


ASuckUmms::ASuckUmms(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = PCIP.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("Sprite"));
	
	bPlayerHas = false;
	bPlayerThrow = false;	
}


void ASuckUmms::BeginPlay()
{
	Super::PostBeginPlay();
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
			this->SetActorLocation(FMath::Lerp(GetActorLocation(), (OwningCharacter->GetActorLocation() + (OwningCharacter->GetActorForwardVector()*-FollowRateOffset)), FMath::Clamp(RateOfMovement, 0.0f, 1.0f)));
		}
	}
}

void ASuckUmms::PickUp(ASuckEmUpCharacter* character, float followRate)
{
	RateOfMovement = 0;
	OwningCharacter = character;
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
		ASuckEmUpCharacter* character = Cast<ASuckEmUpCharacter>(OtherActor);
		if (character && character != OwningCharacter)
		{
			OwningCharacter->StunPlayer(2.0f);
		}
	}
}