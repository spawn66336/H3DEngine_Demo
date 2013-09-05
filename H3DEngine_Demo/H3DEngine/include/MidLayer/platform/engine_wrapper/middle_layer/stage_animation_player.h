/**
* @file stage_animation_player.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author ����
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by ����2010.11.16
*/

#ifndef _STAGE_ANIMATION_PLAYER_H_
#define _STAGE_ANIMATION_PLAYER_H_

#include "middle_layer_header.h"

class DynamicEffectObject;
class StageAnimationClipMng;
class AnimationScript;
class StageAnimationClip;
struct StageAnimationCuttingData;
//class CEditInfoTable;
class CDefinedItemCollection;
class CameraEffectObject;
class ITargetPositionMng;
struct SceneLodInfo;

#define STD_FRAME_PER_SECOND (33.333333f)
#define STD_SECONDS_PER_BAR (2.4f)
#define STD_FRAME_PER_BAR  (STD_FRAME_PER_SECOND * STD_SECONDS_PER_BAR)

//���ڶ�ȡ�⻷���Ե���
class HaloConfig
{
public:
	//�⻷����
	H3DI::tLensEnv m_lensEnv;
	H3DI::tLensFlareParameters* m_lensFalres;
	H3DVec3 m_sunPos;
	std::vector<std::string> m_lensMatVec;
	std::vector<std::string> m_lensTypeVec;
	int m_lensFlaresNums;
	//�Ƿ��ȡ�ɹ�
	bool m_load_succ;
public:
	HaloConfig();
	~HaloConfig();
	//��ȡ�⻷����
	bool loadLensValue(BiXmlNode* node);
};
class StageAnimationPlayerEditor;
class StageAnimationPlayer : public IStageAnimationPlayer
{
	friend StageAnimationPlayerEditor;
private:
	enum PlayMode{
		PLAY_STOP	= 0x0,
		PLAY_LEVEL	= 0x01,
		PLAY_CLIP	= 0x01<<1,
		PLAY_OBJECT = 0x01<<2
	};

	int	m_level_lod;
	//����ģʽ
	int m_play_mode;
	//��̨
	IStage* m_stage;
	//����������
	StageAnimationClipMng* m_ani_clip_mng;
	//�ű�������
	AnimationScript* m_ani_script;
	//ÿ��֡��,�ⲿ����
	double m_frame_per_second;
	//ʱ�����ű���
	double m_time_scale;
	//�����ۼ�ʱ��
	float m_level_playing_acct;
	// ���ڲ��ŵĹؿ���Ϣ
	std::set<StageAnimationCuttingData*> m_playing_data_info;
	//���ڲ��ŵ�clip
	std::set<StageAnimationClip*> m_playing_clip_set;
	//���ڲ��ŵ�DynamicEffectObject
	std::set<DynamicEffectObject*> m_playing_object_set;
	//Ԥ����������Ϣ
	//CEditInfoTable* m_edit_info_table;
	//����ε�������Ϣ
	CDefinedItemCollection* m_defined_items;

	//��ǰ�����
	CameraEffectObject*	 m_cur_camera;
	StageAnimationClip*	 m_cur_camera_clip;
	//��̬���岥��
	DynamicEffectObject* m_static_object;
	std::map<std::string,std::pair<std::string,int> > static_property_map;
	std::set<std::string> point_light_set;
	std::set<std::string> direction_light_set;
	std::set<std::string> wimble_light_set;
	bool m_static_object_loaded;

	// Ŀ��λ�û�ȡ������
	ITargetPositionMng* m_targetposmng;
	//entertime,����
	int m_entertime;
	//�α����ɢ����ʾ�������ļ���ȡ��
	bool m_env_loaded; //�����������Ƿ������
	bool m_stage_loaded; //���������Ƿ������

	//��Ӱ��Χ����
	float m_shadow_scope;
	
	float m_ambient_actor_color[4];
	float m_ambient_color[4];
	float m_trans_color[4];
	//��demo���ĵ�����ȡ������� lishan 110114
	std::string m_lut_texture;
	//�⻷Ч������  lishan 110114
	HaloConfig m_halo_config;

	bool m_loop; //�Ƿ����ѭ������

	//ActorManager::PrepassLightCollection m_light_status_col;
	//bool		  m_optimize_light;
	std::set<H3DI::IPrePassLight*> m_using_light_col;
	
