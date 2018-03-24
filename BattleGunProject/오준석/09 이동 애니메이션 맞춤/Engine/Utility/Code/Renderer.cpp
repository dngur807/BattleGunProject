#include "Renderer.h"
#include "Scene.h"
#include "Export_Function.h"
#include "GameObject.h"

Engine::CRenderer::CRenderer(MYGDI* pMyGDI)
	: m_pMyGDI(pMyGDI)
	, m_fTime(0.f)
	, m_dwCount(0)
{
	ClearRenderGroup();
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

	Render_Priority();
	Render_NoneAlpha();
	Render_Alpha();
	Render_Effect();
	Render_UI();

	if (m_pScene != NULL)
		m_pScene->Render();

	/*Engine::Get_FontMgr()->Render(L"����", D3DXVECTOR3(0.f, 0.f, 0.f)
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

void Engine::CRenderer::Render_Priority(void)
{
	RENDERLIST::iterator	iter = m_RenderGroup[TYPE_PRIORITY].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[TYPE_PRIORITY].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

void Engine::CRenderer::Render_NoneAlpha()
{
	//���� ����

	RENDERLIST::iterator	iter = m_RenderGroup[TYPE_NONEALPHA].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[TYPE_NONEALPHA].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

void Engine::CRenderer::Render_Effect()
{

}

void Engine::CRenderer::Render_UI()
{
	
	Engine::Get_GraphicDev()->Belend_Begin();
	Engine::Get_GraphicDev()->ZBufferOff();
	RENDERLIST::iterator	iter = m_RenderGroup[TYPE_UI].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[TYPE_UI].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
	Engine::Get_GraphicDev()->ZBufferOn();
	Engine::Get_GraphicDev()->Blend_End();
}

void Engine::CRenderer::AddRenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	NULL_CHECK_RETURN(pGameObject, );

	m_RenderGroup[eRenderGroup].push_back(pGameObject);
}

void Engine::CRenderer::DeleteRenderObject(CGameObject* pGameObject)
{
	NULL_CHECK_RETURN(pGameObject, );

	for (int i = 0; i < TYPE_END; i++)
	{
		RENDERLIST::iterator	iter = m_RenderGroup[i].begin();
		RENDERLIST::iterator	iter_end = m_RenderGroup[i].end();
		for (; iter != iter_end; iter++)
		{
			if ((*iter) == pGameObject)
			{
				m_RenderGroup[i].erase(iter);
				return;
			}
		}
	}
}

void Engine::CRenderer::DeleteRenderObject(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	NULL_CHECK_RETURN(pGameObject, );
	auto iter = m_RenderGroup[eRenderGroup].begin();
	auto iter_end = m_RenderGroup[eRenderGroup].end();
	for (iter; iter != iter_end; )
	{
		if ((*iter) == pGameObject)
		{
			m_RenderGroup[eRenderGroup].erase(iter);
		}
		else
			++iter;
	}
}

void Engine::CRenderer::ClearRenderGroup()
{
	for (int i = 0; i < TYPE_END; ++i)
		m_RenderGroup[i].clear();
}

void Engine::CRenderer::Render_Alpha(void)
{
	m_RenderGroup[TYPE_ALPHA].sort(Compare_ViewZ);

	auto	iter = m_RenderGroup[TYPE_ALPHA].begin();
	auto	iter_end = m_RenderGroup[TYPE_ALPHA].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

bool Compare_ViewZ(Engine::CGameObject * pSour, Engine::CGameObject * pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

