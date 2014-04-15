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
#include "InputManager.h"

//-----------------------------------------------------------------------------
// CLASS FUNCTIONS
//-----------------------------------------------------------------------------
InputManager::InputManager()
{

}


InputManager::~InputManager()
{
	if (m_pkeyboard)
	{
		m_pkeyboard->Unacquire();
		m_pkeyboard->Release();
		m_pkeyboard = nullptr;
	}

	if (m_pdirectInput)
	{
		m_pdirectInput->Release();
		m_pdirectInput = nullptr;
	}
}


HRESULT InputManager::CreateWindowDependentResources(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT hr;

	hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pdirectInput, NULL);
	hr = m_pdirectInput->CreateDevice(GUID_SysKeyboard, &m_pkeyboard, NULL);
	hr = m_pkeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pkeyboard->Acquire();

	return hr;
}


HRESULT InputManager::Update()
{
	HRESULT hr;
	hr = m_pkeyboard->GetDeviceState(sizeof(m_keys), (LPVOID)&m_keys);

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
			m_pkeyboard->Acquire();
		else
			return E_FAIL;

	}

	return S_OK;
}


bool InputManager::IsKeyDown(UINT key)
{
	if (m_keys[key] & 0x80)
		return true;
	
	return false;
}