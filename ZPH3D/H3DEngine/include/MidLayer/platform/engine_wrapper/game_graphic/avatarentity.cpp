/**
* @file actor_entity.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.29
*/

#include "game_graphic_body.h"
#include "operation.h"

AvatarEntity::AvatarEntity(CGraphicsResMng* res_mng, const std::string& name, bool is_male, int lod)
: CSkeletonModelEntity(res_mng, name, "", lod)
, m_is_male(is_male)
, m_adornment_surpress(false)
, action_time(0.f)
{
	setShadow(true);
	set_ralativetoactor(true);

	memset(m_bpt_op, 0, sizeof(m_bpt_op));
	memset(m_ad_op, 0, sizeof(m_ad_op));
	m_is_first_view = true;

	H3DI::sCreateOp co;
	co.geo_lod = m_lod_level;
	co.mat_lod = m_lod_level;
	set_engine_object_impl(m_res_mng->get_render_system()->get_IRender()->CreateActor(co, name.c_str(), is_male, H3DI::ACTION_UPDATE_X52));
	after_create_impl();
}

AvatarEntity::~AvatarEntity()
{
}

void AvatarEntity::setColorFactor(float* color, int size) 
{
	m_avatar->SetMaterialColorFactor(color, size);
}

void AvatarEntity::set_body_part(int body_part, const std::string& name, bool is_sync)
{
	assert(body_part>=0 && body_part<H3DI::BODYPART_NUM);

	std::string bpt_name = name;
	convert_path_string(bpt_name);

	if (body_part>=0 && body_part<H3DI::BODYPART_NUM)
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

/*
*@breif 分割body_group到body_list里，以分号“;”为分割符，调用客户保证body_group的长度大于0
*@param body_group 要分割的长字符串
*@param body_list 分割结果
*/
void _get_body_part_list(const std::string& body_group, std::vector<std::string>& body_list)
{
	body_list.clear();

	unsigned int ch_start = 0; //分割起始点
	unsigned int ch_length = 0; // 每次分割长度
	unsigned int ch_pos = 0; //分割符所在位置
	for(; ch_start < body_group.size(); ch_start += ch_length)
	{
		ch_pos = body_group.find_first_of(";", ch_start);//查找分号分割符所在位置

		if (ch_pos == body_group.npos)
		{
			body_list.push_back(body_group.substr(ch_start, body_group.size() - ch_start));
			ch_length = body_group.size() - ch_start;
		} 
		else
		{
			body_list.push_back(body_group.substr(ch_start, ch_pos - ch_start));
			ch_length = ch_pos - ch_start;
		}

		ch_length += 1; //分割符占1个字节
	}
}

void AvatarEntity::set_body_part_groups(const std::string& body_group)
{
	if (body_group.size() <= 0)
	{
		return;
	}

	std::vector<std::string> body_list;
	_get_body_part_list(body_group, body_list);

	std::vector<std::string>::iterator iter = body_list.begin();
	while (iter != body_list.end())
	{
		if (iter->size() > 0)
		{
			m_avatar->SetBodyPart(iter->c_str());
		}
		++iter;
	}
}

void AvatarEntity::set_skin_color(float* skin_colors)
{
	m_avatar->SetSkinColor(skin_colors);
}

void AvatarEntity::change_sex(bool is_male)
{
	//if (is_male == m_is_male)
	//{
	//	return;
	//}

	//std::string actor_name;
	////创建新IActor
	//H3DI::IModel* model = get_graphics_res_mng()->get_render_system()->get_IRender()->CreateActor("avatar_change_sex", is_male);

	//if (NULL == model)
	//{
	//	return;
	//}

	//m_is_male = is_male;
	//change_media_object(model, true);
}

bool AvatarEntity::change_media_object(H3DI::IMoveObject* object, bool use_preproperty)
{
	return false;
	//H3DI::IActor* actor = dynamic_cast<H3DI::IActor*>(object);
	//if (NULL == actor)
	//{
	//	return false;
	//}

	////以后要添加要保留的原有属性的设置

	//m_avatar = actor;

	//return CSkeletonModelEntity::change_media_object(object, use_preproperty);
}

H3DI::IActor* AvatarEntity::get_IActor() const 
{ 
	return m_avatar;
}

bool AvatarEntity::is_kind_of(int obj_type)
{
	if (MEDIA_END_ENTITY_TYPE_AVATAR == obj_type)
	{
		return true;
	}
	return CSkeletonModelEntity::is_kind_of(obj_type);
}

int	AvatarEntity::type(void) const
{
	return MEDIA_END_ENTITY_TYPE_AVATAR;
}

//动作相关
void AvatarEntity::set_action(unsigned int channel_id, const char* action, bool loop, bool is_sync)
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
		m_avatar->GetAnmChannel(channel_id)->SetAction("", false);
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

void AvatarEntity::set_next_action(unsigned int channel_id, const char* action, bool loop, bool is_sync)
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
		m_avatar->GetAnmChannel(channel_id)->SetNextAction("", false);
		return;
	}

	if (is_sync)
	{
		m_avatar->GetAnmChannel(channel_id)->SetNextAction(action, loop);
	}
	else
	{
		ActionOpParam param;
		param.channel_id = channel_id;
		param.action_name = action ? action : "";
		param.loop = loop;
		param.actor_type = H3DI::ACTOR_HUMAN;
		param.is_male = m_is_male;
		param.entity = this;

		NextActionOperation* pOp = new NextActionOperation(m_is_view_sync, m_priority, m_res_mng->get_render_system()->get_IRender(), param, next_wait());
		m_cache_op_list.push_back(pOp);

		m_next_action_op = pOp;
	}
};

