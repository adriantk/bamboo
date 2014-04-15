//-----------------------------------------------------------------------------
// Licensed under the EUPL, Version 1.1 (the "Licence");
// You may not use this work except in compliance with the Licence.
// You may obtain a copy of the Licence at :
//
// http ://ec.europa.eu/idabc/eupl5

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
#pragma once
#include "dxstdafx.h"
#include "DeviceResources.h"
#include "Renderer.h"

#include <string>
#include <memory>

//-----------------------------------------------------------------------------
// CLASS DECLARATIONS
//-----------------------------------------------------------------------------

class MainClass
{
public:
	MainClass();
	~MainClass();

	HRESULT CreateDesktopWindow();

	HWND GetWindowHandle() { return m_hWnd; };

	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT Run(std::shared_ptr<DeviceResources> deviceResources, std::shared_ptr<Renderer> renderer);

private:
	HMENU     m_hMenu;
	RECT      m_rc;
	HWND      m_hWnd;

};

// These are STATIC because this sample only creates one window.
static HINSTANCE m_hInstance;
static std::wstring m_windowClassName;