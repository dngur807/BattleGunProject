#include "Line2D.h"



Engine::CLine2D::CLine2D()
{
}


Engine::CLine2D::~CLine2D()
{
}

HRESULT Engine::CLine2D::InitLine(const XMFLOAT3 * pStartPoint, const XMFLOAT3 * pEndPoint)
{
	m_vStartPoint = /**pStartPoint;*/ XMFLOAT2(pStartPoint->x, pStartPoint->z);
	m_vEndPoint = /**pEndPoint;*/ XMFLOAT2(pEndPoint->x, pEndPoint->z);

	m_vNormal = XMFLOAT2(m_vStartPoint.y - m_vEndPoint.y,
		m_vEndPoint.x - m_vStartPoint.x);

	//

	XMVECTOR vDir = XMLoadFloat2(&m_vEndPoint) - XMLoadFloat2(&m_vStartPoint);
	XMStoreFloat2(&m_vNormal, XMVector2Normalize(XMLoadFloat2(&m_vNormal)));

	return S_OK;
}

Engine::CLine2D * Engine::CLine2D::Create(const XMFLOAT3 * pStartPoint, const XMFLOAT3 * pEndPoint)
{
	CLine2D* pLine = new CLine2D;
	if (FAILED(pLine->InitLine(pStartPoint, pEndPoint)))
		Safe_Delete(pLine);

	return pLine;
}
bool Engine::CLine2D::CheckLinePass(const XMFLOAT2* pMoveEndPoint)
{
	//XMFLOAT2 vDir = *pMoveEndPoint - m_vStartPoint;

	//XMVECTOR vDir = XMLoadFloat2(&(*pMoveEndPoint) - XMLoadFloat2(&m_vStartPoint);

	XMVECTOR vDir = XMLoadFloat2(pMoveEndPoint) - XMLoadFloat2(&m_vStartPoint);

	float	fDotResult;
	//fDotResult = XMVector2Dot(&vDir, &m_vNormal);
	XMStoreFloat(&fDotResult, XMVector2Dot(vDir, XMLoadFloat2(&m_vNormal)));

	//XMStoreFloat(&fDot, XMVector2Dot(vPointToPos, vNormal));
	//if (0 < fDot)
	//	return false;

	if (fDotResult > 0)
		return true;
	else
		return false;
}
