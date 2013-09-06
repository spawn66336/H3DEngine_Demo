/**
* @file stage.h
* 这个文件的主要功能是：
* 
* @author 李汕
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 李汕2010.11.16
*/
#include "middle_layer_body.h"
#include "platform/include/i_entity_pool.h"

DefinedObjectFactory::DefinedObjectFactory(Stage* stage)
:m_stage(stage)
,m_defined_item_collection(NULL)
{	
};
DefinedObjectFactory::~DefinedObjectFactory()
{

};

//建立单一物体
DynamicEffectObject* DefinedObjectFactory::createDynamicEffectObject(ActorManager* actor_manager, const char* name, const char* type)
{
	if(type == NULL )//子actor没有这个定义
		return new DynamicEffectObject(actor_manager,name, "");
	else if(stricmp(type,"x52_Camera") == 0)//相机actor
		return new CameraEffectObject(actor_manager,name, type);
	else if(stricmp(type,"x52_Follow") == 0)//相机actor
		return new FollowLightDynamicEffectObject(actor_manager,name, type);
	else
		return new DynamicEffectObject(actor_manager,name, type);
};
//递归建立动态效果物体和子实体
DynamicEffectObject* DefinedObjectFactory::createObjectRecursive(const DefinedItem* defined_item_0,const char* obj_id)
{
	//建立actor
	DynamicEffectObject* dynamic_effect_object = createDynamicEffectObject(m_stage->getActorMng(),obj_id, defined_item_0->m_item_type.c_str());

	std::list<DefinedItem*>::const_iterator iter = defined_item_0->m_sub_item.begin();
	for(; iter != defined_item_0->m_sub_item.end(); ++ iter)
	{
		if((*iter)->m_node_type == DefinedItem::NORMAL_ENTITY)
		{
			//建立实体
			CMediaEndEntity* entity = m_stage->getActorMng()->create_entity((*iter)->m_item_type.c_str(),(*iter)->m_id.c_str());
			assert(entity);
			//加入当前actor
			if(!dynamic_effect_object->add_entity(entity))
			{
				delete dynamic_effect_object;
				return NULL;
			}
		}
		else if((*iter)->m_node_type == DefinedItem::DEFINED_EFFECT_OBJECT || (*iter)->m_node_type == DefinedItem::NORMAL_EFFECT_OBJECT)
		{//建立子actor
			DynamicEffectObject* obj_tmp = createObjectRecursive(*iter, (*iter)->m_id.c_str());
			assert(obj_tmp);
			if(!dynamic_effect_object->add_actor(obj_tmp))
			{
				delete dynamic_effect_object;
				return NULL;
			}
		}
		else
			assert(0);
	}
	return dynamic_effect_object;
}
//工厂建立动态效果物体
DynamicEffectObject* DefinedObjectFactory::createDynamicActor(const char* actor_name, const char* actor_type)
{
	const DefinedItem* defined_item = m_defined_item_collection->getDefinedItem(actor_type) ;
	assert(defined_item);
	//建立actor	
	DynamicEffectObject* object = createObjectRecursive(defined_item,actor_name);
	if(object == NULL)
		return NULL;
	
	//取播放器
	StageAnimationPlayer* ani_player = dynamic_cast<StageAnimationPlayer*>(m_stage->getPlayer());
	assert(ani_player);
	//建立缺省属性
	if(!object->createAnimationClipByItem(DynamicEffectObject::getDefaultPropertyName(),defined_item))
	{
		delete object;
		return NULL;
	}
	return object;
};


Stage::Stage()
{
	m_actor_manager = new DynEffectObjectManager();
#ifdef _EDITOR
	m_ani_player = new StageAnimationPlayerEditor(this);//建立播放器
#else
	m_ani_player = new StageAnimationPlayer(this);//建立播放器
#endif
	
	m_defined_object_factory= new DefinedObjectFactory(this);
	m_defined_object_file_path = "../resources/stage/defined_object.xml";
	m_is_outdoor = false;
	m_min_color_brightness = - 1;
	m_stage_lod = LL_ZERO;
	m_post_processor = new StagePostProcessor(this);
};
Stage::~Stage()
{
	if (get_pool())
	{
		get_pool()->unregisterStage(this);
	}

	if(m_defined_object_factory)
		delete m_defined_object_factory;

	if(m_ani_player)
		delete m_ani_player;

	if(m_actor_manager)
		delete m_actor_manager;

	if (m_post_processor)
	{
		delete m_post_processor;
	}

	BiXmlHelper::ClearXMLDocument();
};
//取预定义动态效果物体文件名
const char* Stage::getDefinedObjectFilename()
{
	return m_defined_object_file_path.c_str();
}

