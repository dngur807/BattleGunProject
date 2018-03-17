/*!
* \file Layer.h
* \date 2015/04/03 16:06
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/
#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CLayer
{
private:
	explicit CLayer(MYGDI* pMyGDI);

public:
	~CLayer(void);

private:
	void Release(void);

public:
	//const CComponent* GetComponent(const wstring& wstrObjKey, const wstring& wstrComponentKey);
	HRESULT AddObject(const TCHAR* pObjectKey, CGameObject* pGameObject);

public:
	int Update(void);
	void Render(void);

public:
	static CLayer* Create(MYGDI* pMyGDI);


private:
	MYGDI*		m_pMyGDI;

private:
	typedef std::list<CGameObject*>				OBJECTLIST;
	typedef std::map<const TCHAR*, OBJECTLIST>	MAPOBJLIST;
	MAPOBJLIST		m_mapObjlist;
};

END

#endif // Layer_h__