/*
*@file  skeleton_model_entity.h
*@breif 实现了引擎层的ISkeletonModel类的封装类CSkeletonModelEntity的实现
*@author mozisi
*/

#include "game_graphic_body.h"
#include "operation.h"

CSkeletonModelEntity::CSkeletonModelEntity(CGraphicsResMng* res_mng, const std::string& name
										   , const std::string& resource_name, int lod)
: CModelEntity(res_mng, name, resource_name, lod)
, m_skeleton_model(0)
, m_fix_foot(true)
, m_played_time(0.f)
, m_transition_time_from_current(0.45f)
, m_transition_time_to_next(0.45f)
, m_actor_phy(true)
, m_has_lightmap(false)
, m_action_op(0)
, m_next_action_op(0)
, m_is_all_attach_item_visible(true)
{
	m_is_view_sync = false;
	m_level_layer = H3DI::SL_Actors;

	for (int i = 0; i < ANIMATION_CHANNEL_COUNT; ++i)
	{
		m_action_loop[i] = false;
		m_action_id[i] = "";
		m_next_action_loop[i] = false;
		m_next_action_id[i] = "";
	}
}

CSkeletonModelEntity::~CSkeletonModelEntity()
{
	remove_all_attached_model();
}

H3DI::ISkeletonModel* CSkeletonModelEntity::getISkeletonModel() const
{
	return m_skeleton_model;
}

bool CSkeletonModelEntity::change_resource(const std::string& res_name)
{
	std::string input_res_name = res_name;
	convert_path_string(input_res_name);

	if(stricmp(resource_name().c_str(),input_res_name.c_str()) == 0)
	{
		return true;
	}

	H3DI::IModel* model = m_res_mng->get_render_system()->get_IRender()->CreateModel(input_res_name.c_str());
	H3DI::ISkeletonModel* skeleton = dynamic_cast<H3DI::ISkeletonModel*>(model);
	if (NULL == skeleton)
	{
		return false;
	}
	if (change_media_object(skeleton, true))
	{
		set_resource_name(input_res_name);
		return true;
	}

	return false;
}

void CSkeletonModelEntity::setAction(unsigned int channel_id, const char* action, bool loop)
{
	//动作更新时间重置
	if (channel_id == ANIMATION_CHANNEL_ACTION)
	{
		m_played_time = 0.f;
	}

	//保留动作信息
	add_set_actions(channel_id, action, loop);

	if (m_skeleton_model)
	{
		m_skeleton_model->GetAnmChannel(channel_id)->SetAction(action, loop);
	}
}

void CSkeletonModelEntity::setNextAction(unsigned int channel_id, const char* action, bool loop)
{
	//保留动作信息
	add_set_next_actions(channel_id, action, loop);

	if (m_skeleton_model)
	{
		m_skeleton_model->GetAnmChannel(channel_id)->SetNextAction(action, loop);
	}
}

void CSkeletonModelEntity::set_action(unsigned int channel_id, const char* action, bool loop, bool is_sync)
{
	setAction(channel_id, action, loop);
}

void CSkeletonModelEntity::set_next_action(unsigned int channel_id, const char* action, bool loop, bool is_sync)
{
	setNextAction(channel_id, action, loop);
}

bool CSkeletonModelEntity::is_valid_channel(ANIMATION_CHANNEL_ID channel_id) const
{
	return (channel_id > ANIMATION_CHANNEL_ALL) && (channel_id < ANIMATION_CHANNEL_COUNT);
}

void CSkeletonModelEntity::set_channel_loop(ANIMATION_CHANNEL_ID channel_id, bool channel_loop)
{
	m_action_loop[channel_id] = channel_loop;

	if (m_skeleton_model)
	{
		m_skeleton_model->GetAnmChannel(channel_id)->SetActionLoop(channel_loop);
	}
}

bool CSkeletonModelEntity::change_media_object(H3DI::IMoveObject* object, bool use_preproperty)
{
	H3DI::ISkeletonModel* model = dynamic_cast<H3DI::ISkeletonModel*>(object);
	if (NULL == model)
	{
		return false;
	}

	//更改资源
	m_skeleton_model = model;
	
	set_fixfoot_transmode(ANIMATION_CHANNEL_ACTION, true);
	add_dominating_channel(ANIMATION_CHANNEL_EYES);
	add_dominating_channel(ANIMATION_CHANNEL_LEFT_HAND);
	add_dominating_channel(ANIMATION_CHANNEL_RIGHT_HAND);

	m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_EYES)->SetAction("", true);
	m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_LEFT_HAND)->SetAction("", true);
	m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_RIGHT_HAND)->SetAction("", true);

	m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_EYES)->SetTransitionTimeToNext(0);
	m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_EYES)->SetTransitionTimeFromCurrent(0);

	m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_LEFT_HAND)->SetTransitionTimeToNext(0);
	m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_LEFT_HAND)->SetTransitionTimeFromCurrent(0);

	m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_RIGHT_HAND)->SetTransitionTimeToNext(0);
	m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_RIGHT_HAND)->SetTransitionTimeFromCurrent(0);

	return CModelEntity::change_media_object(object, use_preproperty);
}

