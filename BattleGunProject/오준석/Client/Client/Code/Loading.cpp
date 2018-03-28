#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"
#include "Include.h"
#include <process.h>
#include "GameMgr.h"

CLoading::CLoading(Engine::MYGDI* pMyGDI, LOADINGID eLoadID)
	: m_pMyGDI(pMyGDI)
	, m_eLoadID(eLoadID)
	, m_hThread(NULL)
	, m_bComplete(false)
{
	ZeroMemory(&m_CSKey, sizeof(CRITICAL_SECTION));
	ZeroMemory(m_szLoadingMessage, sizeof(TCHAR) * 256);
}

CLoading::~CLoading(void)
{
	Release();
}

HRESULT CLoading::InitLoading(void)
{
	//InitializeCriticalSection(&m_CSKey);
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);
	NULL_CHECK_RETURN(m_hThread, E_FAIL);
	return S_OK;
}

HRESULT CLoading::IngameLoading()
{
	HRESULT hr = NULL;
	lstrcpy(m_szLoadingMessage, L"버퍼 로딩중...");
	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_CUBETEX
		, L"Buffer_CubeTex");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_TERRAIN
		, L"Buffer_TerrainTex"
		, VTXCNTX, VTXCNTZ, VTXITV);
	FAILED_CHECK_RETURN(hr, E_FAIL);




	//hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::CVIBuffer::BUFFER_SPHERE
	//	, L"Buffer_Sphere"
	//	);
	//FAILED_CHECK_RETURN(hr, E_FAIL);


	lstrcpy(m_szLoadingMessage, L"텍스쳐 로딩중...");

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_TerrainTex"
		, L"../bin/Resources/Texture/Terrain/Terrain%d.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_CUBE
		, L"Texture_Cube"
		, L"../bin/Resources/Texture/CubeTile/CubeTile_%d.dds", 103);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	//hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::TEXTURE_NORMAL
	//	, L"Texture_Stone"
	//	, L"..\\bin\\Resources\\Texture\\Mesh\\StaticMesh\\TombStone\\PA_GRAVESTONE_03.tga");
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadingMessage, L"메쉬 로딩중...");

	switch (CGameMgr::GetInstance()->GetGameInfo().m_eMapType)
	{
	case MAP_GESTALT:
		GestaltLoading();
		break;
	case MAP_CONVOY:
		ConvoyLoading();
		break;
	default:
		break;
	} 

	// 테스트용
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"StaticMesh_Stone"
		, L"..\\bin\\Resources\\Texture\\Mesh\\StaticMesh\\TombStone\\"
		, L"TombStone.x");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	


	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"DynamicMesh_Sylva"
		, L"../bin/Resources/Mesh/DynamicMesh/HumanMaleA/",
		L"HumanMaleA.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	//1인칭 무기
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_FlakCannon1st"
		, L"../bin/Resources/Mesh/DynamicMesh/Weapon/1st/FlakCannon/"
		, L"FlakCannon.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"Mesh_AssultRifle1st"
		, L"../bin/Resources/Mesh/DynamicMesh/Weapon/1st/AssultRifle/"
		, L"AssultRifle.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	//3인칭
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_FlakCannon3rd"
		, L"../bin/Resources/Mesh/StaticMesh/Weapon3rd/"
		, L"FlakCannon.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"Mesh_AssultRifle3rd"
		, L"../bin/Resources/Mesh/StaticMesh/Weapon3rd/"
		, L"AssultRifle.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	

	lstrcpy(m_szLoadingMessage, L"로딩완료...");
#ifdef _CLIENTDEBUGING_
	cout << "로딩 완료" << endl;
#endif // _CLIENTDEBUGING_
	m_bComplete = true;
	return 0;
}

