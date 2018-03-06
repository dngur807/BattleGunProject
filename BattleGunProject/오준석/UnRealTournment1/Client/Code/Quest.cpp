#include "stdafx.h"
#include "Quest.h"
#include "Layer.h"
//#include "QuestBack.h"
#include "Export_Function.h"
#include "Include.h"
#include "SceneSelector.h"
#include "Loading.h"
#include "DynamicCamera.h"
#include "Eye1.h"
#include "Eye2.h"
#include "LogoBack.h"


#include "Mouse.h"






CQuest::CQuest(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pManagement(Engine::Get_Management())
{

}

CQuest::~CQuest(void)
{
	//Release();
}

HRESULT CQuest::InitScene(void)
{
	HRESULT		hr = NULL;

	/*hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_Quest, Engine::TEXTURE_NORMAL
		, L"Texture_Quest"
		, L"../bin/Resources/Texture/UI/20.tga", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);*/

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_QUEST
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest"
		, L"../bin/Resources/Texture/Quest/%d.jpg", 5);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_QUEST
		, Engine::TEXTURE_NORMAL
		, L"Texture_Quest1"
		, L"../bin/Resources/Texture/Quest1/%d.png", 11);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_QUEST, Engine::TEXTURE_NORMAL
		, L"Texture_Quest2"
		, L"../bin/Resources/Texture/Quest1/99.tga", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");
	return S_OK;
}

int CQuest::Update(void)
{
	CMouse::SetMousePos();
	Engine::CScene::Update();

	//if (m_pLoading->GetComplete())
	{
		//if (GetAsyncKeyState(VK_F1))
		{
		/*	HRESULT hr = m_pManagement->SceneChange(CSceneQuestor(CSceneQuestor::SCENE_STAGE));
			FAILED_CHECK_RETURN_MSG(hr, 0, L"씬 전환 실패");*/
		}
	}
	if (CMouse::GetMousePos().x >= 40 && CMouse::GetMousePos().x <= 160 &&
		CMouse::GetMousePos().y >= 130 && CMouse::GetMousePos().y <= 160)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			HRESULT hr = m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_STAGE));
			FAILED_CHECK_RETURN_MSG(hr, 0, L"씬 전환 실패");
			//cout << 1 << endl;
		}

	}

	return 0;
}

void CQuest::Render(void)
{
	Engine::CScene::Render();

	/*Engine::Get_FontMgr()->Render(L"바탕", D3DXVECTOR3(100.f, 500.f, 0.f)
		, m_pLoading->GetLoadMessage(), D3DCOLOR_ARGB(255, 255, 255, 0));

	Engine::Get_FontMgr()->Render(L"궁서", D3DXVECTOR3(0.f, 100.f, 0.f)
	, L"JusinGameAcademy", D3DCOLOR_ARGB(255, 0, 255, 0));

	Engine::Get_FontMgr()->Render(L"굴림", D3DXVECTOR3(0.f, 200.f, 0.f)
	, L"JusinGameAcademy", D3DCOLOR_ARGB(255, 255, 0, 255));*/
}

CQuest* CQuest::Create(Engine::MYGDI* pMyGDI)
{
	CQuest* pQuest = new CQuest(pMyGDI);
	if (FAILED(pQuest->InitScene()))
		Engine::Safe_Delete(pQuest);

	return pQuest;
}

HRESULT CQuest::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CQuest::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = NULL;

	/*pGameObject = CQuestBack::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CQuestBack 생성 실패");
	pLayer->AddObject(L"QuestBack", pGameObject);*/


	//pGameObject = CEye2::Create(m_pMyGDI);
	//NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CEye2 생성 실패");
	//pLayer->AddObject(L"CEye2", pGameObject);

	/*XMFLOAT3 pEye(0.0f, 0.0f, -1.0f);
	XMFLOAT3 pAt(0.0f, 0.0f, 0.0f);
	pGameObject = CDynamicCamera::Create(m_pMyGDI, &pEye, &pAt, NULL);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Camera 생성 실패");
	pLayer->AddObject(L"DynamicCamera", pGameObject);*/

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

void CQuest::Release(void)
{
	//Engine::Safe_Delete(m_pLoading);
}

HRESULT CQuest::Add_Light(void)
{
	/*D3DLIGHT9		Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(0.f, 0.f, 1.f);
	Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Attenuation0 = 1.f;
	 

	HRESULT hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &Light, 0);
	FAILED_CHECK(hr);*/
	return S_OK;
}