	/// ����ʹ�õ�����ƹ�
	std::map<H3DI::IPrePassLight*, const LightCachedProperty*> m_using_actor_light_col;


	bool m_enableScene;//�Ƿ���ʾ����

	///< �����ű��б�, ������ �ű�id�ַ����� �ű�����ָ��
	std::map<std::string, AnimationScript*> m_ani_script_list;
	///< ��ǰ���ŵĶ����ű�
	AnimationScript* m_playing_script;

	///< �ű��������У�������ʾ��˳�򲥷ŵ�һ�����ű�
	//typedef std::vector<AnimationScript*> ScriptSequence;
	//ScriptSequence	m_script_sequence;

	///< ����Ƿ�ͨ������������������̨
	//bool	m_play_by_script_sequence;

	///< ����ڲ������нű�ʱ���Ƿ��Ѿ���reset������״̬
	//bool    m_script_sequence_resetted;

	///<��ȥ�������õ�����scale
	std::list<std::pair<float,double> > m_pastScaleList;

	std::set<std::pair<std::string,std::string> > m_aniClipPair;//ȡ�����ӹ�ϵ��cliop
public:
	//����ؿ����������Ӳ����б����Ƴ�
	void ExcludeClipAndDynEffObj4Level();
	//������̨Ч������Ķ�̬��Ч����Ӳ����б����Ƴ�
	void ExcluedLevelAndDynEffObj4Clip(const char* ani_name);
	//���붯̬��Ч���廥�������Ӳ����б����Ƴ�
	void ExcludeLevelAndClip4DynEffObj(DynamicEffectObject* obj);
private:
	//�Ӳ����б����Ƴ��볡���������̨Ч��
	void stopAllStageAnimationClip();

	//�Ӳ����б����Ƴ��볡������Ķ�̬��Ч����
	void stopAllDynamicEffectObject();

	//////////////////////////////////////////
	//�жϹؿ������ڲ��ŵĶ����������Ƿ��ͻ//
	//////////////////////////////////////////
	bool isLevelAndClipConflicted();
	bool isLevelAndDynEffObjConflicted();
	bool isClipAndDynEffObjConflicted(const char* ani_name);
	bool isClipAndLevelConflicted(const char* ani_name);
	bool isDynEffObjAndLevelConflicted(DynamicEffectObject* obj);
	bool isDynEffObjAndClipConflicted(DynamicEffectObject* obj);


	void resetPlayMode(PlayMode ePlayMode);
	void setAllRendered(bool bRendered);
	//void resetPlayMode();
	///<����clip�ָ�����
	void calculateClipResetModifier(const char* firstClipId,const char* secondClipId);
public://interface func
	StageAnimationPlayer(IStage* stage);
	virtual ~StageAnimationPlayer();

	//��Ⱦ
	virtual void render() ;
	//����, ��λ����
	virtual void update(float t) ; 

	//����ÿ��֡��
	virtual void setFramePerSecond(double frame);
	//����ÿ��֡��,ͬʱ����ʷ������¼�������Ա�scaleֻӰ�����õ����� lishan 111202
	virtual void setFramePerSecondAndRestorePastTime(double frame);
	//��ȡ֡��
	virtual double getFramePerSecond() {return m_frame_per_second;};
	//����entertime,��λ����
	virtual void setEnterTime(int enterTime) {m_entertime = enterTime;};
	//���entertime,��λ����
	virtual int getEnterTime() {return m_entertime;};

	//��ȡ��ǰ���
	virtual CameraEffectObject* getCurCamera();

	//���Źؿ�
	virtual void playLevel(bool play);
	///<��¼clip��
	void recordClipResetModifier(const char* firstClipId,const char* secondClipId);
	
	//���ž�̬����
	void playStaticObject(bool play);

private:
	//�ع��Ĳ��ź�����ִ��playLevel���ڲ�����
	void _do_play_level( bool play );
	void _loadEnvironmentLight();
	void _playStaticObject(bool play);
	void _setSystemInfo();