void CSkeletonModelEntity::set_fixfoot_transmode(unsigned int channel_id, bool fixfoot)
{
	m_fix_foot = fixfoot;

	int flag = H3DI::TRANSITION_NEXT_UPDATE;
	if(fixfoot)
	{
		flag |= H3DI::TRANSITION_CURRENT_UPDATE|H3DI::TRANSITION_NEXT_UPDATE|H3DI::TRANSITION_LOCALLY| H3DI::TRANSITION_FIXED_FOOT;
	}
	else
	{
		flag |= H3DI::TRANSITION_CURRENT_UPDATE|H3DI::TRANSITION_NEXT_UPDATE;//不取消一拍
	}
	set_transition_mode(channel_id, flag);
}

void CSkeletonModelEntity::set_transition_mode(unsigned int channelid, int mode)
{
	if (m_skeleton_model)
	{
		m_skeleton_model->GetAnmChannel(channelid)->SetTransitionMode(mode);
	}
}

int CSkeletonModelEntity::get_transition_mode(unsigned int channelid)
{
	if (m_skeleton_model)
	{
		return m_skeleton_model->GetAnmChannel(channelid)->GetTransitionMode();
	}

	return 0;
};

void CSkeletonModelEntity::set_transition_time_from_current(ANIMATION_CHANNEL_ID channel_id, float interval)
{
	if (channel_id == ANIMATION_CHANNEL_ACTION)
	{
		m_transition_time_from_current = interval;
	}

	if (m_skeleton_model)
	{
		m_skeleton_model->GetAnmChannel(channel_id)->SetTransitionTimeFromCurrent(interval);
	}
}

float CSkeletonModelEntity::get_transition_time_from_current(ANIMATION_CHANNEL_ID channel_id) const
{
	if (channel_id == ANIMATION_CHANNEL_ACTION)
	{
		return m_transition_time_from_current;
	}
	else if (m_skeleton_model)
	{
		return m_skeleton_model->GetAnmChannel(channel_id)->GetTransitionTimeFromCurrent();
	}

	return -1.0f;
}

void CSkeletonModelEntity::set_transition_time_to_next(ANIMATION_CHANNEL_ID channel_id, float interval)
{
	if (channel_id == ANIMATION_CHANNEL_ACTION)
	{
		m_transition_time_to_next = interval;
	}

	if (m_skeleton_model)
	{
		m_skeleton_model->GetAnmChannel(channel_id)->SetTransitionTimeToNext(interval);
	}
}

float CSkeletonModelEntity::get_transition_time_to_next(ANIMATION_CHANNEL_ID channel_id) const
{
	if (channel_id == ANIMATION_CHANNEL_ACTION)
	{
		return m_transition_time_to_next;
	}
	else if (m_skeleton_model)
	{
		return m_skeleton_model->GetAnmChannel(channel_id)->GetTransitionTimeToNext();
	}

	return -1.0f;
}

float CSkeletonModelEntity::get_cur_action_time(unsigned int channel_id)
{
	if (channel_id == ANIMATION_CHANNEL_ACTION)
	{
		return m_played_time/1000.f;
	}
	else if (m_skeleton_model)
	{
		return m_skeleton_model->GetAnmChannel(channel_id)->GetCurrActionTime();
	}

	return 0.f;
}

float CSkeletonModelEntity::get_cur_real_action_time(unsigned int channel_id)
{
	if (m_skeleton_model)
	{
		H3DI::IAction* action = m_skeleton_model->GetAnmChannel(channel_id)->GetCurrAction();
		if (action && m_action_id[channel_id]==action->GetName())
		{
			return m_skeleton_model->GetAnmChannel(channel_id)->GetCurrActionTime();
		}
	}

	return 0.f;
}

