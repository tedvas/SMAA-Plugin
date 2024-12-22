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

FSMAASceneExtension::FSMAASceneExtension(const FAutoRegister& AutoReg, const FTexture2DResource* SMAAAreaTexture, const FTexture2DResource* SMAASearchTexture)
	: FSceneViewExtensionBase(AutoReg)
{
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

	//const FSceneViewFamily& ViewFamily = *View.Family;

	//// We need to make sure to take Windows and Scene scale into account.
	//float ScreenPercentage = ViewFamily.SecondaryViewFraction;

	//if (ViewFamily.GetScreenPercentageInterface())
	//{
	//	DynamicRenderScaling::TMap<float> UpperBounds = ViewFamily.GetScreenPercentageInterface()->GetResolutionFractionsUpperBound();
	//	ScreenPercentage *= UpperBounds[GDynamicPrimaryResolutionFraction];
	//}

	//const FIntRect PrimaryViewRect = UE::FXRenderingUtils::GetRawViewRectUnsafe(View);

	return FScreenPassTexture(InOutInputs.GetInput(EPostProcessMaterialInput::SceneColor));
}

