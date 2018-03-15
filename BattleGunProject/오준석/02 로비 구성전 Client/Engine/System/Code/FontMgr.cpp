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
