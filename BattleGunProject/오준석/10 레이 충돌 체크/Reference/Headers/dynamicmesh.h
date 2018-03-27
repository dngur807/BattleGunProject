#pragma once
#include "Mesh.h"

BEGIN(Engine)

class CSkinnedModel;
class CSceneAnimator;
class CPrimitiveFactory;

class ENGINE_DLL CDynamicMesh :
	public CMesh
{
private:
	explicit CDynamicMesh(MYGDI* pMyGDI);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
public:
	virtual ~CDynamicMesh();

private:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pName);
	vector<Weights> CalculateWeights(aiMesh* mesh, CSceneAnimator* animator);
	int FindValidPath(aiString* p_szString);
	bool TryLongerPath(char* szTemp, aiString* p_szString);

public:
	void SetAnimation(int iIndex);
	void InitElapsedTime(void) { m_fElapsedTime = 0.f; }

public:
	UINT GetTrackPos(void);
	UINT GetNumFrames(void);
	bool GetChangeScene(void);
	float* GetAccumWeight(void);
	XMMATRIX& GetAllMatrix(UINT iIndex);

	const XMFLOAT4X4* FindFrame( const std::string strFrameName);
public:
	virtual int Update(void);
	virtual void RenderMesh(const XMMATRIX& pWorldMatrix);
	virtual DWORD Release(void);

public:
	virtual CResources* CloneResource(void);

public:
	static CDynamicMesh* Create(MYGDI* pMyGDI, const TCHAR* pPath, const TCHAR* pName);

private:
	CSkinnedModel* m_pSkinnedModel;
	CSceneAnimator* m_pAnimator;
	int		m_iCurAnimIndex;
	float	m_fElapsedTime;

private:
	map<string, CSkinnedModel*> m_mapSkinnedModel;
	string						m_strFileName;

	float						m_fAnimationTime;
public:
	void SetAnimationTime(float fSetTime = 1.0f) {
		m_fAnimationTime = fSetTime;
	}
	void SetTrackPos(float fPos);
	float GetAnimationTime() { return m_fAnimationTime; }
	float GetElapsedTime() { return m_fElapsedTime; }
	void SetElapsedTime(float time) { m_fElapsedTime = time; }
	float GetDuration();

	float GetTickPerSec();
	float GetLastTime();

	float GetPeriod()
	{
		return GetTickPerSec() / GetDuration();
	}
	

};

END