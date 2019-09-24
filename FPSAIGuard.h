#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent; 

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

	UFUNCTION(BlueprintNativeEvent, Category = "AIVariables | Character")
	void GetPerceptionLocRot(FVector &OutLocation, FRotator &OutRotation) const; 
	void GetPerceptionLocRot_Implementation(FVector &OutLocation, FRotator &OutRotation) const; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override; 

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* PawnHeard, const FVector& Location, float Volume);

	FRotator OriginalRotation; 

	UFUNCTION()
	void ResetOrientation(); 

	FTimerHandle TimerHandle_ResetOrientation; 

	EAIState GuardState; 

	void SetGuardState(EAIState NewState); 

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState); 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetActorEyesViewPoint(FVector &Location, FRotator &Rotation) const override;
};
