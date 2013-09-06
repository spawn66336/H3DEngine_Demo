/*
*@file  avatar_skeleton_mode_entity.cpp
*@breif 实现对引擎层的IAvatarSkeletonModel的封装
*@author mozisi
*/
#include "game_graphic_body.h"
#include "operation.h"

CAvatarSkeletonModelEntity::CAvatarSkeletonModelEntity(CGraphicsResMng* res_mng, const std::string& name
													   , bool is_male, int lod)
: CSkeletonModelEntity(res_mng, name, "", lod)
, m_is_male(is_male)
, m_adornment_surpress(false)
{
	setShadow(true);
	set_ralativetoactor(true);

	memset(m_bpt_op, 0, sizeof(m_bpt_op));
	memset(m_ad_op, 0, sizeof(m_ad_op));
	m_is_first_view = true;

	H3DI::sCreateOp co;
	co.geo_lod = m_lod_level;
	co.mat_lod = m_lod_level;
	set_engine_object_impl(m_res_mng->get_render_system()->get_IRender()->CreateAvatarSkeletonModel(co, name.c_str(), is_male, H3DI::ACTION_UPDATE_X52));
	after_create_impl();
}

CAvatarSkeletonModelEntity::~CAvatarSkeletonModelEntity()
{
}

H3DI::IAvatarSkeletonModel* CAvatarSkeletonModelEntity::getIAvatarSkeletonModel() const
{
	return m_avatar_skeleton_model;
}

int CAvatarSkeletonModelEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL;
}

bool CAvatarSkeletonModelEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL == obj_type)
	{
		return true;
	}
	return CSkeletonModelEntity::is_kind_of(obj_type);
}

bool CAvatarSkeletonModelEntity::change_media_object(H3DI::IMoveObject* object, bool use_preproperty)
{
	//IAvatarSkeletonModel无此接口，覆盖父类，不作任何事
	return false;
}

void CAvatarSkeletonModelEntity::set_body_part(int body_part, const std::string& name, bool is_sync)
{
	assert(body_part>=0 && body_part<H3DI::PET_BODYPART_NUM);

	std::string bpt_name = name;
	convert_path_string(bpt_name);

	if (body_part>=0 && body_part<H3DI::PET_BODYPART_NUM)
	{
		//重复换装直接return
		if (m_body_part[body_part] == bpt_name)
		{
			return;
		}

		if (m_bpt_op[body_part] && m_entity_state!=ES_HIDE)
		{
			m_bpt_op[body_part]->stop();
			m_bpt_op[body_part] = 0;
		}

		m_body_part[body_part] = bpt_name;
	}

	if (m_entity_state != ES_UNLOAD)
	{
		set_body_part_immediate(body_part, bpt_name, is_sync);
	}
	else
	{
		middleAssert(!is_sync && "Unload状态不能使用同步接口");
	}
}

void CAvatarSkeletonModelEntity::setAdornment(int ad_position, const char* ad_file, const char* action, bool is_sync)
{
	m_res_mng->get_render_system()->log("宠物设置挂件 %x 位置:%d", this, ad_position);

	std::string ad_name = ad_file ? ad_file : "";
	convert_path_string(ad_name);

	//重复设挂件直接return
	if (m_adorment[ad_position] == ad_name)
	{
		return;
	}

	if (m_ad_op[ad_position] && m_entity_state!=ES_HIDE)
	{
		m_ad_op[ad_position]->stop();
		m_ad_op[ad_position] = 0;
	}

	m_adorment[ad_position] = ad_name;
	if ((H3DI::PET_ADORNMENT_LEFTHAND == ad_position) || (H3DI::PET_ADORNMENT_RIGHTHAND == ad_position))
	{
		add_set_actions(get_channel_id(ad_position), action ? action : "",  action ? true : false);
	}

	//处理取消挂件
	if (m_adorment[ad_position].empty())
	{
		m_avatar_skeleton_model->SetAdornment((H3DI::EPetAdornmentPosition)ad_position, 0);
		if ((H3DI::PET_ADORNMENT_LEFTHAND == ad_position) || (H3DI::PET_ADORNMENT_RIGHTHAND == ad_position))
		{
			m_avatar_skeleton_model->GetAnmChannel(get_channel_id(ad_position))->SetAction("", true);
		}
		return;
	}

	if (m_entity_state != ES_UNLOAD)
	{
		set_adornment_immediate(ad_position, ad_name.c_str(), action, is_sync);
	}
	else
	{
		middleAssert(!is_sync && "Unload状态不能使用同步接口");
	}
}

