/*!
* \file GameObject.h
* \date 2015/04/03 18:11
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
	const CComponent* GetComponent(const wstring& wstrComponentKey);
protected:
	MYGDI*		m_pMyGDI;

protected:
	typedef std::map<wstring, CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT		m_mapComponent;
};

END

#endif // GameObject_h__