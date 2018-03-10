#include "WavesTex.h"
#include "Effects.h"
#include "Export_System.h"
#include "ResourceMgr.h"
#include "RenderStates.h"
#include "ParticleSystem.h"

Engine::CWavesTex::CWavesTex(MYGDI * pMyGDI)
	:CVIBuffer(pMyGDI),
	mfxMaterial(0),
	mfxDirLight(0),
	mEyePosW(0.0f, 0.0f, 0.0f),
	mWaterTexOffset(0.0f, 0.0f),
	mGameTime(0.f),
	mTimeStep(0.f),
	mAge(0.f)
{
	m_pTech = m_pMyGDI->pEffect->GetWavesTech();
	mWaves.Init(200, 200, 0.8f, 0.03f, 3.25f, 0.4f);

	mWavesMat.Ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	mWavesMat.Diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	mWavesMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);
	//mWavesMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//mWavesMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//mWavesMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

	// Directional light.
	mDirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	// Point light--position is changed every frame to animate in UpdateScene function.
	mPointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight.Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mPointLight.Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mPointLight.Att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	mPointLight.Range = 25.0f;

	// Spot light--position and direction changed every frame to animate in UpdateScene function.
	mSpotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	mSpotLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSpotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mSpotLight.Spot = 96.0f;
	mSpotLight.Range = 10000.0f;


	//Engine::Get_Effects()->InitAll(pMyGDI, &m_pFX);
	//Engine::Get_Effects()->InitAll(pMyGDI, &m_pFX);
	//mfxDirLight = m_pFX->GetVariableByName("gDirLight");
	//mfxMaterial = m_pFX->GetVariableByName("gMaterial");
	//World = ->GetVariableByName("gWorld")->AsMatrix();

	mRain.Init(m_pMyGDI->pDevice, NULL, NULL, 10000, pMyGDI);


}


Engine::CWavesTex::CWavesTex(const CWavesTex & rhs)
	:CVIBuffer(rhs)
{

}


Engine::CWavesTex::~CWavesTex()
{
	RenderStates::DestroyAll();
}

HRESULT Engine::CWavesTex::CreateBuffer(void)
{
	//ID3D11Resource* texResource = nullptr;
	//HR(DirectX::CreateDDSTextureFromFile(m_pMyGDI->pDevice,
	//	L"Textures/water2.dds", &texResource, &mWavesMapSRV));
	//ReleaseCOM(texResource); // view saves reference

	/*HRESULT hr = NULL;
	hr = CResourceMgr::GetInstance()->AddTexture(m_pMyGDI
		, 1
		, Engine::TEXTURE_CUBE
		, L"Texture_Water"
		, L"../bin/Resources/Texture/water/water%d.dds", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	mWavesMapSRV-> CResourceMgr::GetInstance()->CloneResource(1, L"Texture_Water");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);*/

	D3D11_INPUT_ELEMENT_DESC vertexDesc[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));

	m_dwVtxSize = sizeof(VertexWaves);
	m_dwVtxCnt = sizeof(VertexWaves) * mWaves.VertexCount();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexWaves) * mWaves.VertexCount();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	HR(m_pMyGDI->pDevice->CreateBuffer(&vbd, 0, &m_pVB));


	// Create the index buffer

	m_dwIdxSize = sizeof(UINT);
	m_dwIndexCnt = 3 * mWaves.TriangleCount(); // 삼각형의 개수 * 삼각형 1개당 버텍스 수

	std::vector<UINT> indices(3 * mWaves.TriangleCount()); // 3 indices per face

														   // Iterate over each quad.
	UINT m = mWaves.RowCount();
	UINT n = mWaves.ColumnCount();
	int k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i*n + j;
			indices[k + 1] = i*n + j + 1;
			indices[k + 2] = (i + 1)*n + j;

			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i*n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;

			k += 6; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pMyGDI->pDevice->CreateBuffer(&ibd, &iinitData, &m_pIB));

	return S_OK;
}

Engine::CResources * Engine::CWavesTex::CloneResource(void)
{
	++m_dwRefCnt;

	CResources* pResources = new CWavesTex(*this);

	return pResources;
}

Engine::CWavesTex * Engine::CWavesTex::Create(MYGDI * pMyGDI)
{
	CWavesTex* pBuffer = new CWavesTex(pMyGDI);
	if (FAILED(pBuffer->CreateBuffer()))
	{
		Safe_Delete(pBuffer);
	}
	//mWaves pBuffer
	return pBuffer;
}

float Engine::CWavesTex::GetHeight(float x, float z) const
{
	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
	//return 0.0f;
}

