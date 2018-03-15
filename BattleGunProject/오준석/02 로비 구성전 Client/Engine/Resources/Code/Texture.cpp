#include "Texture.h"
#include "Effects.h"

Engine::CTexture::CTexture(MYGDI* pMyGDI)
	:CResources(pMyGDI),
	m_pEffect(Engine::CEffects::GetInstance()->GetBasicFX())
{
}


Engine::CTexture::~CTexture()
{
	Release();
}

DWORD Engine::CTexture::Release(void)
{
	if (m_dwRefCnt == 0)
	{
		DWORD		dwSize = m_vecSRV.size();
		for (DWORD i = 0; i < dwSize; ++i)
		{
			Engine::Safe_Release(m_vecSRV[i]);
		}
		m_vecSRV.clear();

		return 0;
	}
	else
		--m_dwRefCnt;

	return 0;
}

void Engine::CTexture::Render(const DWORD & dwStage, const DWORD & iIndex)
{
	if (iIndex >= m_dwContainerSize)
		return;

	m_pEffect->SetDiffuseMap(m_vecSRV[iIndex]);
}

ID3D11ShaderResourceView** Engine::CTexture::GetTextureView(const DWORD & iIndex)
{
	return &(m_vecSRV[iIndex]);
}

Engine::CResources * Engine::CTexture::CloneResource(void)
{
	CResources*	pResources = new CTexture(*this);

	m_dwRefCnt;

	return pResources;
}

HRESULT Engine::CTexture::LoadTexture(TEXTURETYPE eTextureType,
	const std::wstring & wstrFilePath, const WORD & wCnt)
{
	if (wCnt <= 0) // 수업 소스에는 == 임. vector의 reserve에 사용되므로 수정
		return E_FAIL;

	ID3D11ShaderResourceView* pSRV;

	m_vecSRV.reserve(int(wCnt));

	TCHAR	szFullPath[_MAX_DIR] = L"";

	for (size_t i = 0; i < wCnt; ++i)
	{
		wsprintf(szFullPath, wstrFilePath.c_str(), i);

		std::wstring wstrTemp = szFullPath;

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

		FAILED_CHECK_MSG(hr, szFullPath);

		HR(CreateShaderResourceView(m_pMyGDI->pDevice,
			image.GetImages(), image.GetImageCount(),
			image.GetMetadata(), &pSRV));

		m_vecSRV.push_back(pSRV);
	}

	//// vector에 저장된 이미지가 몇장인지 보관
	m_dwContainerSize = m_vecSRV.size();

	return S_OK;
}

Engine::CTexture * Engine::CTexture::Create(MYGDI* pMyGDI,
	TEXTURETYPE eTextureType, const std::wstring & wstrFilePath, const WORD & wCnt)
{
	CTexture* pTexture = new CTexture(pMyGDI);
	if (FAILED(pTexture->LoadTexture(eTextureType, wstrFilePath, wCnt)))
		Engine::Safe_Delete(pTexture);

	return pTexture;
}

//
//DirectX::ScratchImage Engine::CTexture::LoadTextureFromFile(LPCWSTR filename)
//{
//	std::wstring wstrTemp = filename;
//
//	DirectX::ScratchImage cImage;
//
//	ZeroMemory(&cImage, sizeof(DirectX::ScratchImage));
//
//	WCHAR ext[_MAX_EXT];
//	_wsplitpath_s(wstrTemp.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
//
//	if (_wcsicmp(ext, L".dds") == 0)
//	{
//		if(FAILED(DirectX::LoadFromDDSFile(wstrTemp.c_str(), DirectX::DDS_FLAGS_NONE,
//			nullptr, cImage)))
//			MSG_BOX(L"Texture Load Failed");
//	}
//	else if (_wcsicmp(ext, L".tga") == 0)
//	{
//		if (FAILED(DirectX::LoadFromTGAFile(wstrTemp.c_str(), nullptr, cImage)))
//			MSG_BOX(L"Texture Load Failed");
//	}
//	else
//	{
//		if (FAILED(DirectX::LoadFromWICFile(wstrTemp.c_str(), DirectX::DDS_FLAGS_NONE,
//			nullptr, cImage)))
//			MSG_BOX(L"Texture Load Failed");
//	}
//
//	return cImage;
//}
