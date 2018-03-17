#pragma once

#include "Include.h"

class CMouse abstract
{
private:
	static POINT m_ptMouse;	

public:
	static POINT GetMousePos(void);
	static void SetMousePos(void);
	
};
