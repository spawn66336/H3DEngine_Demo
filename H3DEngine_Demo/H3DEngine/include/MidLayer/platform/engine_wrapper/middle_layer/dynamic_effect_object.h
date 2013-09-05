/**
* @file dynamic_effect_object.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author ����
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2010.11.16
*/

#ifndef _DYNAMIC_EFFECT_OBJECT_H_
#define _DYNAMIC_EFFECT_OBJECT_H_

#include "middle_layer_header.h"

class IPropertyModifier;
struct DefinedItem;
struct StageAnimationCuttingData;

class DynamicEffectObject : public Actor
{
private:
	std::string m_defined_type;
	//����������¼��������release
	ActorAnimationControl* m_main_ani_control;
protected:
	std::list<DynamicEffectObject*> sub_object_list;
	std::list<DynamicEffectObject*>::const_iterator sub_object_iter;
public:
	//ȡdefault property clip����
	static const char* getDefaultPropertyName();
	//ȡstage property clip����
	static const char* getStagePropertyName();
	//ȫ��ϵͳ��������clip��ȫ�ֲ������״̬����
	//static DynamicEffectObject* s_system_object;
	bool m_object_updated;

	DynamicEffectObject(ActorManager* actor_manager, const char* name, const char* defined_type);
    virtual ~DynamicEffectObject();
	//ȡԤ��������
	const char* getDefinedType();
	//����Ԥ������Ϣ����clip
	bool createAnimationClipByItem(const char* clip_name,const DefinedItem* defined_item);
	//��ȡActorAnimationControl
	virtual ActorAnimationControl* getAnimationControl(){return m_main_ani_control;};
	
	//��Ӽ�������
	bool addSubObject(DynamicEffectObject* sub_obj);
	//�Ƴ���������
	bool removeSubObject(DynamicEffectObject* sub_obj);
	//���ü������������
	void resetSubObjectIterator();
	//ȡ��������
	DynamicEffectObject* nextSubObject();

	//��ȡ��actorid,����
	Actor* find_actor_recur(const char* subactor_id);

	//���¼���
	void reload();

	virtual bool remove_entity(CMediaEndEntity* entity);
};

//������嶨��
class CameraEffectObject: public DynamicEffectObject
{
public:
	typedef CCameraEntity::PostprocessParamCollection PostParamCollection;

	typedef CCameraEntity::PostprocessCollection PostProcessCollection;

public:
	CameraEffectObject(ActorManager* actor_manager, const char* name, const char* defined_type);
	virtual ~CameraEffectObject();

	//��ȡ��ǰ�����Ϣ
	virtual void getCameraInfo(CameraInfo& info);

	//��ȡ��ǰ���������Ϣ
	virtual const PostProcessCollection& getCameraPostParam() ;


	/**
	* @brief
	*		��������������Ϣ
	*/
	virtual void disableCameraPostprocess(const char* postprocess_name);

	/**
	* @brief
	*		���ú�������Ƿ����
	*/
	void setCameraPostProcessEnable(const char* post_process_name, bool enable);

	virtual CCameraEntity* getCamera();
};

class ModifierTypeFunctorCurveInsensitive
{
public:
	bool operator()(const PropertyModifierType& a, const PropertyModifierType& b) const
	{
		int cr = a.property_name.compare(b.property_name);
		if(cr > 0)
			return true;
		else if(cr < 0)
			return false;

		cr = a.child_entity_id.compare(b.child_entity_id);
		if(cr > 0)
			return true;
		else if(cr < 0)
			return false;

		cr = a.child_actor_id.compare(b.child_actor_id);
		if(cr > 0)
			return true;
		else if(cr < 0)
			return false;

		cr = a.property_fixed_param.compare(b.property_fixed_param);
		if(cr > 0)
			return true;
		else if(cr < 0)
			return false;

		cr = a.child_actor_id.compare(b.child_actor_id);
		if(cr > 0)
			return true;
		else if(cr < 0)
			return false;
		else
			return false;
	}
};
class DynEffObjControl : public ActorAnimationControl
{
private:
	//�ڲ�����ȱʡmodifier�����õı���
	StageAnimationCuttingData* m_last_cutting_data;
	//�ڲ�����ȱʡmodifier�����õı���
	std::set<PropertyModifierType*> m_last_set_type_temp;
	
public:
	std::map<PropertyModifierType, IPropertyModifier*, ModifierTypeFunctorCurveInsensitive> m_default_modifier_map;//����default �� stage��
	std::map<std::pair<std::string,std::string>,std::set<IPropertyModifier*> > m_reset_modifier_map;//CLIP CLIP-RESET-MODIFIER; ���� ǰ-�� clip��֧�֣���֧��clip�ķ�֧���� lishan 111201

	///< ������ʾ�л��ű����Ż���Ϣ, �Ż���ǰ��cuttingָ��ԣ� �Ż���reset_modifier��Ϣ
	/*typedef std::map<std::pair<StageAnimationCuttingData*, StageAnimationCuttingData*>, std::set<IPropertyModifier*> > OptInfoMap;
	OptInfoMap m_opt_info_map;*/
public:
	DynEffObjControl(const char* name);
	virtual ~DynEffObjControl();

