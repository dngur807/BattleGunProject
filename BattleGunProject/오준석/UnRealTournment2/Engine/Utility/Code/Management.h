/*!
* \file Management.h
* \date 2015/04/02 22:59
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/
#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Renderer.h"

BEGIN(Engine)

class CScene;
class CComponent;
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

public:
	//const CComponent* GetLayerComponent(WORD LayerID, const wstring& wstrObject, const wstring& wstrComponentKey);
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