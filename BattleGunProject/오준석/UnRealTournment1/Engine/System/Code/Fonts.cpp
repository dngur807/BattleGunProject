#include "Fonts.h"


Engine::CFont::CFont(MYGDI* pMyGDI)
	: m_pMyGDI(pMyGDI)
	//, m_pFont(NULL)
	//, m_iHeight(0)
	//, m_iWidth(0)
{

}

Engine::CFont::~CFont(void)
{
	//Release();
}

HRESULT Engine::CFont::InitFont(const UINT& iHeight, const UINT& iWidth
	, const UINT& iWeight, const TCHAR* pFaceName)
{
	/*D3DXFONT_DESC		Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.Height = m_iHeight = iHeight;
	Font_Desc.Width = m_iWidth = iWidth;
	Font_Desc.Weight = iWeight;
	lstrcpy(Font_Desc.FaceName, pFaceName);
	Font_Desc.CharSet = HANGUL_CHARSET;

	FAILED_CHECK(D3DXCreateFontIndirect(m_pDevice, &Font_Desc, &m_pFont));*/
	return S_OK;
}

void Engine::CFont::Render(const TCHAR* pString , XMFLOAT3 Color, const XMFLOAT3& vPos)
{
	RECT		rc = { (long)vPos.x, (long)vPos.y
		, (long)vPos.x + m_iWidth * lstrlen(pString), (long)vPos.y + m_iHeight };
	//m_pFont->DrawTextW(NULL, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);
}

Engine::CFont* Engine::CFont::Create(MYGDI* pMyGDI, const UINT& iHeight, const UINT& iWidth, const UINT& iWeight, const TCHAR* pFaceName)
{
	CFont* pFont = new CFont(pMyGDI);
	if (FAILED(pFont->InitFont(iHeight, iWidth, iWeight, pFaceName)))
		Safe_Delete(pFont);

	return pFont;
}

void Engine::CFont::Release(void)
{
	//Safe_Release(m_pFont);
}

