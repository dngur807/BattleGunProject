#include "stdafx.h"
#include "BackGround.h"
#include "StaticMesh.h"
#include "DynamicCamera.h"
#include "Transform.h"
#include "Include1.h"
#include "Terrain.h"
#include "NaviMgr.h"
#include "Stone.h"

CBackGround::CBackGround(Engine::MYGDI * pMyGDi)
	: m_pMyGDI(pMyGDi)
	, m_pStaticMesh(nullptr)
	, m_dwMeshNum(0)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_bTileCreate(true)
	, m_bTileModify(false)
	, m_bTerrainModify(false)
	, MeshAngleX(0.f)
	, MeshAngleY(0.f)
	, MeshAngleZ(0.f)
	, MeshX(0.f)
	, MeshY(0.f)
	, MeshZ(0.f)
	, MeshSizeX(1.f)
	, MeshSizeY(1.f)
	, MeshSizeZ(1.f)
	, m_iCount(0)
	, m_iOption(0)
	, m_iSamePoint(0)
	, stoney(0.5f)
{
}

CBackGround::~CBackGround(void)
{
	if (m_vecMesh.size() != 0)
	{
		vector<Engine::CStaticMesh*>::iterator	iter = m_vecMesh.begin();
		vector<Engine::CStaticMesh*>::iterator	iter_end = m_vecMesh.end();
		for (iter; iter != iter_end; ++iter)
		{
			Safe_Delete(*iter);
			m_vecMesh.erase(iter);
		}
		m_vecMesh.clear();
	}
}

HRESULT CBackGround::AddComponent(void)
{
	/*m_pStaticMesh = Engine::CStaticMesh::Create(m_pMyGDI,
	L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build1/FBX/", L"__LOD_Ace_A2_Mesh0.FBX");*/

	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_8");
	//m_pStaticMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);

	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));
	
	return NOERROR;
}

HRESULT CBackGround::InitScene(void)
{


	HRESULT hr = NULL;
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build1/FBX/", L"__LOD_Ace_A2_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	AddBuilding();
	FAILED_CHECK(AddComponent());
	FAILED_CHECK(LoadNaviMesh());

	m_pDynamicCamera = CDynamicCamera::Create(m_pMyGDI
		, &XMFLOAT3(0.f, 5.f, -1.f), &XMFLOAT3(0.f, 0.f, 0.f));

	//Terrain
	//hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::CVIBuffer::BUFFER_TILEMAP
	//	, L"Buffer_TerrainTex"
	//	, VTXCNTX, VTXCNTZ, VTXITV);
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	//hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::TEXTURE_NORMAL
	//	, L"Texture_TerrainTex"
	//	//, L"../bin/Resources/Texture/Terrain/Terrain%d.png", 1);
	//	, L"../bin/Resources/Texture/Terrain/Korhal%d.png",1);
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	//m_pTerrain = CTerrain::Create(m_pMyGDI);
	//NULL_CHECK_RETURN_MSG(m_pTerrain, E_FAIL, L"CTerrain 생성 실패");

	m_pStone = CStone::Create(m_pMyGDI, m_pDynamicCamera);
	NULL_CHECK_RETURN_MSG(m_pStone, E_FAIL, L"m_pStone 생성 실패");
	// 벡터 초기화
	vector<Engine::CStaticMesh*>::iterator	iter = m_vecMesh.begin();
	vector<Engine::CStaticMesh*>::iterator	iter_end = m_vecMesh.end();
	for (iter; iter != iter_end; ++iter)
	{
		(*iter)->Release();
		Safe_Delete(*iter);
	}
	m_vecMesh.clear();

	//LoadMap();



	return NOERROR;
}



CBackGround * CBackGround::Create(Engine::MYGDI * pMyGDi)
{
	CBackGround* pBackGround = new CBackGround(pMyGDi);
	if (FAILED(pBackGround->InitScene()))
		Engine::Safe_Delete(pBackGround);

	//MeshAdd();
	return pBackGround;
}

void CBackGround::CheckClockWise(XMFLOAT3 vecNavMesh[3])
{
	//XMFLOAT3 vec1 = vecNavMesh[1] - vecNavMesh[0];
	//XMFLOAT3 vec2 = vecNavMesh[2] - vecNavMesh[1];

	XMFLOAT3 vec1 = XMFLOAT3(vecNavMesh[1].x - vecNavMesh[0].x,
		vecNavMesh[1].y - vecNavMesh[0].y,
		vecNavMesh[1].z - vecNavMesh[0].z);

	XMFLOAT3 vec2 = XMFLOAT3(vecNavMesh[2].x - vecNavMesh[1].x,
		vecNavMesh[2].y - vecNavMesh[1].y,
		vecNavMesh[2].z - vecNavMesh[1].z);

	XMFLOAT3 vecCross;

	//XMVector3Cross(vec1, vec2);

	//D3DXVec3Cross(&vecCross, &vec1, &vec2);


	//XMVECTOR vNormal = XMLoadFloat2(pCell->GetLine2D(i)->GetNormal());
	//XMFLOAT2 vPoint = XMFLOAT2(pCell->GetPoint()[i].x,
	//	pCell->GetPoint()[i].z);
	//XMVECTOR vPointToPos = vPos - XMLoadFloat2(&vPoint);

	XMStoreFloat3(&vecCross, XMVector3Cross(XMLoadFloat3(&vec1), XMLoadFloat3(&vec2)));


	if (vecCross.y < 0)
	{
		XMFLOAT3 vecTemp = vecNavMesh[1];
		vecNavMesh[1] = vecNavMesh[2];
		vecNavMesh[2] = vecTemp;
	}
}



void CBackGround::Update()
{
	//cout << m_dwMeshNum << endl;
	//133이하이어야함!
	MeshModify();
	NaviModify();
	m_pDynamicCamera->Update();
	Engine::Get_Input()->SetInputState();



	m_pInfo->m_vPos = m_pDynamicCamera->GetEye();
	dynamic_cast<CStone*>(m_pStone)->GetStoneY(stoney);
	m_pStone->Update();

	//m_pStone->();
}

