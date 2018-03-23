#include "SceneAnimator.h"
#include "SkinnedModel.h"

void TransformMatrix(XMFLOAT4X4& out, const aiMatrix4x4& in) {// there is some type of alignment issue with my mat4 and the aimatrix4x4 class, so the copy must be manually
	out._11 = in.a1;
	out._12 = in.a2;
	out._13 = in.a3;
	out._14 = in.a4;

	out._21 = in.b1;
	out._22 = in.b2;
	out._23 = in.b3;
	out._24 = in.b4;

	out._31 = in.c1;
	out._32 = in.c2;
	out._33 = in.c3;
	out._34 = in.c4;

	out._41 = in.d1;
	out._42 = in.d2;
	out._43 = in.d3;
	out._44 = in.d4;
}
//---------------------------------------------------------------------------------
//## constructor on a given animation
Engine::CAnimEvaluator::CAnimEvaluator(const aiAnimation* pAnim)
{
	m_fLastTime = 0.f;
	m_fTicksPerSec = static_cast<float>(
		pAnim->mTicksPerSecond != 0.0f ? pAnim->mTicksPerSecond : 100.f);
	m_fDuration = static_cast<float>(pAnim->mDuration);
	m_strName = pAnim->mName.data;
	m_vecChannel.resize(pAnim->mNumChannels);
	for (UINT i = 0; i < pAnim->mNumChannels; ++i)
	{
		m_vecChannel[i].strName = pAnim->mChannels[i]->mNodeName.data;
		for (UINT j = 0; j < pAnim->mChannels[i]->mNumPositionKeys; ++j) {
			m_vecChannel[i].vecPosKey.push_back(pAnim->mChannels[i]->mPositionKeys[j]);
		}
		for (UINT j = 0; j < pAnim->mChannels[i]->mNumRotationKeys; ++j) {
			m_vecChannel[i].vecRotKey.push_back(pAnim->mChannels[i]->mRotationKeys[j]);
		}
		for (UINT j = 0; j < pAnim->mChannels[i]->mNumScalingKeys; ++j) {
			m_vecChannel[i].vecScaleKey.push_back(pAnim->mChannels[i]->mScalingKeys[j]);
		}
	}
	m_vecLastPos.resize(pAnim->mNumChannels, std::make_tuple(0, 0, 0));
}

