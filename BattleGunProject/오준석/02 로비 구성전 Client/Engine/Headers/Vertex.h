#include "Engine_Include.h"

BEGIN(Engine)

struct Basic32
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 Tex;
};

struct Vertex
{
	Vertex() {}
	Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n,
		const DirectX::XMFLOAT3& t, const DirectX::XMFLOAT2& uv)
		: Position(p), Normal(n), TangentU(t), TexC(uv) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v)
		: Position(px, py, pz), Normal(nx, ny, nz),
		TangentU(tx, ty, tz), TexC(u, v) {}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 TangentU;
	DirectX::XMFLOAT2 TexC;
};

struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<UINT>	Indices;
};

class ENGINE_DLL InputLayoutDesc
{
public:
	// Init like const int A::a[4] = { 0, 1, 2, 3 }; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC Basic32[3];
};

class ENGINE_DLL InputLayouts
{
public:
	static void InitAll(ID3D11Device* pDevice);
	static void DestroyAll(void);

	static ID3D11InputLayout* Basic32;
};

END
