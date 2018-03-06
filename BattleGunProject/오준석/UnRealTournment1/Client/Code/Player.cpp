#include "stdafx.h"
#include "Player.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "CubeColor.h"
#include "Bow.h"
#include "NaviCell.h"

CPlayer::CPlayer(Engine::MYGDI* pMyGDI)
	: CLandObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_iAniIndex(0)
	, m_fSpeed(0.f)
	, m_pBow(nullptr)
	, m_pBoxBuffer(nullptr)
	, m_dwIndex(0)
	, m_fTop(0)
	, m_bJump(false)
	, m_pMyCell(nullptr)
{
	m_bDown = false;
	m_fJumpAcc = 0.f;

}

CPlayer::~CPlayer(void)
{
	Safe_Delete(m_pMesh);
	Safe_Delete(m_pBoxBuffer);
	Safe_Delete(m_pBow);
	Safe_Delete(m_pMyCell);
}

HRESULT CPlayer::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_dwIndex = 0;
	m_iAniIndex = 0;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] += 180.f;
	m_pInfo->m_vPos = XMFLOAT3(105.f, 0.4f, 9.f);
	m_pInfo->m_vScale = XMFLOAT3(200.f, 200.f, 200.f);

	mDirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	m_fSpeed = 300.f;
	m_pMesh->SetAnimation(0);

	Engine::Get_InfoSubject()->AddData(MSG_PLAYERINFO, m_pInfo);

	m_BoundingBox = m_pMesh->GetBoundingBox();
	m_pInfo->m_vCenter = m_BoundingBox.Center;
	m_pInfo->m_vExt = m_BoundingBox.Extents;

	//
	m_pInfo->m_tPlayer.iHp = 100;
	m_pInfo->m_tPlayer.iMaxHp = 100;
	m_pInfo->m_tPlayer.iExp = 100;
	return S_OK;
}

int CPlayer::Update(void)
{
	m_fTime += Engine::Get_TimeMgr()->DeltaTime();
	
	
	//cout << m_dwIndex << endl;
	//if (GetAsyncKeyState(VK_F1) & 0x0001)
	//{
	//	//cout << m_iAniIndex << endl;
	//	m_iAniIndex++;
	//	m_pMesh->SetAnimation(m_iAniIndex);
	//}
	//if (!m_pMyCell)
	m_dwIndex = Engine::Get_NaviMgr()->FindIndex(&m_pInfo->m_vPos);
	m_fTop = Engine::Get_NaviMgr()->FindfTop(&m_pInfo->m_vPos);
	//cout << m_fTop << endl;
	//if (m_bJump || m_bDown)
	//{
	//	
	//}

	/*if (m_bJump == false && m_bDown == false)
		m_fTop = m_pInfo->m_vPos.y;*/

	Jump();
	KeyCheck();
	MyFrame();

	//D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, -1.f), &m_pInfo->m_matWorld);

	
	//m_pMesh->m_fElapsedTime += m_fTime*10.f;
	m_pMesh->Update();
	//m_pMesh1->Update();

	Engine::CGameObject::Update();

	/*system("cls");
	cout << m_pInfo->m_vPos.x << endl;
	cout << m_pInfo->m_vPos.y << endl;
	cout << m_pInfo->m_vPos.z << endl;*/

	//cout << m_pInfo->m_fAngle[Engine::ANGLE_Y] << endl;

	/*D_CAST(CBow, m_pBow)->SetMatrixInfo(XMLoadFloat4x4(&m_pInfo->m_matWorld),
		m_pMesh->GetAllMatrix(34), m_pMesh->GetChangeScene());*/
	//m_pBow->Update();

	//
	static bool BUTON7 = false;
	if (Engine::Get_Input()->GetDIKeyState(DIK_F1) )
	{
		BUTON7 = true;
	}
	else
	{
		if (BUTON7)
		{
			//기능
			m_pInfo->m_tPlayer.iHp+=10;
			m_pInfo->m_tPlayer.iExp += 10;
			BUTON7 = false;
		}
	}

	static bool BUTON8 = false;
	if (Engine::Get_Input()->GetDIKeyState(DIK_F2))
	{
		BUTON8 = true;
	}
	else
	{
		if (BUTON8)
		{
			//기능
			m_pInfo->m_tPlayer.iHp -= 10;
			m_pInfo->m_tPlayer.iExp -= 10;
			BUTON8 = false;
		}
	}

	return 0;
}

void CPlayer::Render(void)
{
	//와이어 프레임
	D3D11_RASTERIZER_DESC dxRasterDesc;
	::memset(&dxRasterDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	//dxRasterDesc.FillMode = D3D11_FILL_WIREFRAME;       //와이어
	dxRasterDesc.FillMode = D3D11_FILL_SOLID;          //아닌거
	dxRasterDesc.CullMode = D3D11_CULL_NONE;
	dxRasterDesc.DepthClipEnable = TRUE;
	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);

	
	m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));

	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);

	//	m_pBoxBuffer->Render(m_pInfo->m_matWorld);
	//m_pBow->Render();
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"DynamicMesh_Marine");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));


	// BoundingBox
	/*m_BoundingBox.Center = XMFLOAT3(0.f, 0.5f, 0.f);
	m_BoundingBox.Extents = XMFLOAT3(0.2f, 0.5f, 0.2f);
	m_pInfo->m_vCenter = m_BoundingBox.Center;
	m_pInfo->m_vExt = m_BoundingBox.Extents;
	XMVECTOR MIN = XMLoadFloat3(&m_BoundingBox.Center) - XMLoadFloat3(&m_BoundingBox.Extents);
	XMVECTOR MAX = XMLoadFloat3(&m_BoundingBox.Center) + XMLoadFloat3(&m_BoundingBox.Extents);
	pComponent = m_pBoxBuffer = Engine::CCubeColor::Create(m_pMyGDI, MIN, MAX,
		static_cast<XMFLOAT4>(Colors::Yellow));*/

	return S_OK;
}

