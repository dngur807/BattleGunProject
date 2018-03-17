#include "RcTex.h"
#include "Texture.h"
#include "Effects.h"
#include "Vertex.h"


Engine::CRcTex::CRcTex(MYGDI* pMyGDI)
	:CVIBuffer(pMyGDI)
{
}

//Engine::CRcTex::CRcTex(const CRcTex & rhs)
//	:CVIBuffer(rhs)
//{
//}


Engine::CRcTex::~CRcTex()
{
}

Engine::CResources * Engine::CRcTex::CloneResource(void)
{
	++m_dwRefCnt;

	CResources* pResources = new CRcTex(*this);

	return pResources;
}

HRESULT Engine::CRcTex::CreateBuffer(TCHAR* szFullPath)
{
	//Create the Vertex input layout
	D3D11_INPUT_ELEMENT_DESC vtxDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;

	D3DX11_PASS_DESC passDesc;

	m_pMyGDI->pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	//m_pMyGDI->pEffect->GetDefaultTech()->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vtxDesc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout));

	VTXTEX pVertices[4] = {};

	pVertices[0].vPos = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.f);
	pVertices[0].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[0].vTexC = DirectX::XMFLOAT2(0.f, 0.f);

	pVertices[1].vPos = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.f);
	pVertices[1].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[1].vTexC = DirectX::XMFLOAT2(1.f, 0.f);

	pVertices[2].vPos = DirectX::XMFLOAT3(0.5f, 0.5f, 0.f);
	pVertices[2].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[2].vTexC = DirectX::XMFLOAT2(1.f, 1.f);

	pVertices[3].vPos = DirectX::XMFLOAT3(0.5f, -0.5f, 0.f);
	pVertices[3].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[3].vTexC = DirectX::XMFLOAT2(0.f, 1.f);



	D3D11_BUFFER_DESC tBufDesc;
	ZeroMemory(&tBufDesc, sizeof(D3D11_BUFFER_DESC));
	tBufDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tBufDesc.ByteWidth = m_dwVtxSize * m_dwVtxCnt;
	tBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tBufDesc.CPUAccessFlags = 0;
	tBufDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA tVtxInit;
	tVtxInit.pSysMem = pVertices;
	HR(m_pMyGDI->pDevice->CreateBuffer(&tBufDesc, &tVtxInit, &m_pVB));

	////
	//## Create Index Buffer
	m_dwIdxSize = sizeof(UINT);
	m_dwIndexCnt = 6; // 삼각형의 개수 * 삼각형 1개당 버텍스 수

	UINT pIndices[] = {
		// front
		0, 1, 2,
		0, 2, 3
	};

	D3D11_BUFFER_DESC tIdxDesc;
	ZeroMemory(&tIdxDesc, sizeof(D3D11_BUFFER_DESC));
	tIdxDesc.Usage = D3D11_USAGE_DEFAULT;
	tIdxDesc.ByteWidth = m_dwIdxSize * m_dwIndexCnt;
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIdxDesc.CPUAccessFlags = 0;
	tIdxDesc.MiscFlags = 0;
	tIdxDesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA tIdxInit;
	tIdxInit.pSysMem = pIndices;
	HR(m_pMyGDI->pDevice->CreateBuffer(&tIdxDesc, &tIdxInit, &m_pIB));


	return S_OK;
}

void Engine::CRcTex::Render(void)
{
	m_pMyGDI->pContext->IASetInputLayout(m_pInputLayout);
	m_pMyGDI->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Engine::Basic32);
	UINT offset = 0;
}

Engine::CRcTex * Engine::CRcTex::Create(MYGDI* pMyGDI)
{
	CRcTex* pBuffer = new CRcTex(pMyGDI);
	if (FAILED(pBuffer->CreateBuffer()))
	{
		Safe_Delete(pBuffer);
	}

	return pBuffer;
}
