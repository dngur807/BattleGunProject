#pragma once
#include "Subject.h"

BEGIN(Engine)

typedef list<void*>			DATALIST;
typedef map<int, DATALIST>		MAPDATALIST;

class ENGINE_DLL CInfoSubject :
	public CSubject
{
public:
	DECLARE_SINGLETON(CInfoSubject)

private:
	CInfoSubject();
	virtual ~CInfoSubject();

private:
	void Release(void);

public:
	const list<void*>* GetDatalist(int iMsg);

public:
	void AddData(int iMsg, void* pData);
	void RemoveData(int iMsg, void* pData);

private:
	MAPDATALIST		m_mapDatalist;
};

END