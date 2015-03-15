#pragma once

#include "Poco/Mutex.h"
#include <set>
#include <limits>

// ID分配器（类似于内存分配器）
template <typename T>
class IDAllocater
{
	typedef T ValueType;
public:
	IDAllocater (): m_auto (1) {}
	IDAllocater (ValueType initAutoValue): m_auto (initAutoValue) {}
	~IDAllocater () {}

public:
	// 分配
	ValueType Alloc ()
	{
		Poco::Mutex::ScopedLock guard (m_lock);
		ValueType id = 0;
		if (!m_sets.empty ()) {
			id = *(m_sets.begin ());
			m_sets.erase (id);
		}
		else {
			if ( m_auto + 1 >= (std::numeric_limits<ValueType>::max) ()) { // 如此调用是放置与max宏冲突
				m_auto = 1;
			}
			id = m_auto++;
		}
		return id;
	}

	// 回收
	void Recycle (ValueType id)
	{
		Poco::Mutex::ScopedLock guard (m_lock);
		m_sets.insert (id);
	}

	// 获取目前还处于活跃的id数量
	ValueType GetActiveIDCount ()
	{
		return m_auto - 1 - m_sets.size ();
	}
	
private:
	std::set<ValueType> m_sets;
	ValueType m_auto; // 初始化为1
	Poco::Mutex		m_lock;
};