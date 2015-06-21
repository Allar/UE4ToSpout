// Copyright(c) 2015, Michael Allar
// All rights reserved.

#include "SpoutPluginPrivatePCH.h"
#include "Core.h"
#include "Engine.h"
#include "RenderCore.h"
#include "TickableObjectRenderThread.h"
#include "AllowWindowsPlatformTypes.h" 
#include "Spout.h"
#include "HideWindowsPlatformTypes.h"


class FSpoutPlugin : public ISpoutPlugin, public FTickableObjectRenderThread
{
public:
	FSpoutPlugin() : FTickableObjectRenderThread(false), SpoutSenderInstance(nullptr), SpoutReceiverInstance(nullptr), SenderSourceTexture(nullptr), bTextureReady(false) {}
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	SpoutSender* SpoutSenderInstance;
	SpoutReceiver* SpoutReceiverInstance;

public:
	virtual void SetSpoutTextureSource(UTextureRenderTarget2D* SourceTexture) override;

protected:
	UTextureRenderTarget2D* SenderSourceTexture;
	FTickerDelegate TickDelegate;
	//bool HandleTicker(float DeltaTime);
	bool bTextureReady;
};

IMPLEMENT_MODULE( FSpoutPlugin, SpoutPlugin )



void FSpoutPlugin::StartupModule()
{
	bTextureReady = false;

#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	LoadLibraryW(TEXT("../../../Engine/Binaries/ThirdParty/Spout/Win64/Spout.dll"));
#else	//32-bit platform
	LoadLibraryW(TEXT("../../../Engine/Binaries/ThirdParty/Spout/Win32/Spout.dll"));
#endif
#endif	//PLATFORM_WINDOWS

	SpoutSenderInstance = new SpoutSender;
	//SpoutSenderInstance->SetMemoryShareMode(false);
	//SpoutReceiverInstance->SetDX9(true);

	SenderSourceTexture = NewObject<UTextureRenderTarget2D>();
	SenderSourceTexture->bNeedsTwoCopies = false;
	SenderSourceTexture->InitCustomFormat(800, 600, PF_B8G8R8A8, true);
	SenderSourceTexture->AddToRoot();
	SenderSourceTexture->UpdateResource();

	bTextureReady = true;

	char SenderName[256];
	memset(&SenderName, 0, 256);
	sprintf_s(SenderName, "UE4 Spout Plugin");
	

	if (SpoutSenderInstance->CreateSender(SenderName, 800, 600, 87))
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
			RegisterSpoutPluginTick,
			FSpoutPlugin*, UnregisteredSpoutPlugin, this,
			{
				UnregisteredSpoutPlugin->Register();
			}
		);
	}
	else
	{
		UE_LOG(LogSpout, Error, TEXT("FUCK"));
	}

	
}


void FSpoutPlugin::ShutdownModule()
{
	bTextureReady = false;

	Unregister();

	if (SpoutSenderInstance)
	{
		SpoutSenderInstance->ReleaseSender();
		delete SpoutSenderInstance;
		SpoutSenderInstance = nullptr;
	}

	if (SenderSourceTexture)
	{
		SenderSourceTexture->RemoveFromRoot();
		SenderSourceTexture = nullptr;
	}
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FSpoutPlugin::SetSpoutTextureSource(UTextureRenderTarget2D* SourceTexture)
{
	bTextureReady = false;

	if (SpoutSenderInstance)
	{
		SpoutSenderInstance->ReleaseSender();
	}

	if (SenderSourceTexture)
	{
		SenderSourceTexture->RemoveFromRoot();
		SenderSourceTexture = nullptr;
	}
	SenderSourceTexture = SourceTexture;
	if (SenderSourceTexture)
	{
		SenderSourceTexture->bNeedsTwoCopies = true;
		SenderSourceTexture->InitCustomFormat(SenderSourceTexture->GetSurfaceWidth(), SenderSourceTexture->GetSurfaceHeight(), PF_B8G8R8A8, false);
		SenderSourceTexture->UpdateResource();

		char SenderName[256];
		memset(&SenderName, 0, 256);
		sprintf_s(SenderName, "UE4 Spout Plugin");

		if (SpoutSenderInstance->CreateSender(SenderName, SenderSourceTexture->GetSurfaceWidth(), SenderSourceTexture->GetSurfaceHeight(), 87))
		{
		}
		else
		{
			UE_LOG(LogSpout, Error, TEXT("FUCK"));
		}

		bTextureReady = true;
	}	
}

bool FSpoutPlugin::IsTickable() const
{
	return bTextureReady;
}

TStatId FSpoutPlugin::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FSpoutPlugin, STATGROUP_Tickables);
}

void FSpoutPlugin::Tick(float DeltaTime)
{
	if (!bTextureReady)
	{
		return;
	}

	if (SpoutSenderInstance && SenderSourceTexture && SenderSourceTexture->GetRenderTargetResource())
	{
		uint32 width = SenderSourceTexture->GetSurfaceWidth();
		uint32 height = SenderSourceTexture->GetSurfaceHeight();
		uint32 MemSize = width*height * 4;

		uint32 stride = 0;
		void* MipData = GDynamicRHI->RHILockTexture2D(
			SenderSourceTexture->GetRenderTargetResource()->GetRenderTargetTexture(), 0, RLM_ReadOnly, stride, false);
		if (MipData)
		{
			//@TODO: Figure out why sending the MipData directly won't work
			unsigned char* sendOut = new unsigned char[MemSize];
			FMemory::Memcpy(sendOut, MipData, MemSize);
			GDynamicRHI->RHIUnlockTexture2D(SenderSourceTexture->GetRenderTargetResource()->GetRenderTargetTexture(), 0, false);
			SpoutSenderInstance->SendImage(sendOut, width, height, 0x80E1, false, false);
			delete sendOut;
		}
		else
		{
			UE_LOG(LogSpout, Error, TEXT("FUCK"));
		}
	}
}

DEFINE_LOG_CATEGORY(LogSpout)


