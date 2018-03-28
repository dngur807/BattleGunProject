#ifndef Engine_Struct_h__
#define Engine_Struct_h__


namespace Engine
{
	class CBasicEffect;
	// 맵 저장용 구조체
	typedef struct tagNavMesh
	{
		int			iOption;
		XMFLOAT3 vecNavMesh[3];
	}NAVMESH;
	typedef struct tagDeviceInterface
	{
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;
		ID3DX11EffectTechnique* pTech;
		CBasicEffect*	pEffect;
	}MYGDI;

	typedef struct tagVertexColor
	{
		DirectX::XMFLOAT3	vPos;
		DirectX::XMFLOAT4 	vColor;
	}VTXCOL;

	typedef struct tagVertexNormal
	{
		DirectX::XMFLOAT3	vPos;
		DirectX::XMFLOAT3 	vNormal;
	}VTXNOR;

	typedef struct tagVertexCube
	{
		XMFLOAT3 vPos;
		XMFLOAT3 vNormal;
		XMFLOAT3 vTexC;
	}VTXCUBE;

	typedef struct tagVertexColorNormal
	{
		DirectX::XMFLOAT3	vPos;
		DirectX::XMFLOAT4 	vColor;
		DirectX::XMFLOAT3 	vNormal;
	}VTXCN;

	typedef struct tagVertexTexture
	{
		tagVertexTexture() {}
		tagVertexTexture(XMFLOAT3 _vPos, XMFLOAT3 _vNormal, XMFLOAT2 _vTex)
			:vPos(_vPos), vNormal(_vNormal), vTexC(_vTex) {}

		DirectX::XMFLOAT3 vPos;
		DirectX::XMFLOAT3 vNormal;
		DirectX::XMFLOAT2 vTexC;
	}VTXTEX;

	//! Skinned vertex.
	struct SkinnedVertex
	{
		SkinnedVertex() {}
		SkinnedVertex(float px, float py, float pz, float nx, float ny, float nz)
			: Pos(px, py, pz), Normal(nx, ny, nz) {}
		SkinnedVertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float tz, float u, float v)
			: Pos(px, py, pz), Normal(nx, ny, nz), Tangent(tx, ty, tz, 1.0f), Tex(u, v) {
			WeightsA = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
			WeightsB = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
			BoneIndicesA[0] = BoneIndicesA[1] = BoneIndicesA[2] = BoneIndicesA[3] = 0;
			BoneIndicesB[0] = BoneIndicesB[1] = BoneIndicesB[2] = BoneIndicesB[3] = 0;
		}

		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 Tangent;
		XMFLOAT4 WeightsA;
		XMFLOAT4 WeightsB;
		BYTE BoneIndicesA[4];
		BYTE BoneIndicesB[4];
	};

	typedef struct tagVertexTotal
	{
		tagVertexTotal() {}
		tagVertexTotal(const DirectX::XMFLOAT3& p,
			const DirectX::XMFLOAT3& n,
			const DirectX::XMFLOAT3& t,
			const DirectX::XMFLOAT2& uv)
			: vPos(p), vNormal(n), vTanU(t), vTexC(uv) {}
		tagVertexTotal(
			float px, float py, float pz, 
			float nx, float ny, float nz, 
			float tx, float ty, float tz, 
			float u, float v)
			:vPos(px, py, pz), vNormal(nx, ny, nz),
			vTanU(tx, ty, tz), vTexC(u, v) {}

		DirectX::XMFLOAT3 vPos;
		DirectX::XMFLOAT3 vNormal;
		DirectX::XMFLOAT3 vTanU;
		DirectX::XMFLOAT2 vTexC;
	}VTXTOT;

	typedef struct tagMeshdata
	{
		std::vector<tagVertexTotal> Vertices;
		std::vector<UINT> Indices;
	}MESHINFO;

	/*typedef struct tagVertexCube
	{
		DirectX::XMFLOAT3	vPos;
		DirectX::XMFLOAT3	vTex;
	}VTXCUBE;*/

	typedef struct tagIndex16
	{
		WORD	_1, _2, _3;
	}INDEX16;

	/*typedef struct tagIndex32
	{
		DWORD	_1, _2, _3;
	}INDEX32;*/

	typedef struct tagAnimation
	{
		tagAnimation(float _fStartFrame, float _fEndFrame,
			float fTotalFrames, float fLength)
		{
			fStartTime = (_fStartFrame / fTotalFrames) * fLength;
			fEndTime = (_fEndFrame / fTotalFrames) * fLength;
		}
		tagAnimation(float _fStartTime, float _fEndTime)
			:fStartTime(_fStartTime), fEndTime(_fEndTime) {}

		float fStartTime;
		float fEndTime;
	}ANITIME;

	typedef struct tagTexture2D
	{
		ID3D11ShaderResourceView* pSRV;
		float fScale;
		std::string name;
	}TEX2D;

	//! Vertex weights and indices for bone animations.
	struct Weights
	{
		std::vector<int>		boneIndices;
		std::vector<float>	weights;
	};
}

#endif // !Engine_Struct_h__
