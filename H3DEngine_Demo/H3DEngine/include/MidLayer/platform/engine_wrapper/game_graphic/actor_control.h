/**
* @file Actor_control.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.14
*/

#ifndef _ACTOR_CONTROL_H_
#define _ACTOR_CONTROL_H_

#include "game_graphic_header.h"

class ActorAnimationClip;
class IActorControl;
class IPropertyModifier;
class IPropertyCurvModifierStreamable;
class PropertyModifierFactory;

/**
* @brief
*		Actor的通用动画控制器
* @function
*		1、更新动画
*		2、播放动画
*		3、管理动画：添加、删除、查找、遍历动画
* @comment
*		1、动画名称和ActorAnimationClip是一对多的关系
*/
class ActorAnimationControl : public IActorControl
{
public:
	ActorAnimationControl(const char* name);
	virtual ~ActorAnimationControl();

	// 设置需要修改的Actor
	virtual void set_actor(Actor* actor);
	//获取需要修改的Actor
	virtual Actor* get_actor();

	// 更新
	virtual void update(float t);
	// 播放或停止动画控制器
	virtual void play(bool isplay, int anitime, ActorAnimationClip* aniclip);

	// 播放或停止动画控制器
	virtual void play(bool isplay, int anitime, const char* ani_name);


	// 添加一个动画片段
	virtual bool add_animation_clip(const char* ani_name, ActorAnimationClip* aniclip);
	// 删除一个动画片段
	virtual bool remove_animation_clip(const char* ani_name);
	//移除一个动画片段
	virtual void rename_animation_clip(const char* old_name, const char* new_name);
	// 查找指定ID的动画片段
	virtual ActorAnimationClip* find_animation_clip(const char* ani_name);
	//清空所有动画片段
	virtual void clear();

	// 重置动画片段迭代器
	void reset_iterator();
	// 获得动画片段，迭代器指向下一个动画片段，如迭代器无效返回0
	ActorAnimationClip* next();
	// 迭代获得动画片段的名称，迭代器指向下一个动画片段，如迭代器无效返回0
	const char* next_clip_name();

	//获取名称
	virtual const char* get_name() const;
	//设置名称
	virtual void set_name(const char* name);

	//是否负责actor的update
	virtual bool in_charge_of_actor_update() { return false; };
protected:
	Actor*	m_actor; //需要修改的Actor

	typedef std::map<std::string, ActorAnimationClip*> IDToActorAnimationClipMap;
	IDToActorAnimationClipMap m_ani_clip_collection; /// 所记录的动画片段容器
	IDToActorAnimationClipMap::iterator m_cur_ani_clip_iterator; /// 当前动画片段的迭代器

	typedef std::set<ActorAnimationClip*> ActorAnimationClipCollection; 
	ActorAnimationClipCollection m_inplay_animation_clip_collection; /// 正在播放的动画片段

	std::string			m_name; ///< control的名称
};

/// pawn的属性修改函数的类型ID
typedef std::string ActorPropertyType;
/// pawn的类型ID
typedef std::string ActorID;
/// 修改器调用的固定参数
typedef std::string ModifierFixParaInfo;
/// 子pawn的ID
typedef std::string SubEntityID;

/// Actor的属性修改函数的类型ID
struct PropertyModifierType
{
	ActorID child_actor_id; //AcotrId，格式为"/Actor0Id/Actor1Id/Actor2Id"
	SubEntityID child_entity_id; //Actor下的EntityId 格式为"/EntityId" 如果为空，表明关联的是Actor的属性
	ActorPropertyType property_name; //属性名称
	ModifierFixParaInfo property_fixed_param;//修改属性，需要的其他参数的值,格式为"param1/param2/param3",可以为空
	bool is_curv;
};

/**
* @brief
*		记录一个动画片段所需的属性修改器
* @function
*		1、属性修改器的管理：添加、删除、查找、遍历
*		2、播放时设置执行属性修改器
*		3、更新属性修改器
* @comment
*		1、属性修改器和属性是一一对应的
*/
class ActorAnimationClip
{
public:
	ActorAnimationClip();
	virtual ~ActorAnimationClip();

	// 添加属性修改器
	bool add_property_modifier(const PropertyModifierType& modifier_typeid, IPropertyModifier* modifier);
	// 移除属性修改器
	IPropertyModifier* remove_property_modifier(const PropertyModifierType& modifier_typeid);

	// 清空属性修改器
	/*
		modifyed by wangheng 2012.0716
		增加factory参数，用于ActorAnimationControl对
		其所包含的所有PropertyModifier进行释放操作
	*/
	void clear(PropertyModifierFactory* factory);
	// 查找指定类型ID的属性修改器
	IPropertyModifier* find_property_modifier(const PropertyModifierType& modifier_typeid);

	// 重置属性修改器迭代器
	void reset_iterator();
	// 获得属性修改器，迭代器指向下一个属性修改器，如迭代器无效返回0
	IPropertyModifier* next();
	IPropertyModifier* next(PropertyModifierType& type);
	IPropertyModifier* next(PropertyModifierType** type);

	// 播放或停止动画
	void play(bool isplay, int anitime);
	// 更新
	void update(float t);

