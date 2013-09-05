/**
* @file stage_animation_clip_mng.h
* 这个文件的主要功能是：
* 
* @author 李汕
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 李汕2010.11.16
*/

#ifndef _STAGE_ANIMATION_CLIP_MNG_H_
#define _STAGE_ANIMATION_CLIP_MNG_H_

#include "middle_layer_header.h"

class StageAnimationClipMng;
class DynamicEffectObject;
class Stage;
class ActorAnimationClip;
class PropertyModifierFactory;
//class CStagePropertyTable;
class IPropertyModifier;
class CDefinedItemCollection;
class CStageDefinedItemCollection;
// 舞台效果类型
enum StageAnimationClipType
{
	StageAnimationClip_Common,
	StageAnimationClip_Camera, // 相机
};
class StageAnimationClip
{
private:
	//动画片段管理器
	StageAnimationClipMng* m_ani_clip_mng;
	//动画片段ID
	std::string m_id;
	//动画片段类型
	StageAnimationClipType m_ani_clip_type;
	//帧数
	int m_frame_count;
	//物体列表
	std::list<DynamicEffectObject*> m_object_list;
	//物体迭代器
	std::list<DynamicEffectObject*>::const_iterator m_object_list_iter;

	//是否正在播放
	bool	m_is_playing;
	//是否需要被渲染
	bool	m_is_visible;
	//是否开启后处理信息

	//记录当前播放的cutting data,用于给object control 中的复位map做索引
	StageAnimationCuttingData* m_current_cutting_data;
	//记录当前播放的紧邻的下一个cutting data,用于给object control 中的复位map做索引
	StageAnimationCuttingData* m_next_cutting_data;
	//总的更新时间
	float	m_total_update_time;
	//起始小节数
	float m_begin_bar;
private:
	//解析节点，建立物体效果
	bool createDynamicEffect(BiXmlNode* effectNode);
	//外部建立节点，用于外部建立aniClip
	bool externalCreateDynamicEffect(ActorManager* actorManager, DynamicEffectObject* effectObj, BiXmlNode* effectNode);
	//设置后处理切换信息
	void setPostProcessSwitchInfo(const char* filename);
	//播放舞台效果
	void playObj(DynamicEffectObject* obj, bool isPlay, float t);
	//保存该clip中系统物体的动画数据
	bool saveClipStageSystemObjects(BiXmlNode* parentNode, bool consistentId ,
		const char* file_main_name, bool isOptimize=true);
public:
	StageAnimationClip(StageAnimationClipMng* clip_mng);
	virtual ~StageAnimationClip();

	StageAnimationClipMng* getClipMng() { return m_ani_clip_mng;}

	//解析文件
	bool load(const char* filename, bool isBinary);
	//存储文件
	bool save(const char* filename, bool isBinary, bool consistentId = false, bool isOptimize = true);

	bool externalLoad(ActorManager* actorManager, DynamicEffectObject* effectObj,const char* filename0, bool isBinary);

	static bool removeDynamicEffectObjectFromFile(const char* filename, const char* id);
	//解析文件,返回clip id
	static std::string getFileClipId(const char* filename, bool isBinary);

	//解析文件,返回clip type
	static std::string getFileClipType(const char* filename, bool isBinary);
	
	// 重置物体信息迭代器
	virtual void resetObjectIterator() ;
	// 获得物体信息，迭代器指向下一个物体信息，如迭代器无效返回0
	virtual DynamicEffectObject* nextObject() ;

	//设置舞台效果id
	virtual const char* getID() const{return m_id.c_str();};
	//获取舞台效果id
	virtual void setID(const char* id);
	//获取clip的长度
	virtual int getFrameCount() const{return m_frame_count;};
	//设置clip的长度
	virtual void setFrameCount(int count){ m_frame_count = count;};
	//设置动画类型
	StageAnimationClipType getClipType(){return m_ani_clip_type;};
	//取动画类型
	void setClipType(StageAnimationClipType clip_type){m_ani_clip_type = clip_type;};

	//判断是否播放
	bool isPlaying();

	//播放
	virtual void play(bool isPlay, float t);
	//渲染
	virtual void render();
	virtual void set_visible(bool visible);
	virtual bool is_visible();
	//进行硬件选择渲染
	virtual unsigned int pushHardwareSelect(bool sel_line);
	//更新, 单位毫秒
	virtual void update(float t);

	//添加效果物体
	virtual void insert(DynamicEffectObject* eff_obj);
	//移除效果物体
	virtual void remove(DynamicEffectObject* eff_obj,bool remove_clip = true);
	//查找
	virtual DynamicEffectObject* findEffectObject(const char*);
	
