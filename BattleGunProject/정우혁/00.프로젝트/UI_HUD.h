#pragma once
#include "UI.h"
#include "Include.h"

class CMyCharacter;
class CUser;
class CText;

class CUI_Hud
	: public CUI
{
private:
	CUI_Hud(Engine::MYGDI*		pMyGDI);
public:
	virtual ~CUI_Hud();

public:
	virtual HRESULT Initialize();
	virtual int Update();
	virtual void Render();
	void SetMyCharacter(CMyCharacter* pCharacter);

	void Hit();
	void GameResult(bool IsVictory);
private:
	HRESULT AddComponent();

	void RenderHUD();
	void RenderCrossHair();
	void RenderText();
	void RenderResult();
	void RenderNameTag();
	void MakeNameTag();
public:
	static CUI_Hud* Create(Engine::MYGDI* pMyGDI);

private:
	Engine::CVIBuffer*		m_pCrossHair;
	Engine::CTexture*		m_pCrossHairTex;
	Engine::CTexture*		m_pHealthBackTex;
	Engine::CTexture*		m_pHealthFrontTex;
	Engine::CTexture*		m_pPort;
	Engine::CTexture*		m_pHitEffectTex;
	Engine::CTexture*		m_pVictoryTex;
	Engine::CTexture*		m_pDefeatTex;
	Engine::CVIBuffer*		m_pBuffer;



	CMyCharacter*			m_pMyCharacter;

	CText*					m_pIDText;
	CText*					m_pHPText;
	TCHAR					m_szIDText[MAX_STR];
	TCHAR					m_szHPText[MAX_STR];
private:
	XMFLOAT4X4				matOldView;
	XMFLOAT4X4				matOldProj;

	bool					m_bHit;
	bool					m_IsVictory;
	float					m_fHitAlpha;
	TCHAR					m_szText[MAX_STR];
	NAMETAG					m_tNameTag[8];
};