void Stage::setDefinedObjectFileName(const char* filePath)
{
	assert(NULL != filePath);
	m_defined_object_file_path = filePath;
}

//初始化
bool Stage::initialize()
{
	BiXmlHelper::LoadXMLDocument(getDefinedObjectFilename());
	//m_actor_manager->initialize(); 此初始化尚未完成
	if(!m_ani_player->loadEditInfoTable(getDefinedObjectFilename(),false))//此处设置了非bin ，后面需要重新处理一下 lishan 101125
	{
		//LogError("[Stage::initialize] load obj file failed:",getDefinedObjectFilename());
		return false;
	}
	//给工厂设置预定义信息
	m_defined_object_factory->setDefinedItemCollection(m_ani_player->getDefinedItemCollection());

	BiXmlHelper::ClearXMLDocument();
	return true;
} ;
bool Stage::initialize(H3DI::IRender* irender, ITargetPositionMng* targetposmng, H3DI::ILevel* ilevel/* = NULL*/)
{
	if(!m_actor_manager->initialize(irender, ilevel)) //此初始化尚未完成
		return false;

	if (get_pool())
	{
		get_pool()->registerStage(this);
		switchToLod(get_pool()->getLodLevel());
	}

	m_ani_player->setTargetPositionManager(targetposmng);

	BiXmlHelper::LoadXMLDocument(getDefinedObjectFilename());
	if(!m_ani_player->loadEditInfoTable(getDefinedObjectFilename(),false))//此处设置了非bin ，后面需要重新处理一下 lishan 101125
	{
		//LogError("[Stage::initialize2] load obj file failed:",getDefinedObjectFilename());
		return false;
	}
	//给工厂设置预定义信息
	m_defined_object_factory->setDefinedItemCollection(m_ani_player->getDefinedItemCollection());

	BiXmlHelper::ClearXMLDocument();
	return true;	
}
//重置
void Stage::reset()
{//how
	assert(false);
} ;
//根据节点信息递归建立动态效果物体
DynamicEffectObject* Stage::createStageDynamicEffectObjectRecursive(BiXmlNode* effectObjNode)
{
	const char* id = BiXmlUtility::get_attribute(effectObjNode, "id");
	assert(id);

	const char* defined_type = BiXmlUtility::get_attribute(effectObjNode, "defined_type");
	assert(defined_type);

	//建立actor	
	DynamicEffectObject* obj = m_defined_object_factory->createDynamicActor(id,defined_type);
	assert(obj);
	//查找级联物体
	BiXmlNode* linkObjList = effectObjNode->FirstChild("LinkDynamicEffectObjectList");
	if(linkObjList)
	{
		BiXmlNode* subobj_node = 0;
		while (subobj_node = linkObjList->IterateChildren("DynamicEffectObject", subobj_node))
		{
			DynamicEffectObject* subobj = createStageDynamicEffectObjectRecursive(subobj_node);
			assert(subobj);
			obj->addSubObject(subobj);
		}
	}
	return obj;
}
//读取stage文件，建立动态效果物体
bool Stage::loadStageDynamicEffectObjects(const char* stagefile0, bool binary, I_Keep_Client_Alive* pLp)
{
	//create dynamic effect object
	assert(!binary);

	//转换分隔符
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(stagefile0,fullFileName);

	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		//LogError("[Stage::loadStageDynamicEffectObjects] eff obj file failed:",fullFileName.c_str());
		assert(0);
		return false;
	}
	// 读取序列
	BiXmlNode* stageRootNode = doc->FirstChild("Stage");
	if (stageRootNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	// 读取动态效果物体的节点
	BiXmlNode* dynamicEffectNodes = stageRootNode->FirstChild("SubEntitys");
	if (dynamicEffectNodes != NULL)
	{
		BiXmlNode* item = 0;
		while (item = dynamicEffectNodes->IterateChildren("DynamicEffectObject", item))
		{
			if ( pLp )
			{
				pLp->update();
			}
			DynamicEffectObject* main_obj = createStageDynamicEffectObjectRecursive(item);
			getActorMng()->add_actor(main_obj);
		}
	}
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return true;
}
//载入舞台的一些杂项信息，比如：舞台类型、最低颜色值
bool Stage::loadStageMiscInfo(const char* stageFile, bool isBinary)
{
	assert(!isBinary);

	// 读取舞台类型以及最低颜色
	m_min_color_brightness = - 1;
	m_is_outdoor = false;

	std::string fullFileName = stageFile;
	std::string::size_type lastseperatorpos = fullFileName.find_last_of("/");
	if(lastseperatorpos != - 1)
		fullFileName = fullFileName.substr(lastseperatorpos + 1);
	fullFileName = "../resources/stage/" + fullFileName;

	// 
	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	if(BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true))
	{
		// 读取序列
		BiXmlNode* stageRootNode = doc->FirstChild("Stage");
		if (stageRootNode != NULL)
		{
			// 判断室外或室内场景
			if(const char* str = BiXmlHelper::get_attribute(stageRootNode, "OutDoor"))
				m_is_outdoor = atoi(str) == 1;

			if(const char* str = BiXmlHelper::get_attribute(stageRootNode, "MinBrightNess"))
				m_min_color_brightness = atof(str);
		}

		XmlDocFactory::ReleaseDocument(doc);

		return true;
	}

	return false;
}
// 载入舞台上的动态效果物体
bool Stage::loadDynamicObjects(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp /* = 0 */)
{
	// 载入舞台上的动态效果体，并建立每个物体的缺省属性
	if (!loadStageDynamicEffectObjects(stageFile, isBinary, pLp))
		return false;

	// 为每个载入的动态效果物体建立缺省的舞台效果
	if (!m_ani_player->loadDefaultStageClips(stageFile, isBinary))
		return false;

	// 载入该场景所包含的所有舞台效果的列表
	if (!m_ani_player->loadStageEffectListPath(stageFile, isBinary))
		return false;

	// 读取舞台类型以及最低颜色
	if(!loadStageMiscInfo(stageFile, isBinary))
		return false;

	m_actor_manager->get_level_entity()->RestruceOctree();

	switchToLod(m_stage_lod);

	BiXmlHelper::ClearXMLDocument();
	return true;
}
// 读取舞台信息（动态效果物体、舞台效果）
bool Stage::loadStageFile(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp)
{
	if(!loadStageDynamicEffectObjects(stageFile,isBinary, pLp))
		return false;	
	if(!m_ani_player->loadStageFile(stageFile,isBinary, pLp))
		return false;

	// 读取舞台类型以及最低颜色
	{
		m_min_color_brightness = - 1;
		m_is_outdoor = false;

		std::string fullFileName = stageFile;
		std::string::size_type lastseperatorpos = fullFileName.find_last_of("/");
		if(lastseperatorpos != - 1)
			fullFileName = fullFileName.substr(lastseperatorpos + 1);
		fullFileName = "../resources/stage/" + fullFileName;

		// 
		BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
		if(BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true))
		{
			// 读取序列
			BiXmlNode* stageRootNode = doc->FirstChild("Stage");
			if (stageRootNode != NULL)
			{
				// 判断室外或室内场景
				if(const char* str = BiXmlHelper::get_attribute(stageRootNode, "OutDoor"))
					m_is_outdoor = atoi(str) == 1;

				if(const char* str = BiXmlHelper::get_attribute(stageRootNode, "MinBrightNess"))
					m_min_color_brightness = atof(str);
			}

			//delete doc;
			XmlDocFactory::ReleaseDocument(doc);
		}
	}
	m_actor_manager->get_level_entity()->RestruceOctree();

	switchToLod(m_stage_lod);
	return true;
};
// 读取剧本信息
bool Stage::loadAnimationFile(const char* levelFile, bool isBinary)
{
	if(!m_ani_player->loadAnimationFile(levelFile,isBinary))
		return false;
	return true;
} ;
bool Stage::saveStageDynamicEffectObjectRecursive(BiXmlNode* parentNode, DynamicEffectObject* obj ,StageAnimationClipMng* clip_mng)
{
	if( obj == NULL )
		return false;

	//BiXmlElement item("DynamicEffectObject");
	BiXmlElement* item = parentNode->InsertEndChild("DynamicEffectObject")->ToElement();
	item->SetAttribute("id", obj->get_name());
	item->SetAttribute("defined_type", obj->getDefinedType());
	//添加物体stage属性
	if(!clip_mng->generateStagePropertyInfo(item, obj  ))
	{
		return false;
	}
	//添加 级联实体属性
	obj->resetSubObjectIterator();
	DynamicEffectObject* sub_obj = obj->nextSubObject();
	if(sub_obj)
	{
		//BiXmlElement sub_obj_list("LinkDynamicEffectObjectList");
		BiXmlElement* sub_obj_list = item->InsertEndChild("LinkDynamicEffectObjectList")->ToElement();
		while(sub_obj)
		{
			if(!saveStageDynamicEffectObjectRecursive(sub_obj_list, sub_obj, clip_mng))
				return false;
			
			sub_obj = obj->nextSubObject();
		}
		//item.InsertEndChild(sub_obj_list);
	}
	//加入节点
	//parentNode->InsertEndChild(item);

	return true;
};
bool Stage::saveStageSystemObjects(BiXmlNode* entityNode)
{
	m_actor_manager->reset_some_sysactor_iter(LL_ZERO);
	
	while(Actor* actor = m_actor_manager->next_some_sysactor(LL_ZERO))
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(actor);
		if(obj != NULL )
		{
			if(!saveStageDynamicEffectObjectRecursive(entityNode,obj,m_ani_player->getClipManger()))
				return false;
		}
	}

	m_actor_manager->reset_some_sysactor_iter(LL_ONE);
	while(Actor* actor = m_actor_manager->next_some_sysactor(LL_ONE))
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(actor);
		if(obj != NULL )
		{
			if(!saveStageDynamicEffectObjectRecursive(entityNode,obj,m_ani_player->getClipManger()))
				return false;
		}
	}

	m_actor_manager->reset_some_sysactor_iter(LL_TWO);
	while(Actor* actor = m_actor_manager->next_some_sysactor(LL_TWO))
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(actor);
		if(obj != NULL )
		{
			if(!saveStageDynamicEffectObjectRecursive(entityNode,obj,m_ani_player->getClipManger()))
				return false;
		}
	}
	return true;
}
bool Stage::saveStageDynamicEffectObjects(const char* stage_file0, bool isBinary)
{
	assert(!isBinary);//尚不支持2进制
	//BiXmlDocument* doc = new BiXmlDocument();
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();
	if (doc == NULL)
		return false;

	//转换分隔符
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(stage_file0,fullFileName);
	//取工作路径
	std::string working_path ;
	MiddleLayerFilePathHelper::getWorkingPath(stage_file0,working_path);

	doc->LoadFile(fullFileName.c_str());
	// 找到Stage节点
	BiXmlNode* stageNode = doc->FirstChild("Stage");
	if (stageNode == NULL)
	{
		/*stageNode = new BiXmlElement("Stage");
		doc->LinkEndChild(stageNode);*/
		stageNode = doc->InsertEndChild("Stage");
	}

	BiXmlNode* subEntitysNode = stageNode->FirstChild("SubEntitys");
	if (subEntitysNode == NULL)
	{
		/*subEntitysNode = new BiXmlElement("SubEntitys");
		stageNode->LinkEndChild(subEntitysNode);*/
		subEntitysNode = stageNode->InsertEndChild("SubEntitys");
	}
	subEntitysNode->Clear();
	//保存场景中除了当前lod级别之外的所有系统物体

	if(!saveStageSystemObjects(subEntitysNode))
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	//保存当前lod级别中的所有物体
	// 重置迭代器
	m_actor_manager->reset_iterator();
	Actor* actor = m_actor_manager->next();
	
	while(actor != NULL)
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(actor);
		if(obj != NULL )
		{
			if(!saveStageDynamicEffectObjectRecursive(subEntitysNode,obj,m_ani_player->getClipManger()))
			{
				//delete doc;//发生错误不保存，直接返回
				XmlDocFactory::ReleaseDocument(doc);
				return false;
			}
		}
		actor = m_actor_manager->next();
	}

	bool ret = doc->SaveFile(fullFileName.c_str());
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return ret;
}
// 保存舞台信息（动态效果物体、舞台效果）
bool Stage::saveStageFile(const char* stageFile, bool isBinary, bool isOptimize)
{
	if(!saveStageDynamicEffectObjects(stageFile,isBinary))
		return false;
	if(!m_ani_player->saveStageFile(stageFile,isBinary, isOptimize))
		return false;
	return true;
} ;

