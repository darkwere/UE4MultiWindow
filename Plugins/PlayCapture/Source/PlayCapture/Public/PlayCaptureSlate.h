#pragma once

#include "Core.h"
#include "SCompoundWidget.h"
#include "UnrealClient.h"

#include "Runtime/Slate/Public/Widgets/Input/SComboBox.h"


class SWindow;
class SViewport;
class FSceneViewport;

class FPlayCaptureViewportClient
	: public FViewportClient
{
public:
	/** Constructor */
	FPlayCaptureViewportClient();
	~FPlayCaptureViewportClient();

	/** FViewportClient interface */
	virtual void Draw(FViewport* Viewport, FCanvas* Canvas) override;
	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed = 1.0f, bool bGamepad = false) override;
	virtual bool InputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples = 1, bool bGamepad = false) override;
	virtual bool InputGesture(FViewport* Viewport, EGestureEvent GestureType, const FVector2D& GestureDelta, bool bIsDirectionInvertedFromDevice) override;
	virtual UWorld* GetWorld() const override { return nullptr; }
	virtual void RedrawRequested(FViewport* Viewport) override { Viewport->Draw(); }

private:
};

/**
 * Implements the texture editor's view port.
 */
class SPlayCaptureViewport : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPlayCaptureViewport) { }
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);
	//void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SViewport> Viewport;
	TSharedPtr<FSceneViewport> SceneViewport;
	TSharedPtr<FPlayCaptureViewportClient> PlayCaptureViewportClient;

public:
	// SWidget overrides
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

};


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
	TSharedPtr<SPlayCaptureViewport> PlayCaptureViewport;


private:
	void OnWindowClosed(const TSharedRef<SWindow>& Window);

private:
	static TSharedPtr<FPlayCaptureSlate> PlayCaptureSlate;
};