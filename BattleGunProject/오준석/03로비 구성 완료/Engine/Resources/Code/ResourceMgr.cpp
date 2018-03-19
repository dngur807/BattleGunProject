#include "ResourceMgr.h"
#include "Resources.h"
#include "RcTex.h"
#include "Texture.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
IMPLEMENT_SINGLETON(Engine::CResourceMgr)

Engine::CResourceMgr::CResourceMgr(void)
	: m_pmapResource(NULL)
	, m_wReservedSize(0)
{

}

Engine::CResourceMgr::~CResourceMgr(void)
{
	Release();
}

Engine::CResources* Engine::CResourceMgr::Find_Resource(const WORD& wContainerIdx
	, const TCHAR* pResourceKey)
{
	MAPRESOURCE::iterator	iter = find_if(m_pmapResource[wContainerIdx].begin()
		, m_pmapResource[wContainerIdx].end(), CTagFinder(pResourceKey));
	 
	if (iter == m_pmapResource[wContainerIdx].end())
		return NULL;

	return iter->second;
}

Engine::CResources* Engine::CResourceMgr::CloneResource(const WORD& wContainerIdx
	, const TCHAR* pResourceKey)
{
	CResources*		pResource = Find_Resource(wContainerIdx, pResourceKey);
	NULL_CHECK_RETURN(pResource, NULL);

	return pResource->CloneResource();
}

HRESULT Engine::CResourceMgr::ReserveContainerSize(const WORD& wSize)
{
	if (m_wReservedSize != 0 || m_pmapResource != NULL)
		return E_FAIL;

	m_pmapResource = new MAPRESOURCE[wSize];

	m_wReservedSize = wSize;
	return S_OK;
}

HRESULT Engine::CResourceMgr::AddBuffer(MYGDI* pMyGDI
	, const WORD& wContainerIdx
	, CVIBuffer::BUFFERTYPE eBufferType
	, const TCHAR* pResourceKey
	, const WORD& wCntX /*= 0*/
	, const WORD& wCntZ /*= 0*/
	, const WORD& wItv /*= 1*/)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CResources*		pResource = Find_Resource(wContainerIdx, pResourceKey);
	if (pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"버퍼가 이미 추가 되어 있음");
		return E_FAIL;
	}

	CVIBuffer*		pBuffer = NULL;

	switch (eBufferType)
	{
	case CVIBuffer::BUFFER_RCTEX:
		pBuffer = CRcTex::Create(pMyGDI);
		break;

	case CVIBuffer::BUFFER_TERRAIN:
		pBuffer = CTerrainTex::Create(pMyGDI, static_cast<float>(wItv * (wCntX - 1)), 
			static_cast<float>(wItv * (wCntZ - 1)), wCntX, wCntZ);
		break;

	case CVIBuffer::BUFFER_CUBETEX:
		pBuffer = CCubeTex::Create(pMyGDI);
		break;
	}

	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pmapResource[wContainerIdx].insert(MAPRESOURCE::value_type(pResourceKey, pBuffer));
	return S_OK;
}

HRESULT Engine::CResourceMgr::AddTexture(MYGDI* pMyGDI
	, const WORD& wContainerIdx
	, TEXTURETYPE eTextureType
	, const TCHAR* pResourceKey
	, const TCHAR* pFilePath
	, const WORD& wCnt /*= 1*/)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CResources*		pResource = Find_Resource(wContainerIdx, pResourceKey);
	if (pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"텍스쳐가 이미 추가 되어 있음");
		return E_FAIL;
	}

	pResource = CTexture::Create(pMyGDI, eTextureType, pFilePath, wCnt);
	NULL_CHECK_RETURN_MSG(pResource, E_FAIL, L"리소스 할당 실패");

	m_pmapResource[wContainerIdx].insert(MAPRESOURCE::value_type(pResourceKey, pResource));
	return S_OK;
}

