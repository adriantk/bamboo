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

#include <string>
#include <memory>
#include <ppltasks.h>

//-----------------------------------------------------------------------------
// CLASS DEFINITIONS
//-----------------------------------------------------------------------------
class Renderer
{
public:
	Renderer(std::shared_ptr<DeviceResources> deviceResources);
	~Renderer();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void Update();
	void Render();

private:
	HRESULT CreateShaders();
	HRESULT CreateCube();
	void    CreateViewAndPerspective();

	//-----------------------------------------------------------------------------
	// Pointer to device resource manager
	//-----------------------------------------------------------------------------
	std::shared_ptr<DeviceResources> m_deviceResources;

	//-----------------------------------------------------------------------------
	// Variables for rendering the cube
	//-----------------------------------------------------------------------------
	typedef struct _constantBufferStruct {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	} ConstantBufferStruct;

	// Assert that the constant buffer remains 16-byte aligned.
	static_assert((sizeof(ConstantBufferStruct) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

	//-----------------------------------------------------------------------------
	// Vertex data
	//-----------------------------------------------------------------------------
	typedef struct _vertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	} VertexPositionColor;


	ConstantBufferStruct m_constantBufferData;
	unsigned int  m_indexCount;
	unsigned int  m_frameCount;

	//-----------------------------------------------------------------------------
	// Direct3D device resources
	//-----------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_pInputLayoutExtended;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_pConstantBuffer;
};
