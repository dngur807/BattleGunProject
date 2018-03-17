#include "CubeTex.h"
#include "Effects.h"

Engine::CCubeTex::CCubeTex(MYGDI * pMyGDI)
	:CVIBuffer(pMyGDI)
{
	m_pTech = m_pMyGDI->pEffect->GetCubeTech();
	//m_pTech = m_pMyGDI->pEffect->GetLight1Tech();
}

Engine::CCubeTex::~CCubeTex()
{
}

HRESULT Engine::CCubeTex::CreateBuffer(void)
{
	//Create the Vertex input layout
	D3D11_INPUT_ELEMENT_DESC vtxDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
		D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxCnt = 8;

	D3DX11_PASS_DESC passDesc;

	m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vtxDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));

	VTXCUBE pVertices[8] = {};

	pVertices[0].vPos = DirectX::XMFLOAT3(-1.f, -1.f, -1.f);
	pVertices[0].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[0].vTexC = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

	pVertices[1].vPos = DirectX::XMFLOAT3(-1.f, 1.f, -1.f);
	pVertices[1].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[1].vTexC = DirectX::XMFLOAT3(0.f, 1.f, 0.f);

	pVertices[2].vPos = DirectX::XMFLOAT3(1.f, 1.f, -1.f);
	pVertices[2].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[2].vTexC = DirectX::XMFLOAT3(1.f, 1.f, 0.f);

	pVertices[3].vPos = DirectX::XMFLOAT3(1.f, -1.f, -1.f);
	pVertices[3].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[3].vTexC = DirectX::XMFLOAT3(1.f, 0.f, 0.f);

	pVertices[4].vPos = DirectX::XMFLOAT3(-1.f, -1.f, 1.f);
	pVertices[4].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[4].vTexC = DirectX::XMFLOAT3(0.f, 0.f, 1.f);

	pVertices[5].vPos = DirectX::XMFLOAT3(-1.f, 1.f, 1.f);
	pVertices[5].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[5].vTexC = DirectX::XMFLOAT3(0.f, 1.f, 1.f);

	pVertices[6].vPos = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	pVertices[6].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[6].vTexC = DirectX::XMFLOAT3(1.f, 1.f, 1.f);

	pVertices[7].vPos = DirectX::XMFLOAT3(1.f, -1.f, 1.f);
	pVertices[7].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
	pVertices[7].vTexC = DirectX::XMFLOAT3(1.f, 0.f, 1.f);

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
	m_dwIndexCnt = 36; // 삼각형의 개수 * 삼각형 1개당 버텍스 수

	UINT pIndices[] = {
		// front
		0, 1, 2,
		0, 2, 3,
		//back
		4, 6, 5,
		4, 7, 6,
		//left
		4, 5, 1,
		4, 1, 0,
		//right 
		3, 2, 6,
		3, 6, 7,
		//up
		1, 5, 6,
		1, 6, 2,
		//down 
		4, 0, 3,
		4, 3, 7
	};

	//UINT pIndices[] = {
	//	// +x
	//	1, 5, 6,
	//	1, 6, 2,
	//	// -x
	//	4, 0, 3,
	//	4, 3, 7,
	//	// +y
	//	4, 5, 1,
	//	4, 1, 0,
	//	// -y
	//	3, 2, 6,
	//	3, 6, 7,
	//	// +z
	//	7, 6, 5,
	//	7, 5, 4,
	//	// -z
	//	0, 1, 2,
	//	0, 2, 3
	//};

	D3D11_BUFFER_DESC tIdxDesc;
	ZeroMemory(&tIdxDesc, sizeof(D3D11_BUFFER_DESC));
	tIdxDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tIdxDesc.ByteWidth = m_dwIdxSize * m_dwIndexCnt;
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIdxDesc.CPUAccessFlags = 0;
	tIdxDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA tIdxInit;
	tIdxInit.pSysMem = pIndices;
	HR(m_pMyGDI->pDevice->CreateBuffer(&tIdxDesc, &tIdxInit, &m_pIB));

	return S_OK;
}

Engine::CResources * Engine::CCubeTex::CloneResource(void)
{
	++m_dwRefCnt;

	CResources* pResources = new CCubeTex(*this);

	return pResources;
}

Engine::CCubeTex * Engine::CCubeTex::Create(MYGDI * pMyGDI)
{
	CCubeTex* pBuffer = new CCubeTex(pMyGDI);
	if (FAILED(pBuffer->CreateBuffer()))
	{
		Safe_Delete(pBuffer);
	}

	return pBuffer;
}
