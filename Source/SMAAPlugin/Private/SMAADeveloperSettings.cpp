// Fill out your copyright notice in the Description page of Project Settings.


#include "SMAADeveloperSettings.h"

PRAGMA_DISABLE_OPTIMIZATION
void USMAADeveloperSettings::LoadTextures()
{
	if (SMAAAreaTexture == nullptr && !SMAAAreaTextureName.IsNull())
	{
		SMAAAreaTexture = SMAAAreaTextureName.LoadSynchronous();
		SMAAAreaTexture->AddToRoot();
	}

	if (SMAASearchTexture == nullptr && !SMAASearchTextureName.IsNull())
	{
		SMAASearchTexture = SMAASearchTextureName.LoadSynchronous();
		SMAASearchTexture->AddToRoot();
	}
}
PRAGMA_ENABLE_OPTIMIZATION