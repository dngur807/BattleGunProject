#include "stdafx.h"
#include "StaticCamera.h"
#include "Define.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"

CStaticCamera::CStaticCamera(Engine::MYGDI* pMyGDI)
	: Engine::CCamera(pMyGDI)
	, m_pTargetInfo(NULL)
	, m_fTargetDistance(0.0f)
	, m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
	, m_fCamSpeed(0.0f)
{
	m_bTraceOn = true;
}

CStaticCamera::~CStaticCamera()
{

}

void CStaticCamera::SetTargetInfo(const Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

HRESULT CStaticCamera::Initialize()
{
	m_fTargetDistance = 20.0f * g_ScaleRatio;
	m_fAngleX = XMConvertToRadians(-30.0f);
	m_fCamSpeed = 10.0f * g_ScaleRatio;

	m_fFovY = XMConvertToRadians(90.0f);
	m_fAspect = float(WINCX) / WINCY;
	m_fNear = 1.0f;
	m_fFar = 50000.0f;
	Invalidate_ProjMatrix();
	return S_OK;
}

int CStaticCamera::Update()
{
	POINT		ptMouse = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
	MouseMove();

	XMVECTOR vectorDir = XMLoadFloat3(&m_pTargetInfo->m_vDir);

	XMVECTOR vectorEye = vectorDir * -1.0f * m_fTargetDistance;
	XMStoreFloat3(&m_vEye, vectorEye);

	XMFLOAT3 vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(XMFLOAT3));


	XMMATRIX	matRotY;
	XMMATRIX	matRotX;
	XMMATRIX	matRotation;

	matRotY = XMMatrixRotationY(m_fAngleY);
	matRotX = XMMatrixRotationAxis(XMLoadFloat3(&vRight), m_fAngleX);

	matRotation = matRotX * matRotY;

	vectorEye = XMVector3TransformNormal(vectorEye, matRotation);

	XMStoreFloat3(&m_vEye, vectorEye);

	if (m_bTraceOn)
	{
		XMVECTOR vecAdd = XMLoadFloat3(&m_pTargetInfo->m_vPos) + XMLoadFloat3(&m_pTargetInfo->m_vDir);
		XMStoreFloat3(&m_vAt, vecAdd);

		vecAdd = XMLoadFloat3(&m_vEye) + XMLoadFloat3(&m_pTargetInfo->m_vPos);
		XMStoreFloat3(&m_vEye, vecAdd);
	}
	else
	{
		XMVECTOR vecAdd = XMLoadFloat3(&m_vPrevPos) + XMLoadFloat3(&m_pTargetInfo->m_vDir);
		XMStoreFloat3(&m_vAt, vecAdd);

		vecAdd = XMLoadFloat3(&m_vEye) + XMLoadFloat3(&m_vPrevPos);
		XMStoreFloat3(&m_vEye, vecAdd);
	}
	m_vAt.y += 5.0f * g_ScaleRatio;
	m_vEye.y += 15.0f * g_ScaleRatio;

	Engine::CCamera::Update();
	g_vWorldCamera = m_vEye;

	return 0;
}

void CStaticCamera::MouseMove()
{
	int		iDistance = 0;
	if (iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_X))
	{
		m_fAngleY -= XMConvertToRadians(iDistance / 5.f);
	}
	if (iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		m_fAngleX += XMConvertToRadians(iDistance / 5.f);
	}

}

CStaticCamera* CStaticCamera::Create(Engine::MYGDI* pMyGDI)
{
	CStaticCamera* pCamera = new CStaticCamera(pMyGDI);
	if (FAILED(pCamera->Initialize()))
		Engine::Safe_Delete(pCamera);
	return pCamera;
}

void CStaticCamera::SetTrace(bool bTraceOn)
{
	m_bTraceOn = bTraceOn;
	if (bTraceOn == false)
	{
		m_vPrevPos = m_pTargetInfo->m_vPos;
	}
}

