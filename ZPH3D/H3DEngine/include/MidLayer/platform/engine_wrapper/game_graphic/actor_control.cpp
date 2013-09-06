/**
* @file Actor_control.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.14
*/

#include "game_graphic_body.h"

ActorAnimationControl::ActorAnimationControl(const char* name)
{
	assert((NULL != name) && (strlen(name) != 0));

	m_name = name;

	m_actor = 0;
	reset_iterator();
}

ActorAnimationControl::~ActorAnimationControl()
{
	PropertyModifierFactory* factory = get_actor()->get_actor_mngr()->getPropertyModifierFactory();
	assert(factory);
	for(IDToActorAnimationClipMap::iterator pos = m_ani_clip_collection.begin();
		pos != m_ani_clip_collection.end(); ++pos)
	{
		pos->second->clear(factory);
		delete pos->second;
	}
}

void ActorAnimationControl::set_actor(Actor* actor)
{
	m_actor = actor;
}

Actor* ActorAnimationControl::get_actor()
{
	return m_actor;
}

void ActorAnimationControl::update(float t)
{
	for(ActorAnimationClipCollection::iterator pos = m_inplay_animation_clip_collection.begin();
		pos != m_inplay_animation_clip_collection.end(); ++pos)
	{
		ActorAnimationClip* aniclip = *pos;
		aniclip->update(t);
	}
}

void ActorAnimationControl::play(bool isplay, int anitime, ActorAnimationClip* aniclip)
{
	aniclip->play(isplay, anitime);

	if(isplay)
		m_inplay_animation_clip_collection.insert(aniclip);
	else
		m_inplay_animation_clip_collection.erase(aniclip);
}

// 播放或停止动画控制器
void ActorAnimationControl::play(bool isplay, int anitime, const char* ani_name)
{
	assert(NULL != ani_name);
	ActorAnimationClip* clip = find_animation_clip(ani_name);
	if (NULL != clip)
	{
		play(isplay, anitime, clip);
	}

	Actor* owner = get_actor();

	if (NULL != owner)
	{
		owner->reset_actor_iter();
		Actor* child = owner->next_actor();
		while(NULL != child)
		{
			child->reset_control_iter();
			IActorControl* control = child->next_control();
			while(NULL != control)
			{
				ActorAnimationControl* ani_control = dynamic_cast<ActorAnimationControl*>(control);
				if (NULL != ani_control)
				{
					ani_control->play(isplay, anitime, ani_name);
				}

				control = child->next_control();
			}

			child = owner->next_actor();
		}
	}
}

bool ActorAnimationControl::add_animation_clip(const char* ani_name, ActorAnimationClip* aniclip)
{
	if(m_ani_clip_collection.find(ani_name) != m_ani_clip_collection.end())
		return false;

	m_ani_clip_collection[ani_name] = aniclip;
	reset_iterator();

	return true;
}

bool ActorAnimationControl::remove_animation_clip(const char* ani_name)
{
	IDToActorAnimationClipMap::iterator pos = m_ani_clip_collection.find(ani_name);
	if(pos == m_ani_clip_collection.end())
		return false;
	
	m_inplay_animation_clip_collection.erase(pos->second);
	
	PropertyModifierFactory* factory = get_actor()->get_actor_mngr()->getPropertyModifierFactory();
	assert(factory);
	pos->second->clear(factory);
	delete pos->second;

	m_ani_clip_collection.erase(pos);
	reset_iterator();

	return true;
}

void ActorAnimationControl::clear()
{
	PropertyModifierFactory* factory = get_actor()->get_actor_mngr()->getPropertyModifierFactory();
	assert(factory);

	for(IDToActorAnimationClipMap::iterator pos = m_ani_clip_collection.begin();
		pos != m_ani_clip_collection.end(); ++pos)
	{
		pos->second->clear(factory);
		delete pos->second;
	}

	m_ani_clip_collection.clear();
	m_inplay_animation_clip_collection.clear();
	reset_iterator();
}

void ActorAnimationControl::rename_animation_clip(const char* old_name, const char* new_name)
{
	IDToActorAnimationClipMap::iterator pos = m_ani_clip_collection.find(old_name);
	if(pos == m_ani_clip_collection.end())
		return ;

	ActorAnimationClip* clip = pos->second;
	m_inplay_animation_clip_collection.erase(clip);
	m_ani_clip_collection.erase(pos);
	reset_iterator();

	add_animation_clip(new_name, clip);
}

ActorAnimationClip* ActorAnimationControl::find_animation_clip(const char* ani_name)
{
	IDToActorAnimationClipMap::iterator pos = m_ani_clip_collection.find(ani_name);

	if(pos == m_ani_clip_collection.end())
		return NULL;
	else
		return pos->second;
}

