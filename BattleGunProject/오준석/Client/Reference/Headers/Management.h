
#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Renderer.h"

BEGIN(Engine)

class CScene;
class CGameObject;
class CComponent;

class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)
private:
	CManagement(void);
	~CManagement(void);

public:
	HRESULT InitManagement(MYGDI* pMyGDI, const UINT& iMaxSceneCnt);
	void Update(void);
	void Render(const float& fTime);

	const Engine::CComponent* GetComponent(const WORD& LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey);
	list<Engine::CGameObject*>* GetObjectList(const WORD& LayerID, const TCHAR* pObjKey);


	inline CScene* GetScene()
	{
		return m_pScene;
	}
	inline MYGDI* GetGDI()
	{
		return m_pMyGDI;
	}
	inline void SetFX(ID3DX11Effect* pFX) { m_pFX = pFX; }
	inline ID3DX11Effect* GetFX() { return m_pFX; }

	HRESULT Add_Component(const UINT& iIndex, const TCHAR* pComponentTag, CComponent* pComponent);
	CComponent* Find_Component(const UINT& iIndex, const TCHAR* pComponentTag);
	CComponent* Clone_Component(const UINT& iIndex, const TCHAR* pComponentTag);
	void Release_Component(const UINT& iIndex);
	HRESULT Reserve_ComponentMgr(const UINT& iSize);
private:
	map<const TCHAR*, CComponent*>*		m_pMapComponent; // 동적할당할려고.
	typedef map<const TCHAR*, CComponent*>	MAPCOMPONENT;

public:
	template <typename T>
	HRESULT SceneChange(T& Functor);
	HRESULT SceneSwap(CScene* pPrevScene, CScene* pNextScene);

	void AddRenderGroup(RENDERGROUP eRenderGroup, Engine::CGameObject* pGameObject);
	void DeleteRenderObject(Engine::CGameObject* pObject);
	void DeleteRenderObject(RENDERGROUP eRenderGroup , Engine::CGameObject* pObject);
public:
	void Release(void);

private:
	CScene*		m_pScene;
	CRenderer*	m_pRenderer;

private:
	MYGDI*		m_pMyGDI;
	ID3DX11Effect*	m_pFX;
	UINT									m_iContainerSize;
};

template <typename T>
HRESULT Engine::CManagement::SceneChange(T& Functor)
{
	if (m_pScene != NULL)
		Engine::Safe_Delete(m_pScene);

	FAILED_CHECK_RETURN(Functor(&m_pScene, m_pMyGDI), E_FAIL);

	m_pRenderer->SetCurrentScene(m_pScene);
	return S_OK;
}

END

#endif // Management_h__