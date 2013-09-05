// middle_layer_bridge.h

#pragma once

#include "../middle_layer/middle_layer_interface.h"
#include "i_target_position_impl.h"
#include "target_position_impl.h"

using namespace System;

namespace ClientBridge
{
	class StringItemEx
	{
	public:
		StringItemEx(System::String^ str)
		{
			if(str == nullptr)
			{
				m_wbuf = 0;
				m_con = 0;
				return;
			}

			int l = str->Length;
			m_wbuf = new wchar_t[l + 1];
			for(int i = 0; i < l; ++i)
				m_wbuf[i] = str[i];
			m_wbuf[l] = 0;

			int len = ::WideCharToMultiByte( CP_ACP, 0, m_wbuf, wcslen( m_wbuf ) + 1,	NULL, 0, NULL, NULL );
			m_con = new char[ len ];
			::WideCharToMultiByte( CP_ACP, 0, m_wbuf, wcslen( m_wbuf ) + 1, m_con, len, NULL, NULL );
		}

		~StringItemEx()
		{
			if(m_con)
				delete m_con;
			if(m_wbuf)
				delete []m_wbuf;
		}
		operator const char* ()
		{
			return m_con;
		}

	protected:
		wchar_t* m_wbuf;
		char* m_con;
	};


	public ref class LevelInfoItem
	{
	public:
		enum class TrackType
		{
			TrackTypeCamera,
			TrackTypeStageEffect,
			TrackTypeAction,
			TrackTypeRolePosition
		};

		// ��ʼ֡
		int m_begin_frame;
		// ����֡
		int m_end_frame;
		// ���ͣ�0�����1��̨Ч����2������3վλ
		TrackType m_track_type;
		// ��Ӧ����̨Ч������������֣����߶��������֣�����վλ��Ϣ
		System::String^ m_linked_id;
		// ��ǰ��Ϣ��Ӧ��Ψһ��ID�ַ���
		System::String^ m_uni_id;
	};

	public value class CuttingDataStorage
	{
	public:
		StageAnimationCuttingData* m_cutting_data;
	};

	public ref class LevelInfoMng
	{
	public:
		LevelInfoMng(AnimationScript* ani_script, StageAnimationClipMng* stage_ani_clip_mng);
		~LevelInfoMng();
		!LevelInfoMng();

		/// ��ȡ����ؿ���Ϣ����ȡʱ�����ؿ���Ϣ
		bool load(System::String^ filename, bool isBinary);
		bool save(System::String^ filename, bool isBinary);	

		/// ��ӹؿ���Ϣ�����Զ����õ�����
		// type��ʾ���ͣ�0�����1��̨Ч����2������3վλ, begframe��ʼ��֡λ��, length��֡�ĳ���
		LevelInfoItem^ addLevelInfo (System::String^ id, LevelInfoItem::TrackType type, int begframe, int length);
		/// ���¹ؿ���Ϣ
		void updateLevelInfo(LevelInfoItem^ item, int newbegframe, int newlength);
		/// ���¹ؿ���Ϣ,����linkid
		void updateLevelInfo(LevelInfoItem^ item, System::String^ newlinkid);
		/// ɾ���ؿ���Ϣ�����Զ����õ�����
		void removeInfoItem(LevelInfoItem^ item);

		// ���õ�����
		void resetIter();
		// �����һ���ؿ���Ϣ���������ĩβ������0
		LevelInfoItem^ getNextInfo();

	protected:
		// ���Ҹ���item��Ӧ�ļ�������
		StageAnimationCuttingData* findCuttingData(LevelInfoItem^ item);
		// ���Ҹ����������ݶ�Ӧ������
		LevelInfoItem::TrackType getTrackType(StageAnimationCuttingData* cuttingdata);
		// ���ݸ����ļ������ݽ���LevelInfoItem���������¼������������
		LevelInfoItem^ cuttingDataToLevelInfoItem(StageAnimationCuttingData* cuttingdata);


		AnimationScript* m_ani_script;
		StageAnimationClipMng* m_stage_ani_clip_mng;

		// �����л���������
		System::Collections::Generic::Dictionary<LevelInfoItem^, CuttingDataStorage>^ m_cutting_data_uniid_map;
	};
	
