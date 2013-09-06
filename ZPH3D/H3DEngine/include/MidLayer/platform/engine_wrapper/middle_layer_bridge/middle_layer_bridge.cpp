// 这是主 DLL 文件。

#include "stdafx.h"

#include "middle_layer_bridge.h"

// 向外提供函数，可以创建一个stage
namespace ClientBridge
{

	static IPropertyModifierStreamable* s_find_actor_property_modifier(MStageEffect^ eff
		, DynamicEffectObject* dyneffobj, StageAnimationClipMng* clip_mng, PropertyModifierTypeStorage type);

#ifndef FRAME_PER_BAR
#define FRAME_PER_BAR 80.0f
#endif

	System::String^ to_string(const char* value)
	{
		System::String^ s = gcnew System::String(value);
		return s;
	}

	void trim_string(std::string& str)
	{
		if (str.empty())
		{
			return;
		}

		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
	}

	int compare_string(std::string str1, std::string str2)
	{
		trim_string(str1);
		trim_string(str2);
		return strcmp(str1.c_str(), str2.c_str());
	}

	LevelInfoMng::LevelInfoMng(AnimationScript* ani_script, StageAnimationClipMng* stage_ani_clip_mng)
	{
		//P_BEGIN(1);
		m_stage_ani_clip_mng = stage_ani_clip_mng;
		m_ani_script = ani_script;
		m_cutting_data_uniid_map = gcnew System::Collections::Generic::Dictionary<LevelInfoItem^, CuttingDataStorage>();
		//P_END(1);
	}

	LevelInfoMng::~LevelInfoMng()
	{
		m_cutting_data_uniid_map->Clear();
	}

	LevelInfoMng::!LevelInfoMng()
	{
		m_cutting_data_uniid_map->Clear();
	}

	bool LevelInfoMng::load(System::String^ filename, bool isBinary)
	{
		//P_BEGIN(2);
		m_cutting_data_uniid_map->Clear();
		bool r = m_ani_script->load(StringItemEx(filename), isBinary);

		// 轮询每个cuttingdata，建立场景切换数据容器
		m_ani_script->resetIterator();
		while(StageAnimationCuttingData* cutting = m_ani_script->next())
		{
			LevelInfoItem^ lii = cuttingDataToLevelInfoItem(cutting);

			CuttingDataStorage cds;
			cds.m_cutting_data = cutting;
			m_cutting_data_uniid_map->Add(lii, cds);
		}

		//P_END(2);
		return r;
	}

	bool LevelInfoMng::save(System::String^ filename, bool isBinary)
	{
		return m_ani_script->save(StringItemEx(filename), isBinary);
	}

	LevelInfoItem^ LevelInfoMng::addLevelInfo (System::String^ id, LevelInfoItem::TrackType tracktype, int begframe, int length)
	{
		//P_BEGIN(3);
		LevelInfoItem^ lii = nullptr;

		if(tracktype == LevelInfoItem::TrackType::TrackTypeCamera || tracktype == LevelInfoItem::TrackType::TrackTypeStageEffect)
		{
			StageAnimationCuttingData* cuttingdata = new StageAnimationCuttingData();
			cuttingdata->m_animation_clip_id = StringItemEx(id);
			cuttingdata->m_begin_bar = begframe / FRAME_PER_BAR;
			cuttingdata->m_end_bar = (begframe + length) / FRAME_PER_BAR;

			if(tracktype == LevelInfoItem::TrackType::TrackTypeCamera)
				cuttingdata->m_data_type = StageAnimationCuttingData_Camera;
			else if(tracktype == LevelInfoItem::TrackType::TrackTypeStageEffect)
				cuttingdata->m_data_type = StageAnimationCuttingData_Common;

			m_ani_script->insertStageAnimationCuttingData(cuttingdata);

			lii = cuttingDataToLevelInfoItem(cuttingdata);
			CuttingDataStorage cds;
			cds.m_cutting_data = cuttingdata;
			m_cutting_data_uniid_map->Add(lii, cds);

		}
		else
		{
			lii = gcnew LevelInfoItem();
			lii->m_begin_frame = begframe;
			lii->m_end_frame = begframe + length;
			lii->m_linked_id = id;
			lii->m_track_type = tracktype;
		}

		//P_END(3);
		return lii;
	}

	void LevelInfoMng::updateLevelInfo(LevelInfoItem^ item, int newbegframe, int newlength)
	{
		//P_BEGIN(4);
		item->m_begin_frame = newbegframe;
		item->m_end_frame = newbegframe + newlength;

		if(StageAnimationCuttingData* cuttingdata = findCuttingData(item))
		{
			cuttingdata->m_begin_bar = newbegframe / FRAME_PER_BAR;
			cuttingdata->m_end_bar = (newbegframe + newlength) / FRAME_PER_BAR;
		}
		//P_END(4);
	}

	void LevelInfoMng::updateLevelInfo(LevelInfoItem^ item, System::String^ newlinkid)
	{
		//P_BEGIN(5);
		//TODO 这里面存在一个bug，如果没有查找到对应的cuttingdata，就会出现内存访问违规
		StageAnimationCuttingData* cuttingdata = findCuttingData(item); 
		if (cuttingdata)
		{
			findCuttingData(item)->m_animation_clip_id = StringItemEx(newlinkid);
		}
		
		//P_END(5);
	}

	void LevelInfoMng::removeInfoItem(LevelInfoItem^ item)
	{
		//P_BEGIN(6);
		if(StageAnimationCuttingData* cuttingdata = findCuttingData(item))
			m_ani_script->removeStageAnimationCuttingData(cuttingdata);

		m_cutting_data_uniid_map->Remove(item);
		//P_END(6);
	}

	void LevelInfoMng::resetIter()
	{
		//P_BEGIN(7);
		m_ani_script->resetIterator();
		//P_END(7);
	}

	LevelInfoItem^ LevelInfoMng::getNextInfo()
	{
		//P_BEGIN(8);
		StageAnimationCuttingData* cutting = m_ani_script->next();

		if(!cutting)
		{
			//P_END(8);
			return nullptr;
		}
		else
		{
			System::Collections::Generic::Dictionary<LevelInfoItem^, CuttingDataStorage>::Enumerator enumerator = m_cutting_data_uniid_map->GetEnumerator();
			while(enumerator.MoveNext())
			{
				if(enumerator.Current.Value.m_cutting_data == cutting)
				{
					//P_END(8);
					return enumerator.Current.Key;
				}
			}
			//P_END(8);
			return nullptr;
		}
	}

	StageAnimationCuttingData* LevelInfoMng::findCuttingData(LevelInfoItem^ item)
	{
		//P_BEGIN(9);
		CuttingDataStorage findcds;
		findcds.m_cutting_data = 0;
		//TODO 这里存在一个bug，如果item值不在map内，那么就会引发一个异常
		m_cutting_data_uniid_map->TryGetValue(item, findcds);
		//P_END(9);
		return findcds.m_cutting_data;
	}

	LevelInfoItem::TrackType LevelInfoMng::getTrackType(StageAnimationCuttingData* cuttingdata)
	{
		switch(cuttingdata->m_data_type)
		{
		case StageAnimationCuttingData_Common:
			return LevelInfoItem::TrackType::TrackTypeStageEffect;
		case StageAnimationCuttingData_Camera:
			return LevelInfoItem::TrackType::TrackTypeCamera;
		default:
			return LevelInfoItem::TrackType::TrackTypeStageEffect;
		}
	}

	LevelInfoItem^ LevelInfoMng::cuttingDataToLevelInfoItem(StageAnimationCuttingData* cuttingdata)
	{
		//P_BEGIN(10);
		LevelInfoItem^ lii = gcnew LevelInfoItem();
		lii->m_begin_frame = cuttingdata->m_begin_bar * FRAME_PER_BAR;
		lii->m_end_frame = cuttingdata->m_end_bar * FRAME_PER_BAR;
		lii->m_linked_id = to_string(cuttingdata->m_animation_clip_id.c_str());
		lii->m_track_type = getTrackType(cuttingdata);
		//P_END(10);

		return lii;
	}

	AttributeSequenceEvaluator::AttributeSequenceEvaluator(ICalculator* calculator) : m_calculator(calculator)
	{
	}

	AttributeSequenceEvaluator::~AttributeSequenceEvaluator()
	{
	}

	AttributeSequenceEvaluator::!AttributeSequenceEvaluator()
	{
	}

	bool AttributeSequenceEvaluator::getKeyFrames(int sub_index, float frames[], int& buffer_size)
	{
		if(!m_calculator)
			return false;
		else
			return m_calculator->get_key_frames(sub_index, frames, buffer_size) ? true : false;
	}

	bool AttributeSequenceEvaluator::getInterpolatingType(int sub_index, float frame, INTERPOLATING_TYPE& type)
	{
		//P_BEGIN(11);
		if(!m_calculator)
		{
			//P_END(11);
			return false;
		}
		else
		{
			int t = m_calculator->get_interpolating_type(sub_index);

			if(t == INTERPOLATINGTYPE_POINT)
				type = AttributeSequenceEvaluator::INTERPOLATING_TYPE::INTERPOLATING_POINT;
			else if(t == INTERPOLATINGTYPE_LINE)
				type = AttributeSequenceEvaluator::INTERPOLATING_TYPE::INTERPOLATING_LINEAR;
			else if(t == INTERPOLATINGTYPE_BEZIER)
				type = AttributeSequenceEvaluator::INTERPOLATING_TYPE::INTERPOLATING_BEZIER;
			else
			{
				//P_END(11);
				return false;
			}

			//P_END(11);
			return true;
		}

	}

