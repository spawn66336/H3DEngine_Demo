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
	//��ö���������
	virtual IStageAnimationPlayer* getPlayer() = 0;
	// ��ȡ��̨��Ϣ����̬Ч�����塢��̨Ч�����籾��
	virtual bool loadStageAndAnimation(const char* stagefile, const char* scriptfile, bool binary) = 0;
	// ������̨��Ϣ����̬Ч�����塢��̨Ч�����籾��
	virtual bool saveStageAndAnimation(const char* stage_file, const char* level_file, bool isBinary) = 0;
	//��ȡActorMng
	virtual ActorManager* getActorMng() = 0;
};

class Stage : public IStage
{
	// ��ȡ��̨��Ϣ����̬Ч�����塢��̨Ч�����籾��
	virtual bool loadStageAndAnimation(const char* stagefile, const char* scriptfile, bool binary) ;
	// ������̨��Ϣ����̬Ч�����塢��̨Ч�����籾��
	virtual bool saveStageAndAnimation(const char* stage_file, const char* level_file, bool isBinary) ;

	//��ö���������
	virtual IStageAnimationPlayer* getPlayer();
	//��ȡActorMng
	virtual ActorManager* getActorMng() ;
	//��ʼ��
	virtual bool initialize() ;
	//����
	virtual void reset() ;
	//��ʼ��
	virtual bool initialize();
	//����
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
	//��Ⱦ
	virtual void render() = 0;
	//����, ��λ����
	virtual void update(int t) = 0;
	//��ȡ��ǰ�����Ϣ
	virtual void getCurCameraInfo(CameraInfo& info) = 0;

	//����ÿ��֡��
	virtual void setFramePerSecond(int frame) = 0;
	//��ȡ֡��
	virtual int getFramePerSecond() = 0;

	//���Źؿ�
	virtual void playLevel(bool play);
	//Ӧ�ú������
	void applyPostProcessParam();
};

class StageAnimationPlayer : IStageAnimationPlayer
{
public:
	StageAnimationPlayer(IStage* stage);

	//���¶�̬Ч������
	virtual void update(float t, DynamicEffectObject* obj);

	//���Ŷ���
	virtual void playStageAnimationClip(const char* ani_name, bool play);
	//��������
	virtual void playDynamicEffectObject(DynamicEffectObject* obj);

	// ��Ⱦ׶��Ʒ�Χ
	virtual void drawWimblePrepasslightLine();
	//�����ƹ�Ч��
	void enableLightEffect(bool enable);


	virtual StageAnimationClipMng* getClipManger();
	//��ȡ
	virtual AnimationScript* getAnimationScript();

	// ��ȡ�籾������
	virtual StageAnimationClipMng* getClipManger();
	// ��ȡ�ؿ��籾
	virtual AnimationScript* getAnimationScript();
	// ��ñ༭��Ϣ��
	virtual const IEditInfoTableEditor* getEditInfoTable();
};

class AnimationScript
{
public:
	AnimationScript(Stage* stage);
	virtual void clear();
	bool load(const char* filename, bool isBinary);
	bool save(const char* filename, bool isBinary);

	// �����������͵Ķ���������Ϣ
	virtual void insertStageAnimationCuttingData(StageAnimationCuttingData* info) ;
	// ���ٸ����Ķ���������Ϣ
	virtual void removeStageAnimationCuttingData(StageAnimationCuttingData* info) ;

	// ���ö�����Ϣ������
	virtual void reset_iterator() = 0;
	// ��ö�����Ϣ��������ָ����һ��������Ϣ�����������Ч����0
	virtual StageAnimationCuttingData* next() = 0;
};

// ����������Ϣ����
enum StageAnimationCuttingDataType
{
	StageAnimationCuttingData_Common, 
};
// ������̨Ч���Ķ���������Ϣ
struct StageAnimationCuttingData
{
typedef std::string AnimationClipId;
//	enum {CUTTING_ID_MAX_LEN = 64};

	// ��Ӧ��̨Ч����ID
	AnimationClipId m_animation_clip_id;
	// ��������ֹʱ��
	int m_begin_time, m_end_time;
};

// ��̨Ч������
enum StageAnimationClipType
{
	StageAnimationClip_Common,
	StageAnimationClip_Camera, // ���
};

class StageAnimationClipMng
{
public:
	StageAnimationClipMng(IStage* stage);
	bool load(const char* filename, bool isBinary);
	bool save(const char* filename, bool isBinary);

