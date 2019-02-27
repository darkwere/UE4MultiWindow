// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PlayCapture.h"
#include "PlayCaptureStyle.h"
#include "PlayCaptureCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "PlayCaptureSlate.h"


static const FName PlayCaptureTabName("PlayCapture");

#define LOCTEXT_NAMESPACE "FPlayCaptureModule"


void FPlayCaptureModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FPlayCaptureStyle::Initialize();
	FPlayCaptureStyle::ReloadTextures();

	FPlayCaptureCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPlayCaptureCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPlayCaptureModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FPlayCaptureModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FPlayCaptureModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FPlayCaptureModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPlayCaptureStyle::Shutdown();

	FPlayCaptureCommands::Unregister();

	// Disable windows
	FPlayCaptureSlate::Shutdown();
}

TWeakObjectPtr<UTextureRenderTarget2D> FPlayCaptureModule::GetTextureRenderTarget2D()
{
	return TextureRenderTarget2D;
}

void FPlayCaptureModule::SetTextureRenderTarget2D(UTextureRenderTarget2D * TexRenderTarget2D)
{
	TextureRenderTarget2D = TexRenderTarget2D;
}


void FPlayCaptureModule::PluginButtonClicked()
{
	// Init Window
	FPlayCaptureSlate::Initialize();
}

void FPlayCaptureModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FPlayCaptureCommands::Get().OpenPluginWindow);
}

void FPlayCaptureModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FPlayCaptureCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPlayCaptureModule, PlayCapture)