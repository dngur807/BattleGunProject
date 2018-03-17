#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CModelObj;

class ENGINE_DLL CModelMgr
{
	DECLARE_SINGLETON(CModelMgr)

private:
	CModelMgr(void);
public:
	~CModelMgr(void);

	void ReadModel(const char* path);
	void recursive_BuildObjects(const aiNode* pNode);
	void Draw(void);

private:
	const aiScene* m_pModel;
	Assimp::Importer m_Importer;
	typedef std::vector<CModelObj*>		VECMODELOBJ;
	VECMODELOBJ	m_vecModel;
};

END