// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SMAADeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class SMAAPLUGIN_API USMAADeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	/** Area Texture used by SMAA. */
	UPROPERTY()
	TObjectPtr<class UTexture2D> SMAAAreaTexture;

	///** Path of the Area Texture used by SMAA. */
	//UPROPERTY(globalconfig)
	//FSoftObjectPath SMAAAreaTextureName;

	/** Search Texture used by SMAA. */
	UPROPERTY()
	TObjectPtr<class UTexture2D> SMAASearchTexture;

	///** Path of the Search Texture used by SMAA. */
	//UPROPERTY(globalconfig)
	//FSoftObjectPath SMAASearchTextureName;
};