	public value class PropertyModifierTypeStorage
	{
	public:
		// �����޸����޸�����
		const PropertyModifierType* m_property_modifier_type;
	};


	/// ���Ƶ���Ϣ��
	public value class DefinedValueItem
	{
	public:
		System::String^ m_v_type;
		System::String^ m_value;
		System::String^ m_min_value;
		System::String^ m_max_value;
		System::String^ m_caption;
		bool m_is_support_curve; // �Ƿ�Ϊ����
		PropertyModifierTypeStorage m_property_modifier_type_storage;
	};

	public ref class DefinedItem
	{
	public:
		/// �����б�
		System::Collections::Generic::List<DefinedValueItem>^ m_property_list;
		/// ��Ԥ������
		System::Collections::Generic::List<DefinedItem^>^ m_sub_item;

		System::String^ m_id; // ��һ��ʶ
		System::String^ m_type; // ��������
		System::String^ m_defined_type; // Ԥ��������
		System::String^ m_name; // �ⲿ����������
	};

	// �����Ϣ
	public ref class CameraInfo
	{
	public:
		float pos_x, pos_y, pos_z;
		float dir_x, dir_y, dir_z;
		float up_x, up_y, up_z;
		float target_x, target_y, target_z;
		float fov, ratio, fNear, fFar;
		float focalplane, dof_range, confusion;	
	};
	public ref class H3DVec4 
	{
	public:	
		float			x;
		float			y;
		float			z;
		float			w;
	};
	/// ������Ϣ
	public ref class PostProcessParams
	{
	public:	
		typedef System::Collections::Generic::Dictionary<System::String^, H3DVec4^> PostParamCollection;
		typedef System::Collections::Generic::Dictionary<System::String^, PostParamCollection^> PostProcessCollection;

		PostProcessCollection^ m_post_process_lists;
	};

	/// ������ֵ��Ϣ
	public ref class AttributeSequenceEvaluator
	{
	public:
		enum class INTERPOLATING_TYPE
		{
			INTERPOLATING_POINT,
			INTERPOLATING_LINEAR,
			INTERPOLATING_BEZIER,
		};

		AttributeSequenceEvaluator(ICalculator* calculator);
		~AttributeSequenceEvaluator();
		!AttributeSequenceEvaluator();

		///��ȡ�ؼ�֡
		bool getKeyFrames(int sub_index, float frames[], int& buffer_size);

		///��ȡָ��֡�Ĳ�ֵ��ʽ
		bool getInterpolatingType(int sub_index, float frame, INTERPOLATING_TYPE& type);

		///��ȡָ��֡�Ĳ�ֵ
		System::String^ getInterpolatingValue(int sub_index, float frame);

		/// ��ȡ�ӵ�һ���ؼ�֡�����һ���ؼ�֮֡��Ĳ�ֵ(ÿ��0.1֡��ȡһ��)
		typedef System::Collections::Generic::Dictionary<System::String^, System::Collections::Generic::Dictionary<float, System::String^>^> CurvePoints;
		bool getInterpolatingValue(int sub_sequence_index, int length, CurvePoints^ curvePoints);

		ICalculator* m_calculator;
	};

	ref class MStageEffect;

