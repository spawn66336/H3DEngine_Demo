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

#ifndef _STAGE__H_
#define _STAGE__H_

#include "middle_layer_header.h"
#include "stage_post_processor.h"

class StageAnimationPlayer;
class DynamicEffectObject;
class Stage;
class CDefinedItemCollection;
class StageAnimationClipMng;
//��̬Ч�������೧
class DefinedObjectFactory
{
private:
	Stage* m_stage;
	//Ԥ����items
	const CDefinedItemCollection* m_defined_item_collection;
private:
	//����XML�ļ����������˽ṹ
	DynamicEffectObject* createObjectRecursive(const DefinedItem* defined_item,const char* obj_id);
	//������һ����
	DynamicEffectObject* createDynamicEffectObject(ActorManager* actor_manager, const char* name, const char* type);
public:
	DefinedObjectFactory(Stage* stage);
	~DefinedObjectFactory();
	//����Ԥ����items
	void setDefinedItemCollection(const CDefinedItemCollection* collection){m_defined_item_collection = collection;};
	//������̬Ч����̨��Ŀǰ��֧��һ�����͵���� x52_Camera
	virtual DynamicEffectObject* createDynamicActor(const char* actor_name, const char* actor_type);
};

struct SceneLodInfo
{
	SceneLodInfo()
	{
		reset();
	}
	void reset()
	{
		m_small_actor_start_disappear = -1.f;
		m_small_actor_far_disappear = -1.f;
		m_huge_actor_start_disappear = -1.f;
		m_huge_actor_far_disappear = -1.f;
		m_detail_actor_start_disappear = -1.f;
		m_detail_actor_far_disappear = -1.f;
	}
	float		m_small_actor_start_disappear;	//С����������������ʧ����
	float		m_small_actor_far_disappear;	
	float		m_huge_actor_start_disappear;	//��������������������ʧ����
	float		m_huge_actor_far_disappear;
	float		m_detail_actor_start_disappear;	//ϸ��ֲ������������������ʧ����
	float		m_detail_actor_far_disappear;
};

class Stage : public IStage
{
	friend class StageAnimationPlayer;
private:
	//ACTOR MANAGER
	ActorManager* m_actor_manager;
	StageAnimationPlayer* m_ani_player;
	//Ԥ�������������
	DefinedObjectFactory* m_defined_object_factory;
	std::string			  m_defined_object_file_path;

	bool m_is_outdoor; /// �Ƿ�Ϊ���ⳡ��
	float m_min_color_brightness; /// ��������С����
	int m_stage_lod;

	StagePostProcessor*	m_post_processor;

	SceneLodInfo m_scene_lod_info[3];

private:
	//�ݹ齨����̬Ч������
	DynamicEffectObject* createStageDynamicEffectObjectRecursive(BiXmlNode* effectObjNode);
	//�ݹ�洢��̬Ч������
	bool saveStageDynamicEffectObjectRecursive(BiXmlNode* parentNode, DynamicEffectObject* obj ,StageAnimationClipMng* clip_mng);
	//������̨��̬Ч������
	bool loadStageDynamicEffectObjects(const char* stagefile, bool binary, I_Keep_Client_Alive* pLp = 0);
	//�洢��̨��̬Ч������
	bool saveStageDynamicEffectObjects(const char* stage_file, bool isBinary);
	bool saveStageSystemObjects(BiXmlNode* entityNode);

	// ������̨��һЩ������Ϣ������������Ϣ���������ֵ
	bool loadStageMiscInfo(const char* stageFile, bool isBinary);

	//�л�ϵͳ����
	void switchSceneEnv(int lod);

public:
	Stage();
	~Stage();

	//�����ó�ʼ��
	virtual bool initialize();
	//��ʼ��
	virtual bool initialize(H3DI::IRender* irender, ITargetPositionMng* targetposmng, H3DI::ILevel* ilevel = NULL);
	//����
	virtual void reset();

	// ���붯̬����
	virtual bool loadDynamicObjects(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp = 0);

