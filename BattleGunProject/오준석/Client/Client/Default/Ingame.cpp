#include "stdafx.h"
#include "Ingame.h"
#include "Scene.h"
#include "GameMgr.h"
#include "Layer.h"
#include "GameObject.h"
#include "Gestalt.h"
#include "Export_Function.h"
#include "Loading.h"
#include "Stone.h"
#include "DynamicCamera.h"
#include "Transform.h"
#include "Player.h"
#include "Terrain.h"
#include "Export_System.h"
#include "CameraMgr.h"
#include "User.h"
#include "UserMgr.h"
#include "Character.h"
#include "MyCharacter.h"
#include "NetMgr.h"
#include "Protocol.h"
#include "Coder.h"
#include "Me.h"
#include "GeneralMgr.h"
#include "Cube.h"
#include "Engine_Include.h"
#include "NaviMgr.h"

CIngame::CIngame(Engine::MYGDI* pMyGDI)
	: Engine::CScene(pMyGDI)
	, m_pLoading(NULL)
	, m_pFX(Engine::Get_Management()->GetFX())
{

}

CIngame::~CIngame()
{
}

HRESULT CIngame::InitScene()
{
	XMMATRIX P = XMMatrixIdentity();

	m_pMyGDI->pEffect->SetWorld(P);
	m_pMyGDI->pEffect->SetView(P);
	m_pMyGDI->pEffect->SetProj(P);

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"Add_UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");
	FAILED_CHECK_RETURN_MSG(Add_NaviMesh(), E_FAIL, L"Add_NaviMesh Add false");
	return S_OK;
}

int CIngame::Update()
{
	CCameraMgr::GetInstance()->Update();
	CUserMgr::GetInstance()->Update();
	CGeneralMgr::GetInstance()->Update();

	Engine::CScene::Update();


	EnterCriticalSection(&CNetMgr::GetInstance()->CS);
	AnalysisInterMsg();
	LeaveCriticalSection(&CNetMgr::GetInstance()->CS);

	return 0;
}

void CIngame::Render()
{
	//Engine::CScene::Render();

	/*if (g_bNaviRender)
		Engine::CNaviMgr::GetInstance()->Render_NaviMesh();*/
}

CIngame* CIngame::Create(Engine::MYGDI* pMyGDI)
{
	CIngame* pIngame = new CIngame(pMyGDI);
	if (FAILED(pIngame->InitScene()))
		Engine::Safe_Delete(pIngame);
	return pIngame;
}

HRESULT CIngame::Add_Environment_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	// �� ����
	if (CGameMgr::GetInstance()->GetGameInfo().m_eMapType == MAP_GESTALT)
	{
		pGameObject = CGestalt::Create(m_pMyGDI);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);

		pLayer->AddObject(L"GestaltStructure", pGameObject);
	}
	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIRONMENT, pLayer));

	return S_OK;
}

