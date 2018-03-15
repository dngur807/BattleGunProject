#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimeMgr.h"
#include "Input.h"
#include "Effects.h"
#include "FontMgr.h"
BEGIN(Engine)

inline CGraphicDev* Get_GraphicDev(void);
inline CTimeMgr*	Get_TimeMgr(void);
inline CInput*		Get_Input(void);
inline CEffects*	Get_Effects(void);
inline CFontMgr*	Get_FontMgr();
#include "Export_System.inl"

END

#endif // !Export_System_h__
