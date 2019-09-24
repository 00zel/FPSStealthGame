
#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "FPS_BlackHole.h"


// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f)); 
	RootComponent = OverlapComp; 

	OverlapComp->SetHiddenInGame(false);

	//Dynamic binding 
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	//Decals are projected onto a surface 
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f);
	DecalComp->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::HandleOverlap
( 
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	//Checking if OtherActor exists in class AFPSCharacter, MyPawn will inheret properties and methods of OtherActor
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if(MyPawn == nullptr)
	{
		return;
	}

	if (MyPawn->bIsCarryingObjective)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(MyPawn, true);
			UE_LOG(LogTemp, Warning, TEXT("EXTRACTION ZONE MISSION COMPLETE"));

			//ALT METHOD: AFPS_BlackHole* BlackHole = Cast<AFPS_BlackHole>(GetAllActors
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound); 
		UE_LOG(LogTemp, Warning, TEXT("NO LOOT"));

	}
}