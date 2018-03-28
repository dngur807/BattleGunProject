#pragma once


#define HEADERSIZE			4

struct XMFLOAT3
{
	float x;
	float y;
	float z;
};
typedef struct tagNavMesh
{
	int			iOption;
	XMFLOAT3 vecNavMesh[3];
}NAVMESH;
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
	inline void GetInt(int* data)
	{
		CopyMemory(data, pEnd, sizeof(int));
		pEnd += sizeof(int);
	}
	inline void GetNavi(NAVMESH* data)
	{
		CopyMemory(data, pEnd, sizeof(NAVMESH));
		pEnd += sizeof(NAVMESH);
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
	inline void PutInt(int iData)
	{
		CopyMemory(pEnd, &iData, sizeof(int));
		pEnd += sizeof(int);
	}
	inline void PutFloat(float data)
	{
		CopyMemory(pEnd, &data, sizeof(float));
		pEnd += sizeof(float);
	}
	inline void PutNaviMesh(NAVMESH data)
	{
		CopyMemory(pEnd, &data, sizeof(NAVMESH));
		pEnd += sizeof(NAVMESH);
	}
private:
	char*			pBegin;
	char*			pEnd;
};