#pragma once
#include "User.h"

class CMe
	: public CUser
{
public:
	CMe();
	~CMe();
public:
	void Update();
private:
	void KeyCheck();
	
	
};