	bool parseActor(BiXmlNode* entityNode, int& entity_number, ActorAnimationClip* ani_clip, I_Keep_Client_Alive* pLp);
	bool parseLayer(BiXmlNode* layerNode, int&entity_number, ActorAnimationClip* ani_clip, I_Keep_Client_Alive* pLp);
	bool parseLod(BiXmlNode* node, SceneLodInfo& lod_obj_name);

public:
	// �����̨�о�̬����actor
	DynamicEffectObject* getStaticActor() { return m_static_object; }

	StageAnimationCuttingData* getCurrentCuttingData();

	//�л���̬����LOD
	bool switchStaticActorLod(int lod);
	bool switchToLod(int lod);
protected:
	void removeLodSystemClip(LOD_LEVEL lod);
	void addLodSystemClip(int lod);
public:
	// ��ȡ��̨��Ϣ����̬Ч�����塢��̨Ч����
	virtual bool loadStageFile(const char* stageFile, bool isBinary, I_Keep_Client_Alive* pLp = 0) ;
	// ������̨��Ϣ����̬Ч�����塢��̨Ч����
	virtual bool saveStageFile(const char* stageFile, bool isBinary, bool isOptimize) ;

	// ��ȡ�籾��Ϣ
	virtual bool loadAnimationFile(const char* levelFile, bool isBinary) ;
	// ����籾��Ϣ
	virtual bool saveAnimationFile(const char* levelFile, bool isBinary) ;

	//����ÿ����̬Ч�������ȱ����̨Ч��
	virtual bool loadDefaultStageClips(const char* stageFile, bool isBinary);

	/*
	*@breif ��ȡ�ű��ļ������ID�����뵽�ű��б���
	*@param levelFile �ű�·��
	*@param scriptID �ű�ID
	*@param isBinary �Ƿ���2���� 
	*@return �Ƿ�����ɹ�
	*/
	virtual bool loadScriptFileToList(const char* levelFile, const char* scriptID, bool isBinary);

	// ��ȡ��̬Ч������,UNICODE��
	virtual bool loadStaticObject(const char* staticObjectFile, bool isBinary, I_Keep_Client_Alive* pLp = 0) ;
	//��ȡ������Ϣ
	virtual bool loadEnvironmentFile(const char* env_file, bool isBinary);

	//���볡���а�������̨Ч�����б�
	virtual bool loadStageEffectListPath(const char* stageFile, bool isBinary);

	//���뵥������̨Ч��
	virtual bool loadStageEffect(const char* stageEffect, bool isBinary);

	//���Ŷ���
	virtual void playStageAnimationClip(const char* ani_name, bool play);
	//��������
	virtual void playDynamicEffectObject(DynamicEffectObject* obj, bool play);

	// ��Ⱦ׶��Ʒ�Χ
	virtual void drawLightLine();
	//�����ƹ�Ч��
	void enableLightEffect(bool enable);

	// ��ȡ�籾������
	virtual StageAnimationClipMng* getClipManger();
	// ��ȡ�ؿ��籾
	virtual I_AnimationScript* getAnimationScript();

	// ��ȡ�༭��Ϣ�б�
	virtual bool loadEditInfoTable(const char* fileName, bool isBinary);
	// ��ñ༭��Ϣ��
	//virtual IEditInfoTableEditor* getEditInfoTable();
	// ���Ԥ�������б�
	const CDefinedItemCollection* getDefinedItemCollection(){return m_defined_items;};
	// ����Ŀ�������
	void setTargetPositionManager(ITargetPositionMng* targetposmng);

	//��demo���ĺ������ lishan 110114
	const std::string& getLutTexture() const{return m_lut_texture;};

	//����Ӳ��ѡ��
	unsigned int pushHardwareSelect(bool sel_line);
	//���Ʊ�ʾ�ƹ�location����
	void drawLightLocationLine();

	//����ѭ������
	void setLoop(bool loop);
	bool isLoop();

	///�Ż��ƹ�����
	//virtual void setLightOptimize(bool optimize = false);

	virtual void setColorFactor(float* color, int size);

	virtual void enableScene(bool enable);

	virtual float getCurrentTotalAccumulateTime();

	/*
	*@breif ����id��ȡ�ű�ָ��
	*@param script_id
	*@return ��ȡ�Ķ���ָ��
	*/
	I_AnimationScript* getAnimationScriptByID(const char* script_id);

	/*
	*@breif ��ȡ��ǰ���ŵĽű�
	*@return ��ȡ�Ķ���ָ��
	*/
	I_AnimationScript* getCurPlayingScript() { return m_playing_script;}

