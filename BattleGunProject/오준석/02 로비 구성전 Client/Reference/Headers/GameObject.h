
#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"

BEGIN(Engine)
class CComponent;
class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(MYGDI* pMyGDI);

public:
	virtual ~CGameObject(void);

private:
	void Release(void);

public:
	virtual int Update(void);
	virtual void Render(void) {}

public:
	const CComponent* GetComponent(const TCHAR* wstrComponentKey);
protected:
	MYGDI*		m_pMyGDI;

protected:
	typedef std::map<const TCHAR*, CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT		m_mapComponent;
};

END

#endif // GameObject_h__