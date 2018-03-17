#include "CubeColor.h"
#include "Effects.h"

Engine::CCubeColor::CCubeColor(MYGDI* pMyGDI)
	:CVIBuffer(pMyGDI)
{
	m_pTech = m_pMyGDI->pEffect->GetColorTech();
}

Engine::CCubeColor::~CCubeColor()
{
}

HRESULT Engine::CCubeColor::CreateBuffer(const XMVECTOR& MIN,
	const XMVECTOR& MAX, const XMFLOAT4& dwColor)
{
	//Create the Vertex input layout
	D3D11_INPUT_ELEMENT_DESC vtxDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	D3DX11_PASS_DESC passDesc;

	m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vtxDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));

	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 8;

	m_dwIdxSize = sizeof(UINT);
	m_dwIndexCnt = 36;

	XMFLOAT3 vMin, vMax;
	XMStoreFloat3(&vMin, MIN);
	XMStoreFloat3(&vMax, MAX);

	VTXCOL pVertices[8] = {};

	pVertices[0].vPos = DirectX::XMFLOAT3(vMin.x, vMax.y, vMin.z);
	pVertices[0].vColor = dwColor;

	pVertices[1].vPos = DirectX::XMFLOAT3(vMax.x, vMax.y, vMin.z);
	pVertices[1].vColor = dwColor;

	pVertices[2].vPos = DirectX::XMFLOAT3(vMax.x, vMin.y, vMin.z);
	pVertices[2].vColor = dwColor;

	pVertices[3].vPos = DirectX::XMFLOAT3(vMin.x, vMin.y, vMin.z);
	pVertices[3].vColor = dwColor;

	pVertices[4].vPos = DirectX::XMFLOAT3(vMin.x, vMax.y, vMax.z);
	pVertices[4].vColor = dwColor;

	pVertices[5].vPos = DirectX::XMFLOAT3(vMax.x, vMax.y, vMax.z);
	pVertices[5].vColor = dwColor;

	pVertices[6].vPos = DirectX::XMFLOAT3(vMax.x, vMin.y, vMax.z);
	pVertices[6].vColor = dwColor;

	pVertices[7].vPos = DirectX::XMFLOAT3(vMin.x, vMin.y, vMax.z);
	pVertices[7].vColor = dwColor;

	D3D11_BUFFER_DESC tBufDesc;
	ZeroMemory(&tBufDesc, sizeof(D3D11_BUFFER_DESC));
	tBufDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tBufDesc.ByteWidth = m_dwVtxSize * m_dwVtxCnt;
	tBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tVtxInit;
	tVtxInit.pSysMem = pVertices;
	HR(m_pMyGDI->pDevice->CreateBuffer(&tBufDesc, &tVtxInit, &m_pVB));

	////
	//## Create Index Buffer
	m_dwIndexCnt = 36; // 삼각형의 개수 * 삼각형 1개당 버텍스 수

	UINT pIndices[] = {
		// +x
		1, 5, 6,
		1, 6, 2,
		// -x
		4, 0, 3,
		4, 3, 7,
		// +y
		4, 5, 1,
		4, 1, 0,
		// -y
		3, 2, 6,
		3, 6, 7,
		// +z
		7, 6, 5,
		7, 5, 4,
		// -z
		0, 1, 2,
		0, 2, 3
	};

	D3D11_BUFFER_DESC tIdxDesc;
	ZeroMemory(&tIdxDesc, sizeof(D3D11_BUFFER_DESC));
	tIdxDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tIdxDesc.ByteWidth = m_dwIdxSize * m_dwIndexCnt;
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tIdxInit;
	tIdxInit.pSysMem = pIndices;
	HR(m_pMyGDI->pDevice->CreateBuffer(&tIdxDesc, &tIdxInit, &m_pIB));

	return S_OK;
}

Engine::CResources * Engine::CCubeColor::CloneResource(void)
{
	CResources* pResources = new CCubeColor(*this);

	++m_dwRefCnt;

	return pResources;
}

Engine::CCubeColor * Engine::CCubeColor::Create(MYGDI* pMyGDI, XMVECTOR& MIN, XMVECTOR& MAX, 
	XMFLOAT4& vColor)
{
	CCubeColor* pBuffer = new CCubeColor(pMyGDI);
	if (FAILED(pBuffer->CreateBuffer(MIN, MAX, vColor)))
	{
		Safe_Delete(pBuffer);
	}

	return pBuffer;
}
