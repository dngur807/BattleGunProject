
#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Renderer.h"

BEGIN(Engine)

class CScene;

class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)
private:
	CManagement(void);
	~CManagement(void);

public:
	HRESULT InitManagement(MYGDI* pMyGDI);
	void Update(void);
	void Render(const float& fTime);

	inline CScene* GetScene()
	{
		return m_pScene;
	}
	inline MYGDI* GetGDI()
	{
		return m_pMyGDI;
	}
public:
	template <typename T>
	HRESULT SceneChange(T& Functor);

private:
	void Release(void);

private:
	CScene*		m_pScene;
	CRenderer*	m_pRenderer;

private:
	MYGDI*		m_pMyGDI;
};

template <typename T>
HRESULT Engine::CManagement::SceneChange(T& Functor)
{
	if (m_pScene != NULL)
		Engine::Safe_Delete(m_pScene);

	FAILED_CHECK_RETURN(Functor(&m_pScene, m_pMyGDI), E_FAIL);

	m_pRenderer->SetCurrentScene(m_pScene);
	return S_OK;
}

END

#endif // Management_h__