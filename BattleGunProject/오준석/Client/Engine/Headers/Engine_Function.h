#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T>
	void Safe_Delete(T& pointer)
	{
		if (nullptr != pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pointer)
	{
		if (nullptr != pointer)
		{
			delete[] pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Release(T& pointer)
	{
		if (nullptr != pointer)
		{
			pointer->Release();
			pointer = nullptr;
		}
	}

	template <typename TC>
	unsigned long SafeRelease(TC& pPointer)
	{
		unsigned long dwRefCnt = 0;

		if (nullptr != pPointer)
		{
			dwRefCnt = pPointer->Release();
			if (0 == dwRefCnt)
				pPointer = NULL;
		}

		return dwRefCnt;
	}

	template <typename T>
	void Safe_Single_Destroy(T& pointer)
	{
		if (nullptr != pointer)
		{
			pointer->DestroyInstance();
			pointer = nullptr;
		}
	}
}

#endif