	//把对应Entity的modifer删除
	void remove_property_modifier_by_entity(SubEntityID entity_name);

protected:
	typedef std::map<PropertyModifierType, IPropertyModifier*> PropertyModifierCollection;
	PropertyModifierCollection m_property_modifier_collection;
	//添加下面两个变量的原因是为了在更新的时候减少更新时间。在play的时候减少执行时间
	PropertyModifierCollection m_property_collection; //只包含属性的修改器
	PropertyModifierCollection m_curv_collection; //只包含属性序列修改器
	PropertyModifierCollection::iterator m_cur_property_modifier_itr;

	float m_update_time;
};

//////////////////////////////////////////////////////////////////////////
/**
* @brief
*		EyeBlinkControl是人或者宠物等进行随机眨眼的动作控制器
*/
class EyeBlinkControl : public IActorControl
{
public:
	EyeBlinkControl(const char* name);
	/**
	@brief 更新函数 
	@param 更新时间 毫秒
	*/
	virtual void update(float interval);
	/**
	@brief 设置控制目标
	@param 控制目标actor
	*/
	virtual void set_actor(Actor* actor) { m_actor = actor; }
	/**
	@brief 获取需要修改的Actor
	*/
	virtual Actor* get_actor() { return m_actor; }
	/**
	@brief 获取名称
	*/
	virtual const char* get_name() const { return m_name.c_str(); }
	/**
	@brief 设置名称
	*/
	virtual void set_name(const char* name) { m_name = name; }

	//是否负责actor的update
	virtual bool in_charge_of_actor_update() { return false; };

	virtual void set_blink_action_name(const char* action_name) { m_blink_action_name = action_name;};
	virtual const char* get_blink_action_name() const { return m_blink_action_name.c_str();}

protected:
	/// 眨眼
	void blink(bool blinking);
	/// 计算下次眨眼的时间间隔
	void calculateInterval();
	/// 进行随机眨眼动作
	void doRandBlink();

	/// 获取眨眼的动作长度
	virtual float getActionLength() = 0;

	/// 获取当前眨眼动作的时间
	virtual float getCurActionTime() = 0;

	/// 做眨眼动作
	virtual void doBlinkAction(bool blinking) = 0;

	/// 两次眨眼的最大时间间隔
	virtual float getMaxInterval() = 0;
	/// 两次眨眼最小的时间间隔
	virtual float getMinInterval() = 0;

private:
	float		m_acc_time; /// 距离上次眨眼的时间
	float		m_interval_time; /// 两次眨眼的时间间隔
	bool		m_is_blinking; /// 是否在眨眼

	std::string m_name;///< 名称
	Actor*		m_actor; ///< 所属actor

	std::string m_blink_action_name; /// 眨眼动作名称
};

//////////////////////////////////////////////////////////////////////////
class SkeletonModelEyeBlinkActionControl : public EyeBlinkControl
{
public:
	SkeletonModelEyeBlinkActionControl(Actor* actor, CSkeletonModelEntity* skeleton, const char* name);

	/**
	@brief 设置控制目标
	@param 控制目标actor
	*/
	virtual void set_actor(Actor* actor);

	/// 设置entity
	virtual void set_skeleton_model_entity(CSkeletonModelEntity* entity);

protected:
	/// 获取眨眼的动作长度
	virtual float getActionLength();

	/// 获取当前眨眼动作的时间
	virtual float getCurActionTime();

	/// 做眨眼动作
	virtual void doBlinkAction(bool blinking);

	/// 两次眨眼的最大时间间隔
	virtual float getMaxInterval();
	/// 两次眨眼最小的时间间隔
	virtual float getMinInterval();

	virtual const char* get_blink_action_name() const;

private:
	CSkeletonModelEntity*		m_skeleton; /// 所属的宠物实体
	float		m_min_interval;  /// 眨眼的最小时间间隔 
	float		m_max_interval; /// 眨眼的最大时间间隔
	std::string m_pet_blink_action_name; /// 宠物眨眼动作名称
};

//////////////////////////////////////////////////////////////////////////
class AvatarEyeBlinkControl : public EyeBlinkControl
{
public:
	AvatarEyeBlinkControl(Actor* actor, CSkeletonModelEntity* entity, const char* name);

	/**
	@brief 设置控制目标
	@param 控制目标actor
	*/
	virtual void set_actor(Actor* actor);

protected:
	/// 获取眨眼的动作长度
	virtual float getActionLength();

	/// 获取当前眨眼动作的时间
	virtual float getCurActionTime();

	/// 做眨眼动作
	virtual void doBlinkAction(bool blinking);

	/// 两次眨眼的最大时间间隔
	virtual float getMaxInterval();
	/// 两次眨眼最小的时间间隔
	virtual float getMinInterval();

private:
	CSkeletonModelEntity*	m_avatar; /// 所属的Avatar实体
	float		m_min_interval;  /// 眨眼的最小时间间隔 
	float		m_max_interval; /// 眨眼的最大时间间隔
};

class TeachAvatarEyeBlinkControl : public AvatarEyeBlinkControl
{
public:
	TeachAvatarEyeBlinkControl(Actor* actor, CSkeletonModelEntity* entity, const char* name);
	virtual ~TeachAvatarEyeBlinkControl();

	virtual void setAvatarState(bool ava_state);

	virtual void update(float interval);

private:
	bool				m_bNeedBlink;			
};
#endif //_ACTOR_CONTROL_H_
