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
#include "DeviceResources.h"

//-----------------------------------------------------------------------------
// CLASS FUNCTIONS
//-----------------------------------------------------------------------------
DeviceResources::DeviceResources()
{

}


DeviceResources::~DeviceResources()
{

}


HRESULT DeviceResources::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1
	};

	// This flag adds support for surfaces with a color-channel ordering different
	// from the API default. It is required for compatibility with Direct2D.
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	Microsoft::WRL::ComPtr<ID3D11Device>        device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags,
		levels, ARRAYSIZE(levels), D3D11_SDK_VERSION, &device, &m_featureLevel, &context);

	if (FAILED(hr))
	{
		// Handle device interface creation failure if it occurs.
		// For example, reduce the feature level requirement, or fail over 
		// to WARP rendering.
	}

	// Store pointers to the d3d11 device and dcontext
	device.As(&m_pd3dDevice);
	context.As(&m_pd3dDeviceContext);

	return hr;
}


HRESULT DeviceResources::CreateWindowResources(HWND hWnd)
{
	HRESULT hr = S_OK;


	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.Windowed = TRUE; // Sets the initial state of full-screen mode.
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;      //multisampling setting
	desc.SampleDesc.Quality = 0;    //vendor-specific flag
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.OutputWindow = hWnd;

	// Create the DXGI device object to use in other factories, such as Direct2D.
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	m_pd3dDevice.As(&dxgiDevice);

	// Create swap chain.
	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;

	hr = dxgiDevice->GetAdapter(&adapter);

	if (SUCCEEDED(hr))
	{
		adapter->GetParent(IID_PPV_ARGS(&factory));

		hr = factory->CreateSwapChain(
			m_pd3dDevice.Get(),
			&desc,
			&m_pDXGISwapChain
			);
	}

	// Configure the back buffer, stencil buffer, and viewport.
	hr = ConfigureBackBuffer();

	return hr;
}


HRESULT DeviceResources::ConfigureBackBuffer()
{
	HRESULT hr = S_OK;

	hr = m_pDXGISwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer);
	hr = m_pd3dDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr, m_pRenderTarget.GetAddressOf());
	m_pBackBuffer->GetDesc(&m_bbDesc);

	// Create a depth-stencil view for use with 3D rendering if needed.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT> (m_bbDesc.Width),
		static_cast<UINT> (m_bbDesc.Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
		);

	m_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencil);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil.Get(), &depthStencilViewDesc, &m_pDepthStencilView);

	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
	m_viewport.Height = (float)m_bbDesc.Height;
	m_viewport.Width = (float)m_bbDesc.Width;
	m_viewport.MinDepth = 0;
	m_viewport.MaxDepth = 1;

	m_pd3dDeviceContext->RSSetViewports(1, &m_viewport);

	return hr;
}


HRESULT DeviceResources::ReleaseBackBuffer()
{
	HRESULT hr = S_OK;

	m_pRenderTarget.Reset();
	m_pBackBuffer.Reset();

	m_pDepthStencilView.Reset();
	m_pDepthStencil.Reset();

	// We need to call Flush() to ensure that Direct3D also releases any
	// references it might still have to the same resources such as pipeline bindings.
	m_pd3dDeviceContext->Flush();

	return hr;
}

HRESULT DeviceResources::GoFullScreen()
{
	HRESULT hr = S_OK;

	hr = m_pDXGISwapChain->SetFullscreenState(TRUE, NULL);

	// Before calling ResizeBuffers, you have to release all references to the back 
	// buffer device resource.
	ReleaseBackBuffer();

	// Now we can call ResizeBuffers.
	hr = m_pDXGISwapChain->ResizeBuffers(
		0,                   // Number of buffers. Set this to 0 to preserve the existing setting.
		0, 0,                // Width and height of the swap chain. Set to 0 to match the screen resolution.
		DXGI_FORMAT_UNKNOWN, // This tells DXGI to retain the current back buffer format.
		0
		);

	// Then we can recreate the back buffer, depth buffer, and so on.
	hr = ConfigureBackBuffer();

	return hr;
}

HRESULT DeviceResources::GoWindowed()
{
	HRESULT hr = S_OK;

	hr = m_pDXGISwapChain->SetFullscreenState(FALSE, NULL);
	ReleaseBackBuffer();

	hr = m_pDXGISwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	hr = ConfigureBackBuffer();
	return hr;
}


float DeviceResources::GetAspectRatio()
{
	return static_cast<float>(m_bbDesc.Width) / static_cast<float>(m_bbDesc.Height);
}


void DeviceResources::Present()
{
	m_pDXGISwapChain->Present(1, 0);
}