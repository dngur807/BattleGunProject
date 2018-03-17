#include "StdAfx.h"
#include "ObjectCol.h"

#include "Include.h"
#include "ObjectObserver.h"
#include "Export_Function.h"
#include "Transform.h"

CObjectCol::CObjectCol(Engine::MYGDI* pMyGDI)
	:CCollision(pMyGDI),
	m_pObjectObserver(NULL)
{
}

CObjectCol::~CObjectCol(void)
{
	Release();
}

HRESULT CObjectCol::Initialize(void)
{
	m_pObjectObserver = CObjectObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pObjectObserver);

	return S_OK;
}

DWORD CObjectCol::Release(void)
{
	if (0 == m_dwRefCnt)
	{
		Safe_Delete(m_pObjectObserver);
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

int CObjectCol::Update(void)
{
	const list<Engine::CTransform*>* pInfolist = m_pObjectObserver->GetObjInfoList();
	
	if (Engine::Get_InfoSubject()->GetDatalist(MSG_PLAYERINFO) != nullptr)
		m_pPlayerInfo = S_CAST(Engine::CTransform*, Engine::Get_InfoSubject()->GetDatalist(MSG_PLAYERINFO)->front());

	if (NULL == pInfolist)
		return 1;

	list<Engine::CTransform*>::const_iterator iter_begin = pInfolist->begin();
	list<Engine::CTransform*>::const_iterator iter_end = pInfolist->end();
	list<Engine::CTransform*>::const_iterator iter = iter_begin;

	//## 플레이어 - 몬스터 충돌 체크
	//bool bClsn = false;
	if (Engine::Get_InfoSubject()->GetDatalist(MSG_PLAYERINFO) != nullptr)
	{
		for (; iter_begin != iter_end; ++iter_begin)
		{
			if (CheckClsn(m_pPlayerInfo, *iter_begin))
			{
				//cout << "충돌하고있어" << endl;
				//(*iter_begin)->m_vPos.y += 3.f;
				(*iter_begin)->m_tPlayer.iHp -= 10;
			}
		}
	}

	//iter_begin = pInfolist->begin();
	////## 몬스터끼리 충돌 체크
	//++iter;
	//for (; iter != iter_end;)
	//{
	//	CheckClsn(*iter_begin, *iter);
	//	++iter;

	//	if (iter == iter_end)
	//	{
	//		++iter_begin;
	//		iter = iter_begin;
	//		++iter;
	//	}
	//}

	return 0;
}

bool CObjectCol::CheckClsn(Engine::CTransform* pDstInfo, Engine::CTransform* pSrcInfo)
{
	//! 바운딩박스 센터의 월드 좌표를 구한다.
	XMVECTOR vDstCenter = XMLoadFloat3(&pDstInfo->m_vCenter);
	vDstCenter = XMVector3TransformCoord(vDstCenter, XMLoadFloat4x4(&pDstInfo->m_matWorld));
	XMVECTOR vSrcCenter = XMLoadFloat3(&pSrcInfo->m_vCenter);
	vSrcCenter = XMVector3TransformCoord(vSrcCenter, XMLoadFloat4x4(&pSrcInfo->m_matWorld));

	//! 위치값을 제외한 회전행렬을 생성
	XMMATRIX matDstRot = XMLoadFloat4x4(&pDstInfo->m_matWorld);
	matDstRot.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMMATRIX matSrcRot = XMLoadFloat4x4(&pSrcInfo->m_matWorld);
	matSrcRot.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	//! 회전행렬을 쿼터니언으로 변환
	XMVECTOR vDstQuat = XMQuaternionRotationMatrix(matDstRot);
	XMVECTOR vSrcQuat = XMQuaternionRotationMatrix(matSrcRot);

	XMVECTOR Q = XMQuaternionMultiply(vDstQuat, XMQuaternionConjugate(vSrcQuat));
	XMMATRIX R = XMMatrixRotationQuaternion(Q);

	// Compute the translation of src relative to Dst
	XMVECTOR t = XMVector3InverseRotate(vSrcCenter - vDstCenter, vDstQuat);

	//h(A) = extents of A
	//h(B) = extents of B

	//a(u) = axes of A = (1, 0, 0), (0, 1, 0), (0, 0, 1)
	//b(u) = axes of B relative to A = (r00, r10, r20, (r01, r11, r21), (r02, r12, r22)

	// For each possible separating axis l:
	//	d(A) = sum(for i = u, v, w) h(A)(i) * abs( a(i) dot l)
	//	d(B) = sum(for i = u, v, w) h(B)(i) * abs( b(i) dot l)
	//	if abs(t dot l) > d(A) + d(B) then disjoint

	// Load extents of A and B
	XMVECTOR h_A = XMLoadFloat3(&pDstInfo->m_vExt);
	XMVECTOR h_B = XMLoadFloat3(&pSrcInfo->m_vExt);

	// Rows. Note R[0,1,2]X.w = 0
	XMVECTOR R0X = R.r[0];
	XMVECTOR R1X = R.r[1];
	XMVECTOR R2X = R.r[2];

	R = XMMatrixTranspose(R);

	// Columns. Note RX[0,1,2].w = 0.
	XMVECTOR RX0 = R.r[0];
	XMVECTOR RX1 = R.r[1];
	XMVECTOR RX2 = R.r[2];

	// Absolute value of rows.
	XMVECTOR AR0X = XMVectorAbs(R0X);
	XMVECTOR AR1X = XMVectorAbs(R1X);
	XMVECTOR AR2X = XMVectorAbs(R2X);

	// Absolute value of columns
	XMVECTOR ARX0 = XMVectorAbs(RX0);
	XMVECTOR ARX1 = XMVectorAbs(RX1);
	XMVECTOR ARX2 = XMVectorAbs(RX2);

	// Test each of the 15 possible seperating axii
	XMVECTOR d, d_A, d_B;

	// l = a(u) = (1, 0, 0)
	// t dot l = t.x
	// d(A) = h(A).x
	// d(B) = h(B) dot abs(r00, r01, r02)
	d = XMVectorSplatX(t);
	d_A = XMVectorSplatX(h_A);
	d_B = XMVector3Dot(h_B, AR0X);
	XMVECTOR NoIntersection = XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B));

	// l = a(v) = (0, 1, 0)
	// t dot l = t.y
	// d(A) = h(A).y
	// d(B) = h(B) dot abs(r10, r11, r12)
	d = XMVectorSplatY(t);
	d_A = XMVectorSplatY(h_A);
	d_B = XMVector3Dot(h_B, AR1X);
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(w) = (0, 0, 1)
	// t dot l = t.z
	// d(A) = h(A).z
	// d(B) = h(B) dot abs(r20, r21, r22)
	d = XMVectorSplatZ(t);
	d_A = XMVectorSplatZ(h_A);
	d_B = XMVector3Dot(h_B, AR2X);
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = b(u) = (r00, r10, r20)
	// d(A) = h(A) dot abs(r00, r10, r20)
	// d(B) = h(B).x
	d = XMVector3Dot(t, RX0);
	d_A = XMVector3Dot(h_A, ARX0);
	d_B = XMVectorSplatX(h_B);
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = b(v) = (r01, r11, r2)
	// d(A) = h(A) dot abs(r01, r11, r21)
	// d(B) = h(B).y
	d = XMVector3Dot(t, RX1);
	d_A = XMVector3Dot(h_A, ARX1);
	d_B = XMVectorSplatY(h_B);
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = b(w) = (r02, r12, r22)
	// d(A) = h(A) dot abs(r02, r12, r22)
	// d(B) = h(B).z
	d = XMVector3Dot(t, RX2);
	d_A = XMVector3Dot(h_A, ARX2);
	d_B = XMVectorSplatZ(h_B);
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(u) x b(u) = (0, -r20, r10)
	// d(A) = h(A) dot abs(0, r20, r10)
	// d(B) = h(B) dot abs(0, r02, r01)
	d = XMVector3Dot(t, XMVectorPermute<XM_PERMUTE_0W, XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_0X>(RX0, -RX0));
	d_A = XMVector3Dot(h_A, XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(ARX0));
	d_B = XMVector3Dot(h_B, XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(AR0X));
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(u) x b(v) = (0, -r21, r11)
	// d(A) = h(A) dot abs(0, r21, r11)
	// d(B) = h(B) dot abs(r02, 0, r00)
	d = XMVector3Dot(t, XMVectorPermute<XM_PERMUTE_0W, XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_0X>(RX1, -RX1));
	d_A = XMVector3Dot(h_A, XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(ARX1));
	d_B = XMVector3Dot(h_B, XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(AR0X));
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(u) x b(w) = (0, -r22, r12)
	// d(A) = h(A) dot abs(0, r22, r12)
	// d(B) = h(B) dot abs(r01, r00, 0)
	d = XMVector3Dot(t, XMVectorPermute<XM_PERMUTE_0W, XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_0X>(RX2, -RX2));
	d_A = XMVector3Dot(h_A, XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(ARX2));
	d_B = XMVector3Dot(h_B, XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(AR0X));
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(v) x b(u) = (r20, 0, -r00)
	// d(A) = h(A) dot abs(r20, 0, r00)
	// d(B) = h(B) dot abs(0, r12, r11)
	d = XMVector3Dot(t, XMVectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_1X, XM_PERMUTE_0Y>(RX0, -RX0));
	d_A = XMVector3Dot(h_A, XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(ARX0));
	d_B = XMVector3Dot(h_B, XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(AR1X));
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(v) x b(v) = (r21, 0, -r01)
	// d(A) = h(A) dot abs(r21, 0, r01)
	// d(B) = h(B) dot abs(r12, 0, r10)
	d = XMVector3Dot(t, XMVectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_1X, XM_PERMUTE_0Y>(RX1, -RX1));
	d_A = XMVector3Dot(h_A, XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(ARX1));
	d_B = XMVector3Dot(h_B, XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(AR1X));
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(v) x b(w) = (r22, 0, -r02)
	// d(A) = h(A) dot abs(r22, 0, r02)
	// d(B) = h(B) dot abs(r11, r10, 0)
	d = XMVector3Dot(t, XMVectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_1X, XM_PERMUTE_0Y>(RX2, -RX2));
	d_A = XMVector3Dot(h_A, XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(ARX2));
	d_B = XMVector3Dot(h_B, XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(AR1X));
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(w) x b(u) = (-r10, r00, 0)
	// d(A) = h(A) dot abs(r10, r00, 0)
	// d(B) = h(B) dot abs(0, r22, r21)
	d = XMVector3Dot(t, XMVectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_0Z>(RX0, -RX0));
	d_A = XMVector3Dot(h_A, XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(ARX0));
	d_B = XMVector3Dot(h_B, XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(AR2X));
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(w) x b(v) = (-r11, r01, 0)
	// d(A) = h(A) dot abs(r11, r01, 0)
	// d(B) = h(B) dot abs(r22, 0, r20)
	d = XMVector3Dot(t, XMVectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_0Z>(RX1, -RX1));
	d_A = XMVector3Dot(h_A, XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(ARX1));
	d_B = XMVector3Dot(h_B, XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(AR2X));
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// l = a(w) x b(w) = (-r12, r02, 0)
	// d(A) = h(A) dot abs(r12, r02, 0)
	// d(B) = h(B) dot abs(r21, r20, 0)
	d = XMVector3Dot(t, XMVectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_0Z>(RX2, -RX2));
	d_A = XMVector3Dot(h_A, XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(ARX2));
	d_B = XMVector3Dot(h_B, XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(AR2X));
	NoIntersection = XMVectorOrInt(NoIntersection,
		XMVectorGreater(XMVectorAbs(d), XMVectorAdd(d_A, d_B)));

	// No seperating axis found, boxes must intersect.
	return XMVector4NotEqualInt(NoIntersection, XMVectorTrueInt()) ? true : false;
}

Engine::CCollision* CObjectCol::Clone(void)
{
	CCollision* pCollision = new CObjectCol(*this);

	++(m_dwRefCnt);

	return pCollision;
}

CObjectCol* CObjectCol::Create(Engine::MYGDI* pMyGDI)
{
	CObjectCol* pCollision = new CObjectCol(pMyGDI);
	if (FAILED(pCollision->Initialize()))
	{
		Safe_Delete(pCollision);
	}

	return pCollision;
}
