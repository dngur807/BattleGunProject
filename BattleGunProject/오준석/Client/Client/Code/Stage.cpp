#include "stdafx.h"
#include "Stage.h"

#include "Export_Function.h"
#include "Layer.h"
#include "Terrain.h"
#include "DynamicCamera.h"
#include "Stone.h"
#include "Player.h"
#include "Cube.h"
#include "ObjectCol.h"
#include "GameObject.h"

CStage::CStage(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pTargetPlayer(NULL)
{
}

CStage::~CStage(void)
{
	Release();

}

HRESULT CStage::InitScene(void)
{
	HRESULT		hr = NULL;

	FAILED_CHECK_RETURN(hr, E_FAIL);
	FAILED_CHECK_RETURN_MSG(Add_ETC(), E_FAIL, L"ETC Add Failed");
	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"Add_UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");
//	FAILED_CHECK_RETURN_MSG(Add_NaviMesh(), E_FAIL, L"Add_NaviMesh Add false");

	//! 옵저버 Notify
	//Engine::Get_InfoSubject()->Notify(MSG_INFOLIST);
	//Engine::Get_InfoSubject()->Notify(MSG_PLAYERINFO);

	//hr = Engine::Get_Frustum()->InitFrustum(m_pMyGDI);
	FAILED_CHECK(hr);
	return S_OK;
}

int CStage::Update(void)
{
	//Engine::Get_Frustum()->Transform_Frustum_ToWorld();
	Engine::CScene::Update();

	m_pObjectCol->Update();
	
	return 0;
}

void CStage::Render(void)
{
	Engine::CScene::Render();
	//Engine::Get_NaviMgr()->Render_NaviMesh();
}

HRESULT CStage::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CStage::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CTerrain::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CTerrain 생성 실패");
	pLayer->AddObject(L"Terrain", pGameObject);

	//// CubeTex
	//pGameObject = CCube::Create(m_pMyGDI);
	//NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Cube was nullptr");
	//pLayer->AddObject(L"Cube", pGameObject);

	//// Stone
	pGameObject = CStone::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Stone was nullptr");
	pLayer->AddObject(L"Stone", pGameObject);
	/*for (int i = 0; i < 30; ++i)
	{
		pGameObject = CStone::Create(m_pMyGDI);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Stone was nullptr");
		pLayer->AddObject(L"Stone", pGameObject);
	}*/
	pGameObject = CPlayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Stone was nullptr");
	pLayer->AddObject(L"Player", pGameObject);


	m_pTargetPlayer = pGameObject->GetComponent(L"Transform");

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CStage::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	XMFLOAT3 pEye(0.0f, 5.0f, -20.0f);
	XMFLOAT3 pAt(0.0f, 0.0f, 0.0f);
	pGameObject = CDynamicCamera::Create(m_pMyGDI, &pEye, &pAt);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Camera 생성 실패");
	pLayer->AddObject(L"DynamicCamera", pGameObject);

	//pGameObject = CStaticCamera::Create(m_pDevice, );
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->AddObject(L"StaticCamera", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

void CStage::Release(void)
{
	Engine::Safe_Delete(m_pObjectCol);
	Engine::Get_InfoSubject()->DestroyInstance();

	Engine::Get_Frustum()->DestroyInstance();
	Engine::Get_QuadTree()->DestroyInstance();
	Engine::Get_NaviMgr()->DestroyInstance();
}

CStage* CStage::Create(Engine::MYGDI* pMyGDI)
{
	CStage* pStage = new CStage(pMyGDI);

	if (FAILED(pStage->InitScene()))
	{
		Engine::Safe_Delete(pStage);
	}

	return pStage;
}

HRESULT CStage::Add_Light(void)
{
	/*HRESULT hr = NULL;
	D3DLIGHT9		Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(1.f, -1.f, 1.f);
	Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Attenuation0 = 1.f;

	hr = Engine::Get_LightMgr()->AddLight(m_pMyGDI, &Light, 0);
	FAILED_CHECK(hr);*/

	m_DirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Direction = XMFLOAT3(1.f, -2.f, 1.f);

	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);

	return S_OK;
}

HRESULT CStage::Add_NaviMesh(void)
{
	//Engine::Get_NaviMgr()->ReserveCellSize(m_pMyGDI, 4);
	//HRESULT		hr = NULL;
	//XMFLOAT3	vPoint[3];

	//vPoint[0] = XMFLOAT3(0.f, 0.f, 2.f);
	//vPoint[1] = XMFLOAT3(2.f, 0.f, 0.f);
	//vPoint[2] = XMFLOAT3(0.f, 0.f, 0.f);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	//FAILED_CHECK(hr);

	//vPoint[0] = XMFLOAT3(0.f, 0.f, 2.f);
	//vPoint[1] = XMFLOAT3(2.f, 0.f, 2.f);
	//vPoint[2] = XMFLOAT3(2.f, 0.f, 0.f);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	//FAILED_CHECK(hr);

	//vPoint[0] = XMFLOAT3(0.f, 0.f, 4.f);
	//vPoint[1] = XMFLOAT3(2.f, 0.f, 2.f);
	//vPoint[2] = XMFLOAT3(0.f, 0.f, 2.f);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	//FAILED_CHECK(hr);

	//vPoint[0] = XMFLOAT3(2.f, 0.f, 2.f);
	//vPoint[1] = XMFLOAT3(4.f, 0.f, 0.f);
	//vPoint[2] = XMFLOAT3(2.f, 0.f, 0.f);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	//FAILED_CHECK(hr);
	//// 06 24 04
	//vPoint[0] = XMFLOAT3(0.f, 0.f, 6.f);
	//vPoint[1] = XMFLOAT3(2.f, 0.f, 4.f);
	//vPoint[2] = XMFLOAT3(0.f, 0.f, 4.f);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	//FAILED_CHECK(hr);
	//// 04 24 22
	//vPoint[0] = XMFLOAT3(0.f, 0.f, 4.f);
	//vPoint[1] = XMFLOAT3(2.f, 0.f, 4.f);
	//vPoint[2] = XMFLOAT3(2.f, 0.f, 2.f);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	//FAILED_CHECK(hr);
	//// 24 42 22
	//vPoint[0] = XMFLOAT3(2.f, 0.f, 4.f);
	//vPoint[1] = XMFLOAT3(4.f, 0.f, 2.f);
	//vPoint[2] = XMFLOAT3(2.f, 0.f, 2.f);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	//FAILED_CHECK(hr);
	//// 22 42 40
	//vPoint[0] = XMFLOAT3(2.f, 0.f, 2.f);
	//vPoint[1] = XMFLOAT3(4.f, 0.f, 2.f);
	//vPoint[2] = XMFLOAT3(4.f, 0.f, 0.f);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	//FAILED_CHECK(hr);
	//// 42 06 24
	//vPoint[0] = XMFLOAT3(4.f, 0.f, 2.f);
	//vPoint[1] = XMFLOAT3(6.f, 0.f, 0.f);
	//vPoint[2] = XMFLOAT3(4.f, 0.f, 0.f);
	//hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	//FAILED_CHECK(hr);

	//Engine::Get_NaviMgr()->CreateBuffer();
	//Engine::Get_NaviMgr()->FindNeighbor();

	return S_OK;
}

HRESULT CStage::Add_ETC(void)
{
	Engine::CComponent* pComponent = nullptr;

	//ObjectCol 은 Stage에서 실행
	m_pObjectCol = CObjectCol::Create(m_pMyGDI);

	return S_OK;
}
