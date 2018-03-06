#include "stdafx.h"
#include "Select.h"
#include "Layer.h"
//#include "SelectBack.h"
#include "Export_Function.h"
#include "Include.h"
#include "SceneSelector.h"
#include "Loading.h"
#include "DynamicCamera.h"
#include "Eye1.h"
#include "Eye2.h"
#include "LogoBack.h"
#include "SelectBack.h"

#include "Mouse.h"
#include "SelectMenu.h"

#include "SelectMenu1.h"
#include "SelectMenu2.h"
#include "SelectMenu3.h"
#include "SelectMenu4.h"
#include "SelectMenu5.h"
#include "SelectMenu6.h"
#include "SelectMenu7.h"
#include "SelectMenu8.h"
#include "SelectMenu9.h"
#include "SelectMenu10.h"



CSelect::CSelect(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pManagement(Engine::Get_Management())
{

}

CSelect::~CSelect(void)
{
	//Release();
}

HRESULT CSelect::InitScene(void)
{
	HRESULT		hr = NULL;

	m_pLoading = CLoading::Create(m_pMyGDI, CLoading::LOADING_SELECT);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);



	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_SELECT
		, Engine::TEXTURE_NORMAL
		, L"Texture_Select"
		, L"../bin/Resources/Texture/Select/%d.jpg", 5);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_SELECT
		, Engine::TEXTURE_NORMAL
		, L"Texture_Select1"
		, L"../bin/Resources/Texture/Select1/%d.png", 11);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_SELECT, Engine::TEXTURE_NORMAL
		, L"Texture_Select2"
		, L"../bin/Resources/Texture/Select1/99.tga", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");
	return S_OK;
}

int CSelect::Update(void)
{
	CMouse::SetMousePos();
	Engine::CScene::Update();

	//if (m_pLoading->GetComplete())
	{
		//if (GetAsyncKeyState(VK_F1))
		{
		/*	HRESULT hr = m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_STAGE));
			FAILED_CHECK_RETURN_MSG(hr, 0, L"씬 전환 실패");*/
		}
	}
	if (CMouse::GetMousePos().x >= 310 && CMouse::GetMousePos().x <= 480 &&
		CMouse::GetMousePos().y >= 180 && CMouse::GetMousePos().y <= 210)
	{
		if (GetAsyncKeyState(VK_LBUTTON)&& m_pLoading->GetComplete())
		{
			HRESULT hr = m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_STAGE));
			FAILED_CHECK_RETURN_MSG(hr, 0, L"씬 전환 실패");
		}

	}

	return 0;
}

void CSelect::Render(void)
{
	Engine::CScene::Render();

	/*Engine::Get_FontMgr()->Render(L"바탕", D3DXVECTOR3(100.f, 500.f, 0.f)
		, m_pLoading->GetLoadMessage(), D3DCOLOR_ARGB(255, 255, 255, 0));

	Engine::Get_FontMgr()->Render(L"궁서", D3DXVECTOR3(0.f, 100.f, 0.f)
	, L"JusinGameAcademy", D3DCOLOR_ARGB(255, 0, 255, 0));

	Engine::Get_FontMgr()->Render(L"굴림", D3DXVECTOR3(0.f, 200.f, 0.f)
	, L"JusinGameAcademy", D3DCOLOR_ARGB(255, 255, 0, 255));*/
}

CSelect* CSelect::Create(Engine::MYGDI* pMyGDI)
{
	CSelect* pSelect = new CSelect(pMyGDI);
	if (FAILED(pSelect->InitScene()))
		Engine::Safe_Delete(pSelect);

	return pSelect;
}

HRESULT CSelect::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CSelect::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = NULL;



	pGameObject = CSelectMenu::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu1::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu2::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu3::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu4::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu5::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu6::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu7::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu8::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu9::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);

	pGameObject = CSelectMenu10::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectMenu 생성 실패");
	pLayer->AddObject(L"CSelectMenu", pGameObject);


	pGameObject = CSelectBack::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSelectBack 생성 실패");
	pLayer->AddObject(L"SelectBack", pGameObject);

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

void CSelect::Release(void)
{
	//Engine::Safe_Delete(m_pLoading);
}

HRESULT CSelect::Add_Light(void)
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
