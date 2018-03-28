
#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CLayer;
class CComponent;

class ENGINE_DLL CScene
{
protected:
	explicit CScene(MYGDI* pMyGDI);

public:
	virtual ~CScene(void);

public:
	virtual HRESULT InitScene(void) PURE;
	virtual int Update(void);
	virtual void Render(void);
	map<WORD, Engine::CLayer*>* GetMapLayer(void);
	const Engine::CComponent* GetComponent(const WORD& LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey);

private:
	void Release(void);

protected:
	MYGDI*		m_pMyGDI;

protected:
	typedef std::map<WORD, CLayer*>		MAPLAYER;
	MAPLAYER		m_mapLayer;
};

END

#endif // Scene_h__