#include "Transform.h"

Engine::CTransform::CTransform(const XMFLOAT3& vLook)
	: m_vScale(1.f, 1.f, 1.f)
	, m_vPos(0.f, 0.f, 0.f)
	, m_vDir(vLook)
{
	ZeroMemory(m_fAngle, sizeof(float) * ANGLE_END);
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_matWorld, I);
}

Engine::CTransform::~CTransform(void)
{

}

int Engine::CTransform::Update(void)
{
	XMMATRIX matScale, matRotX, matRotY, matRotZ, matTrans, matWorld;

	matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	matRotX = XMMatrixRotationX(m_fAngle[ANGLE_X]);
	matRotY = XMMatrixRotationY(m_fAngle[ANGLE_Y]);
	matRotZ = XMMatrixRotationZ(m_fAngle[ANGLE_Z]);
	matTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
	XMStoreFloat4x4(&m_matWorld, matWorld);

	XMVECTOR VD = XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 1.f));
	VD = XMVector3TransformNormal(VD,matRotY);
	XMStoreFloat3(&m_vDir, VD);

	return 0;
}

Engine::CTransform* Engine::CTransform::Create(const XMFLOAT3& vLook)
{
	return new CTransform(vLook);
}

DWORD Engine::CTransform::Release(void)
{
	return 0;
}
