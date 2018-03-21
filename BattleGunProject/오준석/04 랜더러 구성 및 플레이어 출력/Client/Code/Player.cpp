#include "stdafx.h"
#include "Player.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "StaticMesh.h"
#include "CubeColor.h"
#include "Bow.h"

CPlayer::CPlayer(Engine::MYGDI* pMyGDI)
	: CLandObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_iAniIndex(0)
	, m_fSpeed(0.f)
	, m_pBow(nullptr)
	, m_pBoxBuffer(nullptr)
	, m_pStaticMesh(nullptr)
	, m_pMesh(nullptr)
{

}

CPlayer::~CPlayer(void)
{
	Safe_Delete(m_pBoxBuffer);
	Safe_Delete(m_pBow);
}

HRESULT CPlayer::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_iAniIndex = 0;
	m_pMesh->SetAnimation(m_iAniIndex);

	m_pInfo->m_fAngle[Engine::ANGLE_Y] += 110.f;

	m_pInfo->m_vScale = XMFLOAT3(0.05f, 0.05f, 0.05f);
	m_pInfo->m_vDir = XMFLOAT3(1.f, 0.f, -1.f);;

	//g_vLook
	m_fSpeed = 50.f;

	//m_pMesh->SetAnimation(0);

	Engine::Get_InfoSubject()->AddData(MSG_PLAYERINFO, m_pInfo);

	/*m_pBow = CBow::Create(m_pMyGDI);
	D_CAST(CBow, m_pBow)->SetWeight(m_pMesh->GetAccumWeight());*/
	//
	return S_OK;
}

int CPlayer::Update(void)
{
	/*if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
	m_iAniIndex++;
	m_pMesh->SetAnimation(m_iAniIndex);
	}*/

	KeyCheck();

	m_pInfo->m_vPos.y -= 50.0f * Engine::Get_TimeMgr()->DeltaTime();

	if (m_pInfo->m_vPos.y < 0.f)
		m_pInfo->m_vPos.y = 0.f;

	// 건물과 충돌 체크


	//D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	//D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	XMVECTOR vecLook;
	XMVECTOR  vDir = XMVector3TransformNormal(XMLoadFloat3(&g_vLook), XMLoadFloat4x4(&m_pInfo->m_matWorld));
	vDir = XMVector3Normalize(vDir);
	XMStoreFloat3(&m_pInfo->m_vDir, vDir);

	Engine::CGameObject::Update();



	//if (m_pMesh->GetNumFrames() - 1 == m_pMesh->GetTrackPos())
	//{
	//	//m_iAniIndex = 0;
	//	m_pMesh->SetAnimation(0);
	//	//m_pMesh->InitElapsedTime();
	//}
	m_pMesh->Update();

	Engine::CGameObject::Update();

	//XMVecto)
	/*D_CAST(CBow, m_pBow)->SetMatrixInfo(XMLoadFloat4x4(&m_pInfo->m_matWorld),
	m_pMesh->GetAllMatrix(34), m_pMesh->GetChangeScene());*/
	//m_pBow->Update();

	return 0;
}

void CPlayer::Render(void)
{
	//m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());
//	m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);
	m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//	m_pBoxBuffer->Render(m_pInfo->m_matWorld);

	//m_pBow->Render();
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;
	Engine::CResources*		pResources = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"DynamicMesh_Sylva");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	//m_pStaticMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));

	/*m_pMesh = Engine::CDynamicMesh::Create(m_pMyGDI,
	L"../bin/Resources/Mesh/DynamicMesh/em002/FBX/", L"e001_merged.FBX");
	*/

	/*m_pMesh = Engine::CDynamicMesh::Create(m_pMyGDI,
	L"../bin/Resources/Mesh/DynamicMesh/em002/FBX/", L"banshee.x");*/

	/*m_pMesh = Engine::CDynamicMesh::Create(m_pMyGDI,
	L"../bin/Resources/Mesh/DynamicMesh/sylva/", L"sylva.X");*/

	//m_pMesh = Engine::CDynamicMesh::Create(m_pMyGDI,
	//	L"../bin/Resources/Mesh/DynamicMesh/Drone/", L"Drone.x");

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

void CPlayer::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->DeltaTime();

	//if (Engine::Get_Input()->GetDIKeyState(DIK_UPARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_W))
	{
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		VP -= VD * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_DOWNARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_S))
	{
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		VP += VD * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_LEFTARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_D))
	{
		//m_pInfo->m_fAngle[Engine::ANGLE_Y] += XMConvertToRadians(-90) * fTime;
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(m_pInfo->m_matWorld._21, m_pInfo->m_matWorld._22, m_pInfo->m_matWorld._23));

		XMVECTOR vRight = XMVector3Cross(vUp , VD);
		vRight = XMVector3Normalize(vRight);
		VP += vRight * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}
	//if (Engine::Get_Input()->GetDIKeyState(DIK_RIGHTARROW))
	if (Engine::Get_Input()->GetDIKeyState(DIK_A))
	{
		XMVECTOR VP = XMLoadFloat3(&m_pInfo->m_vPos);
		XMVECTOR VD = XMLoadFloat3(&m_pInfo->m_vDir);
		XMVECTOR vUp = XMLoadFloat3(&XMFLOAT3(m_pInfo->m_matWorld._21, m_pInfo->m_matWorld._22, m_pInfo->m_matWorld._23));

		XMVECTOR vRight = XMVector3Cross(vUp, VD);
		vRight = XMVector3Normalize(vRight);
		VP -= vRight * m_fSpeed * fTime;
		XMStoreFloat3(&m_pInfo->m_vPos, VP);
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_SPACE))
	{
		m_pInfo->m_vPos.y += 0.5f;
	}
	else
		m_bPush = false;
}
