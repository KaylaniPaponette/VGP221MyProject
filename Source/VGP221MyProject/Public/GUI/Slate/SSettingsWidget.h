// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/GameHUD.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class VGP221MYPROJECT_API SSettingsWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSettingsWidget)
		{
		}
		SLATE_ARGUMENT(int, MyInt);
		SLATE_ARGUMENT(TWeakObjectPtr<class AGameHUD>, OwningHUD);
	SLATE_END_ARGS()

	int MyInt;
	TWeakObjectPtr<class AGameHUD> OwningHUD;

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply OnResumeClicked() const;
	FReply OnSettingsClicked() const;
	FReply OnQuitClicked() const;
};

//{
//public:
//	SLATE_BEGIN_ARGS(SSettingsWidget)
//	{}
//	SLATE_END_ARGS()
//
//	/*
//		---		Smart Pointers in Unreal Engine (UE4/UE5) Intellisense		---
//		Smart Pointers are used to manage the lifetime of objects in Unreal Engine.
//
//		TUniquePtr is a smart pointer that provides exclusive ownership of an object.
//
//		SoftPtr is a smart pointer that can be used to reference an object that may not be loaded yet, allowing for lazy loading.
//
//		SoftObjectPtr is a smart pointer that can be used to reference an object that may not be loaded yet, allowing for lazy loading of UObject-derived classes.
//		- TSoftObjectPtr<Texture> Crosshair;
//			- Crosshair.LoadSynchronous() can be used to load the texture synchronously when needed.
//
//		SharedPtr is a smart pointer that allows multiple references to the same object, enabling shared ownership.
//		SharedRef is a smart pointer that allows multiple references to the same object, enabling shared ownership with reference counting.
//		- TSharedPtr<FMyClass> MySharedPtr;
//		- TSharedRef<FMyClass> MySharedRef;
//		- TSharedPtr<FMyClass> MySharedPtr = MakeShareable(new FMyClass());
//		- TSharedRef<FMyClass> MySharedRef = MakeShareable(new FMyClass());
//		- MakeShareable is a function that creates a shared pointer from a raw pointer.
//		- MakeShared is a function that creates a shared pointer from a raw pointer and allocates memory for the object.
//
//		WeakPtr is a smart pointer that allows for weak references to an object, meaning it does not affect the object's lifetime. (Temporarily holds a reference to an object without preventing it from being destroyed.)
//
//		--- DANNY'S NOTES ON SMART POINTERS ---
//		Smart Pointers
//
//		TUniquePtr - Sole Ownership
//			- Resource management small stuff
//
//		SoftObjectPtr - Lazy loading
//			- Assets that you may not want loaded yet in memory
//			- TSoftObjectPtr<Texture> Crosshair;
//				- Crosshair.LoadSynchonourly();
//
//		SharedPtr/SharedRef - Shared ownership
//			- Want multiple objects to be responsible for this memory
//
//		WeakPtr - No ownership
//			- Temporary access
//	*/
//
//	/** Constructs this widget with InArgs */
//	void Construct(const FArguments& InArgs);
//
//	FReply OnResumeClicked()
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Resume button clicked!"));
//		// Logic to resume the game or close the settings menu
//		return FReply::Handled();
//	}
//	FReply OnSettingsClicked()
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Settings button clicked!"));
//		// Logic to open the settings menu
//		return FReply::Handled();
//	}
//	FReply OnQuitClicked()
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Quit button clicked!"));
//		// Logic to quit the game
//		return FReply::Handled();
//	}
//
//};
