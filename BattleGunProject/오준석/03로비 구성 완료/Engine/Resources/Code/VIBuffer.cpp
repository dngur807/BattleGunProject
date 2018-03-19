#include "VIBuffer.h"
#include "Effects.h"

Engine::CVIBuffer::CVIBuffer(MYGDI* pMyGDI)
	:CResources(pMyGDI),
	m_pVB(nullptr),
	m_dwVtxSize(0),
	m_dwVtxCnt(0),
	m_pIB(nullptr),
	m_dwIdxSize(0),
	m_dwIndexCnt(0),
	m_pInputLayout(nullptr),
	m_fPhi(DirectX::XM_PI * 0.25f),
	m_fTheta(DirectX::XM_PI * 1.5f),
	m_fRad(5.f)
{
	DirectX::XMMATRIX I = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&mWorld, I);
	DirectX::XMStoreFloat4x4(&mView, I);
	DirectX::XMStoreFloat4x4(&mProj, I);

}

//Engine::CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
//	:CResources(rhs)
//{
//}


Engine::CVIBuffer::~CVIBuffer()
{
	Release();
}

HRESULT Engine::CVIBuffer::CreateBuffer(void)
{
	return S_OK;
}

void Engine::CVIBuffer::Render(XMFLOAT4X4& rmatWorld)
{
	m_pMyGDI->pContext->IASetInputLayout(m_pInputLayout);
	m_pMyGDI->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//UINT iStride = m_dwVtxSize;
	UINT iOffset = 0;
	m_pMyGDI->pContext->IASetVertexBuffers(0, 1, &m_pVB, &m_dwVtxSize, &iOffset);
	m_pMyGDI->pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	m_pMyGDI->pEffect->SetWorld(XMLoadFloat4x4(&rmatWorld));
	//m_pMyGDI->pEffect->SetWorld(XMLoadFloat4x4(&rmatWorld));
	//m_pMyGDI->pEffect->SetDiffuseMap(*pTexture);

	//m_pTech->GetDesc(&techDesc);

	//for (UINT i = 0; i < techDesc.Passes; ++i)
	//{
	//	m_pTech->GetPassByIndex(i)->Apply(0, m_pMyGDI->pContext);

	//	m_pMyGDI->pContext->DrawIndexed(m_dwIndexCnt, 0, 0);
	//}

	//*종찬이형이 짜준거
	D3DX11_TECHNIQUE_DESC techDesc;
	ZeroMemory(&techDesc, sizeof(D3DX11_TECHNIQUE_DESC));
	m_pMyGDI->pTech->GetDesc(&techDesc);


	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		m_pMyGDI->pTech->GetPassByIndex(i)->Apply(0, m_pMyGDI->pContext);

		m_pMyGDI->pContext->DrawIndexed(m_dwIndexCnt, 0, 0);
	}
}

void Engine::CVIBuffer::GetVtxInfo(void * pVtxInfo)
{
	/*D3D11_MAPPED_SUBRESOURCE SubResource;
	m_pMyGDI->pContext->Map(m_pVB, 0, D3D11_MAP_READ, 0, &SubResource);
	memcpy(pVtxInfo, SubResource.pData, m_dwVtxSize * m_dwVtxCnt);
	m_pMyGDI->pContext->Unmap(m_pVB, 0);*/
}

void Engine::CVIBuffer::SetVtxInfo(void * pVtxInfo)
{
}

void Engine::CVIBuffer::SetIdxInfo(void * pIdxInfo, const UINT * pIndexCnt)
{
	m_dwIndexCnt = *pIndexCnt;

	D3D11_MAPPED_SUBRESOURCE SubResource;
	m_pMyGDI->pContext->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);
	memcpy(SubResource.pData, pIdxInfo, (*pIndexCnt) * m_dwIdxSize);
	m_pMyGDI->pContext->Unmap(m_pIB, 0);
}

DWORD Engine::CVIBuffer::Release(void)
{
	if (m_dwRefCnt == 0)
	{
		Safe_Release(m_pVB);
		Safe_Release(m_pIB);
		return 0;
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

void Engine::CVIBuffer::CreateGrid(float fWidth, float fDepth, UINT m, UINT n, MESHINFO & MeshInfo)
{
	UINT iVertexCount = m * n;	// Number of Vertices
	UINT iFaceCount = (m - 1) * (n - 1) * 2; // Number of Triangle

											 //## Create Vertices
	float fHalfWidth = 0.5f * fWidth;
	float fHalfDepth = 0.5f * fDepth;

	float fdx = fWidth / (m - 1);
	float fdz = fDepth / (n - 1);

	float fdu = 1.f / (m - 1);
	float fdv = 1.f / (n - 1);

	MeshInfo.Vertices.resize(iVertexCount);
	for (UINT i = 0; i < n; ++i)
	{
		float fz = i * fdz;
		for (UINT j = 0; j < m; ++j)
		{
			float fx = j * fdx;

			MeshInfo.Vertices[j + i * m].vPos = DirectX::XMFLOAT3(fx, 0.f, fz);

			//조명에 쓰이는 특성들
			MeshInfo.Vertices[j + i * m].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
			MeshInfo.Vertices[j + i * m].vTanU = DirectX::XMFLOAT3(1.f, 0.f, 0.f);

			//텍스처 적용에 쓰이는 특성들
			MeshInfo.Vertices[j + i * m].vTexC.x = j * fdu;
			MeshInfo.Vertices[j + i * m].vTexC.y = i * fdv;
		}
	}

	//## Create Indices
	MeshInfo.Indices.resize(iFaceCount * 3); // 삼각형 개수 * 개당 인덱스 수
											 // 각 낱칸을 훑으면서 색인 계산
	UINT k = 0;
	for (UINT i = 0; i < n - 1; ++i)
	{
		for (UINT j = 0; j < m - 1; ++j)
		{
			MeshInfo.Indices[k] = j + m * (i + 1);
			MeshInfo.Indices[k + 1] = j + m * (i + 1) + 1;
			MeshInfo.Indices[k + 2] = j + m * i + 1;

			MeshInfo.Indices[k + 3] = j + m * (i + 1);
			MeshInfo.Indices[k + 4] = j + m * i + 1;
			MeshInfo.Indices[k + 5] = j + m * i;
			k += 6;
		}
	}
}