void Engine::CWavesTex::DrawWaves(XMFLOAT4X4& rmatWorld, float dt, ID3D11ShaderResourceView** pTexture)
{
	if (GetAsyncKeyState('1') & 0x8000)
		mRenderOptions = RenderOptions::Lighting;

	if (GetAsyncKeyState('2') & 0x8000)
		mRenderOptions = RenderOptions::Textures;

	if (GetAsyncKeyState('3') & 0x8000)
		mRenderOptions = RenderOptions::TexturesAndFog;


	switch (mRenderOptions)
	{
	case RenderOptions::Lighting:
		m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight0Tech();
		break;
	case RenderOptions::Textures:
		m_pMyGDI->pTech = m_pMyGDI->pEffect->GetLight1Tech();
		break;
	case RenderOptions::TexturesAndFog:
		m_pMyGDI->pTech = m_pMyGDI->pEffect->GetWavesTech();
		break;
	}


	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_pMyGDI->pEffect->SetFogColor(Colors::Silver);
	m_pMyGDI->pEffect->SetFogStart(15.0f);
	m_pMyGDI->pEffect->SetFogRange(175.0f);

	// Convert Spherical to Cartesian coordinates.
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	////
	// Tile water texture.
	XMMATRIX wavesScale = XMMatrixScaling(15.0f, 15.0f, 0.0f);
	// Translate texture over time.
	mWaterTexOffset.y += 0.05f * dt;
	mWaterTexOffset.x += 0.1f * dt;
	XMMATRIX wavesOffset = XMMatrixTranslation(mWaterTexOffset.x, mWaterTexOffset.y, 0.0f);
	// Combine scale and translation.
	XMStoreFloat4x4(&mWaterTexTransform, wavesScale*wavesOffset);


	mEyePosW = XMFLOAT3(x, y, z);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	
	m_pMyGDI->pEffect->SetDirLights(&mDirLight);
	m_pMyGDI->pEffect->SetWorld(XMLoadFloat4x4(&rmatWorld));

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pMyGDI->pTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
	
		UINT iOffset = 0;
		m_pMyGDI->pContext->IASetVertexBuffers(0, 1, &m_pVB, &m_dwVtxSize, &iOffset);
		m_pMyGDI->pContext->IASetIndexBuffer(m_pIB , DXGI_FORMAT_R32_UINT, 0);
		m_pMyGDI->pEffect->SetDiffuseMap(*pTexture);
		m_pMyGDI->pEffect->SetTexTransform(XMLoadFloat4x4(&mWaterTexTransform));
		m_pMyGDI->pEffect->SetMaterial1(mWavesMat);

		m_pMyGDI->pContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
		m_pMyGDI->pTech->GetPassByIndex(p)->Apply(0, m_pMyGDI->pContext);
		m_pMyGDI->pContext->DrawIndexed(3 * mWaves.TriangleCount(), 0, 0);

		// Restore default blend state
		m_pMyGDI->pContext->OMSetBlendState(0, blendFactor, 0xffffffff);
		// Restore default.
		m_pMyGDI->pContext->RSSetState(0);
	}

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pMyGDI->pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	VertexWaves* v = reinterpret_cast<VertexWaves*>(mappedData.pData);
	for (UINT i = 0; i < mWaves.VertexCount(); ++i)
	{
		v[i].Pos = mWaves[i];
		v[i].Normal = mWaves.Normal(i);
		//v[i].Color = mWaves.Normal(i);
		v[i].Tex.x = 0.5f + mWaves[i].x / mWaves.Width();
		v[i].Tex.y = 0.5f - mWaves[i].z / mWaves.Depth();
	}

	m_pMyGDI->pContext->Unmap(m_pVB, 0);
	//mPointLight.Position.x = 70.0f*cosf(0.2f*mTimer.TotalTime());
	//mPointLight.Position.z = 70.0f*sinf(0.2f*mTimer.TotalTime());
	//mPointLight.Position.y = MathHelper::Max(GetHillHeight(mPointLight.Position.x,
	//mPointLight.Position.z), -3.0f) + 10.0f;

	mSpotLight.Position = mEyePosW;
	XMStoreFloat3(&mSpotLight.Direction, XMVector3Normalize(target - pos));
}

void Engine::CWavesTex::RainUpdate(float dt, float gameTime)
{

	mGameTime = gameTime;
	mTimeStep = dt;
	mAge += dt;
}

void Engine::CWavesTex::DrawRain(XMFLOAT4X4& rmatWorld, float dt, ID3D11ShaderResourceView** pTexture)
{

	if (GetAsyncKeyState('R') & 0x8000)
	{
		//mFire.Reset();
		mRain.Reset();
	}
	//mRain.SetEyePos(mCam.GetPosition());
	//mRain.SetEmitPos(mCam.GetPosition());

	//float fb;
	//fb += dt;
	//mRain.Update(dt, dt);
	mRain.Draw(m_pMyGDI->pContext, rmatWorld,dt,pTexture);
}