// Evaluate the animation tracks for a given time stamp
void Engine::CAnimEvaluator::Evaluate(float fPtime, std::map<std::string, CBone*>& rmapBone)
{
	fPtime *= m_fTicksPerSec;

	float time = 0.0f;
	if (m_fDuration > 0.0)
		time = fmod(fPtime, m_fDuration);

	// calculate the transformations for each animation channel
	for (unsigned int a = 0; a < m_vecChannel.size(); a++) {
		const ANIMCHANNEL* channel = &m_vecChannel[a];
		std::map<std::string, CBone*>::iterator bonenode = rmapBone.find(channel->strName);

		if (bonenode == rmapBone.end()) {
			//OUTPUT_DEBUG_MSG("did not find the bone node "<<channel->Name);
			continue;
		}

		// ******** Position *****
		aiVector3D presentPosition(0, 0, 0);
		if (channel->vecPosKey.size() > 0) {
			// Look for present frame number. Search from last position if time is after the last time, else from beginning
			// Should be much quicker than always looking from start for the average use case.
			unsigned int frame = (time >= m_fLastTime) ? std::get<0>(m_vecLastPos[a]) : 0;
			while (frame < channel->vecPosKey.size() - 1) {
				if (time < channel->vecPosKey[frame + 1].mTime) {
					break;
				}
				frame++;
			}

			// interpolate between this frame's value and next frame's value
			unsigned int nextFrame = (frame + 1) % channel->vecPosKey.size();

			const aiVectorKey& key = channel->vecPosKey[frame];
			const aiVectorKey& nextKey = channel->vecPosKey[nextFrame];
			double diffTime = nextKey.mTime - key.mTime;
			if (diffTime < 0.0)
				diffTime += m_fDuration;
			if (diffTime > 0) {
				float factor = float((time - key.mTime) / diffTime);
				presentPosition = key.mValue + (nextKey.mValue - key.mValue) * factor;
			}
			else {
				presentPosition = key.mValue;
			}
			std::get<0>(m_vecLastPos[a]) = frame;

			if (presentPosition.SquareLength() > 0.f)
				int iA = 0;
		}
		// ******** Rotation *********
		aiQuaternion presentRotation(1, 0, 0, 0);
		if (channel->vecRotKey.size() > 0)
		{
			unsigned int frame = (time >= m_fLastTime) ? std::get<1>(m_vecLastPos[a]) : 0;
			while (frame < channel->vecRotKey.size() - 1) {
				if (time < channel->vecRotKey[frame + 1].mTime)
					break;
				frame++;
			}

			// interpolate between this frame's value and next frame's value
			unsigned int nextFrame = (frame + 1) % channel->vecRotKey.size();

			const aiQuatKey& key = channel->vecRotKey[frame];
			const aiQuatKey& nextKey = channel->vecRotKey[nextFrame];
			double diffTime = nextKey.mTime - key.mTime;
			if (diffTime < 0.0) diffTime += m_fDuration;
			if (diffTime > 0) {
				float factor = float((time - key.mTime) / diffTime);
				aiQuaternion::Interpolate(presentRotation, key.mValue, nextKey.mValue, factor);
			}
			else presentRotation = key.mValue;
			std::get<1>(m_vecLastPos[a]) = frame;
		}

		// ******** Scaling **********
		aiVector3D presentScaling(1, 1, 1);
		if (channel->vecScaleKey.size() > 0) {
			unsigned int frame = (time >= m_fLastTime) ? std::get<2>(m_vecLastPos[a]) : 0;
			while (frame < channel->vecScaleKey.size() - 1) {
				if (time < channel->vecScaleKey[frame + 1].mTime)
					break;
				frame++;
			}

			presentScaling = channel->vecScaleKey[frame].mValue;
			std::get<2>(m_vecLastPos[a]) = frame;
		}

		aiMatrix4x4 mat = aiMatrix4x4(presentRotation.GetMatrix());

		XMFLOAT4X4 matScale, matRot, matTrans;

		XMMATRIX S = XMMatrixScaling(presentScaling.x, presentScaling.y, presentScaling.z);
		TransformMatrix(matRot, mat);
		//XMMatrixTranspose(matRot);
		XMMATRIX R = XMMatrixTranspose(XMLoadFloat4x4(&matRot));
		XMMATRIX T = XMMatrixTranslation(presentPosition.x, presentPosition.y, presentPosition.z);

		XMMATRIX SRT = S * R * T;
		XMMATRIX SRTT = XMMatrixTranspose(SRT);

		XMStoreFloat4x4(&bonenode->second->m_matLocal, SRT);


		/*mat.a1 *= presentScaling.x; mat.a2 *= presentScaling.x; mat.a3 *= presentScaling.x;
		mat.b1 *= presentScaling.y; mat.b2 *= presentScaling.y; mat.b3 *= presentScaling.y;
		mat.c1 *= presentScaling.z; mat.c2 *= presentScaling.z; mat.c3 *= presentScaling.z;
		mat.d1 = presentPosition.x; mat.d2 = presentPosition.y; mat.d3 = presentPosition.z;*/
		////mat.Transpose();

		//TransformMatrix(bonenode->second->m_matLocal, mat);
	}
	m_fLastTime = time;
}

