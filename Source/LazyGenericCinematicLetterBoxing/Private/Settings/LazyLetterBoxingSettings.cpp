// // Copyright (C) 2024 Job Omondiale - All Rights Reserved


#include "Settings/LazyLetterBoxingSettings.h"

ULazyLetterBoxingSettings::ULazyLetterBoxingSettings(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CategoryName = TEXT("Plugins");
	LetterBoxMaterialPath = FSoftObjectPath(TEXT("/LazyGenericCinematicLetterBoxing/Material/M_CinematicLetterBox.M_CinematicLetterBox"));
}