void ActorAnimationControl::reset_iterator()
{
	m_cur_ani_clip_iterator = m_ani_clip_collection.begin();
}

// 获得动画片段，迭代器指向下一个动画片段，如迭代器无效返回0
ActorAnimationClip* ActorAnimationControl::next()
{
	ActorAnimationClip* aniclip = 0;

	if(m_cur_ani_clip_iterator != m_ani_clip_collection.end())
	{
		aniclip = m_cur_ani_clip_iterator->second;
		++m_cur_ani_clip_iterator;
	}

	return aniclip;
}
// 获得动画片段的名称，迭代器指向下一个动画片段名称，如迭代器无效返回0
const char* ActorAnimationControl::next_clip_name()
{
	const char* clip_name = NULL;

	if(m_cur_ani_clip_iterator != m_ani_clip_collection.end())
	{
		clip_name = m_cur_ani_clip_iterator->first.c_str();
		++m_cur_ani_clip_iterator;
	}

	return clip_name;
}

const char* ActorAnimationControl::get_name() const
{
	return m_name.c_str();
}

void ActorAnimationControl::set_name(const char* name)
{
	m_name = name;
}

//////////////////////////////////////////////////////////////////////////

/// ModifierTypeID的比较器
bool operator<(const PropertyModifierType& a, const PropertyModifierType& b)
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

	return a.is_curv > b.is_curv;

}

ActorAnimationClip::ActorAnimationClip()
{
	reset_iterator();
	m_update_time = 0.0f;
}

ActorAnimationClip::~ActorAnimationClip()
{
	for(PropertyModifierCollection::iterator pos = m_property_modifier_collection.begin();
		pos != m_property_modifier_collection.end(); ++pos)
	{
	
		//delete pos->second;
		assert(false);
	}
}

bool ActorAnimationClip::add_property_modifier(const PropertyModifierType& modifier_typeid, IPropertyModifier* modifier)
{
	if(m_property_modifier_collection.find(modifier_typeid) != m_property_modifier_collection.end())
		return false;

	m_property_modifier_collection[modifier_typeid] = modifier;
	reset_iterator();

	if (modifier_typeid.is_curv)
	{
		m_curv_collection[modifier_typeid] = modifier;
	}
	else
	{
		m_property_collection[modifier_typeid] = modifier;
	}
	return true;
}

void ActorAnimationClip::clear(PropertyModifierFactory* factory)
{
	for(PropertyModifierCollection::iterator pos = m_property_modifier_collection.begin();
		pos != m_property_modifier_collection.end(); ++pos)
	{
		//delete pos->second;
		factory->releaseModifier(pos->second);
	}

	m_property_modifier_collection.clear();
	m_property_collection.clear();
	m_curv_collection.clear();
	reset_iterator();
}

IPropertyModifier* ActorAnimationClip::remove_property_modifier(const PropertyModifierType& modifier_typeid)
{
	PropertyModifierCollection::iterator pos = m_property_modifier_collection.find(modifier_typeid);
	if(pos == m_property_modifier_collection.end())
		return NULL;

	IPropertyModifier* modifier = pos->second;
	m_property_modifier_collection.erase(modifier_typeid);
	reset_iterator();

	if (modifier_typeid.is_curv)
	{
		m_curv_collection.erase(modifier_typeid);
	}
	else
	{
		m_property_collection.erase(modifier_typeid);
	}
	return modifier;
}
void ActorAnimationClip::remove_property_modifier_by_entity(SubEntityID entity_name)
{
	PropertyModifierCollection::iterator itrFind;
	for (itrFind = m_property_modifier_collection.begin();itrFind != m_property_modifier_collection.end();)
	{
		if(itrFind->first.child_entity_id == entity_name)
		{
			m_property_modifier_collection.erase(itrFind++);	
		}
		else
		{
			itrFind++;
		}
	}
	for (itrFind = m_property_collection.begin();itrFind != m_property_collection.end();)
	{
		if(itrFind->first.child_entity_id == entity_name)
		{
			m_property_collection.erase(itrFind++);
		}
		else
		{
			itrFind++;
		}
	}
	for (itrFind = m_curv_collection.begin();itrFind != m_curv_collection.end();)
	{
		if(itrFind->first.child_entity_id == entity_name)
		{
			m_curv_collection.erase(itrFind++);
		}
		else
		{
			itrFind++;
		}
	}
	


}
IPropertyModifier* ActorAnimationClip::find_property_modifier(const PropertyModifierType& modifier_typeid)
{
	PropertyModifierCollection::iterator pos = m_property_modifier_collection.find(modifier_typeid);
	if(pos == m_property_modifier_collection.end())
		return false;
	else
		return pos->second;
}

