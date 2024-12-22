// Copyright Epic Games, Inc. All Rights Reserved.

#include "SMAAPlugin.h"

#include "SMAASceneExtension.h"
#include "SMAADeveloperSettings.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

#define LOCTEXT_NAMESPACE "FSMAAPluginModule"

void FSMAAPluginModule::StartupModule()
{
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("SMAAPlugin"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/SMAAPlugin"), PluginShaderDir);

	FCoreDelegates::OnPostEngineInit.AddLambda([this]()
	{
		USMAADeveloperSettings::Get()->LoadTextures();

		UpdateExtensions();
	});
}

void FSMAAPluginModule::ShutdownModule()
{
	SMAASceneExtension.Reset();
}

void FSMAAPluginModule::UpdateExtensions()
{
	if (!SMAASceneExtension.IsValid())
	{
		UTexture2D* AreaTexture = USMAADeveloperSettings::Get()->SMAAAreaTexture;
		UTexture2D* SearchTexture = USMAADeveloperSettings::Get()->SMAASearchTexture;
		const FTexture2DResource* AreaTextureResource = nullptr;
		const FTexture2DResource* SearchTextureResource = nullptr;

		if (ensure(AreaTexture))
		{
			const auto TextureResource = AreaTexture->GetResource();

			if (TextureResource)
			{
				AreaTextureResource = TextureResource->GetTexture2DResource();
			}
		}

		if (ensure(SearchTexture))
		{
			const auto TextureResource = SearchTexture->GetResource();

			if (TextureResource)
			{
				SearchTextureResource = TextureResource->GetTexture2DResource();
			}
		}

		SMAASceneExtension = FSceneViewExtensions::NewExtension<FSMAASceneExtension>(AreaTextureResource, SearchTextureResource);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSMAAPluginModule, SMAAPlugin)