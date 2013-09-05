/**
* @file operation.cpp
* 这个文件的主要功能是：
*			异步操作类Operation的实现
* 
* @author 郑成俊(zhengchengjun@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 郑成俊2012.6.21
*/

#include "game_graphic_body.h"
#include "operation.h"

//--------------------Operation--------------------
Operation::Operation(bool is_view)
: m_is_view(is_view)
, m_state(OPS_NULL)
, m_type(OPT_NULL)
{
}

Operation::~Operation()
{

}

void Operation::release()
{
	stop();
	delete this;
}

OP_STATE Operation::check_state()
{
	switch(m_state)
	{
	case OPS_WAIT:
		to_execute();
		break;
	case OPS_EXECUTE:
		check_finish();
		break;
	case OPS_FINISH:
	case OPS_STOP:
		break;
	default:
		break;
	}

	return m_state;
}

//--------------------TaskOperation--------------------
TaskOperation::TaskOperation(bool is_view, unsigned int priority, H3DI::IRender* irender)
: Operation(is_view)
, m_render(irender)
, m_task_id(0)
, m_priority(priority)
{
}

void TaskOperation::finish_sync()
{
	//完成或终止状态直接return
	if (m_state == OPS_FINISH || m_state == OPS_STOP)
	{
		return;
	}

	//等待状态的需要先创建Task
	if (m_state == OPS_WAIT)
	{
		to_execute();
	}

	m_render->DoTaskImmediately(m_task_id);
	Operation::finish_sync();
}

void TaskOperation::stop()
{
	if (m_state==OPS_EXECUTE || m_state==OPS_FINISH)
	{
		if (m_task_id != 0)
		{
			m_render->CancelTask(m_task_id);	
		}
	}
	Operation::stop();
}

void TaskOperation::check_finish()
{
	int res = m_render->QueryTaskState(m_task_id);
	if (res==H3DI::TASK_OK)
	{
		m_state = OPS_FINISH;
	}
	else if (res==H3DI::TASK_ERROR)
	{
		handle_error();
	}
}

//--------------------ModelOperation--------------------
ModelOperation::ModelOperation(unsigned int priority, H3DI::IRender* irender, const std::string& resource_name, int lod)
: TaskOperation(true, priority, irender)
{
	m_type = OPT_CREATE;

	H3DI::sCreateOp co;
	co.geo_lod = lod;
	co.mat_lod = lod;
	m_task_id = m_render->CreateModelAsyn(resource_name.c_str(), co, m_priority);
	Operation::to_execute();
}

void ModelOperation::do_op(C3DEntity* entity)
{
	entity->set_engine_object_impl(m_render->GetModelAndFinishTask(m_task_id));
	entity->set_entity_state(ES_VIEW);
	entity->after_create_impl();

	Operation::stop();
}

//--------------------EffectOperation--------------------
EffectOperation::EffectOperation(unsigned int priority, H3DI::IRender* irender, const std::string& resource_name, int lod)
: TaskOperation(true, priority, irender)
{
	m_type = OPT_CREATE;

	H3DI::sCreateOp co;
	co.geo_lod = lod;
	co.mat_lod = lod;
	m_task_id = m_render->CreateSpeAsyn(resource_name.c_str(), co, m_priority);
	Operation::to_execute();
}

void EffectOperation::do_op(C3DEntity* entity)
{
	entity->set_engine_object_impl(m_render->GetModelAndFinishTask(m_task_id));
	entity->set_entity_state(ES_VIEW);
	entity->after_create_impl();

	Operation::stop();
}

//--------------------ActionOperation--------------------
ActionOperation::ActionOperation(bool is_view, unsigned int priority, H3DI::IRender* irender, const ActionOpParam& param, bool is_wait)
: TaskOperation(is_view, priority, irender)
, m_param(param)
{
	m_type = OPT_ACTION;

	//判断动作是否存在
	if (m_render->IsActionExist(m_param.action_name.c_str(), m_param.actor_type, m_param.is_male))
	{
		m_state = OPS_FINISH;
		return;
	}

	if (is_wait)
	{
		m_state = OPS_WAIT;
	}
	else
	{
		to_execute();
	}
}

