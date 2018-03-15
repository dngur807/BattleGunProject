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

class CLobby
	: public Engine::CScene
{
private:
	explicit CLobby(Engine::MYGDI* pMyGDI);
public:
	~CLobby();
public:
	virtual HRESULT InitScene();
	virtual int Update();
	virtual void Render();

	static CLobby* Create(Engine::MYGDI* pMyGDI);
private:
	void AnalysisInterMsg();
	void Release();

	void LodingTexture();
private:
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::CManagement*		m_pManagement;
	CText*						m_pChatText;// 채팅 입력 텍스트
	wstring						m_wstrSelectMap;//선택된 맵
	eMapType					m_eMapType;

	bool						m_bActivate;//접속이 완전히 이루져 있는가??
	bool						m_bRoomManager;//내가 방장인가?
	int							m_iRoomManagerIndex;//방장 인덱스
};

#endif // Lobby_h__
