#include "stdafx.h"
#include "SkyBox.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "BoxTex.h"
#include "WavesTex.h"
#include "CubeTex.h"

CSkyBox::CSkyBox(Engine::MYGDI * pMyGDI)
	:CLandObject(pMyGDI),
	m_pResourceMgr(Engine::Get_ResourceMgr()),
	m_pTexture(nullptr),
	m_iSelect(0),
	m_iTextureNum(0)
{
}

CSkyBox::~CSkyBox()
{
}

int CSkyBox::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CSkyBox::Render(void)
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


	
	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetWavesTech();
	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight1Tech();
	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight0Tech();
	XMFLOAT4X4 mTexTransform;
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mTexTransform, I);

	//

	//m_pMyGDI->pEffect->SetTexTransform(matWorld/*XMLoadFloat4x4(&m_pInfo->m_matWorld)*/);
	//m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	//m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	//

	

	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetCubeTech();
	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight0Tech();
	//m_pMyGDI->pEffect->SetDiffuseMap(*(m_pTexture->GetTextureView(0)));
	//이거때문에 2시간;+GetTIME();
	m_pMyGDI->pEffect->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
	//m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetWorld(XMLoadFloat4x4(&mTexTransform));
	//m_pMyGDI->pEffect->SetTexTransform(XMLoadFloat4x4(&mTexTransform));

	//XMMATRIX matWorld;
	//matWorld = XMMatrixIdentity();
	//m_pMyGDI->pEffect->SetTexTransform(matWorld);


	m_pTexture->Render(0, m_iTextureNum);
	m_pBuffer->Render(m_pInfo->m_matWorld);
}

HRESULT CSkyBox::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = XMFLOAT3(1005.f, 1005.f, 1005.f);
	//m_pInfo->m_vScale = XMFLOAT3(10.f, 10.f, 10.f);

	
	switch (m_iSelect)
	{
	case 0:
		m_iTextureNum = 0;
		m_pInfo->m_fAngle[Engine::ANGLE_Z] = XMConvertToRadians(180);
		m_pInfo->m_vPos = XMFLOAT3(0.f, 0.f, 500.f);
		break;
	case 1:
		m_iTextureNum = 0;
		m_pInfo->m_fAngle[Engine::ANGLE_Z] = XMConvertToRadians(180);
		m_pInfo->m_vPos = XMFLOAT3(0.f, 0.f, -500.f);
		break;
	case 2:
		m_iTextureNum = 5;
		m_pInfo->m_fAngle[Engine::ANGLE_X] = XMConvertToRadians(90);
		m_pInfo->m_vPos = XMFLOAT3(0.f, 500.f, 0.f);
		break;
	case 3:
		m_iTextureNum = 4;
		m_pInfo->m_fAngle[Engine::ANGLE_X] = XMConvertToRadians(90);
		m_pInfo->m_vPos = XMFLOAT3(0.f, -500.f, 0.f);
		break;
	case 4:
		m_iTextureNum = 0;
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(90);
		m_pInfo->m_vPos = XMFLOAT3(500.f, 0.f, 0.f);
		break;
	case 5:
		m_iTextureNum = 0;
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(90);
		m_pInfo->m_vPos = XMFLOAT3(-500.f, 0.f, 0.f);
		break;
	}


	//m_pBuffer->SetIdxInfo(m_pIndex, &m_iIndexCnt);

	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);

	//빛초기화!
	m_DirLight.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Direction = XMFLOAT3(1.f, -2.f, 1.f);
	return S_OK;
}

HRESULT CSkyBox::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	////Texture
	//Texture_Rain
	//Texture_SkyBox
	//Texture_SkyBoxT
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_SkyBoxT");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	//Buffer
	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_CubeTex");
	//m_pBuffer = dynamic_cast<Engine::CCubeTex*>(pComponent);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"BoxTex", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Buffer
	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_TerrainTex");
	//m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

CSkyBox * CSkyBox::Create(Engine::MYGDI * pMyGDI,int iSelect)
{

	CSkyBox* pLandObject = new CSkyBox(pMyGDI);
	pLandObject->m_iSelect = iSelect;

	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);



	return pLandObject;
}
