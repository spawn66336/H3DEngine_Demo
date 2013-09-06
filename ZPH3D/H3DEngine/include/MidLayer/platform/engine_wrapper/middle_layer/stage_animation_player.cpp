/**
* @file stage_animation_player.h
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

#define FAILURE 0
#define SUCCESS 1
#define SECOND2MILLONSECONDS (1000.0f)
enum{
	STATIC_DATATYPE_BOOL,
	STATIC_DATATYPE_INT,
	STATIC_DATATYPE_FLOAT,
	STATIC_DATATYPE_COLOR,
	STATIC_DATATYPE_STRING,
	STATIC_DATATYPE_MAT3,
	STATIC_DATATYPE_LOCATIONMAT
};

#define CHAR_TEMP_MAX_SIZE 256

static void unicodeToUtf8(const char* str, char* des)
{
	int len = ::MultiByteToWideChar( CP_ACP, 0, str, strlen( str ) + 1, NULL, 0 );
	assert(len <= CHAR_TEMP_MAX_SIZE);
	wchar_t* unicodeString = new wchar_t[ len ];
	::MultiByteToWideChar( CP_ACP, 0, str, strlen( str ) + 1, unicodeString, len );

	len = ::WideCharToMultiByte( CP_UTF8, 0, unicodeString, wcslen( unicodeString ) + 1, NULL, 0, NULL, NULL );
	assert(len <= CHAR_TEMP_MAX_SIZE);
	::WideCharToMultiByte( CP_UTF8, 0, unicodeString, wcslen( unicodeString ) + 1, des, len, NULL, NULL );

	delete[] unicodeString;
}

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

static std::string fileFullPathExt(std::string strPath)
{
	std::string ext;
	if (strPath.length() < 1)
	{
		return ext;
	}

	strPath.erase(strPath.find_last_not_of(' ') + 1);

	std::string::size_type pos_dot = strPath.find_last_of('.');
	if (pos_dot != std::string::npos && (pos_dot + 1) < strPath.length())
	{
		ext = strPath.substr(pos_dot + 1);
	}

	return ext;
}

HaloConfig::HaloConfig()
:m_lensFalres(NULL)
,m_lensFlaresNums(0)
,m_load_succ(false)
{

}

HaloConfig::~HaloConfig()
{
	if(m_lensFalres)
		delete[] m_lensFalres;
};

bool HaloConfig::loadLensValue(BiXmlNode* halo_node)
{
	//��ȡlensEnv��lensPara
	m_load_succ = false;
	BiXmlNode *para_node = halo_node->FirstChild("LensPara");
	if(!para_node) 
	{
		//clLog(_T("LensFlareConfig file error!\n"));
		return false;
	}
	const char* numStr = BiXmlUtility::get_attribute(para_node, "LensFlareNums");
	if(numStr)
	{
		m_lensFlaresNums = atoi(numStr);
	}
	if(m_lensFlaresNums <= 0)
		return false;

	if(m_lensFalres)
		delete[] m_lensFalres;
	m_lensFalres = new H3DI::tLensFlareParameters[m_lensFlaresNums];
	m_lensTypeVec.resize(m_lensFlaresNums);
	m_lensMatVec.resize(m_lensFlaresNums);
	BiXmlNode *node1 = para_node->FirstChild("LensFlareElement");
	int idx = 0;
	while(node1)
	{
		const char* sizeStr = BiXmlUtility::get_attribute(node1, "LensFlareSize");
		const char* disStr = BiXmlUtility::get_attribute(node1, "LensFlareDis");
		const char* typeStr = BiXmlUtility::get_attribute(node1, "LensFlareType");
		const char* matStr = BiXmlUtility::get_attribute(node1, "LensFlareMat");
		if(sizeStr&&disStr&&typeStr)
		{
			m_lensFalres[idx].size = atof(sizeStr);
			m_lensFalres[idx].farFromSun = atof(disStr);
			m_lensTypeVec[idx] = typeStr;
			m_lensMatVec[idx] = matStr;
			m_lensFalres[idx].flareType = m_lensTypeVec[idx].c_str();
			m_lensFalres[idx].matName = m_lensMatVec[idx].c_str();
		}
		BiXmlNode* cNode = node1->FirstChild("LensFlareColor");
		const char* tmpStr = BiXmlUtility::get_attribute(cNode, "R");
		m_lensFalres[idx].color[0] = atof(tmpStr);
		tmpStr = BiXmlUtility::get_attribute(cNode, "G");
		m_lensFalres[idx].color[1] = atof(tmpStr);
		tmpStr = BiXmlUtility::get_attribute(cNode, "B");
		m_lensFalres[idx].color[2] = atof(tmpStr);
		tmpStr = BiXmlUtility::get_attribute(cNode, "A");
		m_lensFalres[idx].color[3] = atof(tmpStr);

		node1 = node1->NextSiblingElement("LensFlareElement");
		++idx;
	}
	node1 = para_node->FirstChild("LensEnvElement");
	if(node1)
	{
		const char* tmpStr = BiXmlUtility::get_attribute(node1, "MinAngle");
		m_lensEnv.minAngle = atof(tmpStr);
		tmpStr = BiXmlUtility::get_attribute(node1, "MaxAngle");
		m_lensEnv.maxAngle = atof(tmpStr);
		tmpStr = BiXmlUtility::get_attribute(node1, "AngleDamping");
		m_lensEnv.angleDamping = atof(tmpStr);
		tmpStr = BiXmlUtility::get_attribute(node1, "RangeDamping");
		m_lensEnv.sunRangeDamping = atof(tmpStr);
		tmpStr = BiXmlUtility::get_attribute(node1, "MaxRange");
		m_lensEnv.maxSunPixels = atoi(tmpStr);
		BiXmlNode* cNode = node1->FirstChild("SunPos");
		tmpStr = BiXmlUtility::get_attribute(cNode, "X");
		m_sunPos.x = atof(tmpStr);
		tmpStr = BiXmlUtility::get_attribute(cNode, "Y");
		m_sunPos.y = atof(tmpStr);
		tmpStr = BiXmlUtility::get_attribute(cNode, "Z");
		m_sunPos.z = atof(tmpStr);

		m_lensEnv.sunPos[0] = m_sunPos.x;
		m_lensEnv.sunPos[1] = m_sunPos.y;
		m_lensEnv.sunPos[2] = m_sunPos.z;

		//clLog("sunPos x=%f,y=%f,z=%f\n",m_sunPos.x,m_sunPos.y,m_sunPos.z);
	}
	m_load_succ = true;
	return m_load_succ;
};


StageAnimationPlayer::StageAnimationPlayer(IStage* stage)
:m_stage(stage)
,m_frame_per_second(0)
,m_time_scale(1.0f)
,m_play_mode(PLAY_STOP)
,m_level_playing_acct(0)
,m_cur_camera(0)
,m_cur_camera_clip(0)
,m_targetposmng(0)
,m_stage_loaded(false)
,m_env_loaded(false)
,m_loop(false)
,m_static_actor_color(1.0f)
,m_enableScene(true)
,m_playing_script(0)
//,m_play_by_script_sequence(false)
//,m_script_sequence_resetted(false)
{
	m_level_lod = LL_ZERO;
	m_defined_items = new CDefinedItemCollection();
	//m_edit_info_table = new CEditInfoTable();
	m_ani_clip_mng = new StageAnimationClipMng(stage);
	m_ani_script = new AnimationScript(stage);
	setFramePerSecond(STD_FRAME_PER_SECOND);//ȱʡֵ

	m_playing_script = new AnimationScript(stage);
	m_ani_script_list.insert(std::make_pair("default", m_playing_script));

	//������̬Ч�����壬������ActorManager����
	m_static_object = NULL;//new DynamicEffectObject(m_stage->getActorMng(),"__static_object","__static_object");
	m_static_object_loaded = false;

	static_property_map["LocationMat"] = std::make_pair<std::string>("location", STATIC_DATATYPE_LOCATIONMAT);
	static_property_map["�ƹ��Ƿ����"] = std::make_pair<std::string>("Light_Enable", STATIC_DATATYPE_BOOL);
	static_property_map["����Χ"] = std::make_pair<std::string>("Near_Range", STATIC_DATATYPE_FLOAT);
	static_property_map["Զ��Χ"] =  std::make_pair<std::string>("Far_Range", STATIC_DATATYPE_FLOAT);
	static_property_map["�ڽ�"] =  std::make_pair<std::string>("Inner_angle", STATIC_DATATYPE_FLOAT);
	static_property_map["���"] = std::make_pair<std::string>("Outter_angle", STATIC_DATATYPE_FLOAT);
	static_property_map["ǿ��"] =  std::make_pair<std::string>("Light_intensity", STATIC_DATATYPE_FLOAT);
	static_property_map["��ɫ"] = std::make_pair<std::string>("Light_Color", STATIC_DATATYPE_COLOR);
	static_property_map["�����Ƿ�ɼ�"] = std::make_pair<std::string>("Light_Beam_Visible", STATIC_DATATYPE_BOOL);
	static_property_map["��������"] = std::make_pair<std::string>("Light_Beam_Brightness", STATIC_DATATYPE_FLOAT);
	static_property_map["�����ھ�"] = std::make_pair<std::string>("Light_Beam_Inner_Radius", STATIC_DATATYPE_FLOAT);
	static_property_map["��������Χ"] = std::make_pair<std::string>("Light_Beam_Near_Range", STATIC_DATATYPE_FLOAT);
	static_property_map["����Զ��Χ"] = std::make_pair<std::string>("Light_Beam_Far_Range", STATIC_DATATYPE_FLOAT);
	//static_property_map["�����ڽǶ�"] = std::make_pair<std::string>("Light_Beam_Inner_Angle", STATIC_DATATYPE_FLOAT);
	static_property_map["������Ƕ�"] = std::make_pair<std::string>("Light_Beam_Outter_Angle", STATIC_DATATYPE_FLOAT);
	static_property_map["�������ģ����"] = std::make_pair<std::string>("Light_Post_Beam_Blur_Coeff", STATIC_DATATYPE_FLOAT);
	static_property_map["��������"] = std::make_pair<std::string>("Light_Beam_Type", STATIC_DATATYPE_STRING);
	static_property_map["���ȼ�"] =  std::make_pair<std::string>("Set_Light_Priority", STATIC_DATATYPE_INT);
	static_property_map["ResName"] = std::make_pair<std::string>("Resource", STATIC_DATATYPE_STRING);
	static_property_map["�Ƿ��������"] =std::make_pair<std::string>("Light_Shaft_Enable", STATIC_DATATYPE_BOOL);
	static_property_map["�������ɫ"] =  std::make_pair<std::string>("Light_Shaft_Color", STATIC_DATATYPE_COLOR);
	static_property_map["��������"] =  std::make_pair<std::string>("Light_Shaft_Depth", STATIC_DATATYPE_FLOAT);
	static_property_map["�������ֵ"] =  std::make_pair<std::string>("Light_Shaft_Threshold", STATIC_DATATYPE_FLOAT);
	static_property_map["�����ǿ��"] =  std::make_pair<std::string>("Light_Shaft_Intensity", STATIC_DATATYPE_FLOAT);
	//static_property_map["����"] = std::make_pair<std::string>("Rotation", STATIC_DATATYPE_MAT3); ��2������modifier����
	//static_property_map["��ʼλ��"] =  std::make_pair<std::string>("Position", STATIC_DATATYPE_MAT3);��2������modifier����
	static_property_map["�Ƿ������Ӱ"] = std::make_pair<std::string>("Enable_Light_Shadow", STATIC_DATATYPE_BOOL);
	static_property_map["��Ӱ��ɫ"] =  std::make_pair<std::string>("Light_Shadow_Color", STATIC_DATATYPE_COLOR);

	static_property_map["�Ƿ�Ͷ��ͼ��"] =  std::make_pair<std::string>("Enable_proj_texture", STATIC_DATATYPE_BOOL);
	static_property_map["ͼ��"] =  std::make_pair<std::string>("Proj_texture", STATIC_DATATYPE_STRING);
	static_property_map["�ƹ�Ӱ������"] = std::make_pair<std::string>("Light_Affect_Param", STATIC_DATATYPE_STRING);
	
	//point_light_set.insert("Position");
	point_light_set.insert("Visible");
	point_light_set.insert("Light_Color");
	point_light_set.insert("Light_intensity");
	point_light_set.insert("Set_Light_Priority");
	point_light_set.insert("Light_Enable");
	point_light_set.insert("Light_Affect_Param");
	point_light_set.insert("Enable_Light_Shadow");
	point_light_set.insert("Light_Shadow_Color");
	point_light_set.insert("Near_Range");
	point_light_set.insert("Far_Range");
	//point_light_set.insert("DRAW_LIGHT_BEAM_LINE"); �������
	point_light_set.insert("Light_Shaft_Enable");
	point_light_set.insert("Light_Shaft_Color");
	point_light_set.insert("Light_Shaft_Depth");
	point_light_set.insert("Light_Shaft_Threshold");
	point_light_set.insert("Light_Shaft_Intensity");

	//direction_light_set.insert("Rotation");
	direction_light_set.insert("Visible");
	direction_light_set.insert("Light_Enable");
	direction_light_set.insert("Light_Affect_Param");
	direction_light_set.insert("Light_Color");
	direction_light_set.insert("Light_intensity");
	direction_light_set.insert("Set_Light_Priority");
	direction_light_set.insert("Enable_Light_Shadow");
	direction_light_set.insert("Light_Shadow_Color");
	direction_light_set.insert("Light_Shaft_Enable");
	direction_light_set.insert("Light_Shaft_Color");
	direction_light_set.insert("Light_Shaft_Depth");
	direction_light_set.insert("Light_Shaft_Threshold");
	direction_light_set.insert("Light_Shaft_Intensity");


	//wimble_light_set.insert("Position");
	//wimble_light_set.insert("Rotation");
	wimble_light_set.insert("Visible");
	wimble_light_set.insert("Light_Color");
	wimble_light_set.insert("Light_intensity");
	wimble_light_set.insert("Set_Light_Priority");
	wimble_light_set.insert("Light_Enable");
	wimble_light_set.insert("Light_Affect_Param");
	wimble_light_set.insert("Enable_Light_Shadow");
	wimble_light_set.insert("Light_Shadow_Color");
	wimble_light_set.insert("Inner_angle");
	wimble_light_set.insert("Outter_angle");
	wimble_light_set.insert("Enable_proj_texture");
	wimble_light_set.insert("Proj_texture");
	wimble_light_set.insert("Near_Range");
	wimble_light_set.insert("Far_Range");
	wimble_light_set.insert("Light_Beam_Visible");
	wimble_light_set.insert("Light_Beam_Brightness");
	wimble_light_set.insert("Light_Beam_Inner_Radius");
	wimble_light_set.insert("Light_Post_Beam_Blur_Coeff");
	wimble_light_set.insert("Light_Beam_Type");
	wimble_light_set.insert("Light_Shaft_Enable");
	wimble_light_set.insert("Light_Shaft_Color");
	wimble_light_set.insert("Light_Shaft_Depth");
	wimble_light_set.insert("Light_Shaft_Threshold");
	wimble_light_set.insert("Light_Shaft_Intensity");

	//wimble_light_set.insert("Light_Beam_Inner_Angle");
	wimble_light_set.insert("Light_Beam_Outter_Angle");
	wimble_light_set.insert("Light_Beam_Near_Range");
	wimble_light_set.insert("Light_Beam_Far_Range");
	static_property_map["�����ھ�"] = std::make_pair<std::string>("Light_Beam_Inner_Radius", STATIC_DATATYPE_FLOAT);
	
	//ȫ�ֹ�����,���ֻ���ڼ�¼���壬�������ͷ�
//	DynamicEffectObject::s_system_object = NULL;

	m_entertime = 0;

	m_trans_color[0]=m_trans_color[1]=m_trans_color[2]=m_trans_color[3]=0.0f;
// 	m_ambient_color[0]=m_ambient_color[1]=m_ambient_color[2]=m_ambient_color[3]=0.0f;
// 	m_ambient_actor_color[0]=m_ambient_actor_color[1]=m_ambient_actor_color[2]=m_ambient_actor_color[3]=0.0f;
	m_shadow_scope = 100.0f;

	//m_optimize_light = false;
};
StageAnimationPlayer::~StageAnimationPlayer()
{
	// 	H3DI::IRender* renderer = m_stage->getActorMng()->get_graphic_res_mng()->get_render_system()->get_IRender();
	// 	renderer->BeginSetCurrentLight();
	// 	renderer->EndSetCurrentLight();

	if(m_ani_clip_mng)
		delete m_ani_clip_mng;

	if(m_ani_script)
		delete m_ani_script;

	std::map<std::string, AnimationScript*>::iterator iter = m_ani_script_list.begin();
	while (iter != m_ani_script_list.end())
	{
		delete iter->second;
		iter->second = NULL;
		++iter;
	}
	m_ani_script_list.clear();

	m_playing_data_info.clear();

	m_playing_clip_set.clear();

	m_playing_object_set.clear();

	//if(m_edit_info_table)
	//	delete m_edit_info_table;

	if(m_defined_items)
		delete m_defined_items;

	if(m_static_object)
	{
		m_stage->getActorMng()->get_level_entity()->remove_static_object(m_static_object);
		delete m_static_object;
		m_static_object = NULL;
	}

//	DynamicEffectObject::s_system_object = NULL;
};
//����ÿ��֡��
void StageAnimationPlayer::setFramePerSecond(double frame) 
{
	m_time_scale = frame/STD_FRAME_PER_SECOND;//ʱ�����ű���

	m_frame_per_second = frame;

	CSystemInfo::instance()->set_frame_time(1000.0f/STD_FRAME_PER_SECOND);//�м�����ñ�׼֡ʱ�䣬�ⲿ����ͳһʱ������
};

//interface func
//��Ⱦ
void StageAnimationPlayer::render()
{
	//������Ӱ��Χ
	ActorManager* actor_mng = m_stage->getActorMng();
	CRenderSystem& render_system = *(actor_mng->get_graphic_res_mng()->get_render_system());
	actor_mng->get_level_entity()->set_shadow_scope(m_shadow_scope);
	actor_mng->get_level_entity()->begin_render();
	actor_mng->get_level_entity()->render(render_system);
	actor_mng->get_level_entity()->end_render();
	return;

	//���û�д����κ�һ�ֲ���ģʽ��ȫ����Ⱦ
	//if (m_play_mode == PLAY_STOP)
	//{
	//	renderAll();
	//}
	//else
	//{
	//	//��Ⱦ�����Ķ�̬��Ч����
	//	if (m_play_mode & PLAY_OBJECT)
	//	{
	//		renderDyanmicEffectObject();
	//	}

	//	//��Ⱦ�����Ķ���
	//	if (m_play_mode & PLAY_CLIP)
	//	{
	//		renderStageAnimationClip();
	//	}

	//	//��Ⱦ�ؿ�
	//	if (m_play_mode & PLAY_LEVEL)
	//	{
	//		renderLevel();
	//	}

	//	renderStaticObject();
	//}
}
//void StageAnimationPlayer::render() 
//{
//	if( m_play_mode == PLAY_LEVEL )
//	{
//		ActorManager* actor_mng = m_stage->getActorMng();
//		CRenderSystem* render_system = actor_mng->get_graphic_res_mng()->get_render_system();;
//
//		if(m_static_object)//���ƾ�̬����
//		{
//			m_static_object->render(*render_system);
//		}
//
//		actor_mng->reset_iterator();
//
//		while(Actor* p = actor_mng->next())
//		{
//			DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
//			obj->render(*render_system);
//		};
//	}
//	else if( m_play_mode == PLAY_CLIP )
//	{
//		ActorManager* actor_mng = m_stage->getActorMng();
//		CRenderSystem* render_system = actor_mng->get_graphic_res_mng()->get_render_system();;
//
//		if(m_static_object)//���ƾ�̬����
//		{
//			m_static_object->render(*render_system);
//		}
//
//		for(std::set<StageAnimationClip*>::iterator pos = m_playing_clip_set.begin()
//			; pos != m_playing_clip_set.end(); ++pos)
//		{
//			StageAnimationClip* ani_clip = *pos;
//			ani_clip->render();
//		}	
//	}
//	else if( m_play_mode == PLAY_OBJECT )
//	{
//		ActorManager* actor_mng = m_stage->getActorMng();
//		CRenderSystem* render_system = actor_mng->get_graphic_res_mng()->get_render_system();;
//
//		if(m_static_object)//���ƾ�̬����
//		{
//			m_static_object->render(*render_system);
//		}
//
//		for(std::set<DynamicEffectObject*>::iterator pos = m_playing_object_set.begin()
//			; pos != m_playing_object_set.end(); ++pos)
//		{
//			DynamicEffectObject* dynamic_effect_object = *pos;
//			dynamic_effect_object->render(*render_system);
//		}
//	}
//	else
//	{
//		renderAll();
//	}
//};
//����, ��λ����
void StageAnimationPlayer::update(float tInterval)
{
	if (m_play_mode == PLAY_STOP)
	{
		tInterval = tInterval * m_time_scale;
		updateAll(tInterval);
	}
	else
	{
		if (m_play_mode & PLAY_OBJECT)
		{
			updateDynamicEffectObject(tInterval);
		}
		if (m_play_mode & PLAY_CLIP)
		{
			updateStageAnimationClip(tInterval);
		}
		if (m_play_mode & PLAY_LEVEL)
		{
			updateLevel(tInterval);
		}

		updateStaticObject(tInterval);
	}
}

//void StageAnimationPlayer::update(float tInterval)
//{
//	if( m_play_mode == PLAY_LEVEL )
//	{
//		m_level_playing_acct += tInterval;		
//
//		double t_scaled = tInterval * m_time_scale;
//		//if(t > 1000)
//		//	t = t;
//
//		/// �ؿ����Ŷ�Ӧ��֡��
//		/// fixme: Ӧ�ó���ÿ֡��Ӧ��ʱ��
//		double level_playing_acct_frame = ( m_level_playing_acct / 1000.0f ) * m_frame_per_second;
//
//		// ������Ҫֹͣ����̨Ч��
//		for(std::set<StageAnimationCuttingData*>::iterator pos = m_playing_data_info.begin()
//			; pos != m_playing_data_info.end(); )
//		{
//			StageAnimationCuttingData* ii = *pos;
//			
//			// �Ƿ�ʱ
//			if(ii->m_end_bar*STD_FRAME_PER_BAR <= level_playing_acct_frame )//С�ڻ���Ϊ��׼֡�������ź��֡���бȽ�
//			{
//				//����playing clip
//				StageAnimationClip* ani_clip = getClipManger()->find(ii->m_animation_clip_id.c_str());
//				if(ani_clip != NULL)// ֹͣ����
//				{
//					ani_clip->play(false,0.0f);;//ֹͣ����ʱ�������Ƿ����
//					ani_clip->setCurrentCuttingData(NULL);
//				}
//				pos = m_playing_data_info.erase(pos);
//			}
//			else
//				++pos;
//		}
//	
//		// ������Ҫ��ʼ����̨Ч�����������Ѿ�������ʱ����и���
//		getAnimationScript()->resetIterator();
//		while(StageAnimationCuttingData* ii = getAnimationScript()->next())
//		{
//			// �Ƿ��ڷ�Χ��
//			if(level_playing_acct_frame >= ii->m_begin_bar*STD_FRAME_PER_BAR && level_playing_acct_frame < ii->m_end_bar*STD_FRAME_PER_BAR )//С�ڻ���Ϊ��׼֡�������ź��֡���бȽ�
//			{
//				// ��ӻ�������
//				if(m_playing_data_info.find(ii) == m_playing_data_info.end())
//				{
//					StageAnimationClip* ani_clip = getClipManger()->find(ii->m_animation_clip_id.c_str());
//					if(ani_clip == NULL)//���clip�����ڣ�����
//						continue;
//
//					m_playing_data_info.insert(ii);//�������ڲ��ŵĶ���
//					
//					int clip_frame_count = ani_clip->getFrameCount();
//					double ani_frame_count = (ii->m_end_bar - ii->m_begin_bar) * STD_FRAME_PER_BAR;//ÿ��60
//
//					if(ani_clip->getClipType() == StageAnimationClip_Camera) // ���
//					{
//						m_cur_camera_clip = ani_clip;
//						//����ʱ����е���,��ʼλ�ð����ź��ʱ����е�����������
//						ani_clip->play(true,(m_level_playing_acct* m_time_scale - ii->m_begin_bar * STD_SECONDS_PER_BAR * 1000.0f) * clip_frame_count / ani_frame_count);
//
//						if(CameraEffectObject* cam = getCurCamera())
//						{
//							cam->getCamera()->set_target_position_mng(m_targetposmng);
//							cam->getCamera()->play_camera();
//						}
//					}
//					else if(ani_clip->getClipType() == StageAnimationClip_Common)
//					{
//						//����ʱ����е���,��ʼλ�ð����ź��ʱ��������ã�������
//						ani_clip->setCurrentCuttingData(ii);//����cutting data������������ �ָ� default ����
//						ani_clip->play(true,(m_level_playing_acct* m_time_scale - ii->m_begin_bar * STD_SECONDS_PER_BAR * 1000.0f));//��ͨЧ����������lishan 110714 * clip_frame_count / ani_frame_count);
//						ani_clip->setBeginBar(ii->m_begin_bar);//��ͨ��̨Ч������ѭ�����ţ�����һ����ʼС��
//					}
//					else
//						assert(0);
//				}
//			}
//		}
//
//		// ������Ҫ���µ���̨Ч��
//		for(std::set<StageAnimationCuttingData*>::iterator pos = m_playing_data_info.begin()
//			; pos != m_playing_data_info.end(); )
//		{
//			StageAnimationCuttingData* ii = *pos;
//			//����playing clip
//			StageAnimationClip* ani_clip = getClipManger()->find(ii->m_animation_clip_id.c_str());
//			if(ani_clip == NULL)
//			{
//				pos = m_playing_data_info.erase(pos);//�Ƴ������ڵĽű�
//				continue;
//			};
//			// �Ƿ����
//			if(ii->m_end_bar*STD_FRAME_PER_BAR > level_playing_acct_frame)//С�ڻ���Ϊ��׼֡�������ź��֡���бȽ�
//			{
//
//				int camera_frame_count = ani_clip->getFrameCount();
//				double ani_frame_count = (ii->m_end_bar - ii->m_begin_bar) * STD_FRAME_PER_BAR;//ÿ��60
//
//				if(ani_clip->getClipType() == StageAnimationClip_Camera) // ���
//				{
//					m_cur_camera_clip = ani_clip;
//					ani_clip->update(t_scaled * camera_frame_count / ani_frame_count);//�������ʱ����а���������
//				}
//				else if(ani_clip->getClipType() == StageAnimationClip_Common)
//				{//��ͨ��̨Ч������ѭ������
//					float clipBeginBar = ani_clip->getBeginBar();//�õ�clip��ʼС��
//					float clipTotalBar = (float)camera_frame_count/STD_FRAME_PER_BAR;//�õ�clip֡����Ӧ��С��
//					
//					float clipEndBar = clipBeginBar + clipTotalBar;
//
//					if(m_level_playing_acct* m_time_scale >= clipEndBar *  STD_SECONDS_PER_BAR * 1000.0f && !m_loop)//������̨Ч��֡����
//					{
//						//���²���
//						ani_clip->play(true,(m_level_playing_acct* m_time_scale - clipEndBar * STD_SECONDS_PER_BAR * 1000.0f));
//						ani_clip->setBeginBar(clipEndBar);//�����µĿ�ʼλ��
//					}
//					else
//						ani_clip->update(t_scaled);////��ͨЧ����������lishan 110714  * camera_frame_count / ani_frame_count);//����ʱ����а���������			
//				}
//				else
//					assert(0);
//			}
//			++pos;
//		}
//		if(m_static_object)//���¾�̬����
//		{
//			m_static_object->update(t_scaled);
//		}
//
//		ActorManager* actor_mng = m_stage->getActorMng();
//		actor_mng->reset_iterator();
//		while(Actor* p = actor_mng->next())
//		{
//			DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
//			if(!obj->m_object_updated)
//				obj->update(t_scaled);
//			else
//				obj->m_object_updated = false;
//		};
//
//		//���û��clip
//		bool no_clip = m_playing_data_info.size() == 0;
//		if (no_clip)
//		{
//			if (m_loop)
//			{
//				m_level_playing_acct = 0;
//				playLevel(false);
//				playLevel(true);
//				update(0);
//			}
//			else
//			{
//				//resetPlayMode();
//			}
//		}
//	}
//	else if( m_play_mode == PLAY_CLIP )
//	{
//		tInterval = tInterval * m_time_scale;
//
//		for(std::set<StageAnimationClip*>::iterator pos = m_playing_clip_set.begin()
//			; pos != m_playing_clip_set.end(); ++pos)
//		{
//			StageAnimationClip* ani_clip = *pos;
//			ani_clip->update(tInterval);
//
//			if (ani_clip->getClipType() == StageAnimationClip_Camera)
//			{
//				m_cur_camera_clip = ani_clip;
//			}
//
//			if ((m_loop) && (ani_clip->getFrameCount() <= (ani_clip->getTotalUpdateTime() / m_frame_per_second)))
//			{
//				ani_clip->play(false, tInterval);
//				ani_clip->play(true, 0);
//			}
//		}
//
//		if(m_static_object)//���¾�̬����
//		{
//			m_static_object->update(tInterval);
//		}
//	}
//	else if( m_play_mode == PLAY_OBJECT )
//	{
//		tInterval = tInterval * m_time_scale;
//		for(std::set<DynamicEffectObject*>::iterator pos = m_playing_object_set.begin()
//			; pos != m_playing_object_set.end(); ++pos)
//		{
//			DynamicEffectObject* dynaimc_effect_object = *pos;
//			dynaimc_effect_object->update(tInterval);
//		}
//
//		if(m_static_object)//���¾�̬����
//		{
//			m_static_object->update(tInterval);
//		}
//	}
//	else
//	{
//		tInterval = tInterval * m_time_scale;
//		updateAll(tInterval);
//	}
//}

CameraEffectObject* StageAnimationPlayer::getCurCamera()
{
	if( m_play_mode == PLAY_OBJECT )
	{
		return m_cur_camera;
	}
	else if (m_cur_camera_clip != NULL)
	{
		m_cur_camera_clip->resetObjectIterator();
		while(DynamicEffectObject* obj = m_cur_camera_clip->nextObject())
		{
			CameraEffectObject* cam = dynamic_cast<CameraEffectObject*>(obj);
			if (NULL != cam)
			{
				return cam;
			}
		}
	}

	return NULL;
}
void StageAnimationPlayer::ExcludeClipAndDynEffObj4Level()
{
	//ֹͣ��ؿ��е���̨Ч�����ظ�����̨Ч�����ﵽ��ؿ������Ŀ��
	//removeStageAnimationClip();
	stopAllStageAnimationClip();
	//ֹͣ��ؿ��еĶ�̬��Ч�����ظ��Ķ�̬��Ч���壬�ﵽ��ؿ������Ŀ��
	stopAllDynamicEffectObject();
}
void StageAnimationPlayer::ExcluedLevelAndDynEffObj4Clip(const char* ani_name)
{
	if (!ani_name)
		return;

	//ֹͣ�ؿ�����
	playLevel(false);

	//ֹͣ��̬��Ч���岥��
	stopAllDynamicEffectObject();
}
void StageAnimationPlayer::ExcludeLevelAndClip4DynEffObj(DynamicEffectObject* obj)
{
	if (!obj)
		return;

	//ֹͣ�ؿ�����
	playLevel(false);

	//ֹͣ��̨Ч������
	stopAllStageAnimationClip();
}

void StageAnimationPlayer::stopAllStageAnimationClip()
{
	resetStageAnimationClip();
}

void StageAnimationPlayer::stopAllDynamicEffectObject()
{
	resetDynamicEffectObject();
}

//�ؿ��Ƿ������ڲ��ŵĶ�����ͻ
bool StageAnimationPlayer::isLevelAndClipConflicted()
{
	std::set<DynamicEffectObject*> clip_object;

	DynamicEffectObject* object = NULL;

	//��õ������ŵĶ����а����Ķ�̬��Ч����
	for(std::set<StageAnimationClip*>::const_iterator pos = m_playing_clip_set.begin()
		; pos != m_playing_clip_set.end(); ++pos)
	{
		//����playing clip
		StageAnimationClip* ani_clip = (*pos);
		//���clip�еĶ�̬��Ч���塣
		if(ani_clip != NULL)
		{
			ani_clip->resetObjectIterator();
			object = ani_clip->nextObject();
			while(object)
			{
				clip_object.insert(object);
				object = ani_clip->nextObject();
			}				
		}
	}
	getAnimationScript()->resetIterator();
	while(StageAnimationCuttingData* cuttingData = getAnimationScript()->next())
	{
		StageAnimationClip* ani_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());

		if (ani_clip)
		{
			//�뵥�����ŵĶ�����ͻ��
			if (m_playing_clip_set.find(ani_clip)!=m_playing_clip_set.end())
				return true;

			//��ýű������漰�Ķ�̬��Ч����
			ani_clip->resetObjectIterator();
			object = ani_clip->nextObject();
			while(object)
			{
				//�뵥�����ŵĶ����а����Ķ�̬��Ч�����ͻ
				if (clip_object.find(object) != clip_object.end())
					return true;

				object = ani_clip->nextObject();
			}
		}
	}
	return false;
}

//�ؿ��Ƿ������ڲ��ŵĶ�̬��Ч�����ͻ
bool StageAnimationPlayer::isLevelAndDynEffObjConflicted()
{
	bool bResult = false;

	std::set<DynamicEffectObject*> clip_object;

	DynamicEffectObject* object = NULL;

	//�����ؿ�����ùؿ��еĶ�̬��Ч����
	getAnimationScript()->resetIterator();
	while(StageAnimationCuttingData* cuttingData = getAnimationScript()->next())
	{
		StageAnimationClip* ani_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());

		if (ani_clip)
		{
			//��ýű������漰�Ķ�̬��Ч����
			ani_clip->resetObjectIterator();
			object = ani_clip->nextObject();
			while(object)
			{
				//�뵥�����ŵ������ͻ
				if (m_playing_object_set.find(object) != m_playing_object_set.end())
					return true;
				object = ani_clip->nextObject();
			}	
		}
	}

	return false;
}


//�����Ƿ��붯̬��Ч�����ͻ
bool StageAnimationPlayer::isClipAndDynEffObjConflicted(const char* ani_name)
{
	StageAnimationClip* ani_clip = m_ani_clip_mng->find(ani_name);
	if(!ani_clip)
		return false;

	//����clip�еĶ�̬��Ч����
	ani_clip->resetObjectIterator();
	DynamicEffectObject* object = ani_clip->nextObject();
	while(object)
	{
		//�ж��Ƿ������ڲ��ŵĶ�̬��Ч�����ͻ
		if (m_playing_object_set.find(object) != m_playing_object_set.end())
		{
			return true;
		}

		object = ani_clip->nextObject();
	}
	return false;
}

//�������ŵĶ����Ƿ���ؿ���ͻ
bool StageAnimationPlayer::isClipAndLevelConflicted(const char* ani_name)
{
	StageAnimationClip* ani_clip = m_ani_clip_mng->find(ani_name);
	if(!ani_clip)
		return false;

	std::set<DynamicEffectObject*> level_object;
	std::set<DynamicEffectObject*>::iterator object_it;
	DynamicEffectObject* object = NULL;

	getAnimationScript()->resetIterator();
	while(StageAnimationCuttingData* cuttingData = getAnimationScript()->next())
	{
		StageAnimationClip* script_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());
		if (script_clip)
		{
			//�ж�StageAnimationClip�Ƿ��ͻ
			if (script_clip == ani_clip)
			{
				return true;
			}

			//��ýű������漰�Ķ�̬��Ч����
			script_clip->resetObjectIterator();
			object = script_clip->nextObject();
			while(object)
			{
				level_object.insert(object);
				object = script_clip->nextObject();
			}	
		}
	}

	//����clip�еĶ�̬��Ч����
	ani_clip->resetObjectIterator();
	object = ani_clip->nextObject();
	while(object)
	{
		//�ж��Ƿ���ؿ��еĶ�̬��Ч�����ͻ
		if (level_object.find(object) != level_object.end())
		{
			return true;
		}

		object = ani_clip->nextObject();
	}

	return false;
}

bool StageAnimationPlayer::isDynEffObjAndLevelConflicted(DynamicEffectObject* obj)
{
	if (!obj)
		return false;

	getAnimationScript()->resetIterator();
	while(StageAnimationCuttingData* cuttingData = getAnimationScript()->next())
	{
		StageAnimationClip* script_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());
		DynamicEffectObject* object = NULL;

		//��ýű������漰�Ķ�̬��Ч����
		script_clip->resetObjectIterator();
		object = script_clip->nextObject();
		while(object)
		{
			if (object == obj)
			{
				return true;
			}
			object = script_clip->nextObject();
		}				

	}
	return false;
}

bool StageAnimationPlayer::isDynEffObjAndClipConflicted(DynamicEffectObject* obj)
{
	for(std::set<StageAnimationClip*>::iterator pos = m_playing_clip_set.begin()
		; pos != m_playing_clip_set.end(); ++pos)
	{
		//����playing clip
		StageAnimationClip* ani_clip = (*pos);
		DynamicEffectObject* object = NULL;

		//���clip�еĶ�̬��Ч���塣
		if(ani_clip != NULL)
		{
			ani_clip->resetObjectIterator();
			object = ani_clip->nextObject();
			while(object)
			{
				if (object == obj)
				{
					return true;
				}
				object = ani_clip->nextObject();
			}				
		}
	}
	return false;
}

void StageAnimationPlayer::resetPlayMode(PlayMode ePlayMode)
{
	if (ePlayMode&PLAY_OBJECT)
	{
		resetDynamicEffectObject();
	}
	if (ePlayMode&PLAY_CLIP)
	{
		resetStageAnimationClip();
	}
	if (ePlayMode&PLAY_LEVEL)
	{
		resetLevel();
	}
}

void StageAnimationPlayer::setAllRendered(bool bRendered)
{

	if(m_static_object)//���ƾ�̬����
	{
		m_static_object->set_visible(bRendered);
	}

	ActorManager* actor_mng = m_stage->getActorMng();
	actor_mng->reset_iterator();
	Actor* p = actor_mng->next();
	while (NULL != p)
	{
		p->set_visible(bRendered);
		p = actor_mng->next();
	}
}
//���Źؿ�
void StageAnimationPlayer::playLevel(bool play)
{


	if(play)
	{
		//������̨Ч��
		resetLevel();
		m_play_mode |= PLAY_LEVEL;

		//�ж��Ƿ��뵥�����ŵ���̨Ч����ͻ
		if ((m_play_mode&PLAY_CLIP) && isLevelAndClipConflicted())
		{
			m_play_mode &= (~PLAY_LEVEL);
			return;
		}

		//�ж��Ƿ��뵥�����ŵĶ�̬��Ч�����ͻ
		if((m_play_mode&PLAY_OBJECT) && isLevelAndDynEffObjConflicted())
		{
			m_play_mode &= (~PLAY_LEVEL);
			return;
		}

		setFramePerSecond(m_playing_script->getFramePerSecond());

		//��ʼ���Źؿ�
		_do_play_level(play);
	}
	else
	{
		//ȡ���⻷����
		if(m_halo_config.m_load_succ)//�����ȡ����
		{
			m_stage->getActorMng()->get_graphic_res_mng()->get_render_system()->get_IRender()->SetLensParameters(m_halo_config.m_lensEnv,0,0);
		}

		m_play_mode = m_play_mode & (~PLAY_LEVEL);
		resetLevel();
		/*if (m_play_mode == PLAY_STOP)
		{
			setAllRendered(true);
		}*/
	}
};