// 保存剧本信息
bool Stage::saveAnimationFile(const char* levelFile, bool isBinary)
{
	if(!m_ani_player->saveAnimationFile(levelFile,isBinary))
		return false;
	return true;
} ;

//获得动画播放器
IStageAnimationPlayer* Stage::getPlayer()
{
	return m_ani_player;
};
//获取ActorMng
ActorManager* Stage::getActorMng()
{
	return m_actor_manager;
} ;
//生成预定义动态效果物体
DynamicEffectObject* Stage::createDefinedObject(const char* object_name, const char* object_type)
{
	return m_defined_object_factory->createDynamicActor(object_name, object_type);
};

void Stage::deleteDefinedObject(DynamicEffectObject* eff_obj)
{
	m_actor_manager->remove_actor(eff_obj);
	delete eff_obj;
}

void Stage::setDefaultResourcePath(const char* default_eff_res
									, const char* default_model_res
									, const char* default_actor_res
									, const char* default_skeleton_model_res)
{
	m_actor_manager->set_default_resource_path(default_eff_res
												, default_model_res
												, default_actor_res
												, default_skeleton_model_res);
}

void Stage::setLineShaderUILib(const char* line_shader_ui_lib)
{
	m_actor_manager->get_graphic_res_mng()->get_render_system()->set_line_shader_ui_lib_path(line_shader_ui_lib);
}
//添加物体级联关系,parent_obj是ActorManager中的顶层物体。如果Stage clip中已经包含sub_obj，还需要从中删除引用。
bool Stage::addObjectLink(DynamicEffectObject* parent_obj,DynamicEffectObject* sub_obj)
{
	//改变物体间连接及CLIP中的关系
	assert(m_actor_manager->find_actor(parent_obj->get_name()) );
	//actorManager移除子物体
	m_actor_manager->remove_actor(sub_obj);
	//添加到父物体中
	parent_obj->addSubObject(sub_obj);
	//从clip中删除子物体
	StageAnimationClipMng* clip_mng = m_ani_player->getClipManger();

	clip_mng->resetIterator();
	while(StageAnimationClip* ani_clip = clip_mng->next()){
		ani_clip->remove(sub_obj,false);////进行级联时移动时不删除clip
	}

	return true;
};
//移除物体级联关系,parent_obj是ActorManager中的顶层物体。如果Stage clip中已经包含parent_obj，sub_obj将放到parent_obj所在的clip引用中。
bool Stage::removeObjectLink(DynamicEffectObject* parent_obj,DynamicEffectObject* sub_obj)
{
	assert(m_actor_manager->find_actor(parent_obj->get_name()));
	//从父物体中移出
	if(!parent_obj->removeSubObject(sub_obj))
		return false;
	//添加到actorManager
	m_actor_manager->add_actor(sub_obj);
	//clip中添加子物体
	StageAnimationClipMng* clip_mng = m_ani_player->getClipManger();

	clip_mng->resetIterator();
	while(StageAnimationClip* ani_clip = clip_mng->next()){
		if(ani_clip->findEffectObject(parent_obj->get_name()))//sub_obj将放到parent_obj所在的clip引用中
			ani_clip->insert(sub_obj);
	}
	return true;
};

