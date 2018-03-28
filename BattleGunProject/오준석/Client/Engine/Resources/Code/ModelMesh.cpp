#include "ModelMesh.h"
#include "Effects.h"
#include "Texture.h"
#include "ResourceMgr.h"



Engine::CModelMesh::CModelMesh(MYGDI* pMyGDI)
	:m_pMyGDI(pMyGDI), 
	m_iNumVertices(0),
	m_iSizeVertices(0),
	m_iNumIndices(0),
	m_iSizeIndices(0),
	m_iNumFaces(0), 
	m_iVtxOffset(0), 
	m_iIdxOffset(0)
{
}

Engine::CModelMesh::~CModelMesh()
{
	Release();
}

void Engine::CModelMesh::Initialize(const aiMesh * pMesh, const aiMaterial * pMtl)
{
	int iCount = 0;
	m_iNumVertices = pMesh->mNumVertices;
	m_iSizeVertices = sizeof(VTXTEX);
	m_iSizeIndices = sizeof(UINT);
	m_iNumFaces = pMesh->mNumFaces;

	if (pMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
		MSG_BOX(L"Not Triangle Type");

	for (UINT n = 0; n < pMesh->mNumFaces; ++n)
		m_iNumIndices += pMesh->mFaces[n].mNumIndices;



	m_vecIndices.resize(m_iNumIndices);
	UINT iIndex = 0;
	for (UINT i = 0; i < m_iNumFaces; ++i)
	{
		for (UINT j = 0; j < pMesh->mFaces[i].mNumIndices; ++j)
		{
			m_vecIndices[iIndex] = pMesh->mFaces[i].mIndices[j];
			++iIndex;
		}
	}


	m_vecVertices.resize(m_iNumVertices);
	for (UINT i = 0; i < m_iNumVertices; ++i)
	{
		m_vecVertices[i].vPos = XMFLOAT3(pMesh->mVertices[i].x, pMesh->mVertices[i].y,
			pMesh->mVertices[i].z);
		m_vecVertices[i].vNormal = XMFLOAT3(pMesh->mNormals[i].x, pMesh->mNormals[i].y,
			pMesh->mNormals[i].z);
		m_vecVertices[i].vTexC = XMFLOAT2(pMesh->mTextureCoords[0][i].x,
			pMesh->mTextureCoords[0][i].y);
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

	D3DX11_PASS_DESC passDesc;

	m_pMyGDI->pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vtxDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));

	//## Create Vertex Buffer
	D3D11_BUFFER_DESC tBufDesc;
	ZeroMemory(&tBufDesc, sizeof(D3D11_BUFFER_DESC));
	tBufDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tBufDesc.ByteWidth = sizeof(VTXTEX) * m_vecVertices.size();
	tBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tBufDesc.CPUAccessFlags = 0;
	tBufDesc.MiscFlags = 0;
	tBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tVtxInit;
	tVtxInit.pSysMem = &m_vecVertices[0];
	HR(m_pMyGDI->pDevice->CreateBuffer(&tBufDesc, &tVtxInit, &m_pVB));

	// Create Index Buffer
	D3D11_BUFFER_DESC tIdxDesc;
	ZeroMemory(&tIdxDesc, sizeof(D3D11_BUFFER_DESC));
	tIdxDesc.Usage = D3D11_USAGE_IMMUTABLE;
	tIdxDesc.ByteWidth = sizeof(UINT) * m_vecIndices.size();
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIdxDesc.CPUAccessFlags = 0;
	tIdxDesc.MiscFlags = 0;
	tIdxDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tIdxInit;
	tIdxInit.pSysMem = &m_vecIndices[0];
	HR(m_pMyGDI->pDevice->CreateBuffer(&tIdxDesc, &tIdxInit, &m_pIB));
}

void Engine::CModelMesh::SetMaterial(const Material & rMaterial)
{
	m_tMaterial = rMaterial;

	if (XMVector4Equal(XMLoadFloat4(&rMaterial.Ambient), XMVectorZero()))
	{
		m_tMaterial.Ambient = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	}
	/*if (XMVector4Equal(XMLoadFloat4(&rMaterial.Diffuse), XMVectorZero()))
	{
		m_tMaterial.Diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	}
	if (XMVector4Equal(XMLoadFloat4(&rMaterial.Specular), XMVectorZero()))
	{
		m_tMaterial.Specular = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	}*/
}

void Engine::CModelMesh::LoadTexture(const string & strPath)
{
	m_pTexture = CResourceMgr::GetInstance()->LoadMeshTexture(m_pMyGDI, strPath);
}

BoundingBox& Engine::CModelMesh::CreateBoundingBox(void)
{
	XMVECTOR vMin, vMax;
	vMin = vMax = XMLoadFloat3(&m_vecVertices[0].vPos);
	

	for (int i = 1; i < m_vecVertices.size(); ++i)
	{
		XMVECTOR vTemp = XMLoadFloat3(&m_vecVertices[i].vPos);
		vMin = XMVectorMin(vMin, vTemp);
		vMax = XMVectorMax(vMax, vTemp);
	}
	
	XMStoreFloat3(&m_vMin, vMin);
	XMStoreFloat3(&m_vMax, vMax);

	BoundingBox::CreateFromPoints(m_BoundingBox, vMin, vMax);

	return m_BoundingBox;
}

void Engine::CModelMesh::Draw(const XMMATRIX& rmatWorld)
{
	UINT iA = m_vecVertices.size();

	m_pMyGDI->pContext->IASetInputLayout(m_pInputLayout);
	m_pMyGDI->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//UINT iStride = m_dwVtxSize;
	UINT iOffset = 0;
	m_pMyGDI->pContext->IASetVertexBuffers(0, 1, &m_pVB, &m_iSizeVertices, &iOffset);
	m_pMyGDI->pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	m_pMyGDI->pEffect->SetWorld(rmatWorld);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pMyGDI->pTech->GetDesc(&techDesc);

	m_pMyGDI->pEffect->SetDiffuseMap(m_pTexture->pSRV);
	m_pMyGDI->pEffect->SetTexTransform(XMMatrixIdentity());
	m_pMyGDI->pEffect->SetMaterial(m_tMaterial);

	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		m_pMyGDI->pTech->GetPassByIndex(i)->Apply(0, m_pMyGDI->pContext);

		m_pMyGDI->pContext->DrawIndexed(m_iNumIndices, 0, 0);
	}
}

void Engine::CModelMesh::Release(void)
{
	//Safe_Delete_Array(m_pIndices);
	//Safe_Delete_Array(m_pVertices);
}