void StageAnimationPlayer::playStaticObject(bool play)
{
	if(m_static_object)
	{
		ActorAnimationControl* actor_control = m_static_object->getAnimationControl();
		//����clip
		ActorAnimationClip* actor_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
		//����clip
		if(actor_ani_clip)
		{
			actor_control->play(play,0,actor_ani_clip);
		}
	}
}
void StageAnimationPlayer::updateSystemEnv()
{
	//���û������stage modifier
	ActorManager* actor_mng = m_stage->getActorMng();
	actor_mng->reset_iterator();

	while(Actor* p = actor_mng->next())
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
		if( stricmp(obj->getDefinedType() , "x52_system") == 0)//��¼ϵͳ�������� ����ģ��Ժ�ĵ� lishan 101223
		{
			DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(obj->getAnimationControl());  
			assert(dyn_effobj_control);
			ActorAnimationClip* stage_ani_clip = dyn_effobj_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
			//����clip
			if(stage_ani_clip)//����stage����
			{
				// memleak
// 				{
// 					PropertyModifierType modifier_type;
// 					modifier_type.child_entity_id = "system";
// 					modifier_type.is_curv = false;
// 					modifier_type.property_name = "System_Ambient_Color";
// 					//����ȱʡֵ
// 					IPropertyModifier* modifier = stage_ani_clip->find_property_modifier(modifier_type);
// 					if(!modifier)
// 					{
// 						modifier = actor_mng->getPropertyModifierFactory()->create_modifier(obj,modifier_type);
// 
// 						if(!stage_ani_clip->add_property_modifier(modifier_type, modifier))
// 						{
// 							actor_mng->getPropertyModifierFactory()->releaseModifier(modifier);
// 							assert(0);
// 						}
// 					}
// 					assert(modifier);
// 					if(modifier)
// 					{
// 						IPropertyModifierStreamable* s_modifier = dynamic_cast<IPropertyModifierStreamable*>(modifier);
// 						assert(s_modifier);
// 
// 						char amb_color[128]={0};
// 						sprintf(amb_color,"{%f,%f,%f,%f}",m_ambient_color[0],m_ambient_color[1],m_ambient_color[2],m_ambient_color[3]);
// 						s_modifier->set_value(amb_color);//��������ֵ
// 						s_modifier->execute(true,0.0f);
// 					}
// 				}
// 
// 				{
// 					PropertyModifierType modifier_type;
// 					modifier_type.child_entity_id = "system";
// 					modifier_type.is_curv = false;
// 					modifier_type.property_name = "System_AmbientActor_Color";
// 					//����ȱʡֵ
// 					IPropertyModifier* modifier = stage_ani_clip->find_property_modifier(modifier_type);
// 					if(!modifier)
// 					{
// 						modifier = actor_mng->getPropertyModifierFactory()->create_modifier(obj,modifier_type);
// 
// 						if(!stage_ani_clip->add_property_modifier(modifier_type, modifier))
// 						{
// 							actor_mng->getPropertyModifierFactory()->releaseModifier(modifier);
// 							assert(0);
// 						}
// 					}
// 					assert(modifier);
// 					if(modifier)
// 					{
// 						IPropertyModifierStreamable* s_modifier = dynamic_cast<IPropertyModifierStreamable*>(modifier);
// 						assert(s_modifier);
// 
// 						char amb_color[128]={0};
// 						sprintf(amb_color,"{%f,%f,%f,%f}",m_ambient_actor_color[0],m_ambient_actor_color[1],m_ambient_actor_color[2],m_ambient_actor_color[3]);
// 						s_modifier->set_value(amb_color);//��������ֵ
// 						s_modifier->execute(true,0.0f);
// 					}
// 				}

				{
					//������Ӱ��Χ
					//actor_mng->get_graphic_res_mng()->get_render_system()->set_shadow_scope(m_shadow_scope);
					actor_mng->get_level_entity()->set_shadow_scope(m_shadow_scope);
				}	
			}
		}

	}

}

