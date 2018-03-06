#include "stdafx.h"
#include "Cube.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "CubeTex.h"
#include "BoxTex.h"
#include "WavesTex.h"

CCube::CCube(Engine::MYGDI * pMyGDI)
	:CLandObject(pMyGDI), 
	m_pResourceMgr(Engine::Get_ResourceMgr()), 
	m_pTexture(nullptr)
{
}

CCube::~CCube()
{
}

int CCube::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CCube::Render(void)
{
	D3D11_RASTERIZER_DESC RasterDesc;
	ZeroMemory(&RasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	//D3D11_FILL_SOLID
	//RasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	RasterDesc.FillMode = D3D11_FILL_SOLID;
	RasterDesc.CullMode = D3D11_CULL_NONE;
	RasterDesc.DepthClipEnable = true;
	ID3D11RasterizerState* pRsState;
	HR(m_pMyGDI->pDevice->CreateRasterizerState(&RasterDesc, &pRsState));
	m_pMyGDI->pContext->RSSetState(pRsState);
	m_pTexture->Render(0, 0);


	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetBoxTech();
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);

	//XMMATRIX matWorld;
	//matWorld = XMMatrixIdentity();
	//m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pBuffer->Render(m_pInfo->m_matWorld);
}

HRESULT CCube::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = XMFLOAT3(1.f, 1.f, 1.f);
	m_pInfo->m_vPos = XMFLOAT3(0.f, 0.f, 0.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(30);

	//m_pBuffer->SetIdxInfo(m_pIndex, &m_iIndexCnt);



	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);



	//m_BoundingBox = m_pMesh->GetBoundingBox();

	m_pInfo->m_vCenter = XMFLOAT3(1.f, 1.f, 1.f);
	m_pInfo->m_vExt = XMFLOAT3(1.f, 1.f, 1.f);

	Engine::Get_InfoSubject()->AddData(MSG_INFOLIST, m_pInfo);

	return S_OK;
}

HRESULT CCube::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Cube");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_BoxTex");
	m_pBuffer = dynamic_cast<Engine::CBoxTex*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"BoxTex", pComponent));

	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_CubeTex");
	//m_pBuffer = dynamic_cast<Engine::CCubeTex*>(pComponent);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"CubeTex", pComponent));

	return S_OK;
}

CCube * CCube::Create(Engine::MYGDI * pMyGDI)
{
	CCube* pLandObject = new CCube(pMyGDI);
	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);

	return pLandObject;
}