HRESULT CLoading::StageLoading(void)
{
	HRESULT hr = NULL;
	lstrcpy(m_szLoadingMessage, L"버퍼 로딩중...");
	//Buffer
	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_TERRAIN
		, L"Buffer_TerrainTex"
		, VTXCNTX, VTXCNTZ, VTXITV);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_CUBETEX
		, L"Buffer_CubeTex");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadingMessage, L"텍스쳐 로딩중...");
	//Texture
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_TerrainTex"
		, L"../bin/Resources/Texture/Terrain/Terrain%d.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_CUBE
		, L"Texture_Cube"
		, L"../bin/Resources/Texture/CubeTile/CubeTile_%d.dds", 103);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Stone"
		, L"..\\bin\\Resources\\Texture\\Mesh\\StaticMesh\\TombStone\\PA_GRAVESTONE_03.tga");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Texture_Bow"
		, L"..\\bin\\Resources\\Mesh\\StaticMesh\\Bow\\BOW_AUCHINDOUN_D_01.tga");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadingMessage, L"메쉬 로딩중...");
	//Mesh
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"StaticMesh_Stone"
		, L"..\\bin\\Resources\\Texture\\Mesh\\StaticMesh\\TombStone\\"
		, L"TombStone.x");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::MESH_STATIC
	//	, L"StaticMesh_Bow"
	//	, L"..\\bin\\Resources\\Mesh\\StaticMesh\\Bow\\", 
	//	L"Bow.X");
	//FAILED_CHECK_RETURN(hr, E_FAIL);
	
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"DynamicMesh_Sylva"
		, L"../bin/Resources/Mesh/DynamicMesh/HumanMaleA/",
		L"banshee.x");
	FAILED_CHECK_RETURN(hr, E_FAIL);



	lstrcpy(m_szLoadingMessage, L"로딩완료...");

	cout << "로딩 완료" << endl;
 	m_bComplete = true;
	return S_OK;
}

HRESULT CLoading::StageLoading1(void)
{
	return S_OK;
}

HRESULT CLoading::StageLoading2(void)
{
	return S_OK;
}

CLoading* CLoading::Create(Engine::MYGDI* pMyGDI, LOADINGID eLoadID , bool bUseThread)
{
	CLoading* pLoad = new CLoading(pMyGDI, eLoadID);
	if (bUseThread == true)// false면 
	{
		if (FAILED(pLoad->InitLoading()))
			Engine::Safe_Delete(pLoad);
	}
	return pLoad;
}

void CLoading::GestaltLoading()
{
	lstrcpy(m_szLoadingMessage, L"Gestalt Loading...");
	HRESULT hr;
	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
	//	, Engine::MESH_STATIC
	//	, L"Mesh_SkyBall"
	//	, L"../bin/Resources/Mesh/StaticMesh/Map/Moon/"
	//	, L"SkyBall.X");
	//FAILED_CHECK_RETURN(hr, );

	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
	//	, Engine::MESH_STATIC
	//	, L"Mesh_WarpTube"
	//	, L"../bin/Resources/Mesh/StaticMesh/WarpTube/"
	//	, L"WarpTube.X");
	//FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_P1"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P1.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_P2"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P2.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_P3"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P3.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_P4"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P4.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_P5"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P5.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_P6"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P6.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_P7"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P7.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_P8"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P8.X");
	FAILED_CHECK_RETURN(hr, );


	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_P9"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"P9.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_E1"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"E1.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_E2"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"E2.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_BlackBox"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"blackbox.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_SmallL"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"SmallL.X");
	FAILED_CHECK_RETURN(hr, );

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_Gestalt_MassL"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"MassL.X");
	FAILED_CHECK_RETURN(hr, );


	//게슈탈트 충돌용 메쉬
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI, RESOURCE_GESTALT
		, Engine::MESH_STATIC
		, L"Mesh_GestaltCol"
		, L"../bin/Resources/Mesh/StaticMesh/Map/Gestalt/"
		, L"GestaltCol.X");
	FAILED_CHECK_RETURN(hr, );

}

void CLoading::ConvoyLoading()
{

}

void CLoading::Release(void)
{
	//DeleteCriticalSection(&m_CSKey);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
}

UINT WINAPI CLoading::LoadingFunction(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	//EnterCriticalSection(&pLoading->m_CSKey);

	switch (pLoading->m_eLoadID)
	{
	case CLoading::LOADING_STAGE:
		pLoading->StageLoading();
		break;

	case CLoading::LOADING_STAGE1:
		pLoading->StageLoading1();
		break;

	case CLoading::LOADING_STAGE2:
		pLoading->StageLoading2();
		break;
	case CLoading::LOADING_INGAME:
		pLoading->IngameLoading();
		break;
	}

	//LeaveCriticalSection(&pLoading->m_CSKey);
	return 0;
}

