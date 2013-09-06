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

		// 开始帧
		int m_begin_frame;
		// 结束帧
		int m_end_frame;
		// 类型：0相机、1舞台效果、2动作、3站位
		TrackType m_track_type;
		// 对应的舞台效果或相机的名字，或者动作的名字，或者站位信息
		System::String^ m_linked_id;
		// 当前信息对应的唯一的ID字符串
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

		/// 读取保存关卡信息，读取时创建关卡信息
		bool load(System::String^ filename, bool isBinary);
		bool save(System::String^ filename, bool isBinary);	

		/// 添加关卡信息，将自动重置迭代器
		// type表示类型：0相机、1舞台效果、2动作、3站位, begframe开始的帧位置, length总帧的长度
		LevelInfoItem^ addLevelInfo (System::String^ id, LevelInfoItem::TrackType type, int begframe, int length);
		/// 更新关卡信息
		void updateLevelInfo(LevelInfoItem^ item, int newbegframe, int newlength);
		/// 更新关卡信息,更新linkid
		void updateLevelInfo(LevelInfoItem^ item, System::String^ newlinkid);
		/// 删除关卡信息，将自动重置迭代器
		void removeInfoItem(LevelInfoItem^ item);

		// 重置迭代器
		void resetIter();
		// 获得下一个关卡信息，如果到了末尾，返回0
		LevelInfoItem^ getNextInfo();

	protected:
		// 查找给定item对应的剪辑数据
		StageAnimationCuttingData* findCuttingData(LevelInfoItem^ item);
		// 查找给定剪辑数据对应的类型
		LevelInfoItem::TrackType getTrackType(StageAnimationCuttingData* cuttingdata);
		// 根据给定的剪辑数据建立LevelInfoItem，不负责记录到数据容器中
		LevelInfoItem^ cuttingDataToLevelInfoItem(StageAnimationCuttingData* cuttingdata);


		AnimationScript* m_ani_script;
		StageAnimationClipMng* m_stage_ani_clip_mng;

		// 场景切换数据容器
		System::Collections::Generic::Dictionary<LevelInfoItem^, CuttingDataStorage>^ m_cutting_data_uniid_map;
	};
	
	public value class PropertyModifierTypeStorage
	{
	public:
		// 属性修改器修改类型
		const PropertyModifierType* m_property_modifier_type;
	};


	/// 定制的信息项
	public value class DefinedValueItem
	{
	public:
		System::String^ m_v_type;
		System::String^ m_value;
		System::String^ m_min_value;
		System::String^ m_max_value;
		System::String^ m_caption;
		bool m_is_support_curve; // 是否为曲线
		PropertyModifierTypeStorage m_property_modifier_type_storage;
	};

	public ref class DefinedItem
	{
	public:
		/// 属性列表
		System::Collections::Generic::List<DefinedValueItem>^ m_property_list;
		/// 子预定制项
		System::Collections::Generic::List<DefinedItem^>^ m_sub_item;

		System::String^ m_id; // 单一标识
		System::String^ m_type; // 基本类型
		System::String^ m_defined_type; // 预定义类型
		System::String^ m_name; // 外部看到的名字
	};

	// 相机信息
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
	/// 后处理信息
	public ref class PostProcessParams
	{
	public:	
		typedef System::Collections::Generic::Dictionary<System::String^, H3DVec4^> PostParamCollection;
		typedef System::Collections::Generic::Dictionary<System::String^, PostParamCollection^> PostProcessCollection;

		PostProcessCollection^ m_post_process_lists;
	};

	/// 曲线数值信息
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

		///获取关键帧
		bool getKeyFrames(int sub_index, float frames[], int& buffer_size);

		///获取指定帧的插值方式
		bool getInterpolatingType(int sub_index, float frame, INTERPOLATING_TYPE& type);

		///获取指定帧的插值
		System::String^ getInterpolatingValue(int sub_index, float frame);

		/// 获取从第一个关键帧到最后一个关键帧之间的插值(每隔0.1帧获取一次)
		typedef System::Collections::Generic::Dictionary<System::String^, System::Collections::Generic::Dictionary<float, System::String^>^> CurvePoints;
		bool getInterpolatingValue(int sub_sequence_index, int length, CurvePoints^ curvePoints);

		ICalculator* m_calculator;
	};

	ref class MStageEffect;

	public ref class MGraphicEntity
	{
	public:
		MGraphicEntity(DynamicEffectObject* dyneffobj, StageAnimationClipMng* clip_mng, ActorManager* actormng);
		/// 设置物品属性
		void setAttributeInfo(PropertyModifierTypeStorage type, System::String^ value, MStageEffect^ eff);
		/// 设置绝对物品属性，如果该物体为级联物体，则位置的设定之类的改为相对属性
		void setAbsAttributeInfo(PropertyModifierTypeStorage type, System::String^ value, MStageEffect^ eff);
		/// 获取物品属性
		System::String^ getAttributeInfo(PropertyModifierTypeStorage type, MStageEffect^ eff);
		/// 获取级联物品绝对属性, 如果物体为级联物体，将物体的位置等信息变为绝对位置
		System::String^ getAbsAttributeInfo(PropertyModifierTypeStorage type, MStageEffect^ eff);

		/// 获得属性序列
		System::String^ getAttributeSeq(MStageEffect^ eff, PropertyModifierTypeStorage type);
		/// 设置属性序列
		void setAttributeSeq(MStageEffect^ eff, PropertyModifierTypeStorage type, System::String^ value);

		AttributeSequenceEvaluator^ getAttributeSeqEvaluator(MStageEffect^ eff,  PropertyModifierTypeStorage type);

		/// 重新加载
		void reload(System::String^ actorid);

		//修改名称
		void setName(System::String^ id);
		//获取名称
		System::String^ getName();

		// 重置迭代器
		void resetActorIter();
		// 获得下一个关卡信息，如果到了末尾，返回0
		MGraphicEntity^ getNextActor();

		//获取包围盒
		void getAABB(System::Collections::Generic::List<float>^ aabblist);

		//通过id获取级联子物体的名称
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

		/// 获得相机信息
		CameraInfo^ getCamInfo();
		PostProcessParams^ getCameraPostProcessParam();

		/// 设置物品属性
		void setAttributeInfo(System::String^ type, System::String^ value);
		/// 获取物品属性
		System::String^ getAttributeInfo(System::String^ type);

		/// 将指定actor和指定舞台效果、指定类型的属性序列设置为不可用
		void setAttributeSeqEnable(System::String^ type, bool is_enable);
		/// 判断指定actor、指定舞台效果和指定类型的属性序列是否可用
		bool isAttributeSeqEnable(System::String^ type);

		void disableCameraPostProcess(System::String^ postprocess_name);
		/**
		* @brief
		*		设置后处理参数是否可用
		*/
		void setCameraPostProcessEnable(System::String^  post_process_name, bool enable);


		/// 获得属性序列
		System::String^ getAttributeSeq(System::String^ type);
		/// 设置属性序列
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

	/// 舞台效果与相机的文件管理
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
		/// 转换LOD级别
		bool switchToLod(int lod);

		System::Collections::Generic::List<MPrePassLight^>^ getAllLight();
		/// 读取预定制信息
		bool loadDefinedItem(System::String^ filename, bool isBinary);
		/// 读取主文件并创建对应的内容
		bool loadStage (System::String^ pathname, System::String^ filename, bool isBinary);
		/// 保存主文件
		bool saveStage (System::String^ pathname, System::String^ filename, bool isBinary);

		/// 读取文件并创建对应的内容
		MStageEffect^ loadStageEffect(System::String^ filename, bool isBinary);
		MCamera^ loadCamera(System::String^ filename, bool isBinary);
		/// 保存文件，将内部的内容保存在指定的文件中
		bool save(System::String^ filename, MCamera^ cam, bool isBinary);
		bool save(System::String^ filename, MStageEffect^ effect, bool isBinary);

		//删除舞台效果文件中的id与指定值相等的物体的记录
		//filename:舞台效果的文件路径
		//id:要删除的物体的id
		bool removeActorFromFile(System::String^ filename, System::String^ id);

		DefinedItem^ getDefinedInfo();
		// void setDefinedInfo(DefinedItem^ defineditem);

		void resetActorIter(); /// 重置迭代器
		MGraphicEntity^ getNextActor(); /// 获得下一个效果物体，如果到了末尾，返回0
		void resetStageEffectIter(); /// 重置迭代器
		MStageEffect^ getNextStageEffect(); /// 获得下一个舞台效果，如果到了末尾，返回0
		void resetCameraIter(); /// 重置迭代器
		MCamera^ getNextCamera(); /// 获得下一个相机，如果到了末尾，返回0
		MCamera^ getCamera(System::String^  name);

		/// 添加预定制的效果物品
		MGraphicEntity^ createDefinedActor(System::String^  name, System::String^  definedtype, System::String^  info);
		/// 添加最高层的效果物品
		void addActor(MGraphicEntity^ actor);
		/// 删除最高层的效果物品
		void removeActor(MGraphicEntity^ actor);

		/// 添加舞台效果
		MStageEffect^ addStageEffect(System::String^ name, int framecount, System::String^ type, System::String^ info);
		/// 删除舞台效果
		void removeStageEffect(MStageEffect^ eff);

		/// 添加相机
		MCamera^ addCamera (System::String^ name, System::String^ type, System::String^ info);
		/// 删除相机
		void removeCamera(MCamera^ cam);

		/// 管理物品拓扑关系
		void link(MGraphicEntity^ parententity, MGraphicEntity^ subentity); 
		void unlink(MGraphicEntity^ parententity, MGraphicEntity^ entity);
		System::Collections::Generic::List<MGraphicEntity^>^ getLinkedEntities(MGraphicEntity^ parententity);

		//通过id找到对应的actor名称
		System::String^ getActorNameById(unsigned int id);
		//设定是否保存的时候进行优化
		void setSaveOpimize(bool optimize);

	private:
		void addRemovedEntity(MGraphicEntity^ e);
		void removeRemovedEntity(MGraphicEntity^ e);
		bool isRemovedEntityExists(MGraphicEntity^ e);

		void updateLinkEntityAttribute(MGraphicEntity^ parent, MGraphicEntity^ entity, bool islink);

	protected:
		//IEditInfoTableEditor* m_editinfo_table; //不用了，注释掉 lishan 101215
		Stage* m_stage;
		//StageAnimationPlayer* m_ani_player;
		StageAnimationPlayerEditor* m_ani_player;
		System::Collections::Generic::List<MGraphicEntity^>^ m_removed_entities; //被移除的entities
		bool m_save_optimize; /// 进行保存优化
	};

	ref class ClientRenderSys;

	/// 物品播放管理器
	public ref class GraphicEntityPlayerMng
	{
	public:
		//GraphicEntityPlayerMng(StageAnimationPlayer* player);
		GraphicEntityPlayerMng(StageAnimationPlayerEditor* player);
		~GraphicEntityPlayerMng();

		/// 播放
		void playGraphicEntity(MGraphicEntity^ actor, bool play, MStageEffect^ eff);
		/// 更新当前播放的所有效果物体
		void update(float t);
		/// 更新指定的效果物体
		// void update(int t, MGraphicEntity^ actor);

	protected:
		//StageAnimationPlayer* m_player;
		StageAnimationPlayerEditor* m_player;
	};

	

	/// 播放管理器
	public ref class StagePlayerMng
	{
	public:
		//StagePlayerMng(StageAnimationPlayer* player, StageAnimationClipMng* clip_mng, ActorManager* actormng);
		StagePlayerMng(StageAnimationPlayerEditor* player, StageAnimationClipMng* clip_mng, ActorManager* actormng);
		~StagePlayerMng();

		/// 播放
		void playStageEffect(StageInfoMng^ stagemng, MStageEffect^ eff, bool play);
		void playCamera(StageInfoMng^ stagemng, MCamera^ cam, bool play);
		void playLevel(bool play, LevelInfoMng^ levelmng, StageInfoMng^ stagemng);
		/// 更新
		void updateLevel(float t, LevelInfoMng^ levelmng, StageInfoMng^ stagemng); /// 更新level
		void updateCamera(StageInfoMng^ stagemng, float t, MCamera^ cam); /// 更新相机镜头
		void updateStageEffect(StageInfoMng^ stagemng, float t, MStageEffect^ eff); /// 更新舞台效果
		/// 获得当前对应的相机
		MCamera^ getCurCam();
		//渲染锥光灯,将锥光灯的范围用线画出来
		void drawWimblePrepasslightLine();
		//开启灯光效果
		void enableLightEffect(bool enable);
		/// 设置每帧的毫秒长度
		void setFrameMs(double ms);
		/// 获得每帧的毫秒长度
		double getFrameMs();

		//绘制表示灯光location的线
		void drawLightLocationLine();

		//
		//下面两个接口是为了给StageConvertor加上的。
		//
		//加载环境信息
		bool loadEnviromentFile(System::String^ env_file, bool is_binary);
		//加载静态效果物体
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
