#include "GameObject.h"
#include "Export_Function.h"

Engine::CGameObject::CGameObject(MYGDI* pMyGDI)
	: m_pMyGDI(pMyGDI)
{
	m_mapComponent.clear();
	m_fViewZ = 0.f;
	m_eRenderGroup = TYPE_END;
}



Engine::CGameObject::~CGameObject(void)
{
	Release();
}

void Engine::CGameObject::Release(void)
{
	// 랜더그룹 등록되어 있으면 지우자
	/*if (m_eRenderGroup != TYPE_END)
		Engine::Get_Management()->DeleteRenderObject(m_eRenderGroup, this);*/

	for_each(m_mapComponent.begin(), m_mapComponent.end(), CDeleteMap());
	m_mapComponent.clear();
}


int Engine::CGameObject::Update(void)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Update();
	}

	return 0;
}

const Engine::CComponent* Engine::CGameObject::GetComponent(const TCHAR* wstrComponentKey)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.find(wstrComponentKey);
	if (iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}

// 알파 값 사용하는 애들 쓰자
void Engine::CGameObject::Compute_ViewZ(const XMVECTOR* pPos)
{
	XMFLOAT4X4		matView = m_pMyGDI->pEffect->GetView();
	XMMATRIX		matrixView;
	matrixView = XMMatrixInverse(NULL, XMLoadFloat4x4(&matView));

	
	XMVECTOR vRight = matrixView.r[3];
	XMVECTOR vLen = XMVector3Length((vRight - *pPos));

	m_fViewZ = XMVectorGetX(vLen);//길이 모두 같은 값나옴
}

