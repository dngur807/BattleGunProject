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
	CText*						m_pChatText;// ä�� �Է� �ؽ�Ʈ
	CText*						m_pMapName;//���̸�
	TCHAR						m_szChatList[MAX_CHAT_LINE][MAX_STR];//ä��
	D3DCOLOR					m_arrChatColor[MAX_CHAT_LINE];//ä�� ��

	vector<CText*>				m_vecTextChat;//ä��

	CReactUI_2D*				m_pChatUI;//ä�� ������
	CReactUI_2D*				m_pSmallMap1;// ���׸� �� 1
	CReactUI_2D*				m_pSmallMap2;// ���׸� �� 2

	wstring						m_wstrSelectMap;//���õ� ��
	eMapType					m_eMapType;

	bool						m_bActivate;//������ ������ �̷��� �ִ°�??
	bool						m_bRoomManager;//���� �����ΰ�?
	bool						m_bChatMode;//ä�� ���
	int							m_iRoomManagerIndex;//���� �ε���

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
