#include "stdafx.h"
#include "DynamicCamera.h"

#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"

CDynamicCamera::CDynamicCamera(Engine::MYGDI* pMyGDI)
	: Engine::CCamera(pMyGDI)
	, m_fSpeed(0.f)
	, m_bMouseFix(true)
	, m_bClick(false)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

int CDynamicCamera::Update(void)
{
	KeyCheck();
	if (m_bMouseFix)
	{
		MouseMove();
		FixMouse();
	}

	Engine::CCamera::Update();
	g_vWorldCamera = m_vEye;
	return 0;
}

CDynamicCamera* CDynamicCamera::Create(Engine::MYGDI* pMyGDI, const XMFLOAT3* pEye, const XMFLOAT3* pAt)
{
	CDynamicCamera*		pCamera = new CDynamicCamera(pMyGDI);
	if (FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Delete(pCamera);
	return pCamera;
}

HRESULT CDynamicCamera::InitCamera(const XMFLOAT3* pEye, const XMFLOAT3* pAt)
{
	m_fSpeed = 50 *  g_ScaleRatio;
	m_bMouseFix = true;
	m_vEye = *pEye;
	m_vAt = *pAt;

	CCamera::Invalidate_ViewMatrix();

	m_fFovY = XMConvertToRadians(45.0f);
	m_fAspect = float(WINCX) / WINCY;
	m_fNear = 1.f;
	m_fFar = 50000.f;
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

	if (m_bMouseFix == false)
		return;

	if (Engine::Get_Input()->GetDIKeyState(DIK_A))
	{
		XMMATRIX matCamState = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matView));
		XMVECTOR vRight = matCamState.r[0];
		//XMVectorSetW(vRight, 0.0f);
		vRight = XMVector3Normalize(vRight);

		XMVECTOR vEye = XMLoadFloat3(&m_vEye);
		XMVECTOR vAt = XMLoadFloat3(&m_vAt);

		vEye -= vRight * m_fSpeed * fTime;
		vAt -= vRight * m_fSpeed * fTime;

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

		vEye += vRight * m_fSpeed * fTime;
		vAt += vRight * m_fSpeed * fTime;

		XMStoreFloat3(&m_vEye, vEye);
		XMStoreFloat3(&m_vAt, vAt);
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_W))
	{
		XMVECTOR vEye = XMLoadFloat3(&m_vEye);
		XMVECTOR vAt = XMLoadFloat3(&m_vAt);
		XMVECTOR vLook = vAt - vEye;
		vLook = XMVector3Normalize(vLook);

		vEye += vLook * m_fSpeed * fTime;
		vAt += vLook * m_fSpeed * fTime;

		XMStoreFloat3(&m_vEye, vEye);
		XMStoreFloat3(&m_vAt, vAt);
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_S))
	{
		XMVECTOR vEye = XMLoadFloat3(&m_vEye);
		XMVECTOR vAt = XMLoadFloat3(&m_vAt);
		XMVECTOR vLook = vAt - vEye;
		vLook = XMVector3Normalize(vLook);

		vEye -= vLook * m_fSpeed * fTime;
		vAt -= vLook * m_fSpeed * fTime;

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

