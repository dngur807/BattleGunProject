#pragma once
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CWaveFrontReader
{
public:
	struct Material
	{
		DirectX::XMFLOAT3 vAmbient;
		DirectX::XMFLOAT3 vDiffuse;
		DirectX::XMFLOAT3 vSpecular;
		uint32_t nShininess;
		float fAlpha;

		bool bSpecular;

		wchar_t strName[MAX_PATH];
		wchar_t strTexture[MAX_PATH];

		Material() :
			vAmbient(0.2f, 0.2f, 0.2f),
			vDiffuse(0.8f, 0.8f, 0.8f),
			vSpecular(1.0f, 1.0f, 1.0f),
			nShininess(0),
			fAlpha(1.f),
			bSpecular(false)
		{
			memset(strName, 0, sizeof(strName)); memset(strTexture, 0, sizeof(strTexture));
		}
	};

	DECLARE_SINGLETON(CWaveFrontReader)

private:
	CWaveFrontReader();
public:
	~CWaveFrontReader();

public:
	HRESULT LoadObj(_In_z_ const wchar_t* szFileName, bool ccw = true);
	HRESULT LoadMTL(_In_z_ const wchar_t* szFileName);
	void Clear(void);

public:
	std::vector<VTXTEX>*	GetVertices(void) { return &vertices; }
	std::vector<UINT>*		Getindices(void) { return &indices; }
	std::vector<uint32_t>*	Getattributes(void) { return &attributes; }
	std::vector<Material>*	Getmaterials(void) { return &materials; }


private:
	typedef std::unordered_multimap<UINT, UINT> VertexCache;

	DWORD AddVertex(UINT hash, VTXTEX* pVertex, VertexCache& cache);

private:
	std::vector<VTXTEX>     vertices;
	std::vector<UINT>		indices;
	std::vector<uint32_t>   attributes;
	std::vector<Material>   materials;

	std::wstring            name;
	bool                    hasNormals;
	bool                    hasTexcoords;

	BoundingBox    bounds;
};

END