//调整分隔符，转换为 "**/**/**"
void MiddleLayerFilePathHelper::convertSeperator(const char* src_filename, std::string& strFileName)
{
	strFileName = src_filename;
	std::string strOldSeperator = "\\";
	std::string strNewSeperator = "/";
	std::string::size_type pos = 0; 
	std::string::size_type srcLen = strOldSeperator.size(); 
	std::string::size_type dstLen = strNewSeperator.size(); 
	while((pos = strFileName.find(strOldSeperator, pos)) != std::string::npos)
	{ 
	   strFileName.replace(pos, srcLen, strNewSeperator); 
	   pos += dstLen; 
	} 
};
//取工作路径，转换为 "**/**/**"
void MiddleLayerFilePathHelper::getWorkingPath(const char* src_filename, std::string& strWorkingPath)
{
	std::string strFileName ;
	convertSeperator(src_filename,strFileName);
	
	std::string::size_type pos = strFileName.rfind("/");
	if(pos == std::string::npos )
		strWorkingPath = "";
	else
		strWorkingPath = strFileName.substr(0,pos);
};
void MiddleLayerStrHelper::replaceStr(std::string& inputStr, const char* oldstr,const char* newstr)
{
	std::string strOldSeperator = oldstr;
	std::string strNewSeperator = newstr;
	std::string::size_type pos = 0; 
	std::string::size_type srcLen = strOldSeperator.size(); 
	std::string::size_type dstLen = strNewSeperator.size(); 
	while((pos = inputStr.find(strOldSeperator, pos)) != std::string::npos)
	{ 
	   inputStr.replace(pos, srcLen, strNewSeperator); 
	   pos += dstLen; 
	} 
};
//动态添加物体及舞台效果
bool Stage::addDynamicObjectAndEffect(const char* objectName,const char* type,const char* effectPath, bool isBinary)
{
	DynamicEffectObject* object = createDefinedObject(objectName, type);
	if(!object)
		return false;
	getActorMng()->add_actor(object);

	return m_ani_player->getClipManger()->loadEffect(effectPath,false);
};
//根据id取相机文件
const char* Stage::getCameraFile(const char* cameraName)
{
	return m_ani_player->getClipManger()->getCameraFile(cameraName);	
};

