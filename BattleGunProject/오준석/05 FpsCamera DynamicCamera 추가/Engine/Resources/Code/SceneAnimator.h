#pragma once

#include "engine_include.h"

BEGIN(Engine)

class ENGINE_DLL CBone 
{
public:
	CBone() : m_pParent(nullptr) 
	{
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&m_matOffset, I);
	}
	~CBone() {
		for (size_t i(0); i < m_vecChild.size(); i++)
			Safe_Delete(m_vecChild[i]);

		m_vecChild.clear();
		m_vecChild.swap(vector<CBone*>());
	}
	


public:
	std::string m_strName;
	XMFLOAT4X4 m_matOffset, m_matLocal, m_matGlobal, m_OriginalLocal;
	CBone* m_pParent;
	std::vector<CBone*> m_vecChild;
};

typedef struct tagAnimmationChannel
{
	std::string strName;
	std::vector<aiVectorKey>	vecPosKey;
	std::vector<aiQuatKey>		vecRotKey;
	std::vector<aiVectorKey>	vecScaleKey;
}ANIMCHANNEL;

class CAnimEvaluator
{
public:
	CAnimEvaluator()
		:m_fLastTime(0.f), m_fTicksPerSec(0.f), m_fDuration(0.f), m_bPlayForward(true)
	{}
	CAnimEvaluator(const aiAnimation* pAnim);
	void Evaluate(float fPtime, std::map<std::string, CBone*>& pmapBone);
	void Save(std::ofstream& file);
	void Load(std::ifstream& file);
	UINT GetFrameIndexAt(float fPtime);
	std::vector<XMFLOAT4X4>& GetTransform(float fdt)
	{
		return m_vecTransform[GetFrameIndexAt(fdt)];
	}
	vector<XMFLOAT4X4>& GetAllTrans(UINT iCurFrame)
	{
		return m_vecAllTrans[iCurFrame];
	}

public:
	std::string m_strName;
	std::vector<ANIMCHANNEL> m_vecChannel;
	bool m_bPlayForward; // forward == true, backword == false
	float m_fLastTime, m_fTicksPerSec, m_fDuration;
	std::vector<std::tuple<UINT, UINT, UINT>> m_vecLastPos;
	std::vector<std::vector<XMFLOAT4X4>> m_vecTransform;
	std::vector<std::vector<XMFLOAT4X4>> m_vecAllTrans;
};

class CSkinnedModel;

class CSceneAnimator
{
public:
	CSceneAnimator();
	~CSceneAnimator();

public:
	void Init(const aiScene* pScene, CSkinnedModel* pSkinnedModel); // this must be called to fill the SceneAnimator with valid data
	void Release(void);	// frees all memory and initializes everything to a default state
	void Save(std::ofstream& file);
	void Load(std::ifstream& file);
	bool HasSkeleton(void) const { return !m_vecBones.empty(); } // lets the caller know if there is a skeleton present
	//the set animation returns whether the animation changed or is still the same
	bool SetAnimIndex(int32_t iAnimIndex); // this takes an index to set the current animation to
	bool SetAnimation(const std::string& strName); // this takes a string to set the animation to, i.e. SetAnimation("Idle");
	//the nex two functions are good if you want to change the direction of the current animation. You could use a forward walking animation and reverse it to get a walking backwards
	void PlayAnimationForward(void) { m_vecAnimations[m_iCurAnimIndex].m_bPlayForward = true; }
	void PlayAnimationBackward(void) { m_vecAnimations[m_iCurAnimIndex].m_bPlayForward = false; }
	// this function will adjust the current animations speed by a percentage.
	// So, passing 100, would do nothing, passing 50, would decrease the speed by half, and 150 increase it by 50%
	void AdjustAnimationSpeedBy(float fPercent) { m_vecAnimations[m_iCurAnimIndex].m_fTicksPerSec *= fPercent / 100.f; }
	// this will set the animation speed
	void AdjustAnimationSpeedTo(float fTicksPerSec) { m_vecAnimations[m_iCurAnimIndex].m_fTicksPerSec = fTicksPerSec; }
	// get the animationspeed... in ticks per second
	float GetAnimationSpeed() const { return m_vecAnimations[m_iCurAnimIndex].m_fTicksPerSec; }
	//get the transforms needed to pass to the vertex shader. this will wrap the dt value passed, so it is safe to pass 50000000 as a valid number
	std::vector<XMFLOAT4X4>& GetTransform(float fdt) 
	{ 
		return m_vecAnimations[m_iCurAnimIndex].GetTransform(fdt);
		//return m_vecAnimations[m_iCurAnimIndex].GetTransform(fdt); 
	}

