#include "ModelObj.h"
#include "ModelMesh.h"


Engine::CModelObj::CModelObj(MYGDI * pMyGDI)
	:m_pMyGDI(pMyGDI), m_iNumMeshes(0)
{
}

Engine::CModelObj::~CModelObj()
{
	Release();
	//Safe_Delete_Array(m_pMeshes);
}

void Engine::CModelObj::BuildBuffer(const aiScene * pScene, const aiNode* pNode, 
	const aiString& aiPath)
{
	//TODO : Create Mesh Class.
	m_iNumMeshes = pNode->mNumMeshes;

	m_vecMeshes.reserve(m_iNumMeshes);
	//m_pMesh¤²es = new CModelMesh[m_iNumMeshes];
	for (UINT i = 0; i < m_iNumMeshes; ++i)
	{
		aiMesh* aiMesh = pScene->mMeshes[pNode->mMeshes[i]];
		aiMaterial* aiMtl = pScene->mMaterials[aiMesh->mMaterialIndex];

		CModelMesh* pModelMesh = new CModelMesh(m_pMyGDI);
		m_vecMeshes.push_back(pModelMesh);

		// Extract all the ambient, diffuse and specular colors.
		aiColor4D ambient, diffuse, specular;
		aiMtl->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		aiMtl->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aiMtl->Get(AI_MATKEY_COLOR_SPECULAR, specular);

		pModelMesh->SetMaterial(Material(ambient, diffuse, specular));

		// Get the path to the texture in the directory
		aiString aiTex, aiTexPath = aiPath;
		aiMtl->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), aiTex);
		aiTexPath.Append(aiTex.C_Str());

		if (_stricmp(aiTexPath.C_Str(), "") != 0)
			pModelMesh->LoadTexture(aiTexPath.C_Str());

		pModelMesh->Initialize(aiMesh, aiMtl);
	}
}

BoundingBox& Engine::CModelObj::MergeBoundingBox(void)
{
	m_BoundingBox = m_vecMeshes[0]->CreateBoundingBox();

	for (UINT i = 1; i < m_vecMeshes.size(); ++i)
	{
		BoundingBox::CreateMerged(m_BoundingBox, m_BoundingBox, m_vecMeshes[i]->CreateBoundingBox());
	}

	return m_BoundingBox;
}

void Engine::CModelObj::Draw(const XMMATRIX& rmatWorld)
{
	for (UINT i = 0; i < m_vecMeshes.size(); ++i)
	{
		m_vecMeshes[i]->Draw(rmatWorld);
	}
}

void Engine::CModelObj::Release(void)
{
	for (UINT i = 0; i < m_vecMeshes.size(); ++i)
	{
		Safe_Delete(m_vecMeshes[i]);
	}
	m_vecMeshes.clear();
	VECMESH().swap(m_vecMeshes);
}