void ActorAnimationClip::reset_iterator()
{
	m_cur_property_modifier_itr = m_property_modifier_collection.begin();
}

IPropertyModifier* ActorAnimationClip::next()
{
	IPropertyModifier* modifier = 0;
	if(m_cur_property_modifier_itr != m_property_modifier_collection.end())
	{
		modifier = m_cur_property_modifier_itr->second;
		++m_cur_property_modifier_itr;
	}

	return modifier;
}

IPropertyModifier* ActorAnimationClip::next(PropertyModifierType& type)
{
	IPropertyModifier* modifier = 0;
	if(m_cur_property_modifier_itr != m_property_modifier_collection.end())
	{
		modifier = m_cur_property_modifier_itr->second;
		type = m_cur_property_modifier_itr->first;
		++m_cur_property_modifier_itr;
	}

	return modifier;
}

IPropertyModifier* ActorAnimationClip::next(PropertyModifierType** type)
{
	IPropertyModifier* modifier = 0;
	if(m_cur_property_modifier_itr != m_property_modifier_collection.end())
	{
		modifier = m_cur_property_modifier_itr->second;
		*type = const_cast<PropertyModifierType*>(&(m_cur_property_modifier_itr->first));
		++m_cur_property_modifier_itr;
	}

	return modifier;
}

void ActorAnimationClip::play(bool isplay, int anitime)
{
	m_update_time = anitime;
	for(PropertyModifierCollection::iterator pos = m_property_collection.begin();
		pos != m_property_collection.end(); ++pos)
	{
		IPropertyModifier* modifier = pos->second;
		modifier->execute(isplay, anitime); 
	}
}

void ActorAnimationClip::update(float t)
{
	m_update_time += t;
	for(PropertyModifierCollection::iterator pos = m_curv_collection.begin();
		pos != m_curv_collection.end(); ++pos)
	{
		IPropertyModifier* modifier = pos->second;
		modifier->update(m_update_time);
	}
}

// 由谁来负责销毁，需要为了不拉不拉添加一个创建器


// 具体的实现类
/*
class PositionPropertyModifierCreator : public IPropertyModifierCreator
{
public:
	virtual IPropertyModifier* create(const std::string& fixparas)
	{
		return createPropertyModifier(&TestE::foo, fixparas);
	}
};
*/


//////////////////////////////////////////////////////////////////////////
EyeBlinkControl::EyeBlinkControl(const char* name)
{
	assert(NULL != name);
	m_name = name;
	m_actor = NULL;

	m_acc_time = 0;
	m_is_blinking = false;

	m_blink_action_name = "blink";
}

void EyeBlinkControl::update(float interval)
{
	if (m_is_blinking)
	{
		m_acc_time += interval;
		float cur_action_length = getActionLength();
		float cur_action_time = getCurActionTime();// + interval / 1000;

		if (cur_action_length <= cur_action_time)
		{
			//LogInfo("EyeBlink, ActionLength: ", cur_action_length, "ActionTime: ", cur_action_time, "Interval: ", interval / 1000);
			calculateInterval();
			blink(false);
		}
	}
	else
	{
		m_acc_time += interval;
		if (m_interval_time <= m_acc_time)
		{
			blink(true);
		}
	}
}

void EyeBlinkControl::doRandBlink()
{
	float factor = (double)rand() / (RAND_MAX + 1);
	bool blinking = factor >= 0.5;
	blink(blinking);
}

void EyeBlinkControl::calculateInterval()
{
	m_interval_time = (double)rand() / (RAND_MAX + 1) * (getMaxInterval() - getMinInterval()) + getMinInterval();
}

void EyeBlinkControl::blink(bool blinking)
{
	doBlinkAction(blinking);
	m_is_blinking = blinking;
	m_acc_time = 0;
}

//////////////////////////////////////////////////////////////////////////
SkeletonModelEyeBlinkActionControl::SkeletonModelEyeBlinkActionControl(Actor* actor, CSkeletonModelEntity* skeleton, const char* name)
: EyeBlinkControl(name)
{
	set_actor(actor);
	assert(skeleton);

	m_skeleton = skeleton;
	
	m_min_interval = 4.f * 1000 + 0.5;
	m_max_interval = 6.f * 1000 + 0.5;

	m_pet_blink_action_name = "pet_blink";

	//H3D_CLIENT::I_Config* config = m_skeleton->get_graphics_res_mng()->get_ui_wnd_mng()->getClientFront()->getConfiger();
	//if (config)
	//{
	//	float min_interval;
	//	float max_interval;
	//	config->Get(min_interval, "UI.AVATAR_BLINK_MIN_INTERVAL");
	//	config->Get(max_interval, "UI.AVATAR_BLINK_MAX_INTERVAL");
	//	m_min_interval = min_interval * 1000 + 0.5;
	//	m_max_interval = max_interval * 1000 + 0.5;
	//}

	calculateInterval();
	doRandBlink();
}

