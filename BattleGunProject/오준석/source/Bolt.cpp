#include "stdafx.h"
#include "Bolt.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "BoltTex.h"
#include "SphereTex.h"

CBolt::CBolt(Engine::MYGDI * pMyGDI)
	:CLandObject(pMyGDI), 
	m_pResourceMgr(Engine::Get_ResourceMgr()), 
	m_pTexture(nullptr)
{
}

CBolt::~CBolt()
{
}

int CBolt::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CBolt::Render(void)
{
	if (m_pMyGDI->pEffect == nullptr)
		return;
	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight0TexTech();

	XMMATRIX boltScale = XMMatrixScaling(10.0f, 5.0f, 10.0f);
	XMMATRIX boltTranslation = XMMatrixTranslation(Engine::Get_TimeMgr()->TotalTime() * 0.02f, 0.0f, 0.0f);
	XMStoreFloat4x4(&m_pInfo->m_matWorld, boltScale*boltTranslation);


	static float t = 0.0f;
	t += Engine::Get_TimeMgr()->DeltaTime();

	if (t >= 0.033333f)
	{
		mBoltFrameIndex++;
		t = 0.0f;

		if (mBoltFrameIndex == 60)
			mBoltFrameIndex = 0;
	}

	m_pTexture->Render(0, mBoltFrameIndex);
	dynamic_cast<Engine::CBoltTex*>(m_pBuffer)->Draw(m_pInfo->m_matWorld);

	m_pMyGDI->pEffect = Engine::Get_Effects()->GetBasicFX();

}

HRESULT CBolt::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = XMFLOAT3(0.f, 0.f, 0.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = XMConvertToRadians(30);
	m_pInfo->m_vScale = XMFLOAT3(10.f, 10.f, 10.f);
	return S_OK;
}

HRESULT CBolt::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Bolt");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_BoltTex");
	m_pBuffer = dynamic_cast<Engine::CBoltTex*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"BoltTex", pComponent));

	return S_OK;
}

CBolt * CBolt::Create(Engine::MYGDI * pMyGDI)
{
	CBolt* pLandObject = new CBolt(pMyGDI);
	if (FAILED(pLandObject->Initialize()))
		Engine::Safe_Delete(pLandObject);

	return pLandObject;
}
