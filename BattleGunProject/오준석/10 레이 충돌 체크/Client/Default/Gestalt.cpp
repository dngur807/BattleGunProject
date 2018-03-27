#include "stdafx.h"
#include "Gestalt.h"
#include "Transform.h"
#include "Include.h"
#include "StaticMesh.h"
#include "Export_Function.h"
CGestalt::CGestalt(Engine::MYGDI* pMyGDI)
	: CLandObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CGestalt::~CGestalt()
{

}

HRESULT CGestalt::Initialize()
{
	FAILED_CHECK(AddComponent());
	SetPos(XMFLOAT3(0, 0, 0));
	SetScale(XMFLOAT3(0.125f, 0.125f, 0.125f));

	//재질 설정
	



	return 0;
}

int CGestalt::Update(void)
{
	CLandObject::Update();//컴포넌트 업데이트
	return 0;
}

void CGestalt::Render(void)
{
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();

	m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);

	//for (int i = 0; i < 9; i++)
	//{
	//	m_pP[i]->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//}
	//m_pE[0]->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//m_pE[1]->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//m_pMassL->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//m_pSmallL->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
}

CGestalt* CGestalt::Create(Engine::MYGDI* pMyGDI)
{
	CGestalt* pGestalt = new CGestalt(pMyGDI);
	if (FAILED(pGestalt->Initialize()))
		Engine::Safe_Delete(pGestalt);
	return pGestalt;
}

HRESULT CGestalt::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));


	////StaticMesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_P1");
	m_pP[0] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"P1", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_P2");
	m_pP[1] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"P2", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_P3");
	m_pP[2] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"P3", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_P4");
	m_pP[3] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"P4", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_P5");
	m_pP[4] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"P5", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_P6");
	m_pP[5] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"P6", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_P7");
	m_pP[6] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"P7", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_P8");
	m_pP[7] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"P8", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_P9");
	m_pP[8] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"P9", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_E1");
	m_pE[0] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"E1", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_E2");
	m_pE[1] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"E2", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_MassL");
	m_pMassL = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"MassL", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_SmallL");
	m_pSmallL = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"SmallL", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_Gestalt_BlackBox");
	m_pBlackBox = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"BlackBox", pComponent));


	//StaticMesh
	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_GESTALT, L"Mesh_GestaltCol");
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"CollisionMesh", pComponent));

	return S_OK;
}

