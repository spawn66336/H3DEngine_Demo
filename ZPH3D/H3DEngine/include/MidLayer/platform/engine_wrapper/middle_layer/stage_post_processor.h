/*/******************
/* @file	stage_post_processor.h
/* @brief	��̨�����ڴ������Ч�����µĹ�����
/* @author	������
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
	* @brief ���þ�������
	* @param t ����ľ�������
	*/
	virtual void configDOF(DOFType t);
	/**
	* @brief ���÷���������
	* @param t ����ķ���������
	*/
	virtual void configAA(AAType t);
	/**
	* @brief ��ʱȥ������
	* @param surpresse �Ƿ�ȥ������ǻָ�ԭ�еľ������ã���Ҫ�����ڹرճ���ʱҲͬʱ�رվ���
	*/
	virtual void surpresseDOF(bool surpresse);

	virtual void switchToLod(int lod);

	/**
	* @brief ���º���
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

	int m_lod; /// ������lod����Ӱ��Ժ��������

	DOFType m_dof_type;
	AAType m_aa_type;
	bool m_surpresse_dof; /// �Ƿ���ʱ�رվ���

	std::set<std::string> m_post_process_col; /// ����ʹ�õĺ���
	std::set<std::string> m_detained_post_process_col; /// û�и��º���ǰ���ⲿ���õĺ���
};
#endif