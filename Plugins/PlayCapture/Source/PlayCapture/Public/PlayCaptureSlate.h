#pragma once

#include "Core.h"
#include "SCompoundWidget.h"
#include "UnrealClient.h"
#include "Runtime/Slate/Public/Widgets/Input/SComboBox.h"

class SWindow;
class SViewport;
class FSceneViewport;

/**
* Custom Viewport client, where we have access to canvas on Draw function.
* This is the place where we issue canvas draw call to present SceneCaptureComponent Texture to custom window
*/
class FPlayCaptureViewportClient : public FViewportClient
{
public:
	/** FViewportClient interface */
	/**
	* Execute Draw each tick
	* This is the place for issue draw cumments and start rendering
	*/
	virtual void Draw(FViewport* Viewport, FCanvas* Canvas) override;
	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed = 1.0f, bool bGamepad = false) override;
	virtual bool InputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples = 1, bool bGamepad = false) override;
	virtual bool InputGesture(FViewport* Viewport, EGestureEvent GestureType, const FVector2D& GestureDelta, bool bIsDirectionInvertedFromDevice) override;
	virtual UWorld* GetWorld() const override { return nullptr; }
	virtual void RedrawRequested(FViewport* Viewport) override { Viewport->Draw(); }
};

/**
 * Implements the Play Capture viewport
 */
class SPlayCaptureViewport : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPlayCaptureViewport) { }
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SViewport> Viewport;
	TSharedPtr<FSceneViewport> SceneViewport;
	TSharedPtr<FPlayCaptureViewportClient> PlayCaptureViewportClient;

public:
	// SWidget overrides
	/**
	* Tick each engine tick, this is where we call FViewport::Draw() --> FPlayCaptureViewportClient::Draw(...)
	*/
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

};

/** 
* This is main container class, which is create Play Capture Window
* And it assign rendering Viewport for the Window
*/
class FPlayCaptureSlate
{
public:
	FPlayCaptureSlate();
	~FPlayCaptureSlate();

public:
	static void Initialize();
	static void Shutdown();

private:
	TSharedPtr<SWindow> PlayCaptureWindow;
	int PlayCaptureWindowWidth;
	int PlayCaptureWindowHeight;
	FOnWindowClosed InDelegate;

private:
	/**
	* Destroy window and all rendering viewports when window has been closed
	*/
	void OnWindowClosed(const TSharedRef<SWindow>& Window);

private:
	TSharedPtr<SPlayCaptureViewport> PlayCaptureViewport;
	static TSharedPtr<FPlayCaptureSlate> PlayCaptureSlate;
};