#pragma once

#include "Export_Function.h"
#include "Engine_Struct.h"
#include "DynamicCamera.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CResourceMgr;
}

class CDynamicCamera;
class CBackGround
{
public:
	void Update();
	void Render();

private:
	HRESULT AddComponent(void);
	HRESULT InitScene(void);
	explicit CBackGround(Engine::MYGDI* pMyGDi);
	Engine::MYGDI*		m_pMyGDI;
	Engine::CStaticMesh*	m_pStaticMesh = nullptr;
private:
	CDynamicCamera*			m_pDynamicCamera;
	Engine::CTransform*		m_pInfo;
	DWORD					m_dwMeshNum;
	Engine::CResourceMgr*	m_pResourceMgr;
	vector<Engine::CStaticMesh*>	m_vecMesh;
public:
	void CBackGround::SetCubeAngle(float _x, float _y, float _z);
	void SetAngleIni();
	void CameraIni();
	void SetMeshNum(DWORD num) { m_dwMeshNum = num; }
	DWORD GetMeshNum() { return m_dwMeshNum; }
	vector<Engine::CStaticMesh*>* GetMeshVector() { return &m_vecMesh; }
	void LButtonDown();
	void RButtonDown();
	void LoadMap();

	// 편집 상태 확인용
private:
	bool					m_bTileCreate;
	bool					m_bTileModify;
	bool					m_bNaviCreate;
	bool					m_bNaviModify;
	bool					m_bTerrainModify;

	XMFLOAT3				m_vPos;
	XMFLOAT3				m_vSize;
	float					m_fAngle[Engine::ANGLE_END];
	Engine::CGameObject*	m_pTerrain = nullptr; 
	Engine::CGameObject*	m_pStone = nullptr;
	ID3D11RasterizerState*      m_pWire;
	float MeshAngleX;
	float MeshAngleY;
	float MeshAngleZ;
	float MeshX;
	float MeshY;
	float MeshZ;
	float MeshSizeX;
	float MeshSizeY;
	float MeshSizeZ;
private:
	void MeshAdd();
	void MeshDelete();
	void MeshModify();
	HRESULT AddBuilding();
	void ADDVtx();
	void DeleteVtx();
	void Reset();
public:
	virtual ~CBackGround(void);

public:
	static CBackGround* Create(Engine::MYGDI* pMyGDi);

	//네비메쉬
	XMFLOAT3				m_vecNavMesh[3];
	vector<XMFLOAT3>     m_vecSaveMesh; // 저장용
	vector<int>				m_vecSaveOption; // 저장용
	void CheckClockWise(XMFLOAT3 vecNavMesh[3]); // 시계 방향인지 체크하고 바꾸어줌
	HRESULT LoadNaviMesh();
	void NaviModify();
	int						m_iCount;
	int						m_iOption;
	int						m_iSamePoint;
	float GetStoneY() { return stoney; }
	float stoney;
	void SaveNavMesh();
	void LoadNavMesh();




};