void CBackGround::Render()
{
	//cout << m_dwMeshNum << endl;
	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight2Tech();
	Engine::Get_GraphicDev()->RenderBegin();

	//
	//cout << m_vecSaveMesh.size() << endl;
	if(m_vecSaveMesh.size()>=3)
	Engine::Get_NaviMgr()->Render_NaviMesh();

	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight1Tech();
	//와이어 프레임
	D3D11_RASTERIZER_DESC dxRasterDesc;
	::memset(&dxRasterDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	//dxRasterDesc.FillMode = D3D11_FILL_WIREFRAME;       //와이어
	dxRasterDesc.FillMode = D3D11_FILL_SOLID;          //아닌
	dxRasterDesc.CullMode = D3D11_CULL_NONE;
	dxRasterDesc.DepthClipEnable = TRUE;
	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);



	XMFLOAT4X4 matWorld;
	XMMATRIX P = XMMatrixIdentity();
	XMStoreFloat4x4(&matWorld, P);

	//m_pStaticMesh->RenderMesh(XMLoadFloat4x4(&matWorld));


	vector<Engine::CStaticMesh*>::iterator	iter = m_vecMesh.begin();
	vector<Engine::CStaticMesh*>::iterator	iter_end = m_vecMesh.end();
	for (iter; iter != iter_end; ++iter)
	{
		(*iter)->RenderMesh(XMLoadFloat4x4(&(*iter)->GetInfo()->m_matWorld));

		//m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	}

	//m_pTerrain->Render();
	m_pStone->Render();
	Engine::Get_GraphicDev()->RenderEnd();
}



void CBackGround::MeshAdd()
{
	// 메쉬 생성

	//XMFLOAT4X4 matWorld;
	//XMMATRIX P = XMMatrixIdentity();
	//XMStoreFloat4x4(&matWorld, P);


	XMMATRIX P = XMMatrixIdentity();
	XMFLOAT4X4 matWorld, matView, matProj;
	XMStoreFloat4x4(&matWorld, P);
	XMStoreFloat4x4(&matView, P);
	XMStoreFloat4x4(&matProj, P);

	WORD dwTriCheck = 0;
	float fDist = 0.f;

	WORD dwDestIndex = 99;
	float fDestDist = 99999.f;
	XMFLOAT3 vDestPos(0.f, 0.f, 0.f);
	XMFLOAT3 vDestSize(1.f, 1.f, 1.f);

	if (99 != dwDestIndex)
	{

	}
	else
	{
		//TerrainTex와 충돌 체크
		//if (NULL != m_pTerrain)
		{
			/*	m_pInfo->m_vPos.z += 500;
			m_pInfo->m_vPos.y -= 50;*/

			m_pInfo->m_vPos.z += 5;
			m_pInfo->m_vPos.y -= 0.5f;
			vDestPos = m_pInfo->m_vPos;



			Engine::CComponent*		pComponent = NULL;
			Engine::CStaticMesh*		pBuffer;
			m_vSize = XMFLOAT3(0.01f, 0.01f, 0.01f);

			switch (m_dwMeshNum)
			{
			//case 0: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build1"); break;
			//case 1: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build2"); break;
			//case 2: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build3"); break;
			//case 3: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build4"); break;
			//case 4: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build5"); break;
			//case 5: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build6"); break;
			//case 6: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build7"); break;
			//case 7: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build8"); break;
			//case 8: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build9"); break;
			//case 9: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build10"); break;
			//case 10: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build11"); break;
			//case 11: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build12"); break;
			//case 12: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd13_1"); break;
			//case 13: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd13_2"); break;
			//case 14: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_1"); break;
			//case 15: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_2"); break;
			//case 16: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_3"); break;
			//case 17: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_4"); break;
			//case 18: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_5"); break;
			//case 19: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_6"); break;
			//case 20: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_7"); break;
			//case 21: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_8"); break;
			//case 22: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_9"); break;
			//case 23: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_10"); break;
			//case 24: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_11"); break;
			//case 25: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_12"); break;
			//case 26: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_13"); break;
			//case 27: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_14"); break;
			//case 28: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_15"); break;
			//case 29: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_16"); break;
			//case 30: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_17"); break;
			//case 31: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_18"); break;
			//case 32: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_19"); break;
			//case 33: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_20"); break;
			//case 34: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_21"); break;
			//case 35: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_22"); break;
			//case 36: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_23"); break;
			//case 37: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_24"); break;
			//case 38: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_25"); break;
			//case 39: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_1"); break;
			//case 40: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_2"); break;
			//case 41: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_3"); break;
			//case 42: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_4"); break;
			//case 43: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_5"); break;
			//case 44: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_6"); break;
			//case 45: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_7"); break;
			//case 46: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_8"); break;
			//case 47: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_9"); break;
			//case 48: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_10"); break;
			//case 49: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_11"); break;
			//case 50: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_12"); break;
			//case 51: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_13"); break;
			//case 52: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_14"); break;
			//case 53: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_15"); break;
			//case 54: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_16"); break;
			//case 55: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_17"); break;
			//case 56: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_18"); break;
			//case 57: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_19"); break;
			//case 58: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_20"); break;
			//case 59: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_21"); break;
			//case 60: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_22"); break;
			//case 61: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_23"); break;
			//case 62: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_24"); break;
			//case 63: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_25"); break;
			//case 64: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_26"); break;
			//case 65: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_27"); break;
			//case 66: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_28"); break;
			//case 67: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_29"); break;
			//case 68: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_30"); break;
			//case 69: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_31"); break;
			//case 70: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_32"); break;
			//case 71: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_33"); break;
			//case 72: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_34"); break;
			//case 73: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_35"); break;
			//case 74: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_36"); break;
			//case 75: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_37"); break;
			//case 76: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_38"); break;
			//case 77: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_39"); break;
			//case 78: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_40"); break;
			//case 79: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_41"); break;
			//case 80: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_42"); break;
			//case 81: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_43"); break;
			//case 82: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_44"); break;
			//case 83: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_45"); break;
			//case 84: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_46"); break;
			//case 85: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_1"); break;
			//case 86: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_2"); break;
			//case 87: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_3"); break;
			//case 88: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_4"); break;
			//case 89: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_5"); break;
			//case 90: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_6"); break;
			//case 91: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_7"); break;
			//case 92: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_8"); break;
			//case 93: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_9"); break;
			//case 94: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_10"); break;
			//case 95: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_11"); break;
			//case 96: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_12"); break;
			//case 97: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_13"); break;
			//case 98: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_14"); break;
			//case 99: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_15"); break;
			//case 100: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_16"); break;
			//case 101: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_17"); break;
			//case 102: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_18"); break;
			//case 103: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_19"); break;
			//case 104: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_20"); break;
			//case 105: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_21"); break;
			//case 106: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_22"); break;
			//case 107: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_23"); break;
			//case 108: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_24"); break;
			//case 109: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_25"); break;
			//case 110: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_26"); break;
			//case 111: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_1"); break;
			//case 112: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_2"); break;
			//case 113: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_3"); break;
			//case 114: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_4"); break;
			//case 115: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_5"); break;
			//case 116: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_6"); break;
			//case 117: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_7"); break;
			//case 118: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_8"); break;
			//case 119: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_9"); break;
			//case 120: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_10"); break;
			//case 121: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_11"); break;
			//case 122: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_12"); break;
			//case 123: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_13"); break;
			//case 124: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_14"); break;
			//case 125: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_15"); break;
			//case 126: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_16"); break;
			//case 127: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_17"); break;
			//case 128: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_18"); break;
			//case 129: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_19"); break;
			//case 130: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_20"); break;
			//case 131: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_21"); break;
			//case 132: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_22"); break;
			//case 133: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_1"); break;
			//case 134: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_2"); break;
			//case 135: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_3"); break;
			//case 136: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_4"); break;
			//case 137: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_5"); break;
			//case 138: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_6"); break;
			//case 139: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_7"); break;
			//case 140: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_8"); break;
			//case 141: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_9"); break;
			//case 142: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_10"); break;
			//case 143: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_11"); break;
			//case 144: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_12"); break;
			//case 145: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_13"); break;
			//case 146: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_14"); break;
			//case 147: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_15"); break;
			//case 148: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_16"); break;
			//case 149: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_17"); break;
			//case 150: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_18"); break;
			//case 151: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_19"); break;
			//case 152: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_20"); break;
			//case 153: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_21"); break;
			//case 154: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_22"); break;
			//case 155: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_23"); break;
			//case 156: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_24"); break;
			//case 157: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_25"); break;
			//case 158: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_26"); break;
			//case 159: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_27"); break;
			//case 160: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_28"); break;
			//case 161: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_29"); break;
			//case 162: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_30"); break;
			//case 163: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_31"); break;
			//case 164: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_32"); break;
			//case 165: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_33"); break;
			//case 166: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_34"); break;
			//case 167: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_35"); break;
			//case 168: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_36"); break;
			//case 169: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_37"); break;
			//case 170: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_38"); break;
			//case 171: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_39"); break;


			case 0: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_1"); break;
			case 1: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_2"); break;
			case 2: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_3"); break;
			case 3: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_4"); break;
			case 4: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_5"); break;
			case 5: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_6"); break;
			case 6: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_7"); break;
			case 7: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_8"); break;
			case 8: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_9"); break;
			case 9: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_10"); break;
			case 10: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_11"); break;
			case 11: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_12"); break;
			case 12: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_13"); break;
			case 13: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_14"); break;
			case 14: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_15"); break;
			case 15: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_16"); break;
			case 16: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_17"); break;
			case 17: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_18"); break;
			case 18: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_19"); break;
			case 19: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_20"); break;
			case 20: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_21"); break;
			case 21: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_22"); break;
			case 22: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_23"); break;
			case 23: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_24"); break;
			case 24: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_25"); break;
			case 25: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_26"); break;
			case 26: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_27"); break;
			case 27: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_28"); break;
			case 28: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_29"); break;
			case 29: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_30"); break;
			case 30: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_31"); break;
			case 31: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_32"); break;
			case 32: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_33"); break;
			case 33: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_34"); break;
			case 34: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_35"); break;
			case 35: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_36"); break;
			case 36: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_37"); break;
			case 37: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_38"); break;
			case 38: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_39"); break;
			case 39: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_40"); break;
			case 40: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_41"); break;
			case 41: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_42"); break;
			case 42: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_43"); break;
			case 43: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_44"); break;
			case 44: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_45"); break;
			case 45: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_46"); break;
			case 46: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_47"); break;
			case 47: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_48"); break;
			case 48: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_49"); break;
			case 49: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_50"); break;
			case 50: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_51"); break;
			case 51: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_52"); break;
			case 52: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_53"); break;
			case 53: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_54"); break;
			case 54: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_55"); break;
			case 55: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_56"); break;
				//case 56: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_57"); break;
				//case 57: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_58"); break;
			case 58: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_59"); break;
			case 59: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_60"); break;
			case 60: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_61"); break;
			case 61: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_62"); break;
			case 62: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_63"); break;
			case 63: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_64"); break;
			case 64: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_65"); break;
			case 65: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_66"); break;
			case 66: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_67"); break;
			case 67: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_68"); break;
			case 68: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_69"); break;
			case 69: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_70"); break;
			case 70: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_71"); break;
			case 71: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_72"); break;
			case 72: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_73"); break;
			case 73: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_74"); break;
			case 74: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_75"); break;
			case 75: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_76"); break;
			case 76: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_77"); break;
			case 77: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_78"); break;
			case 78: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_79"); break;
			case 79: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_80"); break;
			case 80: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_81"); break;
			case 81: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_82"); break;
			case 82: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_83"); break;
			case 83: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_84"); break;
			case 84: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_85"); break;
			case 85: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_86"); break;
			case 86: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_87"); break;
			case 87: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_88"); break;
			case 88: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_89"); break;
			case 89: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_90"); break;
			case 90: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_91"); break;
			case 91: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_92"); break;
			case 92: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_93"); break;
			case 93: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_94"); break;
			case 94: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_95"); break;
			case 95: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_96"); break;
			case 96: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_97"); break;
			case 97: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_98"); break;
			case 98: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_99"); break;
			case 99: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_100"); break;
			case 100: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_101"); break;
			case 101: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_102"); break;
			case 102: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_103"); break;
			case 103: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_104"); break;
			case 104: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_105"); break;
			case 105: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_106"); break;
			}


			//105번까지만
			pBuffer = dynamic_cast<Engine::CStaticMesh*>(pComponent); NULL_CHECK(pBuffer);
			pBuffer->SetvPosInfo(vDestPos);
			pBuffer->SetvSizeInfo(m_vSize);
			pBuffer->SetAngleInfo(m_fAngle[Engine::ANGLE_X], m_fAngle[Engine::ANGLE_Y], m_fAngle[Engine::ANGLE_Z]);
			pBuffer->SetMeshNum(m_dwMeshNum);
			m_vecMesh.push_back(pBuffer);
		}
	}
}
void CBackGround::MeshDelete()
{
	//cout << "불리고있엉" << endl;
	float fDist = 0.f;

	int iDestIndex = -1;
	float fDestDist = 99999.f;

	if (m_vecMesh.size() != 0)
	{
		vector<Engine::CStaticMesh*>::iterator	iter = m_vecMesh.end();
		iter--;

		for (int i = 0; i < iDestIndex; ++i)
		{
			++iter;
		}

		Safe_Delete(*iter);
		m_vecMesh.erase(iter);
	}

}

void CBackGround::MeshModify()
{
	float	fTime = Engine::Get_TimeMgr()->DeltaTime();

	vector<Engine::CStaticMesh*>::iterator	iter = m_vecMesh.end();
	if (m_vecMesh.size() > 0)
	{
		iter--;
	}
	else {
		return;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_LEFT) & 0x80)
	{
		MeshAngleY += 0.01f;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_RIGHT) & 0x80)
	{
		MeshAngleY -= 0.01f;

	}
	if (GetAsyncKeyState('1'))
	{
		MeshAngleX += 0.01f;

	}
	if (GetAsyncKeyState('2'))
	{
		MeshAngleX -= 0.01f;

	}
	if (GetAsyncKeyState('3'))
	{
		MeshAngleZ += 0.01f;

	}
	if (GetAsyncKeyState('4'))
	{
		MeshAngleZ -= 0.01f;

	}
	if (GetAsyncKeyState('5'))
	{
		MeshAngleX = 0.f;
		MeshAngleY = 0.f;
		MeshAngleZ = 0.f;

	}

	if (GetAsyncKeyState('O'))
	{
		(*iter)->GetInfo()->m_vPos.y += 10.f * fTime;

	}
	if (GetAsyncKeyState('P'))
	{
		(*iter)->GetInfo()->m_vPos.y -= 10.f * fTime;

	}
	if (Engine::Get_Input()->GetDIKeyState(DIK_UP) & 0x80)
	{

		XMVECTOR VP = XMLoadFloat3(&(*iter)->GetInfo()->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&(*iter)->GetInfo()->m_vDir);
		VP += VD * 50.f * fTime;
		XMStoreFloat3(&(*iter)->GetInfo()->m_vPos, VP);
		//(*iter)->GetInfo()->m_vPos += (*iter)->GetInfo()->m_vDir*0.01f;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_DOWN) & 0x80)
	{
		XMVECTOR VP = XMLoadFloat3(&(*iter)->GetInfo()->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&(*iter)->GetInfo()->m_vDir);
		VP -= VD * 50.f * fTime;
		XMStoreFloat3(&(*iter)->GetInfo()->m_vPos, VP);
		//(*iter)->GetInfo()->m_vPos -= (*iter)->GetInfo()->m_vDir*0.01f;


	}


	(*iter)->SetAngleInfo(MeshAngleX, MeshAngleY, MeshAngleZ);
	//(*iter)->SetvPosInfo(XMFLOAT3(MeshX, MeshY, MeshZ));
	//(*iter)->SetvSizeInfo(XMFLOAT3(MeshSizeX, MeshSizeY, MeshSizeZ));
	//(*iter)->GetInfo()->m_vDir= XMFLOAT3 (0.f, 0.f, 1.f);


}