void Engine::CAnimEvaluator::Save(std::ofstream & file)
{
	uint32_t nsize = static_cast<uint32_t>(m_strName.size());
	file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the size of the animation name
	file.write(m_strName.c_str(), nsize);// the size of the animation name
	file.write(reinterpret_cast<char*>(&m_fDuration), sizeof(m_fDuration));// the duration
	file.write(reinterpret_cast<char*>(&m_fTicksPerSec), sizeof(m_fTicksPerSec));// the number of ticks per second
	nsize = static_cast<uint32_t>(m_vecChannel.size());// number of animation channels,
	file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number animation channels
	for (size_t j(0); j< m_vecChannel.size(); j++) {// for each channel
		nsize = static_cast<uint32_t>(m_vecChannel[j].strName.size());
		file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the size of the name
		file.write(m_vecChannel[j].strName.c_str(), nsize);// the size of the animation name

		nsize = static_cast<uint32_t>(m_vecChannel[j].vecPosKey.size());
		file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of position keys
		for (size_t i(0); i< nsize; i++) {// for each channel
			file.write(reinterpret_cast<char*>(&m_vecChannel[j].vecPosKey[i].mTime), sizeof(m_vecChannel[j].vecPosKey[i].mTime));// pos key
			file.write(reinterpret_cast<char*>(&m_vecChannel[j].vecPosKey[i].mValue), sizeof(m_vecChannel[j].vecPosKey[i].mValue));// pos key
		}

		nsize = static_cast<uint32_t>(m_vecChannel[j].vecRotKey.size());
		file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of position keys
		for (size_t i(0); i< nsize; i++) {// for each channel
			file.write(reinterpret_cast<char*>(&m_vecChannel[j].vecRotKey[i].mTime), sizeof(m_vecChannel[j].vecRotKey[i].mTime));// rot key
			file.write(reinterpret_cast<char*>(&m_vecChannel[j].vecRotKey[i].mValue), sizeof(m_vecChannel[j].vecRotKey[i].mValue));// rot key
		}

		nsize = static_cast<uint32_t>(m_vecChannel[j].vecScaleKey.size());
		file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of position keys
		for (size_t i(0); i< nsize; i++) {// for each channel
			file.write(reinterpret_cast<char*>(&m_vecChannel[j].vecScaleKey[i].mTime), sizeof(m_vecChannel[j].vecScaleKey[i].mTime));// rot key
			file.write(reinterpret_cast<char*>(&m_vecChannel[j].vecScaleKey[i].mValue), sizeof(m_vecChannel[j].vecScaleKey[i].mValue));// rot key
		}

	}
}

void Engine::CAnimEvaluator::Load(std::ifstream & file)
{
	uint32_t nsize = 0;
	file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the size of the animation name
	char temp[250];
	file.read(temp, nsize);// the size of the animation name
	temp[nsize] = 0;// null char
	m_strName = temp;
	//OUTPUT_DEBUG_MSG("Creating Animation named: "<<Name);
	file.read(reinterpret_cast<char*>(&m_fDuration), sizeof(m_fDuration));// the duration
	file.read(reinterpret_cast<char*>(&m_fTicksPerSec), sizeof(m_fTicksPerSec));// the number of ticks per second
	file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number animation channels
	m_vecChannel.resize(nsize);
	for (size_t j(0); j< m_vecChannel.size(); j++) {// for each channel
		nsize = 0;
		file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the size of the name
		file.read(temp, nsize);// the size of the animation name
		temp[nsize] = 0;// null char
		m_vecChannel[j].strName = temp;

		nsize = static_cast<uint32_t>(m_vecChannel[j].vecPosKey.size());
		file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of position keys
		m_vecChannel[j].vecPosKey.resize(nsize);
		for (size_t i(0); i< nsize; i++) {// for each channel
			file.read(reinterpret_cast<char*>(&m_vecChannel[j].vecPosKey[i].mTime), sizeof(m_vecChannel[j].vecPosKey[i].mTime));// pos key
			file.read(reinterpret_cast<char*>(&m_vecChannel[j].vecPosKey[i].mValue), sizeof(m_vecChannel[j].vecPosKey[i].mValue));// pos key
		}

		nsize = static_cast<uint32_t>(m_vecChannel[j].vecRotKey.size());
		file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of position keys
		m_vecChannel[j].vecRotKey.resize(nsize);
		for (size_t i(0); i< nsize; i++) {// for each channel
			file.read(reinterpret_cast<char*>(&m_vecChannel[j].vecRotKey[i].mTime), sizeof(m_vecChannel[j].vecRotKey[i].mTime));// pos key
			file.read(reinterpret_cast<char*>(&m_vecChannel[j].vecRotKey[i].mValue), sizeof(m_vecChannel[j].vecRotKey[i].mValue));// pos key
		}

		nsize = static_cast<uint32_t>(m_vecChannel[j].vecScaleKey.size());
		file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of position keys
		m_vecChannel[j].vecScaleKey.resize(nsize);
		for (size_t i(0); i< nsize; i++) {// for each channel
			file.read(reinterpret_cast<char*>(&m_vecChannel[j].vecScaleKey[i].mTime), sizeof(m_vecChannel[j].vecScaleKey[i].mTime));// pos key
			file.read(reinterpret_cast<char*>(&m_vecChannel[j].vecScaleKey[i].mValue), sizeof(m_vecChannel[j].vecScaleKey[i].mValue));// pos key
		}
	}
	m_vecLastPos.resize(m_vecChannel.size(), std::make_tuple(0, 0, 0));
}

