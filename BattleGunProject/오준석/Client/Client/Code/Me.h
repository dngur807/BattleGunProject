#pragma once
#include "User.h"

class CMe
	: public CUser
{
public:
	CMe();
	~CMe();
public:
	virtual void Update();
	void Control_Character();
private:
	float			m_fTime;
	
};