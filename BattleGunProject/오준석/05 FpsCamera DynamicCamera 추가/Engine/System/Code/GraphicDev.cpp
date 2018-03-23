#include "GraphicDev.h"
#include <d3d11.h>

IMPLEMENT_SINGLETON(Engine::CGraphicDev)

Engine::CGraphicDev::CGraphicDev()
	:m_pDevice(nullptr)
	, m_pImmediateContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pDepthStencilBuffer(nullptr)
	, m_pRenderTargetView(nullptr)
	, m_pDepthStencilView(nullptr)
	, m_eDriverType(D3D_DRIVER_TYPE_HARDWARE)
	, m_i4xMASSQual(0)
	, m_bEnable4xMSAA(false)
	, m_bResizing(false)
	, m_pDepthStencilState(NULL)
	, m_pBlendState(NULL)
{
}


Engine::CGraphicDev::~CGraphicDev()
{
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pDepthStencilBuffer);
	Safe_Release(m_pRenderTargetView);
	Safe_Release(m_pSwapChain);
	Safe_Release(m_pBlendState);

	Safe_Release(m_pImmediateContext);
	Safe_Release(m_pDevice);

}

HRESULT Engine::CGraphicDev::InitGraphicDev(WINMODE Mode, HWND hWnd, const WORD & wSizeX, const WORD & wSizeY)
{
	//Create the device and device Context

	UINT iCreateDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	iCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL eFeatureLevel = D3D_FEATURE_LEVEL_10_0;

	HRESULT hr = D3D11CreateDevice(
		0,					// default adapter
		m_eDriverType,
		0,					// no software device
		iCreateDeviceFlags,
		0, 0,				// default feature level array
		D3D11_SDK_VERSION,
		&m_pDevice,
		&eFeatureLevel,
		&m_pImmediateContext);

	//FAILED_CHECK_MSG(hr, L"D3D11CreateDevice Failed");

	if (eFeatureLevel != D3D_FEATURE_LEVEL_11_0 && eFeatureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		MessageBox(0, L"Direct3D Feature Level 11 Unsupported", 0, 0);
		return E_FAIL;
	}

	// Check 4X MASS quality for our back buffer format
	// All Direct3D capable devices support 4X MSAA for all render
	// target formats, so we only need to check quality support.

	HR(m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_i4xMASSQual));
	assert(m_i4xMASSQual > 0);

	// Fill out a DXGI_CHAIN_DESC to describe our swap chain

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = wSizeX;
	sd.BufferDesc.Height = wSizeY;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA?
	if (m_bEnable4xMSAA)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_i4xMASSQual - 1;
	}
	//No MSAA
	else
	{
		sd.SampleDesc.Count		= 1;
		sd.SampleDesc.Quality	 = 0;
	}

	sd.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount		= 1;
	sd.OutputWindow		= hWnd;
	sd.Windowed			= Mode;
	sd.SwapEffect		= DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags			= 0;

	// To correctly create the swap chain, we muse use the IDXGIFactory that was
	// used to create the device. If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error : "IDXGIFactory::CreateSwapChain:
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* pDxgiDevice = nullptr;
	HR(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDxgiDevice));

	IDXGIAdapter* pDxgiAdapter = nullptr;
	HR(pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDxgiAdapter));

	IDXGIFactory* pDxgiFactory = nullptr;
	HR(pDxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDxgiFactory));

	HR(pDxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain));

	ReleaseCOM(pDxgiDevice);
	ReleaseCOM(pDxgiAdapter);
	ReleaseCOM(pDxgiFactory);

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized. So
	// just call the OnResize method here to avoid code duplication
	OnResize(hWnd, wSizeX, wSizeY);
	return S_OK;
}

void Engine::CGraphicDev::OnResize(HWND hWnd, const WORD& wSizeX, const WORD& wSizeY)
{
	assert(m_pImmediateContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	// Release the old views, as they hold references to the buffers we
	// will be destroying. Also release the old depth/stencil buffer.

	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pDepthStencilBuffer);

	// Resize the swap chain and recreate the render target view.

	HR(m_pSwapChain->ResizeBuffers(1, wSizeX, wSizeY, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* pBackBuffer = nullptr;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
	HR(m_pDevice->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView));
	ReleaseCOM(pBackBuffer);

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = wSizeX;
	depthStencilDesc.Height = wSizeY;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? -- must match swap chain MSAA values.
	if (m_bEnable4xMSAA)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_i4xMASSQual - 1;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer));
	HR(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));

	// Bind the render target view and depth/stencil view to the pipeline.

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


	// Depth-Stencil 상태디스크
	D3D11_DEPTH_STENCIL_DESC _depthStencilDesc2;
	ZeroMemory(&_depthStencilDesc2, sizeof(_depthStencilDesc2));

	_depthStencilDesc2.DepthEnable = true;
	_depthStencilDesc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	_depthStencilDesc2.DepthFunc = D3D11_COMPARISON_LESS;
	_depthStencilDesc2.StencilEnable = true;
	_depthStencilDesc2.StencilReadMask = 0xFF;
	_depthStencilDesc2.StencilWriteMask = 0xFF;
	_depthStencilDesc2.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilDesc2.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	_depthStencilDesc2.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilDesc2.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	_depthStencilDesc2.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilDesc2.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	_depthStencilDesc2.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilDesc2.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Depth-Stencil 상태 생성
	m_pDevice->CreateDepthStencilState(&_depthStencilDesc2, &m_pDepthStencilState);
	// Depth-Stencil 상태 세팅
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	// Depth-Stencil Disable 상태디스크
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 상태 생성
	m_pDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pDepthDisabledStencilState);



	// Set the Viewport Transform.
	m_tScreenViewPort.TopLeftX = 0;
	m_tScreenViewPort.TopLeftY = 0;
	m_tScreenViewPort.Width = static_cast<float>(wSizeX);
	m_tScreenViewPort.Height = static_cast<float>(wSizeY);
	m_tScreenViewPort.MinDepth = 0.0f;
	m_tScreenViewPort.MaxDepth = 1.0f;

	m_pImmediateContext->RSSetViewports(1, &m_tScreenViewPort);
	FAILED(Init_Blend());
}

void Engine::CGraphicDev::RenderBegin(void)
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView,
		reinterpret_cast<const float*>(&DirectX::Colors::Blue));

	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Engine::CGraphicDev::RenderEnd(void)
{
	HR(m_pSwapChain->Present(0, 0));
}

void Engine::CGraphicDev::Belend_Begin()
{
	m_pImmediateContext->OMSetBlendState(m_pBlendState, NULL, 0xffffffff);
}
void Engine::CGraphicDev::Blend_End()
{
	m_pImmediateContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

HRESULT Engine::CGraphicDev::Init_Blend()
{
	D3D11_BLEND_DESC tBlend;
	ZeroMemory(&tBlend, sizeof(D3D11_BLEND_DESC));

	tBlend.AlphaToCoverageEnable = TRUE;
	tBlend.IndependentBlendEnable = FALSE;
	tBlend.RenderTarget[0].BlendEnable = TRUE;
	tBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	tBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	tBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	FAILED_CHECK_RETURN(m_pDevice->CreateBlendState(&tBlend, &m_pBlendState), E_FAIL);

	return S_OK;
}

void Engine::CGraphicDev::ZBufferOn()
{
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

}

void Engine::CGraphicDev::ZBufferOff()
{
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}