UINT Engine::CAnimEvaluator::GetFrameIndexAt(float fPtime)
{
	fPtime *= m_fTicksPerSec;

	float fTime = 0.f;
	if(m_fDuration > 0.f)
		fTime = fmod(fPtime, m_fDuration); // fPTime - (fPTime / m_fDuration)

	float fPercent = fTime / m_fDuration;
	if (!m_bPlayForward)
		fPercent = (fPercent - 1.f) * -1.f; // this will invert the percent so the animation plays backwards

	return static_cast<UINT> (static_cast<float>(m_vecTransform.size()) * fPercent);
}



//---------------------------------------------------------------------------------
Engine::CSceneAnimator::CSceneAnimator()
	:m_pSkeleton(nullptr), m_iCurAnimIndex(-1), 
	m_iPreAnimIndex(0), m_bChangeScene(false), 
	m_iPreFrame(0), m_iCurFrame(0),
	m_fWeight(0.04f), m_fLastAccumWeight(0.f), m_fTotalAccumWeight(0.f)
{
}


Engine::CSceneAnimator::~CSceneAnimator()
{
	//Release();
}

// this will build the skeleton based on the scene passed to it and CLEAR EVERYTHING
void Engine::CSceneAnimator::Init(const aiScene * pScene, CSkinnedModel* pSkinnedModel)
{
	if (!pScene->HasAnimations())
		return;
	Release();

	m_pSkeleton = CreateBoneTree(pScene->mRootNode, NULL);
	ExtractAnimations(pScene);

	for (UINT i = 0; i < pScene->mNumMeshes; ++i)
	{
		const aiMesh* pMesh = pScene->mMeshes[i];

		for (UINT n = 0; n < pMesh->mNumBones; ++n)
		{
			const aiBone* pBone = pMesh->mBones[n];
			std::map<std::string, CBone*>::iterator founditer = m_mapBonesByName.find(pBone->mName.data);
			// FOUND IT!! woohoo, make sure its not already in the bone list
			if (founditer != m_mapBonesByName.end())
			{
				bool bSkip = false;
				for (size_t j(0); j < m_vecBones.size(); ++j)
				{
					std::string strBone = pBone->mName.data;
					if (m_vecBones[j]->m_strName == strBone)
					{
						bSkip = true; // already inerted, skip this so as not to insert the same bone multiple times
						break;
					}
				}
				if (!bSkip) // obly insert the bone if is has not already been inserted
				{
					std::string tes = founditer->second->m_strName;
					TransformMatrix(founditer->second->m_matOffset, pBone->mOffsetMatrix);
					XMMATRIX mat = XMLoadFloat4x4(&founditer->second->m_matOffset); // [CUSTOM]
					XMStoreFloat4x4(&founditer->second->m_matOffset, XMMatrixTranspose(mat));
					m_vecBones.push_back(founditer->second);
					m_mapBonesToIndex[founditer->first] = m_vecBones.size() - 1;
				}
			}
		}
	}

	m_vecTransform.resize(m_vecBones.size());
	float fStep = 30.f;
	float fTimeStep = 1.f / fStep; // 30 per second

	float fStartTime = 0.f;

	for (size_t i(0); i < m_vecAnimations.size(); ++i) // pre calculate the animations
	{
		SetAnimIndex(i);
		float fdt = 0;

		for (float ticks = 0; ticks < m_vecAnimations[i].m_fDuration; ticks += m_vecAnimations[i].m_fTicksPerSec / fStep)
		{
			fdt += fTimeStep;
			//fdt++;
			Calculate(fdt);
			m_vecAnimations[i].m_vecTransform.push_back(std::vector<XMFLOAT4X4>());
			std::vector<XMFLOAT4X4>& trans = m_vecAnimations[i].m_vecTransform.back();
			for (size_t a = 0; a < m_vecTransform.size(); ++a)
			{
				XMMATRIX matResult = XMLoadFloat4x4(&m_vecBones[a]->m_matOffset) * XMLoadFloat4x4(&m_vecBones[a]->m_matGlobal);
				XMFLOAT4X4 matRot;
				XMStoreFloat4x4(&matRot, matResult); // [CUSTOM]

				trans.push_back(matRot);
			}

			//-----------------------------------------------
			m_vecAnimations[i].m_vecAllTrans.push_back(std::vector<XMFLOAT4X4>());
			std::vector<XMFLOAT4X4>& alltrans = m_vecAnimations[i].m_vecAllTrans.back();

			map<std::string, CBone*>::iterator iter = m_mapBonesByName.begin(),
				iter_end = m_mapBonesByName.end();

			for (; iter != iter_end; ++iter)
			{
				XMMATRIX matResult = XMLoadFloat4x4(&(iter->second->m_matOffset)) * XMLoadFloat4x4(&(iter->second->m_matGlobal));
				XMFLOAT4X4 matRot;
				XMStoreFloat4x4(&matRot, matResult); // [CUSTOM]

				alltrans.push_back(matRot);
			}
			//-------------------------------------------------
		}
		pSkinnedModel->AddAnimation(fStartTime, fStartTime + fdt);
		fStartTime += fdt;
	}

	map<std::string, CBone*>::iterator iter = m_mapBonesByName.begin(),
		iter_end = m_mapBonesByName.end();
	UINT i = 0;
	for (; iter != iter_end; ++iter)
	{
		m_mapAllBonesIndex.insert(map<string, UINT>::value_type(iter->first, i++));
	}
	int iA = 0;
	//------------------------------------------------------------------------
}

