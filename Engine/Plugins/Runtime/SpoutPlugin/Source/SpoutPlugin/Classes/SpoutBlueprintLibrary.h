// Copyright(c) 2015, Michael Allar
// All rights reserved.

#pragma once
#include "SpoutBlueprintLibrary.Generated.h"

// Library of helper functions for UMGEx Widgets
UCLASS()
class SPOUTPLUGIN_API USpoutBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "Spout")
	static void SetSpoutSenderTextureSource(UTextureRenderTarget2D* SourceTexture);

	UFUNCTION(BlueprintCallable, Category = "Spout")
	static UTextureRenderTarget2D* CreateRenderTarget2DWithOneCopy(int32 Width, int32 Height);

};