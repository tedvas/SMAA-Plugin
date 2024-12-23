// [TEMPLATE]

#pragma once

#include "ScreenPass.h"

//DEFINE_LOG_CATEGORY_STATIC(LogSMAA, Warning, All);

enum class ESMAAEdgeDetectors : uint8
{
	Depth,
	Luminance,
	Colour,
	Normal,

	MAX UMETA(HIDDEN)
};

enum class ESMAAPreset : uint8
{
	Low,
	Medium,
	High,
	Ultra,

	MAX UMETA(HIDDEN)
};

enum class ESMAAPredicationTexture : uint8
{
	None,
	Depth,
	WorldNormal,
	MRS,

	MAX UMETA(HIDDEN)
};

ESMAAPreset GetSMAAPreset();
ESMAAEdgeDetectors GetSMAAEdgeDetectors();
ESMAAPredicationTexture GetPredicateSource();

uint8 GetSMAAMaxSearchSteps();
uint8 GetSMAAMaxDiagonalSearchSteps();
uint8 GetSMAACornerRounding();

float GetSMAAAdaptationFactor();
float GetSMAAReprojectionWeight();
float GetSMAAPredicationThreshold();
float GetSMAAPredicationScale();
float GetSMAAPredicationStrength();
float GetSMAATemporalHistoryBias();


struct FSMAAInputs
{
	// [Optional] Render to the specified output. If invalid, a new texture is created and returned.
	FScreenPassRenderTarget OverrideOutput;

	// [Required] HDR scene color to filter.
	FScreenPassTexture SceneColor;

	// [Required] Depth Buffer.
	//FScreenPassTexture SceneDepth;

	// [Required] Scene Velocity.
	FScreenPassTexture SceneVelocity;

	// [Optional] Predicate
	//FScreenPassTexture PredicateTexture;


	//FScreenPassTexture WorldNormal;

	// SMAA quality.
	ESMAAPreset Quality = ESMAAPreset::MAX;

	// SMAA Edge Detectors
	// What data are we using to detect edges
	ESMAAEdgeDetectors EdgeMode = ESMAAEdgeDetectors::MAX;

	ESMAAPredicationTexture PredicationSource = ESMAAPredicationTexture::MAX;

	// SMAA Max Search Steps
	uint8 MaxSearchSteps = UINT8_MAX;

	// SMAA Max Diagonal Search Steps
	uint8 MaxDiagonalSearchSteps = UINT8_MAX;

	// SMAA Corner Roundness
	uint8 CornerRounding = UINT8_MAX;

	float AdaptationFactor = 2.f;

	// SMAA Reprojection
	float ReprojectionWeight = 30.f;

	float PredicationThreshold = 0.01f;

	float PredicationScale = 2.f;

	float PredicationStrength = 0.4f;

	float TemporalHistoryBias = 0.5f;

};

FScreenPassTexture AddSMAAPasses(FRDGBuilder& GraphBuilder, const FViewInfo& View, const FSMAAInputs& Inputs, const struct FPostProcessMaterialInputs& InOutInputs, TSharedRef<struct FSMAAViewData> ViewData);

FScreenPassTexture AddVisualizeSMAAPasses(FRDGBuilder& GraphBuilder, const FViewInfo& View, const FSMAAInputs& Inputs, const struct FPostProcessMaterialInputs& InOutInputs, TSharedRef<struct FSMAAViewData> ViewData);
