#include "stdafx.h"
#include "FpsCamera.h"
#include "Define.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"

// 사용하려면 대상 타겟 및 카메라 위치 설정해야함(캐릭터 초기화 할때 설정해주자)

CFpsCamera::CFpsCamera(Engine::MYGDI* pMyGID)
	: CCamera(pMyGID)
	, m_pTargetInfo(NULL)
	, m_fAngle(0.0f)
	, m_matEye(NULL)
{
	
	/*XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_matEye, I);*/
}

CFpsCamera::~CFpsCamera()
{

}

void CFpsCamera::SetTargetInfo(const Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

HRESULT CFpsCamera::Initialize()
{
	ShowCursor(false);
	m_fFovY = XMConvertToRadians(90.0f);
	m_fAspect = float(WINCX) / WINCY;
	m_fNear = 0.1f;
	m_fFar = 50000.0f;

	m_vEye = g_vWorldCamera;

	Invalidate_ProjMatrix();
	return S_OK;
}

int CFpsCamera::Update()
{
	if (m_matEye == NULL)
		return 0 ;

	// 마우스 가운데 고정
	POINT ptMouse = { WINCX >> 1 , WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
	MouseMove();

	XMFLOAT3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(XMFLOAT3));

	XMVECTOR vectorPos = XMLoadFloat3(&m_pTargetInfo->m_vPos);
	XMVECTOR vectorDir = XMLoadFloat3(&m_pTargetInfo->m_vDir);
	XMVECTOR vectorAt;
	vectorAt = vectorPos + vectorDir * 10000.f;

	XMStoreFloat3(&m_vAt, vectorAt);
	
	XMMATRIX matrixEye;

	matrixEye = XMLoadFloat4x4(m_matEye);
	XMMATRIX matrixWorld = XMLoadFloat4x4(&m_pTargetInfo->m_matWorld);
	matrixEye = XMMatrixMultiply(matrixEye ,  matrixWorld);
	XMFLOAT4X4 matEye;
	XMStoreFloat4x4(&matEye, matrixEye);

	memcpy(&m_vEye, &matEye.m[3][0], sizeof(XMFLOAT3));



	XMMATRIX matrixRotAxis;

	matrixRotAxis = XMMatrixRotationAxis(XMLoadFloat3(&vRight), m_fAngle);
	vectorAt = XMVector3TransformCoord(XMLoadFloat3(&m_vAt), matrixRotAxis);
	XMStoreFloat3(&m_vAt, vectorAt);

	Engine::CCamera::Update();

	g_vWorldCamera = m_vEye;
	g_vFpsAt = m_vAt;
	return 0;

}

void CFpsCamera::MouseMove()
{
	int iDist = 0;

	if (iDist = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_X))
	{
		const_cast<Engine::CTransform*>(m_pTargetInfo)->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(iDist / 7.f);
	}
	if (iDist = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		m_fAngle += XMConvertToRadians(iDist / 7.f);
		const_cast<Engine::CTransform*>(m_pTargetInfo)->m_fRotateX = m_fAngle;
	}
}

void CFpsCamera::SetFPSEye(const XMFLOAT4X4* matEye)
{
	m_matEye = matEye;
}

CFpsCamera* CFpsCamera::Create(Engine::MYGDI* pMyGID)
{
	CFpsCamera*	pCamera = new CFpsCamera(pMyGID);

	if (FAILED(pCamera->Initialize()))
		Engine::Safe_Delete(pCamera);
	return pCamera;
}

