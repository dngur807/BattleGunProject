#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev
{
public:
	DECLARE_SINGLETON(CGraphicDev)

public:
	enum WINMODE { MODE_FULL, MODE_WIN };

private:
	CGraphicDev();
	~CGraphicDev();

public:
	ID3D11Device* GetDevice(void) { return m_pDevice; }
	ID3D11DeviceContext* GetContext(void) { return m_pImmediateContext; }


	//Ãß°¡
	HRESULT Init_Blend();
	void Belend_Begin();
	void Blend_End();
	void ZBufferOn();
	void ZBufferOff();
	//
public:
	HRESULT InitGraphicDev(WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);
	void OnResize(HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);

public:
	void RenderBegin(void);
	void RenderEnd(void);

private:
	bool m_bResizing;

	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pImmediateContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11Texture2D*			m_pDepthStencilBuffer;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11DepthStencilView*		m_pDepthStencilView;
	D3D11_VIEWPORT				m_tScreenViewPort;

	// Derived class should set these in derived constructor to customize starting values.
	D3D_DRIVER_TYPE		m_eDriverType;
	UINT				m_i4xMASSQual;
	bool				m_bEnable4xMSAA;

private:
	ID3D11BlendState*			m_pBlendState;
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilState*	m_pDepthDisabledStencilState;
};

END