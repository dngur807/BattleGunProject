#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "Stage.h"
#include "Logo.h"
#include "Login.h"
#include "Lobby.h"
#include "Scene_Loading.h"
#include "Ingame.h"

class CSceneSelector
{
public:
	enum SCENE { SCENE_LOGO , SCENE_LOGIN , SCENE_LOBBY , SCENE_LOADING , SCENE_INGAME, SCENE_STAGE, SCENE_END };
public:
	explicit CSceneSelector(SCENE eSceneID) : m_eSceneID(eSceneID) {}
	~CSceneSelector(void) {}
public:
	HRESULT operator () (Engine::CScene** ppScene, Engine::MYGDI* pMyGDI)
	{
		switch (m_eSceneID)
		{
		case SCENE_LOGO:
			*ppScene = CLogo::Create(pMyGDI);
			break;
		case SCENE_LOGIN:
			*ppScene = CLogin::Create(pMyGDI);
			break;
		case SCENE_LOBBY:
			*ppScene = CLobby::Create(pMyGDI);
			break;
		case SCENE_STAGE:
			*ppScene = CStage::Create(pMyGDI);
			break;
		case SCENE_INGAME:
			*ppScene = CIngame::Create(pMyGDI);
			break;
		case SCENE_LOADING:
			*ppScene = CScene_Loading::Create(pMyGDI);
			break;
		}

		NULL_CHECK_RETURN(*ppScene, E_FAIL);

		return S_OK;
	}
private:
	SCENE			m_eSceneID;
};


#endif // SceneSelector_h__