	// ��ȡ��̨��Ϣ����̬Ч�����塢��̨Ч����
	virtual bool loadStageFile(const char* stageFile, bool isBinary,I_Keep_Client_Alive* pLp = 0) ;
	// ������̨��Ϣ����̬Ч�����塢��̨Ч����
	virtual bool saveStageFile(const char* stageFile, bool isBinary, bool isOptimize) ;

	// ��ȡ�籾��Ϣ
	virtual bool loadAnimationFile(const char* levelFile, bool isBinary) ;
	// ����籾��Ϣ
	virtual bool saveAnimationFile(const char* levelFile, bool isBinary) ;

	//��ö���������
	virtual IStageAnimationPlayer* getPlayer();
	//��ȡActorMng
	virtual ActorManager* getActorMng() ;
	//ȡԤ���嶯̬Ч�������ļ���
	virtual const char* getDefinedObjectFilename();
	//����Ԥ���嶯̬Ч�������ļ���
	virtual void setDefinedObjectFileName(const char* filePath);
	//������Դ�ļ����·��
	virtual void setDefaultResourcePath(const char* default_eff_res
										, const char* default_model_res
										, const char* default_actor_res
										, const char* default_skeleton_model_res);
	//�����ߵ�shaderlib
	virtual void setLineShaderUILib(const char* line_shader_ui_lib);
	//����Ԥ���嶯̬Ч������
	virtual DynamicEffectObject* createDefinedObject(const char* object_name, const char* object_type);
	//����Ԥ���������
	virtual void deleteDefinedObject(DynamicEffectObject* eff_obj);
	
	//������弶����ϵ,parent_obj��ActorManager�еĶ������塣���Stage clip���Ѿ�����sub_obj������Ҫ����ɾ�����á�
	bool addObjectLink(DynamicEffectObject* parent_obj,DynamicEffectObject* sub_obj);
	//�Ƴ����弶����ϵ,parent_obj��ActorManager�еĶ������塣���Stage clip���Ѿ�����parent_obj��sub_obj���ŵ�parent_obj���ڵ�clip�����С�
	bool removeObjectLink(DynamicEffectObject* parent_obj,DynamicEffectObject* sub_obj);
	//��̬������弰��̨Ч��
	virtual bool addDynamicObjectAndEffect(const char* objectName,const char* type,const char* effectPath, bool isBinary = false);
	//����idȡ����ļ�
	virtual const char* getCameraFile(const char* cameraName);
	//�Ƿ�Ϊ���ڳ���
	bool isOutdoor() const { return m_is_outdoor; }
	//���������Ƚ��͵����ٺ��ʣ�- 1��ʾ������Ч
	float getMinBrightNess() const { return m_min_color_brightness; }

	bool switchToLod(int lod);

	LOD_LEVEL getLod()const{return (LOD_LEVEL)m_stage_lod;};

	H3DI::ILevel* getILevel(){return m_actor_manager->get_level_entity()->get_ILevel();};

	/**
	* @brief ��ȡ��ǰ��̨�ĺ��������
	*/
	virtual I_Stage_PostProcessor* getPostProcessor(){return m_post_processor;};

	/**
	* @brief ���º���Ч��
	*/
	virtual void updatePostProcess(CCameraEntity* camera, bool particalviewport = false);
	virtual void updatePostProcess(const CCameraEntity::PostprocessCollection& process_collection, bool particalviewport = false);

	/**
	* @brief ȡactorManager
	*/
	ActorManager* getActorManager(){return m_actor_manager;};

	SceneLodInfo& getScenLodInfo(int lod){return m_scene_lod_info[lod];};
};
//·������������
class MiddleLayerFilePathHelper
{
public:
	//�����ָ�����ת��Ϊ "**/**/**"
	static void convertSeperator(const char* src_filename, std::string& strFileName);
	//ȡ����·����ת��Ϊ "**/**/**"
	static void getWorkingPath(const char* src_filename, std::string& strWorkingPath);
};
class MiddleLayerStrHelper
{
public:
	//�滻�ַ���
	static void replaceStr(std::string& inputStr, const char* oldstr,const char* newstr);
};
#endif //_STAGE__H_
