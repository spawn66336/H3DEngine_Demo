#ifndef _I_TARGET_POSITION_IMPL_H_
#define _I_TARGET_POSITION_IMPL_H_

#include "i_target_position.h"

	
	class ITargetPositionImpl : public ITargetPosition
	{
	public:
		ITargetPositionImpl(TargetType targetType, TargetID id, H3DI::ISkeletonModel* target);
		virtual ~ITargetPositionImpl();

		virtual void update(float t);

		virtual void set_info(const char* info_name, const char* info_value);

		virtual void get_target_position(float pos[3]);
		virtual H3DVec3 get_target_position();

		virtual void set_valid_byIdx(int idx, bool bValid) { mbValid = bValid; }

		TargetType getTargetType() { return mTargetType; }
		TargetID getTargetID() { return mTargetID; }
		H3DI::ISkeletonModel* getTarget() { return mTarget; }
		// 不允许重新设置ID
		void setTargetType(TargetType type) { mTargetType = type; }
		void setTarget(H3DI::ISkeletonModel* target) { mTarget = target; }

		void setBoneName(std::string boneName) { mBoneName = boneName; }
		std::string getBoneName() { return mBoneName; }

	protected:
		TargetType mTargetType;
		TargetID mTargetID;
		H3DI::ISkeletonModel* mTarget;
		// 骨骼名字可由配置文件设置
		std::string mBoneName;
		// 是否可用
		bool mbValid;
	};

	class MultiTargetPositionImpl : public ITargetPositionImpl
	{
	public:
		struct tTarget
		{
			tTarget(int idx, H3DI::ISkeletonModel* model, bool valid)
			{
				mIdx = idx;
				mTarget = model;
				mbValid = valid;
			}

			H3DI::ISkeletonModel* mTarget;
			int mIdx;
			bool mbValid;
		};
	public:
		MultiTargetPositionImpl(TargetType targetType, TargetID id, H3DI::ISkeletonModel* target);

		std::vector<tTarget> mTargetList;

		virtual void get_target_position(float pos[3]);
		virtual void set_valid_byIdx(int idx, bool bValid);
	};

	class ITargetPositionMngImpl : public ITargetPositionMng
	{
	public:
		ITargetPositionMngImpl();
		~ITargetPositionMngImpl();

		virtual ITargetPosition* getTargetPosition(TargetType tm, const TargetID& targetid);

		void Update(float t);

		void AddTarget(TargetType type, int idx, H3DI::ISkeletonModel* target);

		void SetBoneName(std::string boneName) { mBoneName = boneName; }

		void SetValidByIdx(int idx, bool bValid);

		virtual void setMinInterval(int interval);
		/**
		@brief 取最小采样间隔，用于计算器更新采样间隔
		*/
		virtual int getMinInterval();

	protected:
		std::string mBoneName;
		std::vector<ITargetPositionImpl*> mTargetPositonList;
		std::vector<ITargetPositionImpl*> mMultiTargetPositionList;

		int	m_min_interval; /// 最小采样间隔
	};
#endif