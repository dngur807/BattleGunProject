#include "stdafx.h"
#include "Terrain.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "TerrainTex.h"

CTerrain::CTerrain(Engine::MYGDI* pMyGDI)
	: Engine::CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_pTerrainVtx(NULL)
	, m_pIndex(NULL)
	, m_iIndexCnt(0)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CTerrain::~CTerrain(void)
{
	Release();
}

HRESULT CTerrain::Initialize(void)
{
	HRESULT		hr = AddComponent();
	FAILED_CHECK(hr);

	m_tMtrl.Ambient = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(0.f, 0.f, 0.f, 1.f);

	//m_pTerrainVtx = new Engine::VTXTEX[VTXCNTX * VTXCNTZ];
	//m_pBuffer->GetVtxInfo(m_pTerrainVtx);
	m_pTerrainVtx = dynamic_cast<Engine::CTerrainTex*>(m_pBuffer)->GetVertex();

	m_pIndex = new UINT[(VTXCNTX - 1) * (VTXCNTZ - 1) * 6];

	//hr = Engine::Get_QuadTree()->InitQuadTree(m_pTerrainVtx, VTXCNTX, VTXCNTZ);
	//FAILED_CHECK(hr);

	//hr = Engine::Get_QuadTree()->InitNeighbor(m_pTerrainVtx, Engine::Get_QuadTree(),
	//	VTXCNTX, VTXCNTZ);
	//FAILED_CHECK(hr);
	
	return S_OK;
}

void CTerrain::Release(void)
{
	Engine::Safe_Delete_Array(m_pIndex);
}

int CTerrain::Update(void)
{
	//Engine::Get_Frustum()->Transform_Frustum_Local(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	//Engine::Get_QuadTree()->IsinFrustum(m_pTerrainVtx);
	//Engine::Get_QuadTree()->Culling_LOD(m_pTerrainVtx, m_pIndex, &(m_iIndexCnt = 0));
	//Engine::Get_QuadTree()->CullingToQuadTree(m_pTerrainVtx, m_pIndex, &(m_iIndexCnt = 0));
	//Engine::Get_Frustum()->CullingForTerrain(m_pIndex, &(m_iIndexCnt = 0), m_pTerrainVtx
	//, VTXCNTX, VTXCNTZ);
	Engine::CGameObject::Update();

	return 0;
}

void CTerrain::Render(void)
{
	//m_pBuffer->SetIdxInfo(m_pIndex, &m_iIndexCnt);

	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(XMLoadFloat4x4(&m_pInfo->m_matWorld));
	m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);

	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetTexTransform(XMMatrixIdentity());

	m_pTexture->Render(0, 0);
	m_pBuffer->Render(m_pInfo->m_matWorld);
	
}

HRESULT CTerrain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_TerrainTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_TerrainTex");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CTerrain* CTerrain::Create(Engine::MYGDI* pMyGDI)
{
	CTerrain*		pGameObject = new CTerrain(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}




