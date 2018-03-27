
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
	HRESULT AddObject(const TCHAR* pObjectKey, CGameObject* pGameObject);
	list<Engine::CGameObject*>*	GetObjectList(const TCHAR* pObjKey);
	const Engine::CComponent* GetComponent(const TCHAR* pObjKey, const TCHAR* pComponentKey);

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