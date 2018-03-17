#ifndef FontMgr_h__
#define FontMgr_h__



#include "Engine_Include.h"

BEGIN(Engine)

class CFonts;

class ENGINE_DLL CFontMgr
{
public:
	DECLARE_SINGLETON(CFontMgr)
private:
	CFontMgr();
	~CFontMgr();
public:
	HRESULT AddFont(Engine::MYGDI* pMyGDI 
	 , const TCHAR* pFindKey, const wstring& wstrPath);
	CFonts* CloneFont(const TCHAR* FindKey);
private:
	typedef map<const TCHAR*, CFonts*>		MAPFONT;
	MAPFONT		m_mapFont;
};
END


#endif // FontMgr_h__
