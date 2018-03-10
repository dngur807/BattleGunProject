// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <cmath>

#include <d3dcommon.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "d3dx11effect.h"
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include "DirectXTex.h"

using namespace DirectX;


//#include <assimp.hpp>
//#include <aiScene.h>
//#include <aiPostProcess.h>
//#include <aiscene.h>
#include <Importer.hpp>
#include <postprocess.h>

// TODO: reference additional headers your program requires here
#include "LightHelper.h"
#include "MathHelper.h"