void Engine::CSceneAnimator::Release(void)
{
	m_iCurAnimIndex = -1;
	m_vecAnimations.clear(); // clear all animations
	Safe_Delete(m_pSkeleton); // this node will delete all children recursively
}

void Engine::CSceneAnimator::Save(std::ofstream & file)
{
	// first recursivly save the skeleton
	if (m_pSkeleton)
		SaveSkeleton(file, m_pSkeleton);

	uint32_t nsize = static_cast<uint32_t>(m_vecAnimations.size());
	file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of animations	
	for (uint32_t i(0); i< nsize; i++) {
		m_vecAnimations[i].Save(file);
	}

	nsize = static_cast<uint32_t>(m_vecBones.size());
	file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of bones

	for (uint32_t i(0); i< m_vecBones.size(); i++) {
		nsize = static_cast<uint32_t>(m_vecBones[i]->m_strName.size());
		file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the size of the bone name
		file.write(m_vecBones[i]->m_strName.c_str(), nsize);// the name of the bone
	}
}

void Engine::CSceneAnimator::Load(std::ifstream & file)
{
	Release();// make sure to clear this before writing new data
	m_pSkeleton = LoadSkeleton(file, NULL);
	uint32_t nsize = 0;
	file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of animations
	m_vecAnimations.resize(nsize);
	//OUTPUT_DEBUG_MSG("Extracting Animations . . ");
	for (uint32_t i(0); i< nsize; i++) {
		m_vecAnimations[i].Load(file);
	}
	for (uint32_t i(0); i< m_vecAnimations.size(); i++) {// get all the animation names so I can reference them by name and get the correct id
		m_mapAnimToID.insert(std::map<std::string, uint32_t>::value_type(m_vecAnimations[i].m_strName, i));
	}
	if (m_vecAnimations.size() >0) m_iCurAnimIndex = 0;// set it to the first animation if there are any
	char bname[250];
	file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of bones
	m_vecBones.resize(nsize);

	for (uint32_t i(0); i< m_vecBones.size(); i++) {
		file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the size of the bone name
		file.read(bname, nsize);// the size of the bone name
		bname[nsize] = 0;
		std::map<std::string, CBone*>::iterator found = m_mapBonesByName.find(bname);
		m_mapBonesToIndex[found->first] = i;
		CBone* tep = found->second;
		m_vecBones[i] = tep;
	}

	m_vecTransform.resize(m_vecBones.size());
	float fStep = 30.f;
	float timestep = 1.0f / fStep;// 30 per second

	for (size_t i(0); i< m_vecAnimations.size(); i++) {// pre calculate the animations
		SetAnimIndex(i);
		float dt = 0;
		for (float ticks = 0; ticks < m_vecAnimations[i].m_fDuration; ticks += m_vecAnimations[i].m_fTicksPerSec / fStep) {
			dt += timestep;
			Calculate(dt);
			m_vecAnimations[i].m_vecTransform.push_back(std::vector<XMFLOAT4X4>());
			std::vector<XMFLOAT4X4>& trans = m_vecAnimations[i].m_vecTransform.back();
			for (size_t a = 0; a < m_vecTransform.size(); ++a) {
				XMMATRIX result = XMLoadFloat4x4(&m_vecBones[a]->m_matOffset) * XMLoadFloat4x4(&m_vecBones[a]->m_matGlobal);
				XMFLOAT4X4 rotationmat;
				XMStoreFloat4x4(&rotationmat, result); // [CUSTOM]

				trans.push_back(rotationmat);
			}
		}
	}
	//OUTPUT_DEBUG_MSG("Finished loading animations with "<<Bones.size()<<" bones");
}

