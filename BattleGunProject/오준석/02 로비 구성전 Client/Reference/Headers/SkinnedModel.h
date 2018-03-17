#pragma once
#include "engine_include.h"

BEGIN(Engine)

class CSkinnedMesh;
class CSceneAnimator;
//struct Material;

typedef std::vector<CSkinnedMesh*> VECSKINNEDMESH;

class ENGINE_DLL CSkinnedModel
{
public:
	explicit CSkinnedModel(MYGDI* pMyGDI);
	~CSkinnedModel();

public:
	void Update(float fdt);
	void Draw(const XMMATRIX& matWorld);
	void Save(string strFileName);
	void Load(string strFileName);
	void AddMesh(CSkinnedMesh* pMesh);
	void SetAnimator(CSceneAnimator* pAnimator);

public:
	void SetAnimation(string strAniName);
	void SetFileName(string strFileName);
	void SetAnimation(int iIndex);
	void SetElapsedTime(float fElapsedTime);
	void SetMeshMaterial(int iMeshID, Material tMtrl);
	void AdjustAnimationSpeedBy(float fPercent);

public:
	VECSKINNEDMESH* GetMeshes(void);
	vector<XMFLOAT4X4> GetFinalTransforms(void);
	BoundingBox GetBoundingBox(void);
	string GetFileName(void);
	void CalculateAABB(void);
	int GetCurrentAnimation(void);
	float GetAnimationSpeed(void);

public:
	void AddAnimation(int iStartFrame, int iEndFrame, float fTotalFrames, float fLength);
	void AddAnimation(float fStartTime, float fEndTime);

private:
	void Release(void);

private:
	MYGDI*	m_pMyGDI;
private:
	VECSKINNEDMESH m_vecMesh;
	CSceneAnimator* m_pAnimator;

	float m_fElapsedTime;
	std::string m_strFileName;
	BoundingBox m_AABB;

	vector<ANITIME> m_vecAnitime;
};

END