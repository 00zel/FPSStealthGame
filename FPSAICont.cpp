
#include "FPSAICont.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AFPSAICont::AFPSAICont()
{

    PerceptionComp = CreateAbstractDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp")); 
    SightConfig = CreateAbstractDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig")); 

    if (PerceptionComp && SightConfig)
    {
        PerceptionComp->ConfigureSense(*SightConfig);
        PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

        SightConfig->SightRadius = 2000.f; 
        SightConfig->LoseSightRadius = 2200.f; 
        SightConfig->PeripheralVisionAngleDegrees = 90.f; 

        SightConfig->DetectionByAffiliation.bDetectEnemies = true; 
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true; 
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true; 
    }

}