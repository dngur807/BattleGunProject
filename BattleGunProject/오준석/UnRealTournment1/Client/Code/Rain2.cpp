#include "stdafx.h"
#include "Rain2.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Camera.h"
#include "Management.h"

CRain2::CRain2(Engine::MYGDI* pMyGDI)
	: Engine::CGameObject(pMyGDI)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pInfo(NULL)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_fSpeed(0.f)
{
	ZeroMemory(&m_tMtrl, sizeof(Material));
}

CRain2::~CRain2(void)
{

}

HRESULT CRain2::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	//m_pInfo->m_matWorld 

	m_pInfo->m_vPos = dynamic_cast<Engine::CCamera*>(m_pCamera)->GetEye();
	m_pInfo->m_vPos.x += rand() % 70 - 35;
	m_pInfo->m_vPos.z += rand() % 70 - 35;
	m_pInfo->m_vPos.y += rand() % 5 + 10;
	m_fSpeed = (rand() % 3 * 0.5f) + 5.f;

	//m_pInfo->m_vPos = XMFLOAT3(0.f, 2.f, 0.f);
	m_pInfo->m_vScale = XMFLOAT3(0.05f, 1.f, 1.3f);

	/*m_pInfo->m_matWorld
	XMMatrixRotationZ(XMConvertToRadians(270));*/
	//m_pInfo->m_fAngle[Engine::ANGLE_Z]= XMConvertToRadians(270);
	//m_pInfo->m_fAngle[Engine::ANGLE_Z] =XMConvertToRadians(270);


	//m_pInfo->m_fAngle[Engine::ANGLE_Y] += 180.f;
	//재질 설정
	m_tMtrl.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_tMtrl.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//빛초기화!
	m_DirLight.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.Direction = XMFLOAT3(1.f, -2.f, 1.f);

	D3D11_RASTERIZER_DESC dxRasterDesc;
	::memset(&dxRasterDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	//dxRasterDesc.FillMode = D3D11_FILL_WIREFRAME;       //와이어
	dxRasterDesc.FillMode = D3D11_FILL_SOLID;          //아닌거
	dxRasterDesc.CullMode = D3D11_CULL_NONE;
	dxRasterDesc.DepthClipEnable = TRUE;

	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);
	return S_OK;
}

void CRain2::Release(void)
{

}

ostream& operator<<(ostream& os, FXMVECTOR v)
{
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);
	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}

int CRain2::Update(void)
{
	//여러개생성해서 지우고 스카이박스하고
	Engine::CGameObject::Update();

	//빌보드== 플레이어를 처다보게
	//const Engine::CComponent* pPlayerInfo = Engine::Get_Management()->GetComponent(CStage::LAYER_GAMELOGIC, L"Player", L"Transform");;
	Engine::CTransform*	pRealPlayer = NULL;
	pRealPlayer = const_cast<Engine::CTransform*>(m_pTargetInfo);
	XMFLOAT3			 vDir, vDir2;
	vDir = XMFLOAT3(0.f, 0.f, 0.f);
	vDir2 = XMFLOAT3(0.f, 0.f, 0.f);

	float      fDistance = 1, fDistance2 = 0.f;
	XMVECTOR PlayerPos, m_InfoPos;
	PlayerPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_InfoPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);



	//PlayerPos=XMLoadFloat3(&pRealPlayer->m_vPos);
	PlayerPos = XMLoadFloat3(&dynamic_cast<Engine::CCamera*>(m_pCamera)->GetEye());
	m_InfoPos = XMLoadFloat3(&m_pInfo->m_vPos);


	//뺄샘
	XMVECTOR b = PlayerPos - m_InfoPos;
	XMStoreFloat3(&vDir2, b);

	//if (fDistance >D3DXVec3Length(&vDir2))
	//{
	vDir2.x = -vDir2.x;
	vDir2.y = -vDir2.y;
	vDir2.z = -vDir2.z;
	vDir = vDir2;
	//fDistance = XMVector3Length(&vDir2);
	//}

	XMVector3Normalize(XMLoadFloat3(&vDir));
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = atan2(vDir.x, vDir.z);


	if (m_pInfo->m_vPos.y < 0.f)
	{
		//m_pInfo->m_vPos.y = 5.f;
		m_pInfo->m_vPos = dynamic_cast<Engine::CCamera*>(m_pCamera)->GetEye();
		m_pInfo->m_vPos.x += rand() % 70 - 35;
		m_pInfo->m_vPos.z += rand() % 70 - 35;
		m_pInfo->m_vPos.y += rand() % 5 + 10;
	}
	m_pInfo->m_vPos.y -= m_fSpeed*Engine::Get_TimeMgr()->DeltaTime();
	return 0;
}

void CRain2::Render(void)
{
	//장치를 통한 9빌보드
	/*D3DXMATRIX      matBill;
	D3DXMatrixIdentity(&matBill);
	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);*/

	//빌보드 첫번째시도 실패
	/*XMFLOAT4X4      matBill;
	matBill=dynamic_cast<Engine::CCamera*>(m_pCamera)->GetView();
	XMMATRIX matCamState = XMMatrixInverse(nullptr, XMLoadFloat4x4(&matBill));
	XMVECTOR vRight = matCamState.r[3];
	m_pMyGDI->pEffect->SetWorldViewProj(matCamState);*/

	//m_pMyGDI->pEffect->SetWorldViewProj()

	//m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight2Tech();
	m_pMyGDI->pTech = m_pMyGDI->pEffect->GetWavesTech();

	//->pContext->OMSetBlendState
	//m_pMyGDI->pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//와이어 프레임
	D3D11_RASTERIZER_DESC dxRasterDesc;
	::memset(&dxRasterDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	//dxRasterDesc.FillMode = D3D11_FILL_WIREFRAME;       //와이어
	dxRasterDesc.FillMode = D3D11_FILL_SOLID;          //아닌거
	dxRasterDesc.CullMode = D3D11_CULL_NONE;
	dxRasterDesc.DepthClipEnable = TRUE;
	m_pMyGDI->pDevice->CreateRasterizerState(&dxRasterDesc, &m_pWire);
	m_pMyGDI->pContext->RSSetState(m_pWire);


	//블렌드
	//D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	////alphaToCoverageDesc.AlphaToCoverageEnable = true;
	////alphaToCoverageDesc.IndependentBlendEnable = false;
	//alphaToCoverageDesc.RenderTarget[0].BlendEnable = true;
	//alphaToCoverageDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//alphaToCoverageDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	////alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//HR(m_pMyGDI->pDevice->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));


	// 재질 설정
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();


	m_pMyGDI->pEffect->SetTexTransform(matWorld/*XMLoadFloat4x4(&m_pInfo->m_matWorld)*/);
	m_pMyGDI->pEffect->SetMaterial(m_tMtrl);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	m_pMyGDI->pEffect->SetWorld(matWorld);
	m_pTexture->Render(0, 1);


	m_pBuffer->Render(m_pInfo->m_matWorld);
}

HRESULT CRain2::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Rain");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CRain2* CRain2::Create(Engine::MYGDI* pMyGDI, const Engine::CTransform* pGameObj, Engine::CGameObject* pCamera)
{
	CRain2*		pGameObject = new CRain2(pMyGDI);

	pGameObject->m_pTargetInfo = pGameObj;
	pGameObject->m_pCamera = pCamera;

	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);


	return pGameObject;
}
