#include <boost/shared_ptr.hpp>  
#include <cstring>
#include <map>
#include "DataType.h"

class ResourceManager;
// 资源加载状态
class Resource{
public:
	enum LoadingState
	{
		LOADSTATE_UNLOADED,
		LOADSTATE_LOADING,
		LOADSTATE_LOADED,
		LOADSTATE_UNLOADED,
		LOADSTATE_UNLOADING
	};
	Resource (ResourceManager *manager, const std::string &name, WIND::HANDLETYPE handle, const std::string &groupName) :	m_manager(manager), m_name (name), m_handle (handle), m_groupName (groupName), m_size (0), m_loadState(LOADSTATE_UNLOADED) {}
	virtual ~Resource () {}
	
public:
	std::string GetResourceName () { return m_name; }
	std::string GetGroupName () { return m_groupName; }
	WIND::HANDLETYPE GetHandle () { return m_handle; }
	virtual size_t CalculateSize ();
	bool IsLoading () { return GetLoadingState () == LOADSTATE_LOADING; }
	bool IsLoaded () { return GetLoadingState () == LOADSTATE_LOADED; }
	LoadingState GetLoadingState () { return m_loadState; }
	void SetLoadingState (LoadingState loadingState) { m_loadState = loadingState; }
	ResourceManager* GetManager () { return m_manager; }

	virtual void Load () = 0;
	virtual void Reload () = 0;
	virtual void Unload () = 0;
	
protected:
	std::string m_name;
	std::string m_groupName;
	WIND::HANDLETYPE m_handle;
	size_t m_size;
	LoadingState m_loadState;
	ResourceManager *m_manager;
};

size_t Resource::CalculateSize ()
{
	size_t memSize = 0;
	memSize += sizeof (ResourceManager*);
	memSize += sizeof (WIND::HANDLETYPE);
	memSize += m_name.size () * sizeof (char);
	memSize += m_groupName.size () * sizeof (char);
	memSize += sizeof (size_t);
	memSize += sizeof (LoadingState);
	
	m_size = memSize;
	
	return m_size;
}

