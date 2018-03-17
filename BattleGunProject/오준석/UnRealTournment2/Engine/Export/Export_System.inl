#include "Export_System.h"

Engine::CGraphicDev* Get_GraphicDev(void)
{
	return Engine::CGraphicDev::GetInstance();
}

Engine::CTimeMgr * Get_TimeMgr(void)
{
	return Engine::CTimeMgr::GetInstance();
}

inline Engine::CInput * Get_Input(void)
{
	return Engine::CInput::GetInstance();
}

inline Engine::CEffects * Get_Effects(void)
{
	return Engine::CEffects::GetInstance();
}
