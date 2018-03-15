#include "ModelMgr.h"
#include "ModelObj.h"

IMPLEMENT_SINGLETON(Engine::CModelMgr)

Engine::CModelMgr::CModelMgr()
{
	
}

Engine::CModelMgr::~CModelMgr()
{
	VECMODELOBJ::iterator	iter = m_vecModel.begin(), 
							iter_end = m_vecModel.end();

	for (; iter != iter_end; ++iter)
	{
		SafeDelete(*iter);
	}
}

void Engine::CModelMgr::ReadModel(const char * path)
{
	m_pModel = m_Importer.ReadFile(path, aiProcess_ConvertToLeftHanded);
	NULL_CHECK_MSG(m_pModel, L"Model Loading Failed");

	recursive_BuildObjects(m_pModel->mRootNode);
}

void Engine::CModelMgr::recursive_BuildObjects(const aiNode * pNode)
{
	/*if (pNode->mNumMeshes > 0)
	{
		CModelObj* pModelObj = new CModelObj(m_pMyGDI);
		m_vecModel.push_back(pModelObj);
		pModelObj->BuildBuffer(m_pModel, pNode);
	}

	for (UINT i = 0; i < pNode->mNumChildren; ++i)
	{
		recursive_BuildObjects(pNode->mChildren[i]);
	}*/
}

void Engine::CModelMgr::Draw(void)
{
}