bool Stage::switchToLod(int lod)
{
	//切换场景中的系统物体与灯光
	getActorMng()->remove_sysactor_from_actorlist(LL_ZERO);
	getActorMng()->remove_sysactor_from_actorlist(LL_ONE);
	getActorMng()->remove_sysactor_from_actorlist(LL_TWO);
	
	//切换舞台上的动态物体
	getActorMng()->get_level_entity()->switch_to_lod(lod);

	getActorMng()->add_sysactor_to_actorlist(lod);

	//切换环境变量
	switchSceneEnv(lod);
	//切换静态物体
	StageAnimationPlayer* player = dynamic_cast<StageAnimationPlayer*>(getPlayer());
	player->switchStaticActorLod(lod);
	//物体上所有的clip的切换
	player->switchToLod(lod);

#ifndef _EDITOR

	m_post_processor->switchToLod(lod);

#endif
	m_stage_lod = lod;
	return true;
}

void Stage::updatePostProcess(CCameraEntity* camera, bool particalviewport)
{
	m_post_processor->updatePostProcess(camera, m_ani_player, particalviewport);
}

void Stage::updatePostProcess(const CCameraEntity::PostprocessCollection& process_collection, bool particalviewport /* = false */)
{
	m_post_processor->updatePostProcess(process_collection, m_ani_player, particalviewport);
}

