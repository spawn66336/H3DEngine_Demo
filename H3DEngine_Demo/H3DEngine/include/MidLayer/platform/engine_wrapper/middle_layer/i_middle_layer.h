/**
* @file i_middle_layer.h
* 这个文件的主要功能是：
* 
* @author 李汕
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 李汕2010.11.16
*/

#ifndef _I_MIDDLE_LAYER_H_
#define _I_MIDDLE_LAYER_H_

#include "middle_layer_header.h"

class ActorManager;
class CameraInfo
{
public:
	float pos_x, pos_y, pos_z;
	float dir_x, dir_y, dir_z;
	float up_x, up_y, up_z;
	float target_x, target_y, target_z;
	float fov, ratio, fNear, fFar;
	float focalplane, dof_range, confusion;	
};
enum StageAnimationCuttingDataType
{
	StageAnimationCuttingData_Common,  //存储 type="common"
	StageAnimationCuttingData_Camera,  //存储 type="camera"
};

// 播放舞台效果的动画段落信息
struct StageAnimationCuttingData
{
	typedef std::string AnimationClipId;

	// 对应舞台效果的ID
	AnimationClipId m_animation_clip_id;
	// 存储cuttingData类型
	StageAnimationCuttingDataType m_data_type;
	// 动画的起止时间
	// int m_begin_frame, m_end_frame;
	double m_begin_bar,m_end_bar;

	//是否是循环播放的舞台效果
	bool   m_loop;

	StageAnimationCuttingData():m_data_type(StageAnimationCuttingData_Common)
								,m_begin_bar(0)
								,m_end_bar(0)
								,m_loop(false)
	{		
	}
	bool operator()(const StageAnimationCuttingData* data1,const StageAnimationCuttingData* data2){   
        return data1->m_begin_bar < data2->m_begin_bar;   
    }   
    bool operator < (const StageAnimationCuttingData& data1) const {   
        return m_begin_bar < data1.m_begin_bar;   
    }   
};
class I_AnimationScript
{
public:
	// 重置段落信息迭代器
	virtual void resetIterator() = 0 ;
	// 获得段落信息，迭代器指向下一个段落信息，如迭代器无效返回0,返回时间紧连的下一舞台效果，用于停止集合选择 lishan 111202
	virtual StageAnimationCuttingData* next(StageAnimationCuttingData** cuttingDataAfterThis = NULL) = 0 ;
	//取当前段落信息，用于播放时。播放时先将数据按时间排序，然后进行播放
	virtual StageAnimationCuttingData* getCurrentCuttingData() = 0;
	//对播放列表按时间排序。主要用在播放开始对CLIP数据进行计算时。
	virtual void sortCuttingData() = 0;
	// 给插播舞台效果用的，只添加舞台效果，内部最终删除
	virtual StageAnimationCuttingData* addStageAnimation(StageAnimationCuttingDataType type,const char* id,float beginBar,float endBar) = 0;

	virtual void adjustStageAnimationCuttingData(StageAnimationCuttingData* info, float beging_bar, float end_bar) = 0;
	//设置缺省舞台效果,在未发现匹配舞台效果时播放缺省舞台效果
	virtual void setDefaultAnimation(const char* defaultAnimation) = 0;
	//取缺省舞台效果
	virtual const char* getDefaultAnimation() = 0;
	// 清空
	virtual void clear() = 0;
};

class IStageAnimationPlayer
{
public:
	//渲染
	virtual void render() = 0;
	//更新, 单位毫秒
	virtual void update(float t) = 0;

	//设置每秒帧数
	virtual void setFramePerSecond(double frame) = 0;
	//设置每秒帧数,同时将历史调整记录下来，以便scale只影响设置的区域 lishan 111202
	virtual void setFramePerSecondAndRestorePastTime(double frame) = 0;
	//获取帧数
	virtual double getFramePerSecond() = 0;

	//设置entertime,单位毫秒
	virtual void setEnterTime(int enterTime) = 0;
	//获得entertime,单位毫秒
	virtual int getEnterTime() = 0;

	// 获取关卡剧本
	virtual I_AnimationScript* getAnimationScript() = 0;
	//播放关卡
	virtual void playLevel(bool play) = 0;

	// 设置静态物体的明暗效果
	virtual void setColorFactor(float* color, int size) = 0;

	//切换背景 除了影响人物的灯光，其他的进行切换是否可见
	virtual void enableScene(bool enable) = 0;