HRESULT CIngame::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;


	map<int, CUser*>*	m_mapUser = CUserMgr::GetInstance()->GetUserList();
	auto iter = m_mapUser->begin();
	auto iter_end = m_mapUser->end();

	if (g_bNetwork == true)
	{
		for (iter; iter != iter_end; ++iter)
		{
			if (iter->second->GetUserInfo().iIndex != g_iIndex)
			{
				// �ٸ� ���� ĳ�� ����
				pGameObject = CCharacter::Create(m_pMyGDI);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
				pLayer->AddObject(L"Character", pGameObject);
				iter->second->SetCharacter((CCharacter*)pGameObject);
			}
		}
		pGameObject = CMyCharacter::Create(m_pMyGDI);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
		pLayer->AddObject(L"MyCharacter", pGameObject);
	
		(*CUserMgr::GetInstance()->GetUserList())[g_iIndex]->SetCharacter((CCharacter*)pGameObject);
	}
	else//�̱��÷��� ���ؼ�
	{
		pGameObject = CMyCharacter::Create(m_pMyGDI);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
		pLayer->AddObject(L"MyCharacter", pGameObject);

		g_iIndex = 0;
		CUserMgr::GetInstance()->GetMe()->GetUserInfo().iIndex = g_iIndex;
		CUserMgr::GetInstance()->GetUserList()->insert(make_pair(g_iIndex, CUserMgr::GetInstance()->GetMe()));
		(*CUserMgr::GetInstance()->GetUserList())[g_iIndex]->SetCharacter((CCharacter*)pGameObject);


		// ���� �÷��̾�
		CUser* pUser = new CUser;
		pUser->GetUserInfo().iIndex = g_iIndex + 1;
		//���� �÷��̾�
		CUserMgr::GetInstance()->GetUserList()->insert(make_pair(g_iIndex + 1 , pUser));

		pGameObject = CCharacter::Create(m_pMyGDI);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
		pLayer->AddObject(L"Character", pGameObject);

		(*CUserMgr::GetInstance()->GetUserList())[pUser->GetUserInfo().iIndex]->SetCharacter((CCharacter*)pGameObject);

	}


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CIngame::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pMyGDI);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;


	//ĳ���� ã��!
	auto iter = m_mapLayer.find(LAYER_GAMELOGIC);
	const Engine::CComponent*	pTargetTransform = iter ->second->GetComponent(L"MyCharacter" , L"Transform");
	NULL_CHECK_RETURN(pTargetTransform, E_FAIL);
	CCameraMgr::GetInstance()->SetTargetInfo(dynamic_cast<const Engine::CTransform*>(pTargetTransform));
	CCameraMgr::GetInstance()->SetCamera(CAMERA_FPS);


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

void CIngame::Release()
{
	Engine::Safe_Delete(m_pLoading);
}

HRESULT CIngame::Add_Light()
{
	m_DirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Direction = XMFLOAT3(1.f, -2.f, 1.f);


	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	return S_OK;
}

void CIngame::AnalysisInterMsg()
{
	queue<INTERMSGDATA>* qInterMsg = CNetMgr::GetInstance()->GetInterMsgQueue();

	CCoder coder;
	while (qInterMsg->empty() == false)
	{
		switch (qInterMsg->front().sHeaderType)
		{
		case NOTIFY_INPUT:
		{
			UINT iMask;
			char cIndex;

			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cIndex);
			coder.GetUInt(&iMask);

			if (g_iIndex != cIndex)
				CUserMgr::GetInstance()->GetUser(cIndex)->SetInput(iMask);
		}
			break;
		case NOTIFY_POSDIR:
		{
			XMFLOAT3 vPos;
			UINT iMask;
			char cIndex;
			float fAngleX, fAngleY;
			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cIndex);
			coder.GetUInt(&iMask);
			coder.GetXMFLOAT3(&vPos);
			coder.GetFloat(&fAngleX);
			coder.GetFloat(&fAngleY);
			if (g_iIndex != cIndex)
			{
				CUserMgr::GetInstance()->GetUser(cIndex)->SetInput(iMask);
				CUserMgr::GetInstance()->GetUser(cIndex)->GetCharacter()->SetPosDir(vPos, fAngleX, fAngleY);
			}
		}
			break;

		case NOTIFY_GIVEDAMAGE:
		{
			char cAttackerIndex;
			char cTargetIndex;
			int iDamage;
			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cAttackerIndex);
			coder.GetChar(&cTargetIndex);
			coder.GetInt(&iDamage);

			if (cTargetIndex == g_iIndex)
			{

				CUserMgr::GetInstance()->GetUser(g_iIndex)
					->GetCharacter()->GetDamage(cAttackerIndex, iDamage);
			}
		}
			break;

		case NOTIFY_DEAD:
		{
			char cVictimIndex;
			char cAttackerIndex;
			coder.SetBuf(qInterMsg->front().tcInterMsg);

			coder.GetChar(&cVictimIndex);
			coder.GetChar(&cAttackerIndex);
			(*CUserMgr::GetInstance()->GetUserList())[cVictimIndex]->SetDead();
			(*CUserMgr::GetInstance()->GetUserList())[cVictimIndex]->GetUserInfo().iDeath += 1;
			(*CUserMgr::GetInstance()->GetUserList())[cAttackerIndex]->GetUserInfo().iKill += 1;

			//ų�ΰ� �ٿ���
		}
			break;
		case NOTIFY_HPSYNC:
		{
			char cIndex;
			int iHp;
			coder.SetBuf(qInterMsg->front().tcInterMsg);
			coder.GetChar(&cIndex);
			coder.GetInt(&iHp);

			if (cIndex != g_iIndex)
			{
				(*CUserMgr::GetInstance()->GetUserList())[cIndex]->GetCharacter()->GetCharInfo().iHp = iHp;
			}
		}
			break;
		}
		qInterMsg->pop();
	}
}