	System::String^ AttributeSequenceEvaluator::getInterpolatingValue(int sub_index, float frame)
	{
		if(!m_calculator)
			return nullptr;
		else
			return to_string(m_calculator->get_interpolating_value(sub_index, frame));
	}

	bool AttributeSequenceEvaluator::getInterpolatingValue(int sub_sequence_index, int length, CurvePoints^ curvePoints)
	{

		//P_BEGIN(12);
		if (m_calculator == 0)
		{
			//P_END(12);
			return false;
		}

		if (!curvePoints->ContainsKey(sub_sequence_index.ToString()))
		{
			curvePoints->Add(sub_sequence_index.ToString(), gcnew System::Collections::Generic::Dictionary<float, System::String^>());
		}

		float i = 0.0f;
		while (i <= length)
		{
			System::String^ val = getInterpolatingValue(sub_sequence_index, i);
			if (val->Length != 0)
			{
				curvePoints[sub_sequence_index.ToString()]->Add(i, val);
			}
			i += 0.1f;
		}

		//P_END(12);
		return true;
	}

	//StageInfoMng::StageInfoMng(Stage* stage, StageAnimationPlayer* ani_player)
	StageInfoMng::StageInfoMng(Stage* stage, StageAnimationPlayerEditor* ani_player)
	{
		//P_BEGIN(13);
		m_ani_player = ani_player;
		m_stage = stage;
		//m_editinfo_table = editinfo_table;
		m_removed_entities = gcnew System::Collections::Generic::List<MGraphicEntity^>();
		m_save_optimize = true;
		//P_END(13);
	}

	StageInfoMng::~StageInfoMng()
	{
		for each(MGraphicEntity^ entity in m_removed_entities)
		{
			delete entity->m_dyneffobj;
		}

		m_removed_entities->Clear();
	}

	StageInfoMng::!StageInfoMng()
	{
	}

	bool StageInfoMng::isRemovedEntityExists(MGraphicEntity^ e)
	{
		for each(MGraphicEntity^ entity in m_removed_entities)
		{
			if (entity->m_dyneffobj == e->m_dyneffobj)
			{
				return true;
			}
		}

		return false;
	}

	void StageInfoMng::addRemovedEntity(MGraphicEntity^ e)
	{
		
		if (!isRemovedEntityExists(e))
		{
			m_removed_entities->Add(e);
		}
	}

	void StageInfoMng::removeRemovedEntity(MGraphicEntity^ e)
	{
		//P_BEGIN(14);
		for each(MGraphicEntity^ entity in m_removed_entities)
		{
			if (entity->m_dyneffobj == e->m_dyneffobj)
			{
				m_removed_entities->Remove(entity);
				//P_END(14);
				return;
			}
		}
		//P_END(14);
	}

	bool StageInfoMng::switchToLod(int lod)
	{
		::LOD_LEVEL level_lod;
		switch(lod)
		{
		case LOD_LEVEL::LL_ZERO:
			level_lod = ::LL_ZERO;
			break;
		case LOD_LEVEL::LL_ONE:
			level_lod = ::LL_ONE;
			break;
		case LOD_LEVEL::LL_TWO:
			level_lod = ::LL_TWO;
			break;
		default:
			break;
		}

		return m_stage->switchToLod(level_lod);
	}

	System::Collections::Generic::List<MPrePassLight^>^ StageInfoMng::getAllLight()
	{
		System::Collections::Generic::List<MPrePassLight^>^ all_lights = gcnew System::Collections::Generic::List<MPrePassLight^>();
		ActorManager* actormng = m_stage->getActorMng();
		actormng->reset_iterator();
		Actor* actor = actormng->next();
		while(actor)
		{
			Actor::EntityList entitylist;
			if(actor->contain_entity_type_recursive(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT, entitylist))
			{
				Actor::EntityList::iterator it = entitylist.begin();
				for (; it != entitylist.end(); ++it)
				{
					CPrePassLightEntity* light = dynamic_cast<CPrePassLightEntity*>(*it);
					MPrePassLight^ mlight = gcnew MPrePassLight();
					mlight->m_light = light->get_IPrePassLight();
					mlight->m_lightmap_shadow_index = light->get_lightmap_shadow_index();
					mlight->m_lightmap_radius = light->get_lightmap_radius();
					all_lights->Add(mlight);
				}
				entitylist.clear();
			}

			actor = actormng->next();
		}
		
		return all_lights;
	}
	/// 读取预定制信息
	bool StageInfoMng::loadDefinedItem(System::String^ filename, bool isBinary)
	{
		return true;
		//return m_editinfo_table->load(StringItemEx(filename), isBinary);
	}
	/// 读取主文件并创建对应的内容
	bool StageInfoMng::loadStage (System::String^ pathname, System::String^ filename, bool isBinary)
	{
 		return m_stage->loadStageFile(StringItemEx(filename), isBinary);
	}
	/// 保存主文件
	bool StageInfoMng::saveStage (System::String^ pathname, System::String^ filename, bool isBinary)
	{
		return m_stage->saveStageFile(StringItemEx(pathname + filename), isBinary, m_save_optimize);
	}

	MStageEffect^ StageInfoMng::loadStageEffect(System::String^ filename, bool isBinary)
	{
		//P_BEGIN(15);
		StageAnimationClip* sac = m_ani_player->getClipManger()->createStageAnimationClip(StageAnimationClipType::StageAnimationClip_Common, "__loading_stage_clip");
		sac->load(StringItemEx(filename), isBinary);
		m_ani_player->getClipManger()->insert(sac);
		switchToLod(m_stage->getLod());
		//P_END(15);
		return gcnew MStageEffect(sac);
	}
	MCamera^ StageInfoMng::loadCamera(System::String^ filename, bool isBinary)
	{
		//P_BEGIN(16);
		//判断传入的配置文件类型是否正确
		std::string cliptype = StageAnimationClip::getFileClipType(StringItemEx(filename), isBinary);
		if (0 != compare_string(cliptype, "camera"))
			return nullptr;

		std::string camid = StageAnimationClip::getFileClipId(StringItemEx(filename), isBinary);

		DynamicEffectObject* dyneffobj  = m_stage->createDefinedObject(camid.c_str(), StringItemEx("x52_Camera"));
		MCamera^ cam = gcnew MCamera(dyneffobj, m_ani_player->getClipManger(), m_stage->getActorMng());
		m_stage->getActorMng()->add_actor(dyneffobj);

		StageAnimationClip* sac = m_ani_player->getClipManger()->createStageAnimationClip(StageAnimationClipType::StageAnimationClip_Camera, "__loading_stage_clip");
		sac->load(StringItemEx(filename), isBinary);
		m_ani_player->getClipManger()->insert(sac);
		sac->insert(dyneffobj);

		ActorAnimationControl* animation_control = dyneffobj->getAnimationControl();
		//P_END(16);

		return cam;
	}
	bool StageInfoMng::save(System::String^ filename, MCamera^ cam, bool isBinary)
	{
		return cam->m_clip_mng->find(cam->m_dyneffobj->get_name())->save(StringItemEx(filename), isBinary, true, m_save_optimize);
	}
	bool StageInfoMng::save(System::String^ filename, MStageEffect^ effect, bool isBinary)
	{
		ActorManager* actor_mng = m_stage->getActorMng();
		
		//删除所有的系统物体
		actor_mng->reset_some_sysactor_iter(::LL_ZERO);
		while(const char* stractor = actor_mng->next_some_sysactor_name(::LL_ZERO))
		{
			if(DynamicEffectObject* actor = effect->m_clip->findEffectObject(stractor))
				effect->m_clip->remove(actor, false);
		}
		actor_mng->reset_some_sysactor_iter(::LL_ONE);
		while(const char* stractor = actor_mng->next_some_sysactor_name(::LL_ONE))
		{
			if(DynamicEffectObject* actor = effect->m_clip->findEffectObject(stractor))
				effect->m_clip->remove(actor, false);
		}

		actor_mng->reset_some_sysactor_iter(::LL_TWO);
		while(const char* stractor = actor_mng->next_some_sysactor_name(::LL_TWO))
		{
			if(DynamicEffectObject* actor = effect->m_clip->findEffectObject(stractor))
				effect->m_clip->remove(actor, false);
		}

		::LOD_LEVEL lod = m_stage->getLod();
		actor_mng->remove_sysactor_from_actorlist(lod);

		//保存特效
		bool bRet =  effect->m_clip->save(StringItemEx(filename), isBinary, false, m_save_optimize);
		
		//将当前级别的系统物体添加回来
		actor_mng->add_sysactor_to_actorlist(lod);
		actor_mng->reset_some_sysactor_iter(lod);
		{
			while(const char* stractor = actor_mng->next_some_sysactor_name(lod))
			{
				if(DynamicEffectObject* actor = dynamic_cast<DynamicEffectObject*>(actor_mng->find_actor(stractor)))
				{
					if (actor->getAnimationControl()->find_animation_clip(effect->m_clip->getID()))
					{
						effect->m_clip->insert(actor);
					}
					
				}
			}
		}

		return bRet;
	}

	bool StageInfoMng::removeActorFromFile(System::String^ filename, System::String^ id)
	{
		return StageAnimationClip::removeDynamicEffectObjectFromFile(StringItemEx(filename), StringItemEx(id));
	}

