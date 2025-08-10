// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyHealthBarWidget.h"
#include "AI/BaseAICharacter.h"

float UEnemyHealthBarWidget::GetOwnerHealthPercent() const
{
	// Cast the owner to our BaseAICharacter
	if (ABaseAICharacter* Enemy = Cast<ABaseAICharacter>(OwnerActor))
	{
		// Calculate and return the health percentage
		return (Enemy->Health > 0.0f) ? (Enemy->Health / 100.0f) : 0.0f;
	}

	return 0.0f;
}
