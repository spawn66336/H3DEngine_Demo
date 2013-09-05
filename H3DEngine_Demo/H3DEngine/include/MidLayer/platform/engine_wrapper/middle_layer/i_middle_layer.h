/**
* @file i_middle_layer.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author ����
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2010.11.16
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
	StageAnimationCuttingData_Common,  //�洢 type="common"
	StageAnimationCuttingData_Camera,  //�洢 type="camera"
};

// ������̨Ч���Ķ���������Ϣ
struct StageAnimationCuttingData
{
	typedef std::string AnimationClipId;

	// ��Ӧ��̨Ч����ID
	AnimationClipId m_animation_clip_id;
	// �洢cuttingData����
	StageAnimationCuttingDataType m_data_type;
	// ��������ֹʱ��
	// int m_begin_frame, m_end_frame;
	double m_begin_bar,m_end_bar;

	//�Ƿ���ѭ�����ŵ���̨Ч��
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
	// ���ö�����Ϣ������
	virtual void resetIterator() = 0 ;
	// ��ö�����Ϣ��������ָ����һ��������Ϣ�����������Ч����0,����ʱ���������һ��̨Ч��������ֹͣ����ѡ�� lishan 111202
	virtual StageAnimationCuttingData* next(StageAnimationCuttingData** cuttingDataAfterThis = NULL) = 0 ;
	//ȡ��ǰ������Ϣ�����ڲ���ʱ������ʱ�Ƚ����ݰ�ʱ������Ȼ����в���
	virtual StageAnimationCuttingData* getCurrentCuttingData() = 0;
	//�Բ����б�ʱ��������Ҫ���ڲ��ſ�ʼ��CLIP���ݽ��м���ʱ��
	virtual void sortCuttingData() = 0;
	// ���岥��̨Ч���õģ�ֻ�����̨Ч�����ڲ�����ɾ��
	virtual StageAnimationCuttingData* addStageAnimation(StageAnimationCuttingDataType type,const char* id,float beginBar,float endBar) = 0;

	virtual void adjustStageAnimationCuttingData(StageAnimationCuttingData* info, float beging_bar, float end_bar) = 0;
	//����ȱʡ��̨Ч��,��δ����ƥ����̨Ч��ʱ����ȱʡ��̨Ч��
	virtual void setDefaultAnimation(const char* defaultAnimation) = 0;
	//ȡȱʡ��̨Ч��
	virtual const char* getDefaultAnimation() = 0;
	// ���
	virtual void clear() = 0;
};

class IStageAnimationPlayer
{
public:
	//��Ⱦ
	virtual void render() = 0;
	//����, ��λ����
	virtual void update(float t) = 0;

	//����ÿ��֡��
	virtual void setFramePerSecond(double frame) = 0;
	//����ÿ��֡��,ͬʱ����ʷ������¼�������Ա�scaleֻӰ�����õ����� lishan 111202
	virtual void setFramePerSecondAndRestorePastTime(double frame) = 0;
	//��ȡ֡��
	virtual double getFramePerSecond() = 0;

	//����entertime,��λ����
	virtual void setEnterTime(int enterTime) = 0;
	//���entertime,��λ����
	virtual int getEnterTime() = 0;

	// ��ȡ�ؿ��籾
	virtual I_AnimationScript* getAnimationScript() = 0;
	//���Źؿ�
	virtual void playLevel(bool play) = 0;

	// ���þ�̬���������Ч��
	virtual void setColorFactor(float* color, int size) = 0;

	//�л����� ����Ӱ������ĵƹ⣬�����Ľ����л��Ƿ�ɼ�
	virtual void enableScene(bool enable) = 0;

	//��ùؿ���ǰ�ܵĲ���ʱ��
	virtual float getCurrentTotalAccumulateTime() = 0;
};

///X52�ͻ�����ʱʹ��
class I_Keep_Client_Alive
{
public:
	I_Keep_Client_Alive(){};
	virtual ~I_Keep_Client_Alive(){};
public:
	virtual void update() = 0;
};

/// ��������
enum DOFType
{
	DOF_HIGH, /// �߾���
	DOF_LOW, /// �;���
	DOF_NO, /// �޾���
};

/// ����������
enum AAType
{
	AA_HIGH, /// �߷�����
	AA_LOW, /// �ͷ�����
	AA_NO, /// �޷�����
};

class I_Stage_PostProcessor
{
public:
	/**
	* @brief ���þ�������
	* @param t ����ľ�������
	*/
	virtual void configDOF(DOFType t) = 0;
	/**
	* @brief ���÷���������
	* @param t ����ķ���������
	*/
	virtual void configAA(AAType t) = 0;
	/**
	* @brief ��ʱȥ������
	* @param surpresse �Ƿ�ȥ������ǻָ�ԭ�еľ������ã���Ҫ�����ڹرճ���ʱҲͬʱ�رվ���
	*/
	virtual void surpresseDOF(bool surpresse) = 0;
};

