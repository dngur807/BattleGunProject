
CManagement* Get_Management(void)
{
	return CManagement::GetInstance();
}
//
//CLightMgr* Get_LightMgr(void)
//{
//	return CLightMgr::GetInstance();
//}
//
CFrustum* Get_Frustum(void)
{
	return CFrustum::GetInstance();
}

CQuadTree* Get_QuadTree(void)
{
	return CQuadTree::GetInstance();
}

CNaviMgr* Get_NaviMgr(void)
{
	return CNaviMgr::GetInstance();
}

CInfoSubject* Get_InfoSubject(void)
{
	return CInfoSubject::GetInstance();
}

CClsnMgr* Get_ClsnMgr(void)
{
	return CClsnMgr::GetInstance();
}