//interface func end
// ��ȡ��̨��Ϣ����̬Ч�����塢��̨Ч����
bool StageAnimationPlayer::loadStageFile(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp) 
{
//	assert(m_env_loaded);//���������ñ����ȼ���
	if(!m_ani_clip_mng->load(stageFile,isBinary, pLp))
		return false;
#ifdef _EDITOR
	if (!m_ani_clip_mng->parseCameraFile("..\\resources\\level\\camera.xml"))
		return false;

	if(!m_ani_clip_mng->loadCommonCamera())
		return false;
#endif

	if (m_env_loaded)
	{
		updateSystemEnv();
	}

	m_stage_loaded = true;

	BiXmlHelper::ClearXMLDocument();
	return true;
};
// ������̨��Ϣ����̬Ч�����塢��̨Ч����
bool StageAnimationPlayer::saveStageFile(const char* stageFile, bool isBinary, bool isOptimize)
{
	if(!m_ani_clip_mng->save(stageFile,isBinary, isOptimize))
		return false;
	return true;	
} ;
//��������޸���
static bool  playerCreateEffectRecursive(PropertyModifierFactory* modifier_factory, 
										 DynamicEffectObject* top_effect_obj,
										 ActorAnimationClip* ani_clip, 
										 const char* child_entity_id, 
										 const char* real_property, 
										 const char* real_value)
//static bool stageAddPropertyModifier(PropertyModifierFactory* modifier_factory
//								,BiXmlNode* propertyNode
//								,const char* child_entity_id
//								,DynamicEffectObject* top_effect_obj
//								,ActorAnimationClip* ani_clip)
{
	//������
	const char* property_name = real_property;//BiXmlHelper::get_attribute(propertyNode,"key");
	const char* value = real_value;//BiXmlHelper::get_attribute(propertyNode,"value");

	//����modifier type
	PropertyModifierType modifier_type;
	//modifier_type.child_actor_id = actor_level;//actor��Σ�ֻ������actor��
	modifier_type.child_entity_id = child_entity_id;//ʵ������

	modifier_type.property_name = property_name;//��������;
	modifier_type.is_curv = false;//�Ƿ�����

	//����modifier
	IPropertyModifier* modifier = modifier_factory->create_modifier(top_effect_obj,modifier_type);
	if(modifier)
	{
		IPropertyModifierStreamable* s_modifier = dynamic_cast<IPropertyModifierStreamable*>(modifier);
		assert(s_modifier);

		if(value != NULL)
		{
			s_modifier->set_value(value);//��������ֵ
			s_modifier->execute(true,0.0f);
		}

		if(!ani_clip->add_property_modifier(modifier_type, modifier))
		{
			modifier_factory->releaseModifier(modifier);
			return false;
		}
	}
	else
	{
		return false;//
	}
	return true;
}

bool StageAnimationPlayer::parseActor(BiXmlNode* entityNode, int& entity_number, ActorAnimationClip* static_ani_clip, I_Keep_Client_Alive* pLp)
{
	if ( pLp )
	{
		pLp->update();
	}

	//�������ƻ���
	const int ENTITY_NAME_BUF_SIZE = 128;
	char entity_name_buf[ENTITY_NAME_BUF_SIZE] = {0};
	char convert_value_buf[256]= {0};

	//��������Ч��
	const char* entity_type = BiXmlUtility::get_attribute(entityNode, "Type");
	const char* entity_guid = BiXmlUtility::get_attribute(entityNode, "Guid");
	BiXmlNode* propertyNode = entityNode->FirstChild("Property");
	const char* entity_real_type = NULL;
	assert(entity_type);

	if(stricmp(entity_type,"RoleActor") ==0) //���ﲻ����
	{
		/*
		entityNode = entityNode->NextSibling("Actor");
		continue;
		*/
		return true;
	}
	if(stricmp(entity_type,"ModelActor") ==0 )//ģ�Ͷ��Ȱ�ģ�ͽ���
	{
		BiXmlNode* res_node = propertyNode->FirstChild("ResName");
		if (res_node)
		{
			std::string res_name = BiXmlUtility::get_value(res_node->ToElement());

			std::transform(res_name.begin(), res_name.end(), res_name.begin(), tolower);

			std::string res_ext = fileFullPathExt(res_name);

			if(res_ext == "chr")
				entity_real_type = "NPC";
			else
				entity_real_type = "model";
		}
		else
		{
			assert(0);
			entity_real_type = "model";
		}
	}
	else if (stricmp(entity_type,"EffectActor") ==0 )
	{
		entity_real_type = "effect";
	}
	else if (stricmp(entity_type,"LightActor") == 0 )
	{	//LightActor �ƹ� ���ͣ�point���Դ��dir����⡢prm׶�ι�
		char type_cvt[CHAR_TEMP_MAX_SIZE];
		unicodeToUtf8("����", type_cvt);
		BiXmlNode* lightTypeNode = propertyNode->FirstChild(type_cvt);//"����" //
		assert(lightTypeNode);
		const char* light_type = BiXmlUtility::get_value(lightTypeNode->ToElement());
		assert(light_type);
		if( stricmp(light_type,"point") == 0)
			entity_real_type = "point_prepass_light";
		else if( stricmp(light_type,"dir") == 0)
			entity_real_type = "direction_prepass_light";
		else if( stricmp(light_type,"prm") == 0)
			entity_real_type = "wimble_prepass_light";
	}
	else
	{
		/*
		entityNode = entityNode->NextSibling("Actor");
		continue;
		*/
		return true;
		//assert(0);//not support yet
	}
	//if (!auto_generate_name)
	{
		BiXmlNode* name_node = propertyNode->FirstChild("Name");
		if (name_node)
		{
			const char* name = BiXmlUtility::get_value(name_node->ToElement());
			//auto_generate_name = true;
			if (m_static_object->find_entity(name) == NULL)
			{
				strcpy_s(entity_name_buf, ENTITY_NAME_BUF_SIZE, name);
				//auto_generate_name = false;
			}
			else
			{
				sprintf(entity_name_buf,"%s_%d",entity_real_type,entity_number++);
			}
		}
		else
		{
			sprintf(entity_name_buf,"%s_%d",entity_real_type,entity_number++);
		}
	}

	//if (auto_generate_name)
	//{
	//	sprintf(entity_name_buf,"%s_%d",entity_real_type,entity_number++);
	//}

	//����ʵ��
	CMediaEndEntity* entity = m_stage->getActorMng()->create_entity(entity_real_type,entity_name_buf);
	entity->set_guid(entity_guid);
	assert(entity);
	//������������Ŀɼ�����
	if(C3DEntity* entity3d = dynamic_cast<C3DEntity*>(entity))
	{
		entity3d->set_visible(false);
		entity3d->set_visible(true);
	}
	//���뵱ǰactor
	if(!m_static_object->add_entity(entity))
	{
		assert(0);
	}
	//����actor	
	PropertyModifierFactory* modifier_factory = m_stage->getActorMng()->getPropertyModifierFactory();//

	bool set_realtime_shadow = false;//������Ӱ

	bool apply_lightmap = false; //�Ƿ�Ӧ��lightmap;

	for(BiXmlNode* child = propertyNode->FirstChild(); child; child = child->NextSibling() )
	{
		if ( pLp )
		{
			pLp->update();
		}
		char property_key[CHAR_TEMP_MAX_SIZE];
		utf8ToUnicode(child->Value(), property_key);//UTF8 NAME
		const char* inner_value = BiXmlUtility::get_value(child->ToElement());
		//������������
		if(stricmp(property_key,"ʵʱ��Ӱ") == 0)
		{
			if(stricmp(entity_type,"ModelActor") == 0 )
			{
				CModelEntity* entityModel = dynamic_cast<CModelEntity*>(entity);
				assert(entityModel);
					
				if(stricmp(inner_value,"true") == 0)
				{
					entityModel->setShadow(true);
					set_realtime_shadow = true;
				}
				else if(stricmp(inner_value,"false") == 0)
				{
					entityModel->setShadow(false);
				}
			}
			continue;//��Ĳ�����
		}
		else if(stricmp(property_key,"����") == 0)
		{
			if(stricmp(entity_type,"LightActor") == 0)
			{
				CPrePassLightEntity* ple = dynamic_cast<CPrePassLightEntity*>(entity);
				float x,y,z;
				sscanf(inner_value,"(%f,%f,%f)",&x,&y,&z);
				H3DVec3 d(x*(-1), y*(-1), z*(-1));
				ple->get_IMoveObject()->SetDirection(d);
				const H3DMat4& mat = ple->get_IMoveObject()->GetLocationMatrix();
				ple->set_location_matrix(mat);
			}
			continue;//��Ĳ�����
		}
		else if(stricmp(property_key,"��ʼλ��") == 0)
		{
			if(stricmp(entity_type,"LightActor") == 0)
			{
				CPrePassLightEntity* ple = dynamic_cast<CPrePassLightEntity*>(entity);
				float x,y,z;
				sscanf(inner_value,"(%f,%f,%f)",&x,&y,&z);

				H3DVec3 p(x, y, z);
				//ple->get_IMoveObject()->SetPosition(p);
				ple->set_position(p);
			}
			continue;//��Ĳ�����
		}
		else if (stricmp(property_key, "Hide") == 0)
		{
			bool visible = stricmp(inner_value, "true") != 0;
			if(C3DEntity* entity3d = dynamic_cast<C3DEntity*>(entity))
			{
				entity3d->set_visible(visible);
			}
		}
		else if (stricmp(property_key, "ϸ����������") == 0)
		{
			if (C3DEntity* entity3d = dynamic_cast<C3DEntity*>(entity))
			{
				if (stricmp(inner_value, "DAT_SMALL_ACTOR") == 0)
					entity3d->set_detail_actor_type(DAT_SMALL_ACTOR);
				else if (stricmp(inner_value, "DAT_HUGE_ACTOR") == 0)
					entity3d->set_detail_actor_type(DAT_HUGE_ACTOR);
				else if (stricmp(inner_value, "DAT_DETAIL_VEGETATION") == 0)
					entity3d->set_detail_actor_type(DAT_DETAIL_ACTOR);
				else
					assert(false);
			}
		}
		else if (stricmp(property_key, "���Ӿ���") == 0)
		{
			if (C3DEntity* entity3d = dynamic_cast<C3DEntity*>(entity))
			{
				float visible_dist = 0.0f;
				sscanf(inner_value, "%f", &visible_dist);
				entity3d->set_visible_dist(visible_dist);
			}
		}
			

		if(stricmp(entity_type,"ModelActor") ==0)
		{
			if(stricmp(property_key, "�Ƿ���ܹ��") == 0)//��̬ģ�͵��Ƿ���ܹ������
			{
				CModelEntity* model_entity = dynamic_cast<CModelEntity*>(entity);
				if (stricmp(inner_value, "true") == 0)
					model_entity->record_recive_fakelightspot(true);
				else if (stricmp(inner_value, "false") == 0)
					model_entity->record_recive_fakelightspot(false);
				else
					assert(false);
			}
			else if (stricmp(property_key, "�Ƿ�Ӧ��Lightmap") == 0)
			{
				if(stricmp(inner_value, "true") == 0)
					apply_lightmap = true;
				else
					apply_lightmap = false;
			}
			else if (stricmp(property_key, "�ܽ�ɫ��") == 0)
			{
				CModelEntity* model_entity = dynamic_cast<CModelEntity*>(entity);
				if(stricmp(inner_value, "true") == 0)
					model_entity->set_level_layer(H3DI::SL_Actors);
				else
					model_entity->set_level_layer(H3DI::SL_DetailObj);
			}
		}
		else if(stricmp(entity_type,"LightActor") ==0)
		{
			if(stricmp(property_key, "����") == 0)
			{
				if(LL_NULL != ActorManager::isLodSystemActorName(inner_value))
				{
					char inner_value_cvt[CHAR_TEMP_MAX_SIZE];
					utf8ToUnicode(inner_value, inner_value_cvt);//UTF8 NAME
					entity->set_name(inner_value_cvt);
				}
			}
			if (stricmp(property_key, "�߹�ǿ��") == 0)
			{
				CDirectionPrePassLightEntity* dpl = dynamic_cast<CDirectionPrePassLightEntity*>(entity);
				if (dpl)
				{
					float fspec_intensity = 0.0f;
					sscanf(inner_value, "%f", &fspec_intensity);
					dpl->set_specular_intensity(fspec_intensity);
				}
			}
		}
			
		const char* real_property = NULL;//��Ҫ��Ӧ��ȷ��
		const char* real_value = NULL;
		//�ж��Ƿ����������
		std::map<std::string,std::pair<std::string,int>>::const_iterator iter = static_property_map.find(property_key);
		if(iter == static_property_map.end())
			continue;

		//�ж���������Ƿ���ȷ
		if (stricmp(entity_type,"LightActor") == 0 )
		{
			if(stricmp(entity_real_type, "point_prepass_light") == 0 )
			{
				std::set<std::string>::const_iterator set_iter = point_light_set.find(iter->second.first.c_str());
				if(set_iter == point_light_set.end())
					continue;
			}
			else if(stricmp(entity_real_type, "direction_prepass_light") == 0 )
			{
				std::set<std::string>::const_iterator set_iter = direction_light_set.find(iter->second.first.c_str());
				if(set_iter == direction_light_set.end())
					continue;
			}
			else if(stricmp(entity_real_type, "wimble_prepass_light") == 0 )
			{
				std::set<std::string>::const_iterator set_iter = wimble_light_set.find(iter->second.first.c_str());
				if(set_iter == wimble_light_set.end())
					continue;
			}
		}
			
		if(iter->first == "LocationMat")//����λ��
		{
			float xx=0; float xy=0; float xz=0; float xw=0;
			float yx=0; float yy=0; float yz=0; float yw=0;
			float zx=0; float zy=0; float zz=0; float zw=0;
			float wx=0; float wy=0; float wz=0; float ww=0; 

			std::string inputStr = inner_value;
			MiddleLayerStrHelper::replaceStr(inputStr," ","");
			MiddleLayerStrHelper::replaceStr(inputStr,"||",",");
			MiddleLayerStrHelper::replaceStr(inputStr,"|","");

			sscanf(inputStr.c_str(),"%f,%f,%f,%f,%f,%f,%f,%f, %f,%f,%f,%f,%f,%f,%f,%f",\
					&xx,  &xy,  &xz,  &xw, &yx,  &yy,  &yz,  &yw,\
					&zx,  &zy,  &zz,  &zw, &wx,  &wy,  &wz,  &ww );

			H3DMat4 matcvt(   xx,  xy, xz, xw,yx,  yy,  yz,  yw,\
					zx,  zy,  zz,  zw, wx,  wy,  wz,  ww );

			H3DVec3					position; ///< ���λ��
			//H3DVec3					eula; ///< ���ŷ����
			H3DQuat					quat; ///< �����Ԫ��
			H3DVec3					scale;///< ������ţ�������Model�ж������޸ġ���ʼֵΪ(1.0f,1.0f,1.0f)

			C3DEntity* entity_3d = dynamic_cast<C3DEntity*>(entity);
			entity_3d->set_location_matrix(matcvt);

			//matcvt.Decompose(scale, quat, position);
			//sprintf(convert_value_buf,"{%f,%f,%f}",position.x,position.y,position.z);
			//real_value = convert_value_buf;

			//bool ret = playerCreateEffectRecursive(modifier_factory, m_static_object,static_ani_clip, entity_name_buf, "Position", real_value);
			//if(!ret)
			//{
			//	delete doc;
			//	return false;
			//}

			//sprintf(convert_value_buf,"{%f,%f,%f}",quat.x,quat.y,quat.z);
			//ret = playerCreateEffectRecursive(modifier_factory, m_static_object,static_ani_clip, entity_name_buf, "Rotation", real_value);
			//if(!ret)
			//{
			//	delete doc;
			//	return false;
			//}

			//sprintf(convert_value_buf,"{%f,%f,%f}",scale.x,scale.y,scale.z);
			//ret = playerCreateEffectRecursive(modifier_factory, m_static_object,static_ani_clip, entity_name_buf, "Scale", real_value);
			//if(!ret)
			//{
			//	delete doc;
			//	return false;
			//}
		}
		else
		{
			real_property = iter->second.first.c_str();//ȡ��������
			switch(iter->second.second)
			{
			case STATIC_DATATYPE_BOOL:
			case STATIC_DATATYPE_INT:
			case STATIC_DATATYPE_FLOAT:
			case STATIC_DATATYPE_STRING:
				real_value = inner_value;
				break;
			case STATIC_DATATYPE_COLOR:
				{
					float r,g,b,a;
					sscanf(inner_value,"%f %f %f %f",&r,&g,&b,&a);
					r/=255.0f; g/=255.0f; b/=255.0f; a/=255.0f;
					sprintf(convert_value_buf,"{%f,%f,%f,%f}",r,g,b,a);

					real_value = convert_value_buf;
				}
				break;
			case STATIC_DATATYPE_MAT3:
				{
					float x,y,z;
					sscanf(inner_value,"(%f,%f,%f)",&x,&y,&z);
					sprintf(convert_value_buf,"{%f,%f,%f}",x,y,z);
					real_value = convert_value_buf;
				}
				break;
			case STATIC_DATATYPE_LOCATIONMAT:
				{
					assert(0);
				}
				break;
			}

			//bool ret = playerCreateEffectRecursive(modifier_factory, m_static_object,static_ani_clip, entity_name_buf, real_property, real_value);
			bool ret = playerCreateEffectRecursive(modifier_factory, m_static_object,static_ani_clip, entity->get_name(), real_property, real_value);
			if(!ret)
				return false;

			if(set_realtime_shadow)
			{
				CModelEntity* entityModel = dynamic_cast<CModelEntity*>(entity);
				entityModel->setShadow(true);
			}
		}
	}
	if(stricmp(entity_type,"ModelActor") ==0)
	{
		if (apply_lightmap)
		{
			BiXmlNode* lightmapNode = entityNode->FirstChild("LightMap");
			if(lightmapNode)//��̬ģ�͵�LightMap����
			{
				CModelEntity* model_entity = dynamic_cast<CModelEntity*>(entity);
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


					model_entity->add_lightmapinfo(nMeshID, lightmap);
					mapnode = mapnode->NextSibling();
				}

				model_entity->load_lightmap();
			}
		}			
	}

	C3DEntity* pEntity = dynamic_cast<C3DEntity*>(entity);
	if (pEntity)
	{
		pEntity->set_is_static(true);
	}

	return true;
}

