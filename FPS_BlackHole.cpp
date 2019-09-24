#include "FPS_BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FPSGameMode.h"
#include "FPSExtractionZone.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPS_BlackHole::AFPS_BlackHole()
{
 	// Set this actor to call Tick() every frame.  You can set this to false to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); 					//can be set in blueprint under Collision Presets
	RootComponent = MeshComp; 														//appears @ top of blueprint components

	//Outer sphere component 
	SphereCompAttract = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCompAttract")); 
	SphereCompAttract->SetSphereRadius(1600);
	SphereCompAttract->SetupAttachment(MeshComp);

	//Inner sphere component 
	SphereCompDestroy = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCompDestroy"));
	SphereCompDestroy->SetSphereRadius(100);
	SphereCompDestroy->SetupAttachment(MeshComp);

}

//Find actor and destroy it. 
void AFPS_BlackHole::OverlapInnerSphere(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy(); 
	}
}

// Called when the game starts or when spawned
void AFPS_BlackHole::BeginPlay()
{
	Super::BeginPlay();

	//Bind to overlap event function (destroy)
	SphereCompDestroy->OnComponentBeginOverlap.AddDynamic(this, &AFPS_BlackHole::OverlapInnerSphere); 	

}

//defining parameters of the function Attract();
void AFPS_BlackHole::Attract()
{
	//Find all overlapping components that can collide and may be physically simulating 
	TArray<UPrimitiveComponent*> OverlappingComps; 
	SphereCompAttract->GetOverlappingComponents(OverlappingComps);
	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i]; 

		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			//Scaled gravity based on radius 
			const float SphereRadius = SphereCompAttract->GetScaledSphereRadius(); 

			// Set as a negative value to pull rather than push away 
			const float ForceStrength = -2000; 

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

// Called every frame
void AFPS_BlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//	Attract();

	Trigger();

	// UE_LOG(LogTemp, Warning, TEXT("TICK"));
}

//create variable, cast to variable, create condition
void AFPS_BlackHole::Trigger()
{
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

	if (GM->WinningGame)
	{
		Attract();
		UE_LOG(LogTemp, Warning, TEXT("FLYING BOXES"));
	}
}