void ActionOperation::do_op(C3DEntity* entity)
{
	CSkeletonModelEntity* pEntity = (CSkeletonModelEntity*)entity;
	pEntity->m_action_op = 0;
	if (m_task_id != 0)
	{
		pEntity->getISkeletonModel()->GetAnmChannel(m_param.channel_id)->SetActionAndFinishTask(m_task_id, m_param.loop);
	}
	else
	{
		pEntity->getISkeletonModel()->GetAnmChannel(m_param.channel_id)->SetAction(m_param.action_name.c_str(), m_param.loop);
	}
	if (m_param.channel_id == ANIMATION_CHANNEL_ACTION)
	{
		pEntity->getISkeletonModel()->GetAnmChannel(m_param.channel_id)->SetCurrActionTime(pEntity->get_cur_action_time(m_param.channel_id));
	}

	Operation::stop();
}

void ActionOperation::handle_error()
{
	Operation::handle_error();

	CSkeletonModelEntity* pEntity = (CSkeletonModelEntity*)m_param.entity;
	pEntity->m_action_op = 0;
}

void ActionOperation::to_execute()
{
	m_task_id = m_render->LoadActionAsyn(m_param.action_name.c_str(), m_param.actor_type, m_param.is_male, m_priority);
	Operation::to_execute();
}

//--------------------NextActionOperation--------------------
NextActionOperation::NextActionOperation(bool is_view, unsigned int priority, H3DI::IRender* irender, const ActionOpParam& param, bool is_wait)
: ActionOperation(is_view, priority, irender, param, is_wait)
{
	m_type = OPT_NEXTACTION;
}

void NextActionOperation::do_op(C3DEntity* entity)
{
	CSkeletonModelEntity* pEntity = (CSkeletonModelEntity*)entity;
	pEntity->m_next_action_op = 0;
	if (m_task_id != 0)
	{
		pEntity->getISkeletonModel()->GetAnmChannel(m_param.channel_id)->SetNextActionAndFinishTask(m_task_id, m_param.loop);
	}
	else
	{
		pEntity->getISkeletonModel()->GetAnmChannel(m_param.channel_id)->SetNextAction(m_param.action_name.c_str(), m_param.loop);
	}

	Operation::stop();
}

void NextActionOperation::handle_error()
{
	Operation::handle_error();

	CSkeletonModelEntity* pEntity = (CSkeletonModelEntity*)m_param.entity;
	pEntity->m_next_action_op = 0;
}

//--------------------BodypartOperation--------------------
BodypartOperation::BodypartOperation(bool is_view, unsigned int priority, H3DI::IRender* irender, const BodypartOpParam& param, bool is_wait)
: TaskOperation(is_view, priority, irender)
, m_param(param)
{
	m_type = OPT_BODYPART;

	if (is_wait)
	{
		m_state = OPS_WAIT;
	}
	else
	{
		to_execute();
	}
}

void BodypartOperation::do_op(C3DEntity* entity)
{
	if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_AVATAR))
	{
		AvatarEntity* pEntity = (AvatarEntity*)entity;
		pEntity->get_IActor()->SetBodyPartAndFinishTask(m_task_id);
		if (m_param.body_part>=0 && m_param.body_part<H3DI::BODYPART_NUM)
		{
			pEntity->m_bpt_op[m_param.body_part] = 0;
		}
	}
	else if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL))
	{
		CAvatarSkeletonModelEntity* pEntity = (CAvatarSkeletonModelEntity*)entity;
		pEntity->getIAvatarSkeletonModel()->SetBodyPartAndFinishTask(m_task_id);
		if (m_param.body_part>=0 && m_param.body_part<H3DI::PET_BODYPART_NUM)
		{
			pEntity->m_bpt_op[m_param.body_part] = 0;
		}
	}

	Operation::stop();
}

void BodypartOperation::handle_error()
{
	Operation::handle_error();

	if (m_param.entity->is_kind_of(MEDIA_END_ENTITY_TYPE_AVATAR))
	{
		AvatarEntity* pEntity = (AvatarEntity*)m_param.entity;
		if (m_param.body_part>=0 && m_param.body_part<H3DI::BODYPART_NUM)
		{
			pEntity->m_bpt_op[m_param.body_part] = 0;
		}
	}
	else if (m_param.entity->is_kind_of(MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL))
	{
		CAvatarSkeletonModelEntity* pEntity = (CAvatarSkeletonModelEntity*)m_param.entity;
		if (m_param.body_part>=0 && m_param.body_part<H3DI::PET_BODYPART_NUM)
		{
			pEntity->m_bpt_op[m_param.body_part] = 0;
		}
	}
}

void BodypartOperation::to_execute()
{
	H3DI::sCreateOp co;
	co.geo_lod = m_param.lod;
	co.mat_lod = m_param.lod;
	m_task_id = m_render->LoadBodypartAsyn(m_param.resource_name.c_str(), m_param.actor_type, co, m_priority);
	Operation::to_execute();
}

