/**
* @file dynamic_effect_object.h
* 这个文件的主要功能是：
* 
* @author 李汕
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 李汕2010.11.16
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
	//主控制器记录，不进行release
	ActorAnimationControl* m_main_ani_control;
protected:
	std::list<DynamicEffectObject*> sub_object_list;
	std::list<DynamicEffectObject*>::const_iterator sub_object_iter;
public:
	//取default property clip名称
	static const char* getDefaultPropertyName();
	//取stage property clip名称
	static const char* getStagePropertyName();
	//全局系统对象，用于clip与全局操作间的状态传递
	//static DynamicEffectObject* s_system_object;
	bool m_object_updated;

	DynamicEffectObject(ActorManager* actor_manager, const char* name, const char* defined_type);
    virtual ~DynamicEffectObject();
	//取预定义类型
	const char* getDefinedType();
	//根据预定义信息建立clip
	bool createAnimationClipByItem(const char* clip_name,const DefinedItem* defined_item);
	//获取ActorAnimationControl
	virtual ActorAnimationControl* getAnimationControl(){return m_main_ani_control;};
	
	//添加级联物体
	bool addSubObject(DynamicEffectObject* sub_obj);
	//移除级联物体
	bool removeSubObject(DynamicEffectObject* sub_obj);
	//重置级联物体迭代器
	void resetSubObjectIterator();
	//取级联物体
	DynamicEffectObject* nextSubObject();

	//获取子actorid,可以
	Actor* find_actor_recur(const char* subactor_id);

	//重新加载
	void reload();

	virtual bool remove_entity(CMediaEndEntity* entity);
};

//相机物体定义
class CameraEffectObject: public DynamicEffectObject
{
public:
	typedef CCameraEntity::PostprocessParamCollection PostParamCollection;

	typedef CCameraEntity::PostprocessCollection PostProcessCollection;

public:
	CameraEffectObject(ActorManager* actor_manager, const char* name, const char* defined_type);
	virtual ~CameraEffectObject();

	//获取当前相机信息
	virtual void getCameraInfo(CameraInfo& info);

	//获取当前相机后处理信息
	virtual const PostProcessCollection& getCameraPostParam() ;


	/**
	* @brief
	*		设置相机后处理的信息
	*/
	virtual void disableCameraPostprocess(const char* postprocess_name);

	/**
	* @brief
	*		设置后处理参数是否可用
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
	//内部用于缺省modifier处理用的变量
	StageAnimationCuttingData* m_last_cutting_data;
	//内部用于缺省modifier处理用的变量
	std::set<PropertyModifierType*> m_last_set_type_temp;
	
public:
	std::map<PropertyModifierType, IPropertyModifier*, ModifierTypeFunctorCurveInsensitive> m_default_modifier_map;//包括default 及 stage的
	std::map<std::pair<std::string,std::string>,std::set<IPropertyModifier*> > m_reset_modifier_map;//CLIP CLIP-RESET-MODIFIER; 加入 前-后 clip对支持，以支持clip的分支播放 lishan 111201

	///< 用来标示切换脚本的优化信息, 优化的前后cutting指针对， 优化的reset_modifier信息
	/*typedef std::map<std::pair<StageAnimationCuttingData*, StageAnimationCuttingData*>, std::set<IPropertyModifier*> > OptInfoMap;
	OptInfoMap m_opt_info_map;*/
public:
	DynEffObjControl(const char* name);
	virtual ~DynEffObjControl();

	//取main control名称
	static const char* getMainControlName();

	//建立初始运行状态
	void initPlayData();
	//清除运行状态
	void clearPlayData();

	//进行复位属性计算
	void calcResetModifier(const char* cutting_clipId,const char* nextClipId);
	//复位属性计算结束
	void endCalcReset();

//	/*
//	*@breif 进行优化属性的计算
//	*@param first_cutting_data 第一脚本的最后一个cutting
//	*@param next_cutting_data 第二脚本的最后一个cutting
//	*@return 
//	*/
//	void calcOptRestModifier(StageAnimationCuttingData* first_cutting_data
//							 , StageAnimationCuttingData* next_cutting_data);
//
//	/*
//	*@breif 清除切换脚本时的数据
//	*/
//	void clearPlayDataForScriptSwitch();
//
//private:
//	/*
//	*@breif 重构函数，执行rest_modifier的 插入操作, 即插入cutting切换时的优化信息
//	*@param 
//	*@return 
//	*/
//	void _insertResModifier( std::set<PropertyModifierType*>::const_iterator last_set_type_iter
//							, std::set<IPropertyModifier*>& ref_set );
//
//	/*
//	*@breif 重构函数，2011 10 21 mozisi 执行迭代查询所有set_type，ruguo  与next_clip中没有相同的，进行保存到ref_set中
//	*@param is_side_by_side 用来判断是否是相邻cutting， 外部直接传入结果
//	*@return 
//	*/
//	void _iterAndCalcRestModifier(bool is_side_by_side, std::set<PropertyModifierType*> &set_type
//							, ActorAnimationClip* next_clip, std::set<IPropertyModifier*>& ref_set );
//
//	/*
//	*@breif 重构函数，给一个clip，获取它的所有cuting的type
//	*@param 
//	*@return 
//	*/
//	void _getClipTypeSet(std::set<PropertyModifierType*>& type_set, ActorAnimationClip* first_clip );
};

/**
* @brief
*		追光灯
* @description
*		追光灯是一种灯光始终打在一个目标上的灯，追光灯在追光的过程中位置不变，只是通过调整光照方向实现灯光照在目标上
*/
class FollowLightDynamicEffectObject : public DynamicEffectObject
{
public:
	FollowLightDynamicEffectObject(ActorManager* actor_manager, const char* name, const char* defined_type);
	~FollowLightDynamicEffectObject();

	/**
	* @brief
	*		更新,更新时间为毫秒
	* @brief
	*		更新时间
	*/
	virtual void update(float t);

	/**
	* @brief
	*		设置追光灯追光类型
	*/
	virtual void set_follow_type(const char* follow_type);

	/**
	* @brief
	*		设置target的信息
	*/
	virtual void set_target_info(const char* info_name, const char* info_value);

	/**
	* @brief
	*		设置目标
	*/
	virtual void set_target(Actor* actor);
private:
	void update_location(float t);
	CPrePassLightEntity* get_light();

	ITargetPosition* create_target_position_calculator(const char* calculator_type);
private:
	ITargetPosition*		m_target_position; ///< 追光灯对应的目标位置计算器
	std::string						m_cur_follow_type; ///< 设置追光类型也就是目标计算器的类型
};

class CSingleTargetPostion : public BaseTargetPosition
{
public:
	CSingleTargetPostion(ActorManager* actor_mng);

	/**
	* @brief
	*		获取目标点的位置
	*/
	virtual H3DVec3 get_target_position();

	/**
	* @brief
	*		设置目标计算器相关的信息
	*/
	virtual void set_info(const char* info_name, const char* info_value);

	/**
	* @brief
	*		设置目标
	*/
	virtual void set_target(Actor* actor);


private:
	C3DEntity*		m_target;
	ActorManager* m_actor_mng;

	bool		m_follow_actor_bone;
};

//////////////////////////////////////////////////////////////////////////
//追光灯的属性和属性序列
//追光类型属性和属性序列Modifier生成器
class FollowTypePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const{return MEDIA_PROPERTY_TYPE_FOLLOW_LIGHT_TYPE;};
private:
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//追光类型属性和属性序列Modifier生成器
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
