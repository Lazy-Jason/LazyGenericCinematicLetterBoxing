// Copyright (C) 2024 Job Omondiale - All Rights Reserved

#include "SubSystem/LazyLetterBoxingSubsystem.h"
#include "Settings/LazyLetterBoxingSettings.h"
#include "TimerManager.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"

void ULazyLetterBoxingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    /*InitializeLetterBoxMaterial();*/
}

void ULazyLetterBoxingSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void ULazyLetterBoxingSubsystem::InitializeLetterBoxMaterial()
{
    const ULazyLetterBoxingSettings* Settings = GetDefault<ULazyLetterBoxingSettings>();
    if (Settings == nullptr) return;

    TickRate = Settings->TickRate;
    UMaterialInterface* BaseMaterial = Cast<UMaterialInterface>(Settings->LetterBoxMaterialPath.TryLoad());
    if (BaseMaterial == nullptr) return;
    
    LetterBoxMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
    if (LetterBoxMaterial == nullptr) return;
    
    ApplyPostProcessMaterial();
}

void ULazyLetterBoxingSubsystem::ApplyPostProcessMaterial()
{
    if (LetterBoxMaterial == nullptr) return;

    const UWorld* World = GetWorld();
    if (World == nullptr) return;

    APostProcessVolume* PostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(World, APostProcessVolume::StaticClass()));
    if (PostProcessVolume == nullptr) return;

    LetterBoxMaterial->SetScalarParameterValue(FName("AspectRatio"), 0);
    PostProcessVolume->AddOrUpdateBlendable(LetterBoxMaterial);
}

void ULazyLetterBoxingSubsystem::OpenCinematicLetterBoxing(const float TransitionTime, const float AspectRatio)
{
    if (bIsLetterBoxEnabled && AspectRatio == TargetAspectRatio || LetterBoxMaterial == nullptr) return;
    
    const ULazyLetterBoxingSettings* Settings = GetDefault<ULazyLetterBoxingSettings>();
    if (Settings == nullptr) return;

    const UWorld* World = GetWorld();
    if (World == nullptr) return;

    bIsLetterBoxEnabled = true;
    TargetAspectRatio = AspectRatio > 0 ? AspectRatio : Settings->DefaultTargetAspectRatio;

    if (TransitionTime == 0) 
    {
        LetterBoxMaterial->SetScalarParameterValue(FName("AspectRatio"), TargetAspectRatio);
        return;
    }
    
    TransitionDuration = TransitionTime;
    TransitionProgress = 0.0f;

    World->GetTimerManager().SetTimer(TickTimerHandle, this, &ULazyLetterBoxingSubsystem::UpdateLetterBoxing, TickRate, true);
}

void ULazyLetterBoxingSubsystem::CloseCinematicLetterBoxing(const float TransitionTime)
{
    if (LetterBoxMaterial == nullptr) return;

    const UWorld* World = GetWorld();
    if (World == nullptr) return;

    bIsLetterBoxEnabled = false;

    if (TransitionTime == 0)
    {
        LetterBoxMaterial->SetScalarParameterValue(FName("AspectRatio"), 0);
        return;
    }
    
    TransitionDuration = TransitionTime;
    TransitionProgress = 0.0f;
    
    World->GetTimerManager().SetTimer(TickTimerHandle, this, &ULazyLetterBoxingSubsystem::UpdateLetterBoxing, TickRate, true);
}

void ULazyLetterBoxingSubsystem::UpdateLetterBoxing()
{
    if (LetterBoxMaterial == nullptr) return;

    const UWorld* World = GetWorld();
    const float DeltaTime = World ? World->GetDeltaSeconds() : TickRate;
    
    TransitionProgress += DeltaTime / TransitionDuration;
    TransitionProgress = FMath::Clamp(TransitionProgress, 0.0f, 1.0f);

    const float CurrentAspectRatio = bIsLetterBoxEnabled ?
        FMath::Lerp(0.0f, TargetAspectRatio, TransitionProgress) : 
        FMath::Lerp(TargetAspectRatio, 0.0f, TransitionProgress);

    LetterBoxMaterial->SetScalarParameterValue(FName("AspectRatio"), CurrentAspectRatio);

    if (TransitionProgress >= 1) ResetCinematicLetteringUpdates();
}

void ULazyLetterBoxingSubsystem::ResetCinematicLetteringUpdates()
{
    const UWorld* World = GetWorld();
    if (World == nullptr) return;

    TransitionDuration = 0.0f;
    TransitionProgress = 0.0f;

    World->GetTimerManager().ClearTimer(TickTimerHandle);
}