	//获得关卡当前总的播放时间
	virtual float getCurrentTotalAccumulateTime() = 0;
};

///X52客户端临时使用
class I_Keep_Client_Alive
{
public:
	I_Keep_Client_Alive(){};
	virtual ~I_Keep_Client_Alive(){};
public:
	virtual void update() = 0;
};

/// 景深类型
enum DOFType
{
	DOF_HIGH, /// 高景深
	DOF_LOW, /// 低景深
	DOF_NO, /// 无景深
};

/// 反走样类型
enum AAType
{
	AA_HIGH, /// 高反走样
	AA_LOW, /// 低反走样
	AA_NO, /// 无反走样
};

class I_Stage_PostProcessor
{
public:
	/**
	* @brief 配置景深类型
	* @param t 具体的景深类型
	*/
	virtual void configDOF(DOFType t) = 0;
	/**
	* @brief 配置反走样类型
	* @param t 具体的反走样类型
	*/
	virtual void configAA(AAType t) = 0;
	/**
	* @brief 暂时去掉景深
	* @param surpresse 是否去掉景深还是恢复原有的景深设置，主要供局内关闭场景时也同时关闭景深
	*/
	virtual void surpresseDOF(bool surpresse) = 0;
};

class IStage
{
public:
	//获得动画播放器
	virtual IStageAnimationPlayer* getPlayer() = 0;
	// 读取舞台信息（动态效果物体、舞台效果）
	virtual bool loadStageFile(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp = 0) = 0;
	// 保存舞台信息（动态效果物体、舞台效果）
	virtual bool saveStageFile(const char* stageFile, bool isBinary, bool isOptimize) = 0;

	// 读取剧本信息
	virtual bool loadAnimationFile(const char* levelFile, bool isBinary) = 0;
	// 保存剧本信息
	virtual bool saveAnimationFile(const char* levelFile, bool isBinary) = 0;
	//获取ActorMng
	virtual ActorManager* getActorMng() = 0;
	//动态添加物体及舞台效果
	virtual bool addDynamicObjectAndEffect(const char* objectName,const char* type,const char* effectPath, bool isBinary = false) = 0;
	//根据id取相机文件
	virtual const char* getCameraFile(const char* cameraName) = 0;
	//将场景切换至lod
	virtual bool switchToLod(int lod) = 0;
	//获得当前场景的LOD级别
	virtual LOD_LEVEL getLod()const = 0;
	/**
	* @brief 获取当前舞台的后处理管理器
	*/
	virtual I_Stage_PostProcessor* getPostProcessor() = 0;

	/**
	* @brief 更新后处理效果
	*/
	virtual void updatePostProcess(CCameraEntity* camera, bool particalviewport = false) = 0;
	virtual void updatePostProcess(const CCameraEntity::PostprocessCollection&, bool particalviewport = false) = 0;
};
struct PropertyModifierType;

typedef std::string DynEffObjType;

// 属性修改器的编辑信息
struct PropertyModifierEditInfo
{
	//数据类型
	std::string m_datatype;
	//用途
	std::string m_usage;
	// 属性默认值
	std::string m_default;
	//最大值
	std::string m_max;
	//最小值
	std::string m_min;
};
// 编辑器信息表
class IEditInfoTableEditor
{
public:
	virtual bool load(const char* filename, bool isBinary) = 0;
	virtual bool save(const char* filename, bool isBinary) = 0;

	// 获得当前所有支持的动态效果物体
	virtual const std::list<DynEffObjType>& findSupportedDynEffObjList() const = 0;
	// 重置对应DynEffObjType的属性修改迭代器
	virtual void resetSupportedPropertyModifierTypeInterator(const DynEffObjType& objtype) = 0;
	//  获得指定动态效果物体类型支持的属性修改器类型，迭代器指向下一个属性修改器类型，如果迭代器无效返回0.同时返回缺省属性结构的指针
	virtual const PropertyModifierType* nextSupportedPropertyModifierType(PropertyModifierEditInfo* &) = 0;
	// 获得指定属性修改器类型对应的编辑信息，如果找不到返回0
	virtual const PropertyModifierEditInfo* findPropertyModifierEditInfo(const DynEffObjType& objtype,const PropertyModifierType& modifiertype) const = 0;
};


#endif //_I_MIDDLE_LAYER_H_
