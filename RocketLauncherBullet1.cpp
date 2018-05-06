#include "stdafx.h"
#include "RocketLauncherBullet1.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "CubeTex.h"
#include "SphereTex.h"
#include "TimeMgr.h"
#include "ParticleTex.h"
#include "SphereCol.h"
#include "BulletMgr.h"
CRocketLauncherBullet1::CRocketLauncherBullet1(Engine::MYGDI * pMyGDI, XMFLOAT3 vPos, XMFLOAT3 vDir, int iIndex)
	: CBullet(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pTexture(nullptr)
{
	m_iIndex = iIndex;
	m_vPos = vPos;
	m_vDir = vDir;
	m_fTime = 0.0f;
	m_fSpeed = 250.0f * 2;
}

CRocketLauncherBullet1::~CRocketLauncherBullet1()
{
}
HRESULT CRocketLauncherBullet1::Initialize(void)
{
	FAILED_CHECK(AddComponent());


	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->SetmaxPaticles(500);

	XMMATRIX matRot;
	XMMATRIX matScale;

	m_vScale.x = 5.0f;
	m_vScale.y = 5.0f;
	m_vScale.z = 5.0f;
	matRot = XMMatrixRotationY(XMConvertToRadians(-90.f));
	matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	
//	m_pInfo->m_vScale = XMFLOAT3(5.f, 5.f, 5.f);
//	m_pInfo->m_vPos = XMFLOAT3(0.f, 5.f, 0.f);

	XMVECTOR vAt = XMLoadFloat3(&m_vPos) + XMLoadFloat3(&m_vDir) * 1000.0f;
	XMMATRIX matrixWorld = XMMatrixLookAtLH(XMLoadFloat3(&m_vPos), vAt, g_xvUp);
	XMStoreFloat4x4(&m_matWorld, matrixWorld);
	m_matWorld._41 = m_matWorld._42 = m_matWorld._43 = 0.0f;


	matrixWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_matWorld));
	XMStoreFloat4x4(&m_matWorld, matrixWorld);
	m_matWorld._41 = m_vPos.x;
	m_matWorld._42 = m_vPos.y;
	m_matWorld._43 = m_vPos.z;
	matrixWorld = matScale * matRot * XMLoadFloat4x4(&m_matWorld);
	XMStoreFloat4x4(&m_matWorld, matrixWorld);

	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->Reset();
	return S_OK;
}
int CRocketLauncherBullet1::Update(void)
{
	//Engine::CGameObject::Update();
	m_fTime += Engine::Get_TimeMgr()->DeltaTime();

	if (m_fTime >= 2.0f)
		return 1;

	XMVECTOR vecPos;
	vecPos = XMLoadFloat3(&m_vPos) + m_fSpeed * XMLoadFloat3(&m_vDir) * Engine::Get_TimeMgr()->DeltaTime();
	XMStoreFloat3(&m_vPos, vecPos);

	memcpy(&m_matWorld.m[3][0], &m_vPos, sizeof(XMFLOAT3));

	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)
		->Update(Engine::Get_TimeMgr()->DeltaTime(), Engine::Get_TimeMgr()->TotalTime());

	//충돌체크
	XMFLOAT3 temp = XMFLOAT3(0.f, 0.f, 0.f); //충돌위치
	if (CBulletMgr::GetInstance()->Collision_Bullet(&m_vPos, m_pSphereCol->GetRadian(), &temp, m_iIndex))
	{
		return 1;
	}

	return 0;
}

void CRocketLauncherBullet1::Render(void)
{
	if (m_pMyGDI->pEffect == nullptr)
		return;

	if (g_bCollRender == true)
		m_pSphereCol->Render(m_vPos);


	m_pMyGDI->pEffect = Engine::Get_Effects()->GetFireFX();
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_pMyGDI->pContext->OMSetBlendState(0, blendFactor, 0xffffffff); // restore default

	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->SetEmitPos(m_vPos);
	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->SetEyePos(g_vWorldCamera);
	dynamic_cast<Engine::CParticleTex*>(m_FireParticleBuffer)->Draw(m_matWorld, mTexArraySRV, mRandomTexSRV,m_vScale);


	// restore default states.
	m_pMyGDI->pContext->RSSetState(0);
	m_pMyGDI->pContext->OMSetDepthStencilState(0, 0);
	m_pMyGDI->pContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	m_pMyGDI->pEffect = Engine::Get_Effects()->GetBasicFX();
}



HRESULT CRocketLauncherBullet1::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	//pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_FireTex");
	m_FireParticleBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"FireTex", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Bullet8_1");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	mRandomTexSRV = m_pTexture->CreateRandomTexture1DSRV(m_pMyGDI);
	mTexArraySRV = m_pTexture->GetTextureView1(0);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	//SphereCol
	pComponent = m_pSphereCol = CSphereCol::Create(m_pMyGDI, 3.0f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pSphereCol->SetTargetInfo(m_pInfo);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"SphereCol", pComponent));

	return S_OK;
}

CRocketLauncherBullet1 * CRocketLauncherBullet1::Create(Engine::MYGDI* pMYGDI, XMFLOAT3 vPos, XMFLOAT3 vDir, int iIndex)
{
	CRocketLauncherBullet1* pLandObject = new CRocketLauncherBullet1(pMYGDI,vPos,vDir, iIndex);
	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);

	return pLandObject;
}
