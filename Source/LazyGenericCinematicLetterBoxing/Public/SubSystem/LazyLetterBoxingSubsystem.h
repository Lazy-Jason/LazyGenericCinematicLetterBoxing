// Copyright (C) 2024 Job Omondiale - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LazyLetterBoxingSubsystem.generated.h"

/**
 * ULazyLetterBoxingSubsystem handles the cinematic letterboxing effect for a game.
 */
UCLASS()
class LAZYGENERICCINEMATICLETTERBOXING_API ULazyLetterBoxingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	// Handle to the post process material
	UPROPERTY()
	UMaterialInstanceDynamic* LetterBoxMaterial = nullptr;

	// Current letterbox state
	bool bIsLetterBoxEnabled = false;

	// Target aspect ratio
	float TargetAspectRatio = 0.1f;

	// Tick rate
	float TickRate = 0.016f;

	// Local variables for managing transition state
	float TransitionDuration = 0.0f;
	float TransitionProgress = 0.0f;
	float CurrentDuration = 0.0f;
	FTimerHandle TickTimerHandle;

public:
	/**
	 * Initialize the subsystem.
	 * @param Collection The subsystem collection to which this subsystem belongs.
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * Deinitialize the subsystem.
	 */
	virtual void Deinitialize() override;

	/**
	 * Smoothly transition to the target aspect ratio specified or uses the default project setting.
	 * @param TransitionTime The time over which the transition should occur.
	 * @param AspectRatio The target aspect ratio for the letterboxing effect. Must be between 0 - 1.
	 * 0 being fully covered with the letterbox on the screen and 1 being fully visible.
	 */
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"), Category = "Lazy Cinematic Letter Boxing")
	void OpenCinematicLetterBoxing(float TransitionTime = 0.5f, float AspectRatio = 0.0f);

	/**
	 * Smoothly transition to removing the letterbox effect, returning the aspect ratio to zero.
	 * @param TransitionTime The time over which the transition should occur.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lazy Cinematic Letter Boxing")
	void CloseCinematicLetterBoxing(float TransitionTime = 0.5f);

	/**
	 * Get the current letterbox state.
	 * @return True if letterboxing is enabled, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Lazy Cinematic Letter Boxing")
	bool IsLetterBoxingEnabled() const { return bIsLetterBoxEnabled; }

private:
	/**
	 * Create and initialize the letterbox material.
	 */
	void InitializeLetterBoxMaterial();

	/**
	 * Apply the post process material to the post process volume.
	 */
	void ApplyPostProcessMaterial();

	/**
	 * Update the effect based on the current transition progress.
	 */
	UFUNCTION()
	void UpdateLetterBoxing();

	/**
	 * Reset the cinematic letterboxing state once the transition is complete.
	 */
	void ResetCinematicLetteringUpdates();
};
