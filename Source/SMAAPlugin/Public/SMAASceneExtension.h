// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SceneViewExtension.h"

// Structure in charge of storing all information about SMAA's history.
struct SMAAPLUGIN_API FSMAAHistory
{
	TRefCountPtr<IPooledRenderTarget> PastFrame;

	// Reference size of RT. Might be different than RT's actual size to handle down res.
	FIntPoint ReferenceBufferSize;

	// Viewport coordinate of the history in RT according to ReferenceBufferSize.
	FIntRect ViewportRect;

	void SafeRelease()
	{
		*this = FSMAAHistory();
	}

	bool IsValid() const
	{
		return PastFrame.IsValid();
	}

	//uint64 GetGPUSizeBytes(bool bLogSizes) const
	//{
	//	return GetRenderTargetGPUSizeBytes(PastFrame, bLogSizes);
	//}
};

struct SMAAPLUGIN_API FSMAAViewData : public TSharedFromThis<FSMAAViewData, ESPMode::ThreadSafe>
{
	// SMAA Specific Textures
	const FTexture2DResource* SMAAAreaTexture;
	const FTexture2DResource* SMAASearchTexture;

	FSMAAHistory SMAAHistory;

	virtual ~FSMAAViewData() {};
};

/**
 * 
 */
class SMAAPLUGIN_API FSMAASceneExtension : public FSceneViewExtensionBase
{
public:
	FSMAASceneExtension(const FAutoRegister& AutoReg, const FTexture2DResource* SMAAAreaTexture, const FTexture2DResource* SMAASearchTexture);

	/**
	 * Called on game thread when creating the view family.
	 */
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) {};

	/**
	 * Called on game thread when creating the view.
	 */
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) {};

	/**
     * Called on game thread when view family is about to be rendered.
     */
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) {};

	/**
	* This will be called at the beginning of post processing to make sure that each view extension gets a chance to subscribe to an after pass event.
	*/
	virtual void SubscribeToPostProcessingPass(EPostProcessingPass Pass, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled);


protected:
	virtual FScreenPassTexture PostProcessPass_RenderThread(
		FRDGBuilder& GraphBuilder,
		const FSceneView& View,
		const FPostProcessMaterialInputs& InOutInputs,
		EPostProcessingPass Pass
	);

	// SMAA Specific Textures
	const FTexture2DResource* SMAAAreaTexture;
	const FTexture2DResource* SMAASearchTexture;

	TMap<uint32, TSharedPtr<FSMAAViewData>> ViewDataMap;
};
