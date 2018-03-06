#include "stdafx.h"
#include "StaticCamera.h"

#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"
#include "Player.h"

CStaticCamera::CStaticCamera(Engine::MYGDI* pMyGDI)
	: Engine::CCamera(pMyGDI)
	, m_fCamSpeed(0.f)
	, m_bMouseFix(true)
	, m_bClick(false)
	, m_pTargetInfo(0)
	, m_fTargetDistance(0.f)
	, m_fCameraHeight(0.f)
{

}


int CStaticCamera::Update(void)
{
	m_fTimeDelta+= Engine::Get_TimeMgr()->DeltaTime();

	XMFLOAT3		vDir;
	XMFLOAT4X4		matCamState;

	DirectX::XMStoreFloat4x4(&matCamState, XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&m_matView)));									//카메라의 월드행렬 구해서
	DirectX::XMStoreFloat3(&vDir, XMVector3TransformNormal(DirectX::XMLoadFloat3(&g_vLook), DirectX::XMLoadFloat4x4(&matCamState)));	//방향 벡터 구하기
	m_vEye = XMFLOAT3(vDir.x * m_fTargetDistance, vDir.y * m_fTargetDistance, vDir.z * m_fTargetDistance);								//방향 벡터에 맞춰서 카메라 위치 다시 셋팅
	KeyCheck();

	m_vAt = m_pPlayerTransform->m_vPos; //플레이어 포스값으로 고정

	m_vAt.y = m_vAt.y + 1.5f;	 //+ 2.f;
	m_vEye = XMFLOAT3(m_vEye.x + m_vAt.x, m_vEye.y + m_vAt.y, m_vEye.z + m_vAt.z); //타겟이 이동 된 만큼 카매라 위치도 이동 시킨다.
	if (m_bMouseFix)
	{
		FixMouse();
	}

	//KeyCheck();
	//if (m_bMouseFix)
	//{
	//	MouseMove();
	//	FixMouse();
	//}
	//else
	//{
	//	FixMouse();
	//	PlayerMouseMove();
	//	TargetRenewal();
	//}

	Engine::CCamera::Update();
	m_pMyGDI->pEffect->SetEyePosW(m_vEye);
	m_pMyGDI->pEffect->SetEmitPosW(m_vEye);
	return 0;
}

//CStaticCamera* CStaticCamera::Create(Engine::MYGDI* pMyGDI, 
//	const XMFLOAT3* pEye, const XMFLOAT3* pAt, const Engine::CTransform* pGameObj)
//{
//	CStaticCamera*		pCamera = new CStaticCamera(pMyGDI);
//	if (FAILED(pCamera->InitCamera(pEye, pAt)))
//		Engine::Safe_Delete(pCamera);
//
//	pCamera->m_pTargetInfo = pGameObj;
//	return pCamera;
//}

CStaticCamera * CStaticCamera::Create(Engine::MYGDI * pMyGDI, const Engine::CTransform* pPlayer, const float & fFovY, const float & fAspect, const float & fNear, const float & fFar)
{
	CStaticCamera*		pCamera = new CStaticCamera(pMyGDI);
	if (FAILED(pCamera->InitCamera(pPlayer, fFovY, fAspect, fNear, fFar)))
	{
		Engine::Safe_Delete(pCamera);
	}
		
	return pCamera;
}

