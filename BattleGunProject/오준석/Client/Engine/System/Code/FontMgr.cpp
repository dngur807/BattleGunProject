#include "FontMgr.h"
#include "Fonts.h"

IMPLEMENT_SINGLETON(Engine::CFontMgr)

Engine::CFontMgr::CFontMgr()
{
	m_mapFont.clear();
}


Engine::CFontMgr::~CFontMgr()
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}

HRESULT Engine::CFontMgr::AddFont(Engine::MYGDI* pMyGDI, const TCHAR* pFindKey, const wstring& wstrPath)
{
	MAPFONT::iterator iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFindKey));
	if (iter != m_mapFont.end())
		return E_FAIL;

	CFonts*	pFont = CFonts::Create(pMyGDI, wstrPath);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.insert(MAPFONT::value_type(pFindKey, pFont));
	return S_OK;
}

Engine::CFonts* Engine::CFontMgr::CloneFont(const TCHAR* FindKey)
{
	MAPFONT::iterator iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(FindKey));
	if (iter == m_mapFont.end())
		return NULL;
	return iter->second->Clone();
}
// 왠만하면 쓰지마라..
void Engine::CFontMgr::DrawFont(TCHAR* pFontKey , TCHAR* szText , float fX, float fY, float fSize, D3DCOLOR dColor)
{
	Engine::CFonts*		pComponent = NULL;
	Engine::CFonts* pFont = NULL;
	pComponent = CloneFont(pFontKey);
	pFont = pComponent;
	
	pFont->m_vPos.x = fX;
	pFont->m_vPos.y = fY;
	pFont->m_eType = Engine::CFonts::FONT_TYPE_OUTLINE;
	pFont->m_nFlag = FW1_VCENTER | FW1_RESTORESTATE;
	pFont->m_fSize = fSize;
	pFont->m_nColor = dColor;
	pFont->m_fOutlineSize = 0.1f;
	pFont->m_nOutlineColor = 0xFF000000;

	pFont->m_wstrText = szText;
	pFont->Render();
	pFont->Release();

}
