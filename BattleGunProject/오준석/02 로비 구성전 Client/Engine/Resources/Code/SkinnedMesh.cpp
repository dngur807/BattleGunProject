#include "SkinnedMesh.h"

#include "ResourceMgr.h"
#include "Effects.h"
#include "Primitive.h"


Engine::CSkinnedMesh::CSkinnedMesh(MYGDI* pMyGDI)
	:m_pMyGDI(pMyGDI), m_pTexture(nullptr), m_pNormalMap(nullptr)
{
	ZeroMemory(&m_tMaterial, sizeof(Material));

		// Create the vertex input layout.
		D3D11_INPUT_ELEMENT_DESC vertexDesc[8] =
		{
			{ "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT ,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTSA",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTSB",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEINDICESA",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEINDICESB",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	
		// Create the input layout.
		D3DX11_PASS_DESC passDesc;
		m_pMyGDI->pEffect->GetSkinnedTech()->GetPassByIndex(0)->GetDesc(&passDesc);
		HR(m_pMyGDI->pDevice->CreateInputLayout(vertexDesc, 8, passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize, &m_pInputLayout));
}


Engine::CSkinnedMesh::~CSkinnedMesh()
{
	Release();
}

void Engine::CSkinnedMesh::Draw()
{
	UINT iA = m_vecVertices.size();
	// Set the material properties for this skinned mesh.
	/*if(m_pTexture->pSRV != nullptr)
		m_pMyGDI->pEffect->SetTexture(m_pTexture);*/
	m_pMyGDI->pEffect->SetDiffuseMap(m_pTexture->pSRV);
	m_pMyGDI->pEffect->SetUseNormalMap(false);
	m_pMyGDI->pEffect->SetNormalMap(m_pNormalMap);
	m_pMyGDI->pEffect->SetMaterial(m_tMaterial);
	m_pMyGDI->pEffect->SetUseTexture(true);
	m_pMyGDI->pEffect->SetUseLighting(true);
	m_pMyGDI->pEffect->SetTexTransform(XMMatrixIdentity());

	m_pMyGDI->pEffect->Apply(m_pMyGDI->pContext, 
		m_pNormalMap == 0 ? Engine::STANDARD_TECH : Engine::NMAP_TECH);

	// Set the input layout and the primitive topology.
	UINT stride = sizeof(SkinnedVertex);
	UINT offset = 0;
	m_pMyGDI->pContext->IASetInputLayout(m_pInputLayout);
	m_pMyGDI->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw the mesh with SkinnedVertex vertices that contains skinning data.
	//DrawPrimitive();
	m_pPrimitive->DrawSkinnedMesh();
}
//
//void Engine::CSkinnedMesh::CreateBuffer(vector<SkinnedVertex> vertices, int iVtxCnt, vector<UINT> indices)
//{
//	// Create the vertex input layout.
//	D3D11_INPUT_ELEMENT_DESC vertexDesc[6] =
//	{
//		{ "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT ,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "WEIGHTS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "BONEINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 }
//	};
//
//	// Create the input layout.
//	D3DX11_PASS_DESC passDesc;
//	m_pMyGDI->pTech->GetPassByIndex(0)->GetDesc(&passDesc);
//	HR(m_pMyGDI->pDevice->CreateInputLayout(vertexDesc, 6, passDesc.pIAInputSignature,
//		passDesc.IAInputSignatureSize, &m_pInputLayout));
//
//	// Fill out the D3D11_BUFFER_DESC struct.
//	D3D11_BUFFER_DESC vbd;
//	vbd.Usage = D3D11_USAGE_DYNAMIC;
//	vbd.ByteWidth = sizeof(SkinnedVertex) * iVtxCnt;
//	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	vbd.MiscFlags = 0;
//
//	// Set the init data.
//	D3D11_SUBRESOURCE_DATA initVertices;
//	initVertices.pSysMem = &vertices[0];
//
//	// Create the vertex buffer.
//	HRESULT hr = m_pMyGDI->pDevice->CreateBuffer(&vbd, &initVertices, &m_pVB);
//
//	m_iNumVertices = iVtxCnt;
//
//	//// Compute the AABB.
//	//ComputeBoundingAxisAlignedBoxFromPoints(&mBoundingBox, size, &vertices[0].Pos, sizeof(VertexType));
//
//
//	// Fill out the D3D11_BUFFER_DESC struct.
//	D3D11_BUFFER_DESC ibd;
//	ibd.Usage = D3D11_USAGE_IMMUTABLE;
//	ibd.ByteWidth = sizeof(UINT) * indices.size();
//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibd.CPUAccessFlags = 0;
//	ibd.MiscFlags = 0;
//	ibd.StructureByteStride = 0;
//
//	// Set the init data.
//	D3D11_SUBRESOURCE_DATA initIndicies;
//	initIndicies.pSysMem = &indices[0];
//
//	// Create the index buffer.
//	HR(m_pMyGDI->pDevice->CreateBuffer(&ibd, &initIndicies, &m_pIB));
//
//	m_iNumIndices = indices.size();
//}
//
//void Engine::CSkinnedMesh::DrawPrimitive(void)
//{
//	UINT stride = sizeof(SkinnedVertex);
//	UINT offset = 0;
//	m_pMyGDI->pContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
//	m_pMyGDI->pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);	// [NOTE] Important with right format!! 16 or 32!
//	m_pMyGDI->pContext->DrawIndexed(m_iNumIndices, 0, 0);
//}

void Engine::CSkinnedMesh::SetPrimitive(CPrimitive * primitive)
{
	m_pPrimitive = primitive;
}

void Engine::CSkinnedMesh::SetVertices(vector<SkinnedVertex> vertices)
{
	m_vecVertices = vertices;
}

void Engine::CSkinnedMesh::SetIndices(vector<UINT> indices)
{
	m_vecIndices = indices;
}

void Engine::CSkinnedMesh::SetMaterial(Material& rMaterial)
{
	m_tMaterial = rMaterial;

	if (XMVector4Equal(XMLoadFloat4(&rMaterial.Ambient), XMVectorZero()))
	{
		m_tMaterial.Ambient = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	}
	//if (XMVector4Equal(XMLoadFloat4(&rMaterial.Diffuse), XMVectorZero()))
	//{
	//	//m_tMaterial.Diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	//}
	//if (XMVector4Equal(XMLoadFloat4(&rMaterial.Specular), XMVectorZero()))
	//{
	//	m_tMaterial.Specular = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	//}
	////m_tMaterial.Specular = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
}

void Engine::CSkinnedMesh::SetTexture(TEX2D * texture)
{
	m_pTexture = texture;
}

void Engine::CSkinnedMesh::SetNormalMap(TEX2D * nmap)
{
	m_pNormalMap = nmap;
}

void Engine::CSkinnedMesh::LoadTexture(string filename)
{
	m_pTexture = CResourceMgr::GetInstance()->LoadMeshTexture(m_pMyGDI, filename);
}

void Engine::CSkinnedMesh::Save(ofstream & fout)
{
	fout << "-Mesh-\r\n";
	fout << "#Texture " << m_pTexture->name << "\r\n";
	fout << "#NumVertices " << m_vecVertices.size() << "\r\n";
	fout << "#NumIndices " << m_vecIndices.size() << "\r\n";
	fout << "#Vertices" << "\r\n";

	for (int i = 0; i < m_vecVertices.size(); i++)
	{
		/*fout.write(reinterpret_cast<char*>(&mVertices[i].Pos.x), sizeof(mVertices[i].Pos.x));
		fout.write(reinterpret_cast<char*>(&mVertices[i].Pos.y), sizeof(mVertices[i].Pos.y));
		fout.write(reinterpret_cast<char*>(&mVertices[i].Pos.z), sizeof(mVertices[i].Pos.z));*/
		fout << "Pos: " << m_vecVertices[i].Pos.x << " " << m_vecVertices[i].Pos.y << " " << m_vecVertices[i].Pos.z << "\r\n";
		fout << "Normal: " << m_vecVertices[i].Normal.x << " " << m_vecVertices[i].Normal.y << " " << m_vecVertices[i].Normal.z << "\r\n";
		fout << "Tangent: " << m_vecVertices[i].Tangent.x << " " << m_vecVertices[i].Tangent.y << " " << m_vecVertices[i].Tangent.z << "\r\n";
		fout << "UV: " << m_vecVertices[i].Tex.x << " " << m_vecVertices[i].Tex.y << "\r\n";
		fout << "BoneId " << (int)m_vecVertices[i].BoneIndicesA[0] << " " << (int)m_vecVertices[i].BoneIndicesA[1] << " " << (int)m_vecVertices[i].BoneIndicesA[2] << " " << (int)m_vecVertices[i].BoneIndicesA[3] << "\r\n";
		fout << "Weights: " << m_vecVertices[i].WeightsA.x << " " << m_vecVertices[i].WeightsA.y << " " << m_vecVertices[i].WeightsA.z << "\r\n";
		fout << "-\r\n";
	}

	fout << "\r\n#Indices" << "\r\n";

	for (int i = 0; i < m_vecIndices.size(); i++)
		fout << m_vecIndices[i] << " ";

	fout << "\r\n";
}

void Engine::CSkinnedMesh::Load(ifstream & fin)
{
	string ignore, texture;
	int numVertices, numIndices;
	fin >> ignore; // -Mesh-
	fin >> ignore >> texture; // #Texture
	fin >> ignore >> numVertices; // #NumVertices
	fin >> ignore >> numIndices; // #NumIndices
	fin >> ignore; // #Vertices

				   // Read all vertices;
	vector<SkinnedVertex> vertices;
	for (int i = 0; i < numIndices; i++)
	{
		SkinnedVertex vertex;

		fin >> ignore >> vertex.Pos.x >> vertex.Pos.y >> vertex.Pos.z;
		fin >> ignore >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z;
		fin >> ignore >> vertex.Tangent.x >> vertex.Tangent.y >> vertex.Tangent.z;
		fin >> ignore >> vertex.Tex.x >> vertex.Tex.y;
		int boneIds[4];
		fin >> ignore >> boneIds[0] >> boneIds[1] >> boneIds[2] >> boneIds[3];
		//fin >> ignore >> vertex.Weights.x >> vertex.Weights.y >> vertex.Weights.z;
		fin >> ignore; // -

		vertex.BoneIndicesA[0] = boneIds[0];
		vertex.BoneIndicesA[1] = boneIds[1];
		vertex.BoneIndicesA[2] = boneIds[2];
		vertex.BoneIndicesA[3] = boneIds[3];

		vertices.push_back(vertex);
	}

	// Read all indices.
	fin >> ignore; // #Indices
	vector<UINT> indices;
	for (int i = 0; i < numIndices; i++)
	{
		UINT index;
		fin >> index;
		indices.push_back(index);
	}

	// Create the primitive.
	m_pPrimitive = new CPrimitive(m_pMyGDI, vertices, indices);
	SetVertices(vertices);
	SetIndices(indices);

	// Load the texture.
	LoadTexture(texture);

	XMFLOAT4 vColor;
	XMStoreFloat4(&vColor, Colors::White);
	SetMaterial(Material(vColor));
}

void Engine::CSkinnedMesh::Release(void)
{
	Safe_Delete(m_pPrimitive);
}
