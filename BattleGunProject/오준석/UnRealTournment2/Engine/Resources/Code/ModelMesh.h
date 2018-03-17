#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CTexture;

class CModelMesh
{
public:
	explicit CModelMesh(MYGDI* pMyGDI);
	~CModelMesh();

public:
	void Initialize(const aiMesh* pMesh, const aiMaterial* pMtl);
	void SetMaterial(const Material& rMaterial);
	void LoadTexture(const string& strPath);
	BoundingBox& CreateBoundingBox(void);
	void Draw(const XMMATRIX& rmatWorld);
	void Release(void);

private:
	MYGDI* m_pMyGDI;
	ID3D11InputLayout*	m_pInputLayout;
	
private:
	Material		m_tMaterial;
	TEX2D*			m_pTexture;

private:
	ID3D11Buffer* m_pVB;
	UINT	m_iNumVertices;
	UINT	m_iSizeVertices;
	UINT	m_iVtxOffset;

	ID3D11Buffer* m_pIB;
	UINT	m_iNumIndices;
	UINT	m_iSizeIndices;
	UINT	m_iIdxOffset;

	UINT m_iNumFaces;
	float m_fColDiff[4];
	UINT m_iFaceType;

private:
	std::vector<VTXTEX> m_vecVertices;
	std::vector<UINT>	m_vecIndices;

private:
	XMFLOAT3	m_vMin, m_vMax;
	BoundingBox			m_BoundingBox;
};

END	