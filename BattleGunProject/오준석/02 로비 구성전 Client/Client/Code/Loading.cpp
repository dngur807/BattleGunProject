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
		L"HumanMaleA.X");
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
	case CLoading::LOADING_STAGE:
		pLoading->StageLoading();
		break;

	case CLoading::LOADING_STAGE1:
		pLoading->StageLoading1();
		break;

	case CLoading::LOADING_STAGE2:
		pLoading->StageLoading2();
		break;
	}

	//LeaveCriticalSection(&pLoading->m_CSKey);
	return 0;
}

