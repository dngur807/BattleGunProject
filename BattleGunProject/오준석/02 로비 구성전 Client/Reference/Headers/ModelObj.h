#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CModelMesh;

class ENGINE_DLL CModelObj
{
public:
	explicit CModelObj(MYGDI* pMyGDI);
	~CModelObj();

public:
	void BuildBuffer(const aiScene* pModel, const aiNode* pNode, const aiString& aiPath);
	BoundingBox& MergeBoundingBox(void);
	void Draw(const XMMATRIX& rmatWorld);
	void Release(void);

private:
	MYGDI*			m_pMyGDI;
private:
	UINT			m_iNumMeshes;
private:
	typedef std::vector<CModelMesh*> VECMESH;
	VECMESH		m_vecMeshes;

private:
	BoundingBox		m_BoundingBox;

};

END