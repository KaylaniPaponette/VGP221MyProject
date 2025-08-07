// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameHUD.h"
#include "Widgets/SWeakWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	// Additional initialization code can go here

	// 2. Slates method of Making UI
	//ShowSettingsMenu();
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!CrosshairTexture) return;

	float CrosshairWidth = CrosshairTexture->GetSurfaceWidth();
	float CrosshairHeight = CrosshairTexture->GetSurfaceHeight();
	float CrosshairScale = 5.0f;

	FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D CrosshairHalfSize(CrosshairWidth * 0.5f, CrosshairHeight * 0.5f);
	FVector2D CrosshairDrawPosition(Center.X - CrosshairHalfSize.X, Center.Y - CrosshairHalfSize.Y);

	FVector2D CrosshairSize(CrosshairWidth * CrosshairScale, CrosshairHeight * CrosshairScale);

	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->GetResource(), CrosshairSize, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

void AGameHUD::ShowSettingsMenu()
{
	SettingsWidget = SNew(SSettingsWidget).MyInt(221).OwningHUD(this);
	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(SlateWidgetContainer, SWeakWidget).PossiblyNullContent(SettingsWidget.ToSharedRef()));

	PlayerOwner->bShowMouseCursor = true;
	PlayerOwner->SetInputMode(FInputModeUIOnly());
}

void AGameHUD::HideSettingsMenu()
{
	GEngine->GameViewport->RemoveViewportWidgetContent(SlateWidgetContainer.ToSharedRef());

	PlayerOwner->bShowMouseCursor = false;
	PlayerOwner->SetInputMode(FInputModeGameOnly());
}
