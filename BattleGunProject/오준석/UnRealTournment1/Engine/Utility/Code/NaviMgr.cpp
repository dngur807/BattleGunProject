#include "NaviMgr.h"
#include "NaviCell.h"
#include "Effects.h"
#include "Line2D.h"

#include <iostream>
using namespace std;
IMPLEMENT_SINGLETON(Engine::CNaviMgr)

Engine::CNaviMgr::CNaviMgr()
	:m_pMyGDI(nullptr),
	m_iReservedSize(0),
	m_iIdxCnt(0),
	m_dwIdxCnt(0)
{
}


Engine::CNaviMgr::~CNaviMgr()
{
	Release();
}

void Engine::CNaviMgr::ReserveCellSize(MYGDI * pMyGDI, const UINT & iSize)
{
	m_pMyGDI = pMyGDI;
	m_iReservedSize = iSize;
	m_vecNaviMesh.reserve(iSize);
}

HRESULT Engine::CNaviMgr::AddCell(const XMFLOAT3 * pPointA, const XMFLOAT3 * pPointB, const XMFLOAT3 * pPointC, const XMFLOAT4& pPlane)
{
	CNaviCell* pCell = CNaviCell::Create(m_pMyGDI,
		pPointA, pPointB, pPointC, m_dwIdxCnt, pPlane);
	NULL_CHECK_RETURN(pCell, E_FAIL);

	m_dwIdxCnt += 1;


	m_vecNaviMesh.push_back(pCell);

	return S_OK;
}

void Engine::CNaviMgr::CreateBuffer(void)
{
	XMFLOAT3 vNormal = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT2 vTex[3] =
	{
		XMFLOAT2(0.f, 0.f), XMFLOAT2(1.f, 0.f), XMFLOAT2(0.f, 1.f)
	};

	for (UINT i = 0; i < m_vecNaviMesh.size(); ++i)
	{
		for (UINT j = 0; j < 3; ++j)
		{
			XMFLOAT3 vPos = m_vecNaviMesh[i]->GetPoint()[j];
			VTXTEX vVertex = VTXTEX(vPos, vNormal, vTex[j]);
			m_vecVertices.push_back(vVertex);
			m_vecIndices.push_back(m_iIdxCnt++);
		}
	}

	D3D11_INPUT_ELEMENT_DESC vtxDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
		D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;

	m_pMyGDI->pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vtxDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));


	//## Create Vertex Buffer
	D3D11_BUFFER_DESC tBufDesc;
	ZeroMemory(&tBufDesc, sizeof(D3D11_BUFFER_DESC));
	tBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufDesc.ByteWidth = sizeof(VTXTEX) * m_vecVertices.size();
	tBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tBufDesc.MiscFlags = 0;
	tBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tVtxInit;
	tVtxInit.pSysMem = &m_vecVertices[0];
	HR(m_pMyGDI->pDevice->CreateBuffer(&tBufDesc, &tVtxInit, &m_pVB));

	// Create Index Buffer
	D3D11_BUFFER_DESC tIdxDesc;
	ZeroMemory(&tIdxDesc, sizeof(D3D11_BUFFER_DESC));
	tIdxDesc.Usage = D3D11_USAGE_DYNAMIC;
	tIdxDesc.ByteWidth = sizeof(UINT) * m_iIdxCnt;
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tIdxDesc.MiscFlags = 0;
	tIdxDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tIdxInit;
	tIdxInit.pSysMem = &m_vecIndices[0];
	HR(m_pMyGDI->pDevice->CreateBuffer(&tIdxDesc, &tIdxInit, &m_pIB));
}