	static void mergePropertyInfo(const std::list<::DefinedItem::ValueItem*>& property_list
		, DefinedItem^ csitem)
	{
		//P_BEGIN(17);
		for(std::list<::DefinedItem::ValueItem*>::const_iterator ppos = property_list.begin();
			ppos != property_list.end(); ++ppos)
		{
			DefinedValueItem defvi;
			defvi.m_v_type = to_string((*ppos)->m_v_type.c_str());
			defvi.m_value = to_string((*ppos)->m_value.c_str());
			defvi.m_caption = to_string((*ppos)->m_caption.c_str());
			defvi.m_min_value = to_string((*ppos)->m_min_value.c_str());
			defvi.m_max_value = to_string((*ppos)->m_max_value.c_str());
			defvi.m_property_modifier_type_storage.m_property_modifier_type = &((*ppos)->m_property_modifier_type);
			defvi.m_is_support_curve = (*ppos)->m_property_support.empty() || (*ppos)->m_property_support == "curve_only";

			csitem->m_property_list->Add(defvi);
		}
		//P_END(17);
	}

	/*static void mergePropertyInfoByOrder(const std::list<std::string>& property_order_recoder
		, const std::map<std::string, ::DefinedItem::ValueItem>& property_list, DefinedItem^ csitem)
	{
		for (std::list<std::string>::const_iterator it = property_order_recoder.begin(); 
			it != property_order_recoder.end(); ++it)
		{
			std::map<std::string, ::DefinedItem::ValueItem>::const_iterator findpos = property_list.find(*it);
			if(findpos == property_list.end())
				continue;

			const ::DefinedItem::ValueItem* defvalueitem = &(findpos->second);

			DefinedValueItem defvi;
			defvi.m_v_type = to_string(defvalueitem->m_v_type.c_str());
			defvi.m_value = to_string(defvalueitem->m_value.c_str());
			defvi.m_min_value = to_string(defvalueitem->m_min_value.c_str());
			defvi.m_max_value = to_string(defvalueitem->m_max_value.c_str());
			defvi.m_caption = to_string(defvalueitem->m_caption.c_str());
			defvi.m_property_modifier_type_storage.m_property_modifier_type = &(defvalueitem->m_property_modifier_type);
			defvi.m_is_support_curve = defvalueitem->m_property_support.empty() || defvalueitem->m_property_support == "curve_only";

			csitem->m_property_list->Add(defvi);
		}
	}*/

	static DefinedItem^ toDefinedItem(const ::DefinedItem* item, bool rootlevel)
	{
	//	//P_BEGIN(18);
		DefinedItem^ csitem = gcnew DefinedItem();
		csitem->m_property_list = gcnew System::Collections::Generic::List<DefinedValueItem>();
		csitem->m_sub_item = gcnew System::Collections::Generic::List<DefinedItem^>();

		for(std::list<::DefinedItem*>::const_iterator pos = item->m_sub_item.begin();
			pos != item->m_sub_item.end(); ++pos)
		{
			if(rootlevel && pos == item->m_sub_item.begin())
			{
				continue;
			}
			else
			{
				DefinedItem^ subitem = toDefinedItem(*pos, false);
				csitem->m_sub_item->Add(subitem);
			}
		}

		/*if (item->m_property_order_recoder.size() > 0)
			mergePropertyInfoByOrder(item->m_property_order_recoder, item->m_property_list, csitem);
		else*/
			mergePropertyInfo(item->m_property_list, csitem);

		if(rootlevel && !item->m_sub_item.empty())
		{
			/*if(!item->m_sub_item.front()->m_property_order_recoder.empty())
				mergePropertyInfoByOrder(item->m_sub_item.front()->m_property_order_recoder, item->m_sub_item.front()->m_property_list, csitem);
			else*/
				mergePropertyInfo(item->m_sub_item.front()->m_property_list, csitem);
		}

		csitem->m_id = to_string(item->m_id.c_str());
		csitem->m_type = to_string(item->m_item_type.c_str());
		csitem->m_name = to_string(item->m_id.c_str());
		csitem->m_defined_type = to_string(item->m_item_type.c_str());

	//	//P_END(18);
		return csitem;
	}


	DefinedItem^ StageInfoMng::getDefinedInfo()
	{
		//P_BEGIN(19);
		//const CDefinedItemCollection* defitemcollections = static_cast<StageAnimationPlayer*>(m_stage->getPlayer())->getDefinedItemCollection();
		const CDefinedItemCollection* defitemcollections = static_cast<StageAnimationPlayerEditor*>(m_stage->getPlayer())->getDefinedItemCollection();
		const std::map<std::string, ::DefinedItem*>& defitemmap = defitemcollections->getDefinedItemMap();

		DefinedItem^ rootitem = gcnew DefinedItem();
		rootitem->m_sub_item = gcnew System::Collections::Generic::List<DefinedItem^>();

		for(std::map<std::string, ::DefinedItem*>::const_iterator pos = defitemmap.begin();
			pos != defitemmap.end(); ++pos)
		{
			const std::string& deftype = pos->first;
			const ::DefinedItem* defitem = pos->second;

			rootitem->m_sub_item->Add(toDefinedItem(defitem, true));
		}

		//P_END(19);
		return rootitem;
	}

	void StageInfoMng::resetActorIter()
	{
		m_stage->getActorMng()->reset_iterator();
	}
	MGraphicEntity^ StageInfoMng::getNextActor()
	{
		DynamicEffectObject* dyneffobj = static_cast<DynamicEffectObject*>(m_stage->getActorMng()->next());
		if(!dyneffobj)
			return nullptr;
		else
		{
			MGraphicEntity^ entity = gcnew MGraphicEntity(dyneffobj, m_ani_player->getClipManger(), m_stage->getActorMng());
			return entity;
		}
	}
	void StageInfoMng::resetStageEffectIter()
	{
		//P_BEGIN(20);
		m_ani_player->getClipManger()->resetIterator();
		//P_END(20);
	}
	MStageEffect^ StageInfoMng::getNextStageEffect()
	{
		StageAnimationClip* clip = m_ani_player->getClipManger()->next();
		if(!clip)
			return nullptr;
		else
		{
			MStageEffect^ stageeff = gcnew MStageEffect(clip);
			return stageeff;
		}
	}
	void StageInfoMng::resetCameraIter()
	{
		m_stage->getActorMng()->reset_iterator();
	}
	MCamera^ StageInfoMng::getNextCamera()
	{
		DynamicEffectObject* dyneffobj = static_cast<DynamicEffectObject*>(m_stage->getActorMng()->next());
		
		while(dyneffobj) 
		{
			if (stricmp(dyneffobj->getDefinedType(),"x52_Camera") == 0)
			{
				MCamera^ cam = gcnew MCamera(dyneffobj, m_ani_player->getClipManger(), m_stage->getActorMng());
				return cam;
			}

			dyneffobj = static_cast<DynamicEffectObject*>(m_stage->getActorMng()->next());
		}

		return nullptr;
	}

	MCamera^ StageInfoMng::getCamera(System::String^ name)
	{
		m_stage->getActorMng()->reset_iterator();
		DynamicEffectObject* dyneffobj = static_cast<DynamicEffectObject*>(m_stage->getActorMng()->next());

		while(dyneffobj) 
		{
			if (stricmp(dyneffobj->getDefinedType(),"x52_Camera") == 0 &&
				stricmp(dyneffobj->get_name(), StringItemEx(name)) == 0)
			{
				MCamera^ cam = gcnew MCamera(dyneffobj, m_ani_player->getClipManger(), m_stage->getActorMng());
				return cam;
			}

			dyneffobj = static_cast<DynamicEffectObject*>(m_stage->getActorMng()->next());
		}

		dyneffobj = m_ani_player->getClipManger()->getCommonCamera(StringItemEx(name));	

		if (dyneffobj)
		{
			MCamera^ cam = gcnew MCamera(dyneffobj, m_ani_player->getClipManger(), m_stage->getActorMng());
			return cam;
		}

		return nullptr;
	}

	/// 添加预定制的效果物品
	MGraphicEntity^ StageInfoMng::createDefinedActor(System::String^  name, System::String^  definedtype, System::String^  info)
	{
		//P_BEGIN(21);
		DynamicEffectObject* obj  = m_stage->createDefinedObject(StringItemEx(name), StringItemEx(definedtype));
		MGraphicEntity^ entity = gcnew MGraphicEntity(obj, m_ani_player->getClipManger(), m_stage->getActorMng());
		//P_END(21);
		return entity;
	}

	/// 添加最高层的效果物品
	void StageInfoMng::addActor(MGraphicEntity^ actor)
	{
		//P_BEGIN(22);
		::LOD_LEVEL lod = ActorManager::isLodSystemActorName(actor->m_dyneffobj->get_name());
		if (lod == LL_NULL || lod == m_stage->getLod())
		{
			m_stage->getActorMng()->add_actor(actor->m_dyneffobj);
		}
		else
		{
			m_stage->getActorMng()->addSystemActor(actor->m_dyneffobj->get_name(), actor->m_dyneffobj);
		}

		removeRemovedEntity(actor);

		ActorAnimationControl* ctrl = actor->m_dyneffobj->getAnimationControl();
		StageAnimationClipMng* cmg = m_ani_player->getClipManger();
		cmg->resetIterator();
		while(StageAnimationClip* clip = cmg->next())
		{
			if (ctrl->find_animation_clip(clip->getID()))
			{
				clip->insert(actor->m_dyneffobj);
			}
		}
		//P_END(22);
	}
	/// 删除最高层的效果物品
	void StageInfoMng::removeActor(MGraphicEntity^ actor)
	{
		//P_BEGIN(23);
		Actor* to_be_removed_actor = m_stage->getActorMng()->find_actor(actor->m_dyneffobj->get_name());
		Actor* to_be_removed_sys_actor = NULL;
		if (NULL == to_be_removed_actor)
		{//如果actor不在ActorMng内
			to_be_removed_sys_actor = m_stage->getActorMng()->getSystemActor(actor->m_dyneffobj->get_name());
			if (NULL == to_be_removed_sys_actor)
				return;
		}
		
		//if (m_stage->getActorMng()->find_actor(actor->m_dyneffobj->get_name()) != actor->m_dyneffobj)
		//{//如果actor不在ActorMng内
		//	//P_END(23);
		//	return;
		//}

		// 去掉所有正在play的内容
		m_ani_player->playDynamicEffectObject(actor->m_dyneffobj, false);
		
		// 从所有clip中去掉
		StageAnimationClipMng* acm = m_ani_player->getClipManger();
		acm->resetIterator();
		while(StageAnimationClip* clip = acm->next())
		{
			clip->remove(actor->m_dyneffobj, false);
		}
		//m_stage->getActorMng()->remove_actor(actor->m_dyneffobj);
		if (NULL == to_be_removed_actor)
		{
			m_stage->getActorMng()->removeSystemActor(to_be_removed_sys_actor->get_name());
		}
		else
		{
			m_stage->getActorMng()->remove_actor(actor->m_dyneffobj);
		}
		addRemovedEntity(actor);
		//P_END(23);
	}