void AvatarEntity::setAdornment( int ad_position, const char* ad_file, const char* action, bool is_sync)
{
	m_res_mng->get_render_system()->log("角色设置挂件 %x 位置:%d", this, ad_position);

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
	if ((H3DI::ACTOR_ADORNMENT_LEFTHAND == ad_position) || (H3DI::ACTOR_ADORNMENT_RIGHTHAND == ad_position))
	{
		add_set_actions(get_channel_id(ad_position), action ? action : "",  action ? true : false);
	}

	//处理取消挂件
	if (m_adorment[ad_position] == "")
	{
		m_avatar->SetAdornment((H3DI::EActorAdornmentPosition)ad_position, 0);
		if ((H3DI::ACTOR_ADORNMENT_LEFTHAND == ad_position) || (H3DI::ACTOR_ADORNMENT_RIGHTHAND == ad_position))
		{
			m_avatar->GetAnmChannel(get_channel_id(ad_position))->SetAction("", true);
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

ANIMATION_CHANNEL_ID AvatarEntity::get_channel_id(int pos)
{
	if (H3DI::ACTOR_ADORNMENT_LEFTHAND == pos)
	{
		return ANIMATION_CHANNEL_LEFT_HAND;
	}
	else if(H3DI::ACTOR_ADORNMENT_RIGHTHAND == pos)
	{
		return ANIMATION_CHANNEL_RIGHT_HAND;
	}

	return -1;
}

void AvatarEntity::do_update(float interval)
{
	CSkeletonModelEntity::do_update(interval);
	m_avatar->UpdateAdornment(interval/1000.f);
};

void AvatarEntity::update_adornment(float interval)
{
	m_avatar->UpdateAdornment(interval/1000.f);
}

void AvatarEntity::reset()
{
	// 设置动作
	for (int i = 0; i < ANIMATION_CHANNEL_COUNT; ++i)
	{
		m_avatar->GetAnmChannel(i)->SetAction(m_action_id[i].c_str(), m_action_loop[i]);
	}

	// 设置挂件
	for (int i = 0; i < H3DI::ACTOR_ADORNMENT_NUM; ++i)
	{
		m_avatar->ResetAdornment((H3DI::EActorAdornmentPosition)i);
	}	
}

void AvatarEntity::set_adornment_surpress(bool surpress)
{
	m_adornment_surpress = surpress;
	
	m_avatar->GetAnmChannel(ANIMATION_CHANNEL_LEFT_HAND)->SetSkipBlend(surpress);
	m_avatar->GetAnmChannel(ANIMATION_CHANNEL_RIGHT_HAND)->SetSkipBlend(surpress);
	m_avatar->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_LEFTHAND, !surpress);
	m_avatar->SetAdornmentVisibility(H3DI::ACTOR_ADORNMENT_RIGHTHAND, !surpress);
}

void AvatarEntity::set_adornment_surpress(bool surpress, H3DI::EActorAdornmentPosition ad_position)
{
	m_avatar->SetAdornmentVisibility(ad_position, !surpress);
	if ((H3DI::ACTOR_ADORNMENT_LEFTHAND == ad_position) || (H3DI::ACTOR_ADORNMENT_RIGHTHAND == ad_position))
	{
		m_avatar->GetAnmChannel(get_channel_id(ad_position))->SetSkipBlend(surpress);
	}
}

void AvatarEntity::set_adornment_visible(bool visible, H3DI::EActorAdornmentPosition ad_position)
{
	m_avatar->SetAdornmentVisibility(ad_position, visible);
}

void AvatarEntity::getAvatarAppearance(AvatarAppearance& appearance)
{
	appearance.geo_lod = m_lod_level;
	appearance.mat_lod = m_lod_level;
	appearance.male = m_is_male;
	for (int i=0; i<H3DI::BODYPART_NUM; i++)
	{
		strcpy(appearance.bodyparts[i], m_body_part[i].c_str()); 
	}
	for (int i=0; i<H3DI::ACTOR_ADORNMENT_NUM; i++)
	{
		strcpy(appearance.adornments[i], m_adorment[i].c_str()); 
	}
}

void AvatarEntity::clear_all_op()
{
	CSkeletonModelEntity::clear_all_op();
	memset(m_bpt_op, 0, sizeof(m_bpt_op));
	memset(m_ad_op, 0, sizeof(m_ad_op));
}

void AvatarEntity::set_engine_object_impl(H3DI::IMoveObject* object)
{
	CSkeletonModelEntity::set_engine_object_impl(object);
	m_avatar = (H3DI::IActor*)object;
}

void AvatarEntity::set_body_part_immediate(int body_part, const std::string& bpt_name, bool is_sync)
{
	if (is_sync)
	{
		finish_all_op();
		m_avatar->SetBodyPart(bpt_name.c_str());
	}
	else
	{
		BodypartOpParam param;
		param.actor_type = H3DI::ACTOR_HUMAN;
		param.body_part = body_part;
		param.resource_name = bpt_name;
		param.lod = m_lod_level;
		param.entity = this;

		BodypartOperation* pOp = new BodypartOperation(m_is_view_sync, m_priority, m_res_mng->get_render_system()->get_IRender()
			, param, next_wait());
		m_execute_op_list[m_priority].push_back(pOp);

		if (body_part>=0 && body_part<H3DI::BODYPART_NUM)
		{
			m_bpt_op[body_part] = pOp;
		}
	}
}

void AvatarEntity::set_adornment_immediate(int ad_position, const char* ad_name, const char* action, bool is_sync)
{
	if (is_sync)
	{
		finish_all_op();
		m_avatar->SetAdornment((H3DI::EActorAdornmentPosition)ad_position, ad_name);
		if ((H3DI::ACTOR_ADORNMENT_LEFTHAND == ad_position) || (H3DI::ACTOR_ADORNMENT_RIGHTHAND == ad_position))
		{
			m_avatar->GetAnmChannel(get_channel_id(ad_position))->SetAction(action ? action : "", true);
		}
	}
	else
	{
		AdornmentOpParam param;
		param.resource_name = ad_name;
		param.lod = m_lod_level;
		param.adornment_pos = ad_position;
		param.action_name = action ? action : "";
		param.actor_type = H3DI::ACTOR_HUMAN;
		param.is_male = m_is_male;
		param.entity = this;

		AdornmentOperation* pOp = new AdornmentOperation(m_is_view_sync, m_priority, m_res_mng->get_render_system()->get_IRender()
			, param, next_wait());
		m_execute_op_list[m_priority].push_back(pOp);

		m_ad_op[ad_position] = pOp;
	}
}

void AvatarEntity::set_action_immediate(unsigned int channel_id, const char* action, bool loop, bool is_sync)
{
	if (is_sync)
	{
		m_avatar->GetAnmChannel(channel_id)->SetAction(action, loop);
	}
	else
	{
		ActionOpParam param;
		param.channel_id = channel_id;
		param.action_name = action;
		param.loop = loop;
		param.actor_type = H3DI::ACTOR_HUMAN;
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

void AvatarEntity::EnableHeelOffset(bool bEable)
{
	if ( get_IActor() )
	{
		get_IActor()->EnableHeelOffset(bEable);
	}
}

int AvatarEntity::setUseCustomBoundBox(bool b)
{
	return m_avatar->SetUseCustomBoundBox(b);
}

bool AvatarEntity::SetAdornmentAction(int adornmentposition, const char* actionName, float transitionTime)
{
	m_res_mng->get_render_system()->log("角色设置挂件动作 %x 位置:%d 动作名:%s", this, adornmentposition, actionName);

	return m_avatar->SetAdornmentAction((H3DI::EActorAdornmentPosition)adornmentposition, actionName, transitionTime);
}

void AvatarEntity::UnloadResources()
{
	if (m_entity_state == ES_UNLOAD)
	{
		return;
	}

	clear_all_op();
	m_entity_state = ES_UNLOAD;
	m_is_first_view = true;
	set_entity_flags(H3DI::I_ENTITY_HIDE, true);
	m_avatar->SetFakeRender(false);

	//释放资源
	m_avatar->UnloadResource();
	UnloadAttachItem();
	UnloadEffect();
}

void AvatarEntity::ReloadResources(bool is_sync, int priority)
{
	if (m_entity_state != ES_UNLOAD)
	{
		return;
	}

	clear_all_op();
	m_entity_state = ES_HIDE;
	
	//重载资源
	m_avatar->ReloadResource();
	m_priority = priority;
	for (unsigned int i=0; i<H3DI::BODYPART_NUM; ++i)
	{
		if (m_body_part[i] != "")
		{
			set_body_part_immediate(i, m_body_part[i], is_sync);
		}
	}
	for (unsigned int i=0; i<H3DI::ACTOR_ADORNMENT_NUM; ++i)
	{
		if (m_adorment[i] != "")
		{
			const char* action = 0;
			if ((H3DI::ACTOR_ADORNMENT_LEFTHAND == i) || (H3DI::ACTOR_ADORNMENT_RIGHTHAND == i))
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
		m_avatar->GetAnmChannel(ANIMATION_CHANNEL_ACTION)->SetCurrActionTime(m_played_time/1000.f);
		
		m_entity_state = ES_VIEW;
		update_engine_entity_visible();
	}
	else
	{
		update_appearance();
	}
}