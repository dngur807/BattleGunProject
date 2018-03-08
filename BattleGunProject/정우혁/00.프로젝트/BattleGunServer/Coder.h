#pragma once
#include "Constant.h"

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
	inline void GetChar(char* cpData)
	{
		*cpData = *m_pEnd;
		m_pEnd++;
	}
	inline void GetText(char* cpData, int iSize)
	{
		CopyMemory(cpData, m_pEnd, iSize);
		m_pEnd += iSize;
	}
	inline void PutChar(char cData)
	{
		*m_pEnd = cData;
		m_pEnd++;
	}
private:
	char*			m_pBegin;
	char*			m_pEnd;
};