	/// 添加舞台效果
	MStageEffect^ StageInfoMng::addStageEffect(System::String^ name, int framecount, System::String^ type, System::String^ info)
	{
		//P_BEGIN(24);
		StageAnimationClip* clip = m_ani_player->getClipManger()->createStageAnimationClip(StageAnimationClip_Common, StringItemEx(name));
		m_ani_player->getClipManger()->insert(clip);
		clip->setFrameCount(framecount);

		//P_END(24);
		return gcnew MStageEffect(clip);
	}
	/// 删除舞台效果
	void StageInfoMng::removeStageEffect(MStageEffect^ eff)
	{
	    //P_BEGIN(25);
		m_ani_player->playStageAnimationClip(eff->m_clip->getID(), false);
		m_ani_player->getClipManger()->remove(eff->m_clip);
		m_ani_player->getClipManger()->releaseStageAnimationClip(eff->m_clip);
		//P_END(25);
	}


	MCamera^ StageInfoMng::addCamera (System::String^ name, System::String^ type, System::String^ info)
	{
		//P_BEGIN(26);
		DynamicEffectObject* dyneffobj  = m_stage->createDefinedObject(StringItemEx(name), StringItemEx("x52_Camera"));
		MCamera^ cam = gcnew MCamera(dyneffobj, m_ani_player->getClipManger(), m_stage->getActorMng());

		m_stage->getActorMng()->add_actor(dyneffobj);

		// todo
		ActorAnimationControl* animation_control = dyneffobj->getAnimationControl();
		animation_control->add_animation_clip(StringItemEx(name), new ActorAnimationClip());

		// 创建一个舞台效果与这个相机相匹配
		StageAnimationClip* clip = m_ani_player->getClipManger()->createStageAnimationClip(StageAnimationClip_Camera, StringItemEx(name));
		m_ani_player->getClipManger()->insert(clip);
		clip->insert(cam->m_dyneffobj);

		//P_END(26);
		return cam;
	}

	void StageInfoMng::removeCamera(MCamera^ cam)
	{
		//P_BEGIN(27);
		StageAnimationClip* clip = m_ani_player->getClipManger()->find(cam->m_dyneffobj->get_name());

		// 删除与之对应的舞台效果
		m_ani_player->playStageAnimationClip(clip->getID(), false);
		m_ani_player->getClipManger()->remove(clip);
		m_ani_player->getClipManger()->releaseStageAnimationClip(clip);

		// 删除相机本身
		m_stage->getActorMng()->remove_actor(cam->m_dyneffobj);
		//P_END(27);
		// todo
		// m_stage->getActorMng(); 
	}

	/// 管理物品拓扑关系
	void StageInfoMng::link(MGraphicEntity^ parententity, MGraphicEntity^ subentity)
	{
		//P_BEGIN(28);
		if (parententity->m_dyneffobj == subentity->m_dyneffobj->get_parent())
		{
			//P_END(28);
			return;
		}

		//将子Actor从ActorManager中remove
		removeActor(subentity);
		removeRemovedEntity(subentity);
		parententity->m_dyneffobj->addSubObject(subentity->m_dyneffobj);
		
		updateLinkEntityAttribute(parententity, subentity, true);
		//P_END(28);		
	}
	void StageInfoMng::unlink(MGraphicEntity^ parententity, MGraphicEntity^ entity)
	{
		//P_BEGIN(29);
		if (parententity->m_dyneffobj != entity->m_dyneffobj->get_parent())
		{
			//P_END(29);
			return;
		}

		parententity->m_dyneffobj->removeSubObject(entity->m_dyneffobj);
		addActor(entity);

		updateLinkEntityAttribute(parententity, entity, false);
		//P_END(29);
	}

	void StageInfoMng::updateLinkEntityAttribute(MGraphicEntity^ parent, MGraphicEntity^ entity, bool islink)
	{
		//P_BEGIN(30);
		//StageAnimationClipMng* clip_mng = ((StageAnimationPlayer*)(m_stage->getPlayer()))->getClipManger();
		StageAnimationClipMng* clip_mng = ((StageAnimationPlayerEditor*)(m_stage->getPlayer()))->getClipManger();

		clip_mng->resetIterator();

		PropertyModifierType pmptr;
		pmptr.is_curv = false;
		PropertyModifierTypeStorage pmts;
		pmts.m_property_modifier_type = &pmptr;

		H3DVec3 pos = entity->m_dyneffobj->position();
		pmptr.property_name = "Position";
		System::String^ value = to_string(GameGraphicHelper::value2Str<H3DVec3>(pos).c_str());
		entity->setAttributeInfo(pmts, value, nullptr);

		H3DVec3 rot = entity->m_dyneffobj->rotation();
		pmptr.property_name = "Rotation";
		value = to_string(GameGraphicHelper::value2Str<H3DVec3>(rot).c_str());
		entity->setAttributeInfo(pmts, value, nullptr);

		H3DVec3 scale = entity->m_dyneffobj->get_scale();
		pmptr.property_name = "Scale";
		value = to_string(GameGraphicHelper::value2Str<H3DVec3>(scale).c_str());
		entity->setAttributeInfo(pmts, value, nullptr);

		//获取绝对位置
		const H3DMat4& mat = entity->m_dyneffobj->get_world_location_matrix();
		H3DQuat quat;
		mat.Decompose(scale, quat, pos);

		quat = quat.Inverse();
		H3DAngles angle = quat.ToAngles();						
		rot = H3DVec3(DEG2RAD(angle.roll), DEG2RAD(angle.pitch), DEG2RAD(angle.yaw));

		while (StageAnimationClip* clip = clip_mng->next())
		{
			MStageEffect^ eff = gcnew MStageEffect(clip);

			pmptr.is_curv = false;
			pmptr.property_name = "Position";
			IPropertyModifierStreamable* pos_pm = s_find_actor_property_modifier(eff, entity->m_dyneffobj, entity->m_clip_mng, pmts);
			H3DVec3 eff_pos = (pos_pm == 0) ? pos : GameGraphicHelper::str2Value<H3DVec3>(pos_pm->get_value());

			pmptr.property_name = "Rotation";
			IPropertyModifierStreamable* rot_pm = s_find_actor_property_modifier(eff, entity->m_dyneffobj, entity->m_clip_mng, pmts);
			H3DVec3 eff_rot = (rot_pm == 0) ? rot : GameGraphicHelper::str2Value<H3DVec3>(rot_pm->get_value());
			H3DAngles eff_angle(RAD2DEG(eff_rot.y), RAD2DEG(eff_rot.z), RAD2DEG(eff_rot.x));
			H3DQuat eff_quat = eff_angle.ToQuat();
			eff_quat = eff_quat.Inverse();

			pmptr.property_name = "Scale";
			IPropertyModifierStreamable* scale_pm = s_find_actor_property_modifier(eff, entity->m_dyneffobj, entity->m_clip_mng, pmts);
			H3DVec3 eff_scale = (scale_pm == 0) ? scale : GameGraphicHelper::str2Value<H3DVec3>(scale_pm->get_value());

			H3DMat4 new_mat;
			new_mat.Compose(eff_scale, eff_quat, eff_pos);
			if (islink)
			{
				new_mat = parent->m_dyneffobj->get_world_location_matrix().Inverse() * new_mat;
			}
			else
			{
				new_mat = parent->m_dyneffobj->get_world_location_matrix() * new_mat;
			}
			new_mat.Decompose(eff_scale, eff_quat, eff_pos);

			eff_quat = eff_quat.Inverse();
			angle = eff_quat.ToAngles();						
			eff_rot = H3DVec3(DEG2RAD(angle.roll), DEG2RAD(angle.pitch), DEG2RAD(angle.yaw));



			if (pos_pm != 0) //设置位置
			{
				pos_pm->set_value(GameGraphicHelper::value2Str<H3DVec3>(eff_pos).c_str());
			}
			if (rot_pm != 0) //设置旋转
			{
				rot_pm->set_value(GameGraphicHelper::value2Str<H3DVec3>(eff_rot).c_str());
			}
			if (scale_pm != 0) //设置缩放
			{
				scale_pm->set_value(GameGraphicHelper::value2Str<H3DVec3>(eff_scale).c_str());
			}

			//更新位置的属性序列
			pmptr.is_curv = true;
			pmptr.property_name = "Position";
			AttributeSequenceEvaluator^ calc = entity->getAttributeSeqEvaluator(eff, pmts);
			if (calc->m_calculator)
			{
				for (int i = 0; i < 3; i++)
				{
					float value = eff_pos.ToFloatPtr()[i];
					int buffersize = 0;
					calc->getKeyFrames(i, NULL, buffersize);
					if (buffersize > 0)
					{
						float* buf = new float[buffersize + 1];
						calc->getKeyFrames(i, buf, buffersize);
						System::String^ interVal = calc->getInterpolatingValue(i, buf[0]);
						float val = GameGraphicHelper::str2Value<float>((const char*)(StringItemEx(interVal)));
						val = value - val;

						for (int j = 0; j < buffersize; ++j)
						{
							System::String^ interVal = calc->getInterpolatingValue(i, buf[j]);
							float p = GameGraphicHelper::str2Value<float>((const char*)(StringItemEx(interVal)));
							p = p + val;
							calc->m_calculator->set_key_frame_value(i, buf[j], GameGraphicHelper::value2Str<float>(p).c_str());
						}

						delete buf;
					}
				}
			}

			//更新旋转的属性序列
			pmptr.property_name = "Rotation";
			calc = entity->getAttributeSeqEvaluator(eff, pmts);
			if (calc->m_calculator)
			{
				for (int i = 0; i < 3; i++)
				{
					float value = eff_rot.ToFloatPtr()[i];
					int buffersize = 0;
					calc->getKeyFrames(i, NULL, buffersize);
					if (buffersize > 0)
					{
						float* buf = new float[buffersize + 1];
						calc->getKeyFrames(i, buf, buffersize);
						System::String^ interVal = calc->getInterpolatingValue(i, buf[0]);
						float val = GameGraphicHelper::str2Value<float>((const char*)(StringItemEx(interVal)));
						val = value - val;

						for (int j = 0; j < buffersize; ++j)
						{
							System::String^ interVal = calc->getInterpolatingValue(i, buf[j]);
							float p = GameGraphicHelper::str2Value<float>((const char*)(StringItemEx(interVal)));
							p = p + val;
							calc->m_calculator->set_key_frame_value(i, buf[j], GameGraphicHelper::value2Str<float>(p).c_str());
						}

						delete buf;
					}
				}
			}

			//更新缩放的属性序列
			pmptr.property_name = "Scale";
			calc = entity->getAttributeSeqEvaluator(eff, pmts);
			if (calc->m_calculator)
			{
				for (int i = 0; i < 3; i++)
				{
					float value = eff_scale.ToFloatPtr()[i];
					int buffersize = 0;
					calc->getKeyFrames(i, NULL, buffersize);
					if (buffersize > 0)
					{
						float* buf = new float[buffersize + 1];
						calc->getKeyFrames(i, buf, buffersize);
						System::String^ interVal = calc->getInterpolatingValue(i, buf[0]);
						float val = GameGraphicHelper::str2Value<float>((const char*)(StringItemEx(interVal)));
						val = value - val;

						for (int j = 0; j < buffersize; ++j)
						{
							System::String^ interVal = calc->getInterpolatingValue(i, buf[j]);
							float p = GameGraphicHelper::str2Value<float>((const char*)(StringItemEx(interVal)));
							p = p + val;
							calc->m_calculator->set_key_frame_value(i, buf[j], GameGraphicHelper::value2Str<float>(p).c_str());
						}

						delete buf;
					}
				}
			}
		} 
		//P_END(30);
	}


