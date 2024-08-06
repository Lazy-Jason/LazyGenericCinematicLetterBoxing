// // Copyright (C) 2024 Job Omondiale - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LazyLetterBoxingSettings.generated.h"

/**
 * 
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Lazy Cinematic Letter Boxing"))
class LAZYGENERICCINEMATICLETTERBOXING_API ULazyLetterBoxingSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	ULazyLetterBoxingSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(config, EditAnywhere, Category = "Letter Boxing", meta = (AllowedClasses = "Material"))
	FSoftObjectPath LetterBoxMaterialPath;

	UPROPERTY(config, EditAnywhere, Category = "Letter Boxing", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DefaultTargetAspectRatio = 0.1;

	UPROPERTY(config, EditAnywhere, Category = "Letter Boxing", meta = (ClampMin = "1.0", ClampMax = "3.0"))
	float TickRate = 0.016f;
};
