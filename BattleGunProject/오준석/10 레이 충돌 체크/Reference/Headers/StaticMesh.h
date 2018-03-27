#pragma once

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(MYGDI* pMyGDI);
	explicit CStaticMesh(const CStaticMesh& rhs);
public:
	~CStaticMesh();

private:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void BuildMesh(const aiNode* pNode, const aiString& aiPath);

public:
	virtual void RenderMesh(const XMMATRIX& pWorldMatrix);
	virtual DWORD Release(void);

public:
	void BuildBoundingBox(void);

public:
	virtual CResources* CloneResource(void);

public:
	static CStaticMesh* Create(MYGDI* pMyGDI, const TCHAR* pPath, const TCHAR* pFileName);
};
END	