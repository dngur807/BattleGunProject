#pragma once




const DWORD		MIN_STR = 64;
const DWORD     MID_STR = 128;
const DWORD		MAX_STR = 256;



const UINT g_iCntX = 129;
const UINT g_iCntZ = 129;
const float g_fWidth = 256.f;
const float g_fDepth = 256.f;

const UINT		VTXCNTX = 129;
const UINT		VTXCNTZ = 129;
const UINT		VTXITV = 1;

const XMFLOAT3 g_vLook(0.f, 0.f, -1.f);
const XMVECTOR g_xvLook = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const XMVECTOR g_xvUp =	 XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


#define KEY_UP	 0x01
#define KEY_DOWN   0x02
#define KEY_LEFT  0x04
#define KEY_RIGHT  0x08
#define KEY_SPACE  0x10
#define MOUSE_LEFT  0x20
#define MOUSE_RIGHT  0x40
#define WEAPON_CHANGE  0x80



// 무기 데미지
const int      ASSULTBULLET_DMG = 10;

const int	   g_ScaleRatio = 8;
