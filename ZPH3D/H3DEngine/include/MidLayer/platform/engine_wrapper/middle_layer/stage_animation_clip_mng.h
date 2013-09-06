/**
* @file stage_animation_clip_mng.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author ����
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2010.11.16
*/

#ifndef _STAGE_ANIMATION_CLIP_MNG_H_
#define _STAGE_ANIMATION_CLIP_MNG_H_

#include "middle_layer_header.h"

class StageAnimationClipMng;
class DynamicEffectObject;
class Stage;
class ActorAnimationClip;
class PropertyModifierFactory;
//class CStagePropertyTable;
class IPropertyModifier;
class CDefinedItemCollection;
class CStageDefinedItemCollection;
// ��̨Ч������
enum StageAnimationClipType
{
	StageAnimationClip_Common,
	StageAnimationClip_Camera, // ���
};
class StageAnimationClip
{
private:
	//����Ƭ�ι�����
	StageAnimationClipMng* m_ani_clip_mng;
	//����Ƭ��ID
	std::string m_id;
	//����Ƭ������
	StageAnimationClipType m_ani_clip_type;
	//֡��
	int m_frame_count;
	//�����б�
	std::list<DynamicEffectObject*> m_object_list;
	//���������
	std::list<DynamicEffectObject*>::const_iterator m_object_list_iter;

	//�Ƿ����ڲ���
	bool	m_is_playing;
	//�Ƿ���Ҫ����Ⱦ
	bool	m_is_visible;
	//�Ƿ���������Ϣ

	//��¼��ǰ���ŵ�cutting data,���ڸ�object control �еĸ�λmap������
	StageAnimationCuttingData* m_current_cutting_data;
	//��¼��ǰ���ŵĽ��ڵ���һ��cutting data,���ڸ�object control �еĸ�λmap������
	StageAnimationCuttingData* m_next_cutting_data;
	//�ܵĸ���ʱ��
	float	m_total_update_time;
	//��ʼС����
	float m_begin_bar;
private:
	//�����ڵ㣬��������Ч��
	bool createDynamicEffect(BiXmlNode* effectNode);
	//�ⲿ�����ڵ㣬�����ⲿ����aniClip
	bool externalCreateDynamicEffect(ActorManager* actorManager, DynamicEffectObject* effectObj, BiXmlNode* effectNode);
	//���ú����л���Ϣ
	void setPostProcessSwitchInfo(const char* filename);
	//������̨Ч��
	void playObj(DynamicEffectObject* obj, bool isPlay, float t);
	//�����clip��ϵͳ����Ķ�������
	bool saveClipStageSystemObjects(BiXmlNode* parentNode, bool consistentId ,
		const char* file_main_name, bool isOptimize=true);
public:
	StageAnimationClip(StageAnimationClipMng* clip_mng);
	virtual ~StageAnimationClip();

	StageAnimationClipMng* getClipMng() { return m_ani_clip_mng;}

	//�����ļ�
	bool load(const char* filename, bool isBinary);
	//�洢�ļ�
	bool save(const char* filename, bool isBinary, bool consistentId = false, bool isOptimize = true);

	bool externalLoad(ActorManager* actorManager, DynamicEffectObject* effectObj,const char* filename0, bool isBinary);

	static bool removeDynamicEffectObjectFromFile(const char* filename, const char* id);
	//�����ļ�,����clip id
	static std::string getFileClipId(const char* filename, bool isBinary);

	//�����ļ�,����clip type
	static std::string getFileClipType(const char* filename, bool isBinary);
	
	// ����������Ϣ������
	virtual void resetObjectIterator() ;
	// ���������Ϣ��������ָ����һ��������Ϣ�����������Ч����0
	virtual DynamicEffectObject* nextObject() ;

	//������̨Ч��id
	virtual const char* getID() const{return m_id.c_str();};
	//��ȡ��̨Ч��id
	virtual void setID(const char* id);
	//��ȡclip�ĳ���
	virtual int getFrameCount() const{return m_frame_count;};
	//����clip�ĳ���
	virtual void setFrameCount(int count){ m_frame_count = count;};
	//���ö�������
	StageAnimationClipType getClipType(){return m_ani_clip_type;};
	//ȡ��������
	void setClipType(StageAnimationClipType clip_type){m_ani_clip_type = clip_type;};

	//�ж��Ƿ񲥷�
	bool isPlaying();

	//����
	virtual void play(bool isPlay, float t);
	//��Ⱦ
	virtual void render();
	virtual void set_visible(bool visible);
	virtual bool is_visible();
	//����Ӳ��ѡ����Ⱦ
	virtual unsigned int pushHardwareSelect(bool sel_line);
	//����, ��λ����
	virtual void update(float t);

