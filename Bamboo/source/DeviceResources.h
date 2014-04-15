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

#include <string>
#include <memory>
//-----------------------------------------------------------------------------
// CLASS DECLARATIONS
//-----------------------------------------------------------------------------
class DeviceResources
{
public:
	DeviceResources();
	~DeviceResources();

	HRESULT CreateDeviceResources();
	HRESULT CreateWindowResources(HWND hwnd);

	HRESULT ConfigureBackBuffer();
	HRESULT ReleaseBackBuffer();
	HRESULT GoFullScreen();
	HRESULT GoWindowed();

	float GetAspectRatio();

	ID3D11Device*           GetDevice() { return m_pd3dDevice.Get(); };
	ID3D11DeviceContext*    GetDeviceContext() { return m_pd3dDeviceContext.Get(); };
	ID3D11RenderTargetView* GetRenderTarget() { return m_pRenderTarget.Get(); }
	ID3D11DepthStencilView* GetDepthStencil() { return m_pDepthStencilView.Get(); }

	void Present();

private:

	Microsoft::WRL::ComPtr<ID3D11Device>        m_pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pd3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>      m_pDXGISwapChain;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>        m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_pDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;

	D3D_FEATURE_LEVEL       m_featureLevel;
	D3D11_TEXTURE2D_DESC    m_bbDesc;
	D3D11_VIEWPORT          m_viewport;

};
