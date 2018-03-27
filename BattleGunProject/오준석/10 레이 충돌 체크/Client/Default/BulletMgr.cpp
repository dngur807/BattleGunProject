#include "stdafx.h"
#include "BulletMgr.h"
#include "UserMgr.h"
#include "User.h"
#include "Character.h"
#include "Transform.h"
#include "SphereCol.h"
#include "CharCol.h"
#include "NetMgr.h"

IMPLEMENT_SINGLETON(CBulletMgr)
CBulletMgr::CBulletMgr()
{

}

CBulletMgr::~CBulletMgr()
{

}

bool CBulletMgr::RayCast_CharSphere(XMFLOAT3* vRayPos, XMFLOAT3* vRayDir, float* fDist, CCharacter* pCharacter)
{
	XMFLOAT3 vCharPos = pCharacter->GetInfo()->m_vPos;
	vCharPos.y += 6.f;
	bool bPartCol = false;
	float fMin = 1000.f;
	float fCur = 0.f;

	if (RayCast_Sphere(vRayPos, vRayDir, &vCharPos, 6.0f))
	{
		XMFLOAT3* vSphereList = pCharacter->GetCharCol()->GetColPos();

		if (RayCast_Sphere(vRayPos, vRayDir, &vSphereList[0], 2.0f))
		{
			XMVECTOR vec = XMLoadFloat3(&vSphereList[0]) - XMLoadFloat3(vRayPos);

			vec = XMVector3Length(vec);
			fCur = XMVectorGetX(vec);
			fCur -= 2.0f;

			if (fCur < fMin)
				fMin = fCur;
			bPartCol = true;
		}
		if (RayCast_Sphere(vRayPos, vRayDir, &vSphereList[1], 1.5f))
		{
			XMVECTOR vec = XMLoadFloat3(&vSphereList[1]) - XMLoadFloat3(vRayPos);

			vec = XMVector3Length(vec);
			fCur = XMVectorGetX(vec);
			fCur -= 1.5f;

			if (fCur < fMin)
				fMin = fCur;
			bPartCol = true;
		}

		if (RayCast_Sphere(vRayPos, vRayDir, &vSphereList[2], 1.5f))
		{
			XMVECTOR vec = XMLoadFloat3(&vSphereList[2]) - XMLoadFloat3(vRayPos);

			vec = XMVector3Length(vec);
			fCur = XMVectorGetX(vec);
			fCur -= 1.5f;

			if (fCur < fMin)
				fMin = fCur;
			bPartCol = true;
		}

		if (RayCast_Sphere(vRayPos, vRayDir, &vSphereList[3], 1.5f))
		{
			XMVECTOR vec = XMLoadFloat3(&vSphereList[3]) - XMLoadFloat3(vRayPos);

			vec = XMVector3Length(vec);
			fCur = XMVectorGetX(vec);
			fCur -= 1.5f;

			if (fCur < fMin)
				fMin = fCur;
			bPartCol = true;
		}

		if (RayCast_Sphere(vRayPos, vRayDir, &vSphereList[4], 1.5f))
		{
			XMVECTOR vec = XMLoadFloat3(&vSphereList[4]) - XMLoadFloat3(vRayPos);

			vec = XMVector3Length(vec);
			fCur = XMVectorGetX(vec);
			fCur -= 1.5f;

			if (fCur < fMin)
				fMin = fCur;
			bPartCol = true;
		}
		for (int i = 5; i < 16; ++i)
		{
			if (RayCast_Sphere(vRayPos, vRayDir, &vSphereList[i], 1.2f))
			{
				XMVECTOR vec = XMLoadFloat3(&vSphereList[i]) - XMLoadFloat3(vRayPos);

				vec = XMVector3Length(vec);
				fCur = XMVectorGetX(vec);
				fCur -= 1.2f;

				if (fCur < fMin)
					fMin = fCur;
				bPartCol = true;
			}
		}
		if (bPartCol == true)
		{
			(*fDist) = fMin;
			return true;
		}
		else
			return false;	
	}
	return false;
}

bool CBulletMgr::RayCast_Sphere(XMFLOAT3* vRayPos, XMFLOAT3* vRayDir, XMFLOAT3* vDestPos, float fRadius)
{
	XMFLOAT3		vPos = *vRayPos;
	XMFLOAT3		vDir = *vRayDir;
	XMVECTOR		m = XMLoadFloat3(&vPos) - XMLoadFloat3(vDestPos);
	XMVECTOR vecDot = XMVector3Dot(m, m);
	float c = XMVectorGetX(vecDot) - (fRadius* fRadius);

	if (c <= 0)
		return true;
	vecDot =  XMVector3Dot(m, XMLoadFloat3(&vDir));
	float b = XMVectorGetX(vecDot);

	if (b > 0)
		return false;
	float d = (b* b) - c;

	if (d < 0)
		return false;
	return true;
}

void CBulletMgr::Fire_AssultBullet(XMFLOAT3* vPos, XMFLOAT3* vDir, int iIndex)
{
	float fMin = 1000.0f;
	float fCur = 10.0f;
	int iHitIndex;
	HITTARGET	eHitTarget = HIT_NONE;

	auto pMapUser = CUserMgr::GetInstance()->GetUserList();

	auto iter = pMapUser->begin();
	auto iter_end = pMapUser->end();
	for (iter; iter != iter_end; ++iter)
	{
		if (iter->second->GetUserInfo().iIndex == iIndex
			|| iter->second->GetCharacter()->GetCharInfo().eState == CHARSTATE_DEAD)
			continue;
		if (RayCast_CharSphere(vPos, vDir, &fCur, iter->second->GetCharacter()))
		{
			if (fCur < fMin)
			{
				iHitIndex = iter->second->GetUserInfo().iIndex;
				fMin = fCur;
				eHitTarget = HIT_CHARACTER;


			}
		}
	}
	if (eHitTarget == HIT_CHARACTER)
	{
		//CUserMgr::GetInstance()->GetUser(iHitIndex)->GetCharacter()->이펙트;
		if (g_bNetwork == true && g_iIndex == iIndex)
		{
#ifdef _CLIENTDEBUGING_
			printf("히트 캐릭터 Target : %d\n", iHitIndex);
#endif
			CNetMgr::GetInstance()->RequestGiveDamage(iHitIndex, ASSULTBULLET_DMG);
		}


	}

}

