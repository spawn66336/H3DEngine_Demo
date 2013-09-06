/**
* @file stage.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author ����
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2010.11.16
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

//������һ����
DynamicEffectObject* DefinedObjectFactory::createDynamicEffectObject(ActorManager* actor_manager, const char* name, const char* type)
{
	if(type == NULL )//��actorû���������
		return new DynamicEffectObject(actor_manager,name, "");
	else if(stricmp(type,"x52_Camera") == 0)//���actor
		return new CameraEffectObject(actor_manager,name, type);
	else if(stricmp(type,"x52_Follow") == 0)//���actor
		return new FollowLightDynamicEffectObject(actor_manager,name, type);
	else
		return new DynamicEffectObject(actor_manager,name, type);
};
//�ݹ齨����̬Ч���������ʵ��
DynamicEffectObject* DefinedObjectFactory::createObjectRecursive(const DefinedItem* defined_item_0,const char* obj_id)
{
	//����actor
	DynamicEffectObject* dynamic_effect_object = createDynamicEffectObject(m_stage->getActorMng(),obj_id, defined_item_0->m_item_type.c_str());

	std::list<DefinedItem*>::const_iterator iter = defined_item_0->m_sub_item.begin();
	for(; iter != defined_item_0->m_sub_item.end(); ++ iter)
	{
		if((*iter)->m_node_type == DefinedItem::NORMAL_ENTITY)
		{
			//����ʵ��
			CMediaEndEntity* entity = m_stage->getActorMng()->create_entity((*iter)->m_item_type.c_str(),(*iter)->m_id.c_str());
			assert(entity);
			//���뵱ǰactor
			if(!dynamic_effect_object->add_entity(entity))
			{
				delete dynamic_effect_object;
				return NULL;
			}
		}
		else if((*iter)->m_node_type == DefinedItem::DEFINED_EFFECT_OBJECT || (*iter)->m_node_type == DefinedItem::NORMAL_EFFECT_OBJECT)
		{//������actor
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
//����������̬Ч������
DynamicEffectObject* DefinedObjectFactory::createDynamicActor(const char* actor_name, const char* actor_type)
{
	const DefinedItem* defined_item = m_defined_item_collection->getDefinedItem(actor_type) ;
	assert(defined_item);
	//����actor	
	DynamicEffectObject* object = createObjectRecursive(defined_item,actor_name);
	if(object == NULL)
		return NULL;
	
	//ȡ������
	StageAnimationPlayer* ani_player = dynamic_cast<StageAnimationPlayer*>(m_stage->getPlayer());
	assert(ani_player);
	//����ȱʡ����
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
	m_ani_player = new StageAnimationPlayerEditor(this);//����������
#else
	m_ani_player = new StageAnimationPlayer(this);//����������
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
//ȡԤ���嶯̬Ч�������ļ���
const char* Stage::getDefinedObjectFilename()
{
	return m_defined_object_file_path.c_str();
}

void Stage::setDefinedObjectFileName(const char* filePath)
{
	assert(NULL != filePath);
	m_defined_object_file_path = filePath;
}

//��ʼ��
bool Stage::initialize()
{
	BiXmlHelper::LoadXMLDocument(getDefinedObjectFilename());
	//m_actor_manager->initialize(); �˳�ʼ����δ���
	if(!m_ani_player->loadEditInfoTable(getDefinedObjectFilename(),false))//�˴������˷�bin ��������Ҫ���´���һ�� lishan 101125
	{
		//LogError("[Stage::initialize] load obj file failed:",getDefinedObjectFilename());
		return false;
	}
	//����������Ԥ������Ϣ
	m_defined_object_factory->setDefinedItemCollection(m_ani_player->getDefinedItemCollection());

	BiXmlHelper::ClearXMLDocument();
	return true;
} ;
bool Stage::initialize(H3DI::IRender* irender, ITargetPositionMng* targetposmng, H3DI::ILevel* ilevel/* = NULL*/)
{
	if(!m_actor_manager->initialize(irender, ilevel)) //�˳�ʼ����δ���
		return false;

	if (get_pool())
	{
		get_pool()->registerStage(this);
		switchToLod(get_pool()->getLodLevel());
	}

	m_ani_player->setTargetPositionManager(targetposmng);

	BiXmlHelper::LoadXMLDocument(getDefinedObjectFilename());
	if(!m_ani_player->loadEditInfoTable(getDefinedObjectFilename(),false))//�˴������˷�bin ��������Ҫ���´���һ�� lishan 101125
	{
		//LogError("[Stage::initialize2] load obj file failed:",getDefinedObjectFilename());
		return false;
	}
	//����������Ԥ������Ϣ
	m_defined_object_factory->setDefinedItemCollection(m_ani_player->getDefinedItemCollection());

	BiXmlHelper::ClearXMLDocument();
	return true;	
}
//����
void Stage::reset()
{//how
	assert(false);
} ;
//���ݽڵ���Ϣ�ݹ齨����̬Ч������
DynamicEffectObject* Stage::createStageDynamicEffectObjectRecursive(BiXmlNode* effectObjNode)
{
	const char* id = BiXmlUtility::get_attribute(effectObjNode, "id");
	assert(id);

	const char* defined_type = BiXmlUtility::get_attribute(effectObjNode, "defined_type");
	assert(defined_type);

	//����actor	
	DynamicEffectObject* obj = m_defined_object_factory->createDynamicActor(id,defined_type);
	assert(obj);
	//���Ҽ�������
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
//��ȡstage�ļ���������̬Ч������
bool Stage::loadStageDynamicEffectObjects(const char* stagefile0, bool binary, I_Keep_Client_Alive* pLp)
{
	//create dynamic effect object
	assert(!binary);

	//ת���ָ���
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
	// ��ȡ����
	BiXmlNode* stageRootNode = doc->FirstChild("Stage");
	if (stageRootNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	// ��ȡ��̬Ч������Ľڵ�
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
//������̨��һЩ������Ϣ�����磺��̨���͡������ɫֵ
bool Stage::loadStageMiscInfo(const char* stageFile, bool isBinary)
{
	assert(!isBinary);

	// ��ȡ��̨�����Լ������ɫ
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
		// ��ȡ����
		BiXmlNode* stageRootNode = doc->FirstChild("Stage");
		if (stageRootNode != NULL)
		{
			// �ж���������ڳ���
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
// ������̨�ϵĶ�̬Ч������
bool Stage::loadDynamicObjects(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp /* = 0 */)
{
	// ������̨�ϵĶ�̬Ч���壬������ÿ�������ȱʡ����
	if (!loadStageDynamicEffectObjects(stageFile, isBinary, pLp))
		return false;

	// Ϊÿ������Ķ�̬Ч�����彨��ȱʡ����̨Ч��
	if (!m_ani_player->loadDefaultStageClips(stageFile, isBinary))
		return false;

	// ����ó�����������������̨Ч�����б�
	if (!m_ani_player->loadStageEffectListPath(stageFile, isBinary))
		return false;

	// ��ȡ��̨�����Լ������ɫ
	if(!loadStageMiscInfo(stageFile, isBinary))
		return false;

	m_actor_manager->get_level_entity()->RestruceOctree();

	switchToLod(m_stage_lod);

	BiXmlHelper::ClearXMLDocument();
	return true;
}
// ��ȡ��̨��Ϣ����̬Ч�����塢��̨Ч����
bool Stage::loadStageFile(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp)
{
	if(!loadStageDynamicEffectObjects(stageFile,isBinary, pLp))
		return false;	
	if(!m_ani_player->loadStageFile(stageFile,isBinary, pLp))
		return false;

	// ��ȡ��̨�����Լ������ɫ
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
			// ��ȡ����
			BiXmlNode* stageRootNode = doc->FirstChild("Stage");
			if (stageRootNode != NULL)
			{
				// �ж���������ڳ���
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
// ��ȡ�籾��Ϣ
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
	//�������stage����
	if(!clip_mng->generateStagePropertyInfo(item, obj  ))
	{
		return false;
	}
	//��� ����ʵ������
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
	//����ڵ�
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
	assert(!isBinary);//�в�֧��2����
	//BiXmlDocument* doc = new BiXmlDocument();
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();
	if (doc == NULL)
		return false;

	//ת���ָ���
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(stage_file0,fullFileName);
	//ȡ����·��
	std::string working_path ;
	MiddleLayerFilePathHelper::getWorkingPath(stage_file0,working_path);

	doc->LoadFile(fullFileName.c_str());
	// �ҵ�Stage�ڵ�
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
	//���泡���г��˵�ǰlod����֮�������ϵͳ����

	if(!saveStageSystemObjects(subEntitysNode))
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	//���浱ǰlod�����е���������
	// ���õ�����
	m_actor_manager->reset_iterator();
	Actor* actor = m_actor_manager->next();
	
	while(actor != NULL)
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(actor);
		if(obj != NULL )
		{
			if(!saveStageDynamicEffectObjectRecursive(subEntitysNode,obj,m_ani_player->getClipManger()))
			{
				//delete doc;//�������󲻱��棬ֱ�ӷ���
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
// ������̨��Ϣ����̬Ч�����塢��̨Ч����
bool Stage::saveStageFile(const char* stageFile, bool isBinary, bool isOptimize)
{
	if(!saveStageDynamicEffectObjects(stageFile,isBinary))
		return false;
	if(!m_ani_player->saveStageFile(stageFile,isBinary, isOptimize))
		return false;
	return true;
} ;

// ����籾��Ϣ
bool Stage::saveAnimationFile(const char* levelFile, bool isBinary)
{
	if(!m_ani_player->saveAnimationFile(levelFile,isBinary))
		return false;
	return true;
} ;

//��ö���������
IStageAnimationPlayer* Stage::getPlayer()
{
	return m_ani_player;
};
//��ȡActorMng
ActorManager* Stage::getActorMng()
{
	return m_actor_manager;
} ;
//����Ԥ���嶯̬Ч������
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
//������弶����ϵ,parent_obj��ActorManager�еĶ������塣���Stage clip���Ѿ�����sub_obj������Ҫ����ɾ�����á�
bool Stage::addObjectLink(DynamicEffectObject* parent_obj,DynamicEffectObject* sub_obj)
{
	//�ı���������Ӽ�CLIP�еĹ�ϵ
	assert(m_actor_manager->find_actor(parent_obj->get_name()) );
	//actorManager�Ƴ�������
	m_actor_manager->remove_actor(sub_obj);
	//��ӵ���������
	parent_obj->addSubObject(sub_obj);
	//��clip��ɾ��������
	StageAnimationClipMng* clip_mng = m_ani_player->getClipManger();

	clip_mng->resetIterator();
	while(StageAnimationClip* ani_clip = clip_mng->next()){
		ani_clip->remove(sub_obj,false);////���м���ʱ�ƶ�ʱ��ɾ��clip
	}

	return true;
};
//�Ƴ����弶����ϵ,parent_obj��ActorManager�еĶ������塣���Stage clip���Ѿ�����parent_obj��sub_obj���ŵ�parent_obj���ڵ�clip�����С�
bool Stage::removeObjectLink(DynamicEffectObject* parent_obj,DynamicEffectObject* sub_obj)
{
	assert(m_actor_manager->find_actor(parent_obj->get_name()));
	//�Ӹ��������Ƴ�
	if(!parent_obj->removeSubObject(sub_obj))
		return false;
	//��ӵ�actorManager
	m_actor_manager->add_actor(sub_obj);
	//clip�����������
	StageAnimationClipMng* clip_mng = m_ani_player->getClipManger();

	clip_mng->resetIterator();
	while(StageAnimationClip* ani_clip = clip_mng->next()){
		if(ani_clip->findEffectObject(parent_obj->get_name()))//sub_obj���ŵ�parent_obj���ڵ�clip������
			ani_clip->insert(sub_obj);
	}
	return true;
};

//�����ָ�����ת��Ϊ "**/**/**"
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
//ȡ����·����ת��Ϊ "**/**/**"
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
//��̬������弰��̨Ч��
bool Stage::addDynamicObjectAndEffect(const char* objectName,const char* type,const char* effectPath, bool isBinary)
{
	DynamicEffectObject* object = createDefinedObject(objectName, type);
	if(!object)
		return false;
	getActorMng()->add_actor(object);

	return m_ani_player->getClipManger()->loadEffect(effectPath,false);
};
//����idȡ����ļ�
const char* Stage::getCameraFile(const char* cameraName)
{
	return m_ani_player->getClipManger()->getCameraFile(cameraName);	
};

bool Stage::switchToLod(int lod)
{
	//�л������е�ϵͳ������ƹ�
	getActorMng()->remove_sysactor_from_actorlist(LL_ZERO);
	getActorMng()->remove_sysactor_from_actorlist(LL_ONE);
	getActorMng()->remove_sysactor_from_actorlist(LL_TWO);
	
	//�л���̨�ϵĶ�̬����
	getActorMng()->get_level_entity()->switch_to_lod(lod);

	getActorMng()->add_sysactor_to_actorlist(lod);

	//�л���������
	switchSceneEnv(lod);
	//�л���̬����
	StageAnimationPlayer* player = dynamic_cast<StageAnimationPlayer*>(getPlayer());
	player->switchStaticActorLod(lod);
	//���������е�clip���л�
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

	//���ý����������ʧ����
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