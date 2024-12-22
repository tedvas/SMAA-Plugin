// Fill out your copyright notice in the Description page of Project Settings.


#include "SMAASceneExtension.h"

#include "SceneView.h"
#include "ScreenPass.h"
#include "CommonRenderResources.h"
#include "PostProcess/PostProcessing.h"
#include "PostProcess/PostProcessMaterial.h"
#include "ScenePrivate.h"
#include "Engine/TextureRenderTarget2D.h"
#include "DynamicResolutionState.h"
#include "FXRenderingUtils.h"
#include "Rendering/Texture2DResource.h"

#include "PostProcess/PostProcessSMAA.h"

TAutoConsoleVariable<int32> CVarSMAAEnabled(
	TEXT("r.SMAA"), 0,
	TEXT(" 0 - off\n")
	TEXT(" 1 - on"),
	ECVF_RenderThreadSafe);

FSMAASceneExtension::FSMAASceneExtension(const FAutoRegister& AutoReg, FTexture2DResource* InSMAAAreaTexture, FTexture2DResource* InSMAASearchTexture)
	: FSceneViewExtensionBase(AutoReg)
	, SMAAAreaTexture(InSMAAAreaTexture)
	, SMAASearchTexture(InSMAASearchTexture)
{
	//check(SMAAAreaTexture)
}

void FSMAASceneExtension::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
	GetOrCreateViewData(InView);
}

TSharedPtr<FSMAAViewData> FSMAASceneExtension::GetOrCreateViewData(const FSceneView& InView)
{
	if (InView.State == nullptr)
	{
		return nullptr;
	}

	const uint32 Index = InView.State->GetViewKey();
	TSharedPtr<FSMAAViewData>& ViewData = ViewDataMap.FindOrAdd(Index);
	if (!ViewData.IsValid())
	{
		ViewData = MakeShared<FSMAAViewData>();
		ViewData->SMAAAreaTexture = SMAAAreaTexture;
		ViewData->SMAASearchTexture = SMAASearchTexture;
	}
	return ViewData;
}

bool FSMAASceneExtension::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const
{
	return CVarSMAAEnabled.GetValueOnAnyThread() == 1;
}

void FSMAASceneExtension::SubscribeToPostProcessingPass(EPostProcessingPass Pass, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
{
	if (Pass == EPostProcessingPass::FXAA)
	{
		InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FSMAASceneExtension::PostProcessPass_RenderThread, Pass));
	}
}

FScreenPassTexture FSMAASceneExtension::PostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& InOutInputs, EPostProcessingPass Pass)
{
	InOutInputs.Validate();

	SMAAAreaTexture->InitRHI(GetImmediateCommandList_ForRenderCommand());
	SMAASearchTexture->InitRHI(GetImmediateCommandList_ForRenderCommand());

	auto& SceneTextureParameters = InOutInputs.SceneTextures.SceneTextures;

	{
		auto PredicateSource = GetPredicateSource();

		FSMAAInputs PassInputs;
		//PassSequence.AcceptOverrideIfLastPass(EPass::SMAA, PassInputs.OverrideOutput);
		PassInputs.SceneColor = FScreenPassTexture(InOutInputs.GetInput(EPostProcessMaterialInput::SceneColor));
		PassInputs.SceneVelocity = FScreenPassTexture(InOutInputs.GetInput(EPostProcessMaterialInput::Velocity));
		PassInputs.Quality = GetSMAAPreset();
		PassInputs.EdgeMode = GetSMAAEdgeDetectors();
		PassInputs.PredicationSource = PredicateSource;
		PassInputs.MaxSearchSteps = GetSMAAMaxSearchSteps();
		PassInputs.MaxDiagonalSearchSteps = GetSMAAMaxDiagonalSearchSteps();
		PassInputs.CornerRounding = GetSMAACornerRounding();
		PassInputs.AdaptationFactor = GetSMAAAdaptationFactor();
		PassInputs.ReprojectionWeight = GetSMAAReprojectionWeight();
		PassInputs.PredicationThreshold = GetSMAAPredicationThreshold();
		PassInputs.PredicationScale = GetSMAAPredicationScale();
		PassInputs.PredicationStrength = GetSMAAPredicationStrength();
		PassInputs.TemporalHistoryBias = GetSMAATemporalHistoryBias();

		check(View.bIsViewInfo);
		auto SceneColorSlice = FScreenPassTextureSlice::CreateFromScreenPassTexture(GraphBuilder, AddSMAAPasses(GraphBuilder, (const FViewInfo&)View, PassInputs, InOutInputs, GetOrCreateViewData(View).ToSharedRef()));
		return FScreenPassTexture(SceneColorSlice);
	}

	return FScreenPassTexture(InOutInputs.GetInput(EPostProcessMaterialInput::SceneColor));
}

