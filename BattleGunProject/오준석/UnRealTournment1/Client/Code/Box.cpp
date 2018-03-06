#include "stdafx.h"
#include "Box.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "BoxTex.h"

CBox::CBox(Engine::MYGDI * pMyGDI)
	:CLandObject(pMyGDI), 
	m_pResourceMgr(Engine::Get_ResourceMgr()), 
	m_pTexture(nullptr)
{
}

CBox::~CBox()
{
}

int CBox::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CBox::Render(void)
{
	D3D11_RASTERIZER_DESC RasterDesc;
	ZeroMemory(&RasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	//D3D11_FILL_SOLID
	//RasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	RasterDesc.FillMode = D3D11_FILL_SOLID;
	RasterDesc.CullMode = D3D11_CULL_BACK;
	RasterDesc.DepthClipEnable = true;
	ID3D11RasterizerState* pRsState;
	HR(m_pMyGDI->pDevice->CreateRasterizerState(&RasterDesc, &pRsState));
	m_pMyGDI->pContext->RSSetState(pRsState);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render(m_pInfo->m_matWorld);
}

HRESULT CBox::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = XMFLOAT3(0.f, 0.f, 0.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(30);

	return S_OK;
}

HRESULT CBox::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Box");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_BoxTex");
	m_pBuffer = dynamic_cast<Engine::CBoxTex*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"BoxTex", pComponent));

	return S_OK;
}

CBox * CBox::Create(Engine::MYGDI * pMyGDI)
{
	CBox* pLandObject = new CBox(pMyGDI);
	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);

	return pLandObject;
}
