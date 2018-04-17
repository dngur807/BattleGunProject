#include "stdafx.h"
#include "Rain.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "CubeTex.h"
#include "SphereTex.h"
#include "TimeMgr.h"
#include "ParticleTex.h"
CRain::CRain(Engine::MYGDI * pMyGDI)
	:CLandObject(pMyGDI),
	m_pResourceMgr(Engine::Get_ResourceMgr()),
	m_pTexture(nullptr)
{
}

CRain::~CRain()
{
}

int CRain::Update(void)
{
	Engine::CGameObject::Update();
	//m_fTime += Engine::Get_TimeMgr()->DeltaTime();

	if (GetAsyncKeyState('R') & 0x8000)
	{
		dynamic_cast<Engine::CParticleTex*>(m_RainParticleBuffer)->Reset();
	}

	dynamic_cast<Engine::CParticleTex*>(m_RainParticleBuffer)
		->Update(Engine::Get_TimeMgr()->DeltaTime(), Engine::Get_TimeMgr()->TotalTime());
	return 0;
}

void CRain::Render(void)
{
	if (m_pMyGDI->pEffect == nullptr)
		return;
	m_pMyGDI->pEffect = Engine::Get_Effects()->GetRainFX();
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//	m_pTexture->ParticleRender(0, 0, mRandomTexSRV);
	//	m_pTexture->Render(0, 0);

	dynamic_cast<Engine::CParticleTex*>(m_RainParticleBuffer)->SetEmitPos(g_vWorldCamera);
	dynamic_cast<Engine::CParticleTex*>(m_RainParticleBuffer)->SetEyePos(g_vWorldCamera);
	dynamic_cast<Engine::CParticleTex*>(m_RainParticleBuffer)->Draw(m_pInfo->m_matWorld, mTexArraySRV, mRandomTexSRV);


	// restore default states.
	m_pMyGDI->pContext->RSSetState(0);
	m_pMyGDI->pContext->OMSetDepthStencilState(0, 0);
	m_pMyGDI->pContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	m_pMyGDI->pEffect = Engine::Get_Effects()->GetBasicFX();
}

HRESULT CRain::Initialize(void)
{
	FAILED_CHECK(AddComponent());


	dynamic_cast<Engine::CParticleTex*>(m_RainParticleBuffer)->SetmaxPaticles(10000);
	//dynamic_cast<Engine::CParticleTex*>(m_RainParticleBuffer)->SetEmitPos(XMFLOAT3(0.0f, 1.0f, 120.0f));

	m_pInfo->m_vScale = XMFLOAT3(1000.f, 1000.f, 1000.f);
	m_pInfo->m_vPos = XMFLOAT3(0.f, 15.f, 0.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(30);


	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	return S_OK;
}

HRESULT CRain::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_ParticleTex");
	m_RainParticleBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"RainTex", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Rain");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	mRandomTexSRV = m_pTexture->CreateRandomTexture1DSRV(m_pMyGDI);
	mTexArraySRV = m_pTexture->GetTextureView1(0);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

CRain * CRain::Create(Engine::MYGDI * pMyGDI)
{
	CRain* pLandObject = new CRain(pMyGDI);
	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);

	return pLandObject;
}
