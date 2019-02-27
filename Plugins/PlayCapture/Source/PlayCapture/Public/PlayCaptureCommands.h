// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "PlayCaptureStyle.h"

class FPlayCaptureCommands : public TCommands<FPlayCaptureCommands>
{
public:

	FPlayCaptureCommands()
		: TCommands<FPlayCaptureCommands>(TEXT("PlayCapture"), NSLOCTEXT("Contexts", "PlayCapture", "PlayCapture Plugin"), NAME_None, FPlayCaptureStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};