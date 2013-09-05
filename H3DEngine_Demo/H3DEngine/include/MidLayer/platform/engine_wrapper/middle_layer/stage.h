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

#ifndef _STAGE__H_
#define _STAGE__H_

#include "middle_layer_header.h"
#include "stage_post_processor.h"

class StageAnimationPlayer;
class DynamicEffectObject;
class Stage;
class CDefinedItemCollection;
class StageAnimationClipMng;
//动态效果物体类厂
class DefinedObjectFactory
{
private:
	Stage* m_stage;
	//预定义items
	const CDefinedItemCollection* m_defined_item_collection;
private:
	//解析XML文件，建立拓扑结构
	DynamicEffectObject* createObjectRecursive(const DefinedItem* defined_item,const char* obj_id);
	//建立单一物体
	DynamicEffectObject* createDynamicEffectObject(ActorManager* actor_manager, const char* name, const char* type);
public:
	DefinedObjectFactory(Stage* stage);
	~DefinedObjectFactory();
	//设置预定义items
	void setDefinedItemCollection(const CDefinedItemCollection* collection){m_defined_item_collection = collection;};
	//建立动态效果舞台，目前能支持一个类型的相机 x52_Camera
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
	float		m_small_actor_start_disappear;	//小物体的消隐距离和消失距离
	float		m_small_actor_far_disappear;	
	float		m_huge_actor_start_disappear;	//大型物体的消隐距离和消失距离
	float		m_huge_actor_far_disappear;
	float		m_detail_actor_start_disappear;	//细节植被物体的消隐距离和消失距离
	float		m_detail_actor_far_disappear;
};

class Stage : public IStage
{
	friend class StageAnimationPlayer;
private:
	//ACTOR MANAGER
	ActorManager* m_actor_manager;
	StageAnimationPlayer* m_ani_player;
	//预定义对象建立工厂
	DefinedObjectFactory* m_defined_object_factory;
	std::string			  m_defined_object_file_path;

	bool m_is_outdoor; /// 是否为室外场景
	float m_min_color_brightness; /// 场景的最小亮度
	int m_stage_lod;

	StagePostProcessor*	m_post_processor;

	SceneLodInfo m_scene_lod_info[3];

private:
	//递归建立动态效果物体
	DynamicEffectObject* createStageDynamicEffectObjectRecursive(BiXmlNode* effectObjNode);
	//递归存储动态效果物体
	bool saveStageDynamicEffectObjectRecursive(BiXmlNode* parentNode, DynamicEffectObject* obj ,StageAnimationClipMng* clip_mng);
	//加载舞台动态效果物体
	bool loadStageDynamicEffectObjects(const char* stagefile, bool binary, I_Keep_Client_Alive* pLp = 0);
	//存储舞台动态效果物体
	bool saveStageDynamicEffectObjects(const char* stage_file, bool isBinary);
	bool saveStageSystemObjects(BiXmlNode* entityNode);

	// 载入舞台的一些杂项信息，比如类型信息、最低亮度值
	bool loadStageMiscInfo(const char* stageFile, bool isBinary);

	//切换系统环境
	void switchSceneEnv(int lod);

public:
	Stage();
	~Stage();

	//测试用初始化
	virtual bool initialize();
	//初始化
	virtual bool initialize(H3DI::IRender* irender, ITargetPositionMng* targetposmng, H3DI::ILevel* ilevel = NULL);
	//重置
	virtual void reset();

	// 载入动态物体
	virtual bool loadDynamicObjects(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp = 0);

	// 读取舞台信息（动态效果物体、舞台效果）
	virtual bool loadStageFile(const char* stageFile, bool isBinary,I_Keep_Client_Alive* pLp = 0) ;
	// 保存舞台信息（动态效果物体、舞台效果）
	virtual bool saveStageFile(const char* stageFile, bool isBinary, bool isOptimize) ;

	// 读取剧本信息
	virtual bool loadAnimationFile(const char* levelFile, bool isBinary) ;
	// 保存剧本信息
	virtual bool saveAnimationFile(const char* levelFile, bool isBinary) ;

	//获得动画播放器
	virtual IStageAnimationPlayer* getPlayer();
	//获取ActorMng
	virtual ActorManager* getActorMng() ;
	//取预定义动态效果物体文件名
	virtual const char* getDefinedObjectFilename();
	//设置预定义动态效果物体文件名
	virtual void setDefinedObjectFileName(const char* filePath);
	//设置资源文件相关路径
	virtual void setDefaultResourcePath(const char* default_eff_res
										, const char* default_model_res
										, const char* default_actor_res
										, const char* default_skeleton_model_res);
	//设置线的shaderlib
	virtual void setLineShaderUILib(const char* line_shader_ui_lib);
	//生成预定义动态效果物体
	virtual DynamicEffectObject* createDefinedObject(const char* object_name, const char* object_type);
	//销毁预定义的物体
	virtual void deleteDefinedObject(DynamicEffectObject* eff_obj);
	
	//添加物体级联关系,parent_obj是ActorManager中的顶层物体。如果Stage clip中已经包含sub_obj，还需要从中删除引用。
	bool addObjectLink(DynamicEffectObject* parent_obj,DynamicEffectObject* sub_obj);
	//移除物体级联关系,parent_obj是ActorManager中的顶层物体。如果Stage clip中已经包含parent_obj，sub_obj将放到parent_obj所在的clip引用中。
	bool removeObjectLink(DynamicEffectObject* parent_obj,DynamicEffectObject* sub_obj);
	//动态添加物体及舞台效果
	virtual bool addDynamicObjectAndEffect(const char* objectName,const char* type,const char* effectPath, bool isBinary = false);
	//根据id取相机文件
	virtual const char* getCameraFile(const char* cameraName);
	//是否为室内场景
	bool isOutdoor() const { return m_is_outdoor; }
	//场景的亮度降低到多少合适，- 1表示亮度无效
	float getMinBrightNess() const { return m_min_color_brightness; }

	bool switchToLod(int lod);

	LOD_LEVEL getLod()const{return (LOD_LEVEL)m_stage_lod;};

	H3DI::ILevel* getILevel(){return m_actor_manager->get_level_entity()->get_ILevel();};

	/**
	* @brief 获取当前舞台的后处理管理器
	*/
	virtual I_Stage_PostProcessor* getPostProcessor(){return m_post_processor;};

	/**
	* @brief 更新后处理效果
	*/
	virtual void updatePostProcess(CCameraEntity* camera, bool particalviewport = false);
	virtual void updatePostProcess(const CCameraEntity::PostprocessCollection& process_collection, bool particalviewport = false);

	/**
	* @brief 取actorManager
	*/
	ActorManager* getActorManager(){return m_actor_manager;};

	SceneLodInfo& getScenLodInfo(int lod){return m_scene_lod_info[lod];};
};
//路径调整帮助类
class MiddleLayerFilePathHelper
{
public:
	//调整分隔符，转换为 "**/**/**"
	static void convertSeperator(const char* src_filename, std::string& strFileName);
	//取工作路径，转换为 "**/**/**"
	static void getWorkingPath(const char* src_filename, std::string& strWorkingPath);
};
class MiddleLayerStrHelper
{
public:
	//替换字符串
	static void replaceStr(std::string& inputStr, const char* oldstr,const char* newstr);
};
#endif //_STAGE__H_
