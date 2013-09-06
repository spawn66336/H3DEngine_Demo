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

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "game_graphic_header.h"

// 编辑器接口，提供各种中间层编辑功能
class IMiddleLayerEditorInterface
{
public:
	// 读取舞台物品以及舞台效果
	virtual bool loadStage(const char* stagefile, bool binary) = 0;
	// 保存舞台物品以及舞台效果
	virtual bool saveStage(const char* stagefile, bool binary) = 0;
	// 读取动画脚本
	virtual bool loadAnimationScript(const char* scriptfile, bool binary) = 0;
	// 保持动画脚本
	virtual bool saveAnimationScript(const char* scriptfile, bool binary) = 0;
	// 播放
	virtual void play() = 0;
	virtual void play(StageAnimationClip* stageaniclip) = 0;
	virtual void play(DynamicEffectObject* obj) = 0;
	// 绘制
	virtual void render() = 0;
	// 更新
	virtual void update(int t) = 0;

	// 动态效果物体管理器
	virtual ActorManager* getDynEffObjMng() = 0;
	// 获得动画脚本
	virtual AnimationScript* getAnimationScript() = 0;
	// 舞台效果管理器
	virtual StageAnimationClipMng* getStageAnimationClipMng() = 0;
	// 获得编辑信息表
	virtual const IEditInfoTableEditor* getEditInfoTable() = 0;

	// 渲染锥光灯范围
	virtual void drawWimblePrepasslightLine();
	//应用后处理参数
	void applyPostProcessParam();
	//开启灯光效果
	void enableLightEffect(bool enable);
	//获取当前相机信息
	virtual CameraInfo getCurCameraInfo();
	//设置每秒帧数
	virtual void setFramePerSecond(int t);
	//获取每秒帧数
	virtual int getFramePerSecond();
	//获取舞台效果帧数
	virtual int getFrameCount(StageAnimationClip* clip);

	//
	virtual void setAttributeSeqEnable(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type , bool is_enable);
	virtual bool getAttributeSeqEnable(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type);
	///获取关键帧
	bool getKeyFrames(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type, float frames[], int& buffer_size);

	///获取指定帧的插值方式
	bool getInterpolatingType(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type, float frame, INTERPOLATING_TYPE& type);
	///获取指定帧的插值
	System::String^ getInterpolatingValue(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type, float frame);
};




#endif //_INTERFACE_H_
