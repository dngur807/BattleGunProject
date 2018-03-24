#ifndef Text_h__
#define Text_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CFonts;
};

class CText
	: public Engine::CGameObject
{
public:
	CText(Engine::MYGDI* pMyGDI, TCHAR* pFontKey, int iMax);
	~CText(void);
public:
	virtual HRESULT Initialize(TCHAR* pFontKey);
	virtual int		Update(void);
	virtual void    Render(void);
private:
	void Release(void);
public:
	static CText*	Create(Engine::MYGDI* pMyGDI, TCHAR* pFontKey, int iMax);

private:
	HRESULT AddComponent(TCHAR* pFontKey);
public:
	void AddText(TCHAR ch);
	void RemoveText(void);
	void ClearText(void);
	void SetPos(float fX, float fY);
	void SetColor(D3DCOLOR dColor);
	void SetSize(float size);
	void SetText(TCHAR* szText);
	TCHAR*		GetText(void);

private:
	TCHAR			m_pFontKey[MIN_STR];
	DWORD			m_dColor;
	TCHAR			m_szText[MAX_STR];
//	XMFLOAT3		m_vPos;
	int				m_iMax;


private:
	Engine::CFonts*		m_pFont;
};

#endif // Text_h__
