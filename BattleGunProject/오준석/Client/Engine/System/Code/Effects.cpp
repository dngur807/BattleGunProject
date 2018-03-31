#include "Effects.h"

#pragma region Effect
Engine::CEffect::CEffect(MYGDI* pMyGDI, ID3DX11Effect** ppFX,
	const std::wstring& wstrFileName)
	:m_pMyGDI(pMyGDI), m_pFX(nullptr)
{
	std::ifstream fin(wstrFileName, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], 
		size, 0, m_pMyGDI->pDevice, &m_pFX));

	*ppFX = m_pFX;

	//XMMATRIX I = XMMatrixIdentity();
	//XMStoreFloat4x4(&m_matWorld, I);
	//XMStoreFloat4x4(&m_matView, I);
	//XMStoreFloat4x4(&m_matProj, I);


}


Engine::CEffect::~CEffect()
{
	ReleaseCOM(m_pFX);
}

//! Applies the effect to the render pipeline.
void Engine::CEffect::Apply(ID3D11DeviceContext* pContext, EffectTech tech)
{
	// Applies the changes effect and it changes to the pipeline.
	m_pMyGDI->pTech->GetPassByIndex(0)->Apply(0, pContext);
}

#pragma endregion

#pragma region BasicEffect

Engine::CBasicEffect::CBasicEffect(MYGDI * pMyGDI,
	ID3DX11Effect** ppFX, const std::wstring & wstrFileName)
	:Engine::CEffect(pMyGDI, ppFX, wstrFileName)
{
	LightWavesTech = m_pFX->GetTechniqueByName("WavesTech");

	mfxCubeTech = m_pFX->GetTechniqueByName("CubeTech");
	mfxColorTech = m_pFX->GetTechniqueByName("ColorTech");

	Light1Tech = m_pFX->GetTechniqueByName("Light1");
	Light2Tech = m_pFX->GetTechniqueByName("Light2");
	Light3Tech = m_pFX->GetTechniqueByName("Light3");

	Light0TexTech = m_pFX->GetTechniqueByName("Light0Tex");
	Light1TexTech = m_pFX->GetTechniqueByName("Light1Tex");
	Light2TexTech = m_pFX->GetTechniqueByName("Light2Tex");
	Light3TexTech = m_pFX->GetTechniqueByName("Light3Tex");

	FogColor = m_pFX->GetVariableByName("gFogColor")->AsVector();
	FogStart = m_pFX->GetVariableByName("gFogStart")->AsScalar();
	FogRange = m_pFX->GetVariableByName("gFogRange")->AsScalar();

	World = m_pFX->GetVariableByName("gWorld")->AsMatrix();
	View = m_pFX->GetVariableByName("gView")->AsMatrix();
	Proj = m_pFX->GetVariableByName("gProj")->AsMatrix();
	WorldInvTranspose = m_pFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	BoneTransforms = m_pFX->GetVariableByName("gBoneTransforms")->AsMatrix();
	TexTransform = m_pFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW = m_pFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights = m_pFX->GetVariableByName("gDirLights");
	Mat = m_pFX->GetVariableByName("gMaterial");
	DiffuseMap = m_pFX->GetVariableByName("gDiffuseMap")->AsShaderResource();

	mfxSkinnedTech = m_pFX->GetTechniqueByName("SkinnedTech");
	mfxTexTransform = m_pFX->GetVariableByName("gTexTransform")->AsMatrix();
	mfxBoneTransforms = m_pFX->GetVariableByName("gBoneTransforms")->AsMatrix();
	mfxShadowTransform = m_pFX->GetVariableByName("gShadowTransform")->AsMatrix();
	mfxLights = m_pFX->GetVariableByName("gLights");
	//mfxMaterial = m_pFX->GetVariableByName("gMaterial");
	mfxNumLights = m_pFX->GetVariableByName("gNumLights");
	mfxUseLighting = m_pFX->GetVariableByName("gUseLighting")->AsScalar();
	UseTexture = m_pFX->GetVariableByName("gUseTexture")->AsScalar();
	mfxTexture = m_pFX->GetVariableByName("gTexture")->AsShaderResource();
	mfxFogStart = m_pFX->GetVariableByName("gFogStart")->AsScalar();
	mfxFogRange = m_pFX->GetVariableByName("gFogRange")->AsScalar();
	mfxFogColor = m_pFX->GetVariableByName("gFogColor")->AsVector();
	mfxNormalMap = m_pFX->GetVariableByName("gNormalMap")->AsShaderResource();
	mfxShadowMap = m_pFX->GetVariableByName("gShadowMap")->AsShaderResource();
	mfxUseNormalMap = m_pFX->GetVariableByName("gUseNormalMap")->AsScalar();
	mfxUseAnimation = m_pFX->GetVariableByName("gUseAnimation")->AsScalar();
	mfxRenderingToShadowMap = m_pFX->GetVariableByName("gRenderingToShadowMap")->AsScalar();
	mfxNormalMapTech = m_pFX->GetTechniqueByName("NormalMapTech");
	mfxShadowMapTech = m_pFX->GetTechniqueByName("ShadowMapTech");
	//Interplation to Change Animation Scene
	mfxWeightFactor = m_pFX->GetVariableByName("gWeightFactor")->AsScalar();
	mfxChangeScene = m_pFX->GetVariableByName("gChangeScene")->AsScalar();
	mfxPreBoneTransforms = m_pFX->GetVariableByName("gPreBoneTransforms")->AsMatrix();
	mfxPreWorld = m_pFX->GetVariableByName("gPreWorld")->AsMatrix();



	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_matWorld, I);
	XMStoreFloat4x4(&m_matView, I);
	XMStoreFloat4x4(&m_matProj, I);


}

