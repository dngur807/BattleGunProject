#ifndef		Engine_Functor_h__
#define		Engine_Functor_h__

namespace Engine
{
	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}

	public:
		template <typename T>
		void operator () (T& pInstance)
		{
			if (nullptr != pInstance)
			{
				delete pInstance;
				pInstance = nullptr;
			}
		}
	};

	// 연관 컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
		
	public:
		template <typename T>
		void operator () (T& Pair)
		{
			if (nullptr != Pair.second)
			{
				delete Pair.second;
				Pair.second = nullptr;
			}
		}
	};

	class CTagFinder
	{
	public:
		explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CTagFinder(void) {}
	public:
		template <typename T> bool operator () (T& Pair)
		{
			int iResult = lstrcmp(m_pTag, Pair.first);

			if (0 == iResult)
				return true;
			return false;
		}
	private:
		const TCHAR*				m_pTag;
	};
}

#endif // ! Engine_Functor_h__
