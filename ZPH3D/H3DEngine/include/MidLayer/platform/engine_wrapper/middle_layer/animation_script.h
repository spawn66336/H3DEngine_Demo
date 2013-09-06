/**
* @file animation_script.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author ����
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2010.11.16
*/

#ifndef _ANIMATION_SCRIPT_H_
#define _ANIMATION_SCRIPT_H_

#include "middle_layer_header.h"
class Stage;
// ����������Ϣ����
class AnimationScript:public I_AnimationScript
{
private:
	IStage* m_stage;
	// ��Ϣ�б�
	std::list<StageAnimationCuttingData*> m_info_list;
	// ��ǰ������λ��
	std::list<StageAnimationCuttingData*>::iterator m_info_pos;
	//ȱʡ��̨Ч��
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
	//�Բ����б�ʱ��������Ҫ���ڲ��ſ�ʼ��CLIP���ݽ��м���ʱ��
	virtual void sortCuttingData();
	// ���ö�����Ϣ������
	virtual void resetIterator() ;
	// ��ö�����Ϣ��������ָ����һ��������Ϣ�����������Ч����0 ����ʱ���������һ��̨Ч��������ֹͣ����ѡ�� lishan 111202
	virtual StageAnimationCuttingData* next(StageAnimationCuttingData** cuttingDataAfterThis = NULL) ;
	//ȡ��ǰ������Ϣ�����ڲ���ʱ������ʱ�Ƚ����ݰ�ʱ������Ȼ����в���
	virtual StageAnimationCuttingData* getCurrentCuttingData();

	// �����������͵Ķ���������Ϣ�����룬����ָ�룬������ʱ��������
	virtual void insertStageAnimationCuttingData(StageAnimationCuttingData* info) ;
	// ���ٸ����Ķ���������Ϣ,ȥ������������
	virtual void removeStageAnimationCuttingData(StageAnimationCuttingData* info) ;
	/*
	* @param
		info:�������Ķ�������
		beging_bar:�����Ŀ�ʼС��
		end_bar: �����Ľ���С��
	*/
	virtual void adjustStageAnimationCuttingData(StageAnimationCuttingData* info, float begin_bar, float end_bar);

	// ���岥��̨Ч���õģ�ֻ�����̨Ч�����ڲ�����ɾ��
	virtual StageAnimationCuttingData* addStageAnimation(StageAnimationCuttingDataType type,const char* id,float beginBar,float endBar);

	//����ȱʡ��̨Ч��,��δ����ƥ����̨Ч��ʱ����ȱʡ��̨Ч��
	virtual void setDefaultAnimation(const char* defaultAnimation){m_defaultAnimation = defaultAnimation;};
	//ȡȱʡ��̨Ч��
	virtual const char* getDefaultAnimation(){return m_defaultAnimation.c_str();};

	///*
	//*@breif ��ȡ��ǰ��һ��cuttingdata��ע�⣬δ����
	//*/
	//StageAnimationCuttingData* getFirstCuttingData();

	///*
	//*@breif ��ȡ��ǰ���һ��cuttingdata��ע�⣬δ����
	//*/
	//StageAnimationCuttingData* getLastCuttingData();
	
	bool load(const char* filename, bool isBinary);
	bool save(const char* filename, bool isBinary);
};
#endif //_ANIMATION_SCRIPT_H_