CPlayer* CPlayer::Create(Engine::MYGDI* pMyGDI)
{
	CPlayer*		pGameObject = new CPlayer(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}


void CPlayer::MyFrame()
{

	if (m_dwState != m_dwPrevState)
	{
		switch (m_dwState)
		{
		case STATE_STAND:
			m_pMesh->SetAnimation(0);
			break;
		case STATE_WALK:
			m_pMesh->SetAnimation(2);
			break;
		}
		//변경된 현재 상태 값을 이전 상태값에 보관...
		m_dwPrevState = m_dwState;
	}

	if (m_pMesh->GetNumFrames() - 1 == m_pMesh->GetTrackPos())
	{

		if (m_dwState != STATE_STAND)
		{
			m_dwState = STATE_STAND;
		}
	
	}
}

void CPlayer::KeyCheck(void)
{
	fTime = Engine::Get_TimeMgr()->DeltaTime();


	//m_pInfo->m_vPos.y += fTime;
	//if (Engine::Get_Input()->GetDIKeyState(DIK_UPARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_W) && g_iCameraNum == 2)
	{
		m_dwState = STATE_WALK;
		//대각선인거타게하자!
	/*	m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &XMFLOAT3(-m_pInfo->m_vDir.x* m_fSpeed*fTime, -m_pInfo->m_vDir.y* m_fSpeed*fTime, -m_pInfo->m_vDir.z* m_fSpeed*fTime)
			, m_dwIndex, m_fTop);*/

		if (m_bJump || m_bDown || m_fTop>8.f) //1층을제외한건 걍떨어지게
		{
			XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
			XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
			VP -= VD * m_fSpeed * fTime;
			XMStoreFloat3(&m_pInfo->m_vPos, VP);
		}
		else
		{
			m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
				, &XMFLOAT3(-m_pInfo->m_vDir.x* m_fSpeed*fTime, -m_pInfo->m_vDir.y* m_fSpeed*fTime, -m_pInfo->m_vDir.z* m_fSpeed*fTime)
				, m_dwIndex, m_fTop);
		}

	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_DOWNARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_S) && g_iCameraNum == 2)
	{
		m_dwState = STATE_WALK;

		if (m_bJump || m_bDown || m_fTop>8.f) //1층을제외한건 걍떨어지게   //계단높이!
		{
			XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
			XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
			VP += VD * m_fSpeed * fTime;
			XMStoreFloat3(&m_pInfo->m_vPos, VP);
		}
		else
		{
			m_dwIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
				, &XMFLOAT3(m_pInfo->m_vDir.x* m_fSpeed*fTime, m_pInfo->m_vDir.y* m_fSpeed*fTime, m_pInfo->m_vDir.z* m_fSpeed*fTime)
				, m_dwIndex, m_fTop);
		}
	}
	if (Engine::Get_Input()->GetDIKeyState(DIK_D) && g_iCameraNum == 2)
	{
		m_dwState = STATE_WALK;
		//m_pInfo->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(-90) * fTime;
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		XMMATRIX M = XMMatrixRotationY(XMConvertToRadians(-90));
		VD = XMVector3TransformNormal(VD, M);
		VP += VD * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_RIGHTARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_A) && g_iCameraNum == 2)
	{
		m_dwState = STATE_WALK;
		//m_pInfo->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(90) * fTime;
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		XMMATRIX M = XMMatrixRotationY(XMConvertToRadians(90));
		VD = XMVector3TransformNormal(VD, M);
		VP += VD * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}

	/*float		fTime = Engine::Get_TimeMgr()->DeltaTime();

	if (Engine::Get_Input()->GetDIKeyState(DIK_UP))
	{
		m_pInfo->m_vPos += m_pInfo->m_vDir * m_fSpeed * fTime;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_DOWN))
	{
		m_pInfo->m_vPos -= m_pInfo->m_vDir * m_fSpeed * fTime;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_LEFT))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(90.f) * fTime;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_RIGHT))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f) * fTime;
	}*/


	if (Engine::Get_Input()->GetDIKeyState(DIK_SPACE))
	{
		if (m_bPush == true)
			return;
		m_bPush = true;
		m_bJump = true;
	}
	else
		m_bPush = false;
}

void CPlayer::Jump()
{
	if (m_bJump == true && m_bDown == false)
	{
		m_fJumpAcc += 0.6f*fTime;
		//위치에 점프속도를 더하고
		//속도에 가속도를 더한다..
		m_pInfo->m_vPos.y += -0.001f + m_fJumpAcc;
	}

	if (m_pInfo->m_vPos.y> 1.5f + m_fTop)
	{
		m_bDown = true;
	}

	if (m_bDown == true)
	{
		m_fJumpAcc -= 0.2f*fTime;

		m_pInfo->m_vPos.y += -0.001f + m_fJumpAcc;

	}
	if (m_pInfo->m_vPos.y <= m_fTop  && m_bDown == true)
	{

		m_bDown = false;
		m_bJump = false;
		m_fJumpAcc = 0.f;
		m_pInfo->m_vPos.y = m_fTop;

	}

}
