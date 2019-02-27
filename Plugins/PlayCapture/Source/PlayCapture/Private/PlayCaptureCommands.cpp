// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PlayCaptureCommands.h"

#define LOCTEXT_NAMESPACE "FPlayCaptureModule"

void FPlayCaptureCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "PlayCapture", "Bring up PlayCapture window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
