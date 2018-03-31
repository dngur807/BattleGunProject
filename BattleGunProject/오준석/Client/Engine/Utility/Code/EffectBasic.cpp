#include "EffectBasic.h"

Engine::CEffectBasic::CEffectBasic(MYGDI* pMyGDI, const wstring& wstrFileName)
	: m_pMyGDI(pMyGDI)
	, m_pFX(nullptr)
{
	CreateFX(wstrFileName);
	InitEffect();
}

Engine::CEffectBasic::CEffectBasic(const CEffectBasic& rhs)
	: m_pMyGDI(rhs.m_pMyGDI)
	, m_pFX(rhs.m_pFX)
	, m_fxTech(rhs.m_fxTech)
	, m_fxVecEyePosW(rhs.m_fxVecEyePosW)
	, m_fxMatWorld(rhs.m_fxMatWorld)
	, m_fxMatView(rhs.m_fxMatView)
	, m_fxMatProj(rhs.m_fxMatProj)
	, m_fxMatTextureTransform(rhs.m_fxMatTextureTransform)
	, m_fxMatShadowTransform(rhs.m_fxMatShadowTransform)
	, m_fxMatWorldInvTranspose(rhs.m_fxMatWorldInvTranspose)
	, m_fxTextureMap(rhs.m_fxTextureMap)
	, m_fxNormalMap(rhs.m_fxNormalMap)
	, m_fxSpecularMap(rhs.m_fxSpecularMap)
	, m_fxShadowMap(rhs.m_fxShadowMap)
	, m_fxDepthMap(rhs.m_fxDepthMap)
	, m_fxMaterial(rhs.m_fxMaterial)
	, m_fxDirLights(rhs.m_fxDirLights)
	, m_fxSpotLights(rhs.m_fxSpotLights)
	, m_fxPlayerAngleY(rhs.m_fxPlayerAngleY)
	, m_fxPlayerAngleX(rhs.m_fxPlayerAngleX)
	, m_fxWorldViewProj(rhs.m_fxWorldViewProj)
	, mfxFogStart(rhs.mfxFogStart)
	, mfxFogRange(rhs.mfxFogRange)
	, mfxFogColor(rhs.mfxFogColor)
	, m_fxLastMap(rhs.m_fxLastMap)
	, m_fxLastWorld(rhs.m_fxLastWorld)
	, m_fxGrappling(rhs.m_fxGrappling)
{
}

void Engine::CEffectBasic::CreateFX(const wstring& wstrFileName)
{
	ifstream fin(wstrFileName, ios::binary);

	fin.seekg(0, ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, ios_base::beg);
	vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, m_pMyGDI->pDevice, &m_pFX));

	compiledShader.clear();
}


void Engine::CEffectBasic::InitEffect(void)
{

	m_fxMatWorld = m_pFX->GetVariableByName("g_matWorld")->AsMatrix();
	m_fxMatView = m_pFX->GetVariableByName("g_matView")->AsMatrix();
	m_fxMatProj = m_pFX->GetVariableByName("g_matProj")->AsMatrix();
	m_fxVecEyePosW = m_pFX->GetVariableByName("g_EyePos")->AsVector();
	m_fxLastWorld = m_pFX->GetVariableByName("g_PreWorld")->AsMatrix();

	m_fxTextureMap = m_pFX->GetVariableByName("g_DiffuseMap")->AsShaderResource();
	m_fxNormalMap = m_pFX->GetVariableByName("g_NormalMap")->AsShaderResource();
	m_fxSpecularMap = m_pFX->GetVariableByName("g_SpecularMap")->AsShaderResource();
	m_fxShadowMap = m_pFX->GetVariableByName("g_ShadowMap")->AsShaderResource();
	m_fxDepthMap = m_pFX->GetVariableByName("g_DepthMap")->AsShaderResource();
	m_fxLastMap = m_pFX->GetVariableByName("g_LastMap")->AsShaderResource();

	m_fxMatTextureTransform = m_pFX->GetVariableByName("g_matTextureTransform")->AsMatrix();
	m_fxMatWorldInvTranspose = m_pFX->GetVariableByName("g_matWorldInvTranspose")->AsMatrix();

	m_fxMaterial = m_pFX->GetVariableByName("g_Material");
	m_fxDirLights = m_pFX->GetVariableByName("g_DirLights");
	m_fxSpotLights = m_pFX->GetVariableByName("g_SpotLights");
	m_fxPlayerAngleY = m_pFX->GetVariableByName("gPlayerAngleY")->AsScalar();
	m_fxPlayerAngleX = m_pFX->GetVariableByName("gPlayerAngleX")->AsScalar();

	m_fxWorldViewProj = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();

	mfxFogStart = m_pFX->GetVariableByName("gFogStart")->AsScalar();
	mfxFogRange = m_pFX->GetVariableByName("gFogRange")->AsScalar();
	mfxFogColor = m_pFX->GetVariableByName("gFogColor")->AsVector();
	m_fxGrappling = m_pFX->GetVariableByName("g_Grappling")->AsScalar();

}