void Engine::CNaviMgr::Render_NaviMesh(void)
{
	//system("cls");
	//cout <<"m_iIdxCnt :"<< m_iIdxCnt << endl;
	//cout << "m_dwIdxCnt :" << m_dwIdxCnt << endl;

	//와이어 프레임
	D3D11_RASTERIZER_DESC dxRasterDesc;
	::memset(&dxRasterDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	dxRasterDesc.FillMode = D3D11_FILL_WIREFRAME;       //와이어
	//dxRasterDesc.FillMode = D3D11_FILL_SOLID;          //아닌
	dxRasterDesc.CullMode = D3D11_CULL_NONE;
	dxRasterDesc.DepthClipEnable = TRUE;

	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);


	m_pMyGDI->pContext->IASetInputLayout(m_pInputLayout);
	m_pMyGDI->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//pVertices[1].vColor = (const DirectX::XMFLOAT4)(DirectX::Colors::Yellow);
	//m_vecVertices[0].vTexC = (const DirectX::XMFLOAT2)(DirectX::Colors::Yellow);
	UINT iStride = sizeof(VTXTEX);
	UINT iOffset = 0;

	m_pMyGDI->pContext->IASetVertexBuffers(0, 1, &m_pVB, &iStride, &iOffset);
	m_pMyGDI->pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	m_pMyGDI->pEffect->SetWorld(XMMatrixIdentity());


	D3DX11_TECHNIQUE_DESC techDesc;
	m_pMyGDI->pTech->GetDesc(&techDesc);

	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight2Tech();

	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		m_pMyGDI->pTech->GetPassByIndex(i)->Apply(0, m_pMyGDI->pContext);

		m_pMyGDI->pContext->DrawIndexed(m_iIdxCnt, 0, 0);
	}
}

Engine::CNaviCell* Engine::CNaviMgr::FindLocation(const XMFLOAT3* _vPos)
{
	// 객체의 x z 좌표를 2차원 벡터에 저장
	XMFLOAT2 vTemp(_vPos->x, _vPos->z);
	XMVECTOR vPos = XMLoadFloat2(&vTemp);

	// 각 삼각형을 순회하면서 각 점마다 내적을 수행.
	// 내적 결과가 모두 음의 값인 삼각형이 객체가 위치한 삼각형이다.
	for (UINT i = 0; i < m_vecNaviMesh.size(); ++i)
	{
		float fDot;
		for (int j = 0; j < LINE_END; ++j)
		{
			XMVECTOR vNormal = XMLoadFloat2(m_vecNaviMesh[i]->GetLine2D(j)->GetNormal());
			XMFLOAT2 vPoint = XMFLOAT2(m_vecNaviMesh[i]->GetPoint()[j].x,
				m_vecNaviMesh[i]->GetPoint()[j].z);
			XMVECTOR vPointToPos = vPos - XMLoadFloat2(&vPoint);
			XMStoreFloat(&fDot, XMVector2Dot(vPointToPos, vNormal));
			if (0 < fDot)
				break;
		}
		//해당 삼각형을 찾았으므로 Cell을 반환하고 함수를 종료한다.
		if (0 >= fDot)
		{
			return m_vecNaviMesh[i];
		}
	}

	// 찾지 못하면 nullptr 반환
	return nullptr;
}

DWORD Engine::CNaviMgr::FindIndex(const XMFLOAT3 * _vPos)
{
	// 객체의 x z 좌표를 2차원 벡터에 저장
	XMFLOAT2 vTemp(_vPos->x, _vPos->z);
	XMVECTOR vPos = XMLoadFloat2(&vTemp);

	// 각 삼각형을 순회하면서 각 점마다 내적을 수행.
	// 내적 결과가 모두 음의 값인 삼각형이 객체가 위치한 삼각형이다.
	for (UINT i = 0; i < m_vecNaviMesh.size(); ++i)
	{
		float fDot;
		for (int j = 0; j < LINE_END; ++j)
		{
			XMVECTOR vNormal = XMLoadFloat2(m_vecNaviMesh[i]->GetLine2D(j)->GetNormal());
			XMFLOAT2 vPoint = XMFLOAT2(m_vecNaviMesh[i]->GetPoint()[j].x,
				m_vecNaviMesh[i]->GetPoint()[j].z);
			XMVECTOR vPointToPos = vPos - XMLoadFloat2(&vPoint);
			XMStoreFloat(&fDot, XMVector2Dot(vPointToPos, vNormal));
			if (0 < fDot)
				break;
		}
		//해당 삼각형을 찾았으므로 Cell을 반환하고 함수를 종료한다.
		if (0 >= fDot)
		{
			//cout << m_vecNaviMesh[i]->GetPoint()[0].y << endl;
			//cout << *fTop << endl;
			//fTop = &m_vecNaviMesh[i]->GetPoint()[0].y;
			return m_vecNaviMesh[i]->GetIndex();
		}
	}

	// 찾지 못하면 nullptr 반환
	//return nullptr;

	return 0;
}