void CBackGround::NaviModify()
{
	//cout << "stoney :" << "\t" << stoney << endl;
	float	fTime = Engine::Get_TimeMgr()->DeltaTime();
	if (GetAsyncKeyState('O'))
	{
		stoney += fTime*5.f;

	}
	if (GetAsyncKeyState('P'))
	{
		stoney -= fTime*5.f;

	}
	if (GetAsyncKeyState('I'))
	{
		stoney = 0.f;

	}
}



HRESULT CBackGround::AddBuilding()
{
	HRESULT hr = NULL;
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build1/FBX/", L"__LOD_Ace_A2_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build2/FBX/", L"__LOD_Ace_A2_Mesh45.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build3/FBX/", L"__LOD_Ace_A2_Mesh5.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build4/FBX/", L"__LOD_CityY_19_Mesh92.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build5/FBX/", L"__LOD_CityZ_00_Mesh11.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build6/FBX/", L"__LOD_CityZ_12_Mesh15.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build7/FBX/", L"__LOD_CityY_19_Mesh92.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build8/FBX/", L"__LOD_Stagg_A2_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build9/FBX/", L"__LOD_CityZ_00_Mesh30.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build10/FBX/", L"__LOD_CityZ_01_Mesh50.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build11/FBX/", L"__LOD_CityZ_02_Mesh44.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build12/FBX/", L"__LOD_CityZ_10_Mesh109.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd13_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd13/FBX/", L"__LOD_CityX_01_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd13_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd13/FBX/", L"__LOD_CityX_01_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh1.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh4.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh6.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh5.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh10.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh13.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh20.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_13", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh21.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_14", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh4.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_15", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_16", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh11.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_17", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh13.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_18", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh18.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_19", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh22.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_20", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh39.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_21", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh70.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_22", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh72.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_23", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"Debris_WoodenBoat_Covered.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_24", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"Lockup_Floor_512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_25", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"Vehicle_Boat_Generic.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_01_Mesh6.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_04_Mesh71.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh3.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh5.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh6.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh10.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh12.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh14.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh15.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_13", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh16.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_14", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh17.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_15", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh19.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_16", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh21.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_17", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh26.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_18", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh28.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_19", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh30.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_20", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh33.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_21", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh36.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_22", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh37.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_23", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh38.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_24", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh39.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_25", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh40.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_26", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh43.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_27", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh44.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_28", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh45.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_29", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh46.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_30", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh48.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_31", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh51.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_32", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh52.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_33", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh54.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_34", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh61.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_35", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh63.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_36", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh64.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_37", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh67.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_38", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Bank_Lift.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_39", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"City_Shopfront_Steel_Shutter512_double.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_40", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Film_Floor_EdgeGrate_Pillar.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_41", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"GamePlayOBJ_ReWire_Panel.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_42", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Lockup_Cells_Wall_512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_43", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Lockup_Floor_512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_44", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Lockup_Floor_Carpet_Clean_256x512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_45", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"SIGNInternationalPlaza.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_46", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"TANK_Lite_Tank_Base.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Warehouse_Floor_02_Vent.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"ARCH_Sewer_Pipe_WallA_Loops_512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"CAT.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"DownTown_01.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Objects_SpeakersBalloons.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_ArkhamIsland.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_Building03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_BuildingBlock02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_BuildingBlock03b.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_BuildingBlock04.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_lighthouse.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_LODS_01.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_13", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_LODS_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_14", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_LODS_03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_15", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_LODS_04.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_16", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"GS_Island_Hangbridge.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_17", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"GS_Island_Stonebridge.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_18", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"OBJ_Boat_Barge.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_19", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Security_Hut_Bridger.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_20", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Security_Hut_C4.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_21", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Tyger_Helicopter.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_22", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Underworld_Tower_LOD_Mid.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_23", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"UW_OBJ_Sewer.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_24", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Warehouse_Floor_01.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_25", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Warehouse_Floor_01_Vent.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_26", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Warehouse_Floor_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_BSP_B1_Main.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_BSP_B1_Main_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_BSP_B1_Main_05.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_BSP_B1_SwimmingPool.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Road_C2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Road_C5.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Rubble_StreetDetritus.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Skyline_CityFar.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Skyline_LODS_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Skyline_LODS_03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Skyline_LODS_04.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Skyline_Streets.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_13", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Skyline_WonderBuilding1.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_14", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Skyline_WonderFoundation.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_15", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"Gotham_Snow_768.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_16", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"GS_Island_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_17", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"GS_Island_03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_18", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"GS_Island_04.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_19", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"OBJ_Boat_Barge.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_20", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"OW_River_terrain.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_21", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"OW_River_terrain2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Terrain_22", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/Terrain/FBX/", L"PDLC_OW_C_Road_C1.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	////133개
	////39개더추가
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_1", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_Ace_B1_Mesh6.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_2", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_Bank_Large_Mesh11.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_3", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_Bank_Med_Mesh15.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_4", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_01_Mesh4.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_5", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_19_Mesh8.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_6", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_20_Mesh3.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_7", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_20_Mesh4.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_8", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_20_Mesh8.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_9", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_20_Mesh13.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_10", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_20_Mesh48.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_11", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_28_Mesh11.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_12", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_28_Mesh16.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_13", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_28_Mesh22.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_14", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_28_Mesh25.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_15", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_U16_Mesh24.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_16", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityX_U16_Mesh26.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_17", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityY_02_Mesh44.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_18", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"__LOD_CityY_04_Mesh54.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_19", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"Ace_Exterior_DOCKSign.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_20", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"Ace_Exterior_SecurityOfficeSign.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_21", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"Bank_of_gotham.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_22", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"City_ARCH_GlassTower_HotelSign.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_23", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"City_ARCH_Hosptial_Sign_Elliot.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_24", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"City_ARCH_WaynePlaza_WLogoTEXT.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_25", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"CITY_Neon_Dragon.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_26", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"CITY_Neon_Generic_Jewelry.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_27", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"City_Neon_gotham_today.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_28", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"City_Neon_Hospital_v2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_29", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"City_Neon_Metrovision.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_30", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"CITY_OBJ_InfoBooth_Model.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_31", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"CITY_TunnelSign_Parking.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_32", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"Delivery_Van.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_33", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"Dumpster_Truck.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_34", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"Generic_Van.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_35", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"Gotham_ACEchemicals2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_36", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"OBJ_Mobilephone.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_37", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"OBJ_Mobilephone_2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_38", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"shark_film.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Last2_39", L"../../Client/bin/Resources/Mesh/StaticMesh/Last2/FBX/", L"VFX_Player_Batman_AR.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);



	//alt누르고 복사한후 하나에서 복붙
	//stage2
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_1", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Dome.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_2", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Arch.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_3", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Arch_Control_Booth_Window.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_4", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Fences_Partition_WoodAdmin.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_5", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Floors_Court_Planks1_Broken.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_6", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Floors_Theatre.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_7", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Floors_Theatre_Damaged.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_8", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Ironwork_Supports_Girder128.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_9", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Ironwork_Supports_Girder1024.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_10", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Panels_Deco256.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_11", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_ArtNouveau_Marble.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_12", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Catacombs_Rubble_Small_01Coll.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_13", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Court_Roof.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_14", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Court_RoofBeams.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_15", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Orphanage_Arch.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_16", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Orphanage_Ledge512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_17", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Orphanage_Pillar.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_18", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Theatre_Balcony_1.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_19", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Theatre_Balcony_2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_20", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Theatre_Corner.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_21", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Theatre_Corner_Top.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_22", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Pillars_Theatre_CornerDome.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_23", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"arch_rubble_pile_concrete01_Coll_SIMPLIFIED.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_24", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Sewer_Pipe_WallA_Wire01.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_25", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Sewer_Pipe_WallA_Wire02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_26", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Sewer_Pipe_WallA_Wire03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_27", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Arch_SW1_Walls_FurnaceShaft_ColumnBroken.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_28", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Walls_Court_BasePanels_Clean.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_29", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Walls_Court_Plaster_1.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_30", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"ARCH_Walls_Court_WallPanels_Clean.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_31", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Batman_Screen_Skin.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_32", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Blast_Radius_Cylinder.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_33", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Broken_Boards.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_34", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"BurningChairs.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_35", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"BurningFloor_Aftermath.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_36", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Chair_Rubble.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_37", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Chair01.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_38", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Chair02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_39", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Chair03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_40", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Chair05.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_41", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Chair08.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_42", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Chair09.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_43", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Chair10.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_44", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Ball_Mesh.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_45", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Blob.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_46", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Body_Coup_Skin.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_47", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Body_Skin.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_48", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Coup_Skin.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_49", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Coup_Splitted_Skin.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_50", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Drone_Full.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_51", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Drone_Horiz_Btm_Slice.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_52", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Drone_Horiz_Top_Slice.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_53", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Drone_Thin_Diag_L_Btm_Slice.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_54", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Drone_Thin_Diag_L_Top_Slice.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_55", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Drone_Thin_Diag_R_Btm_Slice.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_56", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Drone_Thin_Diag_R_Top_Slice.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
  //hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_57", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Arch_Control_BooClayface_Drone_Vert_L_Sliceth_Window.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
  //hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_58", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Arch_Control_BClayface_Drone_Vert_R_Sliceooth_Window.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_59", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Frozen.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_60", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Left_Arm_Skin.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_61", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Morph_Left_Axe_Source.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_62", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Morph_Left_Hammer_Source.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_63", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Morph_Right_Axe_Source.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_64", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Morph_Right_Hammer_Source.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_65", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Paddle.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_66", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Projectile.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_67", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Projectile2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_68", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Clayface_Right_Arm_Skin.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_69", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Cone.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_70", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Doors_Court_Double_Internal.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_71", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"DOORS_Static_Court_Wooden_192x384.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_72", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Elec_Mesh.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_73", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Elec_Mesh2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_74", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_Bataramg_Sonic_Anim.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_75", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_Batarang.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_76", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_Batarang_Controllable_Batarang.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_77", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_Batracker_Explosive_Gel_Detonator.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_78", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_Explosive_Gel.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_79", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_GrappleGun.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_80", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_GrappleGun_Bat_Claw.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_81", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_GrappleGun_Boost.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_82", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_GrappleGun_Claw.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_83", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_LineLauncher.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_84", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_Resonator.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_85", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Equipment_Smokebomb.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_86", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Explosive_Gel_Dummy_Rig.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_87", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FIT_Cables_Thin_Hanging512Loop.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_88", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FIT_Electric_Emergency_Button_Joker.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_89", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FIT_Pipes_Industrial_copper_part01.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_90", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FIT_Pipes_Industrial_copper_part02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_91", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FIT_TheMachine_Spindle.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_92", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FIT_TheMachine_WIRES.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_93", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FIT_Visitor_Generator02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_94", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Fracture_IcePlatform.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_95", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"Freeze_Jammer.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_96", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FreezeBlast.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_97", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FreezeCluster.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_98", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FX_electriccurrentArc.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_99", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FX_electriccurrentSmall.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_100", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"fx_Goo.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_101", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"fx_goo_BatParts_T.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_102", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"fx_goo_BatParts_T2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_103", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"fx_goo_BatParts_WL.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_104", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"fx_goo_BatParts_WR.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_105", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FX_Grime_Dust_Piles_Straight.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"Stage2_106", L"../../Client/bin/Resources/Mesh/StaticMesh/Stage2/FBX/", L"FX_Grime_Dustpile.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	return NOERROR;
}

