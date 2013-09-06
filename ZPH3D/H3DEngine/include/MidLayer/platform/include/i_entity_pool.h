/******************
/* @file	i_entity_pool.h
/* @brief	�м�����ĳؽӿ�
/* @author	������
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
	@brief ����LOD�ȼ� 0-2
	*/
	virtual void setLodLevel(int level) = 0;
	/**
	@brief �õ�LOD�ȼ� 0-2
	*/
	virtual int getLodLevel() = 0;
	/**
	@brief ע��һ���´�����STAGE������ͳһ�����ù���
	*/
	virtual void registerStage(IStage* pStage) = 0;
	/**
	@brief ע��һ���´�����STAGE
	*/
	virtual void unregisterStage(IStage* pStage) = 0;
	/**
	@brief ע��һ���´���������
	*/
	virtual void registerAvatar(AvatarEntity* pAvatar) = 0;
	/**
	@brief ע��һ������
	*/
	virtual void unregisterAvatar(AvatarEntity* pAvatar) = 0;
	/**
	@brief ע��һ���´�����ģ��
	*/
	virtual void registerModel(CModelEntity* pModel) = 0;
	/**
	@brief ע��һ��ģ��
	*/
	virtual void unregisterModel(CModelEntity* pModel) = 0;
	/**
	@brief ע��һ���´����Ĺ���ģ��
	*/
	virtual void registerSkeletonModel(CSkeletonModelEntity* pModel) = 0;
	/**
	@brief ע��һ������ģ��
	*/
	virtual void unregisterSkeletonModel(CSkeletonModelEntity* pModel) = 0;
	/**
	@brief ע��һ���´����ĳ���
	*/
	virtual void registerAvatarSkeletonModel(CAvatarSkeletonModelEntity* pModel) = 0;
	/**
	@brief ע��һ������
	*/
	virtual void unregisterAvatarSkeletonModel(CAvatarSkeletonModelEntity* pModel) = 0;
	/**
	@brief ע��һ���´�������Ч
	*/
	virtual void registerEffect(CEffectEntity* pEffect) = 0;
	/**
	@brief ע��һ����Ч
	*/
	virtual void unregisterEffect(CEffectEntity* pEffect) = 0;
	/**
	@brief ���þ���
	*/
	virtual void configDOF(int dof) = 0;
	/**
	@brief ����AA
	*/
	virtual void configAA(int aa) = 0;
};


#endif