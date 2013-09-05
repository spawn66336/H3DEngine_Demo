/**
* @file Actor_control.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.11.14
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
*		Actor��ͨ�ö���������
* @function
*		1�����¶���
*		2�����Ŷ���
*		3������������ӡ�ɾ�������ҡ���������
* @comment
*		1���������ƺ�ActorAnimationClip��һ�Զ�Ĺ�ϵ
*/
class ActorAnimationControl : public IActorControl
{
public:
	ActorAnimationControl(const char* name);
	virtual ~ActorAnimationControl();

	// ������Ҫ�޸ĵ�Actor
	virtual void set_actor(Actor* actor);
	//��ȡ��Ҫ�޸ĵ�Actor
	virtual Actor* get_actor();

	// ����
	virtual void update(float t);
	// ���Ż�ֹͣ����������
	virtual void play(bool isplay, int anitime, ActorAnimationClip* aniclip);

	// ���Ż�ֹͣ����������
	virtual void play(bool isplay, int anitime, const char* ani_name);


	// ���һ������Ƭ��
	virtual bool add_animation_clip(const char* ani_name, ActorAnimationClip* aniclip);
	// ɾ��һ������Ƭ��
	virtual bool remove_animation_clip(const char* ani_name);
	//�Ƴ�һ������Ƭ��
	virtual void rename_animation_clip(const char* old_name, const char* new_name);
	// ����ָ��ID�Ķ���Ƭ��
	virtual ActorAnimationClip* find_animation_clip(const char* ani_name);
	//������ж���Ƭ��
	virtual void clear();

	// ���ö���Ƭ�ε�����
	void reset_iterator();
	// ��ö���Ƭ�Σ�������ָ����һ������Ƭ�Σ����������Ч����0
	ActorAnimationClip* next();
	// ������ö���Ƭ�ε����ƣ�������ָ����һ������Ƭ�Σ����������Ч����0
	const char* next_clip_name();

	//��ȡ����
	virtual const char* get_name() const;
	//��������
	virtual void set_name(const char* name);

	//�Ƿ���actor��update
	virtual bool in_charge_of_actor_update() { return false; };
protected:
	Actor*	m_actor; //��Ҫ�޸ĵ�Actor

	typedef std::map<std::string, ActorAnimationClip*> IDToActorAnimationClipMap;
	IDToActorAnimationClipMap m_ani_clip_collection; /// ����¼�Ķ���Ƭ������
	IDToActorAnimationClipMap::iterator m_cur_ani_clip_iterator; /// ��ǰ����Ƭ�εĵ�����

	typedef std::set<ActorAnimationClip*> ActorAnimationClipCollection; 
	ActorAnimationClipCollection m_inplay_animation_clip_collection; /// ���ڲ��ŵĶ���Ƭ��

	std::string			m_name; ///< control������
};

/// pawn�������޸ĺ���������ID
typedef std::string ActorPropertyType;
/// pawn������ID
typedef std::string ActorID;
/// �޸������õĹ̶�����
typedef std::string ModifierFixParaInfo;
/// ��pawn��ID
typedef std::string SubEntityID;

/// Actor�������޸ĺ���������ID
struct PropertyModifierType
{
	ActorID child_actor_id; //AcotrId����ʽΪ"/Actor0Id/Actor1Id/Actor2Id"
	SubEntityID child_entity_id; //Actor�µ�EntityId ��ʽΪ"/EntityId" ���Ϊ�գ�������������Actor������
	ActorPropertyType property_name; //��������
	ModifierFixParaInfo property_fixed_param;//�޸����ԣ���Ҫ������������ֵ,��ʽΪ"param1/param2/param3",����Ϊ��
	bool is_curv;
};

/**
* @brief
*		��¼һ������Ƭ������������޸���
* @function
*		1�������޸����Ĺ�����ӡ�ɾ�������ҡ�����
*		2������ʱ����ִ�������޸���
*		3�����������޸���
* @comment
*		1�������޸�����������һһ��Ӧ��
*/
class ActorAnimationClip
{
public:
	ActorAnimationClip();
	virtual ~ActorAnimationClip();

	// ��������޸���
	bool add_property_modifier(const PropertyModifierType& modifier_typeid, IPropertyModifier* modifier);
	// �Ƴ������޸���
	IPropertyModifier* remove_property_modifier(const PropertyModifierType& modifier_typeid);

	// ��������޸���
	/*
		modifyed by wangheng 2012.0716
		����factory����������ActorAnimationControl��
		��������������PropertyModifier�����ͷŲ���
	*/
	void clear(PropertyModifierFactory* factory);
	// ����ָ������ID�������޸���
	IPropertyModifier* find_property_modifier(const PropertyModifierType& modifier_typeid);

	// ���������޸���������
	void reset_iterator();
	// ��������޸�����������ָ����һ�������޸��������������Ч����0
	IPropertyModifier* next();
	IPropertyModifier* next(PropertyModifierType& type);
	IPropertyModifier* next(PropertyModifierType** type);

	// ���Ż�ֹͣ����
	void play(bool isplay, int anitime);
	// ����
	void update(float t);

