/**
* @file actor_manager.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.11.16
*/

#ifndef _ACTOR_MANAGER_H_
#define _ACTOR_MANAGER_H_

#include "game_graphic_header.h"

class Actor;
class CGraphicsResMng;
class PropertyModifierFactory;
/**
* @brief
*		actor�Ĺ�������entity��������
* @func
*		1������Actor����ӡ��Ƴ������ҡ�����
*		2�����𴴽�entity�Ĵ���������
*/
class ActorManager
{
public:
	typedef std::list<Actor*> ActorList;
	typedef std::map<CPrePassLightEntity*, bool> PrepassLightCollection;
public:
	ActorManager();
	virtual ~ActorManager();
	
	//��ʼ��
	virtual bool initialize(H3DI::IRender* irender, H3DI::ILevel* ilevel);

	virtual CGraphicsResMng* get_graphic_res_mng();

	// �����Actor
	virtual bool add_actor(Actor* actor);
	// �Ƴ���Actor�����������٣�
	virtual bool remove_actor(Actor* actor);
	// ����ָ�����Ƶ��ӷ�װʵ��
	virtual Actor* find_actor(const char* actor_name);
	
	// ����Actor������
	virtual void reset_iterator();
	// ���Actor��������ָ����һ��Actor�����������Ч����0
	virtual Actor* next();
	
	//����Entity
	virtual CMediaEndEntity* create_entity(const char* entitytypeid, const char* id);
	//����Entity
	virtual void release_entity(CMediaEndEntity* entity);

	//level_entity���
	virtual CLevelEntity* create_level_entity(H3DI::ILevel* ilevel = NULL);
	virtual void release_level_entity();
	virtual CLevelEntity* get_level_entity();

	//��ȡ�����޸����Ĵ�������
	virtual PropertyModifierFactory* getPropertyModifierFactory();

	/**
	* @brief
	*		���ƹ����
	*/
	virtual void drawLightLine(CPrePassLightEntity* light);

	/**
	* @brief
	*		���ʹ�õĵƹ�
	*/
	virtual void add_using_light(CPrePassLightEntity* light);

	/**
	* @brief
	*		��ȡ���еƹ�
	*/
	virtual const PrepassLightCollection& get_light_collection();

	/**
	* @brief
	*		�����������ʹ�õĵƹ�
	*/
	virtual void reset_light_status();

    /**
	* @brief
	*		������е�actor
	*/
    virtual void clear_actor();

	//������Դ�ļ����·��
	virtual void set_default_resource_path(const char* default_eff_res
											, const char* default_model_res
											, const char* default_actor_res
											, const char* default_skeleton_model_res);

	void add_to_level(Actor* actor);
	void remove_from_level(Actor* actor);

	//ͨ��entityid��ȡ�����ActorName 
	const char* getTopActorNameById(unsigned int id);

	//���ϵͳ����ı�ʾ����
	static const char* getLodSystemActorName(LOD_LEVEL lod);
	static LOD_LEVEL	isLodSystemActorName(const char* strName);

	//���ĳ�����Ƶ�ϵͳ����
	Actor* getSystemActor(const char* strName);

	//��ϵͳ�����б�����ӻ����Ƴ�����
	bool addSystemActor(const char* strName, Actor* sysActor);
	bool removeSystemActor(const char* strName);

	//�ӳ�����ɾ��ĳ�������ϵͳ����
	void remove_sysactor_from_actorlist(int lod);
	//���ĳ�������lod������������
	void add_sysactor_to_actorlist(int lod);

	//������ȡĳ��LOD�����е�����ϵͳ���������
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
	ActorList	m_actor_collectioon; ///< �����Actor
	ActorList::iterator m_iterator; ///< �����õĵ�����
	
	/*
	�����ϣ�levelentityӦ����ÿ��stageһ������actormanagerƽ����actormanager��ÿ��actor���԰��������������Ҫattach��
	levelentity����������������Ŀǰ��ʱ�Ȱ�levelentity�������ڵĹ������Actormanager��Ժ�����������
	*/
	CLevelEntity*	m_level;		//���ActorManager�����õ�Level;

	CGraphicsResMng* m_graphic_res_mng;
	PropertyModifierFactory*		m_modifier_factory; ///< ����Modifier�Ĵ�������

	PrepassLightCollection	m_created_light_collection; ///< ��ǰ�����ĵƹ�Ĵ�������

	std::string				m_def_effect_res_path; ///< ��Ч��Դ�����ļ���·��
	std::string				m_def_model_res_path; ///< ģ����Դ�����ļ���·��
	std::string				m_def_actor_res_path; ///< �赸����Դ�����ļ���·��
	std::string				m_def_skeleton_model_res_path; ///< NPC��Դ�����ļ���·��
	std::string				m_def_sound_res_path; ///< ��Ч��Դ�����ļ���·��

	///< ���ε�Ĭ���Ա�
	bool					m_is_male;

	///< �����Ĭ���Ա�
	bool					m_pet_is_male;

public:

	/*
	*@breif ��������Ĭ���Ա�
	*@param sex �Ա�
	*/
	void setAvatarSex(bool sex) {m_is_male = sex;}

	/*
	*@breif ���ó���Ĭ���Ա�
	*@param sex �Ա�
	*/
	void setPetSex(bool sex) {m_pet_is_male = sex;}
};

#endif //_ACTOR_MANAGER_H_
