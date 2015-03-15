#pragma once

#include "Poco/Mutex.h"
#include <set>
#include <limits>

// ID���������������ڴ��������
template <typename T>
class IDAllocater
{
	typedef T ValueType;
public:
	IDAllocater (): m_auto (1) {}
	IDAllocater (ValueType initAutoValue): m_auto (initAutoValue) {}
	~IDAllocater () {}

public:
	// ����
	ValueType Alloc ()
	{
		Poco::Mutex::ScopedLock guard (m_lock);
		ValueType id = 0;
		if (!m_sets.empty ()) {
			id = *(m_sets.begin ());
			m_sets.erase (id);
		}
		else {
			if ( m_auto + 1 >= (std::numeric_limits<ValueType>::max) ()) { // ��˵����Ƿ�����max���ͻ
				m_auto = 1;
			}
			id = m_auto++;
		}
		return id;
	}

	// ����
	void Recycle (ValueType id)
	{
		Poco::Mutex::ScopedLock guard (m_lock);
		m_sets.insert (id);
	}

	// ��ȡĿǰ�����ڻ�Ծ��id����
	ValueType GetActiveIDCount ()
	{
		return m_auto - 1 - m_sets.size ();
	}
	
private:
	std::set<ValueType> m_sets;
	ValueType m_auto; // ��ʼ��Ϊ1
	Poco::Mutex		m_lock;
};