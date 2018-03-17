#include "Primitive.h"


Engine::CPrimitive::CPrimitive(MYGDI * pMyGDI, vector<SkinnedVertex> vertices, vector<UINT> indices)
	:m_pMyGDI(pMyGDI)
{
	SetVertices(m_pMyGDI->pDevice, vertices, vertices.size());
	SetIndices(m_pMyGDI->pDevice, indices);
}

Engine::CPrimitive::CPrimitive()
{
}

Engine::CPrimitive::~CPrimitive()
{
}


void Engine::CPrimitive::Cleanup()
{
	// Release the buffers.
	ReleaseCOM(m_pVB);
	ReleaseCOM(m_pIB);
}

void Engine::CPrimitive::SetVertices(ID3D11Device * device, vector<SkinnedVertex> vertices, int size)
{
	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(SkinnedVertex) * size;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	// Set the init data.
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertices[0];

	// Create the vertex buffer.
	HRESULT hr = device->CreateBuffer(&vbd, &initData, &m_pVB);

	m_iNumVertices = size;

	// Compute the AABB.
	//ComputeBoundingAxisAlignedBoxFromPoints(&m_BoundingBox, size, &vertices[0].Pos, sizeof(SkinnedVertex));
}

void Engine::CPrimitive::SetIndices(ID3D11Device * device, vector<UINT> indices)
{
	// Fill out the D3D11_BUFFER_DESC struct.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Set the init data.
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &indices[0];

	// Create the index buffer.
	HR(device->CreateBuffer(&ibd, &initData, &m_pIB));

	m_iNumIndices = indices.size();
}

ID3D11Buffer * Engine::CPrimitive::GetVertices()
{
	return m_pVB;
}

ID3D11Buffer * Engine::CPrimitive::GetIndices()
{
	return m_pIB;
}

int Engine::CPrimitive::NumVertices()
{
	return m_iNumVertices;
}

int Engine::CPrimitive::NumIndices()
{
	return m_iNumIndices;
}

void Engine::CPrimitive::DrawSkinnedMesh(void)
{
	UINT stride = sizeof(SkinnedVertex);
	UINT offset = 0;
	m_pMyGDI->pContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	m_pMyGDI->pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);	// [NOTE] Important with right format!! 16 or 32!
	m_pMyGDI->pContext->DrawIndexed(m_iNumIndices, 0, 0);
}

BoundingBox Engine::CPrimitive::GetBoundingBox()
{
	return m_BoundingBox;
}