bool Engine::CSceneAnimator::SetAnimIndex(int32_t iAnimIndex)
{
	if (iAnimIndex >= m_vecAnimations.size())
		return false; // no change, or the animation data is out of bounds
	int32_t iOldIndex = m_iCurAnimIndex;
	m_iCurAnimIndex = iAnimIndex; // only set this after the check for goood data and the object was actually inserted
	
	return iOldIndex != m_iCurAnimIndex;
}

bool Engine::CSceneAnimator::SetAnimation(const std::string & strName)
{
	std::map<std::string, uint32_t>::iterator iter = m_mapAnimToID.find(strName);
	int32_t iOldIndex = m_iCurAnimIndex;
	if (iter != m_mapAnimToID.end())
		m_iCurAnimIndex = iter->second;
	else
		MSG_BOX(L"Name Not Found");
	
	return iOldIndex != m_iCurAnimIndex;
}

int Engine::CSceneAnimator::GetBoneIndex(const std::string & strBoneName)
{
	std::map<std::string, unsigned int>::iterator found = m_mapBonesToIndex.find(strBoneName);
	if (found != m_mapBonesToIndex.end())
		return found->second; 
	else 
		return -1;
}

XMFLOAT4X4 Engine::CSceneAnimator::GetBoneTransform(float fdt, const std::string & strBoneName)
{
	int bindex = GetBoneIndex(strBoneName);
	if (bindex == -1) 
		return XMFLOAT4X4(); 
	return m_vecAnimations[m_iCurAnimIndex].GetTransform(fdt)[bindex];
}

XMFLOAT4X4 Engine::CSceneAnimator::GetBoneTransform(float fdt, UINT iBoneIndex)
{
	return m_vecAnimations[m_iCurAnimIndex].GetTransform(fdt)[iBoneIndex];
}

