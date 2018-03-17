#include "Resources.h"



Engine::CResources::CResources(MYGDI* pMyGDI)
	:m_pMyGDI(pMyGDI)
{
}

Engine::CResources::CResources(const CResources & rhs)
	:CComponent(rhs), m_pMyGDI(rhs.m_pMyGDI)
{
}


Engine::CResources::~CResources()
{
}