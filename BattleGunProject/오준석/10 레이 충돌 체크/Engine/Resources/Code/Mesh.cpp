#include "Mesh.h"

Engine::CMesh::CMesh(MYGDI* pMyGDI)
	: CResources(pMyGDI)
{

}

Engine::CMesh::CMesh(const CMesh& rhs)
	: CResources(rhs), m_BoundingBox(rhs.m_BoundingBox)
{

}

Engine::CMesh::~CMesh(void)
{

}

DWORD Engine::CMesh::Release(void)
{
	VECSRV::iterator	iter = m_vecSRV.begin(),
						iter_end = m_vecSRV.end();

	for (; iter != iter_end; ++iter)
	{
		Safe_Release(*iter);
	}
	m_vecSRV.clear();
	m_vecSRV.swap(VECSRV());

	return 0;
}
