//***************************************************************************************
// CParticleSystem.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "ParticleSystem.h"
//#include "TextureMgr.h"
//#include "Vertex.h"
#include "Effects.h"
#include "Camera.h"



Engine::CParticleSystem::CParticleSystem()
{
}

Engine::CParticleSystem::CParticleSystem(MYGDI * pMyGDI)
:m_pMyGDI(pMyGDI),mInitVB(0), mDrawVB(0), mStreamOutVB(0), mTexArraySRV(0), mRandomTexSRV(0)
{

	mFirstRun = true;
	mGameTime = 0.0f;
	mTimeStep = 0.0f;
	mAge      = 0.0f;

	mEyePosW  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mEmitPosW = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mEmitDirW = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_pInputLayout =0;



}

Engine::CParticleSystem::~CParticleSystem()
{
	ReleaseCOM(mInitVB);
	ReleaseCOM(mDrawVB);
	ReleaseCOM(mStreamOutVB);
}

//Engine::CResources * Engine::CParticleSystem::CloneResource(void)
//{
//	++m_dwRefCnt;
//
//	CResources* pResources = new CParticleSystem(*this);
//
//	return pResources;
//}

//Engine::CParticleSystem * Engine::CParticleSystem::Create(MYGDI * pMyGDI)
//{
//	CParticleSystem* pBuffer = new CParticleSystem(pMyGDI);
//	if (FAILED(pBuffer->CreateBuffer()))
//	{
//		Safe_Delete(pBuffer);
//	}
//	//mWaves pBuffer
//	return pBuffer;
//}


float Engine::CParticleSystem::GetAge()const
{
	return mAge;
}

void Engine::CParticleSystem::SetEyePos(const XMFLOAT3& eyePosW)
{
	mEyePosW = eyePosW;
}

void Engine::CParticleSystem::SetEmitPos(const XMFLOAT3& emitPosW)
{
	mEmitPosW = emitPosW;
}

void Engine::CParticleSystem::SetEmitDir(const XMFLOAT3& emitDirW)
{
	mEmitDirW = emitDirW;
}

void Engine::CParticleSystem::Init(ID3D11Device* device, ID3D11ShaderResourceView* texArraySRV,
	                      ID3D11ShaderResourceView* randomTexSRV, UINT maxParticles, MYGDI * pMyGDI)
{
	mMaxParticles = maxParticles;

	//mFX = fx;

	mTexArraySRV  = texArraySRV;
	mRandomTexSRV = randomTexSRV; 

	BuildVB(device);

	m_pMyGDI = pMyGDI;
	m_pTech1 = m_pMyGDI->pEffect->GetStreamOutTech(); //여기부터하면돼! 스트림아웃 페스에서 쉐이더에서 만들기!
	m_pTech2 = m_pMyGDI->pEffect->GetRainTech();
}

void Engine::CParticleSystem::Reset()
{
	mFirstRun = true;
	mAge      = 0.0f;
}

void Engine::CParticleSystem::Update(float dt, float gameTime)
{
	mGameTime = gameTime;
	mTimeStep = dt;
	mAge += dt;
}

void Engine::CParticleSystem::Draw(ID3D11DeviceContext* dc,XMFLOAT4X4& rmatWorld, float dt, ID3D11ShaderResourceView** pTexture)
{
	//XMMATRIX VP = cam.ViewProj();

	//
	// Set constants.
	//
	//mFX->SetViewProj(VP);

	//XMMATRIX P = XMMatrixIdentity();
//	m_pMyGDI->pEffect->SetViewProj(P);
	m_pMyGDI->pEffect->SetWorld(XMLoadFloat4x4(&rmatWorld));
	//m_pMyGDI->pEffect->SetWorld(P);
	m_pMyGDI->pEffect->SetGameTime(mGameTime);
	m_pMyGDI->pEffect->SetTimeStep(mTimeStep);

	m_pMyGDI->pEffect->SetEyePosW(XMFLOAT3(0, 1, 0));
	m_pMyGDI->pEffect->SetEmitPosW(XMFLOAT3(0, 1, 0));

	mEmitDirW = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_pMyGDI->pEffect->SetEmitDirW(mEmitDirW);

	m_pMyGDI->pEffect->SetTexArray(*pTexture);
	m_pMyGDI->pEffect->SetRandomTex(*pTexture);



	// Set IA stage.
	//

	//HR(m_pMyGDI->pDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature,
	//	passDesc.IAInputSignatureSize, &m_pInputLayout));

	
	const D3D11_INPUT_ELEMENT_DESC Particlee[5]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	D3DX11_PASS_DESC passDesc;
	//m_pTech = m_pMyGDI->pEffect->GetStreamOutTech(); //여기부터하면돼! 스트림아웃 페스에서 쉐이더에서 만들기!

	m_pTech1->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(Particlee, 5, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));


	//
	// Set IA stage.
	//
	dc->IASetInputLayout(m_pInputLayout); //여기다름
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(Particle);
    UINT offset = 0;

	// On the first pass, use the initialization VB.  Otherwise, use
	// the VB that contains the current particle list.
	if( mFirstRun )
		dc->IASetVertexBuffers(0, 1, &mInitVB, &stride, &offset);
	else
		dc->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);

	//
	// Draw the current particle list using stream-out only to update them.  
	// The updated vertices are streamed-out to the target VB. 
	//
	dc->SOSetTargets(1, &mStreamOutVB, &offset);

    D3DX11_TECHNIQUE_DESC techDesc;
	m_pTech1->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		m_pTech1->GetPassByIndex( p )->Apply(0, dc);
        
		if( mFirstRun )
		{
			dc->Draw(1, 0);
			mFirstRun = false;
		}
		else
		{
			dc->DrawAuto();
		}
    }

	// done streaming-out--unbind the vertex buffer
	ID3D11Buffer* bufferArray[1] = {0};
	dc->SOSetTargets(1, bufferArray, &offset);

	// ping-pong the vertex buffers
	std::swap(mDrawVB, mStreamOutVB);

	//
	// Draw the updated particle system we just streamed-out. 
	//
	dc->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);

	//m_pTech = m_pMyGDI->pEffect->GetRainTech();
	m_pTech2->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		m_pTech2->GetPassByIndex( p )->Apply(0, dc);
        
		dc->DrawAuto();
    }
}

void Engine::CParticleSystem::BuildVB(ID3D11Device* device)
{
	//
	// Create the buffer to kick-off the particle system.
	//

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Particle) * 1;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// The initial particle emitter has type 0 and age 0.  The rest
	// of the particle attributes do not apply to an emitter.
	Particle p;
	ZeroMemory(&p, sizeof(Particle));
	p.Age  = 0.0f;
	p.Type = 0; 
 
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &p;

	HR(device->CreateBuffer(&vbd, &vinitData, &mInitVB));
	
	//
	// Create the ping-pong buffers for stream-out and drawing.
	//
	vbd.ByteWidth = sizeof(Particle) * mMaxParticles;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

    HR(device->CreateBuffer(&vbd, 0, &mDrawVB));
	HR(device->CreateBuffer(&vbd, 0, &mStreamOutVB));
}