float CSkeletonModelEntity::get_cur_action_length(unsigned int channel_id)
{
	if (m_skeleton_model)
	{
		H3DI::IAction* action = m_skeleton_model->GetAnmChannel(channel_id)->GetCurrAction();
		if (action && m_action_id[channel_id]==action->GetName())
		{
			return m_skeleton_model->GetAnmChannel(channel_id)->GetCurrActionLength();
		}
	}

	return 0.f;
}

void CSkeletonModelEntity::add_dominating_channel(int action_channel)
{
	if (m_skeleton_model)
	{
		const int BUF_LEN = 20;
		int buf[BUF_LEN] = {0};
		int rel_len = m_skeleton_model->GetDominatingChannel(buf, BUF_LEN);

		bool has = false;
		if (rel_len > 0)
		{
			for (int i = 0; i < rel_len; ++i)
			{
				if (buf[i] == action_channel)
				{
					has = true;
					break;
				}
			}
		}
		if (!has)
		{
			buf[rel_len] = action_channel;
			m_skeleton_model->SetDominatingChannel(buf, rel_len + 1);
		}
	}
}

void CSkeletonModelEntity::remove_dominating_channel(int action_channel)
{
	if (m_skeleton_model)
	{
		const int BUF_LEN = 20;
		int buf[BUF_LEN] = {0};
		int rel_len = m_skeleton_model->GetDominatingChannel(buf, BUF_LEN);

		if (rel_len > 0)
		{
			bool has = false;
			int new_channels[BUF_LEN] = {0};
			int new_channels_idx = 0;
			for (int i = 0; i < rel_len; ++i)
			{
				if (buf[i] == action_channel)
				{
					has = true;
					continue;
				}

				new_channels[new_channels_idx++] = buf[i];
			}

			if (has)
			{
				if (rel_len > 1)
				{
					m_skeleton_model->SetDominatingChannel(new_channels, rel_len - 1);
				}
				else
				{
					m_skeleton_model->SetDominatingChannel(NULL, 0);
				}
			}
		}
	}
}

void CSkeletonModelEntity::do_update(float interval)
{	
	UpdateAttachItem();
	CModelEntity::do_update(interval);
	m_played_time += interval;
}

void CSkeletonModelEntity::add_set_actions(ANIMATION_CHANNEL_ID channel_id, const char* action, bool loop)
{
	m_action_id[channel_id] = action ? action : "";
	m_action_loop[channel_id] = loop;
}

void CSkeletonModelEntity::add_set_next_actions(ANIMATION_CHANNEL_ID channel_id, const char* action, bool loop)
{
	m_next_action_id[channel_id] = action ? action : "";
	m_next_action_loop[channel_id] = loop;
}

void CSkeletonModelEntity::reset()
{
	if (!m_skeleton_model)
		return;

	for (int i = 0; i < ANIMATION_CHANNEL_COUNT; ++i)
	{
		m_skeleton_model->GetAnmChannel(i)->SetAction(m_action_id[i].c_str(), m_action_loop[i]);
	}
}

int CSkeletonModelEntity::type()const
{
	return MEDIA_END_ENTITY_TYPE_SKELETON_MODEL;
}

bool CSkeletonModelEntity::is_kind_of(int obj_type)
{
	if(MEDIA_END_ENTITY_TYPE_SKELETON_MODEL == obj_type)
	{
		return true;
	}

	return CModelEntity::is_kind_of(obj_type);
}

void CSkeletonModelEntity::attach_model(const AttachItemInfo& info)
{
	if (!info.m_entity)
	{
		return;
	}

	if ( -1 != find_attach_info(info) )
	{
		return;
	}

	info.m_entity->set_visible(m_is_all_attach_item_visible);
	m_attach_item_info[info.m_info.m_bodyPart].push_back(info);
	AttachItemInfo& pInfo = m_attach_item_info[info.m_info.m_bodyPart].back();

	if (m_entity_state!=ES_NONE && m_entity_state!=ES_UNLOAD)
	{
		attach_model_immediate(pInfo);
	}
	else if (m_skeleton_model && pInfo.m_entity->get_is_create_sync() && pInfo.m_entity->get_IModel())
	{
		middleAssert(false && "Unload状态不能设置同步创建的配饰");
	}
}

