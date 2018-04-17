#include "stdafx.h"
#include "Fire.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "CubeTex.h"
#include "SphereTex.h"
#include "TimeMgr.h"
#include "ParticleTex.h"
CFire::CFire(Engine::MYGDI * pMyGDI, XMFLOAT3& vPos, XMFLOAT3& vDir, const XMFLOAT4X4* matWeaponInfo, const XMFLOAT4X4* matWeaponRef)
	:CLandObject(pMyGDI), 
	m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pmatWeaponInfo(matWeaponInfo)
	, m_pmatWeaponRef(matWeaponRef)
	, m_pTexture(nullptr)
{
	m_fSpeed = 500.f;
}

CFire::~CFire()
{
}

int CFire::Update(void)
{
	//Engine::CGameObject::Update();
	m_fTime += Engine::Get_TimeMgr()->DeltaTime();

	/*if (GetAsyncKeyState('R') & 0x8000)
	{
		dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->Reset();
	}*/

	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)
		->Update(Engine::Get_TimeMgr()->DeltaTime(), Engine::Get_TimeMgr()->TotalTime());


	
	XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
	XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
	VP -= VD * m_fSpeed * m_fTime;
	XMStoreFloat3(&m_pInfo->m_vPos, VP);

	if (m_fTime > 3.f)
	{
		Engine::Get_Management()->DeleteRenderObject(this);
		return 1;
	}

	return 0;
}

void CFire::Render(void)
{
	if (m_pMyGDI->pEffect == nullptr)
		return;

	/*XMMATRIX mat = XMLoadFloat4x4(&m_pInfo->m_matWorld) * XMLoadFloat4x4(&(*m_pmatWeaponRef))  * XMLoadFloat4x4(&(*m_pmatWeaponInfo));
	XMStoreFloat4x4(&m_pInfo->m_matWorld, mat);*/

	m_pMyGDI->pEffect = Engine::Get_Effects()->GetFireFX();
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_pMyGDI->pContext->OMSetBlendState(0, blendFactor, 0xffffffff); // restore default
	//m_pTexture->ParticleRender(0, 0, mRandomTexSRV);
	//dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->SetImgSize(m_pInfo->m_vScale);


	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->SetEmitPos(m_pInfo->m_vPos);
	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->SetEyePos(g_vWorldCamera);
	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->Draw(m_pInfo->m_matWorld,mTexArraySRV,mRandomTexSRV);


	// restore default states.
	m_pMyGDI->pContext->RSSetState(0);
	m_pMyGDI->pContext->OMSetDepthStencilState(0, 0);
	m_pMyGDI->pContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	m_pMyGDI->pEffect = Engine::Get_Effects()->GetBasicFX();
}

HRESULT CFire::Initialize(void)
{
	FAILED_CHECK(AddComponent());


	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->SetmaxPaticles(500);
	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->SetEmitPos(XMFLOAT3(0.0f, 30.0f, 120.0f));

	m_pInfo->m_vScale = XMFLOAT3(10.f, 10.f, 10.f);
	m_pInfo->m_vPos = XMFLOAT3(0.f, 5.f, 0.f);

	XMMATRIX matDstRot = XMLoadFloat4x4(&(*m_pmatWeaponRef))  * XMLoadFloat4x4(&(*m_pmatWeaponInfo));
	XMVECTOR pos;
	pos = matDstRot.r[3];
	XMStoreFloat3(&m_pInfo->m_vPos, pos);

	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	return S_OK;
}

HRESULT CFire::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_ParticleTex");
	m_FireParticleBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"FireTex", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Fire");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	mRandomTexSRV =m_pTexture->CreateRandomTexture1DSRV(m_pMyGDI);
	mTexArraySRV = m_pTexture->GetTextureView1(0);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

CFire * CFire::Create(Engine::MYGDI * pMyGDI, XMFLOAT3& vPos, XMFLOAT3& vDir, const XMFLOAT4X4* matWeaponInfo, const XMFLOAT4X4* matWeaponRef)
{
	CFire* pLandObject = new CFire(pMyGDI, vPos,  vDir, matWeaponInfo, matWeaponRef);
	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);

	pLandObject->m_pInfo->m_vPos = vPos;
	pLandObject->m_pInfo->m_vDir = vDir;

	return pLandObject;
}