void Stage::switchSceneEnv(int lod)
{
	StageAnimationPlayer* player = dynamic_cast<StageAnimationPlayer*>(getPlayer());
	DynamicEffectObject* static_object = player->getStaticActor();
	if (!static_object)
	{
		return;
	}

	//设置渐隐距离和消失距离
	Actor::EntityList model_list;
	
	static_object->contain_entity_type_recursive(MEDIA_END_ENTITY_TYPE_MODEL, model_list);
	static_object->contain_entity_type_recursive(MEDIA_END_ENTITY_TYPE_EFFECT, model_list);

	for (Actor::EntityList::iterator itr = model_list.begin(); itr != model_list.end(); ++itr)
	{
		C3DEntity* model = dynamic_cast<C3DEntity*>(*itr);
		if (model->get_detail_actor_type() == DAT_SMALL_ACTOR)
		{
			model->set_visible_dist(m_scene_lod_info[lod].m_small_actor_far_disappear);
		}
		else if (model->get_detail_actor_type() == DAT_HUGE_ACTOR)
		{
			model->set_visible_dist(m_scene_lod_info[lod].m_huge_actor_far_disappear);
		}
		else if (model->get_detail_actor_type() == DAT_DETAIL_ACTOR)
		{
			model->set_visible_dist(m_scene_lod_info[lod].m_detail_actor_far_disappear);
		}
	}
}