float Engine::CNaviMgr::FindfTop(const XMFLOAT3 * _vPos)
{
	// 객체의 x z 좌표를 2차원 벡터에 저장
	XMFLOAT2 vTemp(_vPos->x, _vPos->z);
	XMVECTOR vPos = XMLoadFloat2(&vTemp);

	// 각 삼각형을 순회하면서 각 점마다 내적을 수행.
	// 내적 결과가 모두 음의 값인 삼각형이 객체가 위치한 삼각형이다.
	for (UINT i = 0; i < m_vecNaviMesh.size(); ++i)
	{
		float fDot;
		for (int j = 0; j < LINE_END; ++j)
		{
			XMVECTOR vNormal = XMLoadFloat2(m_vecNaviMesh[i]->GetLine2D(j)->GetNormal());
			XMFLOAT2 vPoint = XMFLOAT2(m_vecNaviMesh[i]->GetPoint()[j].x,
				m_vecNaviMesh[i]->GetPoint()[j].z);
			XMVECTOR vPointToPos = vPos - XMLoadFloat2(&vPoint);
			XMStoreFloat(&fDot, XMVector2Dot(vPointToPos, vNormal));
			if (0 < fDot)
				break;
		}
		//해당 삼각형을 찾았으므로 Cell을 반환하고 함수를 종료한다.
		if (0 >= fDot)
		{
			//cout << (*_vPos) << endl;

			//cout << m_vecNaviMesh[i]->GetPoint()[0].y << endl;

			//cout << abs(_vPos->y - m_vecNaviMesh[i]->GetPoint()[0].y) << endl;
			if( 4.f> abs(_vPos->y- m_vecNaviMesh[i]->GetPoint()[0].y))
			return m_vecNaviMesh[i]->GetPoint()[0].y;
		}
	}

	// 찾지 못하면 nullptr 반환
	//return nullptr;
	return 0.0f;
}

//## 셀의 세 포인트 중 2개가 겹치는 셀을 찾는다.
void Engine::CNaviMgr::FindNeighbor(void)
{
	for (UINT i = 0; i < m_vecNaviMesh.size() - 1; ++i)
	{
		const XMFLOAT3* pPoint = m_vecNaviMesh[i]->GetPoint();

		for (UINT j = i + 1; j < m_vecNaviMesh.size(); ++j)
		{
			int iCnt = 0;

			for (int k = 0; k < POINT_END; ++k)
			{
				for (int l = 0; l < POINT_END; ++l)
				{
					if (pPoint[k].x == m_vecNaviMesh[j]->GetPoint()[l].x &&
						pPoint[k].z == m_vecNaviMesh[j]->GetPoint()[l].z)
						++iCnt;
				}
			}

			if (iCnt == 2 || iCnt == 1) // 점 2개가 겹치면 이웃
			{
				m_vecNaviMesh[i]->GetvecNeighbor()->push_back(m_vecNaviMesh[j]);
				m_vecNaviMesh[j]->GetvecNeighbor()->push_back(m_vecNaviMesh[i]);
			}
		}
	}
}

