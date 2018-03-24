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
	inline void GetUInt(unsigned int* Data)
	{
		CopyMemory(Data, pEnd, sizeof(unsigned int));
		pEnd += sizeof(unsigned int);
	}
	inline void GetXMFLOAT3(XMFLOAT3* vec)
	{
		CopyMemory(vec, pEnd, sizeof(XMFLOAT3));
		pEnd += sizeof(XMFLOAT3);
	}
	inline void GetFloat(float* data)
	{
		CopyMemory(data, pEnd, sizeof(float));
		pEnd += sizeof(float);
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
	inline void PutXMFLOAT3(XMFLOAT3* vec)
	{
		CopyMemory(pEnd, vec, sizeof(XMFLOAT3));
		pEnd += sizeof(XMFLOAT3);
	}
	inline void PutUint(unsigned int iInput)
	{
		CopyMemory(pEnd, &iInput, sizeof(unsigned int));
		pEnd += sizeof(unsigned int);
	}
	inline void PutFloat(float data)
	{
		CopyMemory(pEnd, &data, sizeof(float));
		pEnd += sizeof(float);
	}

private:
	char*			pBegin;
	char*			pEnd;
};