#include "Stdafx.h"
#include "i_target_position_impl.h"


	void ITargetPositionImpl::update( float t )
	{
		// do not need do anything now
	}

	void ITargetPositionImpl::set_info( const char* info_name, const char* info_value )
	{

	}

	static H3DVec3 get_target_from_bone(H3DI::ISkeletonModel* target)
	{
		H3DI::ISkeletonModel* skeleton_model = target;
		if (NULL != skeleton_model)
		{
			H3DI::ISkeleton* skeleton = skeleton_model->GetSkeleton();
			if (skeleton)
			{
				H3DI::IBone* bone = skeleton->GetBone("Bip01");
				assert(bone);

				const H3DMat4& mat = skeleton_model->GetCurMatrix(bone->GetID());
				H3DVec3 offset;
				mat.GetTranslation(offset);
				return offset;
			}
		}

		return H3DVec3(0.0f, 0.0f, 0.0f);
		
	}

	H3DVec3 ITargetPositionImpl::get_target_position()
	{
		float pos[3] = {0.0f};
		get_target_position(pos);
		return H3DVec3(pos[0], pos[1], pos[2]);
	}

	void ITargetPositionImpl::get_target_position( float pos[3] )
	{
		if (!mbValid)
		{
			pos[0] = pos[1] = pos[2] = 0;
		}
		else
		{
			H3DVec3 tarpos = get_target_from_bone(mTarget);

			pos[0] = tarpos[0];
			pos[1] = tarpos[1];
			pos[2] = tarpos[2];
		}
	}

	ITargetPositionImpl::ITargetPositionImpl( TargetType targetType, TargetID id, H3DI::ISkeletonModel* target )
	{
		mTargetType = targetType;
		mTargetID = id;
		mTarget = target;

		mbValid = true;

		mBoneName = "Bip01";
	}

	ITargetPositionImpl::~ITargetPositionImpl()
	{
		// 这里不能释放
		// mTarget->Release();
	}

	ITargetPosition* ITargetPositionMngImpl::getTargetPosition( TargetType tm, const TargetID& targetid )
	{
		switch (tm)
		{
		case TargetType_ALL_TARGET:
			if (mMultiTargetPositionList.size() > 0)
			{
				return mMultiTargetPositionList[0];
			}
			else
			{
				return 0;
			}
			break;
		case TargetType_SPECIFIED_TARGET:
			{
				for (int i = 0; i < mTargetPositonList.size(); ++i)
				{
					if (mTargetPositonList[i]->getTargetType() == tm && mTargetPositonList[i]->getTargetID() == targetid)
					{
						return mTargetPositonList[i];
					}
				}
			}
		}
		return 0;
	}

	MultiTargetPositionImpl::MultiTargetPositionImpl(TargetType targetType, TargetID id, H3DI::ISkeletonModel* target)
		:ITargetPositionImpl(targetType, id, target)
	{
		mTargetList.clear();
		mTargetList.push_back(tTarget(id, target, true));
	}

	void MultiTargetPositionImpl::get_target_position(float pos[3])
	{
		H3DVec3 acc(0, 0, 0);
		int cnt = 0;

		for(std::vector<tTarget>::iterator itr = mTargetList.begin();
			itr!= mTargetList.end(); ++itr)
		{
			if ((*itr).mbValid)
			{
				H3DVec3 tarpos = get_target_from_bone((*itr).mTarget);
				acc += tarpos;
				++cnt;
			}
		}

		if(cnt > 0)
		{
			acc /= cnt;
		}
		

		pos[0] = acc[0];
		pos[1] = acc[1];
		pos[2] = acc[2];
	}

	void MultiTargetPositionImpl::set_valid_byIdx(int idx, bool bValid)
	{
		for(std::vector<tTarget>::iterator itr = mTargetList.begin();
			itr!= mTargetList.end(); ++itr)
		{
			if ((*itr).mIdx == idx)
			{
				(*itr).mbValid = bValid;
				return;
			}
		}
	}

	void ITargetPositionMngImpl::Update( float t )
	{
		// do not need do anything now
	}

	void ITargetPositionMngImpl::AddTarget( TargetType type,  int idx, H3DI::ISkeletonModel* target )
	{
		switch (type)
		{
		case TargetType_ALL_TARGET:
			{
				// 如果存储多目标的列表中有，则直接使用
				// 否则创建一个
				MultiTargetPositionImpl* multi = NULL;
				if (mMultiTargetPositionList.size() > 0)
				{
					multi = static_cast<MultiTargetPositionImpl*>(mMultiTargetPositionList[0]);
					for (std::vector<MultiTargetPositionImpl::tTarget>::iterator itr = multi->mTargetList.begin(); itr != multi->mTargetList.end(); ++itr)
					{
						if ((*itr).mIdx == idx)
						{
							(*itr).mTarget = target;
							return;
						}
					}
					// 如果没有对应的idx的，则创建一个
					multi->mTargetList.push_back(MultiTargetPositionImpl::tTarget(idx, target, true));
				}
				else
				{
					multi = new MultiTargetPositionImpl(type, idx, target);
					multi->setBoneName(mBoneName);
					mMultiTargetPositionList.push_back(multi);
				}
			}
			break;
		case TargetType_SPECIFIED_TARGET:
			{
				// 查找是否有相同ID的
				for (int i = 0; i < mTargetPositonList.size(); ++i)
				{
					if (mTargetPositonList[i]->getTargetID() == idx)
					{
						mTargetPositonList[i]->setTargetType(type);
						mTargetPositonList[i]->setTarget(target);
						mTargetPositonList[i]->setBoneName(mBoneName);

						return;
					}
				}
				// 如果没有相同ID的，则创建一个
				ITargetPositionImpl* newTarget = new ITargetPositionImpl(type, idx, target);
				newTarget->setBoneName(mBoneName);
				mTargetPositonList.push_back(newTarget);
			}
			break;
		default:
			break;
		}
	}

	void ITargetPositionMngImpl::SetValidByIdx(int idx, bool bValid)
	{
		for (std::vector<ITargetPositionImpl*>::iterator itr = mTargetPositonList.begin(); itr != mTargetPositonList.end(); ++itr)
		{
			if ((*itr)->getTargetID() == idx)
			{
				(*itr)->set_valid_byIdx(idx, bValid);
				break;
			}
		}

		for (std::vector<ITargetPositionImpl*>::iterator itr = mMultiTargetPositionList.begin(); itr != mMultiTargetPositionList.end(); ++itr)
		{
			(*itr)->set_valid_byIdx(idx, bValid);
		}
	}

	ITargetPositionMngImpl::ITargetPositionMngImpl()
	{
		mTargetPositonList.clear();
		mBoneName = "Bip01";
		m_min_interval = 0;
	}

	ITargetPositionMngImpl::~ITargetPositionMngImpl()
	{
 		for (int i = 0; i < mTargetPositonList.size(); ++i)
 		{
			delete mTargetPositonList[i]; 			
 		}
		mTargetPositonList.clear();

		for (int i = 0; i < mMultiTargetPositionList.size(); ++i)
		{
			delete mMultiTargetPositionList[i]; 			
		}
		mMultiTargetPositionList.clear();
	}

	void ITargetPositionMngImpl::setMinInterval(int interval)
	{
		m_min_interval = interval;
	};

	int ITargetPositionMngImpl::getMinInterval()
	{
		return m_min_interval;
	}

