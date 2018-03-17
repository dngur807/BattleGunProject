
#ifndef Loading_h__
#define Loading_h__

#include "Engine_Include.h"

class CLoading
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_STAGE1, LOADING_STAGE2 };

private:
	explicit CLoading(Engine::MYGDI* pMyGDI, LOADINGID eLoadID);

public:
	~CLoading(void);

public:
	const bool GetComplete(void) { return m_bComplete; }
	const TCHAR* GetLoadMessage(void) { return m_szLoadingMessage; }

public:
	HRESULT InitLoading(void);
	HRESULT StageLoading(void);
	HRESULT StageLoading1(void);
	HRESULT StageLoading2(void);

public:
	static CLoading* Create(Engine::MYGDI* pMyGDI, LOADINGID eLoadID);

private:
	static UINT WINAPI LoadingFunction(void* pArg);

private:
	void Release(void);

private:
	Engine::MYGDI*			m_pMyGDI;
	LOADINGID				m_eLoadID;
	CRITICAL_SECTION		m_CSKey;
	HANDLE					m_hThread;

private:
	TCHAR					m_szLoadingMessage[256];
	bool					m_bComplete;
};

#endif // Loading_h__