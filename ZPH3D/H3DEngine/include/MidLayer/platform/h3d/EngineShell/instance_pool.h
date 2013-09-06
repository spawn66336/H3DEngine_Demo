#ifndef INSTANCE_POOL
#define INSTANCE_POOL

#include <map>
#include <string>
#include <list>
#include <algorithm>

namespace H3DI
{
	class IObject;
};

class ObjectPool
{
	std::map<std::string, std::list<H3DI::IObject*> > m_free_pool;
	std::map<std::string, std::list<H3DI::IObject*> > m_pool;
	std::map<H3DI::IObject*, std::string> m_instance_to_resname;

public:
	static std::map<H3DI::IObject*, std::string> s_instance_to_instancename;
	static std::map<std::string, H3DI::IObject*> s_instancename_to_instance;


	// ��Ҫ������ʱ�򣬵���
	virtual H3DI::IObject* createObj(const char* resname, const char* instancename = 0);
	// �ⲿ��ȷ����Ҫ���ٵ�ʱ�򣬵���releaseobj
	virtual void releaseObj(H3DI::IObject* obj);
	// 
	void dump();
	//
	void clearMat()
	{
		for(std::map<std::string, std::list<H3DI::IObject*> >::iterator pos = m_free_pool.begin();
			pos != m_free_pool.end(); ++pos)
		{
			for(std::list<H3DI::IObject*>::iterator subpos  = pos->second.begin();
				subpos != pos->second.end(); ++subpos)
			{
				H3DI::IObject* obj = *subpos;
				realDestoryObj(obj);
			}
		}
		m_free_pool.clear();
	}
	


protected:
	//
	void recordObj(H3DI::IObject* obj, const char* resname, const char* instancename = 0);
	//
	void removeObj(H3DI::IObject* obj);
	// ��ѯ��ʱ����ã������Ƿ���ָ����ʵ��
	H3DI::IObject* getObjByInstanceName(const char* instancename);
	// �ӳ��н��в���
	H3DI::IObject* getFreeObj(const char* resname);
	// �Żص����ɳ�
	void putToFreePool(H3DI::IObject* obj);
	//
	virtual H3DI::IObject* realCreateObj(const char* resname) = 0;
	virtual void realDestoryObj(H3DI::IObject* obj) = 0;
};

// Ҫ�����������ظ���instancename��ʵ����
class UnifyInstanceNameObjectPool : public ObjectPool
{
public:
	// ��Ҫ������ʱ�򣬵���
	virtual H3DI::IObject* createObj(const char* resname, const char* instancename = 0);
	// �ⲿ��ȷ����Ҫ���ٵ�ʱ�򣬵���releaseobj
	virtual void releaseObj(H3DI::IObject* obj);
};

struct EnginePool
{
	static void init();
	static void destory();
	static void dump();
	static void clearMaterailLib();

	static ObjectPool* m_effect_pool;
	static ObjectPool* m_shader_pool;
	static ObjectPool* m_texture_pool;
	static ObjectPool* m_rendertarget_pool;
	static ObjectPool* m_level_pool;
	static ObjectPool* m_light_pool;
	static ObjectPool* m_camera_pool;
	static ObjectPool* m_model_pool;
	
	static ObjectPool* m_avaskeleton_pool;
	static ObjectPool* m_skeleton_pool;
	static ObjectPool* m_actor_pool;
	
	static std::list<std::pair<std::string, ObjectPool*> > m_all_pools;
};

#endif