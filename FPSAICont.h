// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAICont.generated.h"


UCLASS()
class FPSGAME_API AFPSAICont : public AAIController
{
	GENERATED_BODY()

public: 

AFPSAICont(); 

UPROPERTY(VisibleAnywhere, Category = "Perception")
class UAIPerceptionComponent* PerceptionComp;

UPROPERTY(VisibleAnywhere, Category = "Perception")
class UAISenseConfig_Sight* SightConfig; 


};
