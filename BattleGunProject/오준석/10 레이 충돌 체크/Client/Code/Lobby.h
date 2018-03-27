#ifndef Lobby_h__
#define Lobby_h__

#include "Scene.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;
	class CManagement;
}
class CText;
class CReactUI_2D;

class CLobby
	: public Engine::CScene
{
public:
	enum LAYERID { LAYER_UI };
private:
	explicit CLobby(Engine::MYGDI* pMyGDI);
public:
	virtual ~CLobby();
public:
	virtual HRESULT InitScene();
	virtual int Update();
	virtual void Render();
	void	PushChatList(TCHAR* pText, D3DCOLOR dColor);

	static CLobby* Create(Engine::MYGDI* pMyGDI);

private:
	void AnalysisInterMsg();
	void Release();

	HRESULT LodingTexture();
	HRESULT	Add_UI_Layer(void);

	void	RenderChatList(void);
	


private:
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::CManagement*		m_pManagement;
	CText*						m_pChatText;// 채팅 입력 텍스트
	CText*						m_pMapName;//맵이름
	TCHAR						m_szChatList[MAX_CHAT_LINE][MAX_STR];//채팅
	D3DCOLOR					m_arrChatColor[MAX_CHAT_LINE];//채팅 색

	vector<CText*>				m_vecTextChat;//채팅

	CReactUI_2D*				m_pChatUI;//채팅 유아이
	CReactUI_2D*				m_pSmallMap1;// 조그만 맵 1
	CReactUI_2D*				m_pSmallMap2;// 조그만 맵 2

	wstring						m_wstrSelectMap;//선택된 맵
	eMapType					m_eMapType;

	bool						m_bActivate;//접속이 완전히 이루져 있는가??
	bool						m_bRoomManager;//내가 방장인가?
	bool						m_bChatMode;//채팅 모드
	int							m_iRoomManagerIndex;//방장 인덱스

private:
	TCHAR					m_szText[MAX_STR];
	map<int , CText*>			m_mapUserText;
	RECT	m_rtStartBtn;
	RECT	m_rtChatInput;
	RECT    m_rtPrevMapBtn;
	RECT	m_rtNextMapBtn;
	RECT	m_rtATeamBtn;
	RECT    m_rtBTeamBtn;
};

#endif // Lobby_h__
