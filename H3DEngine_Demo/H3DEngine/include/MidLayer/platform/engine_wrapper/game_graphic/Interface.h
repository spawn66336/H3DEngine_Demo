/**
* @file Interface.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.15
*/

#ifdef _INTERFACE_H_
//#define _INTERFACE_H_

#include "game_graphic_header.h"

class IStageAnimationPlayer;
class ActorManager;
class StageAnimationClipMng
class AnimationScript;
struct StageAnimationCuttingData;
class StageAnimationClip;
class Actor;
struct CameraInfo;

class IStage
{
public:
	//获得动画播放器
	virtual IStageAnimationPlayer* getPlayer() = 0;
	// 读取舞台信息（动态效果物体、舞台效果、剧本）
	virtual bool loadStageAndAnimation(const char* stagefile, const char* scriptfile, bool binary) = 0;
	// 保存舞台信息（动态效果物体、舞台效果、剧本）
	virtual bool saveStageAndAnimation(const char* stage_file, const char* level_file, bool isBinary) = 0;
	//获取ActorMng
	virtual ActorManager* getActorMng() = 0;
};

class Stage : public IStage
{
	// 读取舞台信息（动态效果物体、舞台效果、剧本）
	virtual bool loadStageAndAnimation(const char* stagefile, const char* scriptfile, bool binary) ;
	// 保存舞台信息（动态效果物体、舞台效果、剧本）
	virtual bool saveStageAndAnimation(const char* stage_file, const char* level_file, bool isBinary) ;

	//获得动画播放器
	virtual IStageAnimationPlayer* getPlayer();
	//获取ActorMng
	virtual ActorManager* getActorMng() ;
	//初始化
	virtual bool initialize() ;
	//重置
	virtual void reset() ;
	//初始化
	virtual bool initialize();
	//重置
	virtual void reset();
};

public class CameraInfo
{
public:
	float pos_x, pos_y, pos_z;
	float dir_x, dir_y, dir_z;
	float up_x, up_y, up_z;
	float target_x, target_y, target_z;
	float fov, ratio, fNear, fFar;
	float focalplane, dof_range, confusion;	
};

class IStageAnimationPlayer
{
public:
	//渲染
	virtual void render() = 0;
	//更新, 单位毫秒
	virtual void update(int t) = 0;
	//获取当前相机信息
	virtual void getCurCameraInfo(CameraInfo& info) = 0;

	//设置每秒帧数
	virtual void setFramePerSecond(int frame) = 0;
	//获取帧数
	virtual int getFramePerSecond() = 0;

	//播放关卡
	virtual void playLevel(bool play);
	//应用后处理参数
	void applyPostProcessParam();
};

class StageAnimationPlayer : IStageAnimationPlayer
{
public:
	StageAnimationPlayer(IStage* stage);

	//更新动态效果物体
	virtual void update(float t, DynamicEffectObject* obj);

	//播放动画
	virtual void playStageAnimationClip(const char* ani_name, bool play);
	//播放物体
	virtual void playDynamicEffectObject(DynamicEffectObject* obj);

	// 渲染锥光灯范围
	virtual void drawWimblePrepasslightLine();
	//开启灯光效果
	void enableLightEffect(bool enable);


	virtual StageAnimationClipMng* getClipManger();
	//获取
	virtual AnimationScript* getAnimationScript();

	// 获取剧本管理器
	virtual StageAnimationClipMng* getClipManger();
	// 获取关卡剧本
	virtual AnimationScript* getAnimationScript();
	// 获得编辑信息表
	virtual const IEditInfoTableEditor* getEditInfoTable();
};

class AnimationScript
{
public:
	AnimationScript(Stage* stage);
	virtual void clear();
	bool load(const char* filename, bool isBinary);
	bool save(const char* filename, bool isBinary);

	// 创建给定类型的动画段落信息
	virtual void insertStageAnimationCuttingData(StageAnimationCuttingData* info) ;
	// 销毁给定的动画段落信息
	virtual void removeStageAnimationCuttingData(StageAnimationCuttingData* info) ;

	// 重置段落信息迭代器
	virtual void reset_iterator() = 0;
	// 获得段落信息，迭代器指向下一个段落信息，如迭代器无效返回0
	virtual StageAnimationCuttingData* next() = 0;
};

// 动画段落信息类型
enum StageAnimationCuttingDataType
{
	StageAnimationCuttingData_Common, 
};
// 播放舞台效果的动画段落信息
struct StageAnimationCuttingData
{
typedef std::string AnimationClipId;
//	enum {CUTTING_ID_MAX_LEN = 64};

	// 对应舞台效果的ID
	AnimationClipId m_animation_clip_id;
	// 动画的起止时间
	int m_begin_time, m_end_time;
};

// 舞台效果类型
enum StageAnimationClipType
{
	StageAnimationClip_Common,
	StageAnimationClip_Camera, // 相机
};

class StageAnimationClipMng
{
public:
	StageAnimationClipMng(IStage* stage);
	bool load(const char* filename, bool isBinary);
	bool save(const char* filename, bool isBinary);

	// 创建给定类型的舞台效果
	virtual StageAnimationClip* createStageAnimationClip(StageAnimationClipType t, const char* clipid);
	// 销毁给定的舞台效果
	virtual void releaseStageAnimationClip(StageAnimationClip* stageaniclip);

