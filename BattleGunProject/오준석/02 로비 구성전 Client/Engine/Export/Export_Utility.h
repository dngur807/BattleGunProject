/*!
 * \file Export_Utility.h
 * \date 2015/04/03 1:44
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
//#include "LightMgr.h"
#include "Frustum.h"
#include "QuadTree.h"
#include "NaviMgr.h"
#include "ClsnMgr.h"
#include "InfoSubject.h"

BEGIN(Engine)

inline CManagement* Get_Management(void);
//inline CLightMgr* Get_LightMgr(void);
inline CFrustum* Get_Frustum(void);
inline CQuadTree* Get_QuadTree(void);
inline CNaviMgr*	Get_NaviMgr(void);
inline CClsnMgr*	Get_ClsnMgr(void);
inline CInfoSubject*	Get_InfoSubject(void);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__