#ifndef RectUI_2D_h__
#define RectUI_2D_h__


#include "GameObject.h"
#include "Include.h"

// 로그인 버튼 및 클릭 할 수 있는 UI
namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CReactUI_2D
	: public Engine::CGameObject
{
private:
	CReactUI_2D(Engine::MYGDI* pMyGDi);
public:
	virtual ~CReactUI_2D();
public:
	virtual HRESULT Initialize(RESOURCETYPE eType, TCHAR* pResourceKey);
	virtual int Update();
	virtual void Render();

	bool IsMouseIn();//마우스 들어왔는지 확인
	void SetPos(float fX, float fY);
	void SetSize(float fX, float fY);
	void SetScale(float fX, float fY);

	void UIRoationZ(float fAngle , float fScaleX = 0.3f, float fScaleY= 0.3f);
private:
	HRESULT AddComponent(RESOURCETYPE eType, TCHAR* pResourceKey);
public:
	static CReactUI_2D* Create(Engine::MYGDI* pMyGDi, RESOURCETYPE eType, TCHAR* pResourceKey);
private:
	Engine::CTransform*			m_pInfo;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CTexture*			m_pTexture;
	RECT						m_rt;

	float		m_fPosX;
	float		m_fPosY;
	float		m_fSizeX;
	float		m_fSizeY;
private:
	Material		m_tMtrl;
	DirectionalLight			m_DirLight;

};
#endif // RectUI_2D_h__
