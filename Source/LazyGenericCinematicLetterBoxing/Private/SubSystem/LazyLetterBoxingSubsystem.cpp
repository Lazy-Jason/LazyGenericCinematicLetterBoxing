// Copyright (C) 2024 Job Omondiale - All Rights Reserved

#include "SubSystem/LazyLetterBoxingSubsystem.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/LazyLetterBoxingSettings.h"

void ULazyLetterBoxingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    InitializeLetterBoxMaterial();
}

void ULazyLetterBoxingSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void ULazyLetterBoxingSubsystem::OpenCinematicLetterBoxing(const float TransitionTime, const float AspectRatio)
{
    if (bIsLetterBoxEnabled && AspectRatio == TargetAspectRatio) return; // return if already active
    
    const ULazyLetterBoxingSettings* Settings = GetDefault<ULazyLetterBoxingSettings>();
    if (!IsValid(Settings)) return; // return if not valid
    
    const UWorld* World = GetWorld();
    if (!IsValid(World)) return; // return if not valid
    bIsLetterBoxEnabled = true;
    TargetAspectRatio = AspectRatio > 0 ? AspectRatio : Settings->DefaultTargetAspectRatio;

    // if the transition is 0 instantaneously set the aspect ratio
    if(TransitionTime == 0) 
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
    const UWorld* World = GetWorld();
    if (!IsValid(World)) return; // return if not valid
    bIsLetterBoxEnabled = false;

    // if the transition is 0 instantaneously set the aspect ratio
    if(TransitionTime == 0)
    {
        LetterBoxMaterial->SetScalarParameterValue(FName("AspectRatio"), 0);
        return;
    }
    
    TransitionDuration = TransitionTime;
    TransitionProgress = 0.0f;
    
    World->GetTimerManager().SetTimer(TickTimerHandle, this, &ULazyLetterBoxingSubsystem::UpdateLetterBoxing, TickRate, true);
}

void ULazyLetterBoxingSubsystem::InitializeLetterBoxMaterial()
{
    const ULazyLetterBoxingSettings* Settings = GetDefault<ULazyLetterBoxingSettings>();
    if (!IsValid(Settings)) return; // return if not valid

    TickRate = Settings->TickRate;
    UMaterial* BaseMaterial = Cast<UMaterial>(Settings->LetterBoxMaterialPath.TryLoad());
    if (!IsValid(BaseMaterial)) return; // return if not valid
    
    LetterBoxMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
    ApplyPostProcessMaterial();
}

void ULazyLetterBoxingSubsystem::ApplyPostProcessMaterial()
{
    if (!IsValid(LetterBoxMaterial)) return;

    const UWorld* World = GetWorld();
    if (!IsValid(World)) return; // return if not valid

    APostProcessVolume* PostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(World, APostProcessVolume::StaticClass()));
    if (!IsValid(PostProcessVolume)) return; // return if not valid

    PostProcessVolume->AddOrUpdateBlendable(LetterBoxMaterial);
}

void ULazyLetterBoxingSubsystem::UpdateLetterBoxing()
{
    if (!IsValid(LetterBoxMaterial)) return; // return if not valid
    
    const float DeltaTime = GetWorld() ? GetWorld()->GetDeltaSeconds() : TickRate;
    
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
    if (!IsValid(World)) return; // return if not valid

    TransitionDuration = 0.0f;
    TransitionProgress = 0.0f;

    World->GetTimerManager().ClearTimer(TickTimerHandle);
}