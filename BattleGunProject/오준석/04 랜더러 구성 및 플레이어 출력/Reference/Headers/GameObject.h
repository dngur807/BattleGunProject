
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
	const float& Get_ViewZ()const { return m_fViewZ; }
public:
	const Engine::CComponent* GetComponent(const TCHAR* wstrComponentKey);
	//const CComponent* GetComponent(const TCHAR* wstrComponentKey);
protected:
	MYGDI*		m_pMyGDI;
protected:
	void Compute_ViewZ(const XMVECTOR* pPos);
protected:
	typedef std::map<const TCHAR*, Engine::CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT		m_mapComponent;
	float				m_fViewZ;

};

END

#endif // GameObject_h__