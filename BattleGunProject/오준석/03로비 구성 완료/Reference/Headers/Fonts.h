#pragma once

#include "Engine_Include.h"
#include "FW1FontWrapper.h"

BEGIN(Engine)
class ENGINE_DLL CFonts
{
public:
	enum FontType
	{
		FONT_TYPE_BASIC , 
		FONT_TYPE_OUTLINE 
	};


private:
	IFW1FontWrapper*		m_pFW1FontWarpper;
	IFW1Factory*			m_pFW1FontFactory;
	IDWriteFactory*			m_pWriteFactory;

	ID3D11DeviceContext*	m_pDeivceContext;

public:
	FontType		m_eType;

	wstring			m_wstrText;
	float			m_fSize;
	UINT32			m_nColor;
	UINT			m_nFlag;

	XMFLOAT2		m_vPos;

	float			m_fOutlineSize;
	UINT32			m_nOutlineColor;
private:
	MYGDI*	m_pMyGDI;
	WORD*					m_pRefCnt;


public:
	static  CFonts*		Create(Engine::MYGDI* pMyGDI, const wstring& wstPath);
	virtual CFonts*		Clone();

public:
	HRESULT			Load_Font(const wstring& wstPath);

public:
	HRESULT			Init(const wstring& wstPath);
	virtual void	Update();
	void			Render();
	virtual void	Release();

private:
	explicit CFonts(Engine::MYGDI* pMyGDI);
	explicit CFonts(const CFonts& rhs);
public:
	 ~CFonts(void);

};

END