	//�Ѷ�ӦEntity��modiferɾ��
	void remove_property_modifier_by_entity(SubEntityID entity_name);

protected:
	typedef std::map<PropertyModifierType, IPropertyModifier*> PropertyModifierCollection;
	PropertyModifierCollection m_property_modifier_collection;
	//�����������������ԭ����Ϊ���ڸ��µ�ʱ����ٸ���ʱ�䡣��play��ʱ�����ִ��ʱ��
	PropertyModifierCollection m_property_collection; //ֻ�������Ե��޸���
	PropertyModifierCollection m_curv_collection; //ֻ�������������޸���
	PropertyModifierCollection::iterator m_cur_property_modifier_itr;

	float m_update_time;
};

//////////////////////////////////////////////////////////////////////////
/**
* @brief
*		EyeBlinkControl���˻��߳���Ƚ������գ�۵Ķ���������
*/
class EyeBlinkControl : public IActorControl
{
public:
	EyeBlinkControl(const char* name);
	/**
	@brief ���º��� 
	@param ����ʱ�� ����
	*/
	virtual void update(float interval);
	/**
	@brief ���ÿ���Ŀ��
	@param ����Ŀ��actor
	*/
	virtual void set_actor(Actor* actor) { m_actor = actor; }
	/**
	@brief ��ȡ��Ҫ�޸ĵ�Actor
	*/
	virtual Actor* get_actor() { return m_actor; }
	/**
	@brief ��ȡ����
	*/
	virtual const char* get_name() const { return m_name.c_str(); }
	/**
	@brief ��������
	*/
	virtual void set_name(const char* name) { m_name = name; }

	//�Ƿ���actor��update
	virtual bool in_charge_of_actor_update() { return false; };

	virtual void set_blink_action_name(const char* action_name) { m_blink_action_name = action_name;};
	virtual const char* get_blink_action_name() const { return m_blink_action_name.c_str();}

protected:
	/// գ��
	void blink(bool blinking);
	/// �����´�գ�۵�ʱ����
	void calculateInterval();
	/// �������գ�۶���
	void doRandBlink();

	/// ��ȡգ�۵Ķ�������
	virtual float getActionLength() = 0;

	/// ��ȡ��ǰգ�۶�����ʱ��
	virtual float getCurActionTime() = 0;

	/// ��գ�۶���
	virtual void doBlinkAction(bool blinking) = 0;

	/// ����գ�۵����ʱ����
	virtual float getMaxInterval() = 0;
	/// ����գ����С��ʱ����
	virtual float getMinInterval() = 0;

private:
	float		m_acc_time; /// �����ϴ�գ�۵�ʱ��
	float		m_interval_time; /// ����գ�۵�ʱ����
	bool		m_is_blinking; /// �Ƿ���գ��

	std::string m_name;///< ����
	Actor*		m_actor; ///< ����actor

	std::string m_blink_action_name; /// գ�۶�������
};

//////////////////////////////////////////////////////////////////////////
class SkeletonModelEyeBlinkActionControl : public EyeBlinkControl
{
public:
	SkeletonModelEyeBlinkActionControl(Actor* actor, CSkeletonModelEntity* skeleton, const char* name);

	/**
	@brief ���ÿ���Ŀ��
	@param ����Ŀ��actor
	*/
	virtual void set_actor(Actor* actor);

	/// ����entity
	virtual void set_skeleton_model_entity(CSkeletonModelEntity* entity);

protected:
	/// ��ȡգ�۵Ķ�������
	virtual float getActionLength();

	/// ��ȡ��ǰգ�۶�����ʱ��
	virtual float getCurActionTime();

	/// ��գ�۶���
	virtual void doBlinkAction(bool blinking);

	/// ����գ�۵����ʱ����
	virtual float getMaxInterval();
	/// ����գ����С��ʱ����
	virtual float getMinInterval();

	virtual const char* get_blink_action_name() const;

private:
	CSkeletonModelEntity*		m_skeleton; /// �����ĳ���ʵ��
	float		m_min_interval;  /// գ�۵���Сʱ���� 
	float		m_max_interval; /// գ�۵����ʱ����
	std::string m_pet_blink_action_name; /// ����գ�۶�������
};

//////////////////////////////////////////////////////////////////////////
class AvatarEyeBlinkControl : public EyeBlinkControl
{
public:
	AvatarEyeBlinkControl(Actor* actor, CSkeletonModelEntity* entity, const char* name);

	/**
	@brief ���ÿ���Ŀ��
	@param ����Ŀ��actor
	*/
	virtual void set_actor(Actor* actor);

protected:
	/// ��ȡգ�۵Ķ�������
	virtual float getActionLength();

	/// ��ȡ��ǰգ�۶�����ʱ��
	virtual float getCurActionTime();

	/// ��գ�۶���
	virtual void doBlinkAction(bool blinking);

	/// ����գ�۵����ʱ����
	virtual float getMaxInterval();
	/// ����գ����С��ʱ����
	virtual float getMinInterval();

private:
	CSkeletonModelEntity*	m_avatar; /// ������Avatarʵ��
	float		m_min_interval;  /// գ�۵���Сʱ���� 
	float		m_max_interval; /// գ�۵����ʱ����
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
