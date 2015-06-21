// Copyright(c) 2015, Michael Allar
// All rights reserved.

#include "SpoutPluginPrivatePCH.h"

USpoutBlueprintLibrary::USpoutBlueprintLibrary(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
}

void USpoutBlueprintLibrary::SetSpoutSenderTextureSource(UTextureRenderTarget2D* SourceTexture)
{
	if (ISpoutPlugin::IsAvailable())
	{
		ISpoutPlugin::Get().SetSpoutTextureSource(SourceTexture);
	}
	
}

UTextureRenderTarget2D* USpoutBlueprintLibrary::CreateRenderTarget2DWithOneCopy(int32 Width, int32 Height)
{
	UTextureRenderTarget2D* OutputTexture = NewObject<UTextureRenderTarget2D>();
	OutputTexture->bNeedsTwoCopies = false;
	OutputTexture->InitCustomFormat(Width, Height, PF_B8G8R8A8, true);
	OutputTexture->AddToRoot();
	OutputTexture->UpdateResource();

	return OutputTexture;
}
