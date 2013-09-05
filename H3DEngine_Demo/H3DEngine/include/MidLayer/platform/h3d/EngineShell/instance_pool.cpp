// EngineShell.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "instance_pool.h"

#include "../../../platform/platform_shared/biboframe/biboplatform.h"
#include "../../../platform/platform_shared/biboframe/BiboAssert.h"

#include "engineshell.h"
#include "levelshell.h"
#include "modelshell.h"
#include "specialeffectshell.h"
#include "textureandrendertarget.h"



std::map<H3DI::IObject*, std::string> ObjectPool::s_instance_to_instancename;
std::map<std::string, H3DI::IObject*> ObjectPool::s_instancename_to_instance;

H3DI::IObject* ObjectPool::getObjByInstanceName(const char* instancename)
{
	alwaysAssert(instancename);

	std::map<std::string, H3DI::IObject*>::iterator pos = s_instancename_to_instance.find(instancename);
	if(pos != s_instancename_to_instance.end())
		return pos->second;
	else
		return 0;
}

void ObjectPool::dump()
{
	LogInfo("ObjectPool::dump begin");

	int freec = 0;
	int usedc = 0;

	for(std::map<std::string, std::list<H3DI::IObject*> >::iterator pos = m_free_pool.begin();
		pos != m_free_pool.end(); ++pos)
	{
		unsigned int fc = pos->second.size();
		if(fc)
		{
			freec += fc;
			LogInfo("   free:", pos->first, " with count: ", fc);
		}
	}

	for(std::map<std::string, std::list<H3DI::IObject*> >::iterator pos = m_pool.begin();
		pos != m_pool.end(); ++pos)
	{
		unsigned int uc = pos->second.size();
		if(uc)
		{
			usedc += uc;
			LogInfo("   used:", pos->first, " with count: ", uc);
		}
	}

	alwaysAssert(usedc == (int)m_instance_to_resname.size());


	LogInfo("free:", freec, " used:", usedc);

	LogInfo("ObjectPool::dump end");
}

void ObjectPool::recordObj(H3DI::IObject* obj, const char* resname, const char* instancename)
{
	if(instancename)
	{
		alwaysAssert(s_instance_to_instancename.find(obj) == s_instance_to_instancename.end());
		alwaysAssert(s_instancename_to_instance.find(instancename) == s_instancename_to_instance.end());

		s_instance_to_instancename[obj] = instancename;
		s_instancename_to_instance[instancename] = obj;
	}

	alwaysAssert(m_instance_to_resname.find(obj) == m_instance_to_resname.end());
	alwaysAssert(std::find(m_pool[resname].begin(), m_pool[resname].end(), obj) == m_pool[resname].end());
	m_pool[resname].push_back(obj);
	m_instance_to_resname[obj] = resname; 
}

void ObjectPool::removeObj(H3DI::IObject* obj)
{
	std::map<H3DI::IObject*, std::string>::iterator namepos = s_instance_to_instancename.find(obj);
	if(namepos != s_instance_to_instancename.end())
	{
		s_instancename_to_instance.erase(namepos->second);
		s_instance_to_instancename.erase(namepos);
	}

	std::map<H3DI::IObject*, std::string>::iterator intitr = m_instance_to_resname.find(obj);
	alwaysAssert(intitr != m_instance_to_resname.end());

	std::list<H3DI::IObject*>::iterator poolitr = std::find(m_pool[intitr->second].begin(), m_pool[intitr->second].end(), obj);
	alwaysAssert(poolitr != m_pool[intitr->second].end());
	m_pool[intitr->second].erase(poolitr);
	if(m_pool[intitr->second].empty())
		m_pool.erase(intitr->second);

	m_instance_to_resname.erase(intitr);
}

void ObjectPool::putToFreePool(H3DI::IObject* obj)
{
	std::map<H3DI::IObject*, std::string>::iterator intitr = m_instance_to_resname.find(obj);
	alwaysAssert(intitr != m_instance_to_resname.end());

	realDestoryObj(obj);

	// m_free_pool[intitr->second].push_back(obj);
}

H3DI::IObject* ObjectPool::getFreeObj(const char* resname)
{
	alwaysAssert(resname);
	std::map<std::string, std::list<H3DI::IObject*> >::iterator pos = m_free_pool.find(resname);
	if(pos != m_free_pool.end() && !pos->second.empty())
	{
		H3DI::IObject* obj = pos->second.front();
		pos->second.pop_front();
		return obj;
	}
	else
		return 0;
}

H3DI::IObject* ObjectPool::createObj(const char* resname, const char* instancename)
{
	// 如果空白池中无内容，则创建一个
	H3DI::IObject* obj = getFreeObj(resname);
	if(!obj)
		obj = realCreateObj(resname);

	recordObj(obj, resname, instancename);
	
	return obj;
}

void ObjectPool::releaseObj(H3DI::IObject* obj)
{
	std::map<H3DI::IObject*, std::string>::iterator namepos = s_instance_to_instancename.find(obj);
	alwaysAssert(namepos == s_instance_to_instancename.end());


	// 放入free池
	putToFreePool(obj);
	// 从已知内容中移除
	removeObj(obj);
}

