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
		CopyMemory(m_pBegin, &iSize, sizeof(short));
		CopyMemory(m_pBegin + sizeof(short), &sType, sizeof(short));
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
	inline void PutText(char* cpData, int iSize)
	{
		CopyMemory(m_pEnd, cpData, iSize);
		m_pEnd += iSize;
	}
	inline void PutText(TCHAR* cpData, int iSize)
	{
		CopyMemory(m_pEnd, cpData, iSize);
		m_pEnd += iSize;
	}
private:
	char*			m_pBegin;
	char*			m_pEnd;
};