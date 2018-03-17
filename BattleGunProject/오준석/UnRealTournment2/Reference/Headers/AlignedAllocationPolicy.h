#pragma once

//warning C4316 ó����
template<size_t T>
class CAlignedAllocationPolicy
{
public:
#ifdef NDEBUG
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}
	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
#endif // NDEBUG
};