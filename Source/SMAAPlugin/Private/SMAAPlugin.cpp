// Copyright Epic Games, Inc. All Rights Reserved.

#include "SMAAPlugin.h"

#include "SMAASceneExtension.h"
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
		SMAASceneExtension = FSceneViewExtensions::NewExtension<FSMAASceneExtension>(nullptr, nullptr);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSMAAPluginModule, SMAAPlugin)