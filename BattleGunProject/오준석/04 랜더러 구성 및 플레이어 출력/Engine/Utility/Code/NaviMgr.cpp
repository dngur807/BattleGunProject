#include "NaviMgr.h"
#include "NaviCell.h"
#include "Effects.h"
#include "Line2D.h"

IMPLEMENT_SINGLETON(Engine::CNaviMgr)

Engine::CNaviMgr::CNaviMgr()
	:m_pMyGDI(nullptr), 
	m_iReservedSize(0), 
	m_iIdxCnt(0)
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

HRESULT Engine::CNaviMgr::AddCell(const XMFLOAT3 * pPointA, const XMFLOAT3 * pPointB, const XMFLOAT3 * pPointC)
{
	CNaviCell* pCell = CNaviCell::Create(m_pMyGDI,
		pPointA, pPointB, pPointC, m_iIdxCnt);
	NULL_CHECK_RETURN(pCell, E_FAIL);

	//m_iIdxCnt += 3;

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
	m_pMyGDI->pContext->IASetInputLayout(m_pInputLayout);
	m_pMyGDI->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT iStride = sizeof(VTXTEX);
	UINT iOffset = 0;
	m_pMyGDI->pContext->IASetVertexBuffers(0, 1, &m_pVB, &iStride, &iOffset);
	m_pMyGDI->pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	m_pMyGDI->pEffect->SetWorld(XMMatrixIdentity());

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pMyGDI->pTech->GetDesc(&techDesc);

	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		m_pMyGDI->pTech->GetPassByIndex(i)->Apply(0, m_pMyGDI->pContext);

		m_pMyGDI->pContext->DrawIndexed(m_iIdxCnt, 0, 0);
	}
}

Engine::CNaviCell* Engine::CNaviMgr::FindLocation(const XMFLOAT3* _vPos)
{
	// ��ü�� x z ��ǥ�� 2���� ���Ϳ� ����
	XMFLOAT2 vTemp(_vPos->x, _vPos->z);
	XMVECTOR vPos = XMLoadFloat2(&vTemp);

	// �� �ﰢ���� ��ȸ�ϸ鼭 �� ������ ������ ����.
	// ���� ����� ��� ���� ���� �ﰢ���� ��ü�� ��ġ�� �ﰢ���̴�.
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
		//�ش� �ﰢ���� ã�����Ƿ� Cell�� ��ȯ�ϰ� �Լ��� �����Ѵ�.
		if(0 >= fDot)
		{
			return m_vecNaviMesh[i];
		}
	}
	
	// ã�� ���ϸ� nullptr ��ȯ
	return nullptr;
}

//## ���� �� ����Ʈ �� 2���� ��ġ�� ���� ã�´�.
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

			if (iCnt == 2 || iCnt == 1) // �� 2���� ��ġ�� �̿�
			{
				m_vecNaviMesh[i]->GetvecNeighbor()->push_back(m_vecNaviMesh[j]);
				m_vecNaviMesh[j]->GetvecNeighbor()->push_back(m_vecNaviMesh[i]);
			}
		}
	}
}

void Engine::CNaviMgr::Release(void)
{
	for_each(m_vecNaviMesh.begin(), m_vecNaviMesh.end(), CDeleteObj());
	m_vecNaviMesh.clear();
}
