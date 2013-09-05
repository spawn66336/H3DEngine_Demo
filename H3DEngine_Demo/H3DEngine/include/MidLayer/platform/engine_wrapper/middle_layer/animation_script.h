/**
* @file animation_script.h
* 这个文件的主要功能是：
* 
* @author 李汕
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 李汕2010.11.16
*/

#ifndef _ANIMATION_SCRIPT_H_
#define _ANIMATION_SCRIPT_H_

#include "middle_layer_header.h"
class Stage;
// 动画段落信息类型
class AnimationScript:public I_AnimationScript
{
private:
	IStage* m_stage;
	// 信息列表
	std::list<StageAnimationCuttingData*> m_info_list;
	// 当前迭代器位置
	std::list<StageAnimationCuttingData*>::iterator m_info_pos;
	//缺省舞台效果
	std::string m_defaultAnimation;

	double	m_frame_per_second;
private:
	bool parseSpecifyNode(BiXmlNode* rootNode, int mTrackType);
public:
	AnimationScript(IStage* stage);
	virtual ~AnimationScript();
	virtual void clear();
	double getFramePerSecond() const { return m_frame_per_second;};
	void setFramePerSecond(double frame){m_frame_per_second = frame;} 
	//对播放列表按时间排序。主要用在播放开始对CLIP数据进行计算时。
	virtual void sortCuttingData();
	// 重置段落信息迭代器
	virtual void resetIterator() ;
	// 获得段落信息，迭代器指向下一个段落信息，如迭代器无效返回0 返回时间紧连的下一舞台效果，用于停止集合选择 lishan 111202
	virtual StageAnimationCuttingData* next(StageAnimationCuttingData** cuttingDataAfterThis = NULL) ;
	//取当前段落信息，用于播放时。播放时先将数据按时间排序，然后进行播放
	virtual StageAnimationCuttingData* getCurrentCuttingData();

	// 创建给定类型的动画段落信息，加入，保存指针，并析构时负责销毁
	virtual void insertStageAnimationCuttingData(StageAnimationCuttingData* info) ;
	// 销毁给定的动画段落信息,去除，并不销毁
	virtual void removeStageAnimationCuttingData(StageAnimationCuttingData* info) ;
	/*
	* @param
		info:待调整的动画段落
		beging_bar:期望的开始小节
		end_bar: 期望的结束小节
	*/
	virtual void adjustStageAnimationCuttingData(StageAnimationCuttingData* info, float begin_bar, float end_bar);

	// 给插播舞台效果用的，只添加舞台效果，内部最终删除
	virtual StageAnimationCuttingData* addStageAnimation(StageAnimationCuttingDataType type,const char* id,float beginBar,float endBar);

	//设置缺省舞台效果,在未发现匹配舞台效果时播放缺省舞台效果
	virtual void setDefaultAnimation(const char* defaultAnimation){m_defaultAnimation = defaultAnimation;};
	//取缺省舞台效果
	virtual const char* getDefaultAnimation(){return m_defaultAnimation.c_str();};

	///*
	//*@breif 获取当前第一个cuttingdata，注意，未排序
	//*/
	//StageAnimationCuttingData* getFirstCuttingData();

	///*
	//*@breif 获取当前最后一个cuttingdata，注意，未排序
	//*/
	//StageAnimationCuttingData* getLastCuttingData();
	
	bool load(const char* filename, bool isBinary);
	bool save(const char* filename, bool isBinary);
};
#endif //_ANIMATION_SCRIPT_H_
