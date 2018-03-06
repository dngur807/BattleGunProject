#include "Renderer.h"

#include "Scene.h"
#include "Export_Function.h"

Engine::CRenderer::CRenderer(MYGDI* pMyGDI)
	: m_pMyGDI(pMyGDI)
	, m_fTime(0.f)
	, m_dwCount(0)
{
}

Engine::CRenderer::~CRenderer(void)
{

}

void Engine::CRenderer::SetCurrentScene(CScene* pScene) { m_pScene = pScene; }

HRESULT Engine::CRenderer::InitScene(void)
{
	return S_OK;
}

void Engine::CRenderer::Render(const float& fTime)
{
	m_fTime += fTime;
	++m_dwCount;

	if (m_fTime >= 1.f)
	{
		//wsprintf(m_szFps, L"FPS : %d", m_dwCount);
		m_fTime = 0.f;
		m_dwCount = 0;
	}

	Engine::Get_GraphicDev()->RenderBegin();

	if (m_pScene != NULL)
		m_pScene->Render();

	/*Engine::Get_FontMgr()->Render(L"¹ÙÅÁ", D3DXVECTOR3(0.f, 0.f, 0.f)
		, m_szFps, D3DCOLOR_ARGB(255, 255, 255, 255));*/

	Engine::Get_GraphicDev()->RenderEnd();
}

Engine::CRenderer* Engine::CRenderer::Create(MYGDI* pMyGDI)
{
	CRenderer* pRenderer = new CRenderer(pMyGDI);
	if (FAILED(pRenderer->InitScene()))
	{
		Engine::Safe_Delete(pRenderer);
	}
	return pRenderer;
}