/**
* @file stage_animation_player.h
* 这个文件的主要功能是：
* 
* @author 李汕
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 李汕2010.11.16
*/

#ifndef _STAGE_ANIMATION_PLAYER_H_
#define _STAGE_ANIMATION_PLAYER_H_

#include "middle_layer_header.h"

class DynamicEffectObject;
class StageAnimationClipMng;
class AnimationScript;
class StageAnimationClip;
struct StageAnimationCuttingData;
//class CEditInfoTable;
class CDefinedItemCollection;
class CameraEffectObject;
class ITargetPositionMng;
struct SceneLodInfo;

#define STD_FRAME_PER_SECOND (33.333333f)
#define STD_SECONDS_PER_BAR (2.4f)
#define STD_FRAME_PER_BAR  (STD_FRAME_PER_SECOND * STD_SECONDS_PER_BAR)

//用于读取光环属性的类
class HaloConfig
{
public:
	//光环设置
	H3DI::tLensEnv m_lensEnv;
	H3DI::tLensFlareParameters* m_lensFalres;
	H3DVec3 m_sunPos;
	std::vector<std::string> m_lensMatVec;
	std::vector<std::string> m_lensTypeVec;
	int m_lensFlaresNums;
	//是否读取成功
	bool m_load_succ;
public:
	HaloConfig();
	~HaloConfig();
	//读取光环属性
	bool loadLensValue(BiXmlNode* node);
};
class StageAnimationPlayerEditor;
class StageAnimationPlayer : public IStageAnimationPlayer
{
	friend StageAnimationPlayerEditor;
private:
	enum PlayMode{
		PLAY_STOP	= 0x0,
		PLAY_LEVEL	= 0x01,
		PLAY_CLIP	= 0x01<<1,
		PLAY_OBJECT = 0x01<<2
	};

	int	m_level_lod;
	//播放模式
	int m_play_mode;
	//舞台
	IStage* m_stage;
	//动画管理器
	StageAnimationClipMng* m_ani_clip_mng;
	//脚本管理器
	AnimationScript* m_ani_script;
	//每秒帧数,外部设置
	double m_frame_per_second;
	//时间缩放比例
	double m_time_scale;
	//播放累计时间
	float m_level_playing_acct;
	// 正在播放的关卡信息
	std::set<StageAnimationCuttingData*> m_playing_data_info;
	//正在播放的clip
	std::set<StageAnimationClip*> m_playing_clip_set;
	//正在播放的DynamicEffectObject
	std::set<DynamicEffectObject*> m_playing_object_set;
	//预定义类型信息
	//CEditInfoTable* m_edit_info_table;
	//带层次的类型信息
	CDefinedItemCollection* m_defined_items;

	//当前的相机
	CameraEffectObject*	 m_cur_camera;
	StageAnimationClip*	 m_cur_camera_clip;
	//静态物体播放
	DynamicEffectObject* m_static_object;
	std::map<std::string,std::pair<std::string,int> > static_property_map;
	std::set<std::string> point_light_set;
	std::set<std::string> direction_light_set;
	std::set<std::string> wimble_light_set;
	bool m_static_object_loaded;

	// 目标位置获取管理器
	ITargetPositionMng* m_targetposmng;
	//entertime,毫秒
	int m_entertime;
	//次表面光散射演示，配置文件中取得
	bool m_env_loaded; //表明环境光是否加载了
	bool m_stage_loaded; //表明场景是否加载了

	//阴影范围属性
	float m_shadow_scope;
	
	float m_ambient_actor_color[4];
	float m_ambient_color[4];
	float m_trans_color[4];
	//给demo做的单独读取后处理参数 lishan 110114
	std::string m_lut_texture;
	//光环效果参数  lishan 110114
	HaloConfig m_halo_config;

	bool m_loop; //是否进行循环播放

	//ActorManager::PrepassLightCollection m_light_status_col;
	//bool		  m_optimize_light;
	std::set<H3DI::IPrePassLight*> m_using_light_col;
	