ANIMATION_CHANNEL_ID CAvatarSkeletonModelEntity::get_channel_id(int pos)
{
	if (H3DI::PET_ADORNMENT_LEFTHAND == pos)
	{
		return ANIMATION_CHANNEL_LEFT_HAND;
	}
	else if(H3DI::PET_ADORNMENT_RIGHTHAND == pos)
	{
		return ANIMATION_CHANNEL_RIGHT_HAND;
	}

	return -1;
}

void CAvatarSkeletonModelEntity::set_adornment_surpress(bool surpress)
{
	m_adornment_surpress = surpress;

	m_avatar_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_LEFT_HAND)->SetSkipBlend(surpress);
	m_avatar_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_RIGHT_HAND)->SetSkipBlend(surpress);
	m_avatar_skeleton_model->SetAdornmentVisibility(H3DI::PET_ADORNMENT_LEFTHAND, !surpress);
	m_avatar_skeleton_model->SetAdornmentVisibility(H3DI::PET_ADORNMENT_RIGHTHAND, !surpress);
}

void CAvatarSkeletonModelEntity::set_adornment_surpress(bool surpress, H3DI::EPetAdornmentPosition ad_position)
{
	m_avatar_skeleton_model->SetAdornmentVisibility(ad_position, !surpress);
	if ((H3DI::PET_ADORNMENT_LEFTHAND == ad_position) || (H3DI::PET_ADORNMENT_RIGHTHAND == ad_position))
	{
		m_avatar_skeleton_model->GetAnmChannel(get_channel_id(ad_position))->SetSkipBlend(surpress);
	}
}

void CAvatarSkeletonModelEntity::set_action(unsigned int channel_id, const char* action, bool loop, bool is_sync)
{
	//动作更新时间重置
	if (channel_id == ANIMATION_CHANNEL_ACTION)
	{
		m_played_time = 0.f;

		if (m_action_op)
		{
			m_action_op->stop();
			m_action_op = 0;
		}
	}

	//保留动作信息
	add_set_actions(channel_id, action, loop);

	//处理取消动作
	if (!action || *action=='\0')
	{
		m_avatar_skeleton_model->GetAnmChannel(channel_id)->SetAction("", false);
		return;
	}

	if (m_entity_state != ES_UNLOAD)
	{
		set_action_immediate(channel_id, action, loop, is_sync);
	}
	else
	{
		middleAssert(!is_sync && "Unload状态不能使用同步接口");
	}
};

void CAvatarSkeletonModelEntity::set_next_action(unsigned int channel_id, const char* action, bool loop, bool is_sync)
{
	//保留动作信息
	add_set_next_actions(channel_id, action, loop);

	if (m_next_action_op)
	{
		m_next_action_op->stop();
		m_next_action_op = 0;
	}

	//处理取消动作
	if (!action || *action=='\0')
	{
		m_avatar_skeleton_model->GetAnmChannel(channel_id)->SetNextAction("", false);
		return;
	}

	if (is_sync)
	{
		m_avatar_skeleton_model->GetAnmChannel(channel_id)->SetNextAction(action, loop);
	}
	else
	{
		ActionOpParam param;
		param.channel_id = channel_id;
		param.action_name = action ? action : "";
		param.loop = loop;
		param.actor_type = H3DI::ACTOR_PET;
		param.is_male = m_is_male;
		param.entity = this;

		NextActionOperation* pOp = new NextActionOperation(m_is_view_sync, m_priority, m_res_mng->get_render_system()->get_IRender(), param, next_wait());
		m_cache_op_list.push_back(pOp);

		m_next_action_op = pOp;
	}
};