	/*
	*@breif ���õ�ǰ���Žű�
	*@param script_id Ҫ���ŵĽű���id
	*/
	void playScriptByID(const char* script_id);

	/*
	*@breif ���õ�ǰ���Žű�
	*@param script_id Ҫ����Ľű���id
	*@param levelFile Ҫ����Ľű����ļ���
	*@param isBinary Ҫ����Ľű��Ƿ������
	*/
	bool saveScriptByID(const char* script_id, const char* levelFile, bool isBinary) ;

	///*
	//*@breif ����ǰ�������ű��ľ��������Ż���ϵ
	//*@param script_id Ҫ�����ǰ�ű���id
	//*@param levelFile Ҫ�������һ���νӽű���id
	//*/
	//void setScriptConnection(const char* script_id, const char* next_script_id);

	///*
	//*@breif ��ִ��˳�򣬲��벥�ŵĽű�����
	//*@param script_id Ҫ����Ľű���id
	//*/
	//void insertScriptSequence(const char* script_id);

	///*
	//*@breif �����Ƿ�Ҫ���в�����̨
	//*@param using_sequence �Ƿ�ʹ�ýű�����
	//*/
	//void setUsingSequence(bool using_sequence);

	///*
	//*@breif �����еĲ������н���cache�Ż�
	//*/
	//void cacheSequence();

	///*
	//*@breif �Բ������н���ǰ�ڹ��� 
	//*/
	//void doPreWorkForPlaySeq( bool play );

private:
	//����̬Ч��������׶��Ƶ���
	virtual void drawLightLine(DynamicEffectObject* eff_obj);
	//����̨Ч����׶��Ƶ���
	virtual void drawLightLine(StageAnimationClip* ani_clip);
	//���ùؿ�
	void resetLevel();
	//���þ�̬����(��̬���岻�����ڹؿ���)
	void resetStaticObject();
	//���ö���
	void resetStageAnimationClip();
	//��������
	void resetDynamicEffectObject();

	//���¹ؿ�
	void updateLevel(float fInterval);

	//���¾�̬����
	void updateStaticObject(float fInterval);

	//����clip
	void updateStageAnimationClip(float fInterval);

	//��������
	void updateDynamicEffectObject(float fInterval);

	//������������
	virtual void updateAll(int t);

	//��Ⱦ�ؿ�
	void renderLevel();

	//��Ⱦ��̬����
	void renderStaticObject();
	//��Ⱦclip
	void renderStageAnimationClip();
	//��Ⱦ��̬��Ч����
	void renderDyanmicEffectObject();
	//��Ⱦ��������
	virtual void renderAll();

	//���»���������
	virtual void updateSystemEnv();

	//�Ƿ���Ҫ����ϵͳ�ƹ�
	//virtual bool isNeedResetSysLight(const ActorManager::PrepassLightCollection& col);
	virtual void updateUsingLightCol();

	float m_static_actor_color;

	//����ű�ǰ���νӹ�ϵ���б�,�ֱ���ǰ���id
	//typedef std::set<std::pair<std::string, std::string> > ScriptConnectionList;
	//ScriptConnectionList	m_script_connection_list;

	/*
	*@breif ���ǰ�������ű����Ƿ���������Ż���ϵ
	*@param opt_pair ���ص��Ż��������id��������obj��control��ѯ����Ӧ�ķ�����Ϣ
	*@param script_id Ҫ����ǰ�ű���id
	*@param levelFile Ҫ������һ���νӽű���id
	*@return �Ƿ�����Ż�����
	*/
	//bool _checkScriptConnection(std::pair<StageAnimationCuttingData*, StageAnimationCuttingData*>& opt_pair
	//							, const char* script_id, const char* next_script_id);

private:
	//void _do_play_sequence(bool play);

	//void _doScriptConnectOpt();

	//void _doCurrentScriptOpt();

	//void _checkObjConflict();

};


class StageAnimationPlayerEditor: public StageAnimationPlayer
{
public:
	StageAnimationPlayerEditor(IStage* stage);
	~StageAnimationPlayerEditor();

	void updateClip(float t, StageAnimationClip* ani_clip);
	virtual void update(float t);
	IStage* getStage();

};
#endif //_STAGE_ANIMATION_PLAYER_H_
