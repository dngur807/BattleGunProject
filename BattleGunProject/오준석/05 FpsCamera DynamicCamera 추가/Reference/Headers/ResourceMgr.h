#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#include "Engine_Include.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CResourceMgr
{
	DECLARE_SINGLETON(CResourceMgr)
private:
	CResourceMgr(void);
	~CResourceMgr(void);

public:
	CResources* Find_Resource(const WORD& wContainerIdx, const TCHAR* pResourceKey);
	CResources* CloneResource(const WORD& wContainerIdx, const TCHAR* pResourceKey);

public:
	HRESULT ReserveContainerSize(const WORD& wSize);

public:
	HRESULT AddBuffer(MYGDI* pMyGDI, const WORD& wContainerIdx
		, CVIBuffer::BUFFERTYPE eBufferType, const TCHAR* pResourceKey
		, const WORD& wCntX = 0, const WORD& wCntZ = 0, const WORD& wItv = 1);

	HRESULT AddTexture(MYGDI* pMyGDI, const WORD& wContainerIdx
		, TEXTURETYPE eTextureType
		, const TCHAR* pResourceKey
		, const TCHAR* pFilePath
		, const WORD& wCnt = 1);

	HRESULT AddMesh(MYGDI* pMyGDI, const WORD& wContainerIdx,
		MESHTYPE eMeshType, const TCHAR* pResourceKey,
		const TCHAR* pFilePath, const TCHAR* pFileName);

	TEX2D* LoadMeshTexture(MYGDI* pMyGDI, string filename, float scale = 1.0f);

public:
	void ResourceReset(const WORD& wContainerIdx);

private:
	void Release(void);

private:
	typedef std::map<const TCHAR*, CResources*>		MAPRESOURCE;
	MAPRESOURCE*		m_pmapResource;
	WORD				m_wReservedSize;

	map<string, TEX2D*>			m_mapTexture;
};

END

#endif // ResourceMgr_h__