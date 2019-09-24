// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPS_BlackHole.generated.h"

class USphereComponent; 
class UStaticMeshComponent; 

UCLASS()
class FPSGAME_API AFPS_BlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPS_BlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp; 

	//Outer sphere attracts components 
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereCompAttract;

	//Inner sphere destroys components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereCompDestroy;

	//Marked with UFunction to bind overlap event 
	UFUNCTION()
	void OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Attract();
	void Trigger(); 
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
