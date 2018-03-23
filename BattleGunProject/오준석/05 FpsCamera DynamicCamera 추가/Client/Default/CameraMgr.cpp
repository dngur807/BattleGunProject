#include "stdafx.h"
#include "CameraMgr.h"
#include "DynamicCamera.h"
#include "Export_Function.h"
#include "FpsCamera.h"

IMPLEMENT_SINGLETON(CCameraMgr);

CCameraMgr::CCameraMgr()
{
	m_pDynamicCamera = NULL;
	m_pSelectedCamera = NULL;
	m_pFpsCamera = NULL;
	Initialize();
}

CCameraMgr::~CCameraMgr()
{
	Engine::Safe_Delete(m_pFpsCamera);
	Engine::Safe_Delete(m_pDynamicCamera);
}

void CCameraMgr::Initialize()
{
	//카메라 모두 생성
	m_pFpsCamera = CFpsCamera::Create(Engine::Get_Management()->GetGDI());
	m_pDynamicCamera = CDynamicCamera::Create(Engine::Get_Management()->GetGDI() , &XMFLOAT3(0.f, 5.f, -20.f), &XMFLOAT3(0.f, 0.f, 0.f));

	//실제 사용하는 카메라 설정
	m_pSelectedCamera = m_pFpsCamera;
	//m_pSelectedCamera->Invalidate_ViewMatrix();
	//m_pSelectedCamera->Invalidate_ProjMatrix();
}

void CCameraMgr::Update()
{
	if (m_pSelectedCamera)
		m_pSelectedCamera->Update();
}

void CCameraMgr::SetTargetInfo(const Engine::CTransform* pTargetInfo)
{
	m_pFpsCamera->SetTargetInfo(pTargetInfo);

}

void CCameraMgr::SetFPSEye(const XMFLOAT4X4* matEye)
{
	m_pFpsCamera->SetFPSEye(matEye);
}

void CCameraMgr::SetCamera(CAMERAMODE eMode)
{
	switch (eMode)
	{
	case CAMERA_FPS:
		m_pSelectedCamera = m_pFpsCamera;
		g_iCamMode = CAMERA_FPS;
		break;
	case CAMERA_STATIC:
		g_iCamMode = CAMERA_STATIC;
		break;
	case CAMERA_DYNAMIC:
		m_pSelectedCamera = m_pDynamicCamera;
		g_iCamMode = CAMERA_DYNAMIC;
		break;
	case CAMERA_END:
		break;
	default:
		break;
	}
//	m_pSelectedCamera->Invalidate_ViewMatrix();
//	m_pSelectedCamera->Invalidate_ProjMatrix();
	
}