	public ref class MGraphicEntity
	{
	public:
		MGraphicEntity(DynamicEffectObject* dyneffobj, StageAnimationClipMng* clip_mng, ActorManager* actormng);
		/// ������Ʒ����
		void setAttributeInfo(PropertyModifierTypeStorage type, System::String^ value, MStageEffect^ eff);
		/// ���þ�����Ʒ���ԣ����������Ϊ�������壬��λ�õ��趨֮��ĸ�Ϊ�������
		void setAbsAttributeInfo(PropertyModifierTypeStorage type, System::String^ value, MStageEffect^ eff);
		/// ��ȡ��Ʒ����
		System::String^ getAttributeInfo(PropertyModifierTypeStorage type, MStageEffect^ eff);
		/// ��ȡ������Ʒ��������, �������Ϊ�������壬�������λ�õ���Ϣ��Ϊ����λ��
		System::String^ getAbsAttributeInfo(PropertyModifierTypeStorage type, MStageEffect^ eff);

		/// �����������
		System::String^ getAttributeSeq(MStageEffect^ eff, PropertyModifierTypeStorage type);
		/// ������������
		void setAttributeSeq(MStageEffect^ eff, PropertyModifierTypeStorage type, System::String^ value);

		AttributeSequenceEvaluator^ getAttributeSeqEvaluator(MStageEffect^ eff,  PropertyModifierTypeStorage type);

		/// ���¼���
		void reload(System::String^ actorid);

		//�޸�����
		void setName(System::String^ id);
		//��ȡ����
		System::String^ getName();

		// ���õ�����
		void resetActorIter();
		// �����һ���ؿ���Ϣ���������ĩβ������0
		MGraphicEntity^ getNextActor();

		//��ȡ��Χ��
		void getAABB(System::Collections::Generic::List<float>^ aabblist);

		//ͨ��id��ȡ���������������
		System::String^ getChildActorNameById(unsigned int id);

		DynamicEffectObject* m_dyneffobj;
		StageAnimationClipMng* m_clip_mng;
		ActorManager* m_actor_manager;
	};

	public ref class MCamera : public MGraphicEntity
	{
	public:
		MCamera(DynamicEffectObject* dyneffobj, StageAnimationClipMng* clip_mng, ActorManager* actormng);

		void setFrameCount(int count);
		int getFrameCount();

		bool equal(MCamera^ target);
		void setID(System::String^ id);
		System::String^ getID();

		/// ��������Ϣ
		CameraInfo^ getCamInfo();
		PostProcessParams^ getCameraPostProcessParam();

		/// ������Ʒ����
		void setAttributeInfo(System::String^ type, System::String^ value);
		/// ��ȡ��Ʒ����
		System::String^ getAttributeInfo(System::String^ type);

		/// ��ָ��actor��ָ����̨Ч����ָ�����͵�������������Ϊ������
		void setAttributeSeqEnable(System::String^ type, bool is_enable);
		/// �ж�ָ��actor��ָ����̨Ч����ָ�����͵����������Ƿ����
		bool isAttributeSeqEnable(System::String^ type);

		void disableCameraPostProcess(System::String^ postprocess_name);
		/**
		* @brief
		*		���ú�������Ƿ����
		*/
		void setCameraPostProcessEnable(System::String^  post_process_name, bool enable);


		/// �����������
		System::String^ getAttributeSeq(System::String^ type);
		/// ������������
		void setAttributeSeq(System::String^ type, System::String^ value);

		AttributeSequenceEvaluator^ getAttributeSeqEvaluator(System::String^ type);

	protected:
		MStageEffect^ getRelStageEffect();
		PropertyModifierType getPropertyModifierType(System::String^ type, bool iscurv);
		PropertyModifierTypeStorage getPropertyModifierTypeStorage(PropertyModifierType* pmtype);

		IPropertyCurvModifierStreamable* getPropertyCurvModifierFromType(System::String^ type);
	};

	public ref class MPrePassLight
	{
	public:
		int getPtr(){ return (int)(m_light);};
		H3DI::IPrePassLight* m_light;
		float m_lightmap_shadow_index;
		float m_lightmap_radius;
		
	};
	public ref class MStageEffect
	{
	public:
		MStageEffect(StageAnimationClip* clip);
		int getFrameCount();
		void setFrameCount(int count);
		void setID(System::String^ id);
		System::String^ getID();

		StageAnimationClip* m_clip;
	};

	ref class StagePlayerMng;
	ref class MStageEffect;
	ref class MGraphicEntity;
	ref class MCamera;