	// �����������͵���̨Ч��
	virtual StageAnimationClip* createStageAnimationClip(StageAnimationClipType t, const char* clipid);
	// ���ٸ�������̨Ч��
	virtual void releaseStageAnimationClip(StageAnimationClip* stageaniclip);

	// ���һ����̨Ч��
	virtual void insert(StageAnimationClip* stageaniclip) = 0;
	// ɾ��һ����̨Ч��
	virtual void remove(StageAnimationClip* stageaniclip) = 0;
	//����ָ������̨Ч��
	virtual StageAnimationClip* find(const char* clipid);

	// ������̨Ч��������
	virtual void resetIterator() = 0;
	// �����̨Ч����������ָ����һ����̨Ч�������������Ч����0
	virtual StageAnimationClip* next() = 0;
};

class StageAnimationClip
{
public:
	StageAnimationClip(StageAnimationClipMng* stage);
	//����
	virtual void play(bool isPlay, float t);
	//��Ⱦ
	virtual void render();
	//����, ��λ����
	virtual void update(float t);

	//���Ч������
	virtual void insert(DynamicEffectObject* eff_obj);
	//�Ƴ�Ч������
	virtual void remove(DynamicEffectObject* eff_obj);
	//����
	virtual DynamicEffectObject* find_eff_obj(const char*);

	//��ȡ��̨Ч��id
	virtual const char* getID() const;

	//��ȡclip�ĳ���
	virtual int getFrameCount() const;
};

class DynamicEffectObject : public Actor
{
};

class DynEffObjControl : public ActorAnimationControl
{
		//�����������в�������
	virtual void setCurvModifierEnable(const char* ani_name,  PropertyModifierType type , bool is_enable);
	virtual bool getCurvModifierEnable(const char* ani_name, PropertyModifierType type);

	///��ȡ�ؼ�֡
	bool getKeyFrames(const char* ani_name, PropertyModifierType type, float frames[], int& buffer_size);

	///��ȡָ��֡�Ĳ�ֵ��ʽ
	bool getInterpolatingType(const char* ani_name, PropertyModifierType type, float frame, INTERPOLATING_TYPE& type);
	///��ȡָ��֡�Ĳ�ֵ
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
	char* child_actor_id; //AcotrId����ʽΪ"/Actor0Id/Actor1Id/Actor2Id"
	char* child_entity_id; //Actor�µ�EntityId ��ʽΪ"/EntityId" ���Ϊ�գ�������������Actor������
	char* property_name; //��������
	char* property_fixed_param;//�޸����ԣ���Ҫ������������ֵ,��ʽΪ"param1/param2/param3",����Ϊ��
};

typedef std::string DynEffObjType;

// �༭����Ϣ��
class IEditInfoTableEditor
{
public:
	// �����޸����ı༭��Ϣ
	struct PropertyModifierEditInfo
	{
		// �����޸�������
		char* m_name;
		// ����Ĭ��ֵ
		char* m_value;
		// �����޸�������
		PropertyModifierType m_type;
		// �Ƿ�Ϊһ��ʱ���ڳ����������Ե��޸���
		bool m_modify_for_a_while;
	};
	bool load(const char* filename, bool isBinary);
	bool save(const char* filename, bool isBinary);

	// ��õ�ǰ����֧�ֵĶ�̬Ч������
	virtual const std::list<DynEffObjType>& findSupportedDynEffObjList() const = 0;
	// ���ָ����̬Ч���������͵����ƣ�����Ҳ�������0
	virtual const char* findDynEffObjTypeName(DynEffObjType objtype) const = 0;

	// ���ö�ӦDynEffObjType�������޸ĵ�����
	virtual void resetSupportedPropertyModifierTypeInterator(DynEffObjType objtype) = 0;
	//  ���ָ����̬Ч����������֧�ֵ������޸������ͣ�������ָ����һ�������޸������ͣ������������Ч����0
	virtual PropertyModifierType nextSupportedPropertyModifierType(DynEffObjType objtype) = 0;
	// ���ָ�������޸������Ͷ�Ӧ�ı༭��Ϣ������Ҳ�������0
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

	//��ȡ��ǰ�����Ϣ
	virtual void getCameraInfo(CameraInfo& info);

	//��ȡ��ǰ���������Ϣ
	virtual void getCameraPostParam(ParamConllection& conllection);

public:
	typedef std::map<std::string, H3DVec4> ParamConllection;
};

#endif //_INTERFACE_H_