void CAvatarSkeletonModelEntity::reset()
{
	if (!m_avatar_skeleton_model)
		return;

	for (int i = 0; i < ANIMATION_CHANNEL_COUNT; ++i)
	{
		m_avatar_skeleton_model->GetAnmChannel(i)->SetAction(m_action_id[i].c_str(), m_action_loop[i]);
	}

	for (int i = 0; i < H3DI::PET_ADORNMENT_NUM; ++i)
	{
		m_avatar_skeleton_model->ResetAdornment((H3DI::EPetAdornmentPosition)i);
	}	
}

void CAvatarSkeletonModelEntity::getAvatarSkeletonModelAppearance(AvatarSkeletonAppearance& appearance)
{
	appearance.geo_lod = m_lod_level;
	appearance.mat_lod = m_lod_level;
	appearance.male = m_is_male;
	for (int i=0; i<H3DI::PET_BODYPART_NUM; i++)
	{
		strcpy(appearance.bodyparts[i], m_body_part[i].c_str()); 
	}
	for (int i=0; i<H3DI::PET_ADORNMENT_NUM; i++)
	{
		strcpy(appearance.adornments[i], m_adorment[i].c_str()); 
	}
}

void CAvatarSkeletonModelEntity::do_update(float interval)
{	
	CSkeletonModelEntity::do_update(interval);
	m_avatar_skeleton_model->UpdateAdornment(interval/1000.f);	
}

void CAvatarSkeletonModelEntity::update_adornment(float interval)
{
	m_avatar_skeleton_model->UpdateAdornment(interval/1000.f);
}

void CAvatarSkeletonModelEntity::clear_all_op()
{
	CSkeletonModelEntity::clear_all_op();
	memset(m_bpt_op, 0, sizeof(m_bpt_op));
	memset(m_ad_op, 0, sizeof(m_ad_op));
}

void CAvatarSkeletonModelEntity::set_engine_object_impl(H3DI::IMoveObject* object)
{
	CSkeletonModelEntity::set_engine_object_impl(object);
	m_avatar_skeleton_model = (H3DI::IAvatarSkeletonModel*)object;
}

void CAvatarSkeletonModelEntity::set_body_part_immediate(int body_part, const std::string& bpt_name, bool is_sync)
{
	if (is_sync)
	{
		finish_all_op();
		m_avatar_skeleton_model->SetBodyPart(bpt_name.c_str());
	}
	else
	{
		BodypartOpParam param;
		param.actor_type = H3DI::ACTOR_PET;
		param.body_part = body_part;
		param.resource_name = bpt_name;
		param.lod = m_lod_level;
		param.entity = this;

		BodypartOperation* pOp = new BodypartOperation(m_is_view_sync, m_priority, m_res_mng->get_render_system()->get_IRender()
			, param, next_wait());
		m_execute_op_list[m_priority].push_back(pOp);

		if (body_part>=0 && body_part<H3DI::PET_BODYPART_NUM)
		{
			m_bpt_op[body_part] = pOp;
		}
	}
}

void CAvatarSkeletonModelEntity::set_adornment_immediate(int ad_position, const char* ad_name, const char* action, bool is_sync)
{
	if (is_sync)
	{
		finish_all_op();
		m_avatar_skeleton_model->SetAdornment((H3DI::EPetAdornmentPosition)ad_position, ad_name);
		if ((H3DI::PET_ADORNMENT_LEFTHAND == ad_position) || (H3DI::PET_ADORNMENT_RIGHTHAND == ad_position))
		{
			m_avatar_skeleton_model->GetAnmChannel(get_channel_id(ad_position))->SetAction(action ? action : "", true);
		}
	}
	else
	{
		AdornmentOpParam param;
		param.resource_name = ad_name;
		param.lod = m_lod_level;
		param.adornment_pos = ad_position;
		param.action_name = action ? action : "";
		param.actor_type = H3DI::ACTOR_PET;
		param.is_male = m_is_male;
		param.entity = this;

		AdornmentOperation* pOp = new AdornmentOperation(m_is_view_sync, m_priority, m_res_mng->get_render_system()->get_IRender()
			, param, next_wait());
		m_execute_op_list[m_priority].push_back(pOp);

		m_ad_op[ad_position] = pOp;
	}
}