	/// 正在使用的人物灯光
	std::map<H3DI::IPrePassLight*, const LightCachedProperty*> m_using_actor_light_col;


	bool m_enableScene;//是否显示场景

	///< 动画脚本列表, 参数： 脚本id字符串、 脚本对象指针
	std::map<std::string, AnimationScript*> m_ani_script_list;
	///< 当前播放的动作脚本
	AnimationScript* m_playing_script;

	///< 脚本播放序列，用来标示按顺序播放的一连串脚本
	//typedef std::vector<AnimationScript*> ScriptSequence;
	//ScriptSequence	m_script_sequence;

	///< 标记是否通过播放序列来播放舞台
	//bool	m_play_by_script_sequence;

	///< 标记在播放序列脚本时，是否已经被reset过播放状态
	//bool    m_script_sequence_resetted;

	///<过去曾经设置的所有scale
	std::list<std::pair<float,double> > m_pastScaleList;

	std::set<std::pair<std::string,std::string> > m_aniClipPair;//取有链接关系的cliop
public:
	//将与关卡互斥的物体从播放列表中移除
	void ExcludeClipAndDynEffObj4Level();
	//将与舞台效果互斥的动态特效物体从播放列表中移除
	void ExcluedLevelAndDynEffObj4Clip(const char* ani_name);
	//将与动态特效物体互斥的物体从播放列表中移除
	void ExcludeLevelAndClip4DynEffObj(DynamicEffectObject* obj);
private:
	//从播放列表中移除与场景互斥的舞台效果
	void stopAllStageAnimationClip();

	//从播放列表中移除与场景互斥的动态特效物体
	void stopAllDynamicEffectObject();

	//////////////////////////////////////////
	//判断关卡、正在播放的动画和物体是否冲突//
	//////////////////////////////////////////
	bool isLevelAndClipConflicted();
	bool isLevelAndDynEffObjConflicted();
	bool isClipAndDynEffObjConflicted(const char* ani_name);
	bool isClipAndLevelConflicted(const char* ani_name);
	bool isDynEffObjAndLevelConflicted(DynamicEffectObject* obj);
	bool isDynEffObjAndClipConflicted(DynamicEffectObject* obj);


	void resetPlayMode(PlayMode ePlayMode);
	void setAllRendered(bool bRendered);
	//void resetPlayMode();
	///<计算clip恢复属性
	void calculateClipResetModifier(const char* firstClipId,const char* secondClipId);
public://interface func
	StageAnimationPlayer(IStage* stage);
	virtual ~StageAnimationPlayer();

	//渲染
	virtual void render() ;
	//更新, 单位毫秒
	virtual void update(float t) ; 

	//设置每秒帧数
	virtual void setFramePerSecond(double frame);
	//设置每秒帧数,同时将历史调整记录下来，以便scale只影响设置的区域 lishan 111202
	virtual void setFramePerSecondAndRestorePastTime(double frame);
	//获取帧数
	virtual double getFramePerSecond() {return m_frame_per_second;};
	//设置entertime,单位毫秒
	virtual void setEnterTime(int enterTime) {m_entertime = enterTime;};
	//获得entertime,单位毫秒
	virtual int getEnterTime() {return m_entertime;};

	//获取当前相机
	virtual CameraEffectObject* getCurCamera();

	//播放关卡
	virtual void playLevel(bool play);
	///<记录clip对
	void recordClipResetModifier(const char* firstClipId,const char* secondClipId);
	
	//播放静态物体
	void playStaticObject(bool play);

private:
	//重构的播放函数，执行playLevel的内部操作
	void _do_play_level( bool play );
	void _loadEnvironmentLight();
	void _playStaticObject(bool play);
	void _setSystemInfo();

