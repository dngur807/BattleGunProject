#include "stdafx.h"
#include "RealTerrain.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "TerrainTex.h"
#include "StaticMesh.h"

CRealTerrain::CRealTerrain(Engine::MYGDI* pMyGDI)
	: Engine::CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_pRealTerrainVtx(NULL)
	, m_pIndex(NULL)
	, m_iIndexCnt(0)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CRealTerrain::~CRealTerrain(void)
{
	Release();
}

HRESULT CRealTerrain::Initialize(void)
{
	AddMapMesh();
	HRESULT		hr = AddComponent();
	FAILED_CHECK(hr);

	m_tMtrl.Ambient = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(0.f, 0.f, 0.f, 1.f);


	//m_pRealTerrainVtx = new Engine::VTXTEX[VTXCNTX * VTXCNTZ];
	//m_pBuffer->GetVtxInfo(m_pRealTerrainVtx);

	//m_pRealTerrainVtx = dynamic_cast<Engine::CRealTerrainTex*>(m_pBuffer)->GetVertex();
	//m_pIndex = new UINT[(VTXCNTX - 1) * (VTXCNTZ - 1) * 6];

	//hr = Engine::Get_QuadTree()->InitQuadTree(m_pRealTerrainVtx, VTXCNTX, VTXCNTZ);
	//FAILED_CHECK(hr);

	//hr = Engine::Get_QuadTree()->InitNeighbor(m_pRealTerrainVtx, Engine::Get_QuadTree(),
	//	VTXCNTX, VTXCNTZ);
	//FAILED_CHECK(hr);



	////
	MapMeshLoad();



	
	
	return S_OK;
}

void CRealTerrain::Release(void)
{
	Engine::Safe_Delete_Array(m_pIndex);
}

void CRealTerrain::MapMeshLoad()
{
	HANDLE hFile = CreateFile(L"../../Map/RealMap8.dat"
		, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwByte;
	// 벡터 초기화
	vector<Engine::CStaticMesh*>::iterator	iter = m_VecMeshMap.begin();
	vector<Engine::CStaticMesh*>::iterator	iter_end = m_VecMeshMap.end();
	for (iter; iter != iter_end; ++iter)
	{
		(*iter)->Release();
		Safe_Delete(*iter);
	}
	m_VecMeshMap.clear();

	while (true)
	{
		Engine::MYMESH tTemp;
		ReadFile(hFile, &tTemp, sizeof(Engine::MYMESH), &dwByte, NULL);

		if (dwByte == 0)
			break;

		//Engine::CStaticMesh*		pBuffer;
		//Engine::CComponent*		pComponent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"0");
		Engine::CStaticMesh*		pBuffer;
		Engine::CComponent*		pComponent = NULL;

		switch (tTemp.dwObjNum)
		{
		case 0: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"build1"); break;
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
		case 111: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_1"); break;
		case 112: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_2"); break;
		case 113: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_3"); break;
		case 114: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_4"); break;
		case 115: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_5"); break;
		case 116: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_6"); break;
		case 117: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_7"); break;
		case 118: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_8"); break;
		case 119: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_9"); break;
		case 120: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_10"); break;
		case 121: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_11"); break;
		case 122: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_12"); break;
		case 123: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_13"); break;
		case 124: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_14"); break;
		case 125: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_15"); break;
		case 126: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_16"); break;
		case 127: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_17"); break;
		case 128: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_18"); break;
		case 129: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_19"); break;
		case 130: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_20"); break;
		case 131: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_21"); break;
		case 132: pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"RealTerrain_22"); break;
		}
		pBuffer = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		NULL_CHECK(pBuffer);
		pBuffer->SetvPosInfo(tTemp.vPos);
		/*tTemp.vSize.x *= 100.f;
		tTemp.vSize.y *= 100.f;
		tTemp.vSize.z *= 100.f;*/
		pBuffer->SetvSizeInfo(tTemp.vSize);
		pBuffer->SetAngleInfo(tTemp.fAngle[Engine::ANGLE_X]
			, tTemp.fAngle[Engine::ANGLE_Y], tTemp.fAngle[Engine::ANGLE_Z]);
		//pBuffer->SetObjOption(tTemp.eOption);
		//pBuffer->SetTextureNumber(tTemp.dwTextureNumber);
		m_VecMeshMap.push_back(pBuffer);

		//cout<<tTemp.vSize.y<<endl;  //사이즈가다르네!
	}
	CloseHandle(hFile);
}

