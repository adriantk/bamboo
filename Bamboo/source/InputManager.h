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
#pragma once
#include "dxstdafx.h"

//-----------------------------------------------------------------------------
// CLASS DECLARATIONS
//-----------------------------------------------------------------------------
class InputManager
{
public:
	InputManager();
	~InputManager();

	HRESULT CreateWindowDependentResources(HINSTANCE hinstance, HWND hwnd);
	HRESULT Update();

	bool IsKeyDown(UINT key);

private:
	IDirectInput8*			m_pdirectInput;
	IDirectInputDevice8*	m_pkeyboard;

	UCHAR m_keys[256];
};

