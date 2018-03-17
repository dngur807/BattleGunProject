#include "RcColor.h"


Engine::CRcColor::CRcColor(MYGDI* pMyGDI)
	:CVIBuffer(pMyGDI)
{
}


Engine::CRcColor::~CRcColor()
{
	Release();
}

Engine::CResources * Engine::CRcColor::CloneResource(void)
{
	CResources* pResources = new CRcColor(*this);

	++m_dwRefCnt;

	return pResources;
}

HRESULT Engine::CRcColor::CreateBuffer(void)
{
	// Create the Vertex input Layout
	D3D11_INPUT_ELEMENT_DESC vtxDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, 
		D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, 
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3DX11_PASS_DESC passDesc;

	m_pMyGDI->pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vtxDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));

	VTXCOL pVertices[4] = {};

	pVertices[0].vPos = DirectX::XMFLOAT3(-1.f, 1.f, 0.f);
	pVertices[0].vColor = (const DirectX::XMFLOAT4)(DirectX::Colors::Red);

	pVertices[1].vPos = DirectX::XMFLOAT3(1.f, 1.f, 0.f);
	pVertices[1].vColor = (const DirectX::XMFLOAT4)(DirectX::Colors::Yellow);

	pVertices[2].vPos = DirectX::XMFLOAT3(1.f, -1.f, 0.f);
	pVertices[2].vColor = (const DirectX::XMFLOAT4)(DirectX::Colors::Green);

	pVertices[3].vPos = DirectX::XMFLOAT3(-1.f, -1.f, 0.f);
	pVertices[3].vColor = (const DirectX::XMFLOAT4)(DirectX::Colors::Blue);

	D3D11_BUFFER_DESC tBufDesc;
	tBufDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tBufDesc.ByteWidth = sizeof(VTXCOL) * 4;
	tBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tBufDesc.CPUAccessFlags = 0;
	tBufDesc.MiscFlags = 0;
	tBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tVtxInit;
	tVtxInit.pSysMem = pVertices;
	HR(m_pMyGDI->pDevice->CreateBuffer(&tBufDesc, &tVtxInit, &m_pVB));

	/*INDEX32 pIndices[2] = {};

	pIndices[0] = { 0, 2, 3 };
	pIndices[1] = { 0, 1, 2 };*/

	m_dwIndexCnt = 6;

	UINT pIndices[] = {
		// front face
		0, 2, 3,
		0, 1, 2
	};

	D3D11_BUFFER_DESC tIdxDesc;

	tIdxDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tIdxDesc.ByteWidth = sizeof(UINT) * 6;
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIdxDesc.CPUAccessFlags = 0;
	tIdxDesc.MiscFlags = 0;
	tIdxDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tIdxInit;
	tIdxInit.pSysMem = pIndices;
	HR(m_pMyGDI->pDevice->CreateBuffer(&tIdxDesc, &tIdxInit, &m_pIB));

	return S_OK;
}

Engine::CRcColor * Engine::CRcColor::Create(MYGDI* pMyGDI)
{
	CRcColor* pBuffer = new CRcColor(pMyGDI);
	if (FAILED(pBuffer->CreateBuffer()))
	{
		Safe_Delete(pBuffer);
	}

	return pBuffer;
}