Engine::CBasicEffect::~CBasicEffect(void)
{
}

void Engine::CBasicEffect::SetWorld(DirectX::CXMMATRIX M)
{ 
	XMStoreFloat4x4(&m_matWorld, M);
	World->SetMatrix(reinterpret_cast<const float*>(&M)); 
}
void Engine::CBasicEffect::SetView(DirectX::CXMMATRIX M)
{
	XMStoreFloat4x4(&m_matView, M);
	View->SetMatrix(reinterpret_cast<const float*>(&M));
}
void Engine::CBasicEffect::SetProj(DirectX::CXMMATRIX M)
{
	XMStoreFloat4x4(&m_matProj, M);
	Proj->SetMatrix(reinterpret_cast<const float*>(&M));
}
void Engine::CBasicEffect::SetWorldInvTranspose(DirectX::CXMMATRIX M)
{ WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
void Engine::CBasicEffect::SetBoneTransforms(const XMFLOAT4X4 * M, int cnt)
{ BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }
void Engine::CBasicEffect::SetTexTransform(DirectX::CXMMATRIX M)
{ TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
void Engine::CBasicEffect::SetEyePosW(const DirectX::XMFLOAT3& v)
{ EyePosW->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
void Engine::CBasicEffect::SetDirLights(const DirectionalLight* lights)
{ DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
void Engine::CBasicEffect::SetMaterial(const Material& mat)
{ Mat->SetRawValue(&mat, 0, sizeof(Material)); }
void Engine::CBasicEffect::SetMaterial1(const Material& mat)
{mfxMaterial->SetRawValue(&mat, 0, sizeof(Material));}
void Engine::CBasicEffect::SetDiffuseMap(ID3D11ShaderResourceView* tex)
{ DiffuseMap->SetResource(tex); }
void Engine::CBasicEffect::SetUseNormalMap(bool use)
{
	HRESULT hr = mfxUseNormalMap->SetBool(use);
}

//! Sets the texture to use in the shader.
void Engine::CBasicEffect::SetTexture(TEX2D* texture)
{
	bool f = texture == nullptr ? false : true;
	SetUseTexture(texture == nullptr ? false : true);
	if (texture != nullptr) {
		mfxTexture->SetResource(texture->pSRV);
		XMMATRIX transform = XMMatrixScaling(texture->fScale, texture->fScale, 0);
		TexTransform->SetMatrix((const float*)&transform);
	}
}
//! Sets the normal map to use.
void Engine::CBasicEffect::SetNormalMap(TEX2D* normalMap)
{
	SetUseNormalMap(normalMap == nullptr ? false : true);
	if (normalMap != nullptr) {
		mfxNormalMap->SetResource(normalMap->pSRV);
	}
}

void Engine::CBasicEffect::SetPreBoneTransforms(const XMFLOAT4X4 * M, int cnt)
{
	mfxPreBoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt);
}

void Engine::CBasicEffect::SetPreWorld(const CXMMATRIX M)
{
	mfxPreWorld->SetMatrix(reinterpret_cast<const float*>(&M));
}

void Engine::CBasicEffect::Apply(ID3D11DeviceContext* pContext, EffectTech tech)
{
	// Applies the changes effect and it changes to the pipeline.
	if (tech == STANDARD_TECH)
		//CEffect::Apply(pContext, tech);
		mfxSkinnedTech->GetPassByIndex(0)->Apply(0, pContext);
	//else if (tech == NMAP_TECH)
	else
		mfxNormalMapTech->GetPassByIndex(0)->Apply(0, pContext);
	/*else if (tech = SMAP_TECH)
		mfxShadowMapTech->GetPassByIndex(0)->Apply(0, pContext);*/
}

ID3DX11EffectTechnique * Engine::CBasicEffect::GetSkinnedTech(void)
{
	return mfxSkinnedTech;
}

ID3DX11EffectTechnique * Engine::CBasicEffect::GetCubeTech(void)
{
	return mfxCubeTech;
}

ID3DX11EffectTechnique * Engine::CBasicEffect::GetColorTech(void)
{
	return mfxColorTech;
}


#pragma endregion
 
#pragma region Effects

IMPLEMENT_SINGLETON(Engine::CEffects)

Engine::CEffects::CEffects()
	: BasicFX(NULL)
{
}

Engine::CEffects::~CEffects()
{
	Engine::Safe_Delete(BasicFX);
}

void Engine::CEffects::InitAll(MYGDI * pMyGDI, ID3DX11Effect** ppFX)
{
	Safe_Delete(BasicFX);
	BasicFX = new CBasicEffect(pMyGDI, ppFX, L"../bin/basic.cso");
}

//void Engine::CEffects::DestroyAll()
//{
//	Engine::Safe_Delete(BasicFX);
//}
#pragma endregion