HRESULT CIngame::Add_NaviMesh()
{
	Engine::Get_NaviMgr()->ReserveCellSize(m_pMyGDI, 500);
	DWORD dwByte = 0;
	HRESULT		hr = NULL;
	m_vecSaveMesh.clear();
	m_vecSaveOption.clear();

	while (1)
	{
		if (Engine::Get_NaviMgr()->DeleteCell() == FALSE)
			break;
	}
	m_iCount = 0;

	// ���� �׺�޽� �״�� �ΰ� �̾ �ҷ�����
	HANDLE hFile;
	hFile = CreateFile(L"../bin/Resources/Data/Gestalt_Navi.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	unsigned int iSize = 0;

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);


	XMFLOAT4 Plane;
	Plane.x = 0;
	Plane.y = 0;
	Plane.z = 0;
	Plane.w = 0;

	//static int i=0;
	while (true)
	{
		Engine::NAVMESH* pNavMesh = new Engine::NAVMESH;

		ReadFile(hFile, pNavMesh, sizeof(Engine::NAVMESH), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(pNavMesh);
			break;
		}
		//m_iOption = pNavMesh->iOption;
		CheckClockWise(pNavMesh->vecNavMesh);

		// ������ ����
		for (int i = 0; i<3; ++i)
			m_vecSaveMesh.push_back(pNavMesh->vecNavMesh[i]);

		XMVECTOR vPos4 = XMPlaneFromPoints(XMLoadFloat3(&pNavMesh->vecNavMesh[0]), XMLoadFloat3(&pNavMesh->vecNavMesh[1]), XMLoadFloat3(&pNavMesh->vecNavMesh[2]));
		Plane.x = XMVectorGetX(vPos4);
		Plane.y = XMVectorGetY(vPos4);
		Plane.z = XMVectorGetZ(vPos4);
		Plane.w = XMVectorGetW(vPos4);

		hr = Engine::Get_NaviMgr()->AddCell(&pNavMesh->vecNavMesh[0], &pNavMesh->vecNavMesh[1], &pNavMesh->vecNavMesh[2], Plane);
		FAILED_CHECK(hr);
		Engine::Safe_Delete(pNavMesh);

	}
	Engine::Get_NaviMgr()->CreateBuffer();
	Engine::Get_NaviMgr()->LinkCell();

	return S_OK;
}

void CIngame::CheckClockWise(XMFLOAT3 vecNavMesh[3])
{
	XMFLOAT3 vec1 = XMFLOAT3(vecNavMesh[1].x - vecNavMesh[0].x,
		vecNavMesh[1].y - vecNavMesh[0].y,
		vecNavMesh[1].z - vecNavMesh[0].z);

	XMFLOAT3 vec2 = XMFLOAT3(vecNavMesh[2].x - vecNavMesh[1].x,
		vecNavMesh[2].y - vecNavMesh[1].y,
		vecNavMesh[2].z - vecNavMesh[1].z);

	XMFLOAT3 vecCross;

	XMStoreFloat3(&vecCross, XMVector3Cross(XMLoadFloat3(&vec1), XMLoadFloat3(&vec2)));

	if (vecCross.y < 0)
	{
		XMFLOAT3 vecTemp = vecNavMesh[1];
		vecNavMesh[1] = vecNavMesh[2];
		vecNavMesh[2] = vecTemp;
	}
}
