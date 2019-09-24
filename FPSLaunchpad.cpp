// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchpad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"


// Sets default values
AFPSLaunchpad::AFPSLaunchpad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetBoxExtent(FVector(100, 100, 50)); 
	
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//Enables collision response on pawn
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//Enables collision response on boxes 
	OverlapComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	UE_LOG(LogTemp, Warning, TEXT("box detected"));
	
	
	RootComponent = OverlapComp; 
	
	// OverlapComp->SetHiddenInGame(false);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent); 

	//Bind to event 
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchpad::HandleOverlap);
	
	//Defaults
	LaunchStrength = 1500; 
	LaunchPitchAngle = 60.0f; 
}

void AFPSLaunchpad::HandleOverlap
(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	//Make rotator, pitch angles it up  
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength; 

	AFPSCharacter* OtherCharacter = Cast<AFPSCharacter>(OtherActor);
	if(OtherCharacter)
	{
		//Launch
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);
		UE_LOG(LogTemp, Warning, TEXT("ur flying"));

		//Spawn
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}

	//Does not overlap player, check if there is another physics simulating actor to be launched 
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

		UE_LOG(LogTemp, Warning, TEXT("box detected"));

		// Spawn FX
		// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
}

// Called when the game starts or when spawned
void AFPSLaunchpad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSLaunchpad::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}