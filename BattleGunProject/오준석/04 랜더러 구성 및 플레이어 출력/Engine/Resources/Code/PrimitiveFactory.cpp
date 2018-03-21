#include "PrimitiveFactory.h"



Engine::CPrimitiveFactory::CPrimitiveFactory(MYGDI* pMyGDI)
	:m_pMyGDI(pMyGDI)
{
}


Engine::CPrimitiveFactory::~CPrimitiveFactory()
{
}

void Engine::CPrimitiveFactory::AddPrimitive(string name, CPrimitive * primitive)
{
}

Engine::CPrimitive * Engine::CPrimitiveFactory::CreateBox()
{
	return nullptr;
}

Engine::CPrimitive * Engine::CPrimitiveFactory::CreateGrid(float width, float depth, UINT m, UINT n)
{
	return nullptr;
}

Engine::CPrimitive * Engine::CPrimitiveFactory::CreateQuad()
{
	return nullptr;
}

Engine::CPrimitive * Engine::CPrimitiveFactory::CreateSphere(float radius, UINT sliceCount, UINT stackCount)
{
	return nullptr;
}
