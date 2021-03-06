// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IPlayCapture.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

class FToolBarBuilder;
class FMenuBuilder;

/**
* Plagin Module class
*/
class FPlayCaptureModule : public IPlayCapture
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** IPlayCapture implementation */
	virtual TWeakObjectPtr<UTextureRenderTarget2D> GetTextureRenderTarget2D() override;
	virtual void SetTextureRenderTarget2D(UTextureRenderTarget2D* TexRenderTarget2D) override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:
	/**
	* Play Capture button handler
	*/
	void AddToolbarExtension(FToolBarBuilder& Builder);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TWeakObjectPtr<UTextureRenderTarget2D> TextureRenderTarget2D;
};
