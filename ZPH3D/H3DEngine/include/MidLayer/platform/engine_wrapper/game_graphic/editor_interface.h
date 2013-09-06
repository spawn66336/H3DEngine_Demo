/**
* @file Interface.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.11.15
*/

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "game_graphic_header.h"

// �༭���ӿڣ��ṩ�����м��༭����
class IMiddleLayerEditorInterface
{
public:
	// ��ȡ��̨��Ʒ�Լ���̨Ч��
	virtual bool loadStage(const char* stagefile, bool binary) = 0;
	// ������̨��Ʒ�Լ���̨Ч��
	virtual bool saveStage(const char* stagefile, bool binary) = 0;
	// ��ȡ�����ű�
	virtual bool loadAnimationScript(const char* scriptfile, bool binary) = 0;
	// ���ֶ����ű�
	virtual bool saveAnimationScript(const char* scriptfile, bool binary) = 0;
	// ����
	virtual void play() = 0;
	virtual void play(StageAnimationClip* stageaniclip) = 0;
	virtual void play(DynamicEffectObject* obj) = 0;
	// ����
	virtual void render() = 0;
	// ����
	virtual void update(int t) = 0;

	// ��̬Ч�����������
	virtual ActorManager* getDynEffObjMng() = 0;
	// ��ö����ű�
	virtual AnimationScript* getAnimationScript() = 0;
	// ��̨Ч��������
	virtual StageAnimationClipMng* getStageAnimationClipMng() = 0;
	// ��ñ༭��Ϣ��
	virtual const IEditInfoTableEditor* getEditInfoTable() = 0;

	// ��Ⱦ׶��Ʒ�Χ
	virtual void drawWimblePrepasslightLine();
	//Ӧ�ú������
	void applyPostProcessParam();
	//�����ƹ�Ч��
	void enableLightEffect(bool enable);
	//��ȡ��ǰ�����Ϣ
	virtual CameraInfo getCurCameraInfo();
	//����ÿ��֡��
	virtual void setFramePerSecond(int t);
	//��ȡÿ��֡��
	virtual int getFramePerSecond();
	//��ȡ��̨Ч��֡��
	virtual int getFrameCount(StageAnimationClip* clip);

	//
	virtual void setAttributeSeqEnable(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type , bool is_enable);
	virtual bool getAttributeSeqEnable(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type);
	///��ȡ�ؼ�֡
	bool getKeyFrames(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type, float frames[], int& buffer_size);

	///��ȡָ��֡�Ĳ�ֵ��ʽ
	bool getInterpolatingType(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type, float frame, INTERPOLATING_TYPE& type);
	///��ȡָ��֡�Ĳ�ֵ
	System::String^ getInterpolatingValue(DynamicEffectObject* actor, StageAnimationClip* clip,  PropertyModifierType type, float frame);
};




#endif //_INTERFACE_H_
