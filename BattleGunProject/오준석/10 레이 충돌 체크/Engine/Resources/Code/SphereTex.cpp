#include "SphereTex.h"
#include "Effects.h"
#include "MathHelper.h"

Engine::CSphereTex::CSphereTex(MYGDI * pMyGDI)
	:CVIBuffer(pMyGDI)
{
	m_pTech = m_pMyGDI->pEffect->GetCubeTech();
}

Engine::CSphereTex::~CSphereTex()
{
}

HRESULT Engine::CSphereTex::CreateBuffer(float radius , UINT numSubdivisions)
{

	numSubdivisions = MathHelper::Min(numSubdivisions, 5u);

	const float X = 0.525731f;
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] =
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};
	m_MeshInfo.Vertices.resize(12);
	m_MeshInfo.Indices.resize(60);

	for (UINT i = 0; i < 12; ++i)
		m_MeshInfo.Vertices[i].vPos = pos[i];

	for (UINT i = 0; i < 60; ++i)
		m_MeshInfo.Indices[i] = k[i];
	for (UINT i = 0; i < numSubdivisions; ++i)
		Subdivide(m_MeshInfo);

	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < m_MeshInfo.Vertices.size(); ++i)
	{
		// Project onto unit sphere.
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&m_MeshInfo.Vertices[i].vPos));

		// Project onto sphere.
		XMVECTOR p = radius*n;

		XMStoreFloat3(&m_MeshInfo.Vertices[i].vPos, p);
		XMStoreFloat3(&m_MeshInfo.Vertices[i].vNormal, n);

		// Derive texture coordinates from spherical coordinates.
		float theta = AngleFromXY(
			m_MeshInfo.Vertices[i].vPos.x,
			m_MeshInfo.Vertices[i].vPos.z);

		float phi = acosf(m_MeshInfo.Vertices[i].vPos.y / radius);

		m_MeshInfo.Vertices[i].vTexC.x = theta / XM_2PI;
		m_MeshInfo.Vertices[i].vTexC.y = phi / XM_PI;

		// Partial derivative of P with respect to theta
		m_MeshInfo.Vertices[i].vTanU.x = -radius*sinf(phi)*sinf(theta);
		m_MeshInfo.Vertices[i].vTanU.y = 0.0f;
		m_MeshInfo.Vertices[i].vTanU.z = +radius*sinf(phi)*cosf(theta);

		XMVECTOR T = XMLoadFloat3(&m_MeshInfo.Vertices[i].vTanU);
		XMStoreFloat3(&m_MeshInfo.Vertices[i].vTanU, XMVector3Normalize(T));
	}
	/*vector<VTXCOL>	vertices(m_MeshInfo.Vertices.size());
	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

	for (size_t i = 0; i < m_MeshInfo.Vertices.size(); ++i)
	{
		vertices[i].vPos = m_MeshInfo.Vertices[i].vPos;
		vertices[i].vColor = black;
	}*/

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

	m_dwVtxSize = sizeof(tagVertexTotal);
	m_dwVtxCnt = m_MeshInfo.Vertices.size();

	D3DX11_PASS_DESC passDesc;

	m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vtxDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));

	D3D11_BUFFER_DESC tBufDesc;
	ZeroMemory(&tBufDesc, sizeof(D3D11_BUFFER_DESC));
	tBufDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tBufDesc.ByteWidth = m_dwVtxSize * m_dwVtxCnt;
	tBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tBufDesc.CPUAccessFlags = 0;
	tBufDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA tVtxInit;
	tVtxInit.pSysMem =	&m_MeshInfo.Vertices[0];
	HR(m_pMyGDI->pDevice->CreateBuffer(&tBufDesc, &tVtxInit, &m_pVB));

	////
	//## Create Index Buffer
	std::vector<UINT> indices;
	indices.insert(indices.end(), m_MeshInfo.Indices.begin(), m_MeshInfo.Indices.end());

	m_dwIdxSize = sizeof(UINT);
	m_dwIndexCnt = indices.size(); // 삼각형의 개수 * 삼각형 1개당 버텍스 수


	D3D11_BUFFER_DESC tIdxDesc;
	ZeroMemory(&tIdxDesc, sizeof(D3D11_BUFFER_DESC));
	tIdxDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tIdxDesc.ByteWidth = m_dwIdxSize * m_dwIndexCnt;
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tIdxInit;
	tIdxInit.pSysMem = &indices[0];
	HR(m_pMyGDI->pDevice->CreateBuffer(&tIdxDesc, &tIdxInit, &m_pIB));

	return S_OK;
}

Engine::CResources * Engine::CSphereTex::CloneResource(void)
{
	++m_dwRefCnt;

	CResources* pResources = new CSphereTex(*this);

	return pResources;
}

Engine::CSphereTex * Engine::CSphereTex::Create(MYGDI * pMyGDI , float radius, UINT numSubdivisions)
{
	CSphereTex* pBuffer = new CSphereTex(pMyGDI);
	if (FAILED(pBuffer->CreateBuffer(radius , numSubdivisions)))
	{
		Safe_Delete(pBuffer);
	}

	return pBuffer;
}

void Engine::CSphereTex::Subdivide(MESHINFO& meshData)
{
	MESHINFO inputCopy = meshData;
	meshData.Vertices.resize(0);
	meshData.Indices.resize(0);

	UINT numTris = inputCopy.Indices.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		tagVertexTotal v0 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 0]];
		tagVertexTotal v1 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 1]];
		tagVertexTotal v2 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		tagVertexTotal m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.vPos = XMFLOAT3(
			0.5f*(v0.vPos.x + v1.vPos.x),
			0.5f*(v0.vPos.y + v1.vPos.y),
			0.5f*(v0.vPos.z + v1.vPos.z));

		m1.vPos = XMFLOAT3(
			0.5f*(v1.vPos.x + v2.vPos.x),
			0.5f*(v1.vPos.y + v2.vPos.y),
			0.5f*(v1.vPos.z + v2.vPos.z));

		m2.vPos = XMFLOAT3(
			0.5f*(v0.vPos.x + v2.vPos.x),
			0.5f*(v0.vPos.y + v2.vPos.y),
			0.5f*(v0.vPos.z + v2.vPos.z));

		//
		// Add new geometry.
		//

		meshData.Vertices.push_back(v0); // 0
		meshData.Vertices.push_back(v1); // 1
		meshData.Vertices.push_back(v2); // 2
		meshData.Vertices.push_back(m0); // 3
		meshData.Vertices.push_back(m1); // 4
		meshData.Vertices.push_back(m2); // 5

		meshData.Indices.push_back(i * 6 + 0);
		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 5);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 2);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 1);
		meshData.Indices.push_back(i * 6 + 4);
	}

}

float Engine::CSphereTex::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f*3.141592f; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + 3.141592f; // in [0, 2*pi).

	return theta;
}
