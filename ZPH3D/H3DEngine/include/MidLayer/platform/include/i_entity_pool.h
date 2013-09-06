/******************
/* @file	i_entity_pool.h
/* @brief	中间层对象的池接口
/* @author	张逸梵
/* @date	2012/07/27
/* @note
******************/
#ifndef __I_ENTITY_POOL_H__
#define __I_ENTITY_POOL_H__

class IStage;
class CModelEntity;
class AvatarEntity;
class CSkeletonModelEntity;
class CAvatarSkeletonModelEntity;
class CEffectEntity;

class I_Entity_Pool
{
public:
	/**
	@brief 设置LOD等级 0-2
	*/
	virtual void setLodLevel(int level) = 0;
	/**
	@brief 得到LOD等级 0-2
	*/
	virtual int getLodLevel() = 0;
	/**
	@brief 注册一个新创建的STAGE，加入统一的设置管理
	*/
	virtual void registerStage(IStage* pStage) = 0;
	/**
	@brief 注销一个新创建的STAGE
	*/
	virtual void unregisterStage(IStage* pStage) = 0;
	/**
	@brief 注册一个新创建的人物
	*/
	virtual void registerAvatar(AvatarEntity* pAvatar) = 0;
	/**
	@brief 注销一个人物
	*/
	virtual void unregisterAvatar(AvatarEntity* pAvatar) = 0;
	/**
	@brief 注册一个新创建的模型
	*/
	virtual void registerModel(CModelEntity* pModel) = 0;
	/**
	@brief 注销一个模型
	*/
	virtual void unregisterModel(CModelEntity* pModel) = 0;
	/**
	@brief 注册一个新创建的骨骼模型
	*/
	virtual void registerSkeletonModel(CSkeletonModelEntity* pModel) = 0;
	/**
	@brief 注销一个骨骼模型
	*/
	virtual void unregisterSkeletonModel(CSkeletonModelEntity* pModel) = 0;
	/**
	@brief 注册一个新创建的宠物
	*/
	virtual void registerAvatarSkeletonModel(CAvatarSkeletonModelEntity* pModel) = 0;
	/**
	@brief 注销一个宠物
	*/
	virtual void unregisterAvatarSkeletonModel(CAvatarSkeletonModelEntity* pModel) = 0;
	/**
	@brief 注册一个新创建的特效
	*/
	virtual void registerEffect(CEffectEntity* pEffect) = 0;
	/**
	@brief 注销一个特效
	*/
	virtual void unregisterEffect(CEffectEntity* pEffect) = 0;
	/**
	@brief 设置景深
	*/
	virtual void configDOF(int dof) = 0;
	/**
	@brief 设置AA
	*/
	virtual void configAA(int aa) = 0;
};


#endif