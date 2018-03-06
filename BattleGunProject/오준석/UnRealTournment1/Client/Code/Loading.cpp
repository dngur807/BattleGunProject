#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"
#include "Include.h"
#include <process.h>

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

HRESULT CLoading::LogoLoading(void)
{
	HRESULT hr = NULL;
	lstrcpy(m_szLoadingMessage, L"버퍼 로딩중...");
	//Buffer
	//hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::CVIBuffer::BUFFER_TERRAIN
	//	, L"Buffer_TerrainTex"
	//	, VTXCNTX, VTXCNTZ, VTXITV);
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_CUBETEX
		, L"Buffer_CubeTex");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	//hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::CVIBuffer::BUFFER_CUBETEX
	//	, L"Buffer_CubeSkyBoxTex");
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_BOX
		, L"Buffer_BoxTex");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::CVIBuffer::BUFFER_WAVES
		, L"Buffer_WavesTex");
	FAILED_CHECK_RETURN(hr, E_FAIL);


	/*hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_CUBE
		, L"Texture_Cube"
		, L"../bin/Resources/Texture/CubeTile/CubeTile_%d.dds", 103);
	FAILED_CHECK_RETURN(hr, E_FAIL);*/

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_CUBE
		, L"Texture_Water"
		, L"../bin/Resources/Texture/water/water%d.dds", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_CUBE
		, L"Texture_Rain"
		, L"../bin/Resources/Texture/Particle/flare%d.dds", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	//hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::TEXTURE_CUBE
	//	, L"Texture_SkyBox"
	//	, L"../bin/Resources/Texture/SkyBox/SkyBox%d.dds", 6);
	//FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_CUBE
		, L"Texture_UI"
		, L"../bin/Resources/Texture/UI/%d.tga", 27);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_CUBE
		, L"Texture_UI1"
		, L"../bin/Resources/Texture/UI1/%d.png", 21);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::TEXTURE_CUBE
		, L"flare2"
		, L"../bin/Resources/Texture/Particle/flare2.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	/*hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"DynamicMesh_BatMan"
		, L"../bin/Resources/Mesh/DynamicMesh/CityBatman/FBX/",
		L"BatmanBody.FBX");
	FAILED_CHECK_RETURN(hr, E_FAIL);*/


	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::MESH_DYNAMIC
	//	, L"DynamicMesh_banshee"
	//	, L"../bin/Resources/Mesh/DynamicMesh/em002/FBX/",
	//	L"banshee.x");
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::MESH_DYNAMIC
	//	, L"DynamicMesh_BatMan1"
	//	, L"../bin/Resources/Mesh/DynamicMesh/CityBatman/FBX/",
	//	L"BatmanHead.FBX");
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	//hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
	//	, RESOURCE_STAGE
	//	, Engine::MESH_STATIC
	//	, L"__LOD_Ace_A2_Mesh0"
	//	, L"../bin/Resources/Mesh/StaticMesh/bat/Texture2D/FBX/",
	//	L"__LOD_CityX_01_Mesh7.FBX");
	//FAILED_CHECK_RETURN(hr, E_FAIL);



	lstrcpy(m_szLoadingMessage, L"로딩완료...");

	cout << "로딩씐 완료" << endl;
 	m_bComplete = true;
	return S_OK;
}

HRESULT CLoading::SelectLoading(void)
{

	HRESULT hr = NULL;
	lstrcpy(m_szLoadingMessage, L"메쉬 로딩중...");

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pMyGDI
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"DynamicMesh_Marine"
		, L"../bin/Resources/Mesh/DynamicMesh/Marine/",
		L"Marine.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadingMessage, L"로딩완료...");

	cout << "셀렉씐 완료" << endl;
	m_bComplete = true;


	return S_OK;
}

HRESULT CLoading::StageLoading(void)
{
	return S_OK;
}

CLoading* CLoading::Create(Engine::MYGDI* pMyGDI, LOADINGID eLoadID)
{
	CLoading* pLoad = new CLoading(pMyGDI, eLoadID);
	if (FAILED(pLoad->InitLoading()))
		Engine::Safe_Delete(pLoad);

	return pLoad;
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
	case CLoading::LOADING_LOGO:
		pLoading->LogoLoading();
		break;

	case CLoading::LOADING_SELECT:
		pLoading->SelectLoading();
		break;

	case CLoading::LOADING_STAGE:
		pLoading->StageLoading();
		break;
	}

	//LeaveCriticalSection(&pLoading->m_CSKey);
	return 0;
}

