#pragma once

#include "engine_include.h"

BEGIN(Engine)

class ENGINE_DLL CPrimitive
{
	//!
	//	Contains a vertex and index buffer.
	//!
public:
	CPrimitive(MYGDI* pMyGDI, vector<SkinnedVertex> vertices, vector<UINT> indices);
	CPrimitive();
	~CPrimitive();

	void Cleanup();

	void SetVertices(ID3D11Device* device, vector<SkinnedVertex> vertices, int size);
	void SetIndices(ID3D11Device* device, vector<UINT> indices);
	ID3D11Buffer* GetVertices();
	ID3D11Buffer* GetIndices();
	int NumVertices();
	int NumIndices();

	void DrawSkinnedMesh();

	BoundingBox GetBoundingBox();

private:
	MYGDI*		m_pMyGDI;
private:
	ID3D11Buffer*		  m_pVB;
	ID3D11Buffer*		  m_pIB;
	BoundingBox			 m_BoundingBox;
	UINT m_iNumVertices;
	UINT m_iNumIndices;
};

END