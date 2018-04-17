#pragma once
#include "stdafx.h"
#include "Include.h"

inline XMFLOAT3 GetScreenPos(XMFLOAT3 _vPos, XMFLOAT4X4 _MView, XMFLOAT4X4 _MProj)
{
	XMFLOAT4X4 Mvs;//∫‰Ω∫ƒ…¿œ∏µ
	XMStoreFloat4x4(&Mvs, XMMatrixIdentity());
	D3D11_VIEWPORT _ViewPort;

	_ViewPort.TopLeftX = 0;
	_ViewPort.TopLeftY = 0;
	_ViewPort.Width = static_cast<float>(WINCX);
	_ViewPort.Height = static_cast<float>(WINCY);
	_ViewPort.MinDepth = 0.0f;
	_ViewPort.MaxDepth = 1.0f;


	Mvs._11 = _ViewPort.Width / 2.f;
	Mvs._22 = -(_ViewPort.Height / 2.f);
	Mvs._33 = _ViewPort.MaxDepth - _ViewPort.MinDepth;
	Mvs._41 = _ViewPort.TopLeftX + (_ViewPort.Width / 2.f);
	Mvs._42 = _ViewPort.TopLeftY + (_ViewPort.Height / 2.f);
	Mvs._43 = _ViewPort.MinDepth;
	Mvs._44 = 1;

	float CX = -1.f, CY = 1.f;
	float CW = 2.f, CH = 2.f;
	float Zmax = 1.f, ZMin = 0.f;

	XMFLOAT4X4 MClip;
	XMStoreFloat4x4(&MClip, XMMatrixIdentity());
	MClip._11 = CW / 2.f;
	MClip._22 = CH / 2.f;
	MClip._33 = 1 / (Zmax - ZMin);
	MClip._41 = -1 - 2 * CX / CW;
	MClip._42 = 1 - 2 * CY / CH;
	MClip._43 = 0;
	MClip._44 = 1;


	XMMATRIX MultiMatrix =XMLoadFloat4x4(&_MView) * XMLoadFloat4x4(&_MProj) * XMLoadFloat4x4(&MClip) * XMLoadFloat4x4(&Mvs);
	XMFLOAT4 vPos;
	vPos.x = _vPos.x;
	vPos.y = _vPos.y;
	vPos.z = _vPos.z;
	vPos.w = 1;

	XMFLOAT4X4 MultiMat;
	XMStoreFloat4x4(&MultiMat, MultiMatrix);
	XMFLOAT4 vOut;
	vOut.x = vPos.x *  MultiMat._11 + vPos.y *  MultiMat._21 + vPos.z *  MultiMat._31 + vPos.w *  MultiMat._41;
	vOut.y = vPos.x *  MultiMat._12 + vPos.y *  MultiMat._22 + vPos.z *  MultiMat._32 + vPos.w *  MultiMat._42;
	vOut.z = vPos.x *  MultiMat._13 + vPos.y *  MultiMat._23 + vPos.z *  MultiMat._33 + vPos.w *  MultiMat._43;
	vOut.w = vPos.x *  MultiMat._14 + vPos.y *  MultiMat._24 + vPos.z *  MultiMat._34 + vPos.w *  MultiMat._44;


	XMFLOAT3 vScreenPos;
	vScreenPos.x = vOut.x / vOut.w;
	vScreenPos.y = vOut.y / vOut.w;


	return vScreenPos;

}