	System::Collections::Generic::List<MGraphicEntity^>^ StageInfoMng::getLinkedEntities(MGraphicEntity^ parententity)
	{
		//P_BEGIN(31);
		System::Collections::Generic::List<MGraphicEntity^>^ linked_entities = nullptr;

		if (parententity->m_dyneffobj)
		{
			linked_entities = gcnew System::Collections::Generic::List<MGraphicEntity^>();
			parententity->m_dyneffobj->resetSubObjectIterator();

			while (DynamicEffectObject* obj = parententity->m_dyneffobj->nextSubObject())
			{
				MGraphicEntity^ entity = gcnew MGraphicEntity(obj, m_ani_player->getClipManger(), m_stage->getActorMng());
				linked_entities->Add(entity);
			}
		}
		//P_END(31);
		return linked_entities;
	}

	System::String^ StageInfoMng::getActorNameById(unsigned int id)
	{
		const char* actor_name = m_stage->getActorMng()->getTopActorNameById(id);
		return to_string(actor_name);
	}

	void StageInfoMng::setSaveOpimize(bool optimize)
	{
		m_save_optimize = optimize;
	}


	MGraphicEntity::MGraphicEntity(DynamicEffectObject* dyneffobj, StageAnimationClipMng* clip_mng
		, ActorManager* actormng)
		: m_dyneffobj(dyneffobj), m_clip_mng(clip_mng), m_actor_manager(actormng)
	{
	}

	//获得动态特效物体默认舞台属性集中的某个属性
	static IPropertyModifierStreamable* s_find_actor_stage_default_property_modifier(DynamicEffectObject* dyneffobj
		, StageAnimationClipMng* clip_mng, PropertyModifierTypeStorage type)
	{
		ActorAnimationControl* animation_control = dyneffobj->getAnimationControl();

		ActorAnimationClip* actoraniclip = animation_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
		return static_cast<IPropertyModifierStreamable*>(actoraniclip->find_property_modifier(*type.m_property_modifier_type));
	}

	//获得动态特效物体默认属性集中的某个属性
	static IPropertyModifierStreamable* s_find_actor_default_property_modifier(DynamicEffectObject* dyneffobj
		, StageAnimationClipMng* clip_mng, PropertyModifierTypeStorage type)
	{
		ActorAnimationControl* animation_control = dyneffobj->getAnimationControl();
		ActorAnimationClip* actoraniclip = animation_control->find_animation_clip(DynamicEffectObject::getDefaultPropertyName());
		return static_cast<IPropertyModifierStreamable*>(actoraniclip->find_property_modifier(*type.m_property_modifier_type));
	}

	//获得动态特效物体在某个舞台效果中的属性集(如果不存在的话，返回默认舞台效果的属性集)
	static ActorAnimationClip* find_actor_animation_clip(MStageEffect^ eff
		, DynamicEffectObject* dyneffobj, StageAnimationClipMng* clip_mng)
	{
		ActorAnimationControl* animation_control =  dyneffobj->getAnimationControl();

		ActorAnimationClip* actoraniclip = 0;
		if(eff == nullptr)
			actoraniclip = animation_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
		else
			actoraniclip = animation_control->find_animation_clip(eff->m_clip->getID());

		return actoraniclip;
	}

	/*获得动态特效物体dyneffobj，在舞台效果eff中，的type属性
	*/
	static IPropertyModifierStreamable* s_find_actor_property_modifier(MStageEffect^ eff
		, DynamicEffectObject* dyneffobj, StageAnimationClipMng* clip_mng, PropertyModifierTypeStorage type)
	{
		if(ActorAnimationClip* actoraniclip = find_actor_animation_clip(eff, dyneffobj, clip_mng))
			return static_cast<IPropertyModifierStreamable*>(actoraniclip->find_property_modifier(*type.m_property_modifier_type));
		else
			return nullptr;
	}

	void MGraphicEntity::setAttributeInfo(PropertyModifierTypeStorage type, System::String^ value, MStageEffect^ eff)
	{
		//P_BEGIN(32);
		IPropertyModifierStreamable* pm = s_find_actor_property_modifier(eff, m_dyneffobj, m_clip_mng, type);

		//值为空则是从某个舞台效果中删除某个属性
		if ((nullptr == value) || (value->Length == 0))
		{
			if (0 != pm)
			{
				//将属性从该舞台效果中删除
				if(ActorAnimationClip* actoraniclip = find_actor_animation_clip(eff, m_dyneffobj, m_clip_mng))
				{
					actoraniclip->remove_property_modifier(*type.m_property_modifier_type);
					m_clip_mng->getStage()->getActorMng()->getPropertyModifierFactory()->releaseModifier(pm);
					pm = 0;
				}
			}
			//P_END(32);
			return;
		}

		//属性不在这个舞台效果中，则添加
		if(pm == 0)
		{
			pm = static_cast<IPropertyModifierStreamable*>(m_actor_manager->getPropertyModifierFactory()->create_modifier(m_dyneffobj, *type.m_property_modifier_type));
			ActorAnimationClip* actoraniclip = find_actor_animation_clip(eff, m_dyneffobj, m_clip_mng);
			//eff为空设置的为舞台默认属性，如果不为空则设置的是eff中的属性
			if(eff != nullptr)
			{
				
				eff->m_clip->insert(this->m_dyneffobj);
				if (!m_actor_manager->find_actor(this->m_dyneffobj->get_name()))
				{
					eff->m_clip->remove(this->m_dyneffobj, false);
				}
				actoraniclip = find_actor_animation_clip(eff, m_dyneffobj, m_clip_mng);
			}
			
			assert(actoraniclip);

			actoraniclip->add_property_modifier(*type.m_property_modifier_type, pm);
		}
		assert(pm);

		//设置属性值并生效
		pm->set_value(StringItemEx(value));

		pm->execute(true, 0);

		/*
		如果Actor不在Clip之中，且Actor在ActorManager中时，
		将其添加到clip中
		*/
		if ((nullptr != eff) && 
			(!eff->m_clip->findEffectObject(m_dyneffobj->get_name())) &&
			m_actor_manager->find_actor(this->m_dyneffobj->get_name())
			)
		{
			eff->m_clip->insert(m_dyneffobj);
		}
		//P_END(32);

	}

