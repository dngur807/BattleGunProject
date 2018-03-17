#include "stdafx.h"
#include "ReactUI_2D.h"
#include "Engine_Macro.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Const.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Effects.h"

CReactUI_2D::CReactUI_2D(Engine::MYGDI* pMyGDi)
	: CGameObject(pMyGDi)
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
{
	m_fPosX = 0.0f;
	m_fPosY = 0.0f;
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
}


CReactUI_2D::~CReactUI_2D()
{
}

HRESULT CReactUI_2D::Initialize(RESOURCETYPE eType, TCHAR* pResourceKey)
{
	FAILED_CHECK(AddComponent(eType, pResourceKey));

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_pInfo->m_matWorld, I);

//	m_pInfo->m_vScale = XMFLOAT3(2.0f, 2, 1);
	//m_pInfo->m_vPos = XMFLOAT3(1.0f, -1.0f, 0.0f);
	m_pInfo->m_fAngle[Engine::ANGLE_Z] = XMConvertToRadians(270);

	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//빛초기화!
	m_DirLight.Ambient = XMFLOAT4(1, 1, 1, 1);
	m_DirLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Direction = XMFLOAT3(1.f, 1.0f, 1.f);


	return S_OK;
}

int CReactUI_2D::Update()
{
	Engine::CGameObject::Update();
	return 0;
}

void CReactUI_2D::Render()
{
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();


	m_pMyGDI->pEffect->SetTexTransform(matWorld);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render(m_pInfo->m_matWorld);

	
	//확인 용
	//Rectangle(GetDC(g_hWnd), m_rt.left, m_rt.top, m_rt.right, m_rt.bottom);
}

bool CReactUI_2D::IsMouseIn()
{
	m_rt.left = long(m_fPosX - m_fSizeX  / 2.0f);
	m_rt.right = long(m_fPosX + m_fSizeX / 2.0f);
	m_rt.top = long(m_fPosY - m_fSizeY / 2.0f);
	m_rt.bottom = long(m_fPosY + m_fSizeY  / 2.0f);

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	if (PtInRect(&m_rt, pt))
		return true;
	return false;
}

void CReactUI_2D::SetPos(float fX, float fY)
{
	m_pInfo->m_vPos = XMFLOAT3(fX / float(WINCX / 2) - 1.0f, -fY / float(WINCY / 2) + 1.0f, 0);
	m_fPosX = fX;
	m_fPosY = fY;
}

void CReactUI_2D::SetSize(float fX, float fY)
{
	m_pInfo->m_vScale = XMFLOAT3((fX * 2.f) / float(WINCX), (fY * 2.f) / float(WINCY), 1.f);

	//z크기 안맞네;;
	m_fSizeX = fY ;
	m_fSizeY = fX / 2;
}

HRESULT CReactUI_2D::AddComponent(RESOURCETYPE eType, TCHAR* pResourceKey)
{
	Engine::CComponent*			pComponent = NULL;

	// Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = Engine::CResourceMgr::GetInstance()->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	pComponent = Engine::CResourceMgr::GetInstance()->CloneResource(eType, pResourceKey);
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CReactUI_2D* CReactUI_2D::Create(Engine::MYGDI* pMyGDi, RESOURCETYPE eType, TCHAR* pResourceKey)
{
	CReactUI_2D* pObject = new CReactUI_2D(pMyGDi);
	if (FAILED(pObject->Initialize(eType, pResourceKey)))
		Engine::Safe_Delete(pObject);
	return pObject;
}
