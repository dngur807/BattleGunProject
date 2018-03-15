#include "Camera.h"
#include "Effects.h"

Engine::CCamera::CCamera(MYGDI* pMyGDI)
	: CGameObject(pMyGDI)
	, m_vEye(0.f, 0.f, 0.f)
	, m_vAt(0.f, 0.f, 0.f)
	, m_vUp(0.f, 1.f, 0.f)
	, m_fFovY(0.f)
	, m_fAspect(0.f)
	, m_fNear(0.f)
	, m_fFar(0.f)
	, m_pEffect(Engine::CEffects::GetInstance()->GetBasicFX())
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_matView, I);
	XMStoreFloat4x4(&m_matProj, I);
}

Engine::CCamera::~CCamera(void)
{

}

int Engine::CCamera::Update(void)
{
	Invalidate_ViewMatrix();
	
	return 0;
}

void Engine::CCamera::Invalidate_ViewMatrix(void)
{
	XMVECTOR vEye = XMVectorSet(m_vEye.x, m_vEye.y, m_vEye.z, 1.0f);
	XMVECTOR vAt = XMVectorSet(m_vAt.x, m_vAt.y, m_vAt.z, 1.0f);
	XMVECTOR vUp = XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 1.0f);

	XMMATRIX matView = XMMatrixLookAtLH(vEye, vAt, vUp);
	XMStoreFloat4x4(&m_matView, matView);

	m_pEffect->SetView(matView);
}

void Engine::CCamera::Invalidate_ProjMatrix(void)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI, m_fAspect, m_fNear, m_fFar);
	XMStoreFloat4x4(&m_matProj, P);

	m_pEffect->SetProj(P);
}

