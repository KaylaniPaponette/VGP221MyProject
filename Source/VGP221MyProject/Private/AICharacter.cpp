// -----------------------------------------------------------------------------
// AICharacter.cpp
// -----------------------------------------------------------------------------
#include "AICharacter.h"

// Sets default values
AAICharacter::AAICharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Set the default team for this AI character.
	// Enemy AI characters will have a different team ID than the player character.
    TeamId = FGenericTeamId(1);
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
    Super::BeginPlay();
}
