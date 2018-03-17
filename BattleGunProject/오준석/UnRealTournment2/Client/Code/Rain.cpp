#include "stdafx.h"
#include "Rain.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "WavesTex.h"
#include "BoxTex.h"
#include "VIBuffer.h"

CRain::CRain(Engine::MYGDI * pMyGDI)
	:CLandObject(pMyGDI),
	m_pResourceMgr(Engine::Get_ResourceMgr()),
	m_pTexture(nullptr),
	fTime(0.f)
{
	mRain = new Engine::CWavesTex(pMyGDI);
	mRain=mRain->Create(pMyGDI);
	//mRain->mRain.Init(20, 20, 0.8f, 0.03f, 3.25f, 0.4f);


}

CRain::~CRain()
{
	delete mRain;
}

int CRain::Update(void)
{
	Engine::CGameObject::Update();

	fTime += Engine::Get_TimeMgr()->DeltaTime();

	//mRain->RainUpdate(Engine::Get_TimeMgr()->DeltaTime(), fTime);

	(*mRain).mRain.Update(Engine::Get_TimeMgr()->DeltaTime(), fTime);


	return 0;
}

void CRain::Render(void)
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

	/*m_pMyGDI->pContext->RSSetState(pRsState);
	m_pMyGDI->pContext->OMSetDepthStencilState(0, 0);*/

	//m_pMyGDI->pContext->OMSetBlendState(0, blendFactor, 0xffffffff);



	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetBoxTech();
	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight2Tech();
	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetRainTech();

	//XMMATRIX matWorld;
	//matWorld = XMMatrixIdentity();
	//m_pMyGDI->pEffect->SetWorld(matWorld);

	//m_pMyGDI->pEffect->SetDirLights(&mDirLight);

	//m_pMyGDI->pEffect->SetMaterial1(m_tMtrl);
	//m_pMyGDI->pEffect->SetEyePosW(XMFLOAT3(0, -1, 0));


	//m_pTexture->Render(0, 0);
	//mRain->DrawWaves(m_pInfo->m_matWorld, Engine::Get_TimeMgr()->DeltaTime(), m_pTexture->GetTextureView(0));
	mRain->DrawRain(m_pInfo->m_matWorld, fTime, m_pTexture->GetTextureView(0));
	//(*mRain).mRain.Draw(0, fTime, m_pTexture->GetTextureView(0));

}

HRESULT CRain::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = XMFLOAT3(1.f, 1.f, 1.f);
	m_pInfo->m_vPos = XMFLOAT3(0.f, 2.f,0.f);
	//m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(155);

	//m_pBuffer->SetIdxInfo(m_pIndex, &m_iIndexCnt);

	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

	mDirLight.Ambient = XMFLOAT4(1.f, 1.f, 1.f,1.f);
	mDirLight.Diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	mDirLight.Specular = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	mDirLight.Direction = XMFLOAT3(1.f, -1.f, 1.f);

	return S_OK;
}

HRESULT CRain::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Rain");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_WavesTex");
	m_pBuffer = dynamic_cast<Engine::CWavesTex*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Waves", pComponent));

	return S_OK;
}

CRain * CRain::Create(Engine::MYGDI * pMyGDI)
{
	CRain* pLandObject = new CRain(pMyGDI);
	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);

	return pLandObject;
}