void CSkeletonModelEntity::detach_model(const AttachItemInfo& info)
{
	AttachItemInfo* pInfo = get_attach_info(info.m_info.m_bodyPart, info.m_id);
	if (!pInfo)
	{
		return;
	}

	if (m_skeleton_model)
	{
		m_skeleton_model->DetachModel(pInfo->m_info);
	}

	if ( pInfo->m_entity )
	{
		stop_entity_op(pInfo->m_entity);
		m_res_mng->get_entity_factory()->release(pInfo->m_entity);
	}
	int pos = find_attach_info(*pInfo);
	if ( pos != -1 )
	{
		m_attach_item_info[pInfo->m_info.m_bodyPart].erase(m_attach_item_info[pInfo->m_info.m_bodyPart].begin() + pos);
	}
}


void CSkeletonModelEntity::deatch_bodypart_model(int pos)
{
	int engine_body_part = pos;
	if ( engine_body_part != -1 )
	{
		if ( m_attach_item_info.find(engine_body_part) != m_attach_item_info.end() )
		{
			for ( int i = 0 ; i < m_attach_item_info[engine_body_part].size() ; i++ )
			{
				if (m_skeleton_model)
				{
					m_skeleton_model->DetachModel(m_attach_item_info[engine_body_part][i].m_info);
				}

				if ( m_attach_item_info[engine_body_part][i].m_entity )
				{
					stop_entity_op(m_attach_item_info[engine_body_part][i].m_entity);
					m_res_mng->get_entity_factory()->release(m_attach_item_info[engine_body_part][i].m_entity);
				}
			}
			m_attach_item_info[engine_body_part].clear();
		}
	}
}

void CSkeletonModelEntity::remove_all_attached_model()
{
	for (auto iter=m_attach_item_info.begin(); iter!=m_attach_item_info.end(); ++iter)
	{
		for (unsigned int i=0; i<iter->second.size(); ++i)
		{
			if (m_skeleton_model)
			{
				m_skeleton_model->DetachModel((iter->second)[i].m_info);
			}

			if ( (iter->second)[i].m_entity )
			{
				stop_entity_op((iter->second)[i].m_entity);
				m_res_mng->get_entity_factory()->release((iter->second)[i].m_entity);
			}
		}
	}
	m_attach_item_info.clear();
}

void CSkeletonModelEntity::SetAllAttachedModelVisible(bool val)
{
	m_is_all_attach_item_visible = val;
	for (auto iter=m_attach_item_info.begin(); iter!=m_attach_item_info.end(); ++iter)
	{
		for (unsigned int i=0; i<iter->second.size(); ++i)
		{
			(iter->second)[i].m_entity->set_visible(val);
		}
	}
}

int CSkeletonModelEntity::find_attach_info(const AttachItemInfo& info)
{
	for ( int i = 0 ; i < m_attach_item_info[info.m_info.m_bodyPart].size(); i++ )
	{
		if ( (m_attach_item_info[info.m_info.m_bodyPart])[i].m_id == info.m_id )
		{
			return i;
		}
	}
	return -1;
}

const H3DMat4* CSkeletonModelEntity::GetCurMatrix(unsigned int nBone)
{
	if (m_skeleton_model && m_entity_state!=ES_UNLOAD)
	{
		return &(m_skeleton_model->GetCurMatrix(nBone));
	}
	return 0;
}

void CSkeletonModelEntity::getRootBonePos(float* pos)
{
	H3DI::ISkeleton* actorskeleton = GetSkeleton();
	if (actorskeleton)
	{
		const H3DMat4* mat= GetCurMatrix(actorskeleton->GetRootBone()->GetID());
		if (mat)
		{
			pos[0] = (*mat)[0].w;
			pos[1] = (*mat)[1].w;
			pos[2] = (*mat)[2].w;	
		}		 
	}
}

H3DI::ISkeleton* CSkeletonModelEntity::GetSkeleton()
{
	if (m_skeleton_model)
	{
		return m_skeleton_model->GetSkeleton();
	}
	return 0;
}

const H3DMat4* CSkeletonModelEntity::get_bone_matrix(const std::string& boneName)
{
	if (m_skeleton_model/* && m_entity_state!=ES_UNLOAD*/)
	{
		H3DI::IBone* bone = m_skeleton_model->GetSkeleton()->GetBone(boneName.c_str());
		if (bone)
		{
			return &(m_skeleton_model->GetRendingMatrix(bone->GetID()));
		}
	}
	return 0;
}

void CSkeletonModelEntity::set_actor_phy(bool is)
{
	m_actor_phy = is;
	if (m_skeleton_model)
	{
		m_skeleton_model->SetThisActorPhy(m_actor_phy);
	}
}

void CSkeletonModelEntity::update_skin_pose(int bpID,bool bUpdate)
{
	if (m_skeleton_model)
	{
		m_skeleton_model->UpdateSkinPose(bpID, bUpdate);
	}
}

