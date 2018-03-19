#ifndef Login_h__
#define Login_h__

#include "Scene.h"

namespace Engine
{
	class CResourceMgr;
	class CManagement;
}
class CReactUI_2D;

class CLogoBack;
class CText;

class CLogin
	: public Engine::CScene
{
public:
	enum LAYERID { LAYER_UI };
	enum MenuType {MENU_NONE , MENU_LOGIN , MENU_QUIT};
private:
	explicit CLogin(Engine::MYGDI* pMyGDI);
public:
	virtual ~CLogin();
public:
	virtual HRESULT InitScene() ;
	virtual int Update();
	virtual void Render();

	static CLogin*	Create(Engine::MYGDI* pMyGDI);

	//서버로 부터 온 메시지 분석
	void AnalysisInterMsg();
private:
	HRESULT	Add_UI_Layer(void);
	void Release();
private:
	MenuType					m_eMenuType;

	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::CManagement*		m_pManagement;


	CReactUI_2D*				m_pMainBack;
	CReactUI_2D*				m_pMainTitle;

	CReactUI_2D*				m_pIDInput;
	CReactUI_2D*				m_pIPInput;

	//Lobby UI
	CReactUI_2D*				m_pBackBtn;
	//Start Btn
	CReactUI_2D*				m_pStartBtn;

	CText*						m_pIDText;
	CText*						m_pIPText;
	CText*						m_pPlayerNameText;
	CText*						m_pIPNameText;
	bool						m_bLoading;//로딩 확인용
	CLogoBack*					m_LogoBack;

	RECT						m_rtIDInput;
	RECT						m_rtIPInput;
};

#endif // Login_h__