//--------------------AdornmentOperation--------------------
AdornmentOperation::AdornmentOperation(bool is_view, unsigned int priority, H3DI::IRender* irender, const AdornmentOpParam& param, bool is_wait)
: TaskOperation(is_view, priority, irender)
, m_param(param)
{
	m_type = OPT_ADORNMENT;

	if (is_wait)
	{
		m_state = OPS_WAIT;
	}
	else
	{
		to_execute();
	}
}

void AdornmentOperation::finish_sync()
{
	//完成或终止状态直接return
	if (m_state == OPS_FINISH || m_state == OPS_STOP)
	{
		return;
	}

	//等待状态的需要先创建Task
	if (m_state == OPS_WAIT)
	{
		to_execute();
	}

	m_render->DoTaskImmediately(m_task_id);
	if (m_param.action_task_id != 0)
	{
		m_render->DoTaskImmediately(m_param.action_task_id);	
	}	
	Operation::finish_sync();
}

void AdornmentOperation::stop()
{
	if (m_state==OPS_EXECUTE || m_state==OPS_FINISH)
	{
		if (m_task_id != 0)
		{
			m_render->CancelTask(m_task_id);	
		}
		if (m_param.action_task_id != 0)
		{
			m_render->CancelTask(m_param.action_task_id);	
		}	
	}
	Operation::stop();
}

void AdornmentOperation::do_op(C3DEntity* entity)
{
	if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_AVATAR))
	{
		AvatarEntity* pEntity = (AvatarEntity*)entity;
		H3DI::EActorAdornmentPosition ad_position = (H3DI::EActorAdornmentPosition)m_param.adornment_pos;

		pEntity->get_IActor()->SetAdornmentAndFinishTask(ad_position, m_task_id);
		if ((H3DI::ACTOR_ADORNMENT_LEFTHAND == ad_position) || (H3DI::ACTOR_ADORNMENT_RIGHTHAND == ad_position))
		{
			if (m_param.action_task_id != 0)
			{
				pEntity->get_IActor()->GetAnmChannel(pEntity->get_channel_id(ad_position))->SetActionAndFinishTask(m_param.action_task_id , true);
			}
			else
			{
				pEntity->get_IActor()->GetAnmChannel(pEntity->get_channel_id(ad_position))->SetAction(m_param.action_name.c_str(), true);
			}
		}
		if (m_param.adornment_pos>=0 && m_param.adornment_pos<H3DI::ACTOR_ADORNMENT_NUM)
		{
			pEntity->m_ad_op[m_param.adornment_pos] = 0;
		}
	}
	else if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL))
	{
		CAvatarSkeletonModelEntity* pEntity = (CAvatarSkeletonModelEntity*)entity;
		H3DI::EPetAdornmentPosition ad_position = (H3DI::EPetAdornmentPosition)m_param.adornment_pos;

		pEntity->getIAvatarSkeletonModel()->SetAdornmentAndFinishTask(ad_position, m_task_id);
		if ((H3DI::PET_ADORNMENT_LEFTHAND == ad_position) || (H3DI::PET_ADORNMENT_RIGHTHAND == ad_position))
		{
			if (m_param.action_task_id != 0)
			{
				pEntity->getIAvatarSkeletonModel()->GetAnmChannel(pEntity->get_channel_id(ad_position))->SetActionAndFinishTask(m_param.action_task_id , true);
			}
			else
			{
				pEntity->getIAvatarSkeletonModel()->GetAnmChannel(pEntity->get_channel_id(ad_position))->SetAction(m_param.action_name.c_str(), true);
			}
		}
		if (m_param.adornment_pos>=0 && m_param.adornment_pos<H3DI::PET_ADORNMENT_NUM)
		{
			pEntity->m_ad_op[m_param.adornment_pos] = 0;
		}
	}

	Operation::stop();
}

void AdornmentOperation::handle_error()
{
	Operation::handle_error();

	if (m_param.entity->is_kind_of(MEDIA_END_ENTITY_TYPE_AVATAR))
	{
		AvatarEntity* pEntity = (AvatarEntity*)m_param.entity;
		pEntity->m_ad_op[m_param.adornment_pos] = 0;
	}
	else if (m_param.entity->is_kind_of(MEDIA_END_ENTITY_TYPE_AVATAR_SKELETON_MODEL))
	{
		CAvatarSkeletonModelEntity* pEntity = (CAvatarSkeletonModelEntity*)m_param.entity;
		pEntity->m_ad_op[m_param.adornment_pos] = 0;
	}
}

