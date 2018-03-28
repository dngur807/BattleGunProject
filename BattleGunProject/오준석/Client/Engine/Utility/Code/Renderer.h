
#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"

BEGIN(Engine)
class CScene;
class CGameObject;
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
	void AddRenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject);
	void DeleteRenderObject(CGameObject* pGameObject);
	void DeleteRenderObject(RENDERGROUP eRenderGroup ,CGameObject* pGameObject);
	void ClearRenderGroup();

public:
	static CRenderer* Create(MYGDI* pMyGDI);

private:
	void Render_Priority(void);
	void Render_NoneAlpha();
	void Render_Alpha(void);
	void Render_Effect();
	void Render_UI();

private:
	CScene*		m_pScene;

private:
	MYGDI*		m_pMyGDI;
	float		m_fTime;
	DWORD		m_dwCount;

private:
	typedef list<CGameObject*>		RENDERLIST;
	RENDERLIST			m_RenderGroup[TYPE_END];


};
END

bool Compare_ViewZ(Engine::CGameObject * pSour, Engine::CGameObject * pDest);
#endif // Renderer_h__