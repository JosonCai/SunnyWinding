#include "Resource.h"

// 资源句柄到资源指针
// 资源名到资源指针
// ---------------
// 创建或检索资源
// 卸载一个资源
// 卸载全部资源
// 重新加载一个资源
// 通过名字获取资源
// 通过句柄获取资源
// 获取资源类型
// 检查资源是否在被使用
typedef boost::shared_ptr<Resource> ResourcePtr;
class ResourceManager
{
public:
	ResourceManager ();
	~ResourceManager ();

public:
	ResourcePtr CreateResource (const std::string &name);
	ResourcePtr CreateOrRetrieve (const std::string &name);
	ResourcePtr Load (const std::string &name);
	void Unload (const std::string &name);
	void Unload (WIND::HANDLETYPE handle);
	void UnloadAll ();
	void ReloadAll ();
	ResourcePtr GetResourceByName (const std::string &name);
	ResourcePtr GetResourceByHandle (WIND::HANDLETYPE handle);
	WIND::HANDLETYPE GetHandleByName (const std::string &name);
	std::string GetNameByHandle (WIND::HANDLETYPE handle);
	void Remove ( WIND::HANDLETYPE handle);
	void Remove (const std::string &name);
	void RemoveAll ();
	bool ResourceExists (const std::string &name);
	bool ResourceExists (WIND::HANDLETYPE handle);
	bool HandleExists (const std::string &name);

protected:
	virtual ResourcePtr CreateImpl (const std::string &name) = 0;

private:
	std::map<std::string, WIND::HANDLETYPE> m_nameToHandle;
	std::map<std::string, ResourcePtr> m_resourcesByName;
	std::map<WIND::HANDLETYPE, ResourcePtr> m_resourcesByHandle;
};
