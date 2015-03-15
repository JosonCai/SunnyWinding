#include "Resource.h"

// ��Դ�������Դָ��
// ��Դ������Դָ��
// ---------------
// �����������Դ
// ж��һ����Դ
// ж��ȫ����Դ
// ���¼���һ����Դ
// ͨ�����ֻ�ȡ��Դ
// ͨ�������ȡ��Դ
// ��ȡ��Դ����
// �����Դ�Ƿ��ڱ�ʹ��
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
