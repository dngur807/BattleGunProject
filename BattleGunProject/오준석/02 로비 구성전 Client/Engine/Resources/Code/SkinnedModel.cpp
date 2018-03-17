#include "SkinnedModel.h"

#include "SkinnedMesh.h"
#include "SceneAnimator.h"
#include "Effects.h"

Engine::CSkinnedModel::CSkinnedModel(MYGDI* pMyGDI)
	:m_pMyGDI(pMyGDI)
{
	SetFileName("#NOVALUE");
	m_pAnimator = nullptr;
	m_fElapsedTime = 0.f;

	//??
	/*AddAnimation(0, 37, 662, 20);
	AddAnimation(38, 83, 662, 20);
	AddAnimation(84, 264, 662, 20);
	AddAnimation(265, 445, 662, 20);
	AddAnimation(446, 491, 662, 20);
	AddAnimation(492, 537, 662, 20);
	AddAnimation(538, 553, 662, 20);
	AddAnimation(554, 662, 662, 20);*/
}


Engine::CSkinnedModel::~CSkinnedModel()
{
	for (size_t i = 0; i < m_vecMesh.size(); ++i)
		Safe_Delete(m_vecMesh[i]);

	/*m_pAnimator->Release();
	Safe_Delete(m_pAnimator);*/
}

void Engine::CSkinnedModel::Update(float fdt)
{
	m_fElapsedTime += fdt;

	// Update the elapsed time based on the current animation index.
	/*int currentAnimation = m_pAnimator->GetAnimationIndex();
	float animationLength = m_vecAnitime[currentAnimation].fEndTime - 
		m_vecAnitime[currentAnimation].fStartTime;
	m_fElapsedTime += m_vecAnitime[currentAnimation].fStartTime;

	if(m_fElapsedTime > animationLength || m_fElapsedTime < 0.f)
		m_fElapsedTime = 0;*/
}

// Draw all the skinned meshes. Sets the bone transforms to use in the shader.
void Engine::CSkinnedModel::Draw(const XMMATRIX & matWorld)
{
	// 현재 프레임을 저장
	m_pAnimator->m_iCurFrame = m_pAnimator->
		m_vecAnimations[m_pAnimator->m_iCurAnimIndex].GetFrameIndexAt(m_fElapsedTime);

	m_pMyGDI->pEffect->SetWorld(matWorld);
	m_pMyGDI->pEffect->SetWorldInvTranspose(XMMatrixTranspose(matWorld));
	m_pMyGDI->pEffect->SetUseAnimation(true);

	//Bone Transforms
	//! 현재 씬-프레임의 본 행렬정보를 담는 벡터를 불러온다.
	vector<XMFLOAT4X4> matFinalTransforms = m_pAnimator->GetTransform(m_fElapsedTime);
	m_pMyGDI->pEffect->SetBoneTransforms(&matFinalTransforms[0], matFinalTransforms.size());
	//m_pMyGDI->pEffect->SetLights(pGraphics->GetLightList());

	// Scene Change 가 true이면 보간 수행을 위한 인자를 Shader에 전달
	if (m_pAnimator->m_bChangeScene)
	{
		m_pMyGDI->pEffect->SetPreBoneTransforms(&m_pAnimator->m_vecPreTransforms[0], 
			m_pAnimator->m_vecPreTransforms.size());
		m_pMyGDI->pEffect->SetChangeScene(true);
		m_pAnimator->CalcAccumulateWeight();
		m_pMyGDI->pEffect->SetWeightFactor(m_pAnimator->m_fTotalAccumWeight);

		// 누적 가중치가 0.95 이상이면 보간 종료
		if (m_pAnimator->m_fTotalAccumWeight > 0.98f)
			m_pAnimator->InitInterpolation();
	}

	// Loop through and draw each mesh
	for (int i = 0; i < m_vecMesh.size(); ++i)
		m_vecMesh[i]->Draw();

	m_pMyGDI->pEffect->SetChangeScene(false);
}

