#include "stdafx.h"
#include "TextMgr.h"
#include "Text.h"
#include "Export_Function.h"
#include "Fonts.h"
#include "Component.h"
#include "FontMgr.h"

IMPLEMENT_SINGLETON(CTextMgr)


CTextMgr::CTextMgr()
{
	m_pText = NULL;
}


CTextMgr::~CTextMgr()
{
}

void CTextMgr::Initialize()
{
}

void CTextMgr::AddText(TCHAR ch)
{
	if (m_pText != NULL)
		m_pText->AddText(ch);
}

void CTextMgr::RemoveText()
{
	if (m_pText != NULL)
		m_pText->RemoveText();
}

void CTextMgr::ReleaseText()
{
	m_pText = NULL;
}

void CTextMgr::Render(const TCHAR* text)
{
}