bool StageAnimationPlayer::parseLayer(BiXmlNode* layerNode, int&entity_number, ActorAnimationClip* static_ani_clip, I_Keep_Client_Alive* pLp)
{
	const char* layer_type = BiXmlUtility::get_attribute(layerNode, "Type");
	const char* layer_name = BiXmlUtility::get_attribute(layerNode, "Name");
	if (!layer_type)
		return true;

	//��static�㲻����
	if (stricmp("STATIC", layer_type) != 0)
		return true;

	//������ڵ��µ�����
	BiXmlNode* entityNode = layerNode->FirstChild("Actor");
	while(entityNode != NULL)
	{
		if (!parseActor(entityNode, entity_number, static_ani_clip, pLp))
			return false;

		entityNode = entityNode->NextSibling("Actor");
	}

	//�ݹ������ڵ��µĲ�
	BiXmlNode* sublayerNode = layerNode->FirstChild("Layer");
	while(sublayerNode != NULL)
	{
		if (!parseLayer(sublayerNode, entity_number, static_ani_clip, pLp))
			return false;

		sublayerNode = sublayerNode->NextSibling("Layer");
	}

	return true;
}

bool StageAnimationPlayer::parseLod(BiXmlNode* node, SceneLodInfo& lod_info)
{
	assert(node);

	// ��ȡ�������Ľ�����Χ����ʧ����
	BiXmlNode* detail_actor_vision_node = node->FirstChildElement("DetailActorTypeVision");	
	if (detail_actor_vision_node == NULL)
		return false;

	// ��ȡС������Ľ�����Χ����ʧ����
	BiXmlNode* small_actor_node = detail_actor_vision_node->FirstChildElement("DAT_SMALL_ACTOR");
	if (small_actor_node == NULL)
		return false;

	//С���������������
	{
		const char* small_actor_start_disappear = BiXmlUtility::get_attribute(small_actor_node, "StartDisappear");
		lod_info.m_small_actor_start_disappear = atof(small_actor_start_disappear);
	}

	//С���������ʧ����
	{
		const char* small_actor_far_disappear = BiXmlUtility::get_attribute(small_actor_node, "FarD");
		lod_info.m_small_actor_far_disappear = atof(small_actor_far_disappear);
	}

	//��������
	BiXmlNode* huge_actor_node = detail_actor_vision_node->FirstChildElement("DAT_HUGE_ACTOR");
	if (huge_actor_node == NULL)
		return false;

	//�����������������
	{
		const char* huge_actor_start_disappear = BiXmlUtility::get_attribute(huge_actor_node, "StartDisappear");
		lod_info.m_huge_actor_start_disappear = atof(huge_actor_start_disappear);
	}

	//�����������ʧ����
	{
		const char* huge_actor_far_disappear = BiXmlUtility::get_attribute(huge_actor_node, "FarD");
		lod_info.m_huge_actor_far_disappear = atof(huge_actor_far_disappear);
	}

	//detail
	BiXmlNode* detail_actor_node = detail_actor_vision_node->FirstChildElement("DAT_DETAIL_VEGETATION");
	if (detail_actor_node == NULL)
		return false;

	//ϸ��ֲ������������
	{
		const char* detail_actor_start_disappear = BiXmlUtility::get_attribute(detail_actor_node, "StartDisappear");
		lod_info.m_detail_actor_start_disappear = atof(detail_actor_start_disappear);
	}

	//ϸ��ֲ������ʧ����
	{
		const char* detail_actor_far_disappear = BiXmlUtility::get_attribute(detail_actor_node, "FarD");
		lod_info.m_detail_actor_far_disappear = atof(detail_actor_far_disappear);
	}

	return true;
}

//��ȡ��̬�����ļ�
bool StageAnimationPlayer::loadStaticObject(const char* filename0, bool isBinary, I_Keep_Client_Alive* pLp) 
{
//create dynamic effect object
	assert(!isBinary);
	assert( m_static_object == NULL);
	m_static_object = new DynamicEffectObject(m_stage->getActorMng(),"__static_object","__static_object");

	//ת���ָ���
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(filename0,fullFileName);
	//ȡ����·��
	std::string working_dir ;
	MiddleLayerFilePathHelper::getWorkingPath(filename0,working_dir);

	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		//LogError("[StageAnimationPlayer::loadStaticObject] load static obj file failed:",fullFileName.c_str());
		assert(0);
		return false;
	}
	// ��ȡ����
	BiXmlNode* rootActorNode = doc->FirstChild("Actor");
	if (rootActorNode == NULL)
	{
		//delete doc;
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	
	ActorAnimationControl* ani_control = m_static_object->getAnimationControl();
	assert(ani_control);
	//������StageClipͬ���� ActorAniClip
	ActorAnimationClip* static_ani_clip = ani_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
	assert(static_ani_clip);

	int entity_number = 0;
	
	//��ȡ���ڵ��µ����壬���¸�ʽ�£��ýڵ��´�ŵ�Ϊlod���壬���ϸ�ʽ�£����еľ�̬�������ڴ�
	BiXmlNode* entityNode = rootActorNode->FirstChild("Actor");
	while(entityNode != NULL)
	{
		if (!parseActor(entityNode, entity_number, static_ani_clip, pLp))
		{
			XmlDocFactory::ReleaseDocument(doc);
			return false;
		}

		entityNode = entityNode->NextSibling("Actor");
	}

	BiXmlNode* layerNode = rootActorNode->FirstChild("Layer");
	while(layerNode != NULL)
	{
		if (!parseLayer(layerNode, entity_number, static_ani_clip, pLp))
		{
			XmlDocFactory::ReleaseDocument(doc);
			return false;
		}

		layerNode = layerNode->NextSibling("Layer");
	}

	XmlDocFactory::ReleaseDocument(doc);

	m_static_object_loaded = true;
	m_stage->getActorMng()->get_level_entity()->add_static_object(m_static_object);
	/*m_static_object->set_visible(false);
	m_static_object->set_visible(true);*/
	m_stage->getActorMng()->get_level_entity()->RestruceOctree();
	m_stage->switchToLod(m_stage->getLod());

	BiXmlHelper::ClearXMLDocument();
	return true;
};
//��ȡ������Ϣ
bool StageAnimationPlayer::loadEnvironmentFile(const char* env_file, bool isBinary)
{
	assert(!isBinary);
	assert( !m_env_loaded);

	//ת���ָ���
	std::string fullFileName ;
	MiddleLayerFilePathHelper::convertSeperator(env_file,fullFileName);
	//ȡ����·��
	std::string working_dir ;
	MiddleLayerFilePathHelper::getWorkingPath(env_file,working_dir);

	BiXmlHelper::LoadXMLDocument(fullFileName.c_str());
	BiXmlDocument* doc = BiXmlHelper::PeekXMLDocument(fullFileName.c_str(), true);
	if(doc == NULL)
	{
		//LogError("[StageAnimationPlayer::loadEnvironmentFile] load env file failed:",fullFileName.c_str());
		assert(0);
		return false;
	}
	// ��ȡ
	BiXmlNode* rootActorNode = doc->FirstChild("Stage");
	if (rootActorNode == NULL)
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}

	// ��ȡ
	BiXmlNode* configNode = rootActorNode->FirstChild("SetConfig");	
	if (configNode == NULL)
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}	

// 	BiXmlNode* ambient_config = configNode->FirstChild("Ambient_config");
// 	if (ambient_config == NULL)
// 	{
// 		XmlDocFactory::ReleaseDocument(doc);
// 		return false;
//	}
// 	const char* ambient_value = BiXmlUtility::get_value(ambient_config->ToElement());
// 	sscanf(ambient_value,"%f %f %f %f",m_ambient_color,m_ambient_color+1,m_ambient_color+2,m_ambient_color+3);


	BiXmlNode* ambient_actor_config = configNode->FirstChild("AmbientActor_config");
	if (ambient_actor_config)
	{
		const char* av = BiXmlUtility::get_value(ambient_actor_config->ToElement());
		sscanf(av,"%f %f %f %f",m_ambient_actor_color,m_ambient_actor_color+1,m_ambient_actor_color+2,m_ambient_actor_color+3);	
	}

	BiXmlNode* shadow_scope_config = configNode->FirstChild("ShadowScope");
	if (shadow_scope_config)
	{
		const char* av = BiXmlUtility::get_value(shadow_scope_config->ToElement());
		if(av)
			sscanf(av,"%f",&m_shadow_scope);	
	}

	BiXmlNode* trans_config = configNode->FirstChild("Trans_config");
	if (trans_config == NULL)
	{
		XmlDocFactory::ReleaseDocument(doc);
		return false;
	}
	const char* inner_value = BiXmlUtility::get_value(trans_config->ToElement());
	sscanf(inner_value,"%f %f %f %f",m_trans_color,m_trans_color+1,m_trans_color+2,m_trans_color+3);
	//
	//������ȱʡֵ
	m_lut_texture = "..\\data\\universal\\asset\\RGBTable16x1.png";
	BiXmlNode* lut_texture = configNode->FirstChild("LUT_Texturer_config");
	if(lut_texture)
	{
		const char* tex_file = BiXmlUtility::get_value(lut_texture->ToElement());
		if(tex_file)
			m_lut_texture = tex_file;
	}	
	//��ȡ�⻷����
	BiXmlNode* halo_config = configNode->FirstChild("Halo");
	if(halo_config)
	{
		if(!m_halo_config.loadLensValue(halo_config))
		{
			//clLog("loadEnvironmentFile halo_config failed!");
			// delete doc;
			// return false;
		}
	}

	Stage* stage = (Stage*)m_stage;
	//Lod0����
	BiXmlNode* lod_vision_zero = configNode->FirstChild("LodVisionLL_ZERO");
	if (lod_vision_zero)
	{
		if(!parseLod(lod_vision_zero, stage->getScenLodInfo(LL_ZERO)))
		{
			XmlDocFactory::ReleaseDocument(doc);
			return false;
		}
	}

	//Lod1����
	BiXmlNode* lod_vision_one = configNode->FirstChild("LodVisionLL_ONE");
	if (lod_vision_one)
	{
		if(!parseLod(lod_vision_one, stage->getScenLodInfo(LL_ONE)))
		{
			XmlDocFactory::ReleaseDocument(doc);
			return false;
		}
	}

	//Lod2����
	BiXmlNode* lod_vision_two = configNode->FirstChild("LodVisionLL_TWO");
	if (lod_vision_two)
	{
		if(!parseLod(lod_vision_two, stage->getScenLodInfo(LL_TWO)))
		{
			XmlDocFactory::ReleaseDocument(doc);
			return false;
		}
	}

	stage->switchSceneEnv(m_level_lod);

	m_env_loaded = true;
	//delete doc;
	XmlDocFactory::ReleaseDocument(doc);

	if (m_stage_loaded)
	{
		updateSystemEnv();
	}

	return true;
};