void Engine::CSceneAnimator::InitInterpolation(void)
{
	m_bChangeScene = false;
	m_iPreAnimIndex = m_iCurAnimIndex;
	m_iPreFrame = 0;
	m_fTotalAccumWeight = 0.f;
	m_fLastAccumWeight = 0.f;
}

void Engine::CSceneAnimator::CalcAccumulateWeight(void)
{
	//! Linear : 프레임이 바뀔때에만 더해주는 기능을 추가해야 함.(귀찮아서 안해봄)
	//m_fTotalAccumWeight += (1.f / m_iNumIpFrame);

	//! Logarithmic
	m_fLastAccumWeight = m_fWeight * (1.f - m_fTotalAccumWeight);
	m_fTotalAccumWeight += m_fLastAccumWeight;
}

void Engine::CSceneAnimator::ChangeScene(void)
{
	//! 보간 변수 초기화
	InitInterpolation();
	//! 현재 씬 정보를 저장
	m_iPreAnimIndex = m_iCurAnimIndex;
	m_iPreFrame = m_iCurFrame;
	m_vecPreTransforms = m_vecAnimations[m_iPreAnimIndex].m_vecTransform[m_iPreFrame];

	m_bChangeScene = true;
}

XMMATRIX & Engine::CSceneAnimator::GetAllMatrix(UINT iIndex)
{
	// TODO: insert return statement here
	return XMLoadFloat4x4(&(m_vecAnimations[m_iPreAnimIndex].GetAllTrans(m_iCurFrame)[iIndex]));
}

