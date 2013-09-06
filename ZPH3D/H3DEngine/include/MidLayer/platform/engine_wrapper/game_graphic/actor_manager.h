/**
* @file actor_manager.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.16
*/

#ifndef _ACTOR_MANAGER_H_
#define _ACTOR_MANAGER_H_

#include "game_graphic_header.h"

class Actor;
class CGraphicsResMng;
class PropertyModifierFactory;
/**
* @brief
*		actor的管理器和entity的生成器
* @func
*		1、管理Actor，添加、移除、查找、遍历
*		2、负责创建entity的创建和销毁
*/
class ActorManager
{
public:
	typedef std::list<Actor*> ActorList;
	typedef std::map<CPrePassLightEntity*, bool> PrepassLightCollection;
public:
	ActorManager();
	virtual ~ActorManager();
	
	//初始化
	virtual bool initialize(H3DI::IRender* irender, H3DI::ILevel* ilevel);

	virtual CGraphicsResMng* get_graphic_res_mng();

	// 添加子Actor
	virtual bool add_actor(Actor* actor);
	// 移除子Actor（不负责销毁）
	virtual bool remove_actor(Actor* actor);
	// 查找指定名称的子封装实体
	virtual Actor* find_actor(const char* actor_name);
	
	// 重置Actor迭代器
	virtual void reset_iterator();
	// 获得Actor，迭代器指向下一个Actor，如迭代器无效返回0
	virtual Actor* next();
	
	//创建Entity
	virtual CMediaEndEntity* create_entity(const char* entitytypeid, const char* id);
	//销毁Entity
	virtual void release_entity(CMediaEndEntity* entity);

	//level_entity相关
	virtual CLevelEntity* create_level_entity(H3DI::ILevel* ilevel = NULL);
	virtual void release_level_entity();
	virtual CLevelEntity* get_level_entity();

	//获取属性修改器的创建工厂
	virtual PropertyModifierFactory* getPropertyModifierFactory();

	/**
	* @brief
	*		画灯光的线
	*/
	virtual void drawLightLine(CPrePassLightEntity* light);

	/**
	* @brief
	*		添加使用的灯光
	*/
	virtual void add_using_light(CPrePassLightEntity* light);

	/**
	* @brief
	*		获取所有灯光
	*/
	virtual const PrepassLightCollection& get_light_collection();

	/**
	* @brief
	*		清空所有正在使用的灯光
	*/
	virtual void reset_light_status();

    /**
	* @brief
	*		清空所有的actor
	*/
    virtual void clear_actor();

	//设置资源文件相关路径
	virtual void set_default_resource_path(const char* default_eff_res
											, const char* default_model_res
											, const char* default_actor_res
											, const char* default_skeleton_model_res);

	void add_to_level(Actor* actor);
	void remove_from_level(Actor* actor);

	//通过entityid获取顶层的ActorName 
	const char* getTopActorNameById(unsigned int id);

	//获得系统物体的标示名称
	static const char* getLodSystemActorName(LOD_LEVEL lod);
	static LOD_LEVEL	isLodSystemActorName(const char* strName);

	//获得某个名称的系统物体
	Actor* getSystemActor(const char* strName);

	//从系统物体列表中添加或者移除物体
	bool addSystemActor(const char* strName, Actor* sysActor);
	bool removeSystemActor(const char* strName);

	//从场景中删除某个级别的系统物体
	void remove_sysactor_from_actorlist(int lod);
	//添加某个级别的lod物体至场景中
	void add_sysactor_to_actorlist(int lod);

	//迭代获取某个LOD级别中的所有系统物体的名称
	void reset_some_sysactor_iter(int lod);
	const char* next_some_sysactor_name(int lod);
	Actor*		next_some_sysactor(int lod);
private:
	void doPrepasslightInitialize(CMediaEndEntity* light);
	virtual PropertyModifierFactory* create_modifier_factory();

	typedef std::map<int, std::map<std::string, Actor*> >	LodSystemActors;
	typedef LodSystemActors::iterator LodSystemActorsItr;
	LodSystemActors m_system_actor;
	typedef std::map<std::string, Actor*> SomeLodSystemActors;
	typedef SomeLodSystemActors::iterator SomeLodSystemActorsItr;
	SomeLodSystemActorsItr m_some_lod_system_actor_itr;
	SomeLodSystemActorsItr m_lod_system_actor_itr;
    
private:
	ActorList	m_actor_collectioon; ///< 管理的Actor
	ActorList::iterator m_iterator; ///< 遍历用的迭代器
	
	/*
	概念上，levelentity应该是每个stage一个，与actormanager平级，actormanager下每个actor所以包含的引擎对象都需要attach进
	levelentity所包含的引擎对象里。目前暂时先把levelentity的生命期的管理放在Actormanager里。以后再做调整。
	*/
	CLevelEntity*	m_level;		//这个ActorManager所对用的Level;

	CGraphicsResMng* m_graphic_res_mng;
	PropertyModifierFactory*		m_modifier_factory; ///< 属性Modifier的创建工厂

	PrepassLightCollection	m_created_light_collection; ///< 当前创建的灯光的创建工厂

	std::string				m_def_effect_res_path; ///< 特效资源所在文件夹路径
	std::string				m_def_model_res_path; ///< 模型资源所在文件夹路径
	std::string				m_def_actor_res_path; ///< 舞蹈者资源所在文件夹路径
	std::string				m_def_skeleton_model_res_path; ///< NPC资源所在文件夹路径
	std::string				m_def_sound_res_path; ///< 音效资源所在文件夹路径

	///< 人形的默认性别
	bool					m_is_male;

	///< 宠物的默认性别
	bool					m_pet_is_male;

public:

	/*
	*@breif 设置人形默认性别
	*@param sex 性别
	*/
	void setAvatarSex(bool sex) {m_is_male = sex;}

	/*
	*@breif 设置宠物默认性别
	*@param sex 性别
	*/
	void setPetSex(bool sex) {m_pet_is_male = sex;}
};

#endif //_ACTOR_MANAGER_H_
