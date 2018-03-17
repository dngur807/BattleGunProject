#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimeMgr.h"
#include "Input.h"
#include "Effects.h"

BEGIN(Engine)

inline CGraphicDev* Get_GraphicDev(void);
inline CTimeMgr*	Get_TimeMgr(void);
inline CInput*		Get_Input(void);
inline CEffects*	Get_Effects(void);

#include "Export_System.inl"

END

#endif // !Export_System_h__