//! Saves the model to a file.[TODO]
void Engine::CSkinnedModel::Save(string strFileName)
{
	ofstream fout(strFileName, ios::binary);

	fout << "#Meshes " << m_vecMesh.size() << "\r\n";

	// Save the meshes.
	for (int i = 0; i < m_vecMesh.size(); i++)
		m_vecMesh[i]->Save(fout);

	// Save the animations.
	m_pAnimator->Save(fout);

	fout.close();
}

//! Loads the  model from a file.[TODO]
void Engine::CSkinnedModel::Load(string strFileName)
{
	ifstream fin(strFileName, ios::binary | ios::in);

	string ignore;
	int numMeshes;
	fin >> ignore >> numMeshes;

	for (int i = 0; i < numMeshes; i++)
	{
		CSkinnedMesh* mesh = new CSkinnedMesh(m_pMyGDI);
		mesh->Load(fin);
		AddMesh(mesh);
	}

	// The space after the last index.
	fin >> ignore;

	// Make sure to start loading animation data from the right place.
	// [NOTE] After some investigation it seems like it should be at the first '\f' character.
	char x = fin.peek();
	while (x != '\f') {
		fin >> ignore;
		x = fin.peek();
	}

	m_pAnimator = new CSceneAnimator();
	m_pAnimator->Load(fin);

	fin.close();
}

void Engine::CSkinnedModel::AddMesh(CSkinnedMesh * pMesh)
{
	m_vecMesh.push_back(pMesh);
}

void Engine::CSkinnedModel::SetAnimator(CSceneAnimator * pAnimator)
{
	m_pAnimator = pAnimator;
}

void Engine::CSkinnedModel::SetAnimation(string strAniName)
{
	m_pAnimator->SetAnimation(strAniName);
}

void Engine::CSkinnedModel::SetFileName(string strFileName)
{
	m_strFileName = strFileName;
}

void Engine::CSkinnedModel::SetAnimation(int iIndex)
{
	m_pAnimator->SetAnimIndex(iIndex);
}

void Engine::CSkinnedModel::SetElapsedTime(float fElapsedTime)
{
	m_fElapsedTime = fElapsedTime;
}

void Engine::CSkinnedModel::SetMeshMaterial(int iMeshID, Material tMtrl)
{
	if (iMeshID < m_vecMesh.size() && iMeshID >= 0)
		m_vecMesh[iMeshID]->SetMaterial(tMtrl);
}

void Engine::CSkinnedModel::AdjustAnimationSpeedBy(float fPercent)
{
	m_pAnimator->AdjustAnimationSpeedBy(fPercent);
}

Engine::VECSKINNEDMESH * Engine::CSkinnedModel::GetMeshes(void)
{
	return &m_vecMesh;
}

vector<XMFLOAT4X4> Engine::CSkinnedModel::GetFinalTransforms(void)
{
	return m_pAnimator->GetTransform(m_fElapsedTime);
}

BoundingBox Engine::CSkinnedModel::GetBoundingBox(void)
{
	return m_AABB;
}

string Engine::CSkinnedModel::GetFileName(void)
{
	return m_strFileName;
}

void Engine::CSkinnedModel::CalculateAABB(void)
{
}

int Engine::CSkinnedModel::GetCurrentAnimation(void)
{
	return m_pAnimator->GetAnimationIndex();
}

float Engine::CSkinnedModel::GetAnimationSpeed(void)
{
	return m_pAnimator->GetAnimationSpeed();
}

void Engine::CSkinnedModel::AddAnimation(int iStartFrame, int iEndFrame, float fTotalFrames, float fLength)
{
	m_vecAnitime.push_back(ANITIME(iStartFrame, iEndFrame, fTotalFrames, fLength));
}

void Engine::CSkinnedModel::AddAnimation(float fStartTime, float fEndTime)
{
	m_vecAnitime.push_back(ANITIME(fStartTime, fEndTime));
}

void Engine::CSkinnedModel::Release(void)
{
	for (UINT i = 0; i < m_vecMesh.size(); ++i)
	{
		Safe_Delete(m_vecMesh[i]);
	}
	m_vecMesh.clear();
	m_vecMesh.swap(VECSKINNEDMESH());

}
