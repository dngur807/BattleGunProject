#include "DynamicMesh.h"

#include "SceneAnimator.h"
#include "SkinnedModel.h"
#include "SkinnedMesh.h"
#include "Primitive.h"
#include "PrimitiveFactory.h"
#include "ResourceMgr.h"
#include "TimeMgr.h"
#include "Effects.h"

Engine::CDynamicMesh::CDynamicMesh(MYGDI * pMyGDI)
	:CMesh(pMyGDI)
	, m_pSkinnedModel(nullptr)
	,  m_pAnimator(nullptr)
	,  m_iCurAnimIndex(0)
	,  m_fElapsedTime(0.f)
	, m_fAnimationTime(1.0f)
{
}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh & rhs)
	:CMesh(rhs)
	, m_pSkinnedModel(rhs.m_pSkinnedModel)
	, m_pAnimator(rhs.m_pAnimator)
	, m_iCurAnimIndex(rhs.m_iCurAnimIndex)
	, m_fElapsedTime(rhs.m_fElapsedTime)
	, m_mapSkinnedModel(rhs.m_mapSkinnedModel)
	, m_strFileName(rhs.m_strFileName)
	, m_fAnimationTime(rhs.m_fAnimationTime)
{
}

Engine::CDynamicMesh::~CDynamicMesh()
{
//	Release();
}