HRESULT CStaticCamera::InitCamera(const Engine::CTransform* pPlayer,  const float & fFovY, const float & fAspect, const float & fNear, const float & fFar)
{
	//m_fTargetDistance = 5.f;
	//m_fCameraHeight = 130.f;
	//m_fCamSpeed = 500.f;

	//m_vEye = *pEye;
	//m_vAt = *pAt;
	//CCamera::Invalidate_ViewMatrix();

	//m_fFovY = XMConvertToRadians(45.f);
	//m_fAspect = float(WINCX) / WINCY;
	//m_fNear = 0.1f;
	//m_fFar = 5000.f;
	//CCamera::Invalidate_ProjMatrix();

	//m_pPlayer = pPlayer;
	m_fTargetDistance = 5;
	m_fCamSpeed = 10.f;
	m_fWheelSpeed = 5.f;

	m_pPlayerTransform = pPlayer;

	//m_vAt = *m_pPlayer->GetTransform()->Get_Information(Engine::CTransform::INFO_POSITION); //플레이어 포스값으로 고정
	m_vAt = pPlayer->m_vPos;
	m_vAt.y = m_vAt.y + 2;	//머리를 중심으로 한다(머리 높이가 대략 2정도 높음)
	m_vEye = XMFLOAT3(m_vEye.x * m_fTargetDistance, m_vEye.y * m_fTargetDistance, m_vEye.z * m_fTargetDistance); //얼만큼 떨어질 정도 결정

	//m_fAngle[Engine::ANGLE_Y] += 45;
	m_vEye.y = 60.f;

	XMFLOAT4X4		matAxis;
	DirectX::XMStoreFloat4x4(&matAxis, XMMatrixRotationAxis(DirectX::XMLoadFloat3(&XMFLOAT3(1.0f, 0.f, 0.f)), XMConvertToRadians(45.f)));
	DirectX::XMStoreFloat3(&m_vEye, XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_vEye), DirectX::XMLoadFloat4x4(&matAxis)));
	Invalidate_ViewMatrix();
	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	Invalidate_ProjMatrix();

	return S_OK;
}

void CStaticCamera::KeyCheck(void)
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
	
	////
	//int		iDistance = 0;
	//if (m_pInputDev_Instance->OnceMouseKeyDown(Engine::CInput_Device::DIM_RB))
	//{
	//	if (m_bMouseFix == true)
	//		m_bMouseFix = false;
	//	else
	//		m_bMouseFix = true;
	//}

	//if (m_bMouseFix == false)
	//	return;

	//if (iDistance = m_pInputDev_Instance->Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
	//{

	//	XMFLOAT3		vUp;
	//	XMFLOAT4X4		matCamState;
	//	DirectX::XMStoreFloat4x4(&matCamState, XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&m_matView)));
	//	memcpy(&vUp, &matCamState.m[1][0], sizeof(XMFLOAT3));

	//	XMFLOAT4X4		matAxis;
	//	DirectX::XMStoreFloat4x4(&matAxis, XMMatrixRotationAxis(DirectX::XMLoadFloat3(&vUp), XMConvertToRadians(iDistance / 10.f)));
	//	DirectX::XMStoreFloat3(&m_vEye, XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_vEye), DirectX::XMLoadFloat4x4(&matAxis)));
	//}

	//if (iDistance = m_pInputDev_Instance->Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
	//{
	//	float fTempAngle = m_fAngle[Engine::ANGLE_Y] + (iDistance / 10.f);;
	//	//업 각도 제한 걸기
	//	if (fTempAngle >= 90)
	//		return;
	//	//다운 각도 제한 걸기
	//	if (fTempAngle <= -60)
	//		return;

	//	XMFLOAT3		vRight;
	//	XMFLOAT4X4		matCamState;
	//	DirectX::XMStoreFloat4x4(&matCamState, XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&m_matView)));
	//	memcpy(&vRight, &matCamState.m[0][0], sizeof(XMFLOAT3));
	//	DirectX::XMStoreFloat3(&vRight, XMVector3Normalize(DirectX::XMLoadFloat3(&vRight)));


	//	XMFLOAT4X4		matAxis;

	//	DirectX::XMStoreFloat4x4(&matAxis, XMMatrixRotationAxis(DirectX::XMLoadFloat3(&vRight), XMConvertToRadians(iDistance / 10.f))); //각도 변경
	//	DirectX::XMStoreFloat3(&m_vEye, XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_vEye), DirectX::XMLoadFloat4x4(&matAxis)));
	//	m_fAngle[Engine::ANGLE_Y] += (iDistance / 10.f);

	//}


	////줌인 줌아웃
	//if (iDistance = m_pInputDev_Instance->Get_DIMouseMove(Engine::CInput_Device::DIMS_Z))
	//{
	//	float fTempDistance = m_fTargetDistance + iDistance * m_fTimeDelta;

	//	//최대거리 제한 두기
	//	if (fTempDistance >= 15)
	//		return;

	//	//다운 각도 제한 걸기
	//	if (fTempDistance <= 2)
	//		return;

	//	m_fTargetDistance += iDistance * m_fTimeDelta;
	//}
}

void CStaticCamera::FixMouse(void)
{
	POINT	ptMouse = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CStaticCamera::MouseMove(void)
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

void CStaticCamera::PlayerMouseMove(void)
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

void CStaticCamera::TargetRenewal()
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

