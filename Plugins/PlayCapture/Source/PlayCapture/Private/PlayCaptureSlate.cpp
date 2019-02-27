#include "PlayCaptureSlate.h"
#include "SlateApplication.h"

#include "Engine.h"

#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Runtime/Engine/Public/Slate/SceneViewport.h"
#include "SViewport.h"
#include "SToolTip.h"
#include "SWindow.h"

#include "IPlayCapture.h"



FPlayCaptureViewportClient::FPlayCaptureViewportClient()
{
}

FPlayCaptureViewportClient::~FPlayCaptureViewportClient()
{
}

void FPlayCaptureViewportClient::Draw(FViewport * Viewport, FCanvas * Canvas)
{
	Canvas->Clear(FLinearColor::Gray);

	auto TextRenderTarget2D = IPlayCapture::Get().GetTextureRenderTarget2D();

	if (TextRenderTarget2D.IsValid() && TextRenderTarget2D->Resource != nullptr)
	{
		FCanvasTileItem TileItem(FVector2D(0, 0), TextRenderTarget2D->Resource, 
			FVector2D(Viewport->GetRenderTargetTexture()->GetSizeX(), Viewport->GetRenderTargetTexture()->GetSizeY()),
			FLinearColor::White);
		TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Opaque;
		Canvas->DrawItem(TileItem);
	}
}

bool FPlayCaptureViewportClient::InputKey(FViewport * Viewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
{
	return false;
}

bool FPlayCaptureViewportClient::InputAxis(FViewport * Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	return false;
}

bool FPlayCaptureViewportClient::InputGesture(FViewport * Viewport, EGestureEvent GestureType, const FVector2D & GestureDelta, bool bIsDirectionInvertedFromDevice)
{
	return false;
}


void SPlayCaptureViewport::Construct(const FArguments& InArgs)
{
	Viewport = SNew(SViewport)
		.IsEnabled(true)
		.EnableGammaCorrection(false)// Gamma correction in the game is handled in post processing in the scene renderer
		.ShowEffectWhenDisabled(false)
		.EnableBlending(true)
		.ToolTip(SNew(SToolTip).Text(FText::FromString("SPlayCaptureViewport")));


	PlayCaptureViewportClient = MakeShareable(new FPlayCaptureViewportClient());
	SceneViewport = MakeShareable(new FSceneViewport(PlayCaptureViewportClient.Get(), Viewport));
	Viewport->SetViewportInterface(SceneViewport.ToSharedRef());

	this->ChildSlot
		[
			Viewport.ToSharedRef()
		];
}

void SPlayCaptureViewport::Tick(const FGeometry & AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SceneViewport->Draw();
}


TSharedPtr<FPlayCaptureSlate> FPlayCaptureSlate::PlayCaptureSlate = nullptr;

FPlayCaptureSlate::FPlayCaptureSlate()
	: PlayCaptureWindowWidth(1280)
	, PlayCaptureWindowHeight(720)
{
	// Create SWindow
	PlayCaptureWindow = SNew(SWindow)
		.Title(FText::FromString("PlayCaptureWindow"))
		.ScreenPosition(FVector2D(0, 0))
		.ClientSize(FVector2D(PlayCaptureWindowWidth, PlayCaptureWindowHeight))
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		.UseOSWindowBorder(true)
		.SaneWindowPlacement(false)
		.SizingRule(ESizingRule::UserSized);

	//PlayCaptureWindow->SetViewportSizeDrivenByWindow(true);
	FSlateApplication::Get().AddWindow(PlayCaptureWindow.ToSharedRef());
	InDelegate.BindRaw(this, &FPlayCaptureSlate::OnWindowClosed);
	PlayCaptureWindow->SetOnWindowClosed(InDelegate);


	PlayCaptureViewport = SNew(SPlayCaptureViewport);
	
	PlayCaptureWindow->SetContent(PlayCaptureViewport.ToSharedRef());
}

FPlayCaptureSlate::~FPlayCaptureSlate()
{
	InDelegate.Unbind();
}

void FPlayCaptureSlate::Initialize()
{
	if (!PlayCaptureSlate.IsValid())
	{
		FPlayCaptureSlate::PlayCaptureSlate = MakeShareable(new FPlayCaptureSlate());
	}
}

void FPlayCaptureSlate::Shutdown()
{
	if (PlayCaptureSlate.IsValid())
	{
		FPlayCaptureSlate::PlayCaptureSlate.Reset();
		FPlayCaptureSlate::PlayCaptureSlate = nullptr;;
	}
}

void FPlayCaptureSlate::OnWindowClosed(const TSharedRef<SWindow>& Window)
{
	FPlayCaptureSlate::Shutdown();
}