HRESULT CRealTerrain::AddMapMesh()
{
	HRESULT hr = NULL;
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build1/FBX/", L"__LOD_Ace_A2_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build2/FBX/", L"__LOD_Ace_A2_Mesh45.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build3/FBX/", L"__LOD_Ace_A2_Mesh5.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build4/FBX/", L"__LOD_CityY_19_Mesh92.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build5/FBX/", L"__LOD_CityZ_00_Mesh11.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build6/FBX/", L"__LOD_CityZ_12_Mesh15.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build7/FBX/", L"__LOD_CityY_19_Mesh92.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build8/FBX/", L"__LOD_Stagg_A2_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build9/FBX/", L"__LOD_CityZ_00_Mesh30.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build10/FBX/", L"__LOD_CityZ_01_Mesh50.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build11/FBX/", L"__LOD_CityZ_02_Mesh44.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"build12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/build12/FBX/", L"__LOD_CityZ_10_Mesh109.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd13_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd13/FBX/", L"__LOD_CityX_01_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd13_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd13/FBX/", L"__LOD_CityX_01_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh1.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh4.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh6.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_02_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh5.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh10.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh13.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh20.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_13", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_03_Mesh21.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_14", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh4.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_15", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_16", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh11.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_17", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh13.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_18", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh18.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_19", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh22.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_20", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh39.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_21", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh70.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_22", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"__LOD_CityX_04_Mesh72.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_23", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"Debris_WoodenBoat_Covered.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_24", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"Lockup_Floor_512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd14_25", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd14/FBX/", L"Vehicle_Boat_Generic.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_01_Mesh6.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_04_Mesh71.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh0.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh3.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh5.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh6.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh7.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh10.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh12.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh14.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh15.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_13", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh16.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_14", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh17.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_15", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh19.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_16", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh21.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_17", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh26.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_18", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh28.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_19", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh30.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_20", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh33.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_21", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh36.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_22", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh37.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_23", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh38.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_24", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh39.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_25", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh40.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_26", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh43.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_27", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh44.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_28", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh45.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_29", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh46.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_30", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh48.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_31", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh51.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_32", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh52.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_33", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh54.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_34", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh61.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_35", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh63.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_36", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh64.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_37", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"__LOD_CityY_08_Mesh67.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_38", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Bank_Lift.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_39", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"City_Shopfront_Steel_Shutter512_double.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_40", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Film_Floor_EdgeGrate_Pillar.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_41", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"GamePlayOBJ_ReWire_Panel.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_42", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Lockup_Cells_Wall_512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_43", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Lockup_Floor_512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_44", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"Lockup_Floor_Carpet_Clean_256x512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_45", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"SIGNInternationalPlaza.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd15_46", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd15/FBX/", L"TANK_Lite_Tank_Base.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Warehouse_Floor_02_Vent.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"ARCH_Sewer_Pipe_WallA_Loops_512.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"CAT.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"DownTown_01.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Objects_SpeakersBalloons.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_ArkhamIsland.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_Building03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_BuildingBlock02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_BuildingBlock03b.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_BuildingBlock04.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_lighthouse.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_LODS_01.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_13", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_LODS_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_14", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_LODS_03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_15", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Gotham_Skyline_LODS_04.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_16", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"GS_Island_Hangbridge.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_17", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"GS_Island_Stonebridge.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_18", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"OBJ_Boat_Barge.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_19", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Security_Hut_Bridger.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_20", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Security_Hut_C4.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_21", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Tyger_Helicopter.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_22", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Underworld_Tower_LOD_Mid.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_23", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"UW_OBJ_Sewer.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_24", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Warehouse_Floor_01.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_25", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Warehouse_Floor_01_Vent.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"buildd16_26", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/buildd16/FBX/", L"Warehouse_Floor_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//111개
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_1", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_BSP_B1_Main.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_2", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_BSP_B1_Main_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_3", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_BSP_B1_Main_05.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_4", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_BSP_B1_SwimmingPool.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_5", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Road_C2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_6", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Road_C5.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_7", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Rubble_StreetDetritus.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_8", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Skyline_CityFar.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_9", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Skyline_LODS_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_10", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Skyline_LODS_03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_11", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Skyline_LODS_04.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_12", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Skyline_Streets.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_13", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Skyline_WonderBuilding1.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_14", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Skyline_WonderFoundation.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_15", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"Gotham_Snow_768.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_16", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"GS_Island_02.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_17", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"GS_Island_03.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_18", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"GS_Island_04.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_19", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"OBJ_Boat_Barge.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_20", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"OW_River_RealTerrain.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_21", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"OW_River_RealTerrain2.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_STAGE, Engine::MESH_STATIC, L"RealTerrain_22", L"../../Client/bin/Resources/Mesh/StaticMesh/bat/RealTerrain/FBX/", L"PDLC_OW_C_Road_C1.FBX"); FAILED_CHECK_RETURN(hr, E_FAIL);
	//133개
	return NO_ERROR;

}

