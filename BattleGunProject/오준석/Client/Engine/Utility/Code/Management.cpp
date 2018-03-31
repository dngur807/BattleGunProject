#include "Management.h"
#include "Renderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "Layer.h"
#include "Component.h"
IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
	: m_pScene(nullptr)
	, m_pRenderer(nullptr)
	, m_pMyGDI(nullptr)
	, m_pFX(nullptr)
	, m_iContainerSize(0)
{
}

Engine::CManagement::~CManagement(void)
{
	Release();
}

HRESULT Engine::CManagement::InitManagement(MYGDI* pMyGDI, const UINT& iMaxSceneCnt)
{
	m_pMyGDI = pMyGDI;

	m_pRenderer = CRenderer::Create(pMyGDI);
	Reserve_ComponentMgr(iMaxSceneCnt);

	NULL_CHECK_RETURN_MSG(m_pRenderer, E_FAIL, L"Renderer Gen Failed");

	return S_OK;
}

void Engine::CManagement::Update(void)
{
	if (m_pScene != NULL)
		m_pScene->Update();
}

void Engine::CManagement::Render(const float& fTime)
{
	if (m_pRenderer != NULL)
		m_pRenderer->Render(fTime);
}

void Engine::CManagement::Release(void)
{
	Engine::Safe_Delete(m_pRenderer);
	Engine::Safe_Delete(m_pScene);
}

HRESULT Engine::CManagement::SceneSwap(CScene* pPrevScene, CScene* pNextScene)
{
	Engine::Safe_Delete(pPrevScene);
	m_pScene = pNextScene;

	m_pRenderer->SetCurrentScene(m_pScene);
	return S_OK;
}

void Engine::CManagement::AddRenderGroup(RENDERGROUP eRenderGroup, Engine::CGameObject* pGameObject)
{
	pGameObject->SetRenderGroup(eRenderGroup);
	m_pRenderer->AddRenderGroup(eRenderGroup, pGameObject);
}

void Engine::CManagement::DeleteRenderObject(Engine::CGameObject* pObject)
{
	m_pRenderer->DeleteRenderObject(pObject);
}

void Engine::CManagement::DeleteRenderObject(RENDERGROUP eRenderGroup, Engine::CGameObject* pObject)
{
	m_pRenderer->DeleteRenderObject(eRenderGroup, pObject);
}

const Engine::CComponent* Engine::CManagement::GetComponent(const WORD& LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	return m_pScene->GetComponent(LayerID, pObjKey, pComponentKey);
}

list<Engine::CGameObject*>* Engine::CManagement::GetObjectList(const WORD& LayerID, const TCHAR* pObjKey)
{
	map<WORD, Engine::CLayer*>::iterator iter = m_pScene->GetMapLayer()->find(LayerID);

	return iter->second->GetObjectList(pObjKey);
}

HRESULT Engine::CManagement::Add_Component(const UINT& iIndex, const TCHAR* pComponentTag, CComponent* pComponent)
{
	if (nullptr == m_pMapComponent)
	{
		MSGBOX("예약이 안됐는데?");
		return E_FAIL;
	}

	CComponent*	pComponent_Find = Find_Component(iIndex, pComponentTag);

	if (NULL != pComponent_Find)
		return E_FAIL;

	m_pMapComponent[iIndex].insert(MAPCOMPONENT::value_type(pComponentTag, pComponent));

	return S_OK;
}

Engine::CComponent* Engine::CManagement::Find_Component(const UINT& iIndex, const TCHAR* pComponentTag)
{
	MAPCOMPONENT::iterator	iter = find_if(m_pMapComponent[iIndex].begin(), m_pMapComponent[iIndex].end(), CTagFinder(pComponentTag));

	if (iter == m_pMapComponent[iIndex].end())
		return NULL;

	return iter->second;
}

Engine::CComponent* Engine::CManagement::Clone_Component(const UINT& iIndex, const TCHAR* pComponentTag)
{
	CComponent*	pComponent_Find = Find_Component(iIndex, pComponentTag);

	if (NULL == pComponent_Find)
		return NULL;

	return pComponent_Find;
}

HRESULT Engine::CManagement::Reserve_ComponentMgr(const UINT& iSize)
{
	//if (nullptr != m_pMapComponent)
	//{
	//	MSGBOX("예약되어 있는데?");
	//	return E_FAIL;
	//}

	m_pMapComponent = new MAPCOMPONENT[iSize];

	m_iContainerSize = iSize;

	return S_OK;
}

void Engine::CManagement::Release_Component(const UINT& iIndex)
{
//#ifdef _DEBUG
//	for_each(m_pMapComponent[iIndex].begin(), m_pMapComponent[iIndex].end(), CRelease_Pair());
//#else
//	MAPCOMPONENT::iterator iter = m_pMapComponent[iIndex].begin();
//	MAPCOMPONENT::iterator iter_end = m_pMapComponent[iIndex].end();
//
//	for (iter; iter != iter_end; ++iter)
//	{
//		Engine::Safe_Release(iter->second);
//	}
//#endif
	MAPCOMPONENT::iterator iter = m_pMapComponent[iIndex].begin();
	MAPCOMPONENT::iterator iter_end = m_pMapComponent[iIndex].end();

	for (iter; iter != iter_end; ++iter)
	{
		Engine::Safe_Release(iter->second);
	}
	m_pMapComponent[iIndex].clear();






	/*for (unsigned long i = 0; i < m_iContainerSize; ++i)
	{
		MAPCOMPONENT::iterator iter = m_pMapComponent[i].begin();
		MAPCOMPONENT::iterator iter_end = m_pMapComponent[i].end();

		for (iter; iter != iter_end; ++iter)
		{
			Engine::Safe_Release(iter->second);
		}
		m_pMapComponent[i].clear();
	}*/

	Engine::Safe_Delete_Array(m_pMapComponent);
}