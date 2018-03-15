#pragma once

#include "engine_include.h"

BEGIN(Engine)

class CPrimitive;

class ENGINE_DLL CPrimitiveFactory
{
public:
	CPrimitiveFactory(MYGDI* pMyGDI);
	~CPrimitiveFactory();

	void AddPrimitive(string name, CPrimitive* primitive);

	CPrimitive* CreateBox();
	CPrimitive* CreateGrid(float width, float depth, UINT m, UINT n);
	CPrimitive* CreateQuad();
	CPrimitive* CreateSphere(float radius, UINT sliceCount, UINT stackCount);
	//CPrimitive* CreateTerrain(Terrain* terrain);

private:
	MYGDI*		m_pMyGDI;
private:
	map<string, CPrimitive*> mPrimitiveMap;
};

END	