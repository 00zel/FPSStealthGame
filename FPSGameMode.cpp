// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	WinningGame = false;
}

//Posess player controller 
void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	WinningGame = true;

	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);

		if (SpectatingViewpointClass)
		{
			TArray<AActor*> ReturnedActors; 
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

			AActor* NewViewTarget = nullptr;

			//Change view target if valid actor found
			if (ReturnedActors.Num() > 0)
			{
				NewViewTarget = ReturnedActors[0];
			
				APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
				if(PC)
				{
					PC->SetViewTargetWithBlend(nullptr, 0.5f,EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("viewtargets r broke"));
		}
		
	}
	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
	UE_LOG(LogTemp, Warning, TEXT("GAMEMODE MISSION COMPLETE"));
}
