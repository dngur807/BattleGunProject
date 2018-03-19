#include "stdafx.h"
#include "Stone.h"

#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "NaviMgr.h"
#include "NaviCell.h"
#include "Line2D.h"
#include "CubeTex.h"

CStone::CStone(Engine::MYGDI * pMyGDI)
	:CLandObject(pMyGDI), 
	m_pResourceMgr(Engine::Get_ResourceMgr()),
	m_pMesh(nullptr), m_pMyCell(nullptr)
{
}

CStone::~CStone()
{
}

int CStone::Update(void)
{
	/*if(!m_pMyCell)
		m_pMyCell = Engine::Get_NaviMgr()->FindLocation(&m_pInfo->m_vPos);*/

	KeyCheck();

	Engine::CGameObject::Update();

	/*if (!CheckLocation(m_pMyCell))
		if (!CheckNeighborCell())
			Sliding();*/

	m_vPrePos = m_pInfo->m_vPos;

	return 0;
}

void CStone::Render(void)
{
	m_pMesh->RenderMesh(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//m_pBuffer->Render(m_pInfo->m_matWorld);
}

void CStone::KeyCheck(void)
{
}

bool CStone::CheckLocation(Engine::CNaviCell* pCell)
{
	XMFLOAT2 vTemp(m_pInfo->m_vPos.x, m_pInfo->m_vPos.z);
	XMVECTOR vPos = XMLoadFloat2(&vTemp);

	float fDot = 0.f;

	for (int i = 0; i < Engine::LINE_END; ++i)
	{
		XMVECTOR vNormal = XMLoadFloat2(pCell->GetLine2D(i)->GetNormal());
		XMFLOAT2 vPoint = XMFLOAT2(pCell->GetPoint()[i].x,
			pCell->GetPoint()[i].z);
		XMVECTOR vPointToPos = vPos - XMLoadFloat2(&vPoint);
		XMStoreFloat(&fDot, XMVector2Dot(vPointToPos, vNormal));
		if (0 < fDot)
			return false;
	}
	return true;
}

bool CStone::CheckNeighborCell(void)
{
	std::vector<Engine::CNaviCell*>* pvecCell = m_pMyCell->GetvecNeighbor();

	for (UINT i = 0; i < pvecCell->size(); ++i)
	{
		if (CheckLocation((*pvecCell)[i]))
		{
			m_pMyCell = (*pvecCell)[i];
			return true;
		}
	}
	return false;
}

void CStone::Sliding(void)
{
	XMVECTOR vPos3 = XMLoadFloat3(&m_pInfo->m_vPos);
	XMVECTOR vPRE = XMLoadFloat3(&m_vPrePos);
	XMFLOAT2 vTemp(m_pInfo->m_vPos.x, m_pInfo->m_vPos.z);
	XMVECTOR vPos = XMLoadFloat2(&vTemp);

	float fDot = 0.f;
	int iNum = -1;
	for (int i = 0; i < Engine::LINE_END; ++i)
	{
		XMVECTOR vNormal = XMLoadFloat2(m_pMyCell->GetLine2D(i)->GetNormal());
		XMFLOAT2 vPoint = XMFLOAT2(m_pMyCell->GetPoint()[i].x,
			m_pMyCell->GetPoint()[i].z);
		XMVECTOR vPointToPos = vPos - XMLoadFloat2(&vPoint);
		XMStoreFloat(&fDot, XMVector2Dot(vPointToPos, vNormal));
		if (0 < fDot)
		{
			vPos3 = vPRE;
			iNum = i;
			XMFLOAT2 vNormal = *(m_pMyCell->GetLine2D(iNum)->GetNormal());
			XMFLOAT3 vDir = XMFLOAT3(vNormal.y, 0.f, vNormal.x * -1.f);
			XMVECTOR VDIR = XMVector3Normalize(XMLoadFloat3(&vDir));
			vPos3 += VDIR * 5.f * Engine::Get_TimeMgr()->DeltaTime();
			XMStoreFloat3(&m_pInfo->m_vPos, vPos3);

			if(!CheckLocation(m_pMyCell))
				if(!CheckNeighborCell())
					XMStoreFloat3(&m_pInfo->m_vPos, vPRE);
			return;
		}
	}
}

HRESULT CStone::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	//m_pInfo->m_vPos = XMFLOAT3(5.f, 0.f, 5.f);
	m_pInfo->m_vPos = XMFLOAT3(float(rand() % VTXCNTX), 0.f, float(rand() % VTXCNTZ));

	// 충돌박스 정보 불러오기 및 Transform에 등록
	m_BoundingBox = m_pMesh->GetBoundingBox();
	m_pInfo->m_vCenter = m_BoundingBox.Center;
	m_pInfo->m_vExt = m_BoundingBox.Extents;

	//Engine::Get_InfoSubject()->AddData(MSG_INFOLIST, m_pInfo);

	//## 마우스 충돌 검색을 위한 Object의 Info를 등록ㅕ
	Engine::Get_InfoSubject()->AddData(MSG_INFOLIST, m_pInfo);

	return S_OK;
}

HRESULT CStone::AddComponent(void)
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	////Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"StaticMesh_Stone");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	return S_OK;
}

CStone * CStone::Create(Engine::MYGDI * pMyGDI)
{
	CStone* pGameObject = new CStone(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}
