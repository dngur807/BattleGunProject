#pragma once

const UINT g_iCntX = 129;
const UINT g_iCntZ = 129;
const float g_fWidth = 256.f;
const float g_fDepth = 256.f;

const UINT		VTXCNTX = 129;
const UINT		VTXCNTZ = 129;
const UINT		VTXITV = 1;

const XMFLOAT3 g_vLook(0.f, 0.f, 1.f);
const XMVECTOR g_xvLook = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const XMVECTOR g_xvUp =	 XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

extern UINT g_iCameraNum;

const unsigned int g_iClientCX = 800;
const unsigned int g_iClientCY = 600;