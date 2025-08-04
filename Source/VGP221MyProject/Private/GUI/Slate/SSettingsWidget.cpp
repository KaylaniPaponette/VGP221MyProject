// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/Slate/SSettingsWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSettingsWidget::Construct(const FArguments& InArgs)
{
	MyInt = InArgs._MyInt;
	OwningHUD = InArgs._OwningHUD;

	// Padding
	const FMargin ContentPadding = FMargin(500.0f, 300.0f);
	const FMargin ButtonPadding = FMargin(10.0f);

	// Title
	FSlateFontInfo TitleTextStyle = FCoreStyle::Get().GetFontStyle("EmbassedText");
	TitleTextStyle.Size = 60.0f;

	const FText TitleText = FText::FromString("PAUSE MENU");

	// Buttons
	FSlateFontInfo ButtonTextStyle = TitleTextStyle;
	ButtonTextStyle.Size = 40.0f;

	const FText ResumeText = FText::FromString("RESUME");
	const FText SettingsText = FText::FromString("SETTINGS");
	const FText QuitText = FText::FromString("QUIT");

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
						.ColorAndOpacity(FColor::Black)
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(ContentPadding)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						[
							SNew(STextBlock)
								.Font(TitleTextStyle)
								.Text(TitleText)
								.Justification(ETextJustify::Center)
						]
						// Resume Button
						+ SVerticalBox::Slot()
						.Padding(ButtonPadding)
						[
							SNew(SButton)
								.OnClicked(this, &SSettingsWidget::OnResumeClicked)
								[
									SNew(STextBlock)
										.Font(ButtonTextStyle)
										.Text(ResumeText)
										.Justification(ETextJustify::Center)
								]
						]
					// Settings
					+ SVerticalBox::Slot()
						.Padding(ButtonPadding)
						[
							SNew(SButton)
								.OnClicked(this, &SSettingsWidget::OnSettingsClicked)
								[
									SNew(STextBlock)
										.Font(ButtonTextStyle)
										.Text(SettingsText)
										.Justification(ETextJustify::Center)
								]
						]
					// Quit Button
					+ SVerticalBox::Slot()
						.Padding(ButtonPadding)
						[
							SNew(SButton)
								.OnClicked(this, &SSettingsWidget::OnQuitClicked)
								[
									SNew(STextBlock)
										.Font(ButtonTextStyle)
										.Text(QuitText)
										.Justification(ETextJustify::Center)
								]
						]
				]
		];

}
FReply SSettingsWidget::OnResumeClicked() const
{
	UE_LOG(LogTemp, Warning, TEXT("Resume Clicked"));
	if (OwningHUD.IsValid()) {
		OwningHUD->HideSettingsMenu();
	}

	return FReply::Handled();
}
FReply SSettingsWidget::OnSettingsClicked() const
{
	UE_LOG(LogTemp, Warning, TEXT("Settings Clicked"));
	UE_LOG(LogTemp, Warning, TEXT("MyInt: %i"), MyInt);
	return FReply::Handled();
}
FReply SSettingsWidget::OnQuitClicked() const
{
	UE_LOG(LogTemp, Warning, TEXT("Quit Clicked"));
	if (OwningHUD.IsValid()) {
		if (APlayerController* PC = OwningHUD->PlayerOwner) {
			PC->ConsoleCommand("quit");
		}
	}
	return FReply::Handled();
}

//	// Padding
//	const FMargin ContentPadding(50.0f, 50.0f, 50.0f, 50.0f);
//
//	// Title
//	FSlateFontInfo TitleTextStyle = FCoreStyle::Get().GetFontStyle("EmbassedText");
//	TitleTextStyle.Size = 60.0f;
//
//	const FText TitleText = FText::FromString("Settings Menu");
//
//	// Button
//	FSlateFontInfo ButtonTextStyle = TitleTextStyle;
//	ButtonTextStyle.Size = 40.0f;
//
//	const FText ResumeText = FText::FromString("Settings Menu");
//	
//	ChildSlot
//		[
//				SNew(SOverlay)
//				+ SOverlay::Slot()
//				.HAlign(HAlign_Fill)
//				.VAlign(VAlign_Fill)
//				[
//					SNew(SImage)
//						.ColorAndOpacity(FColor::Black)
//				]
//				+ SOverlay::Slot()
//				.HAlign(HAlign_Fill)
//				.VAlign(VAlign_Fill)
//				.Padding(ContentPadding)
//				[
//					SNew(SVerticalBox)
//					+ SVerticalBox::Slot()
//					.AutoHeight()
//					.HAlign(HAlign_Center)
//					.Padding(10.0f)
//					[
//						SNew(STextBlock)
//						.Text(TitleText)
//						.Font(TitleTextStyle)
//						.ColorAndOpacity(FColor::White)
//					]
//					+ SVerticalBox::Slot()
//					.AutoHeight()
//					.HAlign(HAlign_Center)
//					.Padding(10.0f)
//
//				]
//		];
//}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