//����ÿ����̬Ч�������ȱ����̨���Լ�
bool StageAnimationPlayer::loadDefaultStageClips(const char* stageFile, bool isBinary)
{
	if(!m_ani_clip_mng->loadDefaultStageEffects(stageFile, isBinary))
		return false;

	if (m_env_loaded)
	{
		updateSystemEnv();
	}

	m_stage_loaded = true;

	return true;
}
//����̨�а�����������̨Ч���б�����
bool StageAnimationPlayer::loadStageEffectListPath(const char* stageFile, bool isBinary)
{
	return m_ani_clip_mng->loadEffectPathList(stageFile, isBinary);
}

//��������ÿһ����̨Ч��
bool StageAnimationPlayer::loadStageEffect(const char* stageEffect, bool isBinary)
{
	assert(stageEffect && !isBinary);
	return m_ani_clip_mng->loadStageEffect(stageEffect, isBinary);
}

// ��ȡ�籾��Ϣ
bool StageAnimationPlayer::loadAnimationFile(const char* levelFile, bool isBinary)
{
	//if(!m_ani_script->load(levelFile,isBinary))

	if (!m_ani_script_list.find("default")->second->load(levelFile,isBinary))
		return false;
	return true;
} ;

bool StageAnimationPlayer::loadScriptFileToList(const char* levelFile, const char* scriptID, bool isBinary)
{
	std::string id = scriptID;
	if (m_ani_script_list.find(id) != m_ani_script_list.end())//�Ѿ�����ͬ��id�ľ籾
	{
		return false;
	}

	AnimationScript* script = new AnimationScript(m_stage);

	if (!script->load(levelFile, isBinary))
	{
		delete script;
		return false;
	}

	m_ani_script_list.insert(make_pair(id, script));
	return true;
}
// ����籾��Ϣ
bool StageAnimationPlayer::saveAnimationFile(const char* levelFile, bool isBinary)
{
	//if(!m_ani_script->save(levelFile,isBinary))
	//	return false;
	//return true;

	if (!m_playing_script->save(levelFile,isBinary))
		return false;
	return true;
} ;

//���Ŷ���
void StageAnimationPlayer::playStageAnimationClip(const char* ani_name, bool play)
{
	if(play)
	{
		if(!(m_play_mode & PLAY_CLIP))
		{
			resetStageAnimationClip();
			m_play_mode |= PLAY_CLIP;
		}

		if(!ani_name)
		{
			m_play_mode &= (~PLAY_CLIP);
			return;
		}

		StageAnimationClip* ani_clip = m_ani_clip_mng->find(ani_name);
		if(!ani_clip)
		{
			m_play_mode &= (~PLAY_CLIP);
			return;
		}
		//�ж��Ƿ���ؿ���ͻ
		if((m_play_mode&PLAY_LEVEL) && isClipAndLevelConflicted(ani_name))
		{
			m_play_mode &= (~PLAY_CLIP);
			return;
		}

		//�ж��Ƿ��붯̬��Ч�����ͻ
		if((m_play_mode&PLAY_OBJECT) && isClipAndDynEffObjConflicted(ani_name))
		{
			m_play_mode &= (~PLAY_CLIP);
			return;
		}

		// debug by ZYQ
		_loadEnvironmentLight();
		_playStaticObject(play);
		_setSystemInfo();
		// debug end

		//��ʼ����
		ani_clip->play(true,0.0f);
		m_playing_clip_set.insert(ani_clip);

		//���²��ŵ�stageanimationclip��Ϊ����Ⱦ״̬
		//ani_clip->set_visible(true);
		

		if (ani_clip->getClipType() == StageAnimationClip_Camera)
		{
			ani_clip->resetObjectIterator();
			while(DynamicEffectObject* obj = ani_clip->nextObject())
			{
				CameraEffectObject* cam = dynamic_cast<CameraEffectObject*>(obj);
				if (NULL != cam)
				{
					cam->getCamera()->set_target_position_mng(m_targetposmng);
					cam->getCamera()->play_camera();
				}
			}
		}
	}
	else
	{
		if(ani_name)
		{
			StageAnimationClip* ani_clip = m_ani_clip_mng->find(ani_name);
			if(ani_clip)
			{
				if (m_playing_clip_set.find(ani_clip) != m_playing_clip_set.end())
				{
					ani_clip->play(false,0.0f);//ֹͣ����
					m_playing_clip_set.erase(ani_clip);
				}
				//��ֹͣ���ŵ�stageanimationclip��Ϊ������Ⱦ״̬
				//ani_clip->set_visible(false);
			}
		}

		if(m_playing_clip_set.empty())
		{
			//resetPlayMode(PLAY_CLIP);
			m_play_mode &= (~PLAY_CLIP);
			resetStageAnimationClip();
			/*if (m_play_mode == PLAY_STOP)
			{
				setAllRendered(true);
			}*/
		}
	}
};
//��������
void StageAnimationPlayer::playDynamicEffectObject(DynamicEffectObject* obj, bool play)
{
	if(!obj)
		return;

	if(play)
	{
		if( !(m_play_mode & PLAY_OBJECT))
		{
			resetDynamicEffectObject();
			m_play_mode |= PLAY_OBJECT;
		}

		std::set<DynamicEffectObject*>::const_iterator iter = m_playing_object_set.find(obj);
		if( iter != m_playing_object_set.end() )
		{
			// assert(!"�����ظ��������!");
			m_play_mode &= (~PLAY_OBJECT);
			return;
		}
		//�ж��Ƿ���ؿ��еĶ�̬��Ч�����ͻ
		if ((m_play_mode & PLAY_LEVEL) && isDynEffObjAndLevelConflicted(obj))
		{
			m_play_mode &= (~PLAY_OBJECT);
			return;
		}
		//�ж��Ƿ������ڵ������ŵĶ����еĶ�̬��Ч�����ͻ
		if ((m_play_mode & PLAY_CLIP) && isDynEffObjAndClipConflicted(obj))
		{
			m_play_mode &= (~PLAY_OBJECT);
			return;
		}

		//��������
		if (dynamic_cast<CameraEffectObject*>(obj) != NULL)
		{
			m_cur_camera = dynamic_cast<CameraEffectObject*>(obj);
		}

		ActorAnimationControl* actor_control = obj->getAnimationControl();
		assert(actor_control);
		//����ǲ��ţ�����ȱʡ����

		//����clip
		ActorAnimationClip* default_property_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getDefaultPropertyName());
		//����clip
		actor_control->play(true,0.0f,default_property_ani_clip);
		actor_control->play(false,0.0f,default_property_ani_clip);//ֹͣ

		ActorAnimationClip* stage_property_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
		if(stage_property_ani_clip)
		{
			//����clip
			actor_control->play(true,0.0f,stage_property_ani_clip);
			actor_control->play(false,0.0f,stage_property_ani_clip);//ֹͣ	
		}
		//�������
		m_playing_object_set.insert(obj);
	}
	else
	{

		std::set<DynamicEffectObject*>::iterator iter = m_playing_object_set.find(obj);
		if( iter == m_playing_object_set.end() )
		{
			// assert(!"ɾ�����岻����!");
			return;
		}
		//��ֹͣ���ŵ�������Ϊ������Ⱦ״̬
		//obj->set_visible(false);
		//ɾ������
		m_playing_object_set.erase(iter);
		if(m_playing_object_set.empty())
		{
			//resetPlayMode(PLAY_OBJECT);
			m_play_mode &= (~PLAY_OBJECT);
			resetDynamicEffectObject();
			/*if (m_play_mode == PLAY_STOP)
			{
				setAllRendered(true);
			}*/
		}
	}
};

// ��Ⱦ׶��Ʒ�Χ
void StageAnimationPlayer::drawLightLine()
{
	Stage* stage = dynamic_cast<Stage*>(m_stage);

	if (stage->getLod() == LL_ZERO)
	{
		//���ƾ�̬����Ĺ���
		if (m_static_object)
		{
			drawLightLine(m_static_object);
		}

		if( m_play_mode & PLAY_LEVEL )
		{
			//�����ؿ�
			getAnimationScript()->resetIterator();
			while(StageAnimationCuttingData* cuttingData = getAnimationScript()->next())
			{
				StageAnimationClip* ani_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());

				if (ani_clip && ani_clip->isPlaying())
				{
					drawLightLine(ani_clip);
				}
			}
		}

		if( m_play_mode & PLAY_CLIP )
		{
			for(std::set<StageAnimationClip*>::iterator pos = m_playing_clip_set.begin()
				; pos != m_playing_clip_set.end(); ++pos)
			{
				drawLightLine(*pos);
			}
		}
		if( m_play_mode & PLAY_OBJECT )
		{
			for(std::set<DynamicEffectObject*>::iterator pos = m_playing_object_set.begin()
				; pos != m_playing_object_set.end(); ++pos)
			{
				drawLightLine(*pos);
			}
		}

		if (m_play_mode == PLAY_STOP)
		{
			ActorManager* actor_mng = m_stage->getActorMng();
			actor_mng->reset_iterator();

			Actor* p = actor_mng->next();
			while(NULL != p)
			{
				drawLightLine(dynamic_cast<DynamicEffectObject*>(p));
				p = actor_mng->next();
			}
		}
	}
	else if (stage->getLod() == LL_ONE || stage->getLod() == LL_TWO)
	{
		ActorManager* act_mng = m_stage->getActorMng();
		act_mng->reset_some_sysactor_iter(stage->getLod());
		while(const char* strSysName = act_mng->next_some_sysactor_name(stage->getLod()))
		{
			if (Actor* actor = act_mng->getSystemActor(strSysName))
			{
				if (DynamicEffectObject* lod_light = dynamic_cast<DynamicEffectObject*>(actor))
					drawLightLine(lod_light);
			}
		}
	}
	else 
	{
		assert(false);
	}
	
}

#define _USING_NEW_LIGHT_
#ifdef _USING_NEW_LIGHT_
//�����ƹ�Ч��
void StageAnimationPlayer::enableLightEffect(bool enable)
{
	//H3DI::IRender* renderer = m_stage->getActorMng()->get_graphic_res_mng()->get_render_system()->get_IRender();
	H3DI::ILevel* level = m_stage->getActorMng()->get_level_entity()->get_ILevel();
	if (!enable /*&& !m_optimize_light*/)
	{
		m_stage->getActorMng()->reset_light_status();
		// 		renderer->BeginSetCurrentLight();
		// 		renderer->EndSetCurrentLight();
		return;
	}

	level->SetAmbientColor(m_stage->getActorMng()->get_level_entity()->get_ambient_color().getColor());
	level->SetActorAmbientColor(m_stage->getActorMng()->get_level_entity()->get_ambient_actor_color().getColor());

	//const ActorManager::PrepassLightCollection& created_lightes = m_stage->getActorMng()->get_light_collection();

	//bool need_update = !m_optimize_light || isNeedResetSysLight(created_lightes);
	//if (!need_update)
	//	return;

	//if(m_optimize_light)
	//	LogInfo("set light though use opt light");

	//renderer->BeginSetCurrentLight();

	//// LogInfo("set light ");

	////�����еƹ���Ϊ���ɼ�״̬
	//for (ActorManager::PrepassLightCollection::const_iterator it = created_lightes.begin(); it != created_lightes.end(); ++it)
	//{
	//	CPrePassLightEntity* light = it->first;

	//	bool show_light = it->second && (m_enableScene || (light->get_affect_param() != H3DI::AFFECT_SCENE_ONLY));

	//	if (show_light)
	//	{
	//		// LogInfo("light set ", light->get_actor()->get_name());
	//		renderer->SetCurrentLight(light->get_IPrePassLight());
	//	}
	//	else
	//	{
	//		// LogInfo("light not set ", light->get_actor()->get_name(), " ", light->is_engine_light_enable(), " ", it->second);
	//	}
	//}

	//// LogInfo("set light end");

	//renderer->EndSetCurrentLight();

	//P_END(3);
	//P_FINISH("middle_layer");
	return;
};

#else
//�����ƹ�Ч��
void StageAnimationPlayer::enableLightEffect(bool enable)
{
	if (!enable)
	{
		m_stage->getActorMng()->reset_light_status();

		const ActorManager::PrepassLightCollection& created_lightes = m_stage->getActorMng()->get_light_collection();
		for (ActorManager::PrepassLightCollection::const_iterator it = created_lightes.begin(); it != created_lightes.end(); ++it)
		{
			it->first->set_light_visible_status(it->second);
			it->first->set_light_enable(it->second);
		}
		return;
	}

	//P_BEGIN(3);

	//�����еƹ���Ϊ���ɼ�״̬
	const ActorManager::PrepassLightCollection& created_lightes = m_stage->getActorMng()->get_light_collection();
	for (ActorManager::PrepassLightCollection::const_iterator it = created_lightes.begin(); it != created_lightes.end(); ++it)
	{
		it->first->set_light_visible_status(it->second);
		it->first->set_light_enable(it->second);
	}

	//P_END(3);
	//P_FINISH("middle_layer");
	return;
};
#endif

// ��ȡ�籾������
StageAnimationClipMng* StageAnimationPlayer::getClipManger()
{
	return m_ani_clip_mng;
};
// ��ȡ�ؿ��籾
I_AnimationScript* StageAnimationPlayer::getAnimationScript()
{
	//return m_ani_script;
	return m_playing_script;
};

void StageAnimationPlayer::setTargetPositionManager(ITargetPositionMng* targetposmng)
{
	m_targetposmng = targetposmng;
}

// ��ȡ�༭��Ϣ�б�
bool StageAnimationPlayer::loadEditInfoTable(const char* fileName, bool isBinary)
{
	//����Ԥ���������ļ�
	Stage* stage_tmp = dynamic_cast<Stage*>(m_stage);
	//if(!m_edit_info_table->load(stage_tmp->getDefinedObjectFilename(),isBinary))
	//	return false;//
	if(!m_defined_items->load(stage_tmp->getDefinedObjectFilename(),isBinary))
	{
		return false;
	}
	return true;
}
// ��ñ༭��Ϣ��
//IEditInfoTableEditor* StageAnimationPlayer::getEditInfoTable()
//{
//	return m_edit_info_table;
//};


//����̬Ч��������׶��Ƶ���
void StageAnimationPlayer::drawLightLine(DynamicEffectObject* eff_obj)
{
	eff_obj->reset_entity_iter();
	while(CMediaEndEntity* entity = eff_obj->next_entity())
	{
		if (entity->is_kind_of(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT))
		{
			CPrePassLightEntity* light = dynamic_cast<CPrePassLightEntity*>(entity);
			if ((NULL != light) && light->is_enable() && light->is_visible())
			{
				m_stage->getActorMng()->drawLightLine(light);
			}
		}
	}

	eff_obj->reset_actor_iter();
	while (DynamicEffectObject* child = (DynamicEffectObject*)(eff_obj->next_actor()))
	{
		drawLightLine(child);
	}
}

//����̨Ч����׶��Ƶ���
void StageAnimationPlayer::drawLightLine(StageAnimationClip* ani_clip)
{
	ani_clip->resetObjectIterator();
	while(DynamicEffectObject* eff_obj = ani_clip->nextObject())
	{
		drawLightLine(eff_obj);
	}
}

//���ùؿ�
void StageAnimationPlayer::resetLevel()
{
	DynamicEffectObject* object = NULL;

	//����ؿ����ڲ����еĶ���
	for(std::set<StageAnimationCuttingData*>::iterator pos = m_playing_data_info.begin()
		; pos != m_playing_data_info.end(); )
	{
		//����playing clip
		StageAnimationClip* ani_clip = getClipManger()->find((*pos)->m_animation_clip_id.c_str());

		//���clip�еĶ�̬��Ч���塣
		if(ani_clip != NULL)
		{
			//���õ�ǰ���clip
			if (m_cur_camera_clip == ani_clip)
			{
				m_cur_camera_clip = NULL;
			}

			//ֹͣ����,ֹͣ����ʱ�������Ƿ����
			ani_clip->play(false,0.0f);
		}

		//����ؿ����ڲ����еĶ���
		m_playing_data_info.erase(pos++);
	}

	//��վ�̬����
	resetStaticObject();

	//���clip�еĶ�̬��Ч����
	std::set<DynamicEffectObject*> clip_object;
	std::set<StageAnimationClip*>::const_iterator iter =  m_playing_clip_set.begin();
	for(; iter != m_playing_clip_set.end(); ++iter)
	{
		StageAnimationClip* ani_clip = (*iter);
		if (ani_clip)
		{
			ani_clip->resetObjectIterator();
			object = ani_clip->nextObject();
			while(object)
			{
				clip_object.insert(object);
				object = ani_clip->nextObject();
			}
		}
	}

	//����������ڲ��ŵ�obj���control�����Ϣ
	ActorManager* actor_mng = m_stage->getActorMng();
	actor_mng->reset_iterator();

	while(Actor* p = actor_mng->next())
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
		if (obj)
		{
			if ( m_playing_object_set.find(obj) == m_playing_object_set.end()
				&& clip_object.find(obj) == clip_object.end() )
			{
				DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(obj->getAnimationControl());  
				assert(dyn_effobj_control);
				dyn_effobj_control->clearPlayData();
				obj->m_object_updated = false;
			}
		}
	};

