#include "StaticMesh.h"
#include "ModelObj.h"
#include "CubeColor.h"
#include "Effects.h"

Engine::CStaticMesh::CStaticMesh(MYGDI * pMyGDI)
	:CMesh(pMyGDI)
{
}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh & rhs)
	:CMesh(rhs)
{
	m_pBuffer = rhs.m_pBuffer;
	m_vecModel = rhs.m_vecModel;
}

Engine::CStaticMesh::~CStaticMesh()
{
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR * pPath, const TCHAR* pFileName)
{
	TCHAR szFullPath[_MAX_PATH] = L"";
	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	char cPath[_MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, 0, pPath, _MAX_PATH, cPath, _MAX_PATH,
		nullptr, nullptr);

	aiString aiPath;
	aiPath.Set(cPath);

	char cFullPath[_MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, 0, szFullPath, _MAX_PATH, cFullPath, _MAX_PATH,
		nullptr, nullptr);

	m_pScene = m_aiImporter.ReadFile(cFullPath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_SplitLargeMeshes |
		aiProcess_ConvertToLeftHanded |
		aiProcess_SortByPType 
	);

	BuildMesh(m_pScene->mRootNode, aiPath);

	BuildBoundingBox();

	return S_OK;
}

void Engine::CStaticMesh::BuildMesh(const aiNode * pNode, const aiString& aiPath)
{
	if (pNode->mNumMeshes > 0)
	{
		CModelObj* pModelObj = new CModelObj(m_pMyGDI);
		m_vecModel.push_back(pModelObj);
		pModelObj->BuildBuffer(m_pScene, pNode, aiPath);
	}

	for (UINT i = 0; i < pNode->mNumChildren; ++i)
		BuildMesh(pNode->mChildren[i], aiPath);
}

void Engine::CStaticMesh::RenderMesh(const XMMATRIX & pWorldMatrix)
{
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(pWorldMatrix);
	m_pMyGDI->pEffect->SetWorldInvTranspose(worldInvTranspose);

	for (UINT i = 0; i < m_vecModel.size(); ++i)
	{
		m_vecModel[i]->Draw(pWorldMatrix);
	}

	////! 바운딩 박스 랜더링
	//XMFLOAT4X4 matWorld;	XMStoreFloat4x4(&matWorld, pWorldMatrix);
	//m_pBuffer->Render(matWorld);
}

DWORD Engine::CStaticMesh::Release(void)
{
	for (UINT i = 0; i < m_vecModel.size(); ++i)
	{
		Safe_Delete (m_vecModel[i]);
		Safe_Delete(m_pBuffer);
	}
	m_vecModel.clear();
	m_vecModel.swap(VECMODEL());

	return 0;
}

void Engine::CStaticMesh::BuildBoundingBox(void)
{
	m_BoundingBox = m_vecModel[0]->MergeBoundingBox();

	for (UINT i = 1; i < m_vecModel.size(); ++i)
	{
		BoundingBox::CreateMerged(m_BoundingBox, m_BoundingBox, m_vecModel[i]->MergeBoundingBox());
	}

	XMVECTOR vCenter =  XMLoadFloat3(&m_BoundingBox.Center);
	XMVECTOR vExt = XMLoadFloat3(&m_BoundingBox.Extents);
	XMVECTOR MIN = vCenter - vExt;
	XMVECTOR MAX = vCenter + vExt;
	XMFLOAT4 vColor = static_cast<const XMFLOAT4>(DirectX::Colors::Green);

	m_pBuffer = CCubeColor::Create(m_pMyGDI, MIN, MAX, vColor);
}

Engine::CResources * Engine::CStaticMesh::CloneResource(void)
{
	return new CStaticMesh(*this);
}

Engine::CStaticMesh * Engine::CStaticMesh::Create(MYGDI * pMyGDI, 
	const TCHAR * pPath, const TCHAR* pFileName)
{
	CStaticMesh* pMesh = new CStaticMesh(pMyGDI);
	if (FAILED(pMesh->LoadMeshFromFile(pPath, pFileName)))
	{
		Safe_Delete(pMesh);
	}

	return pMesh;
}