void CSkeletonModelEntity::getSkeletonModelAppearance(SkeletonModelAppearance& appearance)
{
	appearance.geo_lod = m_lod_level;
	appearance.mat_lod = m_lod_level;
	strcpy(appearance.res_name, m_resource_name.c_str());
}

#define CHAR_TEMP_MAX_SIZE 256

static void utf8ToUnicode(const char* str, char* des)
{
	int len = ::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, NULL, 0 );
	assert(len <= CHAR_TEMP_MAX_SIZE);
	wchar_t* unicodeString = new wchar_t[ len ];
	::MultiByteToWideChar( CP_UTF8, 0, str, strlen( str ) + 1, unicodeString, len );

	len = ::WideCharToMultiByte( CP_ACP, 0, unicodeString, wcslen( unicodeString ) + 1,	NULL, 0, NULL, NULL );
	assert(len <= CHAR_TEMP_MAX_SIZE);
	::WideCharToMultiByte( CP_ACP, 0, unicodeString, wcslen( unicodeString ) + 1, des, len, NULL, NULL );

	delete[] unicodeString;
}

void CSkeletonModelEntity::initLightMapInfo(const char* path)
{
	if (!m_has_lightmap)
	{
		m_has_lightmap = true;

		BiXmlHelper::LoadXMLDocument(path);
		BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(path, true);
		if(doc)
		{
			BiXmlNode* lightmapNode = doc->FirstChild("LightMap");
			if(lightmapNode)//静态模型的LightMap属性
			{
				BiXmlNode* mapnode = lightmapNode->FirstChildElement("Map");
				while(mapnode)
				{
					int nMeshID = -1;
					ModelLightMap lightmap;
					for (BiXmlNode* mapchild = mapnode->FirstChild(); mapchild; mapchild = mapchild->NextSibling())
					{
						char map_property_key[CHAR_TEMP_MAX_SIZE];
						utf8ToUnicode(mapchild->Value(), map_property_key);//UTF8 NAME
						const char* map_inner_value = BiXmlUtility::get_value(mapchild->ToElement());

						if (stricmp(map_property_key, "MeshIndex") == 0)
						{
							nMeshID = atoi(map_inner_value);
						}
						if (stricmp(map_property_key, "Path") == 0)
						{
							lightmap.m_MapPath = map_inner_value;
						}
						if (stricmp(map_property_key, "Pos") == 0)
						{
							sscanf(map_inner_value, "(%f, %f)", &lightmap.m_CoordBias[0], &lightmap.m_CoordBias[1]);
						}
						if (stricmp(map_property_key, "Scale") == 0)
						{
							sscanf(map_inner_value, "(%f, %f)", &lightmap.m_CoordScale[0], &lightmap.m_CoordScale[1]);
						}
						if (stricmp(map_property_key, "ColorScale") == 0)
						{
							sscanf(map_inner_value, "(%f, %f, %f, %f)", &lightmap.m_clrScale[0], &lightmap.m_clrScale[1], &lightmap.m_clrScale[2], &lightmap.m_clrScale[3]);
						}
					}


					add_lightmapinfo(nMeshID, lightmap);
					mapnode = mapnode->NextSibling();
				}

				load_lightmap();
			}

			XmlDocFactory::ReleaseDocument(doc);
		}
		else
		{
			m_has_lightmap = false;
		}

		updateLightMap();
	}
}

void CSkeletonModelEntity::updateLightMap()
{
	if (m_has_lightmap || m_lightmaps.size())
	{
		if (m_lod_level == LL_ZERO)
		{
			cancle_lightmap();
			set_recive_fakelightspot(false);
		}
		else if (m_lod_level == LL_ONE)
		{
			apply_lightmap();
			set_recive_fakelightspot(get_recive_fakelightspot_record());
		}
		else if (m_lod_level == LL_TWO)
		{
			apply_lightmap();
			set_recive_fakelightspot(get_recive_fakelightspot_record());
		}
	}
}

void CSkeletonModelEntity::switch_to_lod(int lod, bool is_sync)
{
	CModelEntity::switch_to_lod(lod, is_sync);

	updateLightMap();
}

void CSkeletonModelEntity::clear_all_op()
{
	C3DEntity::clear_all_op();
	m_action_op = 0;
	m_next_action_op = 0;
}

void CSkeletonModelEntity::set_engine_object_impl(H3DI::IMoveObject* object)
{
	CModelEntity::set_engine_object_impl(object);
	m_skeleton_model = (H3DI::ISkeletonModel*)object;
}

