#include "stdafx.h"
#include "CharCol.h"
#include "DynamicMesh.h"
#include "SphereTex.h"
#include "Export_Function.h"
#include "Const.h"

CCharCol::CCharCol(Engine::MYGDI* pMyGDI, const XMFLOAT4X4* pMatPlayer)
	: m_pMyGDI(pMyGDI)
	, m_pmatPlayerInfo(pMatPlayer)
{
	m_vStartPoint = XMFLOAT3(0.f, 0.f, 0.f);
}

CCharCol::~CCharCol()
{

}

CCharCol* CCharCol::Create(Engine::MYGDI* pMyGDI, const XMFLOAT4X4* pMatPlayerInfo, Engine::CDynamicMesh* pCharMesh)
{
	CCharCol* pCharCOl = new CCharCol(pMyGDI, pMatPlayerInfo);
	pCharCOl->Initialize(pCharMesh);
	return pCharCOl;
}

void CCharCol::Initialize(Engine::CDynamicMesh* pCharMesh)
{
	m_pMatPartRef[0] = pCharMesh->FindFrame("bip01_Spine2");
	m_pMatPartRef[1] = pCharMesh->FindFrame("Bip01_L_Calf");
	m_pMatPartRef[2] = pCharMesh->FindFrame("Bip01_R_Calf");
	m_pMatPartRef[3] = pCharMesh->FindFrame("Bip01_L_Thigh");
	m_pMatPartRef[4] = pCharMesh->FindFrame("Bip01_R_Thigh");

	m_pMatPartRef[5] = pCharMesh->FindFrame("Bip01_Head");
	m_pMatPartRef[6] = pCharMesh->FindFrame("Bip01_L_Forearm");
	m_pMatPartRef[7] = pCharMesh->FindFrame("Bip01_R_Forearm");
	m_pMatPartRef[8] = pCharMesh->FindFrame("Bip01_L_Foot");
	m_pMatPartRef[9] = pCharMesh->FindFrame("Bip01_R_Foot");
	m_pMatPartRef[10] = pCharMesh->FindFrame("bip01_l_hand");
	m_pMatPartRef[11] = pCharMesh->FindFrame("Bip01_R_Hand");
	m_pMatPartRef[12] = pCharMesh->FindFrame("Bip01_L_UpperArm");
	m_pMatPartRef[13] = pCharMesh->FindFrame("Bip01_R_UpperArm");
	m_pMatPartRef[14] = pCharMesh->FindFrame("Bip01_L_Toe0");
	m_pMatPartRef[15] = pCharMesh->FindFrame("Bip01_R_Toe0");

	for (int i = 0; i < 16; i++)
		m_vSpherePos[i] = XMFLOAT3(0.f, 0.f, 0.f);

	m_pSphere[0] = Engine::CSphereTex::Create(m_pMyGDI, 2.0f * g_ScaleRatio, 1);
	m_pSphere[1] = Engine::CSphereTex::Create(m_pMyGDI, 1.5f * g_ScaleRatio, 1);
	m_pSphere[2] = Engine::CSphereTex::Create(m_pMyGDI, 1.5f * g_ScaleRatio, 1);
	m_pSphere[3] = Engine::CSphereTex::Create(m_pMyGDI, 1.5f * g_ScaleRatio, 1);
	m_pSphere[4] = Engine::CSphereTex::Create(m_pMyGDI, 1.5f * g_ScaleRatio, 1);
	m_pSphere[5] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);
	m_pSphere[6] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);
	m_pSphere[7] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);
	m_pSphere[8] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);
	m_pSphere[9] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);
	m_pSphere[10] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);


	m_pSphere[11] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);
	m_pSphere[12] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);
	m_pSphere[13] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);
	m_pSphere[14] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);
	m_pSphere[15] = Engine::CSphereTex::Create(m_pMyGDI, 1.2f * g_ScaleRatio, 1);

}

int CCharCol::Update()
{
	for (int i = 0; i < 16; i++)
	{
		XMMATRIX matrix = XMLoadFloat4x4(m_pMatPartRef[i]) * XMLoadFloat4x4(m_pmatPlayerInfo);

		XMVECTOR vPos = XMVector3TransformCoord(XMLoadFloat3(&m_vStartPoint), matrix);

		XMStoreFloat3(&m_vSpherePos[i], vPos);

	}
	return 0;
}

void CCharCol::Render()
{
	Engine::Get_GraphicDev()->WireFrame();
	XMMATRIX	matrixTrans;

	for (int i = 0; i < 16; i++)
	{

		matrixTrans = XMMatrixTranslation(m_vSpherePos[i].x, m_vSpherePos[i].y, m_vSpherePos[i].z);

		XMFLOAT4X4 matTrans;
		XMStoreFloat4x4(&matTrans, matrixTrans);
		m_pSphere[i]->Render(matTrans);
	}
	Engine::Get_GraphicDev()->SolidFrame();
}

DWORD CCharCol::Release()
{
	for (int i = 0; i < 16; ++i)
		Engine::Safe_Delete(m_pSphere[i]);
	return 0;
}