	/// ��̨Ч����������ļ�����
	public ref class StageInfoMng
	{
	public:
		//StageInfoMng(Stage* stage, StageAnimationPlayer* ani_player);
		StageInfoMng(Stage* stage, StageAnimationPlayerEditor* ani_player);
		~StageInfoMng();
		!StageInfoMng();

		enum class LOD_LEVEL
		{
			LL_ZERO,
			LL_ONE,
			LL_TWO,
		};
		/// ת��LOD����
		bool switchToLod(int lod);

		System::Collections::Generic::List<MPrePassLight^>^ getAllLight();
		/// ��ȡԤ������Ϣ
		bool loadDefinedItem(System::String^ filename, bool isBinary);
		/// ��ȡ���ļ���������Ӧ������
		bool loadStage (System::String^ pathname, System::String^ filename, bool isBinary);
		/// �������ļ�
		bool saveStage (System::String^ pathname, System::String^ filename, bool isBinary);

		/// ��ȡ�ļ���������Ӧ������
		MStageEffect^ loadStageEffect(System::String^ filename, bool isBinary);
		MCamera^ loadCamera(System::String^ filename, bool isBinary);
		/// �����ļ������ڲ������ݱ�����ָ�����ļ���
		bool save(System::String^ filename, MCamera^ cam, bool isBinary);
		bool save(System::String^ filename, MStageEffect^ effect, bool isBinary);

		//ɾ����̨Ч���ļ��е�id��ָ��ֵ��ȵ�����ļ�¼
		//filename:��̨Ч�����ļ�·��
		//id:Ҫɾ���������id
		bool removeActorFromFile(System::String^ filename, System::String^ id);

		DefinedItem^ getDefinedInfo();
		// void setDefinedInfo(DefinedItem^ defineditem);

		void resetActorIter(); /// ���õ�����
		MGraphicEntity^ getNextActor(); /// �����һ��Ч�����壬�������ĩβ������0
		void resetStageEffectIter(); /// ���õ�����
		MStageEffect^ getNextStageEffect(); /// �����һ����̨Ч�����������ĩβ������0
		void resetCameraIter(); /// ���õ�����
		MCamera^ getNextCamera(); /// �����һ��������������ĩβ������0
		MCamera^ getCamera(System::String^  name);

		/// ���Ԥ���Ƶ�Ч����Ʒ
		MGraphicEntity^ createDefinedActor(System::String^  name, System::String^  definedtype, System::String^  info);
		/// �����߲��Ч����Ʒ
		void addActor(MGraphicEntity^ actor);
		/// ɾ����߲��Ч����Ʒ
		void removeActor(MGraphicEntity^ actor);

		/// �����̨Ч��
		MStageEffect^ addStageEffect(System::String^ name, int framecount, System::String^ type, System::String^ info);
		/// ɾ����̨Ч��
		void removeStageEffect(MStageEffect^ eff);

		/// ������
		MCamera^ addCamera (System::String^ name, System::String^ type, System::String^ info);
		/// ɾ�����
		void removeCamera(MCamera^ cam);

		/// ������Ʒ���˹�ϵ
		void link(MGraphicEntity^ parententity, MGraphicEntity^ subentity); 
		void unlink(MGraphicEntity^ parententity, MGraphicEntity^ entity);
		System::Collections::Generic::List<MGraphicEntity^>^ getLinkedEntities(MGraphicEntity^ parententity);

		//ͨ��id�ҵ���Ӧ��actor����
		System::String^ getActorNameById(unsigned int id);
		//�趨�Ƿ񱣴��ʱ������Ż�
		void setSaveOpimize(bool optimize);

	private:
		void addRemovedEntity(MGraphicEntity^ e);
		void removeRemovedEntity(MGraphicEntity^ e);
		bool isRemovedEntityExists(MGraphicEntity^ e);

		void updateLinkEntityAttribute(MGraphicEntity^ parent, MGraphicEntity^ entity, bool islink);

	protected:
		//IEditInfoTableEditor* m_editinfo_table; //�����ˣ�ע�͵� lishan 101215
		Stage* m_stage;
		//StageAnimationPlayer* m_ani_player;
		StageAnimationPlayerEditor* m_ani_player;
		System::Collections::Generic::List<MGraphicEntity^>^ m_removed_entities; //���Ƴ���entities
		bool m_save_optimize; /// ���б����Ż�
	};

	ref class ClientRenderSys;

	/// ��Ʒ���Ź�����
	public ref class GraphicEntityPlayerMng
	{
	public:
		//GraphicEntityPlayerMng(StageAnimationPlayer* player);
		GraphicEntityPlayerMng(StageAnimationPlayerEditor* player);
		~GraphicEntityPlayerMng();

		/// ����
		void playGraphicEntity(MGraphicEntity^ actor, bool play, MStageEffect^ eff);
		/// ���µ�ǰ���ŵ�����Ч������
		void update(float t);
		/// ����ָ����Ч������
		// void update(int t, MGraphicEntity^ actor);

	protected:
		//StageAnimationPlayer* m_player;
		StageAnimationPlayerEditor* m_player;
	};

	

