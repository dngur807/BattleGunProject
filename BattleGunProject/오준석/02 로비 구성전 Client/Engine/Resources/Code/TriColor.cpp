#include "TriColor.h"



Engine::CTriColor::CTriColor(MYGDI* pMyGDI)
	:CVIBuffer(pMyGDI)
{
}


Engine::CTriColor::~CTriColor()
{
}

void Engine::CTriColor::Render(void)
{
	m_pMyGDI->pContext->IASetInputLayout(m_pInputLayout);
	m_pMyGDI->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(VTXCOL);
	UINT offset = 0;
	m_pMyGDI->pContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	//m_pContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	m_pMyGDI->pTech->GetPassByIndex(0)->Apply(0, m_pMyGDI->pContext);

	m_pMyGDI->pContext->Draw(3, 0);
}

Engine::CResources * Engine::CTriColor::CloneResource(void)
{
	CResources* pResources = new CTriColor(*this);

	++m_dwRefCnt;

	return pResources;
}

HRESULT Engine::CTriColor::CreateBuffer(void)
{
	// Create the vertex input layout
	D3D11_INPUT_ELEMENT_DESC vtxDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//BuildFX();

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_pMyGDI->pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pMyGDI->pDevice->CreateInputLayout(vtxDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));

	VTXCOL vertices[] = 
	{
		{ DirectX::XMFLOAT3(0.f, 1.f, 0.f), (const DirectX::XMFLOAT4)DirectX::Colors::White },
		{ DirectX::XMFLOAT3(1.f, -1.f, 0.f), (const DirectX::XMFLOAT4)DirectX::Colors::Black },
		{ DirectX::XMFLOAT3(-1.f, -1.f, 0.f), (const DirectX::XMFLOAT4)DirectX::Colors::Red }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VTXCOL) * 3;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitdata;
	vinitdata.pSysMem = vertices;
	HR(m_pMyGDI->pDevice->CreateBuffer(&vbd, &vinitdata, &m_pVB));

	return S_OK;
}

Engine::CTriColor * Engine::CTriColor::Create(MYGDI* pMyGDI)
{
	CTriColor* pBuffer = new CTriColor(pMyGDI);
	if (FAILED(pBuffer->CreateBuffer()))
	{
		Safe_Delete(pBuffer);
	}

	return pBuffer;
}
