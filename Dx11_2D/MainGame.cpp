#include "stdafx.h"
#include "MainGame.h"

MainGame::MainGame()
	: m_pDirLightBuffer(NULL)
{
	srand((UINT)GetTickCount64());

	whlCount = 0;

	// Create VPBuffer
	{
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(ViewProjMatrix);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &vpBuffer);
		assert(SUCCEEDED(hr));
	}

	// Matrix Setting
	{
		vpMatrix = new ViewProjMatrix;

		// View
		D3DXVECTOR3 eye(0, 0, -5);
		D3DXVECTOR3 lookAt(0, 0, 0);
		D3DXVECTOR3 up(0, 1, 0);
		D3DXMatrixLookAtLH(&vpMatrix->View, &eye, &lookAt, &up);
		D3DXMatrixTranspose(&vpMatrix->View, &vpMatrix->View);

		// Projection
		//D3DXMatrixPerspectiveFovLH(&vpMatrix->Projection, RAD(60), WINSIZEX / (float)WINSIZEY, 1, 5);
		//D3DXMatrixTranspose(&vpMatrix->Projection, &vpMatrix->Projection);

		// Projection
		D3DXMatrixOrthoOffCenterLH(&vpMatrix->Projection, 0, (float)WINSIZEX, 0, (float)WINSIZEY, -1, 1);
		D3DXMatrixTranspose(&vpMatrix->Projection, &vpMatrix->Projection);
	}

	// 블렌드 옵션 생성
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Device->CreateBlendState(&desc, &NormalBlendState);

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Device->CreateBlendState(&desc, &AlphaBlendState);
	}

	
}

MainGame::~MainGame()
{

}

void MainGame::Init()
{
	ImGui::Create(g_hWnd, Device, DeviceContext);	// ImGui 생성
	ImGui::StyleColorsDark();						// 스타일 선택

	// 매니져 초기화
	g_pKeyManger->Init();
	g_pTimeManager->Init();
	D2DX->init();
	TEXT->DWInit();

	SCENEMANAGER->Create_Scene("MAIN", new MainScene);
	SCENEMANAGER->Init_Scene();

	SCENEMANAGER->Change_Scene("MAIN");
}

void MainGame::Update()
{
	const float Speed = 550.0f;

	g_pTimeManager->Update(CONSTANT_FPS);

	{
		if (g_pKeyManger->isStayKeyDown(VK_LEFT))
			CameraPositionX -= g_pTimeManager->GetDeltaTime() * Speed;

		else if (g_pKeyManger->isStayKeyDown(VK_RIGHT))
			CameraPositionX += g_pTimeManager->GetDeltaTime() * Speed;

		else if (g_pKeyManger->isStayKeyDown(VK_DOWN))
			CameraPositionY -= g_pTimeManager->GetDeltaTime() * Speed;

		else if (g_pKeyManger->isStayKeyDown(VK_UP))
			CameraPositionY += g_pTimeManager->GetDeltaTime() * Speed;
	}

	ImGui::Update();

	SCENEMANAGER->Update_Scene();

	D3DXVECTOR3 eye(CameraPositionX, CameraPositionY, -5);
	D3DXVECTOR3 lookAt(CameraPositionX, CameraPositionY, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&vpMatrix->View, &eye, &lookAt, &up);
	D3DXMatrixTranspose(&vpMatrix->View, &vpMatrix->View);

	if (m_pDirLightBuffer)
	{
		m_fLightRotZ = 0.5f * g_pTimeManager->GetDeltaTime();

		D3DXMATRIX R;
		D3DXMatrixRotationZ(&R, m_fLightRotZ);

		D3DXVECTOR3 v = (D3DXVECTOR3)m_stDirLight.LightDirection;
		D3DXVec3TransformNormal(&v, &v, &R);
		m_stDirLight.LightDirection = D3DXVECTOR4(v, 0);

		DeviceContext->UpdateSubresource(m_pDirLightBuffer, 0, 0, &m_stDirLight, 0, 0);
		//DeviceContext->UpdateSubresource(m_pCameraPosBuffer, 0, 0, &eye, 0, 0);
	}

}

void MainGame::Render()
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	HRESULT hr = SwapChain->GetDesc(&swapDesc);

	//g_pTimeManager->Render();

	//SHADER->setShader();

	if (SUCCEEDED(hr))
		if (d2Rtg)
		{
			D3DXCOLOR background = D3DXCOLOR(0, 0, 0, 1);

			DeviceContext->ClearRenderTargetView(RTV, (float *)background);

			// Set VPMatrix Buffer
			DeviceContext->UpdateSubresource(vpBuffer, 0, NULL, vpMatrix, 0, 0);
			DeviceContext->VSSetConstantBuffers(0, 1, &vpBuffer);

			DeviceContext->OMSetBlendState(AlphaBlendState, NULL, 0xFF);
			{
				SCENEMANAGER->Render_Scene();
			}
			DeviceContext->OMSetBlendState(NormalBlendState, NULL, 0xFF);

		}

	ImGui::Render();
	SwapChain->Present(0, 0);
}

void MainGame::Release()
{
	ImGui::Delete(); // ImGui 해제

	SAFE_DEL(vpMatrix);

	SCENEMANAGER->release();
	SCENEMANAGER->delInstance();

	//	Release Singleton Memory
	D2DX->delInstance();
	TEXT->release();
	TEXT->delInstance();
	SHADER->delInstance();	

	g_pKeyManger->ReleaseInstance();
	g_pTimeManager->ReleaseInstance();
}

void MainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}