	bool parseActor(BiXmlNode* entityNode, int& entity_number, ActorAnimationClip* ani_clip, I_Keep_Client_Alive* pLp);
	bool parseLayer(BiXmlNode* layerNode, int&entity_number, ActorAnimationClip* ani_clip, I_Keep_Client_Alive* pLp);
	bool parseLod(BiXmlNode* node, SceneLodInfo& lod_obj_name);

public:
	// 获得舞台中静态物体actor
	DynamicEffectObject* getStaticActor() { return m_static_object; }

	StageAnimationCuttingData* getCurrentCuttingData();

	//切换静态物体LOD
	bool switchStaticActorLod(int lod);
	bool switchToLod(int lod);
protected:
	void removeLodSystemClip(LOD_LEVEL lod);
	void addLodSystemClip(int lod);
public:
	// 读取舞台信息（动态效果物体、舞台效果）
	virtual bool loadStageFile(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp = 0) ;
	// 保存舞台信息（动态效果物体、舞台效果）
	virtual bool saveStageFile(const char* stageFile, bool isBinary, bool isOptimize) ;

	// 读取剧本信息
	virtual bool loadAnimationFile(const char* levelFile, bool isBinary) ;
	// 保存剧本信息
	virtual bool saveAnimationFile(const char* levelFile, bool isBinary) ;

	//建立每个动态效果物体的缺陷舞台效果
	virtual bool loadDefaultStageClips(const char* stageFile, bool isBinary);

	/*
	*@breif 读取脚本文件并添加ID，插入到脚本列表里
	*@param levelFile 脚本路径
	*@param scriptID 脚本ID
	*@param isBinary 是否是2进制 
	*@return 是否操作成功
	*/
	virtual bool loadScriptFileToList(const char* levelFile, const char* scriptID, bool isBinary);

	// 读取静态效果物体,UNICODE码
	virtual bool loadStaticObject(const char* staticObjectFile, bool isBinary, I_Keep_Client_Alive* pLp = 0) ;
	//读取环境信息
	virtual bool loadEnvironmentFile(const char* env_file, bool isBinary);

	//载入场景中包含的舞台效果的列表
	virtual bool loadStageEffectListPath(const char* stageFile, bool isBinary);

	//载入单独的舞台效果
	virtual bool loadStageEffect(const char* stageEffect, bool isBinary);

	//播放动画
	virtual void playStageAnimationClip(const char* ani_name, bool play);
	//播放物体
	virtual void playDynamicEffectObject(DynamicEffectObject* obj, bool play);

	// 渲染锥光灯范围
	virtual void drawLightLine();
	//开启灯光效果
	void enableLightEffect(bool enable);

	// 获取剧本管理器
	virtual StageAnimationClipMng* getClipManger();
	// 获取关卡剧本
	virtual I_AnimationScript* getAnimationScript();

	// 读取编辑信息列表
	virtual bool loadEditInfoTable(const char* fileName, bool isBinary);
	// 获得编辑信息表
	//virtual IEditInfoTableEditor* getEditInfoTable();
	// 获得预定义项列表
	const CDefinedItemCollection* getDefinedItemCollection(){return m_defined_items;};
	// 设置目标管理器
	void setTargetPositionManager(ITargetPositionMng* targetposmng);

	//给demo做的后处理参数 lishan 110114
	const std::string& getLutTexture() const{return m_lut_texture;};

	//进行硬件选择
	unsigned int pushHardwareSelect(bool sel_line);
	//绘制表示灯光location的线
	void drawLightLocationLine();

	//设置循环播放
	void setLoop(bool loop);
	bool isLoop();

	///优化灯光设置
	//virtual void setLightOptimize(bool optimize = false);

	virtual void setColorFactor(float* color, int size);

	virtual void enableScene(bool enable);

	virtual float getCurrentTotalAccumulateTime();

	/*
	*@breif 根据id获取脚本指针
	*@param script_id
	*@return 获取的对象指针
	*/
	I_AnimationScript* getAnimationScriptByID(const char* script_id);

	/*
	*@breif 获取当前播放的脚本
	*@return 获取的对象指针
	*/
	I_AnimationScript* getCurPlayingScript() { return m_playing_script;}