HRESULT Engine::CDynamicMesh::LoadMeshFromFile(const TCHAR * pPath, const TCHAR* pName)
{
	TCHAR szFullPath[_MAX_PATH] = L"";
	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pName);

	char cFilePath[_MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, 0, pPath, _MAX_PATH, cFilePath, _MAX_PATH,
		nullptr, nullptr);

	aiString aiFilePath;
	aiFilePath.Set(cFilePath);

	char cPath[_MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, 0, szFullPath, _MAX_PATH, cPath, _MAX_PATH,
		nullptr, nullptr);

	//중요 ! 두 면의 법선벡터 사이 각이 80도를 넘으면 스무스하지않다.
	//Since tha angle between a cubes face normals is 90 the lighting looks
	// very bad if we don't specify this.
	m_aiImporter.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.f);
	m_aiImporter.SetPropertyInteger(AI_CONFIG_IMPORT_TER_MAKE_UVS, 1);
	m_aiImporter.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE);

	// Load Model From File
	m_pScene = m_aiImporter.ReadFile(cPath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_SplitLargeMeshes |
		aiProcess_ConvertToLeftHanded |
		aiProcess_SortByPType);

	if (m_pScene)
	{
		// Create the Model that is getting filled out.
		m_pSkinnedModel = new CSkinnedModel(m_pMyGDI);

		//Create the Animator
		m_pAnimator = new CSceneAnimator;
		m_pAnimator->Init(m_pScene, m_pSkinnedModel);
		m_pSkinnedModel->SetAnimator(m_pAnimator);
		//Loop through all meshes.
		for (int j = 0; j < m_pScene->mNumMeshes; ++j)
		{
			aiMesh* assimpMesh = m_pScene->mMeshes[j];

			//Calculate vertex weight and bone indices.
			vector<Weights> vecWeights = CalculateWeights(assimpMesh, m_pAnimator);

			vector<SkinnedVertex> vertices;
			vector<UINT> indices;

			// Add vertices to the vertex list
			for (int i = 0; i < assimpMesh->mNumVertices; ++i)
			{
				aiVector3D v = assimpMesh->mVertices[i];
				aiVector3D n = assimpMesh->mNormals[i];
				aiVector3D t = aiVector3D(0.f, 0.f, 0.f);
				if (assimpMesh->HasTextureCoords(0))
					t = assimpMesh->mTextureCoords[0][i];

				n = n.Normalize();

				// Pos, normal and texture coordinates.
				SkinnedVertex vertex(v.x, v.y, v.z, n.x, n.y, n.z, 0, 0, 1, t.x, t.y);

				// Bone indices and weights.
				for (UINT k = 0; k < vecWeights[i].boneIndices.size(); ++k)
				{
					if (k < 4)
						vertex.BoneIndicesA[k] = vecWeights[i].boneIndices[k];
					else
						vertex.BoneIndicesB[k - 4] = vecWeights[i].boneIndices[k];
				}

				if (vecWeights[i].weights.size() > 8)
					int iA = 0;

				vertex.WeightsA.x = vecWeights[i].weights.size() >= 1 ? vecWeights[i].weights[0] : 0;
				vertex.WeightsA.y = vecWeights[i].weights.size() >= 2 ? vecWeights[i].weights[1] : 0;
				vertex.WeightsA.z = vecWeights[i].weights.size() >= 3 ? vecWeights[i].weights[2] : 0;
				vertex.WeightsA.w = vecWeights[i].weights.size() >= 4 ? vecWeights[i].weights[3] : 0;

				vertex.WeightsB.x = vecWeights[i].weights.size() >= 5 ? vecWeights[i].weights[4] : 0;
				vertex.WeightsB.y = vecWeights[i].weights.size() >= 6 ? vecWeights[i].weights[5] : 0;
				vertex.WeightsB.z = vecWeights[i].weights.size() >= 7 ? vecWeights[i].weights[6] : 0;
				vertex.WeightsB.w = vecWeights[i].weights.size() >= 8 ? vecWeights[i].weights[7] : 0;

				vertices.push_back(vertex);
			}

			// Add indices to the index list
			for (int i = 0; i < assimpMesh->mNumFaces; ++i)
			{
				for (int k = 0; k < assimpMesh->mFaces[i].mNumIndices; ++k)
					indices.push_back(assimpMesh->mFaces[i].mIndices[k]);
			}

			// Create the mesh and its primitive
			CSkinnedMesh* pMesh = new CSkinnedMesh(m_pMyGDI);

			// Extract all the ambient, diffuse and specular colors.
			aiMaterial* aiMtl = m_pScene->mMaterials[assimpMesh->mMaterialIndex];

			aiColor4D ambient, diffuse, specular;
			aiMtl->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			aiMtl->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			aiMtl->Get(AI_MATKEY_COLOR_SPECULAR, specular);

			pMesh->SetMaterial(Material(ambient, diffuse, specular));

			CPrimitive* pPrimitive = new CPrimitive(m_pMyGDI, vertices, indices);
			pMesh->SetPrimitive(pPrimitive);
			pMesh->SetVertices(vertices);
			pMesh->SetIndices(indices);
			
			// Get the path to the texture in the directory
			aiString aiTex, aiTexPath = aiFilePath;
			string pathsave = aiFilePath.C_Str();
			
			aiMtl->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), aiTex);
			string texName = aiTex.C_Str();
			string Name;
			Name = texName.substr(texName.find_last_of("\\") + 1);
			aiTexPath.Append(Name.c_str());



			//FindValidPath(&aiTexPath);

			// Any Texture?
			if (Name != ""&& _stricmp(aiTexPath.C_Str(), "") != 0)
				pMesh->LoadTexture(aiTexPath.C_Str());

			// Any normal map?
			aiString aistrnmap;
			//FindValidPath(&aistrnmap);
			if (_stricmp(aistrnmap.C_Str(), "") != 0)
				pMesh->SetNormalMap(CResourceMgr::GetInstance()->
					LoadMeshTexture(m_pMyGDI, aistrnmap.C_Str()));

			m_pSkinnedModel->SetFileName(cPath);

			// Add the mesh to the model
			m_pSkinnedModel->AddMesh(pMesh);
		}

		//Pre-Calculate the bounding box
		// m_pSkinnedModel->CalculateAABB();

		// Add the newly created mesh to the map and return it
		m_mapSkinnedModel[cPath] = m_pSkinnedModel;
		//return m_mapSkinnedModel[cPath];
		return S_OK;
	}

	return E_FAIL;
}

//! Calculates the bone weights for each vertex.
vector<Engine::Weights> Engine::CDynamicMesh::CalculateWeights(aiMesh* mesh, CSceneAnimator* animator)
{
	vector<Weights> weights(mesh->mNumVertices);

	// Loop through all bones.
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		// Loop through all the vertices the bone affects.
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			aiVertexWeight weight = mesh->mBones[i]->mWeights[j];

			// Get the bone index from the animator by it's name.
			int index = animator->GetBoneIndex(mesh->mBones[i]->mName.C_Str());
			weights[weight.mVertexId].boneIndices.push_back(index);
			weights[weight.mVertexId].weights.push_back(mesh->mBones[i]->mWeights[j].mWeight);
		}
	}

	return weights;
}