	int32_t GetAnimationIndex(void) const { return m_iCurAnimIndex; }
	std::string GetAnimationName(void) const { return m_vecAnimations[m_iCurAnimIndex].m_strName; }
	// GetBoneIndex will return the index of the bone given its name.
	// the index can be used to index directly into the vector returned from GetTransform
	int GetBoneIndex(const std::string& strBoneName);
	// GetBoneTransform will return the matrix of the bone given its name and the time.
	// be careful with this to make sure and the correct dt. If the dt is diefferent from what the model is currently at, the transform will be off
	XMFLOAT4X4 GetBoneTransform(float fdt, const std::string& strBoneName);
	// same as above, except tackes the index
	XMFLOAT4X4 GetBoneTransform(float fdt, UINT iBoneIndex);
	// 보간 관련 값들을 초기화시켜주는 함수
	void InitInterpolation(void);
	void CalcAccumulateWeight(void);
	void ChangeScene(void);
	// 서브셋을 구성하지 않는 본의 행렬을 불러오자
	XMMATRIX& GetAllMatrix(UINT iIndex);

protected:
	void SaveSkeleton(std::ofstream& file, CBone* pNode);
	CBone* LoadSkeleton(std::ifstream& file, CBone* pNode);

	void UpdateTransforms(CBone* pNode);
	void CalculateBoneToWorldTransform(CBone* pInternalNode); // Calculates the global transformation matrix for the given internal node

	void Calculate(float fPtime);
	void CalcBoneMatrix(void);

	void ExtractAnimations(const aiScene* pScene);
	CBone* CreateBoneTree(aiNode* pNode, CBone* pParent); // Recursively creates an internal node structure matching the current scene and animation

public:
	const XMFLOAT4X4* FindFrame(std::string strName);//부위 찾기
	//m_mapBonesByName//여기서 부위 찾은 뒤 확인
public:
	std::vector<CAnimEvaluator> m_vecAnimations; // a std::vector that holds each animation
	int32_t				m_iCurAnimIndex;  // Current animation index
	
public: // 애니메이션 씬 전환시에 선형보간을 위한 변수
	bool				m_bChangeScene;
	int32_t				m_iPreAnimIndex;	// 이전 씬 인덱스
	UINT				m_iPreFrame;
	UINT				m_iCurFrame;
	vector<XMFLOAT4X4>	m_vecPreTransforms;	// 이전 씬의 마지막 행렬
	float				m_fWeight;			// 보간시 설정할 가중치값
	float				m_fLastAccumWeight;		// 마지막 가중치값
	float				m_fTotalAccumWeight;		// 총 누적 가중치값


protected:
	CBone* m_pSkeleton; // Root Node of the internal scene structure
	std::map<std::string, CBone*> m_mapBonesByName; // Name to node map to quickly find nodes by their name
	std::map<std::string, UINT> m_mapBonesToIndex; // name to node map to quickly finde nodes by their name
	std::map<std::string, uint32_t> m_mapAnimToID; // finde animation quickly
	std::vector<CBone*> m_vecBones; // DO NOT DELETE THESE when the destructor runs... THEY ARE JUST COPIES!!
	std::vector<XMFLOAT4X4> m_vecTransform; // temp array of transforms


	map<string, UINT> m_mapAllBonesIndex;
};

END