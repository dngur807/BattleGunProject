#include "stdafx.h"
#include "Text.h"
#include "TextMgr.h"
#include "Fonts.h"
#include "Export_Function.h"
#include "FontMgr.h"

CText::CText(Engine::MYGDI* pMyGDI, TCHAR* pFontKey, int iMax)
	: Engine::CGameObject(pMyGDI)
	, m_pFont ( NULL )
{
	m_iMax = iMax;
	lstrcpy(m_pFontKey, pFontKey);
	ZeroMemory(m_szText, sizeof(TCHAR) * MAX_STR);
	
}

CText::~CText(void)
{
	Release();
}

HRESULT CText::Initialize(TCHAR* pFontKey)
{
	FAILED_CHECK(AddComponent(pFontKey));
	return S_OK;
}
int CText::Update(void)
{
	return 0;
}

void CText::Render(void)
{
	TCHAR szText[MAX_STR];
	lstrcpy(szText, m_szText);

	if (CTextMgr::GetInstance()->GetText() == this)
	{
		int len = lstrlen(szText);
		szText[len] = '_';
		szText[len + 1] = 0;
	}
	m_pFont->m_wstrText = szText;
	m_pFont->Render();
}

void CText::Release(void)
{
	Engine::Safe_Delete(m_pFont);
}

CText* CText::Create(Engine::MYGDI* pMyGDI, TCHAR* pFontKey, int iMax)
{
	CText* pText = new CText(pMyGDI, pFontKey, iMax);
	if (FAILED(pText->Initialize(pFontKey)))
		Engine::Safe_Delete(pText);

	return pText;
}

HRESULT CText::AddComponent(TCHAR* pFontKey)
{
	Engine::CFonts*		pComponent = NULL;
	pComponent = Engine::Get_FontMgr()->CloneFont(
		pFontKey);
	m_pFont = pComponent;


	m_pFont->m_eType = Engine::CFonts::FONT_TYPE_OUTLINE;
	m_pFont->m_nFlag =  FW1_VCENTER | FW1_RESTORESTATE;
	m_pFont->m_fSize = 30;
	m_pFont->m_nColor = 0xFFFFFFFF;
	m_pFont->m_fOutlineSize = 1.f;
	m_pFont->m_nOutlineColor = 0xFF000000;

	return S_OK;
}

void CText::AddText(TCHAR ch)
{
	int len = lstrlen(m_szText);

	if (len > m_iMax)
		return;

	m_szText[len] = ch;
	m_szText[len + 1] = NULL;
}

void CText::RemoveText(void)
{
	int len = lstrlen(m_szText);
	if (len > 0)
		m_szText[len - 1] = NULL;
}

void CText::ClearText(void)
{
	ZeroMemory(m_szText, sizeof(TCHAR) * MAX_STR);
}

void CText::SetPos(float fX, float fY)
{
	m_pFont->m_vPos.x = fX;
	m_pFont->m_vPos.y = fY;
}

void CText::SetColor(D3DCOLOR dColor)
{
	m_pFont->m_nColor = dColor;
}

TCHAR* CText::GetText(void)
{
	return m_szText;
}

void CText::SetSize(float size)
{
	m_pFont->m_fSize = size;
}

void CText::SetText(TCHAR* szText)
{
	ClearText();
	lstrcpy(m_szText, szText);
}

