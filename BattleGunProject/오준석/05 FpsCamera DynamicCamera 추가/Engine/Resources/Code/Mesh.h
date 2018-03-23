#pragma once
#include "Resources.h"

BEGIN(Engine)

class CModelObj;
class CVIBuffer;

class ENGINE_DLL CMesh :
	public CResources
{
protected:
	explicit CMesh(MYGDI* pMyGDI);
	explicit CMesh(const CMesh& rhs);

public:
	virtual ~CMesh(void);

public:
	virtual CResources* CloneResource(void)PURE;

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pName) { return S_OK; }
	virtual void RenderMesh(const XMMATRIX& pWorldMatrix)PURE;
	virtual DWORD Release(void);

public:
	BoundingBox& GetBoundingBox(void) { return m_BoundingBox; }

protected:
	const aiScene* m_pScene;
	Assimp::Importer m_aiImporter;
	typedef std::vector<CModelObj*>		VECMODEL;
	VECMODEL	m_vecModel;

protected:
	typedef std::vector<ID3D11ShaderResourceView*> VECSRV;
	VECSRV	m_vecSRV;

protected:
	BoundingBox		m_BoundingBox;
	CVIBuffer*		m_pBuffer;
};

END	