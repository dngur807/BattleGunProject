#pragma once
#include "Engine_Include.h"
#include "Include.h"

class CCharacter;

class CBulletMgr
{
	DECLARE_SINGLETON (CBulletMgr)
private:
	CBulletMgr();
	~CBulletMgr();
public:
	void Initialize();
	bool RayCast_CharSphere(XMFLOAT3* vRayPos, XMFLOAT3* vRayDir, float* fDist, CCharacter* pCharacter);
	bool RayCast_Sphere(XMFLOAT3* vRayPos, XMFLOAT3* vRayDir, XMFLOAT3* vDestPos, float fRadius);
	// 어설트 라이플
	void Fire_AssultBullet(XMFLOAT3* vPos, XMFLOAT3* vDir, int iIndex);

};