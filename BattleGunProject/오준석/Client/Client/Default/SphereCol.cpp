#include "stdafx.h"
#include "SphereCol.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Enum.h"
#include "Texture.h"
#include "CubeTex.h"
#include "SphereTex.h"

CSphereCol::CSphereCol(Engine::MYGDI* pMyGDI)
	: m_pMyGDI(pMyGDI)
	, m_vPos(0, 0 , 0)
	, m_pTargetInfo(NULL)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CSphereCol::~CSphereCol()
{
	Release();
}

void CSphereCol::SetTargetInfo(const Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

CSphereCol* CSphereCol::Create(Engine::MYGDI* pMyGDI, float fRad)
{
	CSphereCol* pCol = new CSphereCol(pMyGDI);
	pCol->Initialize(fRad );

	return pCol;
}

void CSphereCol::Initialize(float fRad)
{
	m_fRad = fRad;
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Cube");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);

	//Buffer
	m_pBuffer = Engine::CSphereTex::Create(m_pMyGDI, m_fRad, 1);


	m_tMtrl.Ambient = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(0.f, 0.f, 0.f, 1.f);


}

int CSphereCol::Update()
{
	m_vPos = m_pTargetInfo->m_vPos;

	m_vPos.y += 6.f;

	m_pTexture->Update();
	m_pBuffer->Update();
	return 0;
}

void CSphereCol::Render()
{
	//XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pTargetInfo->m_matWorld));
	//m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);

	//m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	//m_pMyGDI->pEffect->SetTexTransform(XMMatrixIdentity());

	//XMFLOAT4X4 matWorld = m_pTargetInfo->m_matWorld;
	XMMATRIX matrixWorld = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	XMFLOAT4X4 matWorld;
	XMStoreFloat4x4(&matWorld , matrixWorld);


	// 와이어 프레임
	Engine::Get_GraphicDev()->WireFrame();

	//구 랜더
//	m_pTexture->Render(0, 0);
	m_pBuffer->Render(matWorld);
	Engine::Get_GraphicDev()->SolidFrame();




}

DWORD CSphereCol::Release()
{
	Engine::Safe_Delete(m_pTexture);
	Engine::Safe_Delete(m_pBuffer);
	return 0;
}
