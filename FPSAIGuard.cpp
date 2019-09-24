#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "FPSGameMode.h"

// Set default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen); 
	// PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard); 

	GuardState = EAIState::Idle;

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation(); 
}

// Allow actors to initialize themselves after all of their components have been initialized, only called during gameplay
void AFPSAIGuard::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if(SeenPawn == nullptr)
	{
		return;
		UE_LOG(LogTemp, Error, TEXT("NO PAWN"));
	}

	UE_LOG(LogTemp, Error, TEXT("DETECTED BY GUARD"));
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Yellow, false, 10.f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode()); 
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false); 
	} 

	SetGuardState(EAIState::Alerted);
}

void AFPSAIGuard::OnNoiseHeard(APawn* PawnHeard, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return; 
	}

	DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);

	FVector	Direction = Location - GetActorLocation();

	Direction.Normalize(); 

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f; 
 
	SetActorRotation(NewLookAt); 

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);

	if (GuardState != EAIState::Alerted)
	{
		SetGuardState(EAIState::Suspicious);
	}
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return; 
	}
	
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle); 
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if(GuardState == NewState)
	{
		return; 
	}

	GuardState = NewState; 

	OnStateChanged(GuardState); 
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::GetActorEyesViewPoint(FVector &Location, FRotator &Rotation) const
{
	GetPerceptionLocRot(Location, Rotation);

}

void AFPSAIGuard::GetPerceptionLocRot_Implementation(FVector &OutLocation, FRotator &OutRotation) const
{
	OutLocation = GetActorLocation() + FVector(0, 0, 50); 
	OutRotation = GetActorRotation(); 

}