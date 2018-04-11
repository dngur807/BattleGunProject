#include "stdafx.h"
#include "UI.h"
#include "Export_Function.h"
CUI::CUI(Engine::MYGDI* pMyGDI)
	: Engine::CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_fX(0.0f)
	, m_fY(0.0f)
	, m_fSizeX(0.0f)
	, m_fSizeY(0.0f)
{
	XMStoreFloat4x4(&m_matProj , XMMatrixIdentity());
	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());
}

CUI::~CUI()
{

}

HRESULT CUI::Initialize()
{
	return S_OK;
}

int CUI::Update()
{
	return 0;
}

void CUI::Render()
{

}

CUI* CUI::Create(Engine::MYGDI* pMyGDI)
{
	CUI*		pGameObject = new CUI(pMyGDI);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CUI::SetUV(Engine::CVIBuffer* pBuffer, float x1, float y1, float x2, float y2, float SizeX, float SizeY)
{

	pBuffer->GetVtxInfo(m_pVtxTex);
	m_pVtxTex[0].vTexC = XMFLOAT2(x1 / SizeX, y1 / SizeY);
	m_pVtxTex[1].vTexC = XMFLOAT2(x2 / SizeX, y1 / SizeY);
	m_pVtxTex[2].vTexC = XMFLOAT2(x2 / SizeX, y2 / SizeY);
	m_pVtxTex[3].vTexC = XMFLOAT2(x1 / SizeX, y2 / SizeY);
	pBuffer->SetVtxInfo(m_pVtxTex);
}
