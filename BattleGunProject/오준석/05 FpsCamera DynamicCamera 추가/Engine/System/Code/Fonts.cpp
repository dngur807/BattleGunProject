#include "Fonts.h"
#include <D2D1.h>
#include <DWrite.h>
#include "Engine_Macro.h"

#pragma  comment(lib, "FW1FontWrapper.lib")
Engine::CFonts::CFonts(Engine::MYGDI* pMyGDI)
	: m_pMyGDI (pMyGDI)
	, m_pRefCnt(new WORD(0))
	, m_pFW1FontWarpper(NULL)
	, m_pFW1FontFactory(NULL)
	, m_pWriteFactory(NULL)
	, m_pDeivceContext(NULL)
	, m_eType(FONT_TYPE_BASIC)

	, m_wstrText(L"")
	, m_fSize(0.f)
	, m_nColor(0xFFFFFFFF)
	, m_nFlag(FW1_LEFT | FW1_TOP | FW1_RESTORESTATE)

	, m_vPos(0.f, 0.f)

	, m_fOutlineSize(1.f)
	, m_nOutlineColor(0xFF000000)


{

}

Engine::CFonts::CFonts(const CFonts& rhs)
	: m_pMyGDI( rhs.m_pMyGDI)
{
	m_pFW1FontWarpper = rhs.m_pFW1FontWarpper;
	m_pFW1FontFactory = rhs.m_pFW1FontFactory;
	m_pWriteFactory = rhs.m_pWriteFactory;

	m_pDeivceContext = rhs.m_pDeivceContext;

	m_eType = rhs.m_eType;

	m_wstrText = rhs.m_wstrText;
	m_fSize = rhs.m_fSize;
	m_nColor = rhs.m_nColor;
	m_nFlag = rhs.m_nFlag;

	m_vPos = rhs.m_vPos;

	m_fOutlineSize = rhs.m_fOutlineSize;
	m_nOutlineColor = rhs.m_nOutlineColor;

	m_pRefCnt = rhs.m_pRefCnt;
}

Engine::CFonts::~CFonts()
{
	Release();
}
Engine::CFonts* Engine::CFonts::Create(Engine::MYGDI* pMyGDI, const wstring& wstPath)
{
	CFonts* pFont = new CFonts(pMyGDI);

	if (FAILED(pFont->Init(wstPath)))
		Safe_Delete(pFont);

	return pFont;
}

Engine::CFonts* Engine::CFonts::Clone()
{
	++(*m_pRefCnt);
	return new CFonts(*this);
}

HRESULT Engine::CFonts::Load_Font(const wstring& wstPath)
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1FontFactory)))
	{
		MSG_BOX(L"FAILED CreateFactory");
		return E_FAIL;
	}

	if (FAILED(m_pFW1FontFactory->CreateFontWrapper(
		m_pMyGDI->pDevice, wstPath.c_str(), &m_pFW1FontWarpper)))
	{
		MSG_BOX(L"Not Find Font");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Engine::CFonts::Init(const wstring& wstPath)
{
	m_pDeivceContext = m_pMyGDI->pContext;
	Load_Font(wstPath);

	return S_OK;
}

void Engine::CFonts::Update()
{

}

void Engine::CFonts::Render()
{
	if (!m_pFW1FontWarpper)
		return;

	if (FONT_TYPE_OUTLINE == m_eType)
	{
		m_pFW1FontWarpper->DrawString(
			m_pDeivceContext,
			m_wstrText.c_str(), m_fSize,
			m_vPos.x - m_fOutlineSize, m_vPos.y - m_fOutlineSize,
			m_nOutlineColor, m_nFlag);
		m_pFW1FontWarpper->DrawString(
			m_pDeivceContext,
			m_wstrText.c_str(), m_fSize,
			m_vPos.x + m_fOutlineSize, m_vPos.y + m_fOutlineSize,
			m_nOutlineColor, m_nFlag);
		m_pFW1FontWarpper->DrawString(
			m_pDeivceContext,
			m_wstrText.c_str(), m_fSize,
			m_vPos.x + m_fOutlineSize, m_vPos.y - m_fOutlineSize,
			m_nOutlineColor, m_nFlag);
		m_pFW1FontWarpper->DrawString(
			m_pDeivceContext,
			m_wstrText.c_str(), m_fSize,
			m_vPos.x - m_fOutlineSize, m_vPos.y + m_fOutlineSize,
			m_nOutlineColor, m_nFlag);

		m_pFW1FontWarpper->DrawString(
			m_pDeivceContext,
			m_wstrText.c_str(), m_fSize,
			m_vPos.x, m_vPos.y - m_fOutlineSize,
			m_nOutlineColor, m_nFlag);
		m_pFW1FontWarpper->DrawString(
			m_pDeivceContext,
			m_wstrText.c_str(), m_fSize,
			m_vPos.x, m_vPos.y + m_fOutlineSize,
			m_nOutlineColor, m_nFlag);
		m_pFW1FontWarpper->DrawString(
			m_pDeivceContext,
			m_wstrText.c_str(), m_fSize,
			m_vPos.x + m_fOutlineSize, m_vPos.y,
			m_nOutlineColor, m_nFlag);
		m_pFW1FontWarpper->DrawString(
			m_pDeivceContext,
			m_wstrText.c_str(), m_fSize,
			m_vPos.x - m_fOutlineSize, m_vPos.y,
			m_nOutlineColor, m_nFlag);
	}
	m_pFW1FontWarpper->DrawString(
		m_pDeivceContext,
		m_wstrText.c_str(), m_fSize,
		m_vPos.x, m_vPos.y,
		m_nColor, m_nFlag);
}

void Engine::CFonts::Release()
{
	if ((*m_pRefCnt) == 0)
	{
		Safe_Release(m_pFW1FontWarpper);
		Safe_Release(m_pFW1FontFactory);
		Safe_Release(m_pWriteFactory);
		Safe_Delete(m_pRefCnt);
	}
	else
		--(*m_pRefCnt);
}