//	DynamicEffectObject::s_system_object = NULL;

	//m_play_mode =  (m_play_mode^PLAY_LEVEL);

	//�������������� lishan 120115
	m_level_playing_acct = 0;
	m_pastScaleList.clear();
}
//���þ�̬����
void StageAnimationPlayer::resetStaticObject()
{
	//��վ�̬����
	if(m_static_object)
	{
		ActorAnimationControl* actor_control = m_static_object->getAnimationControl();
		//����clip
		ActorAnimationClip* actor_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
		//����clip
		if(actor_ani_clip)
		{
			actor_control->play(false,0,actor_ani_clip);
		}
	}
}
//���ö���
void StageAnimationPlayer::resetStageAnimationClip()
{
	DynamicEffectObject* object = NULL;

	std::set<StageAnimationClip*>::iterator iter =  m_playing_clip_set.begin();
	for(; iter != m_playing_clip_set.end(); )
	{
		//����playing clip
		StageAnimationClip* ani_clip = (*iter);

		//���clip�еĶ�̬��Ч���塣
		if(ani_clip != NULL)
		{
			//���õ�ǰ���clip
			if (m_cur_camera_clip == ani_clip)
			{
				m_cur_camera_clip = NULL;
			}

			//��øöζ����а����Ķ�̬��Ч���岢�����PlayData
			ani_clip->resetObjectIterator();
			object = ani_clip->nextObject();
			//ֹͣ���Ÿöζ���
			ani_clip->play(false,0.0f);

			while(object)
			{
				DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(object->getAnimationControl());  
				assert(dyn_effobj_control);
				dyn_effobj_control->clearPlayData();

				object->m_object_updated = false;

				object = ani_clip->nextObject();
			}
		}
		//ɾ���öζ���
		m_playing_clip_set.erase(iter++);
	}

	//m_play_mode =  (m_play_mode^PLAY_CLIP);
}
//��������
void StageAnimationPlayer::resetDynamicEffectObject()
{
	//��������
	std::set<DynamicEffectObject*>::iterator object_it;
	DynamicEffectObject* object = NULL;

	for (object_it = m_playing_object_set.begin(); object_it!= m_playing_object_set.end();)
	{
		object = *object_it;

		//��ն�̬��Ч�����PlayData
		DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(object->getAnimationControl());  
		assert(dyn_effobj_control);
		dyn_effobj_control->clearPlayData();

		object->m_object_updated = false;

		//ɾ��������
		m_playing_object_set.erase(object_it++);
		
	}

	//m_play_mode =  (m_play_mode^PLAY_OBJECT);
}

//���¹ؿ�
void StageAnimationPlayer::updateLevel(float fInterval)
{
	// P_BEGIN(3);

	m_level_playing_acct += fInterval;	//��ǰ���ű����µ�	

	double t_scaled = fInterval * m_time_scale;

	float pastTotalPlayingAcct = 0;
	double pastTotalAccFrame = 0;
	std::list<std::pair<float,double> >::const_iterator pastScaleIter = m_pastScaleList.begin();//��ʷ���ű����µ�
	for(; pastScaleIter != m_pastScaleList.end(); ++pastScaleIter )
	{
		double time_scale_temp = pastScaleIter->second/STD_FRAME_PER_SECOND;//ʱ�����ű���
		pastTotalPlayingAcct += pastScaleIter->first * time_scale_temp;//�ۼ���ʷʱ��
		pastTotalAccFrame += ( pastScaleIter->first / SECOND2MILLONSECONDS ) * pastScaleIter->second;//�ۼ���ʷ֡��
	}

	pastTotalPlayingAcct += m_level_playing_acct*m_time_scale;
	/// �ؿ����Ŷ�Ӧ��֡��
	/// fixme: Ӧ�ó���ÿ֡��Ӧ��ʱ��
	pastTotalAccFrame += ( m_level_playing_acct / SECOND2MILLONSECONDS ) * m_frame_per_second;

	// ������Ҫֹͣ����̨Ч��
	for(std::set<StageAnimationCuttingData*>::iterator pos = m_playing_data_info.begin()
		; pos != m_playing_data_info.end(); )
	{
		StageAnimationCuttingData* ii = *pos;

		//�����ѭ���ģ��������� 
		if (ii->m_loop)
		{
			++pos;
			continue;
		}

		// �Ƿ�ʱ
			if(ii->m_end_bar*STD_FRAME_PER_BAR <= pastTotalAccFrame ||
				ii->m_begin_bar * STD_FRAME_PER_BAR > pastTotalAccFrame)//С�ڻ���Ϊ��׼֡�������ź��֡���бȽ�
		{
			//����playing clip
			StageAnimationClip* ani_clip = getClipManger()->find(ii->m_animation_clip_id.c_str());
			if(ani_clip != NULL)// ֹͣ����
			{
				ani_clip->play(false,0.0f);//ֹͣ����ʱ�������Ƿ����
				ani_clip->setCurrentCuttingData(NULL,NULL);
			}

			m_playing_data_info.erase(pos++);
		}
		else
			++pos;
	}

	

	// P_BEGIN(4);

	// ������Ҫ���µ���̨Ч��
	for(std::set<StageAnimationCuttingData*>::iterator pos = m_playing_data_info.begin()
		; pos != m_playing_data_info.end(); )
	{
		StageAnimationCuttingData* ii = *pos;
		//����playing clip
		StageAnimationClip* ani_clip = getClipManger()->find(ii->m_animation_clip_id.c_str());
		if(ani_clip == NULL)
		{
			m_playing_data_info.erase(pos++);//�Ƴ������ڵĽű�	
			continue;
		};
		// �Ƿ����
		if(ii->m_end_bar*STD_FRAME_PER_BAR > pastTotalAccFrame)//С�ڻ���Ϊ��׼֡�������ź��֡���бȽ�
		{

			int camera_frame_count = ani_clip->getFrameCount();
			double ani_frame_count = (ii->m_end_bar - ii->m_begin_bar) * STD_FRAME_PER_BAR;//ÿ��60

			if(ani_clip->getClipType() == StageAnimationClip_Camera) // ���
			{
				//m_cur_camera_clip = ani_clip;
				ani_clip->update(t_scaled * camera_frame_count / ani_frame_count);//�������ʱ����а���������
			}
			else if(ani_clip->getClipType() == StageAnimationClip_Common)
			{//��ͨ��̨Ч������ѭ������
				float clipBeginBar = ani_clip->getBeginBar();//�õ�clip��ʼС��
				float clipTotalBar = (float)camera_frame_count/STD_FRAME_PER_BAR;//�õ�clip֡����Ӧ��С��

				float clipEndBar = clipBeginBar + clipTotalBar;

				if(pastTotalPlayingAcct >= clipEndBar *  STD_SECONDS_PER_BAR * 1000.0f && !m_loop)//������̨Ч��֡����
				{
					//���²���
					ani_clip->play(true,(pastTotalPlayingAcct - clipEndBar * STD_SECONDS_PER_BAR * 1000.0f));
					ani_clip->setBeginBar(clipEndBar);//�����µĿ�ʼλ��
					ani_clip->update(0);
				}
				else
					ani_clip->update(t_scaled);////��ͨЧ����������lishan 110714  * camera_frame_count / ani_frame_count);//����ʱ����а���������			
			}
			else
				assert(0);
		}
		++pos;
	}

	// ������Ҫ��ʼ����̨Ч�����������Ѿ�������ʱ����и���
	getAnimationScript()->resetIterator();
	StageAnimationCuttingData* nextPlayCuttingData = NULL;
	while(StageAnimationCuttingData* ii = getAnimationScript()->next(&nextPlayCuttingData))
	{
		// �Ƿ��ڷ�Χ��
		if(pastTotalAccFrame >= ii->m_begin_bar*STD_FRAME_PER_BAR && pastTotalAccFrame < ii->m_end_bar*STD_FRAME_PER_BAR )//С�ڻ���Ϊ��׼֡�������ź��֡���бȽ�
		{
			// ��ӻ�������
			if(m_playing_data_info.find(ii) == m_playing_data_info.end())
			{
				StageAnimationClip* ani_clip = getClipManger()->find(ii->m_animation_clip_id.c_str());
				if(ani_clip == NULL)//���clip�����ڣ�����
					continue;

				m_playing_data_info.insert(ii);//�������ڲ��ŵĶ���

				int clip_frame_count = ani_clip->getFrameCount();
				double ani_frame_count = (ii->m_end_bar - ii->m_begin_bar) * STD_FRAME_PER_BAR;//ÿ��60

				if(ani_clip->getClipType() == StageAnimationClip_Camera) // ���
				{
					m_cur_camera_clip = ani_clip;
					//����ʱ����е���,��ʼλ�ð����ź��ʱ����е�����������
					ani_clip->play(true,(pastTotalPlayingAcct - ii->m_begin_bar * 
						STD_SECONDS_PER_BAR * 1000.0f) * clip_frame_count / ani_frame_count);
					
					if(CameraEffectObject* cam = getCurCamera())
					{
						cam->getCamera()->set_target_position_mng(m_targetposmng);
						cam->getCamera()->play_camera();
					}
					ani_clip->update(0);
				}
				else if(ani_clip->getClipType() == StageAnimationClip_Common)
				{
					//����ʱ����е���,��ʼλ�ð����ź��ʱ��������ã�������
					ani_clip->setCurrentCuttingData(ii,nextPlayCuttingData);//����cutting data������������ �ָ� default ����
					ani_clip->play(true,(pastTotalPlayingAcct - ii->m_begin_bar * 
						STD_SECONDS_PER_BAR * 1000.0f));//��ͨЧ����������lishan 110714 * clip_frame_count / ani_frame_count);
					ani_clip->setBeginBar(ii->m_begin_bar);//��ͨ��̨Ч������ѭ�����ţ�����һ����ʼС��
					ani_clip->update(0);
				}
				else
					assert(0);
			}
		}
	}

		//if(m_static_object)//���¾�̬����
		//{
		//	m_static_object->update(fInterval);
		//}

	ActorManager* actor_mng = m_stage->getActorMng();
	actor_mng->reset_iterator();
	while(Actor* p = actor_mng->next())
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
		if(!obj->m_object_updated)
			obj->update(t_scaled);
		else
			obj->m_object_updated = false;
	};

	// P_END(6);

	//���û��clip
	bool no_clip = m_playing_data_info.size() == 0;
	if (no_clip)
	{
		if (m_loop)
		{
			m_level_playing_acct = 0;
			playLevel(false);
			playLevel(true);

			getAnimationScript()->resetIterator();
			if(StageAnimationCuttingData* cutting_data = getAnimationScript()->next())
			{
				updateLevel(0);
			}
		}
		else
		{
			//resetPlayMode();
		}
	}

	// P_END(3);
}

//���¾�̬����
void StageAnimationPlayer::updateStaticObject(float fInterval)
{
	//if(m_static_object && !m_static_object_updated)//���¾�̬����
	if(m_static_object)//���¾�̬����
	{
		m_static_object->update(fInterval);
	}
}

//���µ����Ķ���
void StageAnimationPlayer::updateStageAnimationClip(float fInterval)
{
	if( m_play_mode & PLAY_CLIP )
	{
		fInterval = fInterval * m_time_scale;

		for(std::set<StageAnimationClip*>::iterator pos = m_playing_clip_set.begin()
			; pos != m_playing_clip_set.end(); ++pos)
		{
			StageAnimationClip* ani_clip = *pos;
			ani_clip->update(fInterval);

			if (ani_clip->getClipType() == StageAnimationClip_Camera)
			{
				m_cur_camera_clip = ani_clip;
			}

			if ((m_loop) && (ani_clip->getFrameCount() <= (ani_clip->getTotalUpdateTime() / m_frame_per_second)))
			{
				ani_clip->play(false, fInterval);
				ani_clip->play(true, 0);
			}
		}
	}
}

//���µ����Ķ�̬��Ч����
void StageAnimationPlayer::updateDynamicEffectObject(float fInterval)
{
	if( m_play_mode & PLAY_OBJECT )
	{
		fInterval = fInterval * m_time_scale;
		for(std::set<DynamicEffectObject*>::iterator pos = m_playing_object_set.begin()
			; pos != m_playing_object_set.end(); ++pos)
		{
			DynamicEffectObject* dynaimc_effect_object = *pos;
			dynaimc_effect_object->update(fInterval);
			dynaimc_effect_object->m_object_updated = true;
		}
	}
}

//������������
void StageAnimationPlayer::updateAll(int t)
{
	if(m_static_object)//���¾�̬����
	{
		m_static_object->update(t);
	}

	ActorManager* actor_mng = m_stage->getActorMng();
	actor_mng->reset_iterator();

	Actor* p = actor_mng->next();
	while(NULL != p)
	{
		p->update(t);
		p = actor_mng->next();
	}
}

//��Ⱦ�ؿ�
void StageAnimationPlayer::renderLevel()
{
	if(!m_enableScene)//�رճ��������������壬ֻ����Ӱ���˵ĵƹ�
		return;
	ActorManager* actor_mng = m_stage->getActorMng();
	CRenderSystem* render_system = actor_mng->get_graphic_res_mng()->get_render_system();

	StageAnimationCuttingData* ani_cutdata = NULL;
	StageAnimationClip* ani_clip = NULL;

	// ��Ⱦ���ڲ��ŵ���̨Ч��
	for(std::set<StageAnimationCuttingData*>::iterator pos = m_playing_data_info.begin()
		; pos != m_playing_data_info.end(); ++pos)
	{
		ani_cutdata = (*pos);

		if (ani_cutdata)
		{
			ani_clip = getClipManger()->find(ani_cutdata->m_animation_clip_id.c_str());

			if (ani_clip)
			{
				ani_clip->render();
			}
		}

	}

	/*ActorManager* actor_mng = m_stage->getActorMng();
	CRenderSystem* render_system = actor_mng->get_graphic_res_mng()->get_render_system();

	actor_mng->reset_iterator();

	while(Actor* p = actor_mng->next())
	{
	DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
	obj->render(*render_system);
	};*/
}
//��Ⱦ��̬����
void StageAnimationPlayer::renderStaticObject()
{
	if(!m_enableScene)//�رճ��������������壬ֻ����Ӱ���˵ĵƹ�
		return;

	if(m_static_object)
	{		
		ActorManager* actor_mng = m_stage->getActorMng();
		CRenderSystem* render_system = actor_mng->get_graphic_res_mng()->get_render_system();
		m_static_object->render(*render_system);
	}
}
//��Ⱦclip
void StageAnimationPlayer::renderStageAnimationClip()
{
	for(std::set<StageAnimationClip*>::iterator pos = m_playing_clip_set.begin()
		; pos != m_playing_clip_set.end(); ++pos)
	{
		StageAnimationClip* ani_clip = *pos;
		ani_clip->render();
	}	
}

//��Ⱦ��̬��Ч����
void StageAnimationPlayer::renderDyanmicEffectObject()
{
	ActorManager* actor_mng = m_stage->getActorMng();
	CRenderSystem* render_system = actor_mng->get_graphic_res_mng()->get_render_system();

	DynamicEffectObject* dynamic_effect_object = NULL;

	for(std::set<DynamicEffectObject*>::iterator pos = m_playing_object_set.begin()
		; pos != m_playing_object_set.end(); ++pos)
	{
		dynamic_effect_object = *pos;
		dynamic_effect_object->render(*render_system);
	}
}

////��Ⱦ��̬����
//void StageAnimationPlayer::renderStaticObject()
//{
//	ActorManager* actor_mng = m_stage->getActorMng();
//	CRenderSystem* render_system = actor_mng->get_graphic_res_mng()->get_render_system();
//
//	//���ƾ�̬����
//	if(m_static_object)
//	{
//		m_static_object->render(*render_system);
//	}
//}

//��Ⱦ��������
void StageAnimationPlayer::renderAll()
{
	ActorManager* actor_mng = m_stage->getActorMng();
	CRenderSystem& render_system = *(actor_mng->get_graphic_res_mng()->get_render_system());

	if(m_static_object)//���ƾ�̬����
	{
		m_static_object->render(render_system);
	}

	actor_mng->reset_iterator();
	Actor* p = actor_mng->next();
	while (NULL != p)
	{
		p->render(render_system);
		p = actor_mng->next();
	}
}

unsigned int StageAnimationPlayer::pushHardwareSelect(bool sel_line)
{
	int count = 0;

	if (m_level_lod == LL_ZERO)
	{
		if (m_static_object)
		{
			CRenderSystem* render_system =  m_ani_clip_mng->getStage()->getActorMng()->get_graphic_res_mng()->get_render_system();
			count += m_static_object->pushHardwareSelect(*render_system, sel_line);
		}

		if( m_play_mode & PLAY_LEVEL )
		{
			/*if (m_static_object)
			{
				CRenderSystem* render_system =  m_ani_clip_mng->getStage()->getActorMng()->get_graphic_res_mng()->get_render_system();
				count += m_static_object->pushHardwareSelect(*render_system, sel_line);
			}*/

			//�����ؿ�
			getAnimationScript()->resetIterator();
			while(StageAnimationCuttingData* cuttingData = getAnimationScript()->next())
			{
				StageAnimationClip* ani_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());

				if (ani_clip && ani_clip->isPlaying())
				{
					count += ani_clip->pushHardwareSelect(sel_line);
				}
			}
			
		}
		if( m_play_mode & PLAY_CLIP )
		{
			for(std::set<StageAnimationClip*>::iterator pos = m_playing_clip_set.begin()
				; pos != m_playing_clip_set.end(); ++pos)
			{
				count += (*pos)->pushHardwareSelect(sel_line);
			}
		}
		if( m_play_mode & PLAY_OBJECT )
		{
			CRenderSystem* render_system =  m_ani_clip_mng->getStage()->getActorMng()->get_graphic_res_mng()->get_render_system();

			for(std::set<DynamicEffectObject*>::iterator pos = m_playing_object_set.begin()
				; pos != m_playing_object_set.end(); ++pos)
			{
				count += (*pos)->pushHardwareSelect(*render_system, sel_line);
			}
		}
		if (m_play_mode == PLAY_STOP)
		{
			CRenderSystem* render_system =  m_ani_clip_mng->getStage()->getActorMng()->get_graphic_res_mng()->get_render_system();

			/*if (m_static_object)
			{
				count += m_static_object->pushHardwareSelect(*render_system ,sel_line);
			}*/

			ActorManager* actor_mng = m_stage->getActorMng();
			actor_mng->reset_iterator();

			Actor* p = actor_mng->next();
			while(NULL != p)
			{
				count += dynamic_cast<DynamicEffectObject*>(p)->pushHardwareSelect(*render_system ,sel_line);
				p = actor_mng->next();
			}
		}
	}
	else if (m_level_lod == LL_ONE || m_level_lod == LL_TWO)
	{
		CRenderSystem* render_system =  m_ani_clip_mng->getStage()->getActorMng()->get_graphic_res_mng()->get_render_system();
		ActorManager* actor_mng = m_stage->getActorMng();
		actor_mng->reset_some_sysactor_iter(m_level_lod);
		while(const char* strname = actor_mng->next_some_sysactor_name(m_level_lod))
		{
			Actor* actor = actor_mng->find_actor(strname);
			if (actor)
			{
				count += dynamic_cast<DynamicEffectObject*>(actor)->pushHardwareSelect(*render_system, sel_line);
			}
		}
	}
	else
	{
		assert(false);
	}
	
	return count;
}


