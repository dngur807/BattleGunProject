#include "stdafx.h"
#include "Ingame.h"
#include "Scene.h"
#include "GameMgr.h"
#include "Layer.h"
#include "GameObject.h"
#include "Gestalt.h"
#include "Export_Function.h"
#include "Loading.h"
#include "Stone.h"
#include "DynamicCamera.h"
#include "Transform.h"
#include "Player.h"
#include "Terrain.h"
#include "Export_System.h"
#include "CameraMgr.h"

CIngame::CIngame(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pLoading(NULL)
	, m_pFX(Engine::Get_Management()->GetFX())
{

}

CIngame::~CIngame()
{
}

HRESULT CIngame::InitScene()
{
	//if (g_bNetwork == false)
	//{
	//	//네트 워크 안켰을때 테스트 용
	//	CGameMgr::GetInstance()->GetGameInfo().m_eMapType = MAP_GESTALT;
	//	m_pLoading = CLoading::Create(m_pMyGDI, CLoading::LOADING_INGAME , false);
	//	m_pLoading->IngameLoading();
	//	NULL_CHECK_RETURN(m_pLoading, E_FAIL);
	//}



	XMMATRIX P = XMMatrixIdentity();

	m_pMyGDI->pEffect->SetWorld(P);
	m_pMyGDI->pEffect->SetView(P);
	m_pMyGDI->pEffect->SetProj(P);

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"Add_UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");

	return S_OK;
}

int CIngame::Update()
{
	CCameraMgr::GetInstance()->Update();
	Engine::CScene::Update();
	return 0;
}

void CIngame::Render()
{
	//Engine::CScene::Render();
}

CIngame* CIngame::Create(Engine::MYGDI* pMyGDI)
{
	CIngame* pIngame = new CIngame(pMyGDI);
	if (FAILED(pIngame->InitScene()))
		Engine::Safe_Delete(pIngame);
	return pIngame;
}

HRESULT CIngame::Add_Environment_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	if (CGameMgr::GetInstance()->GetGameInfo().m_eMapType == MAP_GESTALT)
	{
		pGameObject = CGestalt::Create(m_pMyGDI);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);

		pLayer->AddObject(L"GestaltStructure", pGameObject);
	}
	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIRONMENT, pLayer));

	return S_OK;
}

HRESULT CIngame::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CPlayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Stone was nullptr");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);

	pLayer->AddObject(L"Player", pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CIngame::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;


	//캐릭터 찾자!
	auto iter = m_mapLayer.find(LAYER_GAMELOGIC);
	const Engine::CComponent*	pTargetTransform = iter ->second->GetComponent(L"Player" , L"Transform");
	NULL_CHECK_RETURN(pTargetTransform, E_FAIL);
	CCameraMgr::GetInstance()->SetTargetInfo(dynamic_cast<const Engine::CTransform*>(pTargetTransform));
	CCameraMgr::GetInstance()->SetCamera(CAMERA_FPS);


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

void CIngame::Release()
{
	Engine::Safe_Delete(m_pLoading);
}

HRESULT CIngame::Add_Light()
{
	m_DirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Direction = XMFLOAT3(1.f, -2.f, 1.f);


	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	return S_OK;
}