H3DI::IObject* UnifyInstanceNameObjectPool::createObj(const char* resname, const char* instancename)
{
	// 如果有相同的实例名称，则访问有问题
	alwaysAssert(instancename);
	alwaysAssert(!getObjByInstanceName(instancename));

	// 如果空白池中无内容，则创建一个
	H3DI::IObject* obj = getFreeObj(resname);
	if(!obj)
		obj = realCreateObj(resname);

	recordObj(obj, resname, instancename);

	LogInfo("engine create unify:", resname, " ", instancename, " ", obj);
	
	return obj;
}

void UnifyInstanceNameObjectPool::releaseObj(H3DI::IObject* obj)
{
	std::map<H3DI::IObject*, std::string>::iterator namepos = s_instance_to_instancename.find(obj);
	alwaysAssert(namepos != s_instance_to_instancename.end());

	LogInfo("engine remove unify:", namepos->second.c_str(), " ", obj);

	// 放入free池
	putToFreePool(obj);
	// 从已知内容中移除
	removeObj(obj);
}

template<class T, class B>
class TObjectPool : public B
{
public:
	virtual H3DI::IObject* realCreateObj(const char* resname)
	{
		return new T();
	}
	virtual void realDestoryObj(H3DI::IObject* obj)
	{
		T* actor = dynamic_cast<T*>(obj);
		alwaysAssert(actor);
		delete actor;
	}
};

ObjectPool* EnginePool::m_effect_pool = new TObjectPool<H3DI::CSpecialEffect, ObjectPool>();
ObjectPool* EnginePool::m_shader_pool = new TObjectPool<H3DI::CNewShader, ObjectPool>();
ObjectPool* EnginePool::m_texture_pool = new TObjectPool<H3DI::CTexture, ObjectPool>();
ObjectPool* EnginePool::m_rendertarget_pool = new TObjectPool<H3DI::CRenderTarget, ObjectPool>();
ObjectPool* EnginePool::m_light_pool = new TObjectPool<H3DI::CPrePassLight, ObjectPool>();
ObjectPool* EnginePool::m_camera_pool = new TObjectPool<H3DI::CCamera, ObjectPool>();
ObjectPool* EnginePool::m_model_pool = new TObjectPool<H3DI::CModel, ObjectPool>();
ObjectPool* EnginePool::m_skeleton_pool = new TObjectPool<H3DI::CSkeletonModel, ObjectPool>();

ObjectPool* EnginePool::m_level_pool = new TObjectPool<H3DI::CLevel, UnifyInstanceNameObjectPool>();
ObjectPool* EnginePool::m_avaskeleton_pool = new TObjectPool<H3DI::CAvatarSkeletonModel, UnifyInstanceNameObjectPool>();
ObjectPool* EnginePool::m_actor_pool = new TObjectPool<H3DI::CActor, UnifyInstanceNameObjectPool>();

std::list<std::pair<std::string, ObjectPool*> > EnginePool::m_all_pools;

void EnginePool::init()
{
	alwaysAssert(m_all_pools.empty());

	m_all_pools.push_back(std::make_pair("m_effect_pool", m_effect_pool));
	m_all_pools.push_back(std::make_pair("m_shader_pool", m_shader_pool));
	m_all_pools.push_back(std::make_pair("m_texture_pool", m_texture_pool));
	m_all_pools.push_back(std::make_pair("m_rendertarget_pool", m_rendertarget_pool));
	m_all_pools.push_back(std::make_pair("m_light_pool", m_light_pool));
	m_all_pools.push_back(std::make_pair("m_camera_pool", m_camera_pool));
	m_all_pools.push_back(std::make_pair("m_model_pool", m_model_pool));
	
	m_all_pools.push_back(std::make_pair("m_level_pool", m_level_pool));
	m_all_pools.push_back(std::make_pair("m_avaskeleton_pool", m_avaskeleton_pool));
	m_all_pools.push_back(std::make_pair("m_skeleton_pool", m_skeleton_pool));
	m_all_pools.push_back(std::make_pair("m_actor_pool", m_actor_pool));
}

// 清空所有自由池中的内容
void EnginePool::clearMaterailLib()
{
	for(std::list<std::pair<std::string, ObjectPool*> >::iterator pos = m_all_pools.begin();
		pos != m_all_pools.end(); ++pos)
	{
		pos->second->clearMat();
	}
}

void EnginePool::dump()
{
	// dump all instance
	alwaysAssert(ObjectPool::s_instance_to_instancename.size() == ObjectPool::s_instancename_to_instance.size());
	LogInfo("instance count:", ObjectPool::s_instance_to_instancename.size());

	for(std::map<std::string, H3DI::IObject*>::iterator pos = ObjectPool::s_instancename_to_instance.begin();
		pos != ObjectPool::s_instancename_to_instance.end(); ++pos)
	{
		LogInfo("   ", pos->first, " ");
	}

	for(std::list<std::pair<std::string, ObjectPool*> >::iterator pos = m_all_pools.begin();
		pos != m_all_pools.end(); ++pos)
	{
		LogInfo("Dump:", pos->first);
		pos->second->dump();
	}
}