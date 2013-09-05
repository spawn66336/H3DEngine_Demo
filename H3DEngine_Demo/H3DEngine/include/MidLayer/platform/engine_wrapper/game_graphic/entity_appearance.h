/**
* @file entity_appearance.h
* ����ļ�����Ҫ�����ǣ�
*			��������Դ����
* 
* @author ֣�ɿ�(zhengchengjun@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2012.06.29
*/

#ifndef _ENTITY_APPEARANCE_H_
#define _ENTITY_APPEARANCE_H_

#include "game_graphic_header.h"

const int MODEL_NAME_LENGTH = 128; ///<ģ����Դ����󳤶�
const int SKELETON_MODEL_NAME_LENGTH = 128; ///<����ģ����Դ����󳤶�
const int EFFECT_NAME_LENGTH = 128; ///<��Ч��Դ����󳤶�
const int BODYPART_NAME_LENGTH = 128; ///<bodypart��Դ����󳤶�
const int ADORNMENT_NAME_LENGTH = 128; ///<�Ҽ���Դ����󳤶�

/**
* @brief
*		ModelAppearance��ģ�͵���Դ����
*/
struct ModelAppearance
{
	int geo_lod; ///<����lod����
	int mat_lod; ///<����lod����
	char res_name[MODEL_NAME_LENGTH + 1]; ///<��Դ��

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
*		SkeletonModelAppearance�ǹ���ģ�͵���Դ����
*/
struct SkeletonModelAppearance
{
	int geo_lod; ///<����lod����
	int mat_lod; ///<����lod����
	char res_name[SKELETON_MODEL_NAME_LENGTH + 1]; ///<��Դ��

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
*		AvatarAppearance�ǽ�ɫ����Դ����
*/
struct AvatarAppearance
{
	int geo_lod; ///<����lod����
	int mat_lod; ///<����lod����
	bool male; ///<�Ա�
	char bodyparts[H3DI::BODYPART_NUM][BODYPART_NAME_LENGTH + 1]; ///<bodypart����
	char adornments[H3DI::ACTOR_ADORNMENT_NUM][ADORNMENT_NAME_LENGTH + 1]; ///<�Ҽ�����

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
*		AvatarSkeletonAppearance�ǳ������Դ����
*/
struct AvatarSkeletonAppearance
{
	int geo_lod; ///<����lod����
	int mat_lod; ///<����lod����
	bool male; ///<�Ա�
	char bodyparts[H3DI::PET_BODYPART_NUM][BODYPART_NAME_LENGTH + 1]; ///<bodypart����
	char adornments[H3DI::PET_ADORNMENT_NUM][ADORNMENT_NAME_LENGTH + 1]; ///<�Ҽ�����

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
*		EffectAppearance����Ч����Դ����
*/
struct EffectAppearance
{
	int geo_lod; ///<����lod����
	int mat_lod; ///<����lod����
	char res_name[EFFECT_NAME_LENGTH + 1]; ///<��Դ��

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