void CBackGround::ADDVtx()
{

	//
	// 네비메쉬 생성
	Engine::VTXCOL*		pVertex = NULL;
	WORD dwTriCheck = 0;
	float fDist = 0.f;

	WORD dwDestIndex = 99;
	float fDestDist = 99999.f;
	XMFLOAT3 vDestPos(0.f, 0.f, 0.f);
	XMFLOAT3 vDestSize(1.f, 1.f, 1.f);

	if (99 != dwDestIndex)
	{

	}
	else
	{
		//TerrainTex와 충돌 체크
		//if (NULL != m_pTerrain)
		{

			vDestPos = m_pInfo->m_vPos;
			vDestPos.y = stoney;


			//CheckClockWise(m_vecNavMesh);
			float fLength = 5.f;
			float fTemp = 0.f;
			XMFLOAT3 vecTemp= XMFLOAT3(0.f, 0.f, 0.f);

			// 주변 가까운 점을 찾자 (네비메쉬는 모두 이어져야 한다)
			for (size_t i = 0; i < m_vecSaveMesh.size(); ++i)
			{
				XMVECTOR Temp =XMVector3Length(XMVectorSubtract(XMLoadFloat3(&vDestPos), XMLoadFloat3(&m_vecSaveMesh[i])));
				XMStoreFloat(&fTemp,XMVector3Length(Temp));
				if (fLength > fTemp)
				{
					fLength = fTemp;
					vecTemp = m_vecSaveMesh[i];
				}
			}
			if (fLength < 1.f) // 주변 점
			{
				vDestPos = vecTemp;
				m_iSamePoint++;
			}
			else
			cout << /*"점이겹처서 망했어!!"<<"\t"<<*/fTemp << endl;

			m_vecNavMesh[m_iCount++] = vDestPos;
			m_vecSaveMesh.push_back(vDestPos);

			if (m_iCount == 3)
			{
				m_iCount = 0;
				CheckClockWise(m_vecNavMesh);

				XMFLOAT4		Plane;
				Plane.x = 0;
				Plane.y = 0;
				Plane.z = 0;
				Plane.w = 0;

				Engine::Get_NaviMgr()->AddCell(&m_vecNavMesh[0], &m_vecNavMesh[1], &m_vecNavMesh[2], Plane);

				m_vecSaveOption.push_back(m_iOption);
				m_iSamePoint = 0;
				Engine::Get_NaviMgr()->CreateBuffer();
				Engine::Get_NaviMgr()->LinkCell();
			}
		}
	}

}

