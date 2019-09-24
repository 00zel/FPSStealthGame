
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchpad.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSLaunchpad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchpad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp; 

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComp; 

	UFUNCTION()
	void HandleOverlap
	( 
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);

	//Total impulse added to character on overlap
	UPROPERTY(EditInstanceOnly, Category = "Launchpad")
	float LaunchStrength; 

	//Actor rotation
	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
	float LaunchPitchAngle; 

	//Effect when activating launchpad
	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
	UParticleSystem* ActivateLaunchPadEffect; 
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
