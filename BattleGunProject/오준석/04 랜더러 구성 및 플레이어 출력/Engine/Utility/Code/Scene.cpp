#include "Scene.h"
#include "Layer.h"

Engine::CScene::CScene(MYGDI* pMyGDI)
	: m_pMyGDI(pMyGDI)
{
	m_mapLayer.clear();
}

Engine::CScene::~CScene(void)
{
	Release();
}

int Engine::CScene::Update(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Update();
	}

	return 0;
}

void Engine::CScene::Render(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Render();
	}
}

void Engine::CScene::Release(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
}

map<WORD, Engine::CLayer*>* Engine::CScene::GetMapLayer(void)
{
	return &m_mapLayer;
}

const Engine::CComponent* Engine::CScene::GetComponent(const WORD& LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);
	if (iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetComponent(pObjKey, pComponentKey);
}

