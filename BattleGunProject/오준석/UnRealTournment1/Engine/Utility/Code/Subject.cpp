#include "Subject.h"
#include "Observer.h"

Engine::CSubject::CSubject()
{
}


Engine::CSubject::~CSubject()
{
	Release();
}

void Engine::CSubject::Release(void)
{
	// �� �����ʹ� �����͸� ������ ��ü�� �����ϹǷ� clear�� ����.
	m_Observerlist.clear();
}

void Engine::CSubject::Subscribe(CObserver * pObserver)
{
	NULL_CHECK_MSG(pObserver, L"pObserver was NULL");
	m_Observerlist.push_back(pObserver);
}

void Engine::CSubject::UnSubscribe(CObserver * pObserver)
{
	OBSERVERLIST::iterator iter = m_Observerlist.begin();
	OBSERVERLIST::iterator iter_end = m_Observerlist.end();

	for (; iter != iter_end; ++iter)
	{
		if ((*iter) == pObserver)
		{
			m_Observerlist.erase(iter);
			return;
		}
	}
}

void Engine::CSubject::Notify(int iMsg)
{
	OBSERVERLIST::iterator iter = m_Observerlist.begin();
	OBSERVERLIST::iterator iter_end = m_Observerlist.end();

	for (; iter != iter_end; ++iter)
	{
		if (NULL != (*iter))
			(*iter)->Update(iMsg);
	}
}