void CBackGround::DeleteVtx()
{
	Engine::Get_NaviMgr()->DeleteCell();

	for (int i = 0; i < 3; ++i)
	{
		if (m_vecSaveMesh.size() > 0)
			m_vecSaveMesh.pop_back();
	}
	if (m_vecSaveOption.size() > 0)
		m_vecSaveOption.pop_back();
}

void CBackGround::SetCubeAngle(float _x, float _y, float _z)
{
	//m_vecMesh
	//float fDist = 0.f;

	//int iDestIndex = -1;
	//float fDestDist = 99999.f;

	//if (m_vecMesh.size() != 0)
	//{
	//	vector<Engine::CStaticMesh*>::iterator	iter = m_vecMesh.end();
	//	iter--;

	//	for (int i = 0; i < iDestIndex; ++i)
	//	{
	//		++iter;
	//	}

	//	(*iter)->GetInfo()->m_fAngle[Engine::ANGLE_X] += 0.01f;
	//	(*iter)->SetAngleInfo(_x, _y, _z);
	//}
	MeshAngleX = XMConvertToRadians(_x);
	MeshAngleY = XMConvertToRadians(_y);
	MeshAngleZ = XMConvertToRadians(_z);
}

void CBackGround::SetAngleIni()
{
	MeshAngleX = 0.f;
	MeshAngleY = 0.f;
	MeshAngleZ = 0.f;
}

