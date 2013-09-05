/**
* @file animation_script.h
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

AnimationScript::AnimationScript(IStage* stage)
:m_stage(stage)
{
	resetIterator();
	m_frame_per_second = STD_FRAME_PER_SECOND;
};
AnimationScript::~AnimationScript()
{
	clear();
};

void AnimationScript::clear()
{
	for(std::list<StageAnimationCuttingData*>::iterator pos = m_info_list.begin(); pos != m_info_list.end(); ++pos)
	{
		delete *pos;
	}
	m_info_list.clear();
	resetIterator();
};
//�Բ����б�ʱ��������Ҫ���ڲ��ſ�ʼ��CLIP���ݽ��м���ʱ��
void AnimationScript::sortCuttingData()
{
	m_info_list.sort(StageAnimationCuttingData());//��()�Ƚ�
};
// ���ö�����Ϣ������
void AnimationScript::resetIterator() 
{
	m_info_pos = m_info_list.begin();
};
// ��ö�����Ϣ��������ָ����һ��������Ϣ�����������Ч����0
StageAnimationCuttingData* AnimationScript::next(StageAnimationCuttingData** cuttingDataAfterThis) 
{
	if(cuttingDataAfterThis)
		*cuttingDataAfterThis = NULL;
	if(m_info_pos != m_info_list.end())
	{
		std::list<StageAnimationCuttingData*>::iterator posTemp = m_info_pos;
		for(; posTemp != m_info_list.end() ; posTemp++)
		{
			if((*posTemp)->m_data_type != StageAnimationCuttingData_Common)
				continue;
			if((*posTemp)->m_begin_bar < (*m_info_pos)->m_end_bar - 0.0001)//����ǰ��������pass
				continue;
			if((*posTemp)->m_begin_bar < (*m_info_pos)->m_end_bar + 0.0001)//��һ������������
			{
				if(cuttingDataAfterThis)
					*cuttingDataAfterThis = (*posTemp);//���ؽ��ڵ�cuttingData
			}
			break;
		}
		return *(m_info_pos++);
	}
	else
	{
		return 0;
	}
};

//StageAnimationCuttingData* AnimationScript::getFirstCuttingData()
//{
//	if (m_info_list.size() != 0)
//	{
//		resetIterator();
//		while(StageAnimationCuttingData* cuttingData = next())//����
//		{
//			if(cuttingData->m_data_type == StageAnimationCuttingData_Camera)//���������modifier
//				continue;
//			else
//				return cuttingData;
//		}
//	}
//	else
//		return 0;
//}
//
//StageAnimationCuttingData* AnimationScript::getLastCuttingData()
//{
//	if (m_info_list.size() != 0)
//	{
//		std::list<StageAnimationCuttingData*>::reverse_iterator iter = m_info_list.rbegin();
//		while (iter != m_info_list.rend())
//		{
//			if((*iter)->m_data_type == StageAnimationCuttingData_Camera)//���������modifier
//				++iter;
//			else
//				return *iter;
//		}
//	}
//	else
//		return 0;
//}

StageAnimationCuttingData* AnimationScript::getCurrentCuttingData()
{
	if(m_info_pos == m_info_list.end())
		return 0;
	
	return *(m_info_pos);
}
// �����������͵Ķ���������Ϣ
void AnimationScript::insertStageAnimationCuttingData(StageAnimationCuttingData* info)
{
	std::list<StageAnimationCuttingData*>::iterator pos = m_info_list.begin();
	for(; pos != m_info_list.end(); ++pos)
	{
		if((*pos)->m_begin_bar > info->m_begin_bar)
		{
			m_info_list.insert(pos,info);
			resetIterator();
			return;
		}
	}
	
	m_info_list.push_back(info);
	resetIterator();
} ;
// ���ٸ����Ķ���������Ϣ
void AnimationScript::removeStageAnimationCuttingData(StageAnimationCuttingData* info)
{
	std::list<StageAnimationCuttingData*>::iterator pos = m_info_list.begin();
	for(; pos != m_info_list.end(); ++pos)
	{
		if(*pos == info)
		{
			m_info_list.erase(pos);
			return;
		};
	}
	if(pos == m_info_list.end())
	{
		assert(!"illegal removeStageAnimationCuttingData!");
	}
	resetIterator();
};

void AnimationScript::adjustStageAnimationCuttingData(
	StageAnimationCuttingData* info, float begin_bar, float end_bar)
{
	assert(info);
	assert(end_bar > begin_bar);
	if (info)
	{
		info->m_begin_bar = begin_bar;
		info->m_end_bar = end_bar;
	}
}

bool AnimationScript::parseSpecifyNode(BiXmlNode* rootNode, int mTrackType)
{	
	BiXmlNode* item = 0;
	while (item = rootNode->IterateChildren("Item", item))
	{
		BiXmlNode* beg = item->FirstChild("begin");
		BiXmlNode* end = item->FirstChild("end");
		if (beg == 0 || end == 0)
		{
			return false;
		}

		/*const char* begstr = BiXmlUtility::get_value((BiXmlElement*)beg);
		const char* endstr = BiXmlUtility::get_value((BiXmlElement*)end);*/
		const char* begstr = BiXmlUtility::get_value(beg->ToElement());
		const char* endstr = BiXmlUtility::get_value(end->ToElement());
		if (begstr == 0 || endstr == 0)
		{
			return false;
		}

		double ibeg = atof(begstr);
		double iend = atof(endstr);

		StageAnimationCuttingData* newItem = new StageAnimationCuttingData();
		newItem->m_begin_bar = ibeg;
		newItem->m_end_bar = iend;
		if (mTrackType == 0)
		{
			const char* pcamid = BiXmlUtility::get_attribute(item, "camid");
			if (pcamid == NULL)
			{
				delete newItem;
				return false;
			}
			newItem->m_animation_clip_id = pcamid;
		}
		else if (mTrackType == 1)
		{
			const char* peffectid = BiXmlUtility::get_attribute(item, "effectid");
			if (peffectid == NULL)
			{
				delete newItem;
				return false;
			}
			newItem->m_animation_clip_id = peffectid;
		}
		else
		{
			newItem->m_animation_clip_id = "";
		}

		//��ȡ����
		const char* data_type = BiXmlUtility::get_attribute(item, "datatype");
		if(data_type != NULL)
		{
			if(stricmp(data_type, "common") == 0)
				newItem->m_data_type = StageAnimationCuttingData_Common;
			else if(stricmp(data_type, "camera") == 0)
				newItem->m_data_type = StageAnimationCuttingData_Camera;
			else
				assert(!"CuttingData parse, ���Ͳ�֧��!");
		}

		//��ȡ�Ƿ�ѭ������
		const char* is_loop = BiXmlUtility::get_attribute(item, "is_loop");
		if(is_loop != NULL)
		{
			if(stricmp(is_loop, "true") == 0)
				newItem->m_loop = true;
			else if(stricmp(is_loop, "false") == 0)
				newItem->m_loop = false;
			else
				assert(!"CuttingData parse, ���Ͳ�֧��!");
		}

		//������ʼ֡��������˳�����
		
		m_info_list.push_back(newItem);
	}
	return true;	
};
bool AnimationScript::load(const char* filename, bool isBinary)
{
	assert(!isBinary);
	clear();

	BiXmlHelper::LoadXMLDocument(filename);
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(filename, true);
	if(doc == NULL)
	{
		//LogError("[AnimationScript::load] load ani file failed:",filename);
		assert(0);
		return false;
	}
	BiXmlNode* tnode = doc->FirstChild("Level");
	if (tnode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	BiXmlNode* levelInfoNode = tnode->FirstChild("LevelInfo");
	if (levelInfoNode)
	{
		BiXmlNode* bpmNode = levelInfoNode->FirstChild("BPM");
		const char* bpm_str = BiXmlUtility::get_value(bpmNode->ToElement());
		double bpm = atof(bpm_str);

		m_frame_per_second = STD_FRAME_PER_SECOND * bpm / 100;
	}

	// �洢��̨Ч�������������
	BiXmlNode* stageRootNode = tnode->FirstChild("Stage");
	if (stageRootNode != NULL)
	{	//��ȡʱ�� Effects �� Cams 2 ���ڵ����Ϣ������ʱ�����浽 Effects �ڵ㡣lishan 101116
		// �洢��̨Ч���Ľڵ�
		BiXmlNode* effectNode = stageRootNode->FirstChild("Effects");
		if (effectNode != NULL)
		{
			if (!parseSpecifyNode(effectNode, 1))
			{
				//delete doc;
				XmlDocFactory::ReleaseDocument(doc);
				return false;
			}
		}

		// �洢����Ľڵ�
		BiXmlNode* camNode = stageRootNode->FirstChild("Cams");
		if (camNode != NULL)
		{
			if (!parseSpecifyNode(camNode, 0))
			{
				//delete doc;
				XmlDocFactory::ReleaseDocument(doc);
				return false;
			}
		}
	}

	resetIterator();

	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return true;
};
bool AnimationScript::save(const char* filename, bool isBinary)
{
	assert(!isBinary);//�в�֧��2����
	//BiXmlDocument* doc = new BiXmlDocument();
	BiXmlDocument* doc = XmlDocFactory::CreateDocument();
	if (doc == NULL)
		return false;

	doc->LoadFile(filename);

	// ���ڵ� 
	BiXmlNode* levelNode = doc->FirstChild("Level");
	if (levelNode == NULL)
	{
		//levelNode = new BiXmlElement("Level");
		levelNode = doc->InsertEndChild("Level");
		//doc->LinkEndChild(levelNode);
	}
	// �ҵ�Stage�ڵ�
	BiXmlNode* stageNode = levelNode->FirstChild("Stage");
	if (stageNode == NULL)
	{
		/*stageNode = new BiXmlElement("Stage");
		levelNode->LinkEndChild(stageNode);*/
		stageNode = levelNode->InsertEndChild("Stage");
	}
	stageNode->Clear();

	// �����̨Ч�������
	//BiXmlElement effectNodes("Effects"); //��ȡʱ�� Effects �� Cams 2 ���ڵ����Ϣ������ʱ�����浽 Effects �ڵ㡣lishan 101116
	BiXmlElement* effectNodes = stageNode->InsertEndChild("Effects")->ToElement(); //��ȡʱ�� Effects �� Cams 2 ���ڵ����Ϣ������ʱ�����浽 Effects �ڵ㡣lishan 101116
	
	for (std::list<StageAnimationCuttingData*>::iterator i = m_info_list.begin(); i != m_info_list.end(); ++i)
	{
		//BiXmlElement item("Item");
		BiXmlElement* item = effectNodes->InsertEndChild("Item")->ToElement();

		item->SetAttribute("effectid", (*i)->m_animation_clip_id.c_str());

		if((*i)->m_data_type == StageAnimationCuttingData_Common)
		{
			item->SetAttribute("datatype", "common");
		}
		else if((*i)->m_data_type == StageAnimationCuttingData_Camera)
		{
			item->SetAttribute("datatype", "camera");
		}
		else
			assert(!"Cutting save, ���Ͳ�֧��!");

		//�����Ƿ�ѭ��������
		(*i)->m_loop
			? item->SetAttribute("is_loop","true")
			: item->SetAttribute("is_loop","false");
		
		//BiXmlElement beg("begin");
		BiXmlElement* beg = item->InsertEndChild("begin")->ToElement();
		char begstr[256];
		memset(begstr, 0, 256);
		//_itoa_s((*i)->m_begin_bar, begstr, 256, 10);
		sprintf(begstr,"%f",(*i)->m_begin_bar);
		beg->SetText(begstr);
		//TiXmlText* begtxt = new TiXmlText(begstr);
		//beg.LinkEndChild(begtxt);
		//item.InsertEndChild(beg);
		
		//BiXmlElement end("end");
		BiXmlElement* end = item->InsertEndChild("end")->ToElement();
		char endstr[256];
		memset(endstr, 0, 256);
		//_ftoa_s((*i)->m_end_bar, endstr, 256, 10);
		sprintf(endstr,"%f",(*i)->m_end_bar);
		end->SetText(endstr);
		//TiXmlText* endtxt = new TiXmlText(endstr);
		//end.LinkEndChild(endtxt);
		//item.InsertEndChild(end);

		//effectNodes.InsertEndChild(item);
	}
	//stageNode->InsertEndChild(effectNodes);
	bool ret = doc->SaveFile(filename);
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);
	return ret;
};
// �����̨Ч��
StageAnimationCuttingData* AnimationScript::addStageAnimation(StageAnimationCuttingDataType type,const char* id,float beginBar,float endBar)
{
	StageAnimationCuttingData* aniCuttingData = new StageAnimationCuttingData();
	aniCuttingData->m_data_type = type;
	aniCuttingData->m_animation_clip_id=id;
	aniCuttingData->m_begin_bar = beginBar;
	aniCuttingData->m_end_bar = endBar;
	insertStageAnimationCuttingData(aniCuttingData);
	return aniCuttingData;
};


