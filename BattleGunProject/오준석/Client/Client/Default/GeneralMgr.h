#pragma once
#include "Engine_Include.h"
#include "Include.h"

class CGeneralMgr
{
public:
	DECLARE_SINGLETON(CGeneralMgr)
private:
	CGeneralMgr();
	~CGeneralMgr();

public:
	void Update();
public:
	void AliveKeyCheck(void);
	void DaedKeyCehck();
private:
	int		m_iCameraMode;
};