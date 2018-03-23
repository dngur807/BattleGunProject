#include "Management.h"
#include "Renderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
	: m_pScene(nullptr)
	, m_pRenderer(nullptr)
	, m_pMyGDI(nullptr)
	, m_pFX(nullptr)
{
}

Engine::CManagement::~CManagement(void)
{
	Release();
}

HRESULT Engine::CManagement::InitManagement(MYGDI* pMyGDI)
{
	m_pMyGDI = pMyGDI;

	m_pRenderer = CRenderer::Create(pMyGDI);
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