void SkeletonModelEyeBlinkActionControl::set_actor(Actor* actor)
{
	if (NULL != get_actor())
	{
		return;
	}

	EyeBlinkControl::set_actor(actor);
}

float SkeletonModelEyeBlinkActionControl::getActionLength()
{
	return m_skeleton->get_cur_action_length(ANIMATION_CHANNEL_EYES);
}

float SkeletonModelEyeBlinkActionControl::getCurActionTime()
{
	return m_skeleton->get_cur_action_time(ANIMATION_CHANNEL_EYES);
}

void SkeletonModelEyeBlinkActionControl::doBlinkAction(bool blinking)
{
	H3DI::ISkeletonModel* model = m_skeleton->getISkeletonModel();
	if (model)
	{
		model->GetAnmChannel(ANIMATION_CHANNEL_EYES)->SetAction(blinking?get_blink_action_name():"",  false);
	}
}

float SkeletonModelEyeBlinkActionControl::getMaxInterval()
{
	return m_max_interval;
}

float SkeletonModelEyeBlinkActionControl::getMinInterval()
{
	return m_min_interval;
}

const char* SkeletonModelEyeBlinkActionControl::get_blink_action_name() const
{
	if (m_skeleton->is_kind_of(MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL))
	{
		return m_pet_blink_action_name.c_str();
	}
	else
	{
		return EyeBlinkControl::get_blink_action_name();
	}
}

void SkeletonModelEyeBlinkActionControl::set_skeleton_model_entity( CSkeletonModelEntity* entity )
{
	m_skeleton = entity;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
AvatarEyeBlinkControl::AvatarEyeBlinkControl(Actor* actor, CSkeletonModelEntity* entity, const char* name)
: EyeBlinkControl(name)
{
	set_actor(actor);
	assert(entity);
	m_avatar = entity;

	m_min_interval = 4.f * 1000 + 0.5;
	m_max_interval = 6.f * 1000 + 0.5;

	//H3D_CLIENT::I_Config* config = m_avatar->get_graphics_res_mng()->get_ui_wnd_mng()->getClientFront()->getConfiger();
	//if (config)
	//{
	//	float min_interval;
	//	float max_interval;
	//	config->Get(min_interval, "UI.AVATAR_BLINK_MIN_INTERVAL");
	//	config->Get(max_interval, "UI.AVATAR_BLINK_MAX_INTERVAL");
	//	m_min_interval = min_interval * 1000 + 0.5;
	//	m_max_interval = max_interval * 1000 + 0.5;
	//}

	calculateInterval();
	doRandBlink();
}

void AvatarEyeBlinkControl::set_actor(Actor* actor)
{
	if (NULL != get_actor())
	{
		return;
	}

	EyeBlinkControl::set_actor(actor);
}

float AvatarEyeBlinkControl::getActionLength()
{
	return m_avatar->get_cur_action_length(ANIMATION_CHANNEL_EYES);
}

float AvatarEyeBlinkControl::getCurActionTime()
{
	return m_avatar->get_cur_action_time(ANIMATION_CHANNEL_EYES);
}

void AvatarEyeBlinkControl::doBlinkAction(bool blinking)
{
	H3DI::ISkeletonModel* model = m_avatar->getISkeletonModel();
	if (model)
	{
		model->GetAnmChannel(ANIMATION_CHANNEL_EYES)->SetAction(blinking?get_blink_action_name():"",  false);
	}
}

float AvatarEyeBlinkControl::getMaxInterval()
{
	return m_max_interval;
}

float AvatarEyeBlinkControl::getMinInterval()
{
	return m_min_interval;
}

////////////////////////////////////////////////////////////
TeachAvatarEyeBlinkControl::TeachAvatarEyeBlinkControl( Actor* actor, CSkeletonModelEntity* entity, const char* name )
:AvatarEyeBlinkControl(actor,entity,name)
,m_bNeedBlink(true)
{

}

TeachAvatarEyeBlinkControl::~TeachAvatarEyeBlinkControl()
{

}

void TeachAvatarEyeBlinkControl::update( float interval )
{
	if( m_bNeedBlink )
	{
		AvatarEyeBlinkControl::update(interval);
	}
	else
	{
		return ;
	}
}

void TeachAvatarEyeBlinkControl::setAvatarState( bool need_blink )
{
	m_bNeedBlink = need_blink;
}
