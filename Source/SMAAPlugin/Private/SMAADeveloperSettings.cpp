// Fill out your copyright notice in the Description page of Project Settings.


#include "SMAADeveloperSettings.h"

void USMAADeveloperSettings::LoadTextures()
{
	if (SMAAAreaTexture == nullptr && !SMAAAreaTextureName.IsNull())
	{
		SMAAAreaTexture = SMAAAreaTextureName.LoadSynchronous();
	}

	if (SMAASearchTexture == nullptr && !SMAASearchTextureName.IsNull())
	{
		SMAASearchTexture = SMAASearchTextureName.LoadSynchronous();
	}
}