void CBackGround::CameraIni()
{
	(CDynamicCamera*)m_pDynamicCamera->InitCamera(&XMFLOAT3(0.f, 5.f, -15.f), &XMFLOAT3(0.f, 0.f, 0.f));

}

void CBackGround::LButtonDown()
{
	/*if (m_bTerrainModify)
	HeightMapY();*/
	if (m_bTileCreate)
		MeshAdd();//MeshAdd(); //ADDVtx();//MeshAdd();
				 //if (m_bTileModify)
				 //	CubeSelect();

}

void CBackGround::RButtonDown()
{
	/*if (m_bTerrainModify)
	HeightMapYY();*/
	if (m_bTileCreate)
		MeshDelete();//MeshDelete(); //DeleteVtx();//MeshDelete();
					/*if (m_bTileModify)
					CubeDeselect();*/

}

void CBackGround::LoadMap()
{
	/*HANDLE hFile = CreateFile(L"../../Map/RealMap7777.dat"
		, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);*/
	HANDLE hFile = CreateFile(L"../../Map/Stage3_9.dat"
		, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwByte;

	// 벡터 초기화
	vector<Engine::CStaticMesh*>::iterator	iter = m_vecMesh.begin();
	vector<Engine::CStaticMesh*>::iterator	iter_end = m_vecMesh.end();
	for (iter; iter != iter_end; ++iter)
	{
		(*iter)->Release();
		Safe_Delete(*iter);
	}
	m_vecMesh.clear();

	//ZeroMemory(szFullPath, (sizeof(TCHAR)*255)*20);

	while (true)
	{



		Engine::MYMESH tTemp;
		ReadFile(hFile, &tTemp, sizeof(Engine::MYMESH), &dwByte, NULL);

		if (dwByte == 0)
			break;

		Engine::CStaticMesh*		pBuffer;
		Engine::CComponent*		pComponent = NULL;

		//WORD a;

		//CopyMemory(&a, &tTemp.dwObjNum,sizeof(WORD));


		switch (tTemp.dwObjNum)
		{
	/*	case 0: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build1"); break;
		case 1: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build2"); break;
		case 2: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build3"); break;
		case 3: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build4"); break;
		case 4: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build5"); break;
		case 5: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build6"); break;
		case 6: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build7"); break;
		case 7: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build8"); break;
		case 8: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build9"); break;
		case 9: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build10"); break;
		case 10: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build11"); break;
		case 11: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build12"); break;
		case 12: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd13_1"); break;
		case 13: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd13_2"); break;
		case 14: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_1"); break;
		case 15: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_2"); break;
		case 16: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_3"); break;
		case 17: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_4"); break;
		case 18: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_5"); break;
		case 19: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_6"); break;
		case 20: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_7"); break;
		case 21: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_8"); break;
		case 22: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_9"); break;
		case 23: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_10"); break;
		case 24: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_11"); break;
		case 25: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_12"); break;
		case 26: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_13"); break;
		case 27: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_14"); break;
		case 28: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_15"); break;
		case 29: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_16"); break;
		case 30: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_17"); break;
		case 31: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_18"); break;
		case 32: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_19"); break;
		case 33: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_20"); break;
		case 34: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_21"); break;
		case 35: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_22"); break;
		case 36: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_23"); break;
		case 37: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_24"); break;
		case 38: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd14_25"); break;
		case 39: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_1"); break;
		case 40: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_2"); break;
		case 41: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_3"); break;
		case 42: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_4"); break;
		case 43: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_5"); break;
		case 44: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_6"); break;
		case 45: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_7"); break;
		case 46: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_8"); break;
		case 47: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_9"); break;
		case 48: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_10"); break;
		case 49: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_11"); break;
		case 50: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_12"); break;
		case 51: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_13"); break;
		case 52: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_14"); break;
		case 53: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_15"); break;
		case 54: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_16"); break;
		case 55: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_17"); break;
		case 56: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_18"); break;
		case 57: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_19"); break;
		case 58: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_20"); break;
		case 59: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_21"); break;
		case 60: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_22"); break;
		case 61: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_23"); break;
		case 62: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_24"); break;
		case 63: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_25"); break;
		case 64: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_26"); break;
		case 65: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_27"); break;
		case 66: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_28"); break;
		case 67: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_29"); break;
		case 68: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_30"); break;
		case 69: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_31"); break;
		case 70: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_32"); break;
		case 71: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_33"); break;
		case 72: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_34"); break;
		case 73: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_35"); break;
		case 74: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_36"); break;
		case 75: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_37"); break;
		case 76: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_38"); break;
		case 77: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_39"); break;
		case 78: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_40"); break;
		case 79: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_41"); break;
		case 80: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_42"); break;
		case 81: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_43"); break;
		case 82: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_44"); break;
		case 83: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_45"); break;
		case 84: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd15_46"); break;
		case 85: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_1"); break;
		case 86: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_2"); break;
		case 87: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_3"); break;
		case 88: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_4"); break;
		case 89: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_5"); break;
		case 90: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_6"); break;
		case 91: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_7"); break;
		case 92: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_8"); break;
		case 93: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_9"); break;
		case 94: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_10"); break;
		case 95: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_11"); break;
		case 96: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_12"); break;
		case 97: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_13"); break;
		case 98: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_14"); break;
		case 99: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_15"); break;
		case 100: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_16"); break;
		case 101: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_17"); break;
		case 102: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_18"); break;
		case 103: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_19"); break;
		case 104: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_20"); break;
		case 105: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_21"); break;
		case 106: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_22"); break;
		case 107: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_23"); break;
		case 108: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_24"); break;
		case 109: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_25"); break;
		case 110: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"buildd16_26"); break;
		case 111: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_1"); break;
		case 112: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_2"); break;
		case 113: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_3"); break;
		case 114: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_4"); break;
		case 115: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_5"); break;
		case 116: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_6"); break;
		case 117: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_7"); break;
		case 118: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_8"); break;
		case 119: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_9"); break;
		case 120: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_10"); break;
		case 121: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_11"); break;
		case 122: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_12"); break;
		case 123: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_13"); break;
		case 124: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_14"); break;
		case 125: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_15"); break;
		case 126: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_16"); break;
		case 127: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_17"); break;
		case 128: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_18"); break;
		case 129: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_19"); break;
		case 130: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_20"); break;
		case 131: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_21"); break;
		case 132: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Terrain_22"); break;

		case 133: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_1"); break;
		case 134: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_2"); break;
		case 135: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_3"); break;
		case 136: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_4"); break;
		case 137: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_5"); break;
		case 138: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_6"); break;
		case 139: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_7"); break;
		case 140: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_8"); break;
		case 141: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_9"); break;
		case 142: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_10"); break;
		case 143: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_11"); break;
		case 144: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_12"); break;
		case 145: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_13"); break;
		case 146: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_14"); break;
		case 147: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_15"); break;
		case 148: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_16"); break;
		case 149: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_17"); break;
		case 150: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_18"); break;
		case 151: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_19"); break;
		case 152: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_20"); break;
		case 153: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_21"); break;
		case 154: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_22"); break;
		case 155: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_23"); break;
		case 156: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_24"); break;
		case 157: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_25"); break;
		case 158: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_26"); break;
		case 159: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_27"); break;
		case 160: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_28"); break;
		case 161: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_29"); break;
		case 162: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_30"); break;
		case 163: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_31"); break;
		case 164: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_32"); break;
		case 165: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_33"); break;
		case 166: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_34"); break;
		case 167: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_35"); break;
		case 168: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_36"); break;
		case 169: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_37"); break;
		case 170: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_38"); break;
		case 171: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Last2_39"); break;
		default: continue;*/

case 0: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_1"); break;
case 1: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_2"); break;
case 2: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_3"); break;
case 3: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_4"); break;
case 4: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_5"); break;
case 5: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_6"); break;
case 6: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_7"); break;
case 7: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_8"); break;
case 8: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_9"); break;
case 9: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_10"); break;
case 10: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_11"); break;
case 11: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_12"); break;
case 12: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_13"); break;
case 13: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_14"); break;
case 14: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_15"); break;
case 15: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_16"); break;
case 16: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_17"); break;
case 17: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_18"); break;
case 18: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_19"); break;
case 19: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_20"); break;
case 20: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_21"); break;
case 21: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_22"); break;
case 22: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_23"); break;
case 23: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_24"); break;
case 24: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_25"); break;
case 25: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_26"); break;
case 26: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_27"); break;
case 27: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_28"); break;
case 28: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_29"); break;
case 29: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_30"); break;
case 30: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_31"); break;
case 31: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_32"); break;
case 32: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_33"); break;
case 33: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_34"); break;
case 34: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_35"); break;
case 35: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_36"); break;
case 36: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_37"); break;
case 37: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_38"); break;
case 38: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_39"); break;
case 39: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_40"); break;
case 40: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_41"); break;
case 41: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_42"); break;
case 42: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_43"); break;
case 43: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_44"); break;
case 44: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_45"); break;
case 45: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_46"); break;
case 46: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_47"); break;
case 47: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_48"); break;
case 48: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_49"); break;
case 49: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_50"); break;
case 50: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_51"); break;
case 51: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_52"); break;
case 52: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_53"); break;
case 53: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_54"); break;
case 54: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_55"); break;
case 55: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_56"); break;
//case 56: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_57"); break;
//case 57: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_58"); break;
case 58: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_59"); break;
case 59: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_60"); break;
case 60: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_61"); break;
case 61: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_62"); break;
case 62: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_63"); break;
case 63: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_64"); break;
case 64: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_65"); break;
case 65: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_66"); break;
case 66: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_67"); break;
case 67: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_68"); break;
case 68: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_69"); break;
case 69: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_70"); break;
case 70: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_71"); break;
case 71: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_72"); break;
case 72: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_73"); break;
case 73: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_74"); break;
case 74: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_75"); break;
case 75: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_76"); break;
case 76: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_77"); break;
case 77: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_78"); break;
case 78: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_79"); break;
case 79: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_80"); break;
case 80: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_81"); break;
case 81: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_82"); break;
case 82: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_83"); break;
case 83: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_84"); break;
case 84: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_85"); break;
case 85: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_86"); break;
case 86: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_87"); break;
case 87: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_88"); break;
case 88: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_89"); break;
case 89: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_90"); break;
case 90: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_91"); break;
case 91: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_92"); break;
case 92: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_93"); break;
case 93: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_94"); break;
case 94: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_95"); break;
case 95: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_96"); break;
case 96: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_97"); break;
case 97: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_98"); break;
case 98: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_99"); break;
case 99: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_100"); break;
case 100: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_101"); break;
case 101: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_102"); break;
case 102: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_103"); break;
case 103: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_104"); break;
case 104: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_105"); break;
case 105: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Stage2_106"); break;
default: continue;
		}

		//= Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, szFullPath[pBuffer->GetMeshNum()]);
		pBuffer = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		//NULL_CHECK(pBuffer);
		pBuffer->SetvPosInfo(tTemp.vPos);
		pBuffer->SetvSizeInfo(tTemp.vSize);
		pBuffer->SetAngleInfo(tTemp.fAngle[Engine::ANGLE_X]
							, tTemp.fAngle[Engine::ANGLE_Y]
							, tTemp.fAngle[Engine::ANGLE_Z]);



		//pBuffer->SetObjOption(tTemp.eOption);
		pBuffer->SetMeshNum(tTemp.dwObjNum);
		m_vecMesh.push_back(pBuffer);
	}
	CloseHandle(hFile);
}





void CBackGround::Reset(void)
{
	m_vecSaveMesh.clear();
	m_vecSaveOption.clear();

	while (1)
	{
		if (Engine::Get_NaviMgr()->DeleteCell() == FALSE)
			break;
	}

	m_iCount = 0;
}

HRESULT CBackGround::LoadNaviMesh()
{
	Reset();
	Engine::Get_NaviMgr()->ReserveCellSize(m_pMyGDI, 1000);
	//HRESULT		hr = NULL;
	//XMFLOAT3	vPoint[3];

	//XMFLOAT4 Plane;
	//Plane.x = 0;
	//Plane.y = 0;
	//Plane.z = 0;
	//Plane.w = 0;

	//vPoint[0] = XMFLOAT3(0.f, 0.5f, 5.f);
	//vPoint[1] = XMFLOAT3(5.f, 0.5f, 0.f);
	//vPoint[2] = XMFLOAT3(0.f, 0.5f, 0.f);

	//XMVECTOR vPos4 = XMPlaneFromPoints(XMLoadFloat3(&vPoint[0]), XMLoadFloat3(&vPoint[1]), XMLoadFloat3(&vPoint[2]));
	//Plane.x = XMVectorGetX(vPos4);
	//Plane.y = XMVectorGetY(vPos4);
	//Plane.z = XMVectorGetZ(vPos4);
	//Plane.w = XMVectorGetW(vPos4);

	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2], Plane);
	//FAILED_CHECK(hr);
	//Reset();

	//vPoint[0] = XMFLOAT3(0.f, 0.f, 2.f);
	//vPoint[1] = XMFLOAT3(2.f, 0.f, 2.f);
	//vPoint[2] = XMFLOAT3(2.f, 0.f, 0.f);
	//vPos4 = XMPlaneFromPoints(XMLoadFloat3(&vPoint[0]), XMLoadFloat3(&vPoint[1]), XMLoadFloat3(&vPoint[2]));
	//Plane.x = XMVectorGetX(vPos4);
	//Plane.y = XMVectorGetY(vPos4);
	//Plane.z = XMVectorGetZ(vPos4);
	//Plane.w = XMVectorGetW(vPos4);


	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2], Plane);
	//FAILED_CHECK(hr);

	//vPoint[0] = XMFLOAT3(0.f, 0.f, 4.f);
	//vPoint[1] = XMFLOAT3(2.f, 0.f, 2.f);
	//vPoint[2] = XMFLOAT3(0.f, 0.f, 2.f);
	//vPos4 = XMPlaneFromPoints(XMLoadFloat3(&vPoint[0]), XMLoadFloat3(&vPoint[1]), XMLoadFloat3(&vPoint[2]));
	//Plane.x = XMVectorGetX(vPos4);
	//Plane.y = XMVectorGetY(vPos4);
	//Plane.z = XMVectorGetZ(vPos4);
	//Plane.w = XMVectorGetW(vPos4);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2], Plane);
	//FAILED_CHECK(hr);

	//Engine::Get_NaviMgr()->CreateBuffer();
	//Engine::Get_NaviMgr()->LinkCell();
	return NOERROR;
}
void CBackGround::SaveNavMesh()
{
	// 맵 저장 버튼
	//CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);
	//if (Dlg.DoModal() == IDCANCEL)
	//	return;

	//Dlg.m_ofn.lpstrInitialDir = L"..\\Data";
	//HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//DWORD dwByte;

	//vector<Engine::CStaticMesh*>::iterator		iter = m_pBackGround->GetMeshVector()->begin();
	//vector<Engine::CStaticMesh*>::iterator		iter_end = m_pBackGround->GetMeshVector()->end();

	//for (iter; iter != iter_end; ++iter)
	//{
	//	{
	//		Engine::MYMESH tTemp;
	//		tTemp.vPos = (*iter)->GetInfo()->m_vPos;
	//		tTemp.vSize = (*iter)->GetInfo()->m_vScale;
	//		tTemp.fAngle[Engine::ANGLE_X] = (*iter)->GetInfo()->m_fAngle[Engine::ANGLE_X];
	//		tTemp.fAngle[Engine::ANGLE_Y] = (*iter)->GetInfo()->m_fAngle[Engine::ANGLE_Y];
	//		tTemp.fAngle[Engine::ANGLE_Z] = (*iter)->GetInfo()->m_fAngle[Engine::ANGLE_Z];
	//		tTemp.dwObjNum = (*iter)->GetMeshNum();
	//		WriteFile(hFile, &tTemp, sizeof(Engine::MYMESH), &dwByte, NULL);
	//	}
	//}
	//iter = m_pBackGround->GetMeshVector()->begin();

	//CloseHandle(hFile);
	////

	//time_t timer = time(NULL);
	//tm t;
	//localtime_s(&t, &timer);
	//// 현재 시간으로 저장
	//TCHAR szFileName[64];
	//wsprintf(szFileName, L"../../Map/RealNavi.dat", t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	//HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", NULL);
	if (Dlg.DoModal() == IDCANCEL)
		return;

	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";
	HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwByte;
	unsigned int i = 0;
	unsigned int j = 0;
	size_t iSize = m_vecSaveMesh.size();
	while (iSize % 3 != 0)
	{
		m_vecSaveMesh.pop_back();
		iSize--;
	}

	int iSaveSize = iSize / 3;

	WriteFile(hFile, &(iSaveSize), sizeof(int), &dwByte, NULL); // 전체 갯수

	while ((i + 2) < iSize)
	{
		Engine::NAVMESH NavMesh;

		if (m_vecSaveOption.size() > j)
			NavMesh.iOption = m_vecSaveOption[j++];
		NavMesh.vecNavMesh[0] = m_vecSaveMesh[i++];
		NavMesh.vecNavMesh[1] = m_vecSaveMesh[i++];
		NavMesh.vecNavMesh[2] = m_vecSaveMesh[i++];

		WriteFile(hFile, &NavMesh, sizeof(NavMesh), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CBackGround::LoadNavMesh()
{
	DWORD dwByte = 0;
	Reset();
	// 기존 네비메쉬 그대로 두고 이어서 불러오기
	//HANDLE hFile = CreateFile(L"../../Map/RealNavi2.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", NULL);
	if (Dlg.DoModal() == IDCANCEL)
		return;
	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";
	HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//DWORD dwByte;

	unsigned int iSize = 0;
	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);
	while (true)
	{
		Engine::NAVMESH* pNavMesh = new Engine::NAVMESH;

		ReadFile(hFile, pNavMesh, sizeof(Engine::NAVMESH), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(pNavMesh);
			break;
		}

		m_iOption = pNavMesh->iOption;
		CheckClockWise(pNavMesh->vecNavMesh);

		// 저장할 정보
		for (int i = 0; i<3; ++i)
			m_vecSaveMesh.push_back(pNavMesh->vecNavMesh[i]);
		m_vecSaveOption.push_back(m_iOption);

		//D3DXPLANE plane;

		XMFLOAT4 plane;
		plane.x = 0;
		plane.y = 0;
		plane.z = 0;
		plane.w = 0;

		// Cell 추가
		Engine::Get_NaviMgr()->AddCell(&pNavMesh->vecNavMesh[0], &pNavMesh->vecNavMesh[1], &pNavMesh->vecNavMesh[2], plane);
		Engine::Safe_Delete(pNavMesh);

		Engine::Get_NaviMgr()->CreateBuffer();
		Engine::Get_NaviMgr()->LinkCell();
	}
	Engine::Get_NaviMgr()->CreateBuffer();
	Engine::Get_NaviMgr()->LinkCell();

	CloseHandle(hFile);
}