HRESULT Engine::CResourceMgr::AddMesh(MYGDI * pMyGDI, const WORD & wContainerIdx, 
	MESHTYPE eMeshType, const TCHAR * pResourceKey, 
	const TCHAR * pFilePath, const TCHAR* pFileName)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CResources*		pResource = Find_Resource(wContainerIdx, pResourceKey);
	if (pResource != NULL)
	{
		TAGMSG_BOX(pResourceKey, L"메쉬가 이미 추가 되어 있음");
		return E_FAIL;
	}

	switch (eMeshType)
	{
	case MESH_STATIC:
		pResource = CStaticMesh::Create(pMyGDI, pFilePath, pFileName);
		break;
	case MESH_DYNAMIC:
		pResource = CDynamicMesh::Create(pMyGDI, pFilePath, pFileName);
		break;
	}

	NULL_CHECK_RETURN(pResource, E_FAIL);
	m_pmapResource[wContainerIdx].insert(
		MAPRESOURCE::value_type(pResourceKey, pResource));

	return S_OK;
}

Engine::TEX2D* Engine::CResourceMgr::LoadMeshTexture(MYGDI* pMyGDI, string filename, float scale)
{
	// A bit of a hax... [NOTE][TODO][HAX]
	char buffer[256];
	sprintf(buffer, "%s - %f", filename.c_str(), scale);
	string textureId = buffer;

	// Is the texture already loaded?
	if (m_mapTexture.find(textureId) != m_mapTexture.end()) {
		return m_mapTexture[textureId];
	}
	else // Not already loaded.
	{
		int iLen = MultiByteToWideChar(CP_ACP, 0, &filename[0], filename.size(), nullptr, 0);
		std::wstring wstrTemp(iLen, 0);
		MultiByteToWideChar(CP_ACP, 0, &filename[0], filename.size(), &wstrTemp[0], iLen);

		WCHAR ext[_MAX_EXT];
		_wsplitpath_s(wstrTemp.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

		HRESULT hr = NULL;
		ScratchImage image;

		if (!_wcsicmp(ext, L".dds"))
			hr = LoadFromDDSFile(wstrTemp.c_str(), DDS_FLAGS_NONE, nullptr, image);
		else if (!_wcsicmp(ext, L".tga"))
			hr = LoadFromTGAFile(wstrTemp.c_str(), nullptr, image);
		else if (!_wcsicmp(ext, L".hdr"))
			hr = LoadFromHDRFile(wstrTemp.c_str(), nullptr, image);
		else
			hr = LoadFromWICFile(wstrTemp.c_str(), WIC_FLAGS_NONE, nullptr, image);

		FAILED_CHECK_RETURN_MSG(hr, nullptr, wstrTemp.c_str());

		TEX2D* texture2d = new TEX2D();
		if (FAILED(CreateShaderResourceView(pMyGDI->pDevice,
			image.GetImages(), image.GetImageCount(),
			image.GetMetadata(), &texture2d->pSRV))) {
			char buffer[246];
			sprintf(buffer, "Error loading texture: %s", filename.c_str());
		}

		texture2d->fScale = scale;
		texture2d->name = filename;
		m_mapTexture[textureId] = texture2d;
		return m_mapTexture[textureId];
	}
}

void Engine::CResourceMgr::ResourceReset(const WORD& wContainerIdx)
{
	for_each(m_pmapResource[wContainerIdx].begin(), m_pmapResource[wContainerIdx].end()
		, CDeleteMap());
	m_pmapResource[wContainerIdx].clear();
}

void Engine::CResourceMgr::Release(void)
{
	if (0 == m_wReservedSize)
		return;

	for (size_t i = 0; i < m_wReservedSize; ++i)
	{
		MAPRESOURCE::iterator	iter = m_pmapResource[i].begin();
		MAPRESOURCE::iterator	iter_end = m_pmapResource[i].end();

		DWORD		dwRefCnt = 0;
		for (; iter != iter_end; ++iter)
		{
			dwRefCnt = iter->second->Release();
			if (dwRefCnt == 0)
				Safe_Delete(iter->second);
		}
		m_pmapResource[i].clear();
	}
	Safe_Delete_Array(m_pmapResource);

	map<string, TEX2D*>::iterator iter = m_mapTexture.begin(),
		iter_end = m_mapTexture.end();
	
	for (; iter != iter_end; ++iter)
	{
		Safe_Release(iter->second->pSRV);
		Safe_Delete(iter->second);
	}

}