	// 添加一个舞台效果
	virtual void insert(StageAnimationClip* stageaniclip) = 0;
	// 删除一个舞台效果
	virtual void remove(StageAnimationClip* stageaniclip) = 0;
	//查找指定的舞台效果
	virtual StageAnimationClip* find(const char* clipid);

	// 重置舞台效果迭代器
	virtual void resetIterator() = 0;
	// 获得舞台效果，迭代器指向下一个舞台效果，如迭代器无效返回0
	virtual StageAnimationClip* next() = 0;
};

class StageAnimationClip
{
public:
	StageAnimationClip(StageAnimationClipMng* stage);
	//播放
	virtual void play(bool isPlay, float t);
	//渲染
	virtual void render();
	//更新, 单位毫秒
	virtual void update(float t);

	//添加效果物体
	virtual void insert(DynamicEffectObject* eff_obj);
	//移除效果物体
	virtual void remove(DynamicEffectObject* eff_obj);
	//查找
	virtual DynamicEffectObject* find_eff_obj(const char*);

	//获取舞台效果id
	virtual const char* getID() const;

	//获取clip的长度
	virtual int getFrameCount() const;
};

class DynamicEffectObject : public Actor
{
};

class DynEffObjControl : public ActorAnimationControl
{
		//设置属性序列不起作用
	virtual void setCurvModifierEnable(const char* ani_name,  PropertyModifierType type , bool is_enable);
	virtual bool getCurvModifierEnable(const char* ani_name, PropertyModifierType type);

	///获取关键帧
	bool getKeyFrames(const char* ani_name, PropertyModifierType type, float frames[], int& buffer_size);

	///获取指定帧的插值方式
	bool getInterpolatingType(const char* ani_name, PropertyModifierType type, float frame, INTERPOLATING_TYPE& type);
	///获取指定帧的插值
	const char* getInterpolatingValue(const char* ani_name,  PropertyModifierType type, float frame);
	
	virtual DynEffPropertyModifierFactory* getPropertyModifierFactory();
};

class DynEffPropertyModifierFactory
{
	virtual IPropertyModifier* createCurvModifier(DynamicEffectObject* eff_obj, PropertyModifierType type, float frame);
	virtual void releaseCurvModifier(IPropertyModifier* modifier);
	virtual IPropertyModifier* createPropertyModifier(DynamicEffectObject* eff_obj, PropertyModifierType type, float frame);
	virtual void releasePropertyModifier(IPropertyModifier* modifier);
}

struct PropertyModifierType
{
	char* child_actor_id; //AcotrId，格式为"/Actor0Id/Actor1Id/Actor2Id"
	char* child_entity_id; //Actor下的EntityId 格式为"/EntityId" 如果为空，表明关联的是Actor的属性
	char* property_name; //属性名称
	char* property_fixed_param;//修改属性，需要的其他参数的值,格式为"param1/param2/param3",可以为空
};

typedef std::string DynEffObjType;

// 编辑器信息表
class IEditInfoTableEditor
{
public:
	// 属性修改器的编辑信息
	struct PropertyModifierEditInfo
	{
		// 属性修改器名称
		char* m_name;
		// 属性默认值
		char* m_value;
		// 属性修改器类型
		PropertyModifierType m_type;
		// 是否为一段时间内持续更新属性的修改器
		bool m_modify_for_a_while;
	};
	bool load(const char* filename, bool isBinary);
	bool save(const char* filename, bool isBinary);

	// 获得当前所有支持的动态效果物体
	virtual const std::list<DynEffObjType>& findSupportedDynEffObjList() const = 0;
	// 获得指定动态效果物体类型的名称，如果找不到返回0
	virtual const char* findDynEffObjTypeName(DynEffObjType objtype) const = 0;

	// 重置对应DynEffObjType的属性修改迭代器
	virtual void resetSupportedPropertyModifierTypeInterator(DynEffObjType objtype) = 0;
	//  获得指定动态效果物体类型支持的属性修改器类型，迭代器指向下一个属性修改器类型，如果迭代器无效返回0
	virtual PropertyModifierType nextSupportedPropertyModifierType(DynEffObjType objtype) = 0;
	// 获得指定属性修改器类型对应的编辑信息，如果找不到返回0
	virtual const PropertyModifierEditInfo* findPropertyModifierEditInfo(const PropertyModifierType& modifiertype) const = 0;
};

class DynEffObjManager : public ActorManager
{
public:
	DynEffObjManager(IEditInfoTableEditor* table);
	virtual DynamicEffectObject* createDynamicActor(const char* actor_name, const char* actor_type);
};

class CameraEffectObject: public DynamicEffectObject
{
private:
	std::string m_defined_type;
public:
	CameraEffectObject(ActorManager* actor_manager, const char* name);
	virtual ~CameraEffectObject();

	//获取当前相机信息
	virtual void getCameraInfo(CameraInfo& info);

	//获取当前相机后处理信息
	virtual void getCameraPostParam(ParamConllection& conllection);

public:
	typedef std::map<std::string, H3DVec4> ParamConllection;
};

#endif //_INTERFACE_H_