void StageAnimationPlayer::drawLightLocationLine()
{
	CRenderSystem& render_sys = *(m_stage->getActorMng()->get_graphic_res_mng()->get_render_system());
	Stage* stage = dynamic_cast<Stage*>(m_stage);
	ActorManager* act_mng = m_stage->getActorMng();

	if (stage->getLod() == LL_ZERO)
	{
		const ActorManager::PrepassLightCollection& created_lightes = m_stage->getActorMng()->get_light_collection();
		for (ActorManager::PrepassLightCollection::const_iterator it = created_lightes.begin(); it != created_lightes.end(); ++it)
		{
			Actor* act = act_mng->find_actor(it->first->get_actor()->get_name());
			//if (act != NULL && it->second)
			/*added by wangheng 2012.06.20 �༭��Ҫ��ƹ��Ƿ���ö���Ҫ���������ߣ�
			* ���Ա༭����λ�����Ƿ���Ʋ�������it->second���жϣ�
			* Ӧ������is_enable��is_visible�������������жϡ�
			*/
			if (act != NULL && it->first->is_visible() && it->first->is_enable())
			{
				it->first->draw_light_select_line(render_sys); 
			}
		}
	}
	else if (stage->getLod() == LL_ONE || stage->getLod() == LL_TWO)
	{
		
		act_mng->reset_some_sysactor_iter(stage->getLod());
		while(const char* strSysName = act_mng->next_some_sysactor_name(stage->getLod()))
		{
			if (Actor* actor = act_mng->find_actor(strSysName))
			{
				Actor::EntityList entitylist;
				actor->contain_entity_type_recursive(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT, entitylist);
				for (Actor::EntityList::iterator it = entitylist.begin(); it != entitylist.end(); ++it)
				{
					CPrePassLightEntity* light = dynamic_cast<CPrePassLightEntity*>(*it);
					if (light->is_visible() && light->is_enable())
					{
						light->draw_light_select_line(render_sys); 
					}
				}
			}
		}
	}
}

void StageAnimationPlayer::setLoop(bool loop)
{
	m_loop = loop;
}

bool StageAnimationPlayer::isLoop()
{
	return m_loop;
}

I_AnimationScript* StageAnimationPlayer::getAnimationScriptByID(const char* script_id)
{
	std::map<std::string, AnimationScript*>::iterator iter = m_ani_script_list.find(script_id);
	if (iter != m_ani_script_list.end())
	{
		return iter->second;
	} 
	else
	{
		return NULL;
	}
}
//void StageAnimationPlayer::setLightOptimize(bool optimize)
//{
//	m_optimize_light = optimize;
//}

//bool StageAnimationPlayer::isNeedResetSysLight(const ActorManager::PrepassLightCollection& col)
//{
//	bool ret = false;
// //	if (m_light_status_col.size() != col.size())
// //		ret = true;
// //	else
// //	{
// //		for (ActorManager::PrepassLightCollection::const_iterator it = col.begin(); it != col.end(); ++it)
// //		{
// //			ActorManager::PrepassLightCollection::iterator orgin_it = m_light_status_col.find(it->first);
// //			bool need_reset = (orgin_it == m_light_status_col.end()) || (orgin_it->second != it->second);
// //			if (need_reset)
// //			{
// //				ret = true;
// //				break;;
// //			}
// //		}
// //	}
// //
// //	if (ret)
// //	{
// //		m_light_status_col = col;
// //	}
//
//	ActorManager::PrepassLightCollection using_lights;
//	for (ActorManager::PrepassLightCollection::const_iterator it = col.begin(); it != col.end(); ++it)
//	{
//		if (it->second && (m_enableScene || (it->first->get_affect_param() != H3DI::AFFECT_SCENE_ONLY)))
//		{
//			using_lights[it->first] = it->second;
//		}
//	}
//
//	if (using_lights.size() != m_light_status_col.size())
//	{
//		ret = true;
//	}
//	else
//	{
//		ActorManager::PrepassLightCollection::const_iterator it = using_lights.begin();
//		ActorManager::PrepassLightCollection::const_iterator orgin_it = m_light_status_col.begin();
//		for (; it != using_lights.end(); ++it, ++orgin_it)
//		{
//			if (it->first != orgin_it->first)
//			{
//				ret = true;
//				break;
//			}
//		}
//	}
//
//	if (ret)
//	{
//		m_light_status_col = using_lights;
//	}
//
//	return ret;
//}

void StageAnimationPlayer::updateUsingLightCol()
{
	m_using_light_col.clear();
	m_using_actor_light_col.clear();

	const ActorManager::PrepassLightCollection& created_lightes = m_stage->getActorMng()->get_light_collection();
	for (ActorManager::PrepassLightCollection::const_iterator it = created_lightes.begin(); it != created_lightes.end(); ++it)
	{
		CPrePassLightEntity* e = it->first;
		e->enable_engine_light(it->second);
		
		if(e->get_affect_param() == H3DI::AFFECT_SCENE_ONLY)
		{
			m_using_light_col.insert(e->get_IPrePassLight());
		}
		else
		{
			const LightCachedProperty* lcp = e->get_light_property();
			m_using_actor_light_col[e->get_IPrePassLight()] = lcp;
		}
	}
}

void StageAnimationPlayer::setColorFactor(float* color, int size)
{
	if(m_static_object)
		m_static_object->setColorFactor(color, size);
}

void StageAnimationPlayer::enableScene(bool enable)
{
	m_enableScene = enable;
	const ActorManager::PrepassLightCollection& created_lightes = m_stage->getActorMng()->get_light_collection();
	for (ActorManager::PrepassLightCollection::const_iterator it = created_lightes.begin(); it != created_lightes.end(); ++it)
	{
 		if (it->first->get_affect_param() == H3DI::AFFECT_SCENE_ONLY)
 		{
 			it->first->set_external_visible(enable);
 		}
	}

	
	//���ƾ�̬����
	if(m_static_object)
	{
		//m_static_object->set_visible(enable);
		Actor::EntityList entitys;
		m_static_object->contain_entity_recursive(entitys);
		Actor::EntityList::iterator itr = entitys.begin();
		for(; itr != entitys.end(); ++itr)
		{
			if (!(*itr)->is_kind_of(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT))
			{
				(*itr)->set_external_visible(enable);
			}
		}
	}

	
	StageAnimationCuttingData* ani_cutdata = NULL;
	StageAnimationClip* ani_clip = NULL;

	//��Ⱦ��̨�ϵĶ�̬����
	ActorManager* actor_mng = m_stage->getActorMng();
	actor_mng->reset_iterator();
	while(Actor* act = actor_mng->next())
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(act);
		if (stricmp("x52_model", obj->getDefinedType()) == 0 ||
			stricmp("x52_effect", obj->getDefinedType()) == 0)
		{
			act->set_external_visible(enable);
		}
	}
};

float StageAnimationPlayer::getCurrentTotalAccumulateTime()
{

	float pastTotalPlayingAcct = 0;

	//�ۼ���ʷ���ű����µ�ʱ��
	std::list<std::pair<float,double> >::const_iterator pastScaleIter = m_pastScaleList.begin();
	for(; pastScaleIter != m_pastScaleList.end(); ++pastScaleIter )
	{
		double time_scale_temp = pastScaleIter->second/STD_FRAME_PER_SECOND;//ʱ�����ű���
		pastTotalPlayingAcct += pastScaleIter->first * time_scale_temp;//�ۼ���ʷʱ��
	}
	//�ۼӵ�ǰ���ű����µ�ʱ��
	pastTotalPlayingAcct += m_level_playing_acct*m_time_scale;

	return pastTotalPlayingAcct;
}

//void StageAnimationPlayer::_do_play_sequence(bool play)
//{
//	m_level_playing_acct = 0 ;//- m_entertime;
//	ScriptSequence::iterator iter = m_script_sequence.begin();
//	AnimationScript* last_script = NULL;
//	for (;iter != m_script_sequence.end(); ++iter)
//	{
//		if (*iter == m_playing_script)
//		{
//			if (m_playing_script != m_script_sequence.front())
//			{	
//				last_script = *(--iter);
//				break;
//			}
//			break;
//		}
//	}
//
//
//	if (m_playing_script != m_script_sequence.front() && last_script != NULL)
//	{
//		//�����нű��Ż�
//		std::pair<StageAnimationCuttingData*, StageAnimationCuttingData*> p;
//		//_checkScriptConnection(p, )
//		
//	}
//
//	m_play_mode |= PLAY_LEVEL;
//}
///<��¼clip��
void StageAnimationPlayer::recordClipResetModifier(const char* firstClipId,const char* secondClipId)
{
	if(!secondClipId)
		m_aniClipPair.insert(std::make_pair<std::string,std::string>(firstClipId,""));
	else
		m_aniClipPair.insert(std::make_pair<std::string,std::string>(firstClipId,secondClipId));
};

void StageAnimationPlayer::calculateClipResetModifier(const char* firstClipId,const char* secondClipId)
{
	assert(firstClipId);
	StageAnimationClip* ani_clip = getClipManger()->find(firstClipId);

	if(ani_clip)
	{
		ani_clip->resetObjectIterator();
		while(DynamicEffectObject* ani_obj = ani_clip->nextObject())//������������,���и�λ����
		{
			//ȡ������
			DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(ani_obj->getAnimationControl());  
			assert(dyn_effobj_control);
			//������������и�λ���ü���
			dyn_effobj_control->calcResetModifier(firstClipId,secondClipId);
		}
	}		
};
void StageAnimationPlayer::_do_play_level( bool play )
{
	m_level_playing_acct = 0 ;//- m_entertime;

	if(m_env_loaded)
	{
		m_stage->getActorMng()->get_level_entity()->get_ILevel()->SetTransmissionColor(m_trans_color);

		//���ù⻷
		if(m_halo_config.m_load_succ)//�����ȡ����
		{
			m_stage->getActorMng()->get_graphic_res_mng()->get_render_system()->get_IRender()->SetLensParameters(m_halo_config.m_lensEnv,m_halo_config.m_lensFalres,m_halo_config.m_lensFlaresNums);
		}
	}

	//����clip���л���
	//��¼ϵͳ��������
	ActorManager* actor_mng = m_stage->getActorMng();
	actor_mng->reset_iterator();

	while(Actor* p = actor_mng->next())
	{
		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
		DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(obj->getAnimationControl());  
		assert(dyn_effobj_control);

		if (!dynamic_cast<CameraEffectObject*>(p))
		{
			dyn_effobj_control->initPlayData();//������ʼ����״̬
		}

// 		if( stricmp(obj->getDefinedType() , "x52_system") == 0)//��¼ϵͳ��������
// 			DynamicEffectObject::s_system_object = obj;
	};
	//����cutting data
	getAnimationScript()->sortCuttingData();
	getAnimationScript()->resetIterator();
	StageAnimationCuttingData* nextPlayCuttingData = NULL;
	while(StageAnimationCuttingData* cuttingData = getAnimationScript()->next(&nextPlayCuttingData))//��������ʼʱ�������CLIP
	{
		if(cuttingData->m_data_type == StageAnimationCuttingData_Camera)//���������modifier
			continue;

		if(nextPlayCuttingData)
		{
			assert(nextPlayCuttingData->m_begin_bar >= cuttingData->m_begin_bar);
			assert(nextPlayCuttingData->m_begin_bar >= cuttingData->m_end_bar);
		}

		//�ж��Ƿ������ڵ�clip
		std::string nextClipId;
		if(nextPlayCuttingData)
			nextClipId = nextPlayCuttingData->m_animation_clip_id;

		StageAnimationClip* ani_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());

		if(ani_clip == NULL )//û���ҵ���̨Ч��
			cuttingData->m_animation_clip_id =  getAnimationScript()->getDefaultAnimation();//����ȱʡ��

		ani_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());

		if(ani_clip)
		{
			ani_clip->resetObjectIterator();
			while(DynamicEffectObject* ani_obj = ani_clip->nextObject())//������������,���и�λ����
			{
				//ȡ������
				DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(ani_obj->getAnimationControl());  
				assert(dyn_effobj_control);
				//������������и�λ���ü���
				dyn_effobj_control->calcResetModifier(cuttingData->m_animation_clip_id.c_str(),nextClipId.c_str());
			}
		}
	}

	std::set<std::pair<std::string,std::string> >::const_iterator setIter = m_aniClipPair.begin();
	for(;setIter != m_aniClipPair.end(); ++setIter)
	{
		calculateClipResetModifier(setIter->first.c_str(),setIter->second.c_str());
	}
	//actor_mng->reset_iterator();
	//while(Actor* p = actor_mng->next())
	//{
	//	DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
	//	DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(obj->getAnimationControl());  
	//	assert(dyn_effobj_control);

	//	dyn_effobj_control->endCalcReset();//���ø�λ���������ͬʱ����ȱʡ����
	//}

	//���ž�̬Ч������
	playStaticObject(play);
	
	//����ϵͳ��Ϣ
// 	if(DynamicEffectObject::s_system_object)//����ϵͳ��Ϣ
// 	{
// 		ActorAnimationControl* actor_control = DynamicEffectObject::s_system_object->getAnimationControl();
// 		//����clip
// 		ActorAnimationClip* default_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getDefaultPropertyName());
// 		//����clip
// 		if(default_ani_clip)//����default����
// 		{
// 			actor_control->play(true,0,default_ani_clip);
// 			actor_control->play(false,0,default_ani_clip);
// 		}
// 		//����clip
// 		ActorAnimationClip* stage_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
// 		//����clip
// 		if(stage_ani_clip)//����stage����
// 		{
// 			actor_control->play(true,0,stage_ani_clip);
// 			actor_control->play(false,0,stage_ani_clip);
// 		}
// 	}

	m_play_mode |= PLAY_LEVEL;
}

StageAnimationCuttingData* StageAnimationPlayer::getCurrentCuttingData()
{
	for(std::set<StageAnimationCuttingData*>::iterator pos = m_playing_data_info.begin()
		; pos != m_playing_data_info.end(); )
	{
		StageAnimationCuttingData* ii = *pos;
		return ii;
	}

	return NULL;
}
void StageAnimationPlayer::playScriptByID(const char* script_id)
{
	std::map<std::string, AnimationScript*>::iterator iter = m_ani_script_list.find(script_id);

	if (iter != m_ani_script_list.end())
	{
		m_playing_script = iter->second;
		playLevel(true);
		setLoop(false);
	}
}

bool StageAnimationPlayer::saveScriptByID(const char* script_id, const char* levelFile, bool isBinary)
{
	std::map<std::string, AnimationScript*>::iterator iter = m_ani_script_list.find(script_id);

	if (iter != m_ani_script_list.end())
	{
		if (iter->second->save(levelFile,isBinary))
			return true;
	}
	return false;
}

void StageAnimationPlayer::setFramePerSecondAndRestorePastTime(double frame)
{
	m_pastScaleList.push_back(std::make_pair<int,double>(m_level_playing_acct,m_frame_per_second));
	m_level_playing_acct = 0;		
	setFramePerSecond(frame);
};

void StageAnimationPlayer::_loadEnvironmentLight()
{
	if(m_env_loaded)
	{
		m_stage->getActorMng()->get_level_entity()->get_ILevel()->SetTransmissionColor(m_trans_color);

		//���ù⻷
		if(m_halo_config.m_load_succ)//�����ȡ����
		{
			m_stage->getActorMng()->get_graphic_res_mng()->get_render_system()->get_IRender()->SetLensParameters(m_halo_config.m_lensEnv,m_halo_config.m_lensFalres,m_halo_config.m_lensFlaresNums);
		}
	}
}

void StageAnimationPlayer::_playStaticObject(bool play)
{
	//���ž�̬Ч������
	if(m_static_object)
	{
		ActorAnimationControl* actor_control = m_static_object->getAnimationControl();
		//����clip
		ActorAnimationClip* actor_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
		//����clip
		if(actor_ani_clip)
		{
			actor_control->play(play,0,actor_ani_clip);
		}
	}
}

