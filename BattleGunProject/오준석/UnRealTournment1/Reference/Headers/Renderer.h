/*!
* \file Renderer.h
* \date 2015/04/02 23:05
*
* \author Administrator
* Contact: user@company.com
*
* \brief 씬의 렌더링을 수행하는 클래스
*
* TODO: long description
*
* \note
*/
#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CScene;

class ENGINE_DLL CRenderer
{
private:
	explicit CRenderer(MYGDI* pMyGDI);

public:
	~CRenderer(void);

public:
	void SetCurrentScene(CScene* pScene);

public:
	HRESULT InitScene(void);
	void Render(const float& fTime);

public:
	static CRenderer* Create(MYGDI* pMyGDI);

private:
	CScene*		m_pScene;

private:
	MYGDI*		m_pMyGDI;
	float		m_fTime;
	DWORD		m_dwCount;
};

END

#endif // Renderer_h__