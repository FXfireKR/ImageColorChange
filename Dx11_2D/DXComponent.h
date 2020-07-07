#pragma once
// DXComponent
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dX10.h>
#include <d3dx10math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace D2D1;

#include "Shader.h"

// Dx11 전역 변수
extern IDXGISwapChain* SwapChain;
extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern ID3D11RenderTargetView* RTV;

extern ID3D11VertexShader* VertexShader;
extern ID3D11PixelShader* PixelShader;
extern ID3D10Blob* VsBlob;
extern ID3D10Blob* PsBlob;

extern ID2D1RenderTarget* d2Rtg;
extern ID2D1Factory* d2dFactory;
extern IDWriteFactory* dwFactory;
extern IDXGISurface* pBackBuffer;

typedef D3DXVECTOR2		Vector2;
typedef D3DXVECTOR3		Vector3;
typedef D3DXVECTOR4		Vector4;

struct ViewProjMatrix
{
	D3DXMATRIX View;
	D3DXMATRIX Projection;
};

struct DirectionalLight
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXVECTOR4	SpecPower;
	D3DXVECTOR4 LightDirection;
};

// == FVF (Flexible Vertex Format) : 유연한 정점 속성 =====================
// 포지션, 칼라 버텍스 포맷 설정 및 셰이더 생성
/* --------------------------------------------------------------------------- */
const struct PCVertex	// 포지션 정보, 칼라 정보를 갖는 버텍스 구조체
{
	D3DXVECTOR3	Position;
	D3DXCOLOR	Color;

	PCVertex() {}
	PCVertex(D3DXVECTOR3 p, D3DXCOLOR c) : Position(p), Color(c) {}
};

#define PCElementCount 2
const D3D11_INPUT_ELEMENT_DESC PCElementDesc[PCElementCount] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const wstring PCShaderFile = L"../_Shader/Color.hlsl";
/* --------------------------------------------------------------------------- */
const struct PTVertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR2 TexCoord;

	PTVertex() {}
	PTVertex(D3DXVECTOR3 p, D3DXVECTOR2 t) : Position(p), TexCoord(t) {}
};

#define PTElementCount 2
const D3D11_INPUT_ELEMENT_DESC PTElementDesc[PTElementCount] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const wstring PTShaderFile = L"../_Shader/Texture.hlsl";
const wstring PT_NoiseShaderFile = L"../_Shader/NoiseTexture.hlsl";
/* --------------------------------------------------------------------------- */
const struct PNTVertex
{
	D3DXVECTOR3	Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2	Texture;

	PNTVertex() {}
	PNTVertex(D3DXVECTOR3 p, D3DXVECTOR3 n, D3DXVECTOR2 t) : Position(p), Normal(n), Texture(t) {}
};

#define PNTElementCount 3
const D3D11_INPUT_ELEMENT_DESC PNTElementDesc[PNTElementCount] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const wstring PNTShaderFile = L"../_Shader/Light2.hlsl";
/* --------------------------------------------------------------------------- */
// === 버퍼 생성 함수들 ===================================================
// 콘스탄트 버퍼 생성
inline void CreateConstantBuffer(ID3D11Buffer** ppBuffer, UINT ByteWidth, const void *pSystem)
{
	D3D11_BUFFER_DESC desc = { 0, };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = ByteWidth;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0, };
	data.pSysMem = pSystem;

	HRESULT hr = Device->CreateBuffer(&desc, &data, ppBuffer);
	assert(SUCCEEDED(hr));
}
// 버텍스 버퍼
inline void CreateVertexBuffer(ID3D11Buffer** ppBuffer, UINT ByteWidth, const void *pSystem)
{
	D3D11_BUFFER_DESC desc = { 0, };
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = ByteWidth;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0, };
	data.pSysMem = pSystem;

	HRESULT hr = Device->CreateBuffer(&desc, &data, ppBuffer);
	assert(SUCCEEDED(hr));
}

// 버텍스 버퍼
inline void CreateVertexBuffer(ID3D11Buffer** ppBuffer, UINT ByteWidth, const void *pSystem, D3D11_USAGE usage)
{
	D3D11_BUFFER_DESC desc = { 0, };
	desc.Usage = usage;
	desc.ByteWidth = ByteWidth;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0, };
	data.pSysMem = pSystem;

	HRESULT hr = Device->CreateBuffer(&desc, &data, ppBuffer);
	assert(SUCCEEDED(hr));
}

// 인덱스 버퍼
inline void CreateIndexBuffer(ID3D11Buffer** ppBuffer, UINT ByteWidth, const void *pSystem)
{
	D3D11_BUFFER_DESC desc = { 0, };
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = ByteWidth;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0, };
	data.pSysMem = pSystem;

	HRESULT hr = Device->CreateBuffer(&desc, &data, ppBuffer);
	assert(SUCCEEDED(hr));
}