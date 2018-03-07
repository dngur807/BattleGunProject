#pragma once
#include "Define.h"

class CCoder
{
public:
	inline void SetBuf(char* cpBuf)
	{
		m_pBegin = cpBuf;
		m_pEnd = cpBuf + HEADERSIZE;
	}
	inline int SetHeader(short sType)
	{
		short iSize;
		iSize = m_pEnd - m_pBegin - HEADERSIZE;
		return iSize + HEADERSIZE;
	}
private:
	char*			m_pBegin;
	char*			m_pEnd;
};