/*/******************
/* @file	stage_post_processor.h
/* @brief	舞台中用于处理后处理效果更新的管理类
/* @author	张逸梵
/* @date	2012/03/30
/* @note
******************/
#ifndef __STAGE_POST_PROCESSOR_H__
#define __STAGE_POST_PROCESSOR_H__
#include "middle_layer_header.h"
#include "i_middle_layer.h"

class Stage;
class StageAnimationPlayer;
class StagePostProcessor : public I_Stage_PostProcessor
{
public:
	StagePostProcessor(Stage* pStage);
	virtual ~StagePostProcessor();
public:
	/**
	* @brief 配置景深类型
	* @param t 具体的景深类型
	*/
	virtual void configDOF(DOFType t);
	/**
	* @brief 配置反走样类型
	* @param t 具体的反走样类型
	*/
	virtual void configAA(AAType t);
	/**
	* @brief 暂时去掉景深
	* @param surpresse 是否去掉景深还是恢复原有的景深设置，主要供局内关闭场景时也同时关闭景深
	*/
	virtual void surpresseDOF(bool surpresse);

	virtual void switchToLod(int lod);

	/**
	* @brief 更新后处理
	*/
	void updatePostProcess(CCameraEntity* camera, StageAnimationPlayer* aniplayer, bool particalviewport = false);
	void updatePostProcess(const CCameraEntity::PostprocessCollection& process_collection, StageAnimationPlayer* aniplayer, bool particalviewport = false);
protected:
	void innerRemovePostProcess(const char* postprocessname);

	virtual void addPostProcess(bool particalviewport, const char* postprocessname);
	virtual void removeAllPostProcess();
	virtual void setPostProcessParams(const char* postprocess, const void * ptr, int size = 0);
	virtual bool canSetPostProcessPara(const char* postprocessname);
	virtual void updatePostProcess();
protected:
	Stage*	m_pStage;

	int m_lod; /// 场景的lod，会影响对后处理的配置

	DOFType m_dof_type;
	AAType m_aa_type;
	bool m_surpresse_dof; /// 是否暂时关闭景深

	std::set<std::string> m_post_process_col; /// 正在使用的后处理
	std::set<std::string> m_detained_post_process_col; /// 没有更新后处理前，外部设置的后处理
};
#endif