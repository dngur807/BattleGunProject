#pragma once
#include "Engine_Include.h"
#include "Include.h"

class CGameMgr
{
public:
	DECLARE_SINGLETON(CGameMgr);

private:
	CGameMgr();
	~CGameMgr();

public:
	void Initialize();
	GAMEINFO& GetGameInfo()
	{
		return m_tGameInfo;
	}
	void Reset()
	{
		m_tGameInfo = GAMEINFO();
	}
private:
	void Release();
private:
	GAMEINFO		m_tGameInfo;
};