	/// ���Ź�����
	public ref class StagePlayerMng
	{
	public:
		//StagePlayerMng(StageAnimationPlayer* player, StageAnimationClipMng* clip_mng, ActorManager* actormng);
		StagePlayerMng(StageAnimationPlayerEditor* player, StageAnimationClipMng* clip_mng, ActorManager* actormng);
		~StagePlayerMng();

		/// ����
		void playStageEffect(StageInfoMng^ stagemng, MStageEffect^ eff, bool play);
		void playCamera(StageInfoMng^ stagemng, MCamera^ cam, bool play);
		void playLevel(bool play, LevelInfoMng^ levelmng, StageInfoMng^ stagemng);
		/// ����
		void updateLevel(float t, LevelInfoMng^ levelmng, StageInfoMng^ stagemng); /// ����level
		void updateCamera(StageInfoMng^ stagemng, float t, MCamera^ cam); /// ���������ͷ
		void updateStageEffect(StageInfoMng^ stagemng, float t, MStageEffect^ eff); /// ������̨Ч��
		/// ��õ�ǰ��Ӧ�����
		MCamera^ getCurCam();
		//��Ⱦ׶���,��׶��Ƶķ�Χ���߻�����
		void drawWimblePrepasslightLine();
		//�����ƹ�Ч��
		void enableLightEffect(bool enable);
		/// ����ÿ֡�ĺ��볤��
		void setFrameMs(double ms);
		/// ���ÿ֡�ĺ��볤��
		double getFrameMs();

		//���Ʊ�ʾ�ƹ�location����
		void drawLightLocationLine();

		//
		//���������ӿ���Ϊ�˸�StageConvertor���ϵġ�
		//
		//���ػ�����Ϣ
		bool loadEnviromentFile(System::String^ env_file, bool is_binary);
		//���ؾ�̬Ч������
		bool loadStaticObject(System::String^ static_obj_file, bool is_binary);

		
	protected:
		//StageAnimationPlayer* m_player;
		StageAnimationPlayerEditor* m_player;
		StageAnimationClipMng* m_clip_mng;
		ActorManager* m_actormng;
	};

	public ref class StageMiddleLayer
	{
	public:
		StageMiddleLayer(void* ic, void* ilevel);
		~StageMiddleLayer();
		void setDefaultPath(System::String^ definedObjFilePath, System::String^ defaultEffectResPath, System::String^ defaultModelResPath, System::String^ defaultActorResPath, System::String^ lineShaderUIPath);

		void reset();

		GraphicEntityPlayerMng^ getGEntityPlayerMng()
		{
			return m_g_entity_player_mng;
		}
		StagePlayerMng^ getStagePlayerMng()
		{
			return m_stage_playermng;
		}
		StageInfoMng^ getStageInfoMng()
		{
			return m_stage_infomng;
		}
		LevelInfoMng^ getLevelInfoMng()
		{
			return m_levelinfo_mng;
		}
		TargetPositionMngImpl^ getTargetPositionMng()
		{
			return m_target_position_mng;
		}

		void update(float t);
		void render();
		unsigned int pushHardwareSelect(bool sel_line);

	protected:
		void create();
		void clear();

		GraphicEntityPlayerMng^ m_g_entity_player_mng;
		StagePlayerMng^ m_stage_playermng;
		StageInfoMng^ m_stage_infomng;
		LevelInfoMng^ m_levelinfo_mng;
		TargetPositionMngImpl^ m_target_position_mng;

		//StageAnimationPlayer* m_player;
		StageAnimationPlayerEditor* m_player;
		Stage* m_stage;

		H3DI::IRender* m_irender;
		H3DI::ILevel*	m_ilevel;

		System::String^  m_def_eff_res_path;
		System::String^ m_def_model_res_path;
		System::String^ m_def_actor_res_path;
		System::String^ m_line_shader_ui_path;
		System::String^ m_defined_obj_file_path;
	};

}
