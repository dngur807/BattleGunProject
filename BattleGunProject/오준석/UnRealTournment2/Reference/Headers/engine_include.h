#ifndef Engine_Include_h__
#define Engine_Include_h__

#include <fstream>
#include <map>
#include <unordered_map>
#include <list>
#include <vector>
#include <string>
#include <tuple>
#include <cmath>
#include <algorithm>
using namespace std;


#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>

#include "d3dx11effect.h"
#include "DirectXTex.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
using namespace DirectX;


//#include <assimp.hpp>
//#include <aiScene.h>
//#include <aiPostProcess.h>
#include <aiscene.h>
#include <Importer.hpp>
#include <postprocess.h>
#include <ai_assert.h>

#include "MathHelper.h"
#include <LightHelper.h>

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"

#pragma warning(disable : 4251)

#include <crtdbg.h>

//#ifdef _DEBUG
//#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
//#endif

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

#endif // !Engine_Include_h__
