#include "stdafx.h"
#include "SkyBox1.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "WavesTex.h"
#include "BoxTex.h"
#include "WavesTex.h"
#include "VIBuffer.h"

CSkyBox1::CSkyBox1(Engine::MYGDI * pMyGDI)
	:CLandObject(pMyGDI),
	m_pResourceMgr(Engine::Get_ResourceMgr()),
	m_pTexture(nullptr),
	fTime(0.f)
{
	mSkyBox1 = new Engine::CWavesTex(pMyGDI);
	mSkyBox1 = mSkyBox1->Create(pMyGDI);
	//	mSkyBox1->mSkyBox1.Init(20, 20, 0.8f, 0.03f, 3.25f, 0.4f);


}

CSkyBox1::~CSkyBox1()
{
	delete mSkyBox1;
}

int CSkyBox1::Update(void)
{
	Engine::CGameObject::Update();
	//m_pBuffer->Update();

	//fTime += Engine::Get_TimeMgr()->DeltaTime();


	/*if ((fTime) >= 0.25f)
	{
		fTime = 0.f;
		DWORD i = 5 + rand() % 190;
		DWORD j = 5 + rand() % 190;

		float r = MathHelper::RandF(1.0f, 2.0f);

		mSkyBox1->mSkyBox1.Disturb(i, j, r);
	}
	mSkyBox1->mSkyBox1.Update(1);*/

	return 0;
}

void CSkyBox1::Render(void)
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



	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetBoxTech();
	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight2Tech();
	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetSkyBox1Tech();

	//XMMATRIX matWorld;
	//matWorld = XMMatrixIdentity();
	//m_pMyGDI->pEffect->SetWorld(matWorld);

	//m_pMyGDI->pEffect->SetDirLights(&mDirLight);

	//m_pMyGDI->pEffect->SetMaterial1(m_tMtrl);
	//m_pMyGDI->pEffect->SetEyePosW(XMFLOAT3(0, -1, 0));

	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight1Tech();
	m_pMyGDI->pEffect->SetChangeScene(false);

	//m_pTexture->Render(0, 0);
	mSkyBox1->DrawWaves(m_pInfo->m_matWorld, Engine::Get_TimeMgr()->DeltaTime(), m_pTexture->GetTextureView(2));

}

HRESULT CSkyBox1::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = XMFLOAT3(1.f, 1.f, 1.f);
	m_pInfo->m_vPos = XMFLOAT3(-7.f, 0.f, -1.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(155);

	//m_pBuffer->SetIdxInfo(m_pIndex, &m_iIndexCnt);

	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

	//mDirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	//mDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//mDirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//mDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLight.Ambient = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	mDirLight.Diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	mDirLight.Specular = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	mDirLight.Direction = XMFLOAT3(1.f, -1.f, 1.f);

	return S_OK;
}

HRESULT CSkyBox1::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_SkyBoxT");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_WavesTex");
	m_pBuffer = dynamic_cast<Engine::CWavesTex*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"SkyBox1", pComponent));

	//pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_SkyBox1Tex");
	//m_pBuffer = dynamic_cast<Engine::CSkyBox1Tex*>(pComponent);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"SkyBox1Tex", pComponent));

	return S_OK;
}

CSkyBox1 * CSkyBox1::Create(Engine::MYGDI * pMyGDI)
{
	CSkyBox1* pLandObject = new CSkyBox1(pMyGDI);
	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);

	return pLandObject;
}
