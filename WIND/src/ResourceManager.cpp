#include "ResourceManager.h"
#include "StringHash.h"

ResourcePtr ResourceManager::CreateOrRetrieve(const std::string &name)
{
	ResourcePtr resPtr = GetResourceByName(name);

	if (resPtr != NULL) {
		return resPtr;
	}
	else {
		return CreateResource(name);
	}
}


ResourcePtr ResourceManager::CreateResource(const std::string &name)
{
	ResourcePtr resPtr = CreateImpl(name);

	if (resPtr != NULL) {
		WIND::HANDLETYPE handle = BKDRHash(name.c_str ());
		m_nameToHandle[name] = handle;
		m_resourcesByName[name] = resPtr;
		m_resourcesByHandle[handle] = resPtr;
	}

	return resPtr;
}

ResourcePtr ResourceManager::Load(const std::string &name)
{
	ResourcePtr resPtr = CreateOrRetrieve(name);

	resPtr ->Load();

	return resPtr;
}


void ResourceManager::Unload(const std::string &name)
{
	ResourcePtr resPtr = GetResourceByName(name);

	if (resPtr != NULL) {
		resPtr ->Unload();
	}
}


void ResourceManager::Unload(WIND::HANDLETYPE handle)
{
	ResourcePtr resPtr = GetResourceByHandle(handle);

	if (resPtr != NULL) {
		resPtr ->Unload();
	}
}


void ResourceManager::UnloadAll()
{
	for (auto resPtr = m_resourcesByHandle.begin(); resPtr != m_resourcesByHandle.end(); ++resPtr) {
		resPtr ->second ->Unload ();
	}
}


void ResourceManager::ReloadAll ()
{
	for (auto resPtr = m_resourcesByName.begin(); resPtr != m_resourcesByName.end(); ++resPtr) {
		resPtr ->second ->Reload ();
	}
}


ResourcePtr ResourceManager::GetResourceByName(const std::string &name)
{
	if (m_resourcesByName.count(name) > 0) {
		return m_resourcesByName[name];
	}
	else {
		return NULL;
	}
}


ResourcePtr ResourceManager::GetResourceByHandle(WIND::HANDLETYPE handle)
{
	if (m_resourcesByHandle.count(handle) > 0) {
		return m_resourcesByHandle[handle];
	}
	else {
		return NULL;
	}
}


WIND::HANDLETYPE ResourceManager::GetHandleByName(const std::string &name)
{
	if (HandleExists(name)) {
		return m_nameToHandle[name];
	}
	else {
		return WIND::HANDLE_INVALID;
	}
}


std::string ResourceManager::GetNameByHandle(WIND::HANDLETYPE handle)
{
	for (auto it = m_nameToHandle.begin(); it != m_nameToHandle.end(); ++it) {
		if (it ->second == handle) {
			return it ->first;
		}
	}

	return std::string ();
}


bool ResourceManager::ResourceExists(WIND::HANDLETYPE handle)
{
	return m_resourcesByHandle.count(handle) > 0;
}


bool ResourceManager::ResourceExists(const std::string &name)
{
	return m_resourcesByName.count(name) > 0;
}


bool ResourceManager::HandleExists(const std::string &name)
{
	return m_nameToHandle.count(name) > 0;
}


void ResourceManager::Remove(const std::string &name)
{
	WIND::HANDLETYPE handle = WIND::HANDLE_INVALID;
	handle = GetHandleByName(name);

	if (handle != WIND::HANDLE_INVALID) {
		if (ResourceExists(handle)) {
			m_resourcesByHandle.erase (handle);
		}
		m_nameToHandle.erase(name);
	}

	if (ResourceExists(name)) {
		m_resourcesByName.erase(name);
	}
}


void ResourceManager::Remove(WIND::HANDLETYPE handle)
{
	std::string name;
	name = GetNameByHandle(handle);
	if ( !name.empty() ) {
		if (ResourceExists(name)) {
			m_resourcesByName.erase(name);
		}
		m_nameToHandle.erase(name);
	}

	if (ResourceExists(handle)) {
		m_resourcesByHandle.erase(handle);
	}
}


void ResourceManager::RemoveAll()
{
	m_nameToHandle.clear();
	m_resourcesByHandle.clear();
	m_resourcesByName.clear();
}