DWORD Engine::CNaviMgr::MoveOnNaviMesh(XMFLOAT3 * pPos, const XMFLOAT3 * pDir, const DWORD & dwCurrentIdx, float & pTop)
{
	NEIGHBOR	eNeighborID;
	DWORD		dwNextIndex = dwCurrentIdx;

	if (m_vecNaviMesh[dwCurrentIdx]->CheckPass(pPos, pDir, &eNeighborID))
	{
		CNaviCell*	pNeighbor = nullptr;

		pNeighbor = m_vecNaviMesh[dwCurrentIdx]->GetNeighbor(eNeighborID);

		if (pNeighbor == nullptr)
		{
			//슬라이딩벡터
			//*(-pPos)* += *pDir;
		}
		else
		{

			dwNextIndex = pNeighbor->GetIndex();


			XMVECTOR VP = XMLoadFloat3(pPos);
			XMVECTOR VD = XMLoadFloat3(pDir);

			VP += VD;
			XMStoreFloat3(pPos, VP);
			//if (pPos->y <= pTop)
			pPos->y = (-pNeighbor->m_pPlane.x * pPos->x - pNeighbor->m_pPlane.z * pPos->z - pNeighbor->m_pPlane.w) / pNeighbor->m_pPlane.y;

			//*pPos += *pDir;


		}
	}
	else
	{

		XMVECTOR VP = XMLoadFloat3(pPos);
		XMVECTOR VD = XMLoadFloat3(pDir);

		VP += VD;
		XMStoreFloat3(pPos, VP);
		//if (pPos->y <= pTop)
		pPos->y = ((-m_vecNaviMesh[dwCurrentIdx]->m_pPlane.x * pPos->x - m_vecNaviMesh[dwCurrentIdx]->m_pPlane.z * pPos->z - m_vecNaviMesh[dwCurrentIdx]->m_pPlane.w) / m_vecNaviMesh[dwCurrentIdx]->m_pPlane.y);
		//*pPos += *pDir;
	}
	//cout << (-m_vecNaviMesh[dwCurrentIdx]->m_pPlane.x * pPos->x - m_vecNaviMesh[dwCurrentIdx]->m_pPlane.z * pPos->z - m_vecNaviMesh[dwCurrentIdx]->m_pPlane.w) / m_vecNaviMesh[dwCurrentIdx]->m_pPlane.y << endl;


	return dwNextIndex;
}

void Engine::CNaviMgr::LinkCell(void)
{
	VECCELL::iterator	iter = m_vecNaviMesh.begin();
	if (m_vecNaviMesh.end() == iter)
		return;

	for (; iter != m_vecNaviMesh.end(); ++iter)
	{
		VECCELL::iterator	iter_Target = m_vecNaviMesh.begin();
		while (iter_Target != m_vecNaviMesh.end())
		{
			if (iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if ((*iter_Target)->ComparePoint((*iter)->GetPoint(POINT_A)
				, (*iter)->GetPoint(POINT_B)
				, (*iter)))
			{
				(*iter)->SetNeighbor(NEIGHBOR_AB, (*iter_Target));
			}

			else if ((*iter_Target)->ComparePoint((*iter)->GetPoint(POINT_B)
				, (*iter)->GetPoint(POINT_C)
				, (*iter)))
			{
				(*iter)->SetNeighbor(NEIGHBOR_BC, (*iter_Target));
			}

			else if ((*iter_Target)->ComparePoint((*iter)->GetPoint(POINT_C)
				, (*iter)->GetPoint(POINT_A)
				, (*iter)))
			{
				(*iter)->SetNeighbor(NEIGHBOR_CA, (*iter_Target));
			}
			++iter_Target;
		}
	}
}

void Engine::CNaviMgr::Release(void)
{
	for_each(m_vecNaviMesh.begin(), m_vecNaviMesh.end(), CDeleteObj());
	m_vecNaviMesh.clear();
}

bool Engine::CNaviMgr::DeleteCell()
{
	//cout << m_dwIdxCnt << endl;
	if (m_vecNaviMesh.size() > 0)
	{
		m_dwIdxCnt -= 1;
		m_iIdxCnt -= (3 * m_dwIdxCnt + 3);
	
	

		Safe_Delete(m_vecNaviMesh[m_vecNaviMesh.size() - 1]);
		m_vecNaviMesh.pop_back();
		
		//m_vecVertices.pop_back();
		//m_vecIndices.pop_back();

		//m_iIdxCnt -= 1.5f* m_iIdxCnt*(m_iIdxCnt +1);
		//m_iIdxCnt
		return true;
	}
	return false;

}
