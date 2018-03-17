/*!
* \file Scene.h
* \date 2015/04/02 23:10
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
	//const CComponent* GetComponent(WORD LayerID, const wstring& wstrObject, const wstring& wstrComponentKey);


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