//! Hax function... not mine.
int Engine::CDynamicMesh::FindValidPath(aiString* p_szString)
{
	ai_assert(NULL != p_szString);
	aiString pcpy = *p_szString;
	if ('*' == p_szString->data[0]) {
		// '*' as first character indicates an embedded file
		return 5;
	}

	// first check whether we can directly load the file
	FILE* pFile = fopen(p_szString->data, "rb");
	if (pFile)fclose(pFile);
	else
	{
		// check whether we can use the directory of  the asset as relative base
		char szTemp[MAX_PATH * 2], tmp2[MAX_PATH * 2];
		strcpy(szTemp, m_strFileName.c_str());
		strcpy(tmp2, szTemp);

		char* szData = p_szString->data;
		if (*szData == '\\' || *szData == '/')++szData;

		char* szEnd = strrchr(szTemp, '\\');
		if (!szEnd)
		{
			szEnd = strrchr(szTemp, '/');
			if (!szEnd)szEnd = szTemp;
		}
		szEnd++;
		*szEnd = 0;
		strcat(szEnd, szData);


		pFile = fopen(szTemp, "rb");
		if (!pFile)
		{
			// convert the string to lower case
			for (unsigned int i = 0;; ++i)
			{
				if ('\0' == szTemp[i])break;
				szTemp[i] = (char)tolower(szTemp[i]);
			}

			if (TryLongerPath(szTemp, p_szString))return 1;
			*szEnd = 0;

			// search common sub directories
			strcat(szEnd, "tex\\");
			strcat(szEnd, szData);

			pFile = fopen(szTemp, "rb");
			if (!pFile)
			{
				if (TryLongerPath(szTemp, p_szString))return 1;

				*szEnd = 0;

				strcat(szEnd, "textures\\");
				strcat(szEnd, szData);

				pFile = fopen(szTemp, "rb");
				if (!pFile)
				{
					if (TryLongerPath(szTemp, p_szString))return 1;
				}

				// patch by mark sibly to look for textures files in the asset's base directory.
				const char *path = pcpy.data;
				const char *p = strrchr(path, '/');
				if (!p) p = strrchr(path, '\\');
				if (p) {
					char *q = strrchr(tmp2, '/');
					if (!q) q = strrchr(tmp2, '\\');
					if (q) {
						strcpy(q + 1, p + 1);
						if (pFile = fopen(tmp2, "r")) {
							fclose(pFile);
							strcpy(p_szString->data, tmp2);
							p_szString->length = strlen(tmp2);
							return 1;
						}
					}
				}
				return 0;
			}
		}
		fclose(pFile);

		// copy the result string back to the aiString
		const size_t iLen = strlen(szTemp);
		size_t iLen2 = iLen + 1;
		iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
		memcpy(p_szString->data, szTemp, iLen2);
		p_szString->length = iLen;

	}
	return 1;
}


//! Hax function... not mine.
bool Engine::CDynamicMesh::TryLongerPath(char* szTemp, aiString* p_szString)
{
	char szTempB[MAX_PATH];
	strcpy(szTempB, szTemp);

	// go to the beginning of the file name
	char* szFile = strrchr(szTempB, '\\');
	if (!szFile)szFile = strrchr(szTempB, '/');

	char* szFile2 = szTemp + (szFile - szTempB) + 1;
	szFile++;
	char* szExt = strrchr(szFile, '.');
	if (!szExt)return false;
	szExt++;
	*szFile = 0;

	strcat(szTempB, "*.*");
	const unsigned int iSize = (const unsigned int)(szExt - 1 - szFile);

	HANDLE          h;
	WIN32_FIND_DATAA info;

	// build a list of files
	h = FindFirstFileA(szTempB, &info);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(strcmp(info.cFileName, ".") == 0 || strcmp(info.cFileName, "..") == 0))
			{
				char* szExtFound = strrchr(info.cFileName, '.');
				if (szExtFound)
				{
					++szExtFound;
					if (0 == _stricmp(szExtFound, szExt))
					{
						const unsigned int iSizeFound = (const unsigned int)(
							szExtFound - 1 - info.cFileName);

						for (unsigned int i = 0; i < iSizeFound; ++i)
							info.cFileName[i] = (CHAR)tolower(info.cFileName[i]);

						if (0 == memcmp(info.cFileName, szFile2, min(iSizeFound, iSize)))
						{
							// we have it. Build the full path ...
							char* sz = strrchr(szTempB, '*');
							*(sz - 2) = 0x0;

							strcat(szTempB, info.cFileName);

							// copy the result string back to the aiString
							const size_t iLen = strlen(szTempB);
							size_t iLen2 = iLen + 1;
							iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
							memcpy(p_szString->data, szTempB, iLen2);
							p_szString->length = iLen;
							return true;
						}
					}
					// check whether the 8.3 DOS name is matching
					if (0 == _stricmp(info.cAlternateFileName, p_szString->data))
					{
						strcat(szTempB, info.cAlternateFileName);

						// copy the result string back to the aiString
						const size_t iLen = strlen(szTempB);
						size_t iLen2 = iLen + 1;
						iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
						memcpy(p_szString->data, szTempB, iLen2);
						p_szString->length = iLen;
						return true;
					}
				}
			}
		} while (FindNextFileA(h, &info));

		FindClose(h);
	}
	return false;
}

