#include "Line2D.h"



Engine::CLine2D::CLine2D()
{
}


Engine::CLine2D::~CLine2D()
{
}

HRESULT Engine::CLine2D::InitLine(const XMFLOAT3 * pStartPoint, const XMFLOAT3 * pEndPoint)
{
	m_vStartPoint = *pStartPoint;
	m_vEndPoint = *pEndPoint;

	m_vNormal = XMFLOAT2(m_vStartPoint.z - m_vEndPoint.z,
		m_vEndPoint.x - m_vStartPoint.x);

	XMVECTOR vDir = XMLoadFloat3(&m_vEndPoint) - XMLoadFloat3(&m_vStartPoint);
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
