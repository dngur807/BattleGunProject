#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CPrimitive;

class ENGINE_DLL CSkinnedMesh
{
public:
	CSkinnedMesh(MYGDI* pMyGDI);
	~CSkinnedMesh();

public:
	void Draw();

	//  == Primitive Function
	//void CreateBuffer(vector<SkinnedVertex> vertices, int iVtxCnt, vector<UINT> indices);
	//void DrawPrimitive(void);
	////
	void SetPrimitive(CPrimitive* primitive);
	void SetVertices(vector<SkinnedVertex> vertices);
	void SetIndices(vector<UINT> indices);
	void SetMaterial(Material& material);
	void SetTexture(TEX2D* texture);
	void SetNormalMap(TEX2D* nmap);
	void LoadTexture(string filename);

	void Save(ofstream& fout);
	void Load(ifstream& fin);

private:
	void Release(void);

private:
	MYGDI*				m_pMyGDI;
	ID3D11InputLayout*		m_pInputLayout;

private:
	ID3D11Buffer*		m_pVB;
	ID3D11Buffer*		m_pIB;
	BoundingBox			m_BoundingBox;
	UINT				m_iNumVertices;
	UINT				m_iNumIndices;

private:
	CPrimitive*			m_pPrimitive;
	TEX2D*				m_pTexture;
	TEX2D*				m_pNormalMap;
	Material			m_tMaterial;
	vector<SkinnedVertex>		m_vecVertices;
	vector<UINT>				m_vecIndices;

};

END