void Engine::CDynamicMesh::SetAnimation(int iIndex)
{
	//if (iIndex != m_iCurAnimIndex)
		m_pAnimator->ChangeScene();

	m_iCurAnimIndex = iIndex;
	m_pAnimator->SetAnimIndex(iIndex);
	m_fElapsedTime = 0.f;
}

UINT Engine::CDynamicMesh::GetTrackPos(void)
{
	return m_pAnimator->m_vecAnimations[m_iCurAnimIndex].GetFrameIndexAt(m_fElapsedTime);
}

UINT Engine::CDynamicMesh::GetNumFrames(void)
{
	return m_pAnimator->m_vecAnimations[m_iCurAnimIndex].m_vecTransform.size();
}

bool Engine::CDynamicMesh::GetChangeScene(void)
{
	return m_pAnimator->m_bChangeScene;
}

float * Engine::CDynamicMesh::GetAccumWeight(void)
{
	return &m_pAnimator->m_fTotalAccumWeight;
}

XMMATRIX & Engine::CDynamicMesh::GetAllMatrix(UINT iIndex)
{
	return m_pAnimator->GetAllMatrix(iIndex);
}

int Engine::CDynamicMesh::Update(void)
{
	m_fElapsedTime += CTimeMgr::GetInstance()->DeltaTime() * m_fAnimationTime;
	m_pSkinnedModel->Update(m_fElapsedTime);
	//m_pSkinnedModel->GetAnimationSpeed();
	return 0;
}

void Engine::CDynamicMesh::RenderMesh(const XMMATRIX & pWorldMatrix)
{
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(pWorldMatrix);
	m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);

	m_pSkinnedModel->SetAnimation(m_iCurAnimIndex);
	m_pSkinnedModel->SetElapsedTime(m_fElapsedTime);
	m_pSkinnedModel->Draw(pWorldMatrix);
	//pGraphics->DrawBoundingBox(&GetBoundingBox(), GetWorldMatrix(), Material(Colors::Green));
}

DWORD Engine::CDynamicMesh::Release(void)
{
	Safe_Delete(m_pAnimator);
	Safe_Delete(m_pSkinnedModel);

	return 0;
}

Engine::CResources * Engine::CDynamicMesh::CloneResource(void)
{
	return new CDynamicMesh(*this);
}

Engine::CDynamicMesh * Engine::CDynamicMesh::Create(MYGDI * pMyGDI, 
	const TCHAR * pPath, const TCHAR* pName)
{
	CDynamicMesh* pMesh = new CDynamicMesh(pMyGDI);
	if (FAILED(pMesh->LoadMeshFromFile(pPath, pName)))
	{
		Safe_Delete(pMesh);
	}
	else
		pMesh->Update();

	return pMesh;
}
const XMFLOAT4X4* Engine::CDynamicMesh::FindFrame(const std::string strFrameName)
{
	return m_pAnimator->FindFrame(strFrameName);
}

void Engine::CDynamicMesh::SetTrackPos(float fPos)
{
	m_pAnimator->m_vecAnimations[m_iCurAnimIndex].m_fTicksPerSec = fPos;
}

float Engine::CDynamicMesh::GetDuration()
{
	return m_pAnimator->m_vecAnimations[m_iCurAnimIndex].m_fDuration;
}

float Engine::CDynamicMesh::GetTickPerSec()
{
	return m_pAnimator->m_vecAnimations[m_iCurAnimIndex].m_fTicksPerSec;
}

float Engine::CDynamicMesh::GetLastTime()
{
	return m_pAnimator->m_vecAnimations[m_iCurAnimIndex].m_fLastTime;
}

