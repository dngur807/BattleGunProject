#ifndef LightHelper_h__
#define LightHelper_h__

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace PackedVector;

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct PointLight
{
	PointLight(void) { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	//Packed into 4D vector : (Position, Range)
	XMFLOAT3 Position;
	float Range;

	// Packed into 4D vector : (A0, A1, A2, Pad)
	XMFLOAT3 Att;
	float Pad;
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	// Packed into 4D vector : (Position, Range)
	XMFLOAT3 Position;
	float Range;

	// Packed into 4D vector : (Direction, Spot)
	XMFLOAT3 Direction;
	float Spot;

	// Packed into 4D vector : (Att, Pad)
	XMFLOAT3 Att;
	float Pad;
};

//struct Material
//{
//	Material() { ZeroMemory(this, sizeof(this)); }
//
//	XMFLOAT4 Ambient;
//	XMFLOAT4 Diffuse;
//	XMFLOAT4 Specular; // w = SpecPower
//	XMFLOAT4 Reflect;
//};

typedef struct tagMaterial
{
	tagMaterial() {
		ZeroMemory(this, sizeof(this));
	}

	tagMaterial(XMFLOAT4 color) {
		Ambient = Diffuse = Specular = color;
	}

	tagMaterial(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular) {
		this->Ambient = ambient;
		this->Diffuse = diffuse;
		this->Specular = specular;
	}

	tagMaterial(aiColor4D ambient, aiColor4D diffuse, aiColor4D specular) {
		this->Ambient.x = ambient.r;
		this->Ambient.y = ambient.g;
		this->Ambient.z = ambient.b;
		this->Ambient.w = ambient.a;

		this->Diffuse.x = diffuse.r;
		this->Diffuse.y = diffuse.g;
		this->Diffuse.z = diffuse.b;
		this->Diffuse.w = diffuse.a;

		this->Specular.x = specular.r;
		this->Specular.y = specular.g;
		this->Specular.z = specular.b;
		this->Specular.w = specular.a;
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = SpecPower
	XMFLOAT4 Reflect;
}Material;

#endif