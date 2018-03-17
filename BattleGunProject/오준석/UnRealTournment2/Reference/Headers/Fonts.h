#pragma once


#ifndef Font_h__
#define Font_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CFont
{
private:
	CFont(MYGDI* pMyGDI);  //(LPDIRECT3DDEVICE9 pDevice);

public:
	~CFont(void);

public:
	HRESULT InitFont(const UINT& iHeight, const UINT& iWidth, const UINT& iWeight
		, const TCHAR* pFaceName);
	void Render(const TCHAR* pString, XMFLOAT3 Color, const XMFLOAT3& vPos);

public:
	static CFont* Create(MYGDI* pMyGDI
		, const UINT& iHeight, const UINT& iWidth, const UINT& iWeight
		, const TCHAR* pFaceName);

private:
	void Release(void);

private:
	MYGDI*		m_pMyGDI;
	//LPDIRECT3DDEVICE9		m_pDevice;
	//LPD3DXFONT				m_pFont;
	UINT					m_iHeight;
	UINT					m_iWidth;
};

END

#endif // Font_h__