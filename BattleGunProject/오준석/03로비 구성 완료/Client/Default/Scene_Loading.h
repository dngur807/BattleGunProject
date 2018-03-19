#pragma once
#include "Scene.h"
#include "Include.h"

class CScene_Loading :
	public Engine::CScene
{
private:
	explicit CScene_Loading(Engine::MYGDI* pMyGDI);
public:
	virtual ~CScene_Loading();
};

