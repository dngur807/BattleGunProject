#include "Component.h"



Engine::CComponent::CComponent()
	:m_dwRefCnt(0)
{
}

Engine::CComponent::CComponent(const CComponent & rhs)
	:m_dwRefCnt(rhs.m_dwRefCnt)
{
	++m_dwRefCnt;
}


Engine::CComponent::~CComponent()
{
}

int Engine::CComponent::Update(void)
{
	return 0;
}

void Engine::CComponent::AddRefCnt(void)
{
	++m_dwRefCnt;
}