void CSkeletonModelEntity::after_create_impl()
{
	if (m_skeleton_model)
	{
		CModelEntity::after_create_impl();

		set_fixfoot_transmode(ANIMATION_CHANNEL_ACTION, m_fix_foot);

		add_dominating_channel(ANIMATION_CHANNEL_EYES);
		add_dominating_channel(ANIMATION_CHANNEL_LEFT_HAND);
		add_dominating_channel(ANIMATION_CHANNEL_RIGHT_HAND);

		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_EYES)->SetAction("", true);
		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_LEFT_HAND)->SetAction("", true);
		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_RIGHT_HAND)->SetAction("", true);

		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_EYES)->SetTransitionTimeToNext(0);
		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_EYES)->SetTransitionTimeFromCurrent(0);

		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_LEFT_HAND)->SetTransitionTimeToNext(0);
		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_LEFT_HAND)->SetTransitionTimeFromCurrent(0);

		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_RIGHT_HAND)->SetTransitionTimeToNext(0);
		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_RIGHT_HAND)->SetTransitionTimeFromCurrent(0);

		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_ACTION)->SetTransitionTimeFromCurrent(m_transition_time_from_current);
		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_ACTION)->SetTransitionTimeToNext(m_transition_time_to_next);
		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_ACTION)->SetAction(
			m_action_id[ANIMATION_CHANNEL_ACTION].c_str(), m_action_loop[ANIMATION_CHANNEL_ACTION]);
		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_ACTION)->SetCurrActionTime(m_played_time/1000.f);
		m_skeleton_model->GetAnmChannel(ANIMATION_CHANNEL_ACTION)->SetNextAction(
			m_next_action_id[ANIMATION_CHANNEL_ACTION].c_str(), m_next_action_loop[ANIMATION_CHANNEL_ACTION]);

		m_skeleton_model->SetThisActorPhy(m_actor_phy);
	}
}

void CSkeletonModelEntity::UpdateAttachItem()
{
	std::map<int, std::vector<AttachItemInfo> >::iterator iter = m_attach_item_info.begin();
	for ( ; iter != m_attach_item_info.end() ; iter++ )
	{
		for ( int i = 0 ; i < iter->second.size(); i++ )
		{
			(iter->second)[i].m_entity->update_execute_op();
		}
	}
}

void CSkeletonModelEntity::UnloadAttachItem()
{
	for (auto iter=m_attach_item_info.begin(); iter!=m_attach_item_info.end(); ++iter)
	{
		for (unsigned int i=0; i<iter->second.size(); ++i)
		{
			(iter->second)[i].m_entity->UnloadResources();
			(iter->second)[i].m_info.m_attachModel = 0;
		}
	}
}

void CSkeletonModelEntity::ReloadAttachItem(bool is_sync)
{
	for (auto iter=m_attach_item_info.begin(); iter!=m_attach_item_info.end(); ++iter)
	{
		for (unsigned int i=0; i<iter->second.size(); ++i)
		{
			CModelEntity* entity = (iter->second)[i].m_entity;
			entity->ReloadResources(is_sync, m_priority);
			entity->m_is_create_sync = is_sync;
			attach_model_immediate((iter->second)[i]);
		}
	}
}

AttachItemInfo* CSkeletonModelEntity::get_attach_info(int bodypart, int id)
{
	for ( int i = 0 ; i < m_attach_item_info[bodypart].size(); i++ )
	{
		if ( (m_attach_item_info[bodypart])[i].m_id == id )
		{
			return &(m_attach_item_info[bodypart])[i];
		}
	}
	return 0;
}

void CSkeletonModelEntity::attach_model_immediate(AttachItemInfo& pInfo)
{
	//挂饰同步创建并且Entity之前没有异步操作则立即生效
	if (m_skeleton_model && pInfo.m_entity->get_is_create_sync() && pInfo.m_entity->get_IModel())
	{
		pInfo.m_info.m_attachModel = pInfo.m_entity->get_IModel();
		m_skeleton_model->AttachModel(pInfo.m_info);
	}
	else
	{
		//创建OP
		AttachOpParam param;
		param.bodypart = pInfo.m_info.m_bodyPart;
		param.id = pInfo.m_id;

		AttachOperation* pOp = new AttachOperation(m_is_view_sync, this, pInfo.m_entity, param);
		m_execute_op_list[m_priority].push_back(pOp);

		m_entity_op_map[pInfo.m_entity] = pOp;
	}
}