void StageAnimationPlayer::_setSystemInfo()
{
	//����ϵͳ��Ϣ
// 	if(DynamicEffectObject::s_system_object)//����ϵͳ��Ϣ
// 	{
// 		ActorAnimationControl* actor_control = DynamicEffectObject::s_system_object->getAnimationControl();
// 		//����clip
// 		ActorAnimationClip* default_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getDefaultPropertyName());
// 		//����clip
// 		if(default_ani_clip)//����default����
// 		{
// 			actor_control->play(true,0,default_ani_clip);
// 			actor_control->play(false,0,default_ani_clip);
// 		}
// 		//����clip
// 		ActorAnimationClip* stage_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
// 		//����clip
// 		if(stage_ani_clip)//����stage����
// 		{
// 			actor_control->play(true,0,stage_ani_clip);
// 			actor_control->play(false,0,stage_ani_clip);
// 		}
// 	}
}
//void StageAnimationPlayer::setScriptConnection(const char* script_id, const char* next_script_id)
//{
//	m_script_connection_list.insert(make_pair(script_id, next_script_id));
//}
//
//void _get_cutting_data(StageAnimationCuttingData*& cutting_data, std::map<std::string, AnimationScript*>& script_list
//					   , const char* script_id, bool get_first) 
//{
//	std::map<std::string, AnimationScript*>::iterator iter = script_list.find(script_id);
//	if (iter != script_list.end())
//	{
//		iter->second->sortCuttingData();
//		get_first
//			? cutting_data = iter->second->getFirstCuttingData()
//			: cutting_data = iter->second->getLastCuttingData();
//	}
//	else
//		cutting_data = 0;
//}
//
//bool StageAnimationPlayer::_checkScriptConnection(std::pair<StageAnimationCuttingData*, StageAnimationCuttingData*>& opt_pair
//												  , const char* script_id, const char* next_script_id)
//{
//	std::pair<std::string, std::string> p = make_pair(script_id, next_script_id);
//	ScriptConnectionList::iterator iter = m_script_connection_list.find(p);
//	if (iter != m_script_connection_list.end())
//	{
//		_get_cutting_data(opt_pair.first, m_ani_script_list, script_id, false);
//		_get_cutting_data(opt_pair.second, m_ani_script_list, next_script_id, true);
//
//		if (opt_pair.first != NULL && opt_pair.second != NULL)
//			return true;	 
//	}
//	return false;
//}
//
//void StageAnimationPlayer::insertScriptSequence(const char* script_id)
//{
//	std::map<std::string, AnimationScript*>::iterator iter = m_ani_script_list.find(script_id);
//	if (iter != m_ani_script_list.end())
//	{
//		m_script_sequence.push_back(iter->second);
//	}
//}
//
//void StageAnimationPlayer::setUsingSequence(bool using_sequence)
//{
//	m_script_sequence.size() < 2 && using_sequence && m_script_connection_list.size() == 0
//		? m_play_by_script_sequence = true
//		: m_play_by_script_sequence = false;
//}
//
//void StageAnimationPlayer::doPreWorkForPlaySeq( bool play )
//{
//	resetPlayMode(PLAY_LEVEL);
//
//	_checkObjConflict();
//
//	if(m_env_loaded)
//	{
//		m_stage->getActorMng()->get_graphic_res_mng()->get_render_system()->get_IRender()->SetTransmissionColor(m_trans_color);
//		m_stage->getActorMng()->get_graphic_res_mng()->get_render_system()->set_ambient_color(H3DColor(m_ambient_color));
//		m_stage->getActorMng()->get_graphic_res_mng()->get_render_system()->get_IRender()->SetAmbientColor(m_ambient_color);
//
//		//���ù⻷
//		if(m_halo_config.m_load_succ)//�����ȡ����
//		{
//			m_stage->getActorMng()->get_graphic_res_mng()->get_render_system()->get_IRender()->SetLensParameters(m_halo_config.m_lensEnv,m_halo_config.m_lensFalres,m_halo_config.m_lensFlaresNums);
//		}
//	}
//
//	//����clip���л���
//	//��¼ϵͳ��������
//	ActorManager* actor_mng = m_stage->getActorMng();
//	actor_mng->reset_iterator();
//
//	while(Actor* p = actor_mng->next())
//	{
//		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
//		DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(obj->getAnimationControl());  
//		assert(dyn_effobj_control);
//
//		if (!dynamic_cast<CameraEffectObject*>(p))
//		{
//			dyn_effobj_control->initPlayData();//������ʼ����״̬
//		}
//
//		if( stricmp(obj->getDefinedType() , "x52_system") == 0)//��¼ϵͳ��������
//			DynamicEffectObject::s_system_object = obj;
//	};
//
//	//���ž�̬Ч������
//	if(m_static_object)
//	{
//		ActorAnimationControl* actor_control = m_static_object->getAnimationControl();
//		//����clip
//		ActorAnimationClip* actor_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
//		//����clip
//		if(actor_ani_clip)
//		{
//			actor_control->play(play,0,actor_ani_clip);
//		}
//	}
//	//����ϵͳ��Ϣ
//	if(DynamicEffectObject::s_system_object)//����ϵͳ��Ϣ
//	{
//		ActorAnimationControl* actor_control = DynamicEffectObject::s_system_object->getAnimationControl();
//		//����clip
//		ActorAnimationClip* default_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getDefaultPropertyName());
//		//����clip
//		if(default_ani_clip)//����default����
//		{
//			actor_control->play(true,0,default_ani_clip);
//			actor_control->play(false,0,default_ani_clip);
//		}
//		//����clip
//		ActorAnimationClip* stage_ani_clip = actor_control->find_animation_clip(DynamicEffectObject::getStagePropertyName());
//		//����clip
//		if(stage_ani_clip)//����stage����
//		{
//			actor_control->play(true,0,stage_ani_clip);
//			actor_control->play(false,0,stage_ani_clip);
//		}
//	}
//}
//
//void StageAnimationPlayer::_doCurrentScriptOpt()
//{
//	getAnimationScript()->sortCuttingData();
//	getAnimationScript()->resetIterator();
//	while(StageAnimationCuttingData* cuttingData = getAnimationScript()->next())//��������ʼʱ�������CLIP
//	{
//		if(cuttingData->m_data_type == StageAnimationCuttingData_Camera)//���������modifier
//			continue;
//
//		StageAnimationClip* ani_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());
//
//		if(ani_clip == NULL )//û���ҵ���̨Ч��
//			cuttingData->m_animation_clip_id =  getAnimationScript()->getDefaultAnimation();//����ȱʡ��
//
//		ani_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());
//
//		if(ani_clip)
//		{
//			ani_clip->resetObjectIterator();
//			while(DynamicEffectObject* ani_obj = ani_clip->nextObject())//������������,���и�λ����
//			{
//				//ȡ������
//				DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(ani_obj->getAnimationControl());  
//				assert(dyn_effobj_control);
//				//������������и�λ���ü���
//				dyn_effobj_control->calcResetModifier(cuttingData);
//			}
//		}
//	}
//
//	ActorManager* actor_mng = m_stage->getActorMng();
//	actor_mng->reset_iterator();
//	while(Actor* p = actor_mng->next())
//	{
//		DynamicEffectObject* obj = dynamic_cast<DynamicEffectObject*>(p);
//		DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(obj->getAnimationControl());  
//		assert(dyn_effobj_control);
//
//		dyn_effobj_control->endCalcReset();//���ø�λ���������ͬʱ����ȱʡ����
//	}
//}
//
//void StageAnimationPlayer::_doScriptConnectOpt()
//{
//	if (m_script_sequence.size() < 2)//С�������ű��Ĳ����Ż� 
//	{
//		return;
//	}
//
//	ScriptSequence::iterator iter = m_script_sequence.begin();
//	ScriptSequence::iterator ender = m_script_sequence.end();
//	ender--;//������1��ʱ�ս�
//	for (; iter != ender;)
//	{
//		AnimationScript* script_first = *iter;
//		iter++;
//		AnimationScript* script_next = *iter;
//
//		StageAnimationClip* last_clip = getClipManger()->find(script_first->getLastCuttingData()->m_animation_clip_id.c_str());
//		StageAnimationClip* next_clip = getClipManger()->find(script_next->getFirstCuttingData()->m_animation_clip_id.c_str());
//
//		if (next_clip)
//		{
//			next_clip->resetObjectIterator();
//			while(DynamicEffectObject* ani_obj = next_clip->nextObject())//������������,���и�λ����
//			{
//				//ȡ������
//				DynEffObjControl* dyn_effobj_control = dynamic_cast<DynEffObjControl*>(ani_obj->getAnimationControl());  
//				assert(dyn_effobj_control);
//				//������������и�λ���ü���
//				dyn_effobj_control->calcOptRestModifier(script_first->getLastCuttingData(), script_next->getFirstCuttingData());
//			}
//		}
//	}
//}
//
//void StageAnimationPlayer::cacheSequence()
//{
//	if (!m_play_by_script_sequence)
//		return;
//	
//	//�Ż��ű��ν�
//	_doScriptConnectOpt();
//
//	//�Ż������ű�
//	ScriptSequence::iterator iter = m_script_sequence.begin();
//	while (iter != m_script_sequence.end())
//	{
//		m_playing_script = *iter;
//		_doCurrentScriptOpt();
//	}
//	m_playing_script = m_script_sequence.front();
//}
//
//void StageAnimationPlayer::_checkObjConflict()
//{
//	ScriptSequence::iterator iter = m_script_sequence.begin();
//	for (; iter != m_script_sequence.end(); ++iter)
//	{
//		m_playing_script = *iter;
//		//--------------�ж��Ƿ��뵥�����ŵ�������ߵ������ŵ���Ч��ͻ-------
//		//�������ŵĶ����а����Ķ�̬��Ч����
//		std::set<DynamicEffectObject*> clip_object;
//
//		DynamicEffectObject* object = NULL;
//
//		//��õ������ŵĶ����а����Ķ�̬��Ч����
//		for(std::set<StageAnimationClip*>::const_iterator pos = m_playing_clip_set.begin()
//			; pos != m_playing_clip_set.end(); ++pos)
//		{
//			//����playing clip
//			StageAnimationClip* ani_clip = (*pos);
//			//���clip�еĶ�̬��Ч���塣
//			if(ani_clip != NULL)
//			{
//				ani_clip->resetObjectIterator();
//				object = ani_clip->nextObject();
//				while(object)
//				{
//					clip_object.insert(object);
//					object = ani_clip->nextObject();
//				}				
//			}
//		}
//
//		//�����ؿ�
//		getAnimationScript()->resetIterator();
//		while(StageAnimationCuttingData* cuttingData = getAnimationScript()->next())
//		{
//			StageAnimationClip* ani_clip = getClipManger()->find(cuttingData->m_animation_clip_id.c_str());
//
//			if (ani_clip)
//			{
//				//�뵥�����ŵĶ�����ͻ��
//				if (m_playing_clip_set.find(ani_clip)!=m_playing_clip_set.end())
//				{
//					assert(FAILURE);
//				}
//
//				//��ýű������漰�Ķ�̬��Ч����
//				ani_clip->resetObjectIterator();
//				object = ani_clip->nextObject();
//				while(object)
//				{
//					//�뵥�����ŵĶ����а����Ķ�̬��Ч�����ͻ
//					if (clip_object.find(object) != clip_object.end())
//					{
//						assert(FAILURE);
//					}
//
//					//�뵥�����ŵ������ͻ
//					if (m_playing_object_set.find(object) != m_playing_object_set.end())
//					{
//						assert(FAILURE);
//					}
//
//					object = ani_clip->nextObject();
//				}	
//			}
//
//		}
//	}
//	m_playing_script = m_script_sequence.front();
//}

StageAnimationPlayerEditor::StageAnimationPlayerEditor(IStage* stage)
:StageAnimationPlayer(stage)
{
}

StageAnimationPlayerEditor::~StageAnimationPlayerEditor()
{	
}
void StageAnimationPlayerEditor::updateClip(float fInterval, StageAnimationClip* ani_clip)
{
	assert(ani_clip);
	if( m_play_mode & PLAY_CLIP )
	{
		fInterval = fInterval * m_time_scale;
		ani_clip->update(fInterval);

		if ((m_loop) && (ani_clip->getFrameCount() <= (ani_clip->getTotalUpdateTime() / m_frame_per_second)))
		{
			ani_clip->play(false, fInterval);
			ani_clip->play(true, 0);
		}

		if (ani_clip->getClipType() == StageAnimationClip_Camera)
		{
			m_cur_camera_clip = ani_clip;		

			//���������жϾ�̬����ĸ��¼��ʱ���Ƿ����������һ��
			/*if (m_static_object_updated)
				return;*/

			if (m_play_mode & PLAY_LEVEL)
				return;
			else if (!(m_play_mode & PLAY_OBJECT ||m_play_mode & PLAY_LEVEL))
			{
				std::set<StageAnimationClip*>::iterator itr = m_playing_clip_set.begin();
				for (;itr != m_playing_clip_set.end(); ++itr)
				{
					if ((*itr)->getClipType() == StageAnimationClip_Common)
						return;
				}
			}
		}
		
		updateStaticObject(fInterval);
	}

	
}
void StageAnimationPlayerEditor::update(float tInterval)
{
	if (m_play_mode == PLAY_STOP)
	{
		tInterval = tInterval * m_time_scale;
		updateAll(tInterval);
	}
	else
	{
		if (m_play_mode & PLAY_OBJECT)
		{
			updateDynamicEffectObject(tInterval);
		}
		if (m_play_mode & PLAY_CLIP)
		{
			updateStageAnimationClip(tInterval);
		}
		if (m_play_mode & PLAY_LEVEL)
		{
			updateLevel(tInterval);
		}
		updateStaticObject(tInterval);
		
	}
}
IStage* StageAnimationPlayerEditor::getStage()
{
	return m_stage;
}

bool StageAnimationPlayer::switchStaticActorLod(int lod)
{
	DynamicEffectObject* static_object = getStaticActor();
	if (static_object)
	{
		Actor::EntityList entitylist;
		static_object->contain_entity_recursive(entitylist);
		Actor::EntityList::iterator itr = entitylist.begin();
		while (itr != entitylist.end())
		{
			C3DEntity* entity3d = dynamic_cast<C3DEntity*>(*itr);
			if (entity3d)
			{
				//ϵͳ�ƹⲻ����lod
				if (entity3d->is_kind_of(MEDIA_END_ENTITY_TYPE_PREPASS_LIGHT))
				{
					std::string strName = entity3d->get_name();
					std::transform(strName.begin(), strName.end(), strName.begin(), tolower);

					std::string::size_type pos_lod1 = strName.find(ActorManager::getLodSystemActorName(LL_ONE), 0);
					std::string::size_type pos_lod2 = strName.find(ActorManager::getLodSystemActorName(LL_TWO), 0);
					CPrePassLightEntity* light = dynamic_cast<CPrePassLightEntity*>(entity3d);
					if (lod == LL_ZERO)
					{
						if ((pos_lod1 != std::string::npos && 
							pos_lod1 == (strName.length() - strlen(ActorManager::getLodSystemActorName(LL_ONE)))) ||
							(pos_lod2 != std::string::npos && 
							pos_lod2 == (strName.length() - strlen(ActorManager::getLodSystemActorName(LL_TWO)))))
						{
							light->get_IPrePassLight()->SetLightEnable(false);
							++itr;
							continue;
						}
					}
					else if (lod == LL_ONE)
					{
						if ((pos_lod1 != std::string::npos && 
							pos_lod1 == (strName.length() - strlen(ActorManager::getLodSystemActorName(LL_ONE)))))
						{
							light->get_IPrePassLight()->SetLightEnable(true);
							++itr;
							continue;
						}

						if((pos_lod2 != std::string::npos && 
							pos_lod2 == (strName.length() - strlen(ActorManager::getLodSystemActorName(LL_TWO)))))
						{
							light->get_IPrePassLight()->SetLightEnable(false);
							++itr;
							continue;
						}
					}
					else if (lod == LL_TWO)
					{
						if ((pos_lod1 != std::string::npos && 
							pos_lod1 == (strName.length() - strlen(ActorManager::getLodSystemActorName(LL_ONE)))))
						{
							light->get_IPrePassLight()->SetLightEnable(false);
							++itr;
							continue;
						}

						if((pos_lod2 != std::string::npos && 
							pos_lod2 == (strName.length() - strlen(ActorManager::getLodSystemActorName(LL_TWO)))))
						{
							light->get_IPrePassLight()->SetLightEnable(true);
							++itr;
							continue;
						}
					}

				}

				//
				entity3d->switch_to_lod(lod);
				if (entity3d->type() == MEDIA_END_ENTITY_TYPE_MODEL)
				{
					CModelEntity* model = dynamic_cast<CModelEntity*>(entity3d);
					if (lod == LL_ZERO)
					{
						model->cancle_lightmap();
						model->set_recive_fakelightspot(false);
					}
					else if (lod == LL_ONE)
					{
						//model->load_lightmap();
						model->apply_lightmap();
						model->set_recive_fakelightspot(model->get_recive_fakelightspot_record());
					}
					else if (lod == LL_TWO)
					{
						//model->load_lightmap();
						model->apply_lightmap();
						model->set_recive_fakelightspot(model->get_recive_fakelightspot_record());
					}
				}
			}
			++itr;
		}
	}

	return true;
}

bool StageAnimationPlayer::switchToLod(int lod)
{
	/*if (lod == m_level_lod)
		return true;*/

	//removeLodSystemClip(m_level_lod);
	removeLodSystemClip(LL_ZERO);
	removeLodSystemClip(LL_ONE);
	removeLodSystemClip(LL_TWO);
	addLodSystemClip(lod);

	m_level_lod = lod;
	return true;
}

void StageAnimationPlayer::removeLodSystemClip(LOD_LEVEL lod)
{
	ActorManager* act_mng = m_stage->getActorMng();
	StageAnimationClip* stage_clip = NULL;

	DynamicEffectObject* sys_actor = NULL;
	act_mng->reset_some_sysactor_iter(lod);
	while(const char* sys_name = act_mng->next_some_sysactor_name(lod))
	{
		//sys_actor = dynamic_cast<DynamicEffectObject*>(act_mng->find_actor(sys_name));
		sys_actor = dynamic_cast<DynamicEffectObject*>(act_mng->getSystemActor(sys_name));
		if (sys_actor)
		{
			sys_actor->reset_control_iter();
			while(DynEffObjControl* control = dynamic_cast<DynEffObjControl*>(sys_actor->next_control()))
			{
				control->reset_iterator();
				while(const char* clip_name = control->next_clip_name())
				{
					if (stricmp(clip_name, DynamicEffectObject::getDefaultPropertyName()) == 0 ||
						stricmp(clip_name, DynamicEffectObject::getStagePropertyName()) == 0)
						continue;

					stage_clip = m_ani_clip_mng->find(clip_name);
					stage_clip->remove(sys_actor, false);
				}
			}
		}
	}
}
void StageAnimationPlayer::addLodSystemClip(int lod)
{
	ActorManager* act_mng = m_stage->getActorMng();
	StageAnimationClip* stage_clip = NULL;

	DynamicEffectObject* sys_actor = NULL;
	act_mng->reset_some_sysactor_iter(lod);
	while(const char* sys_name = act_mng->next_some_sysactor_name(lod))
	{
		sys_actor = dynamic_cast<DynamicEffectObject*>(act_mng->find_actor(sys_name));
		if (sys_actor)
		{
			sys_actor->reset_control_iter();
			while(DynEffObjControl* control = dynamic_cast<DynEffObjControl*>(sys_actor->next_control()))
			{
				control->reset_iterator();
				control->initPlayData();
				while(const char* clip_name = control->next_clip_name())
				{
					if (stricmp(clip_name, DynamicEffectObject::getDefaultPropertyName()) == 0 ||
						stricmp(clip_name, DynamicEffectObject::getStagePropertyName()) == 0)
						continue;

					stage_clip = m_ani_clip_mng->find(clip_name);
					if (stage_clip)
					{
						std::set<StageAnimationClip*>::iterator playing_it = m_playing_clip_set.find(stage_clip);
						if (playing_it != m_playing_clip_set.end())
						{
							control->play(false, 0, clip_name);
							control->play(true, 0, clip_name);
						}
						else
						{
							for(std::set<StageAnimationCuttingData*>::iterator pos = m_playing_data_info.begin()
								; pos != m_playing_data_info.end(); )
							{
								StageAnimationCuttingData* ii = *pos;

								//���ڲ��� 
								if (stricmp(clip_name, ii->m_animation_clip_id.c_str()) == 0)
								{
									control->play(false, 0, clip_name);
									control->play(true, 0, clip_name);
									break;
								}
								else
									++pos;
							}
						}
						stage_clip->insert(sys_actor);
					}
				}
			}
		}
	}
}
