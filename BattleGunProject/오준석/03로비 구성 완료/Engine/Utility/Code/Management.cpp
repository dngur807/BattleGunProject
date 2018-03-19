#include "Management.h"

#include "Renderer.h"
#include "Scene.h"

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