	void MGraphicEntity::setAbsAttributeInfo(PropertyModifierTypeStorage type, System::String^ value, MStageEffect^ eff)
	{
		//P_BEGIN(33);
		if (m_dyneffobj->get_parent()
			&& (type.m_property_modifier_type->child_entity_id.length() == 0)				
			&& (!type.m_property_modifier_type->is_curv)
			)
		{
			if ((System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Position") == 0)
				|| (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Rotation") == 0)
				|| (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Scale") == 0)
				)
			{
				if (m_dyneffobj == GameGraphicHelper::find_actor(m_dyneffobj, type.m_property_modifier_type->child_actor_id))
				{
					const H3DMat4& mat = m_dyneffobj->get_world_location_matrix();
					H3DVec3 pos;
					H3DVec3 scale;
					H3DQuat quat;
					mat.Decompose(scale, quat, pos);
					H3DVec3 val = GameGraphicHelper::str2Value<H3DVec3>((const char*)(StringItemEx(value)));
					if (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Position") == 0)
					{
						pos = val;
					}
					else if (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Rotation") == 0)
					{
						H3DVec3 rot = val;

						H3DAngles angle(RAD2DEG(rot.y), RAD2DEG(rot.z), RAD2DEG(rot.x));
						quat = angle.ToQuat();
						quat = quat.Inverse();
					}
					else
					{
						scale = val;
					}

					H3DMat4 new_mat;
					new_mat.Compose(scale, quat, pos);

					new_mat = m_dyneffobj->get_parent()->get_world_location_matrix().Inverse() * new_mat;
					new_mat.Decompose(scale, quat, pos);

					if (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Position") == 0)
					{
						value = to_string(GameGraphicHelper::value2Str<H3DVec3>(pos).c_str());
					}
					else if (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Rotation") == 0)
					{
						quat = quat.Inverse();
						H3DAngles angle = quat.ToAngles();						
						H3DVec3 rot(DEG2RAD(angle.roll), DEG2RAD(angle.pitch), DEG2RAD(angle.yaw));

						value = to_string(GameGraphicHelper::value2Str<H3DVec3>(rot).c_str());
					}
					else
					{
						value = to_string(GameGraphicHelper::value2Str<H3DVec3>(scale).c_str());
					}
				}
			}
		}

		setAttributeInfo(type, value, eff);
		//P_END(33);
	}

	System::String^ MGraphicEntity::getAbsAttributeInfo(PropertyModifierTypeStorage type, MStageEffect^ eff)
	{
		//P_BEGIN(34);
		if (m_dyneffobj->get_parent()
			&& (type.m_property_modifier_type->child_entity_id.length() == 0)				
			&& (!type.m_property_modifier_type->is_curv)
			)
		{
			if ((System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Position") == 0)
				|| (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Rotation") == 0)
				|| (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Scale") == 0)
				)
			{
				if (m_dyneffobj == GameGraphicHelper::find_actor(m_dyneffobj, type.m_property_modifier_type->child_actor_id))
				{
					//NOTICE 现在只假设在当前舞台效果下获取子物体的绝对路径，不支持跨舞台效果调用。如果调用，这里会存在一个bug
					const H3DMat4& mat = m_dyneffobj->get_world_location_matrix();
					H3DVec3 pos;
					H3DVec3 scale;
					H3DQuat quat;
					mat.Decompose(scale, quat, pos);
					if (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Position") == 0)
					{
						//P_END(34);
						return  to_string(GameGraphicHelper::value2Str<H3DVec3>(pos).c_str());
					}
					else if (System::String::Compare(to_string(type.m_property_modifier_type->property_name.c_str()), "Rotation") == 0)
					{
						quat = quat.Inverse();
						H3DAngles angle = quat.ToAngles();						
						H3DVec3 rot(DEG2RAD(angle.roll), DEG2RAD(angle.pitch), DEG2RAD(angle.yaw));

						//P_END(34);
						return to_string(GameGraphicHelper::value2Str<H3DVec3>(rot).c_str());
					}
					else
					{
						//P_END(34);
						return  to_string(GameGraphicHelper::value2Str<H3DVec3>(scale).c_str());
					}
				}
			}
		}

		//P_END(34);
		return getAttributeInfo(type, eff);
	}


	void MGraphicEntity::getAABB(System::Collections::Generic::List<float>^ aabblist)
	{
		//P_BEGIN(35);
		m_dyneffobj->update(0);
		BBOX b;
		m_dyneffobj->get_BBox(b);

		float* p = b.m_min_pos.ToFloatPtr();
		for (int i = 0; i < 3; ++i)
		{
			aabblist[i] = p[i];
		}

		p = b.m_max_pos.ToFloatPtr();
		for (int i = 0; i < 3; ++i)
		{
			aabblist[i+3] = p[i];
		}
		//P_END(35);
	}

	System::String^ MGraphicEntity::getAttributeInfo(PropertyModifierTypeStorage type, MStageEffect^ eff)
	{
		//P_BEGIN(36);
		IPropertyModifierStreamable* pm = s_find_actor_property_modifier(eff, m_dyneffobj, m_clip_mng, type);

		if (pm == 0)
			pm = s_find_actor_stage_default_property_modifier(m_dyneffobj, m_clip_mng, type);
		if(pm == 0)
			pm = s_find_actor_default_property_modifier(m_dyneffobj, m_clip_mng, type);

		if(pm == 0)
		{
			//assert(!type.m_property_modifier_type->is_curv);
			//P_END(36);
			return to_string(0);
		}
		else
		{
			//P_END(36);
			return to_string(pm->get_value());
		}
	}

	void MGraphicEntity::setAttributeSeq(MStageEffect^ eff, PropertyModifierTypeStorage type, System::String^ value)
	{
		//P_BEGIN(37);
		PropertyModifierTypeStorage pmts = type;
		PropertyModifierType pmt = *pmts.m_property_modifier_type;
		pmt.is_curv = true;
		pmts.m_property_modifier_type = &pmt;

		setAttributeInfo(pmts, value, eff);
		//P_END(37);
	}

	System::String^ MGraphicEntity::getAttributeSeq(MStageEffect^ eff, PropertyModifierTypeStorage type)
	{
		//P_BEGIN(38);
		PropertyModifierTypeStorage pmts = type;
		PropertyModifierType pmt = *pmts.m_property_modifier_type;
		pmt.is_curv = true;
		pmts.m_property_modifier_type = &pmt;

		//P_END(38);
		return getAttributeInfo(pmts, eff);
	}

	void MGraphicEntity::reload(System::String^ actorid)
	{
		//P_BEGIN(39);
		DynamicEffectObject* obj = m_dyneffobj;
		if ((actorid != nullptr) && (actorid->Length > 0))
		{
			obj = (DynamicEffectObject*)(obj->find_actor_recur(StringItemEx(actorid)));
		}

		obj->reload();
		//P_END(39);
	}

	AttributeSequenceEvaluator^ MGraphicEntity::getAttributeSeqEvaluator(MStageEffect^ eff,  PropertyModifierTypeStorage type)
	{
		//P_BEGIN(40);
		PropertyModifierTypeStorage pmts = type;
		PropertyModifierType pmt = *pmts.m_property_modifier_type;
		pmt.is_curv = true;
		pmts.m_property_modifier_type = &pmt;

		IPropertyCurvModifierStreamable* pm = dynamic_cast<IPropertyCurvModifierStreamable*>(s_find_actor_property_modifier(eff, m_dyneffobj, m_clip_mng, pmts));

		AttributeSequenceEvaluator^ evaluator = gcnew AttributeSequenceEvaluator(pm ? pm->get_calculator() : 0);
		//P_END(40);
		return evaluator;
	}

	void MGraphicEntity::setName(System::String^ id)
	{
		//1，查询该物体的类型，是顶层物体还是级联的子物体
		bool bTop = true;
		if (m_dyneffobj->get_parent())
			bTop = false;
		//2，根据物体类型判断重名范围，并进行重名检测
		if(bTop)
		{
			//顶层物体重名
			if(m_actor_manager->find_actor(StringItemEx(id)))
				return;
		}
		else
		{
			//级联物体重名
			Actor* act = m_dyneffobj->get_parent();
			if(act->find_actor(StringItemEx(id)))
				return;
		}
		//3，改名
		m_dyneffobj->set_name(StringItemEx(id));
	}

	System::String^ MGraphicEntity::getName()
	{
		return to_string(m_dyneffobj->get_name());
	}

	void MGraphicEntity::resetActorIter()
	{
		m_dyneffobj->resetSubObjectIterator();
	}

	MGraphicEntity^ MGraphicEntity::getNextActor()
	{
		DynamicEffectObject* dyneffobj = m_dyneffobj->nextSubObject();

		if(!dyneffobj)
			return nullptr;
		else
		{
			MGraphicEntity^ entity = gcnew MGraphicEntity(dyneffobj, m_clip_mng, m_actor_manager);
			return entity;
		}
	}

	System::String^ MGraphicEntity::getChildActorNameById(unsigned int id)
	{
		m_dyneffobj->resetSubObjectIterator();
		while (DynamicEffectObject* subObj = m_dyneffobj->nextSubObject())
		{
			if (subObj->find_entity_recursive(id))
			{
				return to_string(subObj->get_name());
			}
		}

		return nullptr;
	}


//////////////////////////////////////////////////////////////////////////
	MStageEffect::MStageEffect(StageAnimationClip* clip) : m_clip(clip)
	{
	}

	int MStageEffect::getFrameCount()
	{
		return m_clip->getFrameCount();
	}

	void MStageEffect::setFrameCount(int count)
	{
		m_clip->setFrameCount(count);
	}

	void MStageEffect::setID(System::String^ id)
	{
		m_clip->getClipMng()->renameClip(m_clip, StringItemEx(id));
	}
	System::String^ MStageEffect::getID()
	{
		return to_string(m_clip->getID());
	}

	//GraphicEntityPlayerMng::GraphicEntityPlayerMng(StageAnimationPlayer* player)
	GraphicEntityPlayerMng::GraphicEntityPlayerMng(StageAnimationPlayerEditor* player)
	{
		m_player = player;
	}

	GraphicEntityPlayerMng::~GraphicEntityPlayerMng()
	{
	}

	void GraphicEntityPlayerMng::playGraphicEntity(MGraphicEntity^ actor, bool play, MStageEffect^ eff)
	{
		//P_BEGIN(41);
		if (play)
		{
			m_player->ExcludeLevelAndClip4DynEffObj(actor->m_dyneffobj);
		}
		m_player->playDynamicEffectObject(actor->m_dyneffobj, play);
		//P_END(41);
	}

	void GraphicEntityPlayerMng::update(float t)
	{
		//P_BEGIN(42);
		m_player->update(t);
		//P_END(42);
	}

	/*
	void GraphicEntityPlayerMng::update(int t, MGraphicEntity^ actor)
	{
	// todo
	actor->m_dyneffobj->update(t);
	}
	*/

	MCamera::MCamera(DynamicEffectObject* dyneffobj, StageAnimationClipMng* clip_mng, ActorManager* actormng) 
		: MGraphicEntity(dyneffobj, clip_mng, actormng)
	{
	}

	void MCamera::setFrameCount(int count)
	{
		StageAnimationClip* clip = getRelStageEffect()->m_clip;
		clip->setFrameCount(count);		
	}
	int MCamera::getFrameCount()
	{
		StageAnimationClip* clip = getRelStageEffect()->m_clip;
		return clip->getFrameCount();
	}

	bool MCamera::equal(MCamera^ target)
	{
		return this->m_dyneffobj == target->m_dyneffobj;
	}

	void MCamera::setID(System::String^ id)
	{
		//P_BEGIN(43);
		m_dyneffobj->set_name(StringItemEx(id));
		StageAnimationClip* clip = getRelStageEffect()->m_clip;
		clip->getClipMng()->renameClip(clip, StringItemEx(id));
		//P_END(43);
	}
	System::String^ MCamera::getID()
	{
		return to_string(m_dyneffobj->get_name());
	}

	/// 获得相机信息
	CameraInfo^ MCamera::getCamInfo()
	{
		//P_BEGIN(44);
		CameraEffectObject* cam = static_cast<CameraEffectObject*>(m_dyneffobj);
		::CameraInfo caminfo;
		cam->getCameraInfo(caminfo);

		ClientBridge::CameraInfo^ bcaminfo = gcnew ClientBridge::CameraInfo();

		bcaminfo->pos_x = caminfo.pos_x;
		bcaminfo->pos_y = caminfo.pos_y;
		bcaminfo->pos_z = caminfo.pos_z;
		bcaminfo->dir_x = caminfo.dir_x;
		bcaminfo->dir_y = caminfo.dir_y;
		bcaminfo->dir_z = caminfo.dir_z;
		bcaminfo->up_x = caminfo.up_x;
		bcaminfo->up_y = caminfo.up_y;
		bcaminfo->up_z = caminfo.up_z;
		bcaminfo->target_x = caminfo.target_x;
		bcaminfo->target_y = caminfo.target_y;
		bcaminfo->target_z = caminfo.target_z;
		bcaminfo->fov = caminfo.fov;
		bcaminfo->ratio = caminfo.ratio;
		bcaminfo->fNear = caminfo.fNear;
		bcaminfo->fFar = caminfo.fFar;
		bcaminfo->focalplane = caminfo.focalplane;
		bcaminfo->dof_range = caminfo.dof_range;
		bcaminfo->confusion = caminfo.confusion;

		//P_END(44);
		return bcaminfo;
	}

	PostProcessParams^ MCamera::getCameraPostProcessParam()
	{
		//P_BEGIN(45);
		CameraEffectObject* cam = static_cast<CameraEffectObject*>(m_dyneffobj);
		const CameraEffectObject::PostProcessCollection& postprocesses = cam->getCameraPostParam();

		PostProcessParams^ ppp = gcnew PostProcessParams();
		ppp->m_post_process_lists = gcnew PostProcessParams::PostProcessCollection;

		for(CameraEffectObject::PostProcessCollection::const_iterator pos = postprocesses.begin();
			pos != postprocesses.end(); ++pos)
		{
			PostProcessParams::PostParamCollection^ post_parames = gcnew PostProcessParams::PostParamCollection;
			for (CameraEffectObject::PostParamCollection::const_iterator it = pos->second.begin(); it != pos->second.end(); ++it)
			{
				System::String^ str = to_string(it->first.c_str());
				const ::H3DVec4& vec4 = it->second;

				ClientBridge::H3DVec4^ bvec4 = gcnew H3DVec4();
				bvec4->x = vec4.x;
				bvec4->y = vec4.y;
				bvec4->z = vec4.z;
				bvec4->w = vec4.w;

				post_parames->Add(str, bvec4);
			}

			ppp->m_post_process_lists->Add(to_string(pos->first.c_str()), post_parames);
		}
		//P_END(45);
		return ppp;
	}

	void MCamera::disableCameraPostProcess(System::String^ postprocess_name)
	{
		CameraEffectObject* cam = dynamic_cast<CameraEffectObject*>(m_dyneffobj);
		cam->disableCameraPostprocess(StringItemEx(postprocess_name));
	}

	void MCamera::setCameraPostProcessEnable(System::String^  post_process_name, bool enable)
	{
		CameraEffectObject* cam = dynamic_cast<CameraEffectObject*>(m_dyneffobj);
		cam->setCameraPostProcessEnable(StringItemEx(post_process_name), enable);
	}

	MStageEffect^ MCamera::getRelStageEffect()
	{
		//P_BEGIN(46);
		StageAnimationClip* clip = m_clip_mng->find(this->m_dyneffobj->get_name());
		assert(clip);

		MStageEffect^ stageeff = gcnew MStageEffect(clip);
		//P_END(46);
		return stageeff;
	}

	PropertyModifierType MCamera::getPropertyModifierType(System::String^ type, bool iscurv)
	{
		//P_BEGIN(47);
		PropertyModifierType pmptr;
		pmptr.is_curv = iscurv;
		pmptr.child_entity_id = "camera";

		int index = type->IndexOf("/");
		if (-1 == index)
		{
			pmptr.property_name = StringItemEx(type);
			//P_END(47);
			return pmptr;
		}

		pmptr.property_name =  StringItemEx(type->Substring(0, index));
		if (System::String::Compare("Camera_Postprocess_Attribute_Sequence", to_string(pmptr.property_name.c_str())) == 0)
		{
			pmptr.property_name = "Camera_PostProcess";
		}

		pmptr.property_fixed_param =  StringItemEx(type->Substring(index + 1, type->Length - 1 - index));

		//P_END(47);
		return pmptr;
	}

	PropertyModifierTypeStorage MCamera::getPropertyModifierTypeStorage(PropertyModifierType* pmptr)
	{
		PropertyModifierTypeStorage pmtype;
		pmtype.m_property_modifier_type = pmptr;
		return pmtype;
	}

	void MCamera::setAttributeInfo(System::String^ type, System::String^ value)
	{
		//P_BEGIN(48);
		MStageEffect^ stageeff = getRelStageEffect();
		PropertyModifierType pmt = getPropertyModifierType(type, false);
		PropertyModifierTypeStorage pmts = getPropertyModifierTypeStorage(&pmt);

		MGraphicEntity::setAttributeInfo(pmts, value, stageeff);
		//P_END(48);
	}

	System::String^ MCamera::getAttributeInfo(System::String^ type)
	{
		//P_BEGIN(49);
		MStageEffect^ stageeff = getRelStageEffect();
		PropertyModifierType pmt = getPropertyModifierType(type, false);
		PropertyModifierTypeStorage pmts = getPropertyModifierTypeStorage(&pmt);

		//P_END(49);
		return MGraphicEntity::getAttributeInfo(pmts, stageeff);
	}

	System::String^ MCamera::getAttributeSeq(System::String^ type)
	{
		//P_BEGIN(50);
		MStageEffect^ stageeff = getRelStageEffect();
		PropertyModifierType pmt = getPropertyModifierType(type, true);
		PropertyModifierTypeStorage pmts = getPropertyModifierTypeStorage(&pmt);

		//P_END(50);
		return MGraphicEntity::getAttributeInfo(pmts, stageeff);
	}

	void MCamera::setAttributeSeq(System::String^ type, System::String^ value)
	{
		//P_BEGIN(51);
		MStageEffect^ stageeff = getRelStageEffect();
		PropertyModifierType pmt = getPropertyModifierType(type, true);
		PropertyModifierTypeStorage pmts = getPropertyModifierTypeStorage(&pmt);

		MGraphicEntity::setAttributeInfo(pmts, value, stageeff);
		//P_END(51);
	}

	AttributeSequenceEvaluator^ MCamera::getAttributeSeqEvaluator(System::String^ type)
	{
		//P_BEGIN(52);
		MStageEffect^ stageeff = getRelStageEffect();
		PropertyModifierType pmt = getPropertyModifierType(type, true);
		PropertyModifierTypeStorage pmts = getPropertyModifierTypeStorage(&pmt);

		//P_END(52);
		return MGraphicEntity::getAttributeSeqEvaluator(stageeff, pmts);
	}

	void MCamera::setAttributeSeqEnable(System::String^ type, bool is_enable)
	{
		IPropertyCurvModifierStreamable* modifier = getPropertyCurvModifierFromType(type);
		if (NULL == modifier)
		{
			return;
		}

		modifier->set_enable(is_enable);
	}

	bool MCamera::isAttributeSeqEnable(System::String^ type)
	{
		IPropertyCurvModifierStreamable* modifier = getPropertyCurvModifierFromType(type);
		if (NULL == modifier)
		{
			return false;
		}

		return modifier->get_enable();
	}
	
	IPropertyCurvModifierStreamable* MCamera::getPropertyCurvModifierFromType(System::String^ type)
	{
		//P_BEGIN(53);
		MStageEffect^ stageeff = getRelStageEffect();
		PropertyModifierType pmt = getPropertyModifierType(type, true);

		PropertyModifierTypeStorage pmts = getPropertyModifierTypeStorage(&pmt);
		IPropertyCurvModifierStreamable* modifier = dynamic_cast<IPropertyCurvModifierStreamable*>(s_find_actor_property_modifier(stageeff, m_dyneffobj, m_clip_mng, pmts));

		//P_END(53);
		return modifier;
	}

	//StagePlayerMng::StagePlayerMng(StageAnimationPlayer* player, StageAnimationClipMng* clip_mng, ActorManager* actormng)
	StagePlayerMng::StagePlayerMng(StageAnimationPlayerEditor* player, StageAnimationClipMng* clip_mng, ActorManager* actormng)
	{
		m_player = player;
		m_clip_mng = clip_mng;
		m_actormng = actormng;
	}

	StagePlayerMng::~StagePlayerMng()
	{
	}

	void StagePlayerMng::playStageEffect(StageInfoMng^ stagemng, MStageEffect^ eff, bool play)
	{
		//P_BEGIN(54);
		//m_player->playLevel(false);
		if (play)
		{
			m_player->ExcluedLevelAndDynEffObj4Clip(eff->m_clip->getID());
		}
		m_player->playStageAnimationClip(eff->m_clip->getID(), play);
		//m_player->playStageEffect(eff->m_clip->getID(), play);
		//P_END(54);
	}

	void StagePlayerMng::playCamera(StageInfoMng^ stagemng, MCamera^ cam, bool play)
	{
		//P_BEGIN(55);
		if (play)
		{
			//m_player->ExcluedLevelAndDynEffObj4Clip(cam->m_dyneffobj->get_name());
		}
		//m_player->playCamera(cam->m_dyneffobj->get_name(), play);
		m_player->playStageAnimationClip(cam->m_dyneffobj->get_name(), play);
		//P_END(55);
	}

	void StagePlayerMng::playLevel(bool play, LevelInfoMng^ levelmng, StageInfoMng^ stagemng)
	{
		//P_BEGIN(56);
		if (play)
		{
			m_player->ExcludeClipAndDynEffObj4Level();
		}
		m_player->playLevel(play);
		//P_END(56);
	}

	void StagePlayerMng::updateLevel(float t, LevelInfoMng^ levelmng, StageInfoMng^ stagemng)
	{
		//P_BEGIN(57);
		m_player->update(t);
		//P_END(57);
	}

	void StagePlayerMng::updateCamera(StageInfoMng^ stagemng, float t, MCamera^ cam)
	{
		//StageAnimationClip* clip = m_clip_mng->find(cam->m_dyneffobj->get_name());
		//clip->update(t);
		//P_BEGIN(58);
		//m_player->update(t);
		//m_player->updateCamera(t);
		StageAnimationClip* clip = m_clip_mng->find(cam->m_dyneffobj->get_name());
		m_player->updateClip(t, clip);
		//P_END(58);
	}

	void StagePlayerMng::updateStageEffect(StageInfoMng^ stagemng, float t, MStageEffect^ eff)
	{
		//P_BEGIN(59);
		//m_player->update(t);
		//m_player->updateStageEffect(t);
		m_player->updateClip(t, eff->m_clip);
		//P_END(59);
		//eff->m_clip->update(t);
	}

	MCamera^ StagePlayerMng::getCurCam()
	{
		CameraEffectObject* camobj = m_player->getCurCamera();
		if(camobj)
			return gcnew MCamera(camobj, m_player->getClipManger(),  m_actormng);
		else
			return nullptr;
	}

	void StagePlayerMng::drawWimblePrepasslightLine()
	{
		//P_BEGIN(60);
		m_player->drawLightLine();
		//P_END(60);
	}

	void StagePlayerMng::enableLightEffect(bool enable)
	{
		//P_BEGIN(61);
		m_player->enableLightEffect(enable);
		//P_END(61);
	}

	void StagePlayerMng::setFrameMs(double ms)
	{
		//P_BEGIN(62);
		double fps = 1000 / ms;
		m_player->setFramePerSecond(fps);
		//P_END(62);
	}
	double StagePlayerMng::getFrameMs()
	{
		//P_BEGIN(63);
		double fps = m_player->getFramePerSecond();
		//P_END(63);
		return 1000 / fps;
	}

	void StagePlayerMng::drawLightLocationLine()
	{
		//P_BEGIN(64);
		m_player->drawLightLocationLine();
		//P_END(64);
	}

	bool StagePlayerMng::loadEnviromentFile(System::String^ env_file, bool is_binary)
	{
		return m_player->loadEnvironmentFile(StringItemEx(env_file), is_binary);
	}

	bool StagePlayerMng::loadStaticObject(System::String^ static_obj_file, bool is_binary)
	{
		return m_player->loadStaticObject(StringItemEx(static_obj_file), is_binary);
	}

	StageMiddleLayer::StageMiddleLayer(void* ic, void* ilevel)
	{
		m_def_eff_res_path = "../resources/media/shaders/null.spe";
		m_def_model_res_path =  "../resources/media/shaders/TerrainObject/cylinder.dml";
		m_def_actor_res_path = "../resources/media/shaders/role/output/dress.xml";
		m_line_shader_ui_path =  "../resources/media/shaders/ui/Coord2.xml";
		m_defined_obj_file_path = "../resources/stage/defined_object.xml";
		m_irender = (H3DI::IRender*)ic;
		m_ilevel = (H3DI::ILevel*)ilevel;
		create();
	}

	StageMiddleLayer::~StageMiddleLayer()
	{
		clear();
	}

	void StageMiddleLayer::reset()
	{
		clear();
		create();
	}

	void StageMiddleLayer::create()
	{
		// 没有engine的接口
		m_stage = new Stage();

		m_target_position_mng = gcnew TargetPositionMngImpl();
		m_stage->initialize(m_irender, m_target_position_mng->mITargetPositionMng, m_ilevel);

		//m_player = static_cast<StageAnimationPlayer*>(m_stage->getPlayer());
		m_player = static_cast<StageAnimationPlayerEditor*>(m_stage->getPlayer());

		m_g_entity_player_mng = gcnew GraphicEntityPlayerMng(m_player);
		m_stage_playermng = gcnew StagePlayerMng(m_player, m_player->getClipManger(), m_stage->getActorMng());
		m_stage_infomng = gcnew StageInfoMng(m_stage, m_player);
		m_levelinfo_mng = gcnew LevelInfoMng(dynamic_cast<AnimationScript*>(m_player->getAnimationScript()), m_player->getClipManger());
		m_stage->setDefinedObjectFileName(StringItemEx(m_defined_obj_file_path));
		m_stage->setDefaultResourcePath(StringItemEx(m_def_eff_res_path), StringItemEx(m_def_model_res_path), StringItemEx(m_def_actor_res_path), StringItemEx(""));

		m_stage->setLineShaderUILib(StringItemEx(m_line_shader_ui_path));
	}

	void StageMiddleLayer::clear()
	{
		m_target_position_mng->~TargetPositionMngImpl();
		m_levelinfo_mng->~LevelInfoMng();
		m_stage_infomng->~StageInfoMng();
		m_stage_playermng->~StagePlayerMng();
		m_g_entity_player_mng->~GraphicEntityPlayerMng();

		delete m_stage;
		m_stage = 0;
		m_player = 0;
	}

	void StageMiddleLayer::update(float t)
	{
		//P_BEGIN(65);
		m_player->update(t);
		//P_END(65);
	}
	void StageMiddleLayer::render()
	{
		//P_BEGIN(66);
		//m_player->render();
		//P_END(66);
	}

	void StageMiddleLayer::setDefaultPath(System::String^ definedObjFilePath, System::String^ defaultEffectResPath, System::String^ defaultModelResPath, System::String^ defaultActorResPath, System::String^ lineShaderUIPath)
	{
		//P_BEGIN(67);
		if (definedObjFilePath->Length > 0)
		{
			m_defined_obj_file_path = definedObjFilePath;
		}
		
		if (defaultEffectResPath->Length > 0)
		{
			m_def_eff_res_path = defaultEffectResPath;
		}

		if (defaultModelResPath->Length > 0)
		{
			m_def_model_res_path = defaultModelResPath;
		}

		if (defaultActorResPath->Length > 0)
		{
			m_def_actor_res_path = defaultActorResPath;
		}

		if (lineShaderUIPath->Length > 0)
		{
			m_line_shader_ui_path = lineShaderUIPath;
		}

		m_stage->setDefinedObjectFileName(StringItemEx(m_defined_obj_file_path));
		m_stage->setDefaultResourcePath(StringItemEx(m_def_eff_res_path), StringItemEx(m_def_model_res_path), StringItemEx(m_def_actor_res_path), StringItemEx(""));

		m_stage->setLineShaderUILib(StringItemEx(m_line_shader_ui_path));
		//P_END(67);
	}

	unsigned int StageMiddleLayer::pushHardwareSelect(bool sel_line)
	{
		return m_player->pushHardwareSelect(sel_line);
	}
}