class IStage
{
public:
	//��ö���������
	virtual IStageAnimationPlayer* getPlayer() = 0;
	// ��ȡ��̨��Ϣ����̬Ч�����塢��̨Ч����
	virtual bool loadStageFile(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp = 0) = 0;
	// ������̨��Ϣ����̬Ч�����塢��̨Ч����
	virtual bool saveStageFile(const char* stageFile, bool isBinary, bool isOptimize) = 0;

	// ��ȡ�籾��Ϣ
	virtual bool loadAnimationFile(const char* levelFile, bool isBinary) = 0;
	// ����籾��Ϣ
	virtual bool saveAnimationFile(const char* levelFile, bool isBinary) = 0;
	//��ȡActorMng
	virtual ActorManager* getActorMng() = 0;
	//��̬������弰��̨Ч��
	virtual bool addDynamicObjectAndEffect(const char* objectName,const char* type,const char* effectPath, bool isBinary = false) = 0;
	//����idȡ����ļ�
	virtual const char* getCameraFile(const char* cameraName) = 0;
	//�������л���lod
	virtual bool switchToLod(int lod) = 0;
	//��õ�ǰ������LOD����
	virtual LOD_LEVEL getLod()const = 0;
	/**
	* @brief ��ȡ��ǰ��̨�ĺ��������
	*/
	virtual I_Stage_PostProcessor* getPostProcessor() = 0;

	/**
	* @brief ���º���Ч��
	*/
	virtual void updatePostProcess(CCameraEntity* camera, bool particalviewport = false) = 0;
	virtual void updatePostProcess(const CCameraEntity::PostprocessCollection&, bool particalviewport = false) = 0;
};
struct PropertyModifierType;

typedef std::string DynEffObjType;

// �����޸����ı༭��Ϣ
struct PropertyModifierEditInfo
{
	//��������
	std::string m_datatype;
	//��;
	std::string m_usage;
	// ����Ĭ��ֵ
	std::string m_default;
	//���ֵ
	std::string m_max;
	//��Сֵ
	std::string m_min;
};
// �༭����Ϣ��
class IEditInfoTableEditor
{
public:
	virtual bool load(const char* filename, bool isBinary) = 0;
	virtual bool save(const char* filename, bool isBinary) = 0;

	// ��õ�ǰ����֧�ֵĶ�̬Ч������
	virtual const std::list<DynEffObjType>& findSupportedDynEffObjList() const = 0;
	// ���ö�ӦDynEffObjType�������޸ĵ�����
	virtual void resetSupportedPropertyModifierTypeInterator(const DynEffObjType& objtype) = 0;
	//  ���ָ����̬Ч����������֧�ֵ������޸������ͣ�������ָ����һ�������޸������ͣ������������Ч����0.ͬʱ����ȱʡ���Խṹ��ָ��
	virtual const PropertyModifierType* nextSupportedPropertyModifierType(PropertyModifierEditInfo* &) = 0;
	// ���ָ�������޸������Ͷ�Ӧ�ı༭��Ϣ������Ҳ�������0
	virtual const PropertyModifierEditInfo* findPropertyModifierEditInfo(const DynEffObjType& objtype,const PropertyModifierType& modifiertype) const = 0;
};


#endif //_I_MIDDLE_LAYER_H_
