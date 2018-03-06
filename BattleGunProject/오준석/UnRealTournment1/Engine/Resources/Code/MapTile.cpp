#include "MapTile.h"


Engine::CMapTile::CMapTile(MYGDI* pMyGDI)
	:CVIBuffer(pMyGDI)
{
	m_pTech = pMyGDI->pTech;
}

//Engine::CMapTile::CMapTile(const CMapTile & rhs)
//	: CVIBuffer(rhs)
//{
//}

Engine::CMapTile::~CMapTile()
{
	if (m_dwRefCnt == 0)
		Safe_Delete_Array(m_pVertex);
}

HRESULT Engine::CMapTile::CreateBuffer(const float & fWidth, 
	const float & fDepth, const UINT & iCntX, const UINT & iCntZ)
{
	MESHINFO tMeshInfo;

	CreateGrid(fWidth, fDepth, iCntX, iCntZ, tMeshInfo);

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = tMeshInfo.Vertices.size();
	m_dwIndexCnt = tMeshInfo.Indices.size();
	m_dwIdxSize = sizeof(UINT);
	// 필요한 정점 특성들을 추출하고, 각 정점에 높이 함수를 적용한다.
	HANDLE hFile;
	DWORD dwByte;

	hFile = CreateFile(L"../bin/Resources/Texture/Terrain/Terrain0.png",
		GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	DWORD* pdwPixel = new DWORD[ih.biWidth * ih.biHeight];

	ReadFile(hFile, pdwPixel, sizeof(DWORD) * ih.biWidth * ih.biHeight,
		&dwByte, NULL);
	CloseHandle(hFile);

	//std::vector<VTXTEX> vecVtxCol(tMeshInfo.Vertices.size());
	m_pVertex = new VTXTEX[tMeshInfo.Vertices.size()];

	for (size_t i = 0; i < tMeshInfo.Vertices.size(); ++i)
	{
		DirectX::XMFLOAT3 vPos = tMeshInfo.Vertices[i].vPos;
		vPos.y = (pdwPixel[i] & 0x000000FF) / 1000.f;

		m_pVertex[i].vPos = vPos;
		m_pVertex[i].vNormal = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
		m_pVertex[i].vTexC = tMeshInfo.Vertices[i].vTexC;
	}

	// Create the Vertex input Layout
	D3D11_INPUT_ELEMENT_DESC vtxDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
			D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
			D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//// Create the vertex input layout.
	//D3D11_INPUT_ELEMENT_DESC vtxDesc[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, 
	//	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, 
	//	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};


	D3DX11_PASS_DESC passDesc;

	m_pMyGDI->pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vtxDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));


	//## Create Vertex Buffer
	D3D11_BUFFER_DESC tBufDesc;
	ZeroMemory(&tBufDesc, sizeof(D3D11_BUFFER_DESC));
	tBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufDesc.ByteWidth = sizeof(VTXTEX) * tMeshInfo.Vertices.size();
	tBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tBufDesc.MiscFlags = 0;
	tBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tVtxInit;
	tVtxInit.pSysMem = &m_pVertex[0];
	HR(m_pMyGDI->pDevice->CreateBuffer(&tBufDesc, &tVtxInit, &m_pVB));

	// Create Index Buffer
	D3D11_BUFFER_DESC tIdxDesc;
	ZeroMemory(&tIdxDesc, sizeof(D3D11_BUFFER_DESC));
	tIdxDesc.Usage = D3D11_USAGE_DYNAMIC;
	tIdxDesc.ByteWidth = sizeof(UINT) * m_dwIndexCnt;
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tIdxDesc.MiscFlags = 0;
	tIdxDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tIdxInit;
	tIdxInit.pSysMem = &tMeshInfo.Indices[0];
	HR(m_pMyGDI->pDevice->CreateBuffer(&tIdxDesc, &tIdxInit, &m_pIB));

	Safe_Delete_Array(pdwPixel);

	return S_OK;
}

float Engine::CMapTile::GetHillHeight(float x, float z) const
{
	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));

}

DirectX::XMFLOAT3 Engine::CMapTile::GetHillNormal(float x, float z) const
{
	// n = (-df/dx, 1, -df/dz)
	DirectX::XMFLOAT3 n(
		-0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
		1.0f,
		-0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));

	DirectX::XMVECTOR unitNormal = DirectX::XMVector3Normalize(XMLoadFloat3(&n));
	DirectX::XMStoreFloat3(&n, unitNormal);

	return n;
}

Engine::CResources * Engine::CMapTile::CloneResource(void)
{
	++m_dwRefCnt;

	CResources* pResources = new CMapTile(*this);

	return pResources;
}

Engine::CMapTile * Engine::CMapTile::Create(MYGDI* pMyGDI,
	const float & fWidth, const float & fDepth, 
	const UINT & iCntX, const UINT & iCntZ)
{
	CMapTile* pBuffer = new CMapTile(pMyGDI);
	if (FAILED(pBuffer->CreateBuffer(fWidth, fDepth, iCntX, iCntZ)))
	{
		Safe_Delete(pBuffer);
	}

	return pBuffer;
}