void Engine::CSceneAnimator::SaveSkeleton(std::ofstream & file, CBone * pParent)
{
	uint32_t nsize = static_cast<uint32_t>(pParent->m_strName.size());
	file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of chars
	file.write(pParent->m_strName.c_str(), nsize);// the name of the bone
	file.write(reinterpret_cast<char*>(&pParent->m_matOffset), sizeof(pParent->m_matOffset));// the bone offsets
	file.write(reinterpret_cast<char*>(&pParent->m_OriginalLocal), sizeof(pParent->m_OriginalLocal));// original bind pose
	nsize = static_cast<uint32_t>(pParent->m_vecChild.size());// number of children
	file.write(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of children
	for (std::vector<CBone*>::iterator it = pParent->m_vecChild.begin(); it != pParent->m_vecChild.end(); ++it)// continue for all children
		SaveSkeleton(file, *it);
}

Engine::CBone * Engine::CSceneAnimator::LoadSkeleton(std::ifstream & file, CBone * pParent)
{
	CBone* internalNode = new CBone();// create a node
	internalNode->m_pParent = pParent; //set the parent, in the case this is theroot node, it will be null
	uint32_t nsize = 0;
	file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of chars
	char temp[250];
	file.read(temp, nsize);// the name of the bone
	temp[nsize] = 0;
	internalNode->m_strName = temp;
	m_mapBonesByName[internalNode->m_strName] = internalNode;// use the name as a key
	file.read(reinterpret_cast<char*>(&internalNode->m_matOffset), sizeof(internalNode->m_matOffset));// the bone offsets
	file.read(reinterpret_cast<char*>(&internalNode->m_OriginalLocal), sizeof(internalNode->m_OriginalLocal));// original bind pose

	internalNode->m_matLocal = internalNode->m_OriginalLocal;// a copy saved
	CalculateBoneToWorldTransform(internalNode);

	file.read(reinterpret_cast<char*>(&nsize), sizeof(uint32_t));// the number of children

																 // continue for all child nodes and assign the created internal nodes as our children
	for (unsigned int a = 0; a < nsize && file; a++) {// recursivly call this function on all children
		internalNode->m_vecChild.push_back(LoadSkeleton(file, internalNode));
	}
	return internalNode;
}

// Recursively updates the internal node transformations from the given matrix array
void Engine::CSceneAnimator::UpdateTransforms(CBone * pNode)
{
	CalculateBoneToWorldTransform(pNode); // update global transform as well;

	for (std::vector<CBone*>::iterator iter = pNode->m_vecChild.begin();
		iter != pNode->m_vecChild.end(); ++iter)
		UpdateTransforms(*iter);
}

void Engine::CSceneAnimator::CalculateBoneToWorldTransform(CBone * pChild)
{
	pChild->m_matGlobal = pChild->m_matLocal;
	CBone* pParent = pChild->m_pParent;

	// this will climb the nodes up along through the parents concentating
	// all the matrices to get the Object to World transform
	// or in this case, the Bone to World transform
	while (pParent)
	{
		XMMATRIX result = XMLoadFloat4x4(&pChild->m_matGlobal) *
			XMLoadFloat4x4(&pParent->m_matLocal); // [CUSTOM]
		
		XMStoreFloat4x4(&pChild->m_matGlobal, result);

		// get the parent of the bone we are working on
		pParent = pParent->m_pParent;
	}
}

// Calculate the node transformations for the scene
void Engine::CSceneAnimator::Calculate(float fPtime)
{
	if ((m_iCurAnimIndex < 0) || (m_iCurAnimIndex >= m_vecAnimations.size()))
		return;

	m_vecAnimations[m_iCurAnimIndex].Evaluate(fPtime, m_mapBonesByName);
	UpdateTransforms(m_pSkeleton);
}

// Calculate the bone matrices for the given mesh.
void Engine::CSceneAnimator::CalcBoneMatrix(void)
{
	for (size_t a = 0; a < m_vecTransform.size(); ++a)
	{
		XMMATRIX matResult = XMLoadFloat4x4(&m_vecBones[a]->m_matOffset) * XMLoadFloat4x4(&m_vecBones[a]->m_matGlobal);
		XMStoreFloat4x4(&m_vecTransform[a], matResult); // [CUSTOM]
	}
}

void Engine::CSceneAnimator::ExtractAnimations(const aiScene * pScene)
{
	for (size_t i = 0; i < pScene->mNumAnimations; ++i)
	{
		m_vecAnimations.push_back(CAnimEvaluator(pScene->mAnimations[i])); // add the animations
	}
	
	// Get All the Animation names so I can reference them by name and get the correct ID
	for (uint32_t i(0); i < m_vecAnimations.size(); ++i)
	{
		m_mapAnimToID.insert(std::map<std::string, uint32_t>::value_type(m_vecAnimations[i].m_strName, i));
	}
	m_iCurAnimIndex = 0;
	//SetAnimation("Idle");
	SetAnimIndex(m_iCurAnimIndex);
}

// Recursively creates an internal node structure matching the current scene and animation
Engine::CBone * Engine::CSceneAnimator::CreateBoneTree(aiNode * pNode, CBone * pParent)
{
	CBone* pInternalNode = new CBone;	// Create a Node
	pInternalNode->m_strName = pNode->mName.data;	// get the name of the bone
	pInternalNode->m_pParent = pParent; // set the parent, in the case this is the root node, 

	m_mapBonesByName[pInternalNode->m_strName] = pInternalNode; // use the name as a key
	TransformMatrix(pInternalNode->m_matLocal, pNode->mTransformation);

	XMMATRIX mat = XMLoadFloat4x4(&pInternalNode->m_matLocal); // [CUSTOM]
	XMStoreFloat4x4(&pInternalNode->m_matLocal, XMMatrixTranspose(mat));

	pInternalNode->m_OriginalLocal = pInternalNode->m_matLocal; // a copy saved
	CalculateBoneToWorldTransform(pInternalNode);

	//continue for all child nodes and assign the created internal nodes as our children
	for (UINT a = 0; a < pNode->mNumChildren; ++a) // recursively call this func on all children
	{
		pInternalNode->m_vecChild.push_back(CreateBoneTree(pNode->mChildren[a], pInternalNode));
	}

	return pInternalNode;
}

const XMFLOAT4X4* Engine::CSceneAnimator::FindFrame(std::string strName)
{
	auto iter = m_mapBonesByName.find(strName);
	if (iter == m_mapBonesByName.end())
		return NULL;
	return &m_mapBonesByName[strName]->m_matGlobal;
}