	/*
	*@breif 设置当前播放脚本
	*@param script_id 要播放的脚本的id
	*/
	void playScriptByID(const char* script_id);

	/*
	*@breif 设置当前播放脚本
	*@param script_id 要保存的脚本的id
	*@param levelFile 要保存的脚本的文件名
	*@param isBinary 要保存的脚本是否二进制
	*/
	bool saveScriptByID(const char* script_id, const char* levelFile, bool isBinary) ;

	///*
	//*@breif 设置前后两个脚本的具有连接优化关系
	//*@param script_id 要保存的前脚本的id
	//*@param levelFile 要保存的下一个衔接脚本的id
	//*/
	//void setScriptConnection(const char* script_id, const char* next_script_id);

	///*
	//*@breif 按执行顺序，插入播放的脚本序列
	//*@param script_id 要插入的脚本的id
	//*/
	//void insertScriptSequence(const char* script_id);

	///*
	//*@breif 启动是否要序列播放舞台
	//*@param using_sequence 是否使用脚本序列
	//*/
	//void setUsingSequence(bool using_sequence);

	///*
	//*@breif 对已有的播放序列进行cache优化
	//*/
	//void cacheSequence();

	///*
	//*@breif 对播放序列进行前期工作 
	//*/
	//void doPreWorkForPlaySeq( bool play );

private:
	//画动态效果物体内锥光灯的线
	virtual void drawLightLine(DynamicEffectObject* eff_obj);
	//画舞台效果内锥光灯的线
	virtual void drawLightLine(StageAnimationClip* ani_clip);
	//重置关卡
	void resetLevel();
	//重置静态物体(静态物体不包含在关卡中)
	void resetStaticObject();
	//重置动画
	void resetStageAnimationClip();
	//重置物体
	void resetDynamicEffectObject();

	//更新关卡
	void updateLevel(float fInterval);

	//更新静态物体
	void updateStaticObject(float fInterval);

	//更新clip
	void updateStageAnimationClip(float fInterval);

	//更新物体
	void updateDynamicEffectObject(float fInterval);

	//更新所有物体
	virtual void updateAll(int t);

	//渲染关卡
	void renderLevel();

	//渲染静态物体
	void renderStaticObject();
	//渲染clip
	void renderStageAnimationClip();
	//渲染动态特效物体
	void renderDyanmicEffectObject();
	//渲染所有物体
	virtual void renderAll();

	//更新环境光设置
	virtual void updateSystemEnv();

	//是否需要重设系统灯光
	//virtual bool isNeedResetSysLight(const ActorManager::PrepassLightCollection& col);
	virtual void updateUsingLightCol();

	float m_static_actor_color;

	//保存脚本前后衔接关系的列表,分别是前后的id
	//typedef std::set<std::pair<std::string, std::string> > ScriptConnectionList;
	//ScriptConnectionList	m_script_connection_list;

	/*
	*@breif 检测前后两个脚本的是否具有连接优化关系
	*@param opt_pair 返回的优化方案结果id，用来给obj的control查询出对应的方案信息
	*@param script_id 要检测的前脚本的id
	*@param levelFile 要检测的下一个衔接脚本的id
	*@return 是否存在优化方案
	*/
	//bool _checkScriptConnection(std::pair<StageAnimationCuttingData*, StageAnimationCuttingData*>& opt_pair
	//							, const char* script_id, const char* next_script_id);

private:
	//void _do_play_sequence(bool play);

	//void _doScriptConnectOpt();

	//void _doCurrentScriptOpt();

	//void _checkObjConflict();

};


class StageAnimationPlayerEditor: public StageAnimationPlayer
{
public:
	StageAnimationPlayerEditor(IStage* stage);
	~StageAnimationPlayerEditor();

	void updateClip(float t, StageAnimationClip* ani_clip);
	virtual void update(float t);
	IStage* getStage();

};
#endif //_STAGE_ANIMATION_PLAYER_H_
