#pragma once
#define HEADERSIZE			4


class CCoder
{
public:
	inline void SetBuf(char* pBuf)
	{
		pBegin = pBuf;
		pEnd = pBuf + HEADERSIZE;
	}
	inline int SetHeader(short sType)
	{
		short			iSize;
		iSize = pEnd - pBegin - HEADERSIZE;
		CopyMemory(pBegin, &iSize, sizeof(short));
		CopyMemory(pBegin + sizeof(short), &sType, sizeof(short));
		return iSize + HEADERSIZE;
	}
	inline void GetChar(char* cpData)
	{
		*cpData = *pEnd;
		pEnd++;
	}
	inline void GetText(char *cpData, int iSize)
	{
		CopyMemory(cpData, pEnd, iSize);
		pEnd += iSize;
	}

	inline void GetHeader(short* sBodySize, short* sType)
	{
		CopyMemory(sBodySize, pBegin, sizeof(short));
		CopyMemory(sType, pBegin + sizeof(short), sizeof(short));
	}

	inline void PutChar(char Data)
	{
		*pEnd = Data;
		pEnd++;
	}
	inline void PutText(char *cpData, int iSize)
	{
		CopyMemory(pEnd, cpData, iSize);
		pEnd += iSize;
	}

private:
	char*			pBegin;
	char*			pEnd;
};