void AdornmentOperation::to_execute()
{
	H3DI::sCreateOp co;
	co.geo_lod = m_param.lod;
	co.mat_lod = m_param.lod;
	m_task_id = m_render->LoadAdornmentAsyn(m_param.resource_name.c_str(), co, m_priority);
	if (m_param.action_name!="" && !m_render->IsActionExist(m_param.action_name.c_str(), m_param.actor_type, m_param.is_male))	
	{
		m_param.action_task_id = m_render->LoadActionAsyn(m_param.action_name.c_str(), m_param.actor_type, m_param.is_male, m_priority);
	}
	Operation::to_execute();
}

void AdornmentOperation::check_finish()
{
	int res = m_render->QueryTaskState(m_task_id);
	int action_res =  H3DI::TASK_OK;
	if (m_param.action_task_id != 0)
	{
		action_res = m_render->QueryTaskState(m_param.action_task_id);
	}

	if (res==H3DI::TASK_OK && action_res==H3DI::TASK_OK)
	{
		m_state = OPS_FINISH;
	}
	else if (res==H3DI::TASK_ERROR || action_res==H3DI::TASK_ERROR)
	{
		handle_error();
	}
}

//--------------------LodOperation--------------------
LodOperation::LodOperation(bool is_view, H3DI::IRender* irender, int lod, C3DEntity* entity, bool is_wait)
: TaskOperation(is_view, 0, irender)
, m_lod(lod)
, m_entity(entity)
{
	m_type = OPT_LOD;

	if (is_wait)
	{
		m_state = OPS_WAIT;
	}
	else
	{
		to_execute();
	}
}

void LodOperation::do_op(C3DEntity* entity)
{
	entity->get_IMoveObject()->ApplyMaterialLodAndFinishTask(m_task_id);

	Operation::stop();
}

void LodOperation::to_execute()
{
	H3DI::IMoveObject* move_object = m_entity->get_IMoveObject();
	assert(move_object);
	m_task_id = m_render->LoadMaterialLodAsyn(move_object, m_lod, m_priority);
	Operation::to_execute();
}

//--------------------EntityOperation--------------------
EntityOperation::EntityOperation(bool is_view, C3DEntity* main_entity, C3DEntity* entity)
: Operation(is_view)
, m_main_entity(main_entity)
, m_entity(entity)
{
	m_state = OPS_EXECUTE;
	check_finish();
}

void EntityOperation::finish_sync()
{
	//完成或终止状态直接return
	if (m_state == OPS_FINISH || m_state == OPS_STOP)
	{
		return;
	}

	m_entity->create_sync();
	Operation::finish_sync();
}

void EntityOperation::stop()
{
	if (m_main_entity)
	{
		m_main_entity->remove_entity_op(m_entity);
		m_entity = 0;
		m_main_entity = 0;
	}
	Operation::stop();
}

void EntityOperation::check_finish()
{
	if (m_entity->get_IMoveObject() != 0)
	{
		m_state = OPS_FINISH;
	}
}

//--------------------AttachOperation--------------------
AttachOperation::AttachOperation(bool is_view, C3DEntity* main_entity, C3DEntity* entity, const AttachOpParam& param)
: EntityOperation(is_view, main_entity, entity)
, m_param(param)
{
	m_type = OPT_ATTACH;
}

void AttachOperation::do_op(C3DEntity* entity)
{
	CSkeletonModelEntity* pEntity = (CSkeletonModelEntity*)entity;
	AttachItemInfo* info = pEntity->get_attach_info(m_param.bodypart, m_param.id);
	if (info)
	{
		info->m_info.m_attachModel = info->m_entity->get_IModel();
		pEntity->getISkeletonModel()->AttachModel(info->m_info);
	}

	EntityOperation::stop();
}

//--------------------SetEffectOperation--------------------
SetEffectOperation::SetEffectOperation(bool is_view, C3DEntity* main_entity, C3DEntity* entity)
: EntityOperation(is_view, main_entity, entity)
{
	m_type = OPT_SET_EFFECT;
}

void SetEffectOperation::do_op(C3DEntity* entity)
{
	CEffectEntity* effect_entity = (CEffectEntity*)m_entity;
	CModelEntity* model_entity = (CModelEntity*)entity;
	EffectActor ea;
	ea.actor = model_entity->get_IModel();
	ea.obj = 0;
	effect_entity->get_IEffect()->setActor(&ea, 1);

	EntityOperation::stop();
}

//--------------------ShowOperation--------------------
ShowOperation::ShowOperation(bool first_view)
: Operation(true)
, m_is_first_view(first_view)
{
	m_type = OPT_SHOW;
	m_state = OPS_FINISH;
}