	//ȡmain control����
	static const char* getMainControlName();

	//������ʼ����״̬
	void initPlayData();
	//�������״̬
	void clearPlayData();

	//���и�λ���Լ���
	void calcResetModifier(const char* cutting_clipId,const char* nextClipId);
	//��λ���Լ������
	void endCalcReset();

//	/*
//	*@breif �����Ż����Եļ���
//	*@param first_cutting_data ��һ�ű������һ��cutting
//	*@param next_cutting_data �ڶ��ű������һ��cutting
//	*@return 
//	*/
//	void calcOptRestModifier(StageAnimationCuttingData* first_cutting_data
//							 , StageAnimationCuttingData* next_cutting_data);
//
//	/*
//	*@breif ����л��ű�ʱ������
//	*/
//	void clearPlayDataForScriptSwitch();
//
//private:
//	/*
//	*@breif �ع�������ִ��rest_modifier�� �������, ������cutting�л�ʱ���Ż���Ϣ
//	*@param 
//	*@return 
//	*/
//	void _insertResModifier( std::set<PropertyModifierType*>::const_iterator last_set_type_iter
//							, std::set<IPropertyModifier*>& ref_set );
//
//	/*
//	*@breif �ع�������2011 10 21 mozisi ִ�е�����ѯ����set_type��ruguo  ��next_clip��û����ͬ�ģ����б��浽ref_set��
//	*@param is_side_by_side �����ж��Ƿ�������cutting�� �ⲿֱ�Ӵ�����
//	*@return 
//	*/
//	void _iterAndCalcRestModifier(bool is_side_by_side, std::set<PropertyModifierType*> &set_type
//							, ActorAnimationClip* next_clip, std::set<IPropertyModifier*>& ref_set );
//
//	/*
//	*@breif �ع���������һ��clip����ȡ��������cuting��type
//	*@param 
//	*@return 
//	*/
//	void _getClipTypeSet(std::set<PropertyModifierType*>& type_set, ActorAnimationClip* first_clip );
};

/**
* @brief
*		׷���
* @description
*		׷�����һ�ֵƹ�ʼ�մ���һ��Ŀ���ϵĵƣ�׷�����׷��Ĺ�����λ�ò��䣬ֻ��ͨ���������շ���ʵ�ֵƹ�����Ŀ����
*/
class FollowLightDynamicEffectObject : public DynamicEffectObject
{
public:
	FollowLightDynamicEffectObject(ActorManager* actor_manager, const char* name, const char* defined_type);
	~FollowLightDynamicEffectObject();

	/**
	* @brief
	*		����,����ʱ��Ϊ����
	* @brief
	*		����ʱ��
	*/
	virtual void update(float t);

	/**
	* @brief
	*		����׷���׷������
	*/
	virtual void set_follow_type(const char* follow_type);

	/**
	* @brief
	*		����target����Ϣ
	*/
	virtual void set_target_info(const char* info_name, const char* info_value);

	/**
	* @brief
	*		����Ŀ��
	*/
	virtual void set_target(Actor* actor);
private:
	void update_location(float t);
	CPrePassLightEntity* get_light();

	ITargetPosition* create_target_position_calculator(const char* calculator_type);
private:
	ITargetPosition*		m_target_position; ///< ׷��ƶ�Ӧ��Ŀ��λ�ü�����
	std::string						m_cur_follow_type; ///< ����׷������Ҳ����Ŀ�������������
};

class CSingleTargetPostion : public BaseTargetPosition
{
public:
	CSingleTargetPostion(ActorManager* actor_mng);

	/**
	* @brief
	*		��ȡĿ����λ��
	*/
	virtual H3DVec3 get_target_position();

	/**
	* @brief
	*		����Ŀ���������ص���Ϣ
	*/
	virtual void set_info(const char* info_name, const char* info_value);

	/**
	* @brief
	*		����Ŀ��
	*/
	virtual void set_target(Actor* actor);


private:
	C3DEntity*		m_target;
	ActorManager* m_actor_mng;

	bool		m_follow_actor_bone;
};

//////////////////////////////////////////////////////////////////////////
//׷��Ƶ����Ժ���������
//׷���������Ժ���������Modifier������
class FollowTypePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const{return MEDIA_PROPERTY_TYPE_FOLLOW_LIGHT_TYPE;};
private:
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//׷���������Ժ���������Modifier������
class FollowLightTargetInfoPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const{return MEDIA_PROPERTY_TYPE_FOLLOW_LIGHT_TARGET_INFO;};
private:
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};


class DynEffPropertyModifierFactory : public PropertyModifierFactory
{
public:
	DynEffPropertyModifierFactory();
};

class DynEffectObjectManager : public ActorManager
{
public:
	virtual PropertyModifierFactory* create_modifier_factory();
};
#endif //_DYNAMIC_EFFECT_OBJECT_H_