	//设置当前cutting data
	void setCurrentCuttingData(StageAnimationCuttingData* cutting_data, StageAnimationCuttingData* next_cutting_data){m_current_cutting_data = cutting_data; m_next_cutting_data = next_cutting_data;};
	
	float getTotalUpdateTime();
	//设置起始小节，用于循环播放
	void setBeginBar(float beginBar){m_begin_bar = beginBar;};
	//取起始小节
	float getBeginBar(){return m_begin_bar;};
};
class StageAnimationClipMng
{
private:
	//缺省clip类型，内部用于处理流程控制
	enum{DEFAULT_OBJECT_PROPERTY,DEFAULT_STAGE_PROPERTY};

	IStage* m_stage;

	//生成的clip
	std::map<std::string,StageAnimationClip*> ani_clip_map;
	// 当前迭代器位置
	std::map<std::string,StageAnimationClip*>::iterator m_clip_pos;
	//相机及对应的效果文件
	std::map<std::string,std::string> m_cameraPathMap;

	std::map<std::string, std::string> m_clips_path_list;

private:
	bool parseStageEffect(BiXmlNode* rootNode, bool isBinary, const std::string& working_dir, I_Keep_Client_Alive* pLp = 0);
	//建立OBJECT缺省值clip
	bool createStagePropertyClip(const char* default_clip_name,const CDefinedItemCollection* defined_item_collection);
	//将某个lod级别的系统物体的舞台效果删除
	void removeSystemActorClip(LOD_LEVEL ll, const char* clipid);
	//将某个lod级别的系统物体的舞台效果重命名
	void renameSystemActorClip(LOD_LEVEL ll, const char* clipid, const char* newname);
public:
	StageAnimationClipMng(IStage* stage);
	virtual ~StageAnimationClipMng();
	//解析文件
	bool load(const char* filename, bool isBinary, I_Keep_Client_Alive* pLp = 0);
	//存储文件
	bool save(const char* filename, bool isBinary, bool isOptimize);

	// 建立所有动态效果物体默认的舞台属性集
	bool loadDefaultStageEffects(const char* filename, bool isBinary);

	// 载入包含的舞台效果列表及路径
	bool loadEffectPathList(const char* filename, bool isBinary);

	// 载入一个单独的舞台效果
	bool loadStageEffect(const char* filename, bool isBinary);

	//取stage指针
	Stage* getStage();

	// 创建给定类型的舞台效果
	virtual StageAnimationClip* createStageAnimationClip(StageAnimationClipType t, const char* clipid);
	// 销毁给定的舞台效果
	virtual void releaseStageAnimationClip(StageAnimationClip* stageaniclip);

	// 添加一个舞台效果
	virtual void insert(StageAnimationClip* stageaniclip) ;
	// 删除一个舞台效果
	virtual void remove(StageAnimationClip* stageaniclip) ;
	// 修改舞台效果名称
	virtual void renameClip(StageAnimationClip* clip, const char* newname);
	//查找指定的舞台效果
	virtual StageAnimationClip* find(const char* clipid);

	// 重置舞台效果迭代器
	virtual void resetIterator() ;
	// 获得舞台效果，迭代器指向下一个舞台效果，如迭代器无效返回0
	virtual StageAnimationClip* next() ;
	//获取stage的缺省属性设置
	bool generateStagePropertyInfo(BiXmlNode* effect_object_node, DynamicEffectObject* effect_object );	
	//从关卡文件外读取舞台效果
	virtual bool loadEffect(const char* fileName, bool isBinary);
	//根据id取相机文件
	const char* getCameraFile(const char* cameraName);

	void clear() {m_cameraPathMap.clear();}
#ifdef _EDITOR

	//生成的clip
	std::map<std::string,StageAnimationClip*> m_common_camera_clips;
	//camera
	std::map<std::string,CameraEffectObject*> m_common_cameras;

	//相机及对应的效果文件
	std::map<std::string,std::string> m_common_camera_pathes;
	
	bool loadCommonCamera();
	/**
	@brief 解析通用镜头定义
	*/
	bool parseCameraFile(const char* cameraFile);

	CameraEffectObject* getCommonCamera(const char* strCameraName);
	StageAnimationClip* getCommonCameraClip(const char* strClipName);

#endif
};


#endif //_STAGE_ANIMATION_CLIP_MNG_H_
//_STAGE_ANIMATION_CLIP_MNG_H_
