#include "stdafx.h"
#include "Terrain.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "TerrainTex.h"
#include "StaticMesh.h"

CTerrain::CTerrain(Engine::MYGDI* pMyGDI)
	: Engine::CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_pTerrainVtx(NULL)
	, m_pIndex(NULL)
	, m_iIndexCnt(0)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CTerrain::~CTerrain(void)
{
	Release();
}

HRESULT CTerrain::Initialize(void)
{
	AddMapMesh();
	HRESULT		hr = AddComponent();
	FAILED_CHECK(hr);

	m_tMtrl.Ambient = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(0.f, 0.f, 0.f, 1.f);

	m_DirLight.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Direction = XMFLOAT3(1.f, -0.7f, 1.f);
	//m_pTerrainVtx = new Engine::VTXTEX[VTXCNTX * VTXCNTZ];
	//m_pBuffer->GetVtxInfo(m_pTerrainVtx);

	//m_pTerrainVtx = dynamic_cast<Engine::CTerrainTex*>(m_pBuffer)->GetVertex();
	//m_pIndex = new UINT[(VTXCNTX - 1) * (VTXCNTZ - 1) * 6];

	//hr = Engine::Get_QuadTree()->InitQuadTree(m_pTerrainVtx, VTXCNTX, VTXCNTZ);
	//FAILED_CHECK(hr);

	//hr = Engine::Get_QuadTree()->InitNeighbor(m_pTerrainVtx, Engine::Get_QuadTree(),
	//	VTXCNTX, VTXCNTZ);
	//FAILED_CHECK(hr);



	////
	MapMeshLoad();



	
	
	return S_OK;
}

void CTerrain::Release(void)
{
	Engine::Safe_Delete_Array(m_pIndex);
}

void CTerrain::MapMeshLoad()
{


}

HRESULT CTerrain::AddMapMesh()
{
	HRESULT hr = NULL;
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build1/FBX/", L"__LOD_Ace_A2_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_blackbox"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"blackbox.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_E1"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"E1.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_E2"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"E2.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_GestaltCol"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"GestaltCol.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);




	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_MassL"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"MassL.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);




	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P1"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P1.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P1_2"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P1_2.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P1_3"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P1_3.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P1_6"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P1_6.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P2"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P2.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P3"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P3.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P4"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P4.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P5"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P5.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P6"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P6.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);




	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P7"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P7.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P8"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P8.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_P9"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P9.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_SmallL"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"SmallL.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	return NO_ERROR;
}

int CTerrain::Update(void)
{
	//Engine::Get_Frustum()->Transform_Frustum_Local(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//Engine::Get_QuadTree()->IsinFrustum(m_pTerrainVtx);
	//Engine::Get_QuadTree()->Culling_LOD(m_pTerrainVtx, m_pIndex, &(m_iIndexCnt = 0));
	//Engine::Get_QuadTree()->CullingToQuadTree(m_pTerrainVtx, m_pIndex, &(m_iIndexCnt = 0));
	//Engine::Get_Frustum()->CullingForTerrain(m_pIndex, &(m_iIndexCnt = 0), m_pTerrainVtx
	//, VTXCNTX, VTXCNTZ);
	Engine::CGameObject::Update();

	return 0;
}

void CTerrain::Render(void)
{
	//와이어 프레임
	D3D11_RASTERIZER_DESC dxRasterDesc;
	::memset(&dxRasterDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	//dxRasterDesc.FillMode = D3D11_FILL_WIREFRAME;       //와이어
	dxRasterDesc.FillMode = D3D11_FILL_SOLID;          //아닌거
	dxRasterDesc.CullMode = D3D11_CULL_NONE;
	//dxRasterDesc.DepthClipEnable = TRUE;

	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);


	// 맵 매쉬 출력

	//vector<Engine::CStaticMesh*>::iterator	iter1 = m_VecMeshMap.begin();
	//vector<Engine::CStaticMesh*>::iterator	iter_end1 = m_VecMeshMap.end();
	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight1Tech();
	//for (iter1; iter1 != iter_end1; ++iter1)
	//{

	//	//m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//	(*iter1)->RenderMesh(XMLoadFloat4x4(&(*iter1)->GetInfo()->m_matWorld));
	//}

	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight0Tech();
	m_pMyGDI->pEffect->SetChangeScene(false);

	for (size_t i = 0; i < m_vecMesh.size(); ++i)
	{
		m_vecMesh[i]->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));

	}
	
}

HRESULT CTerrain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	/*pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));*/

	//Buffer
	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_TerrainTex");
	//m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	////Texture
	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_TerrainTex");
	//m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//StaticMesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_blackbox");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_E1");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_E2");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_MassL");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));


	/*pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_GestaltCol");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));*/

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P1");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P1_2");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));



	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P1_3");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));



	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P1_6");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));



	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P2");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));



	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P3");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P4");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));



	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P5");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);


	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P6");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));



	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P7");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));



	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P8");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));



	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_P9");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));


	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Mesh_SmallL");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_vecMesh.push_back(m_pMesh);
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));


	return S_OK;
}

CTerrain* CTerrain::Create(Engine::MYGDI* pMyGDI)
{
	CTerrain*		pGameObject = new CTerrain(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}




