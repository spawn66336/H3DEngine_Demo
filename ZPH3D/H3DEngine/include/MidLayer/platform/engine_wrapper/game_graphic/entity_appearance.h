/**
* @file entity_appearance.h
* 这个文件的主要功能是：
*			声明了资源描述
* 
* @author 郑成俊(zhengchengjun@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2012.06.29
*/

#ifndef _ENTITY_APPEARANCE_H_
#define _ENTITY_APPEARANCE_H_

#include "game_graphic_header.h"

const int MODEL_NAME_LENGTH = 128; ///<模型资源名最大长度
const int SKELETON_MODEL_NAME_LENGTH = 128; ///<骨骼模型资源名最大长度
const int EFFECT_NAME_LENGTH = 128; ///<特效资源名最大长度
const int BODYPART_NAME_LENGTH = 128; ///<bodypart资源名最大长度
const int ADORNMENT_NAME_LENGTH = 128; ///<挂件资源名最大长度

/**
* @brief
*		ModelAppearance是模型的资源描述
*/
struct ModelAppearance
{
	int geo_lod; ///<几何lod级别
	int mat_lod; ///<材质lod级别
	char res_name[MODEL_NAME_LENGTH + 1]; ///<资源名

	void operator = (const ModelAppearance& other)
	{
		geo_lod = other.geo_lod;
		mat_lod = other.mat_lod;
		strcpy_s(res_name, MODEL_NAME_LENGTH + 1, other.res_name);
	}

	bool operator == (const ModelAppearance& other) const
	{
		if (geo_lod!=other.geo_lod
			|| mat_lod!=other.mat_lod
			|| strcmp(res_name, other.res_name)!=0)
		{
			return false;
		}

		return true;
	}
};

/**
* @brief
*		SkeletonModelAppearance是骨骼模型的资源描述
*/
struct SkeletonModelAppearance
{
	int geo_lod; ///<几何lod级别
	int mat_lod; ///<材质lod级别
	char res_name[SKELETON_MODEL_NAME_LENGTH + 1]; ///<资源名

	void operator = (const SkeletonModelAppearance& other)
	{
		geo_lod = other.geo_lod;
		mat_lod = other.mat_lod;
		strcpy_s(res_name, SKELETON_MODEL_NAME_LENGTH + 1, other.res_name);
	}

	bool operator == (const SkeletonModelAppearance& other) const
	{
		if (geo_lod!=other.geo_lod
			|| mat_lod!=other.mat_lod
			|| strcmp(res_name, other.res_name)!=0)
		{
			return false;
		}

		return true;
	}
};

/**
* @brief
*		AvatarAppearance是角色的资源描述
*/
struct AvatarAppearance
{
	int geo_lod; ///<几何lod级别
	int mat_lod; ///<材质lod级别
	bool male; ///<性别
	char bodyparts[H3DI::BODYPART_NUM][BODYPART_NAME_LENGTH + 1]; ///<bodypart描述
	char adornments[H3DI::ACTOR_ADORNMENT_NUM][ADORNMENT_NAME_LENGTH + 1]; ///<挂件描述

	void operator = (const AvatarAppearance& other)
	{
		geo_lod = other.geo_lod;
		mat_lod = other.mat_lod;
		male = other.male;
		for (int i=0; i<H3DI::BODYPART_NUM; ++i)
		{
			strcpy_s(bodyparts[i], BODYPART_NAME_LENGTH + 1, other.bodyparts[i]);
		}
		for (int i=0; i<H3DI::ACTOR_ADORNMENT_NUM; ++i)
		{
			strcpy_s(adornments[i], ADORNMENT_NAME_LENGTH + 1, other.adornments[i]);
		}
	}

	bool operator == (const AvatarAppearance& other) const
	{
		if (geo_lod!=other.geo_lod
			|| mat_lod!=other.mat_lod
			|| male!=other.male)
		{
			return false;
		}

		for (int i=0; i<H3DI::BODYPART_NUM; ++i)
		{
			if (strcmp(bodyparts[i], other.bodyparts[i]) != 0)
			{
				return false;
			}
		}

		for (int i=0; i<H3DI::ACTOR_ADORNMENT_NUM; ++i)
		{
			if (strcmp(adornments[i], other.adornments[i]) != 0)
			{
				return false;
			}
		}

		return true;
	}
};

/**
* @brief
*		AvatarSkeletonAppearance是宠物的资源描述
*/
struct AvatarSkeletonAppearance
{
	int geo_lod; ///<几何lod级别
	int mat_lod; ///<材质lod级别
	bool male; ///<性别
	char bodyparts[H3DI::PET_BODYPART_NUM][BODYPART_NAME_LENGTH + 1]; ///<bodypart描述
	char adornments[H3DI::PET_ADORNMENT_NUM][ADORNMENT_NAME_LENGTH + 1]; ///<挂件描述

	void operator = (const AvatarSkeletonAppearance& other)
	{
		geo_lod = other.geo_lod;
		mat_lod = other.mat_lod;
		male = other.male;
		for (int i=0; i<H3DI::PET_BODYPART_NUM; ++i)
		{
			strcpy_s(bodyparts[i], BODYPART_NAME_LENGTH + 1, other.bodyparts[i]);
		}
		for (int i=0; i<H3DI::PET_ADORNMENT_NUM; ++i)
		{
			strcpy_s(adornments[i], ADORNMENT_NAME_LENGTH + 1, other.adornments[i]);
		}
	}

	bool operator == (const AvatarSkeletonAppearance& other) const
	{
		if (geo_lod!=other.geo_lod
			|| mat_lod!=other.mat_lod
			|| male!=other.male)
		{
			return false;
		}

		for (int i=0; i<H3DI::PET_BODYPART_NUM; ++i)
		{
			if (strcmp(bodyparts[i], other.bodyparts[i]) != 0)
			{
				return false;
			}
		}

		for (int i=0; i<H3DI::PET_ADORNMENT_NUM; ++i)
		{
			if (strcmp(adornments[i], other.adornments[i]) != 0)
			{
				return false;
			}
		}

		return true;
	}
};

/**
* @brief
*		EffectAppearance是特效的资源描述
*/
struct EffectAppearance
{
	int geo_lod; ///<几何lod级别
	int mat_lod; ///<材质lod级别
	char res_name[EFFECT_NAME_LENGTH + 1]; ///<资源名

	void operator = (const EffectAppearance& other)
	{
		geo_lod = other.geo_lod;
		mat_lod = other.mat_lod;
		strcpy_s(res_name, EFFECT_NAME_LENGTH + 1, other.res_name);
	}

	bool operator == (const EffectAppearance& other) const
	{
		if (geo_lod!=other.geo_lod
			|| mat_lod!=other.mat_lod
			|| strcmp(res_name, other.res_name)!=0)
		{
			return false;
		}

		return true;
	}
};

#endif //_ENTITY_APPEARANCE_H_