void CAvatarSkeletonModelEntity::set_action_immediate(unsigned int channel_id, const char* action, bool loop, bool is_sync)
{
	if (is_sync)
	{
		m_avatar_skeleton_model->GetAnmChannel(channel_id)->SetAction(action, loop);
	}
	else
	{
		ActionOpParam param;
		param.channel_id = channel_id;
		param.action_name = action ? action : "";
		param.loop = loop;
		param.actor_type = H3DI::ACTOR_PET;
		param.is_male = m_is_male;
		param.entity = this;

		ActionOperation* pOp = new ActionOperation(m_is_view_sync, m_priority, m_res_mng->get_render_system()->get_IRender(), param, next_wait());
		m_cache_op_list.push_back(pOp);

		if (channel_id==ANIMATION_CHANNEL_ACTION)
		{
			m_action_op = pOp;
		}
	}
}

bool CAvatarSkeletonModelEntity::SetAdornmentAction(int adornmentposition, const char* actionName, float transitionTime)
{
	m_res_mng->get_render_system()->log("宠物设置挂件动作 %x 位置:%d 动作名:%s", this, adornmentposition, actionName);

	return m_avatar_skeleton_model->SetAdornmentAction((H3DI::EPetAdornmentPosition)adornmentposition, actionName, transitionTime);
}

void CAvatarSkeletonModelEntity::UnloadResources()
{
	if (m_entity_state == ES_UNLOAD)
	{
		return;
	}

	clear_all_op();
	m_entity_state = ES_UNLOAD;
	m_is_first_view = true;
	set_entity_flags(H3DI::I_ENTITY_HIDE, true);
	m_avatar_skeleton_model->SetFakeRender(false);

	//释放资源
	m_avatar_skeleton_model->UnloadResource();
	UnloadAttachItem();
	UnloadEffect();
}

void CAvatarSkeletonModelEntity::ReloadResources(bool is_sync, int priority)
{
	if (m_entity_state != ES_UNLOAD)
	{
		return;
	}

	clear_all_op();
	m_entity_state = ES_HIDE;

	//重载资源
	m_avatar_skeleton_model->ReloadResource();
	m_priority = priority;
	for (unsigned int i=0; i<H3DI::PET_BODYPART_NUM; ++i)
	{
		if (m_body_part[i] != "")
		{
			set_body_part_immediate(i, m_body_part[i], is_sync);
		}
	}
	for (unsigned int i=0; i<H3DI::PET_ADORNMENT_NUM; ++i)
	{
		if (m_adorment[i] != "")
		{
			const char* action = 0;
			if ((H3DI::PET_ADORNMENT_LEFTHAND == i) || (H3DI::PET_ADORNMENT_RIGHTHAND == i))
			{
				action = m_action_id[get_channel_id(i)].c_str();
			}
			set_adornment_immediate(i, m_adorment[i].c_str(), action, is_sync);
		}
	}
	ReloadAttachItem(is_sync);
	ReloadEffect(is_sync, priority);
	if (!m_action_id[ANIMATION_CHANNEL_ACTION].empty())
	{
		set_action_immediate(ANIMATION_CHANNEL_ACTION, m_action_id[ANIMATION_CHANNEL_ACTION].c_str()
			, m_action_loop[ANIMATION_CHANNEL_ACTION], is_sync);
	}

	if (is_sync)
	{
		m_avatar_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_ACTION)->SetCurrActionTime(m_played_time/1000.f);

		m_entity_state = ES_VIEW;
		update_engine_entity_visible();
	}
	else
	{
		update_appearance();
	}
}