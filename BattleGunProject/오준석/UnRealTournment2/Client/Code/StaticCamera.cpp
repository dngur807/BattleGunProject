#include "stdafx.h"
#include "StaticCamera.h"

#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"
#include "Player.h"

CStaticCamera::CStaticCamera(Engine::MYGDI* pMyGDI)
	: Engine::CCamera(pMyGDI)
	, m_fCamSpeed(0.f)
	, m_fWheelSpeed(0.f)
	, m_bMouseFix(true)
	, m_bClick(false)
	, m_pTargetInfo(0)
	, m_fTargetDistance(0.f)
	, m_fCameraHeight(0.f)
	, m_pInputDev_Instance(Engine::CInput::GetInstance())
{
	ZeroMemory(m_fAngle, sizeof(float) * Engine::ANGLE_END);
}


HRESULT CStaticCamera::InitCamera(const Engine::CTransform* pPlayer, const float & fFovY, const float & fAspect, const float & fNear, const float & fFar)
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

	//m_pPlayer = pPlayer;
	m_pPlayerTransform = pPlayer;
	m_fTargetDistance = 500;
	m_fCamSpeed = 500;
	m_fWheelSpeed = 250.f;


	
	m_vAt = pPlayer->m_vPos;
	m_vAt.y = m_vAt.y + 130;	//�Ӹ��� �߽����� �Ѵ�(�Ӹ� ���̰� �뷫 2���� ����)
	m_vEye = XMFLOAT3(m_vEye.x * m_fTargetDistance, m_vEye.y * m_fTargetDistance, m_vEye.z * m_fTargetDistance); //��ŭ ������ ���� ����

																												 //m_fAngle[Engine::ANGLE_Y] += 45;
	m_fAngle[Engine::ANGLE_Y] += 45;
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
int CStaticCamera::Update(void)
{
	//cout << m_pPlayerTransform->m_vPos.x << "\t" << m_pPlayerTransform->m_vPos.y << "\t" << m_pPlayerTransform->m_vPos.z << endl;

	m_fTimeDelta= Engine::Get_TimeMgr()->DeltaTime();

	//�켱�̰Թ����� �� �����ϱ�....
	XMFLOAT3		vDir;
	XMFLOAT4X4		matCamState;
	DirectX::XMStoreFloat4x4(&matCamState, XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&m_matView)));									//ī�޶��� ������� ���ؼ�
	DirectX::XMStoreFloat3(&vDir, XMVector3TransformNormal(DirectX::XMLoadFloat3(&g_vLook), DirectX::XMLoadFloat4x4(&matCamState)));	//���� ���� ���ϱ�
	m_vEye = XMFLOAT3(vDir.x * m_fTargetDistance, vDir.y * m_fTargetDistance, vDir.z * m_fTargetDistance);	
	

	KeyCheck();
	if (m_bMouseFix)
	{
		FixMouse();
	}

	m_vAt = m_pPlayerTransform->m_vPos; //�÷��̾� ���������� ����
	m_vAt.y += m_vAt.y + 130.f;	 //+ 2.f;
	m_vEye = XMFLOAT3(m_vEye.x + m_vAt.x, m_vEye.y + m_vAt.y, m_vEye.z + m_vAt.z); //Ÿ���� �̵� �� ��ŭ ī�Ŷ� ��ġ�� �̵� ��Ų��.



	Engine::CCamera::Update();
	m_pMyGDI->pEffect->SetEyePosW(m_vEye);
	m_pMyGDI->pEffect->SetEmitPosW(m_vEye);
	return 0;
}


CStaticCamera * CStaticCamera::Create(Engine::MYGDI * pMyGDI, const Engine::CTransform* pPlayer, const float & fFovY, const float & fAspect, const float & fNear, const float & fFar)
{
	CStaticCamera*		pCamera = new CStaticCamera(pMyGDI);
	if (FAILED(pCamera->InitCamera(pPlayer, fFovY, fAspect, fNear, fFar)))
	{
		Engine::Safe_Delete(pCamera);
	}
		
	return pCamera;
}


void CStaticCamera::KeyCheck(void)
{
	////
	int		iDistance = 0;
	if (m_pInputDev_Instance->OnceMouseKeyDown(Engine::CInput::DIM_RB))
	{
		if (m_bMouseFix == true)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;
	}

	if (m_bMouseFix == false)
		return;

	if (iDistance = m_pInputDev_Instance->Get_DIMouseMove(Engine::CInput::DIMS_X))
	{

		XMFLOAT3		vUp;
		XMFLOAT4X4		matCamState;
		DirectX::XMStoreFloat4x4(&matCamState, XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&m_matView)));
		memcpy(&vUp, &matCamState.m[1][0], sizeof(XMFLOAT3));

		XMFLOAT4X4		matAxis;
		DirectX::XMStoreFloat4x4(&matAxis, XMMatrixRotationAxis(DirectX::XMLoadFloat3(&vUp), XMConvertToRadians(iDistance / 10.f)));
		DirectX::XMStoreFloat3(&m_vEye, XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_vEye), DirectX::XMLoadFloat4x4(&matAxis)));
	}

	if (iDistance = m_pInputDev_Instance->Get_DIMouseMove(Engine::CInput::DIMS_Y))
	{
		float fTempAngle = m_fAngle[Engine::ANGLE_Y] + (iDistance / 10.f);;
		//�� ���� ���� �ɱ�
		if (fTempAngle >= 90)
			return;
		//�ٿ� ���� ���� �ɱ�
		if (fTempAngle <= -60)
			return;

		XMFLOAT3		vRight;
		XMFLOAT4X4		matCamState;
		DirectX::XMStoreFloat4x4(&matCamState, XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&m_matView)));
		memcpy(&vRight, &matCamState.m[0][0], sizeof(XMFLOAT3));
		DirectX::XMStoreFloat3(&vRight, XMVector3Normalize(DirectX::XMLoadFloat3(&vRight)));


		XMFLOAT4X4		matAxis;

		DirectX::XMStoreFloat4x4(&matAxis, XMMatrixRotationAxis(DirectX::XMLoadFloat3(&vRight), XMConvertToRadians(iDistance / 10.f))); //���� ����
		DirectX::XMStoreFloat3(&m_vEye, XMVector3TransformNormal(DirectX::XMLoadFloat3(&m_vEye), DirectX::XMLoadFloat4x4(&matAxis)));
		m_fAngle[Engine::ANGLE_Y] += (iDistance / 10.f);

	}


	//���� �ܾƿ�
	if (iDistance = m_pInputDev_Instance->Get_DIMouseMove(Engine::CInput::DIMS_Z))
	{
		float fTempDistance = m_fTargetDistance + iDistance * m_fTimeDelta;

		//�ִ�Ÿ� ���� �α�
		if (fTempDistance >= 150)
			return;

		//�ٿ� ���� ���� �ɱ�
		if (fTempDistance <= 200)
			return;

		m_fTargetDistance += iDistance * m_fTimeDelta;
	}
}

void CStaticCamera::FixMouse(void)
{
	POINT	ptMouse = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

