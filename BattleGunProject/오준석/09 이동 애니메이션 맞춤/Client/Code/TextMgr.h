#pragma once

#include "Engine_Include.h"
#include "Fonts.h"


class CText;
class CFonts;

class CTextMgr
{
public:
	DECLARE_SINGLETON(CTextMgr)
private:
	CTextMgr();
	~CTextMgr();
public:
	void Initialize();
public:
	void SetText(CText* pText)
	{
		m_pText = pText;
	}
	CText* GetText()
	{
		return m_pText;
	}

	void AddText(TCHAR ch);
	void RemoveText();
	void ReleaseText();
	void Render(const TCHAR* text);

private:
	CText*							m_pText;
};

