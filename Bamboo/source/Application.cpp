//-----------------------------------------------------------------------------
// Licensed under the EUPL, Version 1.1 (the "Licence");
// You may not use this work except in compliance with the Licence.
// You may obtain a copy of the Licence at :
//
// http ://ec.europa.eu/idabc/eupl5
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) 2014 Adrian Kelly
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PREPROCESSOR DIRECTIVES
//-----------------------------------------------------------------------------
#include "dxstdafx.h"

#include <string>
#include <memory>

#include "MainClass.h"

//-----------------------------------------------------------------------------
// ENTRY POINT
//-----------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HRESULT hr = S_OK;

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::shared_ptr<MainClass> winMain = std::shared_ptr<MainClass>(new MainClass());
	hr = winMain->CreateDesktopWindow();

	if (SUCCEEDED(hr))
	{
		std::shared_ptr<DeviceResources> deviceResources = std::shared_ptr<DeviceResources>(new DeviceResources());
		deviceResources->CreateDeviceResources();

		std::shared_ptr<Renderer> renderer = std::shared_ptr<Renderer>(new Renderer(deviceResources));
		renderer->CreateDeviceDependentResources();

		deviceResources->CreateWindowResources(winMain->GetWindowHandle());
		renderer->CreateWindowSizeDependentResources();


		// Run the program.
		hr = winMain->Run(deviceResources, renderer);
	}

	// Cleanup is handled in destructors.
	return hr;
}