int CRealTerrain::Update(void)
{
	//Engine::Get_Frustum()->Transform_Frustum_Local(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//Engine::Get_QuadTree()->IsinFrustum(m_pRealTerrainVtx);
	//Engine::Get_QuadTree()->Culling_LOD(m_pRealTerrainVtx, m_pIndex, &(m_iIndexCnt = 0));
	//Engine::Get_QuadTree()->CullingToQuadTree(m_pRealTerrainVtx, m_pIndex, &(m_iIndexCnt = 0));
	//Engine::Get_Frustum()->CullingForRealTerrain(m_pIndex, &(m_iIndexCnt = 0), m_pRealTerrainVtx
	//, VTXCNTX, VTXCNTZ);
	Engine::CGameObject::Update();

	return 0;
}

void CRealTerrain::Render(void)
{
	//와이어 프레임
	D3D11_RASTERIZER_DESC dxRasterDesc;
	::memset(&dxRasterDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	//dxRasterDesc.FillMode = D3D11_FILL_WIREFRAME;       //와이어
	dxRasterDesc.FillMode = D3D11_FILL_SOLID;          //아닌거
	dxRasterDesc.CullMode = D3D11_CULL_NONE;
	dxRasterDesc.DepthClipEnable = TRUE;

	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);


	// 맵 매쉬 출력

	vector<Engine::CStaticMesh*>::iterator	iter1 = m_VecMeshMap.begin();
	vector<Engine::CStaticMesh*>::iterator	iter_end1 = m_VecMeshMap.end();
	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight1Tech();
	for (iter1; iter1 != iter_end1; ++iter1)
	{

		//m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
		(*iter1)->RenderMesh(XMLoadFloat4x4(&(*iter1)->GetInfo()->m_matWorld));
	}
	//m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pBuffer->SetIdxInfo(m_pIndex, &m_iIndexCnt);



	//XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);

	//m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	//m_pMyGDI->pEffect->SetTexTransform(XMMatrixIdentity());

	//m_pTexture->Render(0, 0);
	//m_pBuffer->Render(m_pInfo->m_matWorld);
	
}

HRESULT CRealTerrain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	/*pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));*/

	//Buffer
	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_RealTerrainTex");
	//m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	////Texture
	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_RealTerrainTex");
	//m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CRealTerrain* CRealTerrain::Create(Engine::MYGDI* pMyGDI)
{
	CRealTerrain*		pGameObject = new CRealTerrain(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}




