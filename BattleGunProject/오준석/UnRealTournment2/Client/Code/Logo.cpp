#include "stdafx.h"
#include "Logo.h"
#include "Layer.h"
#include "LogoBack.h"
#include "LogoLetter.h"
#include "Export_Function.h"
#include "Include.h"
#include "SceneSelector.h"
#include "Loading.h"
//#include "DynamicCamera.h"


CLogo::CLogo(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pManagement(Engine::Get_Management())
{

}

CLogo::~CLogo(void)
{
	Release();
}

HRESULT CLogo::InitScene(void)
{
	HRESULT		hr = NULL;

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pMyGDI
		, RESOURCE_LOGO
		, Engine::TEXTURE_NORMAL
		, L"Texture_Logo"
		, L"../bin/Resources/Texture/Logo/%d.jpg", 6);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = m_pResourceMgr->AddTexture(m_pMyGDI, RESOURCE_LOGO, Engine::TEXTURE_NORMAL
	, L"Texture_Logo1"
	, L"../bin/Resources/Texture/Logo/6.png", 1);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	m_pLoading = CLoading::Create(m_pMyGDI, CLoading::LOADING_LOGO);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");
	return S_OK;
}

int CLogo::Update(void)
{
	Engine::CScene::Update();

	if (m_pLoading->GetComplete())
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			HRESULT hr = m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_SELECT));
			FAILED_CHECK_RETURN_MSG(hr, 0 , L"¾À ÀüÈ¯ ½ÇÆÐ");
		}
	}

	return 0;
}

void CLogo::Render(void)
{
	Engine::CScene::Render();

	/*Engine::Get_FontMgr()->Render(L"¹ÙÅÁ", D3DXVECTOR3(100.f, 500.f, 0.f)
		, m_pLoading->GetLoadMessage(), D3DCOLOR_ARGB(255, 255, 255, 0));

	Engine::Get_FontMgr()->Render(L"±Ã¼­", D3DXVECTOR3(0.f, 100.f, 0.f)
	, L"JusinGameAcademy", D3DCOLOR_ARGB(255, 0, 255, 0));

	Engine::Get_FontMgr()->Render(L"±¼¸²", D3DXVECTOR3(0.f, 200.f, 0.f)
	, L"JusinGameAcademy", D3DCOLOR_ARGB(255, 255, 0, 255));*/
}

CLogo* CLogo::Create(Engine::MYGDI* pMyGDI)
{
	CLogo* pLogo = new CLogo(pMyGDI);
	if (FAILED(pLogo->InitScene()))
		Engine::Safe_Delete(pLogo);

	return pLogo;
}

HRESULT CLogo::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CLogo::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CLogoLetter::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CLogoBack »ý¼º ½ÇÆÐ");
	pLayer->AddObject(L"LogoBack", pGameObject);
	pGameObject = CLogoBack::Create(m_pMyGDI);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CLogoBack »ý¼º ½ÇÆÐ");
	pLayer->AddObject(L"LogoBack", pGameObject);

	


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

void CLogo::Release(void)
{
	Engine::Safe_Delete(m_pLoading);
}

HRESULT CLogo::Add_Light(void)
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
