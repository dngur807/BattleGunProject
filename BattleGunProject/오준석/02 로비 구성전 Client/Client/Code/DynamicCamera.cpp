#include "stdafx.h"
#include "DynamicCamera.h"

#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"

CDynamicCamera::CDynamicCamera(Engine::MYGDI* pMyGDI)
	: Engine::CCamera(pMyGDI)
	, m_fCamSpeed(0.f)
	, m_bMouseFix(true)
	, m_bClick(false)
	, m_pPlayer(NULL)
	, m_pRealPlayer(NULL)
	, m_fDynmaicTargetDistance(0.f)
	, m_fDynamicHeight(0.f)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

int CDynamicCamera::Update(void)
{
	TargetRenewal();
	KeyCheck();
	/*if (m_bMouseFix)
	{
		MouseMove();
		FixMouse();
	}*/

	Engine::CCamera::Update();

	return 0;
}

CDynamicCamera* CDynamicCamera::Create(Engine::MYGDI* pMyGDI, 
	const XMFLOAT3* pEye, const XMFLOAT3* pAt, const Engine::CTransform* pPlayer)
{
	CDynamicCamera*		pCamera = new CDynamicCamera(pMyGDI);
	if (FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Delete(pCamera);

	pCamera->m_pPlayer = pPlayer;
	pCamera->m_pRealPlayer = const_cast<Engine::CTransform*>(pCamera->m_pPlayer);

	return pCamera;
}

HRESULT CDynamicCamera::InitCamera(const XMFLOAT3* pEye, const XMFLOAT3* pAt)
{
	m_fDynmaicTargetDistance = 2000.f;
	m_fDynamicHeight = 900.f;
	m_fCamSpeed = 5.f;

	m_vEye = *pEye;
	m_vAt = *pAt;
	CCamera::Invalidate_ViewMatrix();

	m_fFovY = XMConvertToRadians(45.f);
	m_fAspect = float(WINCX) / WINCY;
	m_fNear = 1.f;
	m_fFar = 1000.f;
	CCamera::Invalidate_ProjMatrix();

	return S_OK;
}

void CDynamicCamera::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->DeltaTime();

	if (Engine::Get_Input()->GetDIKeyState(DIK_Q))
	{
		if (m_bClick)
			return;

		m_bClick = true;

		if (m_bMouseFix == true)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;
	}
	else
		m_bClick = false;

	/*if (m_bMouseFix == false)
		return;*/

	if (Engine::Get_Input()->GetDIKeyState(DIK_A))
	{
		XMMATRIX matCamState = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matView));
		XMVECTOR vRight = matCamState.r[0];
		//XMVectorSetW(vRight, 0.0f);
		vRight = XMVector3Normalize(vRight);

		XMVECTOR vEye = XMLoadFloat3(&m_vEye);
		XMVECTOR vAt = XMLoadFloat3(&m_vAt);

		vEye -= vRight * m_fCamSpeed * fTime;
		vAt -= vRight * m_fCamSpeed * fTime;

		XMStoreFloat3(&m_vEye, vEye);
		XMStoreFloat3(&m_vAt, vAt);
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_D))
	{
		XMMATRIX matCamState = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matView));
		XMVECTOR vRight = matCamState.r[0];
		//XMVectorSetW(vRight, 0.0f);
		vRight = XMVector3Normalize(vRight);

		XMVECTOR vEye = XMLoadFloat3(&m_vEye);
		XMVECTOR vAt = XMLoadFloat3(&m_vAt);

		vEye += vRight * m_fCamSpeed * fTime;
		vAt += vRight * m_fCamSpeed * fTime;

		XMStoreFloat3(&m_vEye, vEye);
		XMStoreFloat3(&m_vAt, vAt);
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_W))
	{
		XMVECTOR vEye = XMLoadFloat3(&m_vEye);
		XMVECTOR vAt = XMLoadFloat3(&m_vAt);
		XMVECTOR vLook = vAt - vEye;
		vLook = XMVector3Normalize(vLook);

		vEye += vLook * m_fCamSpeed * fTime;
		vAt += vLook * m_fCamSpeed * fTime;

		XMStoreFloat3(&m_vEye, vEye);
		XMStoreFloat3(&m_vAt, vAt);
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_S))
	{
		XMVECTOR vEye = XMLoadFloat3(&m_vEye);
		XMVECTOR vAt = XMLoadFloat3(&m_vAt);
		XMVECTOR vLook = vAt - vEye;
		vLook = XMVector3Normalize(vLook);

		vEye -= vLook * m_fCamSpeed * fTime;
		vAt -= vLook * m_fCamSpeed * fTime;

		XMStoreFloat3(&m_vEye, vEye);
		XMStoreFloat3(&m_vAt, vAt);
	}
}

void CDynamicCamera::FixMouse(void)
{
	POINT	ptMouse = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDynamicCamera::MouseMove(void)
{
	int		iDistance = 0;
	if (iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_X))
	{
		XMMATRIX matAxis = XMMatrixRotationY(XMConvertToRadians(iDistance / 10.f));
			//XMMatrixRotationAxis(g_vUp, );
		XMVECTOR vDir = XMLoadFloat3(&m_vAt) - XMLoadFloat3(&m_vEye);
		vDir = XMVector3TransformNormal(vDir, matAxis);

		XMVECTOR vAt = XMLoadFloat3(&m_vEye) + vDir;
		XMStoreFloat3(&m_vAt, vAt);
	}

	if (iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		XMMATRIX matCamState = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matView));
		XMVECTOR vRight = matCamState.r[0];
		vRight = XMVector3Normalize(vRight);

		XMMATRIX matAxis = XMMatrixRotationAxis(vRight, XMConvertToRadians(iDistance / 10.f));
		XMVECTOR vDir = XMLoadFloat3(&m_vAt) - XMLoadFloat3(&m_vEye);
		vDir = XMVector3TransformNormal(vDir, matAxis);

		XMVECTOR vAt = XMLoadFloat3(&m_vEye) + vDir;
		XMStoreFloat3(&m_vAt, vAt);
	}
}

void CDynamicCamera::TargetRenewal()
{
	
	//m_vEye = m_pRealPlayer->m_vPos - (m_pRealPlayer->m_vDir)*m_fDynmaicTargetDistance;
	//m_vEye.y += m_fDynamicHeight;

	XMVECTOR VP = XMLoadFloat3(&m_pRealPlayer->m_vPos);

	XMVECTOR vEye = XMLoadFloat3(&m_vEye);
	XMVECTOR vAt = XMLoadFloat3(&m_vAt);
	XMVECTOR vLook = vAt - vEye;
	vLook = XMVector3Normalize(vLook);

	XMStoreFloat3(&m_vEye, vEye);
	XMStoreFloat3(&m_vAt, VP);

	
}

