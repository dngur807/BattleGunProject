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
	, m_pTargetInfo(0)
	, m_fTargetDistance(0.f)
	, m_fCameraHeight(0.f)
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
	else
	{
		FixMouse();
		PlayerMouseMove();
		TargetRenewal();
	}

	Engine::CCamera::Update();
	m_pMyGDI->pEffect->SetEyePosW(m_vEye);
	m_pMyGDI->pEffect->SetEmitPosW(m_vEye);
	return 0;
}

CDynamicCamera* CDynamicCamera::Create(Engine::MYGDI* pMyGDI, 
	const XMFLOAT3* pEye, const XMFLOAT3* pAt, const Engine::CTransform* pGameObj)
{
	CDynamicCamera*		pCamera = new CDynamicCamera(pMyGDI);
	if (FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Delete(pCamera);

	pCamera->m_pTargetInfo = pGameObj;
	return pCamera;
}

HRESULT CDynamicCamera::InitCamera(const XMFLOAT3* pEye, const XMFLOAT3* pAt)
{
	m_fTargetDistance = 5.f;
	m_fCameraHeight = 130.f;
	m_fCamSpeed = 500.f;

	m_vEye = *pEye;
	m_vAt = *pAt;
	CCamera::Invalidate_ViewMatrix();

	m_fFovY = XMConvertToRadians(45.f);
	m_fAspect = float(WINCX) / WINCY;
	m_fNear = 0.1f;
	m_fFar = 3000.f;
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
			m_bMouseFix = false, g_iCameraNum = 2;
		else
			m_bMouseFix = true, g_iCameraNum = 1;
	}
	else
		m_bClick = false;

	if (m_bMouseFix == false)
		return;

	if (Engine::Get_Input()->GetDIKeyState(DIK_A)&& g_iCameraNum==1)
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

	if (Engine::Get_Input()->GetDIKeyState(DIK_D) && g_iCameraNum == 1)
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

	if (Engine::Get_Input()->GetDIKeyState(DIK_W) && g_iCameraNum == 1)
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

	if (Engine::Get_Input()->GetDIKeyState(DIK_S) && g_iCameraNum == 1)
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

void CDynamicCamera::PlayerMouseMove(void)
{
	int		iDistance = 0;
	if (iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_X))
	{
		XMMATRIX matAxis = XMMatrixRotationY(XMConvertToRadians(iDistance / 10.f));
		XMVECTOR vDir = XMLoadFloat3(&m_vAt) - XMLoadFloat3(&m_vEye);
		vDir = XMVector3TransformNormal(vDir, matAxis);

		XMVECTOR vAt = XMLoadFloat3(&m_vEye) + vDir;
		XMStoreFloat3(&m_vAt, vAt);

		//
		Engine::CTransform*	pRealPlayer = NULL;
		pRealPlayer = const_cast<Engine::CTransform*>(m_pTargetInfo);
		pRealPlayer->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(iDistance / 10.f);
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

	/*	Engine::CTransform*	pRealPlayer = NULL;
		pRealPlayer = const_cast<Engine::CTransform*>(m_pTargetInfo);
		XMStoreFloat3(&pRealPlayer->m_vDir, XMVector3TransformNormal(XMLoadFloat3(&pRealPlayer->m_vDir), matAxis));*/

		//m_pTargetInfo->m_fAngle[Engine::ANGLE_Y] += 1;

		//Engine::CTransform*	pRealPlayer = NULL;
		//pRealPlayer = const_cast<Engine::CTransform*>(m_pTargetInfo);
		//pRealPlayer->m_fAngle[Engine::ANGLE_X] -= XMConvertToRadians(iDistance / 10.f);
	}

}

void CDynamicCamera::TargetRenewal()
{
	//m_vEye = m_pTargetInfo->m_vPos - (m_pTargetInfo->m_vDir)*m_fTargetDistance;
	//m_vEye = m_pTargetInfo->m_vPos;
	//m_pTargetInfo->m_vPos - m_pTargetInfo->m_vPos;

	XMVECTOR vDir = XMLoadFloat3(&m_pTargetInfo->m_vDir);
	vDir *= m_fTargetDistance;
	XMVECTOR vPos = XMLoadFloat3(&m_pTargetInfo->m_vPos);


	XMStoreFloat3(&m_vEye, vPos + vDir);
	m_vEye.y += m_fCameraHeight;


	//m_vAt = m_pTargetInfo->m_vPos;

	XMMATRIX matCamState = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matView));
	XMVECTOR vRight = matCamState.r[0];
	XMMATRIX matAxis = XMMatrixRotationAxis(vRight, XMConvertToRadians(m_pTargetInfo->m_fAngle[0]));
	vDir = XMLoadFloat3(&m_vAt) - XMLoadFloat3(&m_vEye);
	vDir = XMVector3TransformNormal(vDir, matAxis);
	XMStoreFloat3(&m_vAt, vPos+ vDir);

	m_vAt.y += m_fCameraHeight*0.5f;

	


	//D3DXVec3TransformNormal(&m_vAt, &m_vAt, &matRotAxis);
	//SetViewSpaceMatrix(&m_vEye, &m_vAt, &m_vUp);
}

