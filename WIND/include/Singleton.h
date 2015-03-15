#pragma once

#include "Assert.h"

template <typename T>
class Singleton
{
public:
	Singleton ();
	~Singleton();

public:
	static T*	GetSingletonPtr ();
	static T&	GetSingleton ();

	static void NewInstance ();
	static void DeleteInstance ();

protected:
	static T*	m_singleton;
};

template<typename T> T* Singleton<T>::m_singleton = NULL;



template <typename T>
inline Singleton<T>::Singleton()
{
	assert( !ms_Singleton );
#if defined( _MSC_VER ) && _MSC_VER < 1200
	int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
	ms_Singleton = (T*)((int)this + offset);
#else
	ms_Singleton = static_cast< T* >( this );
#endif
}



template <typename T>
inline Singleton<T>::~Singleton ()
{
	Assert (m_singleton, "Singleton error in ~Singleton.");
	m_singleton = NULL;
}



template <typename T>
inline T& Singleton<T>::GetSingleton()
{
	if (m_singleton == NULL) {
		m_singleton = new T();
		Assert (m_singleton, "Singleton error in GetSingleton. Can't alloc memory.");
	}
	return (*m_singleton);
}



template <typename T>
inline T* Singleton<T>::GetSingletonPtr()
{
	if (m_singleton == NULL) {
		m_singleton = new T();
		Assert (m_singleton, "Singleton error in GetSingletonPtr. Can't alloc memory.");
	}
	return m_singleton;
}



template <typename T>
inline void Singleton<T>::NewInstance()
{
	Assert (m_singleton == NULL, "Singleton error in NewInstance, m_singleton is not NULL.");
	m_singleton = new T();
}



template <typename T>
inline void Singleton<T>::DeleteInstance()
{
	Assert (m_singleton, "Singleton error in DeleteInstance, m_singleton is NULL");
	delete m_singleton;
	m_singleton = NULL;
}
