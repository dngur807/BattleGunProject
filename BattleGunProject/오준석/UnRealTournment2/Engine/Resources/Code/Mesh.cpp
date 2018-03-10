#include "Mesh.h"
#include "VIBuffer.h"
#include "Transform.h"

Engine::CMesh::CMesh(MYGDI* pMyGDI)
	: CResources(pMyGDI)
{
	m_pInfo = Engine::CTransform::Create(XMFLOAT3(0.f, 0.f, 1.f));
	NULL_CHECK(m_pInfo);
}

Engine::CMesh::CMesh(const CMesh& rhs)
	: CResources(rhs), m_BoundingBox(rhs.m_BoundingBox)
{
	m_pInfo = Engine::CTransform::Create(XMFLOAT3(0.f, 0.f, 1.f));
	NULL_CHECK(m_pInfo);
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



void Engine::CMesh::SetvPosInfo(XMFLOAT3 _pos)
{
	m_pInfo->m_vPos = _pos;
	m_pInfo->Update();
}
void Engine::CMesh::SetvSizeInfo(XMFLOAT3 _size)
{
	m_pInfo->m_vScale = _size;
	m_pInfo->Update();
}

void Engine::CMesh::SetAngleInfo(float _x, float _y, float _z)
{
	m_pInfo->m_fAngle[Engine::ANGLE_X] = _x;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = _y;
	m_pInfo->m_fAngle[Engine::ANGLE_Z] = _z;

	m_pInfo->Update();
}

void Engine::CMesh::SetMeshNum(DWORD dNum)
{
	m_dwMeshNum = dNum;
}


//void Engine::CMesh::SetObjOption(OBJOPTION dNum)
//{
//	m_pOption = dNum;
//
//}