void Engine::CEffectBasic::SetWorld(DirectX::CXMMATRIX M)
{
	m_fxMatWorld->SetMatrix(reinterpret_cast<const float*>(&M));
}
void Engine::CEffectBasic::SetView(DirectX::CXMMATRIX M)
{
	m_fxMatView->SetMatrix(reinterpret_cast<const float*>(&M));
}
void Engine::CEffectBasic::SetProj(DirectX::CXMMATRIX M)
{
	m_fxMatProj->SetMatrix(reinterpret_cast<const float*>(&M));
}
void Engine::CEffectBasic::SetLastWorld(CXMMATRIX M)
{
	m_fxLastWorld->SetMatrix(reinterpret_cast<const float*>(&M));
}
void Engine::CEffectBasic::SetTexTransform(DirectX::CXMMATRIX M)
{
	m_fxMatTextureTransform->SetMatrix(reinterpret_cast<const float*>(&M));
}
void Engine::CEffectBasic::SetEyePosW(const DirectX::XMFLOAT3& v)
{
	m_fxVecEyePosW->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3));
}
void Engine::CEffectBasic::SetMaterial(const Material& mat)
{
	m_fxMaterial->SetRawValue(&mat, 0, sizeof(Material));
}
void Engine::CEffectBasic::SetGrapping(const FLOAT & v)
{
	m_fxGrappling->SetFloat(v);
}
void Engine::CEffectBasic::SetWorldInvTranspose(DirectX::CXMMATRIX M)
{
	m_fxMatWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M));
}
void Engine::CEffectBasic::SetDirLights(const DirectionalLight* lights)
{
	m_fxDirLights->SetRawValue(lights, 0, sizeof(DirectionalLight));
}

void Engine::CEffectBasic::SetSpotLights(const SpotLight* lights)
{
	m_fxSpotLights->SetRawValue(lights, 0, sizeof(SpotLight));
}
void Engine::CEffectBasic::SetTextureMap(ID3D11ShaderResourceView* texture)
{
	m_fxTextureMap->SetResource(texture);
}
void Engine::CEffectBasic::SetNormalMap(ID3D11ShaderResourceView* normal)
{
	m_fxNormalMap->SetResource(normal);
}
void Engine::CEffectBasic::SetSpecularMap(ID3D11ShaderResourceView* specular)
{
	m_fxSpecularMap->SetResource(specular);
}
void Engine::CEffectBasic::SetShadowMap(ID3D11ShaderResourceView* shadow)
{
	m_fxShadowMap->SetResource(shadow);
}

void Engine::CEffectBasic::SetDepthMap(ID3D11ShaderResourceView* depth)
{
	m_fxDepthMap->SetResource(depth);
}

void Engine::CEffectBasic::SetLastMap(ID3D11ShaderResourceView* lastmap)
{
	m_fxLastMap->SetResource(lastmap);
}
ID3DX11EffectTechnique* Engine::CEffectBasic::GetTech(void)
{
	return m_fxTech;
}

void Engine::CEffectBasic::Apply()
{
	// Applies the changes effect and it changes to the pipeline.
	m_pMyGDI->pEffectBasic->GetTech()->GetPassByIndex(0)->Apply(0, m_pMyGDI->pContext);
}


DWORD Engine::CEffectBasic::Release(void)
{
	return 0;
}
