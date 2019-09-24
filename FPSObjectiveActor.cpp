#include "FPSObjectiveActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //can be set in blueprint under Collision Presets
	
	RootComponent = MeshComp; //appears @ top of blueprint components

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp")); 
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffects(); 
	
}

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

//  [Super::] overwrites NotifyActorBeginOverlap

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor) 
{
	Super::NotifyActorBeginOverlap(OtherActor); //Super extends function 

	PlayEffects(); 

	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor); 
	if (MyCharacter)
	{
		MyCharacter->bIsCarryingObjective = true; 

		Destroy(); 
	}
}