	//���Ч������
	virtual void insert(DynamicEffectObject* eff_obj);
	//�Ƴ�Ч������
	virtual void remove(DynamicEffectObject* eff_obj,bool remove_clip = true);
	//����
	virtual DynamicEffectObject* findEffectObject(const char*);
	
	//���õ�ǰcutting data
	void setCurrentCuttingData(StageAnimationCuttingData* cutting_data, StageAnimationCuttingData* next_cutting_data){m_current_cutting_data = cutting_data; m_next_cutting_data = next_cutting_data;};
	
	float getTotalUpdateTime();
	//������ʼС�ڣ�����ѭ������
	void setBeginBar(float beginBar){m_begin_bar = beginBar;};
	//ȡ��ʼС��
	float getBeginBar(){return m_begin_bar;};
};
class StageAnimationClipMng
{
private:
	//ȱʡclip���ͣ��ڲ����ڴ������̿���
	enum{DEFAULT_OBJECT_PROPERTY,DEFAULT_STAGE_PROPERTY};

	IStage* m_stage;

	//���ɵ�clip
	std::map<std::string,StageAnimationClip*> ani_clip_map;
	// ��ǰ������λ��
	std::map<std::string,StageAnimationClip*>::iterator m_clip_pos;
	//�������Ӧ��Ч���ļ�
	std::map<std::string,std::string> m_cameraPathMap;

	std::map<std::string, std::string> m_clips_path_list;

private:
	bool parseStageEffect(BiXmlNode* rootNode, bool isBinary, const std::string& working_dir, I_Keep_Client_Alive* pLp = 0);
	//����OBJECTȱʡֵclip
	bool createStagePropertyClip(const char* default_clip_name,const CDefinedItemCollection* defined_item_collection);
	//��ĳ��lod�����ϵͳ�������̨Ч��ɾ��
	void removeSystemActorClip(LOD_LEVEL ll, const char* clipid);
	//��ĳ��lod�����ϵͳ�������̨Ч��������
	void renameSystemActorClip(LOD_LEVEL ll, const char* clipid, const char* newname);
public:
	StageAnimationClipMng(IStage* stage);
	virtual ~StageAnimationClipMng();
	//�����ļ�
	bool load(const char* filename, bool isBinary, I_Keep_Client_Alive* pLp = 0);
	//�洢�ļ�
	bool save(const char* filename, bool isBinary, bool isOptimize);

	// �������ж�̬Ч������Ĭ�ϵ���̨���Լ�
	bool loadDefaultStageEffects(const char* filename, bool isBinary);

	// �����������̨Ч���б�·��
	bool loadEffectPathList(const char* filename, bool isBinary);

	// ����һ����������̨Ч��
	bool loadStageEffect(const char* filename, bool isBinary);

	//ȡstageָ��
	Stage* getStage();

	// �����������͵���̨Ч��
	virtual StageAnimationClip* createStageAnimationClip(StageAnimationClipType t, const char* clipid);
	// ���ٸ�������̨Ч��
	virtual void releaseStageAnimationClip(StageAnimationClip* stageaniclip);

	// ���һ����̨Ч��
	virtual void insert(StageAnimationClip* stageaniclip) ;
	// ɾ��һ����̨Ч��
	virtual void remove(StageAnimationClip* stageaniclip) ;
	// �޸���̨Ч������
	virtual void renameClip(StageAnimationClip* clip, const char* newname);
	//����ָ������̨Ч��
	virtual StageAnimationClip* find(const char* clipid);

	// ������̨Ч��������
	virtual void resetIterator() ;
	// �����̨Ч����������ָ����һ����̨Ч�������������Ч����0
	virtual StageAnimationClip* next() ;
	//��ȡstage��ȱʡ��������
	bool generateStagePropertyInfo(BiXmlNode* effect_object_node, DynamicEffectObject* effect_object );	
	//�ӹؿ��ļ����ȡ��̨Ч��
	virtual bool loadEffect(const char* fileName, bool isBinary);
	//����idȡ����ļ�
	const char* getCameraFile(const char* cameraName);

	void clear() {m_cameraPathMap.clear();}
#ifdef _EDITOR

	//���ɵ�clip
	std::map<std::string,StageAnimationClip*> m_common_camera_clips;
	//camera
	std::map<std::string,CameraEffectObject*> m_common_cameras;

	//�������Ӧ��Ч���ļ�
	std::map<std::string,std::string> m_common_camera_pathes;
	
	bool loadCommonCamera();
	/**
	@brief ����ͨ�þ�ͷ����
	*/
	bool parseCameraFile(const char* cameraFile);

	CameraEffectObject* getCommonCamera(const char* strCameraName);
	StageAnimationClip* getCommonCameraClip(const char* strClipName);

#endif
};


#endif //_STAGE_ANIMATION_CLIP_MNG_H_
//_STAGE_ANIMATION_CLIP_MNG_H_
