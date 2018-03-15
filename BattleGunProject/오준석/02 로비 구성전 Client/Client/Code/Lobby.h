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
	CText*						m_pChatText;// ä�� �Է� �ؽ�Ʈ
	wstring						m_wstrSelectMap;//���õ� ��
	eMapType					m_eMapType;

	bool						m_bActivate;//������ ������ �̷��� �ִ°�??
	bool						m_bRoomManager;//���� �����ΰ�?
	int							m_iRoomManagerIndex;//���� �ε���
};

#endif // Lobby_h__
