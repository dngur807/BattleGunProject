#pragma once
#include "Resources.h"

BEGIN(Engine)

class CModelObj;
class CVIBuffer;
class CTransform;
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
	DWORD			m_dwMeshNum;

// 위치값 저장용 변수
public:
	Engine::CTransform*			m_pInfo;
	Engine::CTransform* GetInfo() { return m_pInfo; }
	void SetvPosInfo(XMFLOAT3 _pos);
	void SetvSizeInfo(XMFLOAT3 _size);
	void SetAngleInfo(float _x, float _y, float _z);
	void SetMeshNum(DWORD dNum);
	DWORD GetMeshNum() { return m_dwMeshNum; }
	//void SetObjOption(OBJOPTION dNum);
	bool m_bRotate;
};

END	