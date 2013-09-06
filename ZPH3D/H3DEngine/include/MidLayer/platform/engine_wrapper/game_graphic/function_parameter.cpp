/**
* @file function_parameter.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.23
*/

#include "game_graphic_body.h"
#include "function_parameter.h"

ByteString::ByteString()
{
	m_str_value = "";
	m_binary_value = NULL;
	m_binary_len = 0;

	m_has_binary = false;
	m_has_str = false;
}

ByteString::ByteString(const std::string& v)
{
	m_str_value = "";
	m_binary_value = NULL;
	m_binary_len = 0;

	m_has_binary = false;
	m_has_str = false;

	set_value(v);
}

ByteString::ByteString(const BYTE* v, int length)
{
	m_str_value = "";
	m_binary_value = NULL;
	m_binary_len = 0;

	m_has_binary = false;
	m_has_str = false;

	set_value(v, length);
}

ByteString::~ByteString()
{
	delete m_binary_value;
}

void ByteString::set_value(const std::string& v)
{
	if (m_str_value.length() > 0)
	{
		reset();
	}

	m_str_value = v;
	m_has_str = true;
}

void ByteString::set_value(const BYTE* v, int length)
{
	if (m_binary_value != NULL)
	{
		reset();		
	}

	m_binary_value = new BYTE[length + 1];
	memset(m_binary_value, 0, length + 1);
	memcpy(m_binary_value, v, length);

	m_binary_len = length;
	m_has_binary = true;
}

const BYTE* ByteString::get_binary_value()
{
	return m_binary_value;
}

const std::string& ByteString::get_str_value()
{
	return m_str_value;
}

int	  ByteString::get_binary_length()
{
	if (has_binary())
	{
		return m_binary_len;
	}

	return 0;
}

bool ByteString::has_binary()
{
	return m_has_binary;
}

bool ByteString::has_string()
{
	return m_has_str;
}


void ByteString::reset()
{
	m_str_value = "";

	delete m_binary_value;
	m_binary_value = NULL;
	m_binary_len = 0;

	m_has_binary = false;
	m_has_str = false;
}

namespace GameGraphicHelper
{
	std::string to_lower(const std::string& str)
	{
		std::string result(str.length(), '\0');
		std::transform(str.begin(), str.end(), result.begin(), tolower);
		return result;
	}

	std::string xml2Str(BiXmlElement* xml)
	{
		std::string str;
		str << xml;
		return str;
	}

//////////////////////////////////////////////////////////////////////////
	template<>
	int str2Value<int>(const std::string& v)
	{
		return atoi(v.c_str());
	}

	template<>
	std::string value2Str<int>(const int& value)
	{
		char buf[20] = {0};
		itoa(value, buf, 10);
		return buf;
	}
//////////////////////////////////////////////////////////////////////////
	template<>
	float str2Value<float>(const std::string& v)
	{
		return (float)atof(v.c_str());
	}

	template<>
	std::string value2Str<float>(const float& value)
	{
		char buf[30] = {0};
		sprintf(buf, "%f", value);
		return buf;	
	}

//////////////////////////////////////////////////////////////////////////
	template<>
	bool str2Value<bool>(const std::string& v)
	{
		if (stricmp(v.c_str(), "true") == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<>
	std::string value2Str<bool>(const bool& value)
	{
		if (value)
		{
			return "true";
		}

		return "false";
	}

	//////////////////////////////////////////////////////////////////////////
	template<>
	std::string str2Value<std::string>(const std::string& v)
	{
		return v;
	}

	template<>
	std::string value2Str<std::string>(const std::string& value)
	{
		return value;
	}

	//////////////////////////////////////////////////////////////////////////
	template<>
	H3DVec3 str2Value<H3DVec3>(const std::string& v)
	{
		H3DVec3 vec;
		sscanf(v.c_str(), "{%f, %f, %f}", &(vec.x), &(vec.y), &(vec.z));
		return vec;
	}

	template<>
	std::string value2Str<H3DVec3>(const H3DVec3& value)
	{
		char buf[100] = {0};
		sprintf(buf, "{%f,%f,%f}", value.x, value.y, value.z);
		return buf;
	}

	//////////////////////////////////////////////////////////////////////////
	template<>
	H3DColor str2Value<H3DColor>(const std::string& v)
	{
		float buf[4] = {0.0f};
		sscanf(v.c_str(), "{%f, %f, %f, %f}", &(buf[0]), &(buf[1]), &(buf[2]), &(buf[3]));
		H3DColor parameter = buf;
		return parameter;
	}

	template<>
	std::string value2Str<H3DColor>(const H3DColor& value)
	{
		char buf[100] = {0};
		sprintf(buf, "{%f,%f,%f,%f}", value.getRed(), value.getGreen(), value.getBlue(), value.getAlpha());
		return buf;		
	}

	template<>
	H3DVec4 str2Value<H3DVec4>(const std::string& v)
	{
		H3DVec4 vec;
		sscanf(v.c_str(), "{%f, %f, %f, %f}", &(vec.x), &(vec.y), &(vec.z), &(vec.w));

		return vec;
	}

	template<>
	std::string value2Str<H3DVec4>(const H3DVec4& value)
	{
		char buf[100] = {0};
		sprintf(buf, "{%f,%f,%f,%f}", value[0], value[1], value[2], value[3]);
		return buf;		
	}


//////////////////////////////////////////////////////////////////////////
	template<>
	H3DMat4 str2Value<H3DMat4>(const std::string& v)
	{
		float result[4][4] = {0};
		sscanf(v.c_str(), "{%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f}", &(result[0][0]), &(result[0][1]), &(result[0][2]), &(result[0][3]),	&(result[1][0]), &(result[1][1]), &(result[1][2]), &(result[1][3]),&(result[2][0]), &(result[2][1]), &(result[2][2]), &(result[2][3]),	&(result[3][0]), &(result[3][1]), &(result[3][2]), &(result[3][3]));

		return H3DMat4(result);
	}

	template<>
	std::string value2Str<H3DMat4>(const H3DMat4& value)
	{
		char buf[100] = {0};
		sprintf(buf, "{%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f}", value[0][0], value[0][1], value[0][2], value[0][3], value[1][0], value[1][1], value[1][2], value[1][3], value[2][0], value[2][1], value[2][2], value[2][3], value[3][0], value[3][1], value[3][2], value[3][3]);
		return buf;	
	}

//////////////////////////////////////////////////////////////////////////
	template<>
	H3DMat3 str2Value<H3DMat3>(const std::string& v)
	{
		float result[3][3] = {0};
		sscanf(v.c_str(), "{%f, %f, %f, %f, %f, %f, %f, %f, %f}", 			&(result[0][0]), &(result[0][1]), &(result[0][2]),			&(result[1][0]), &(result[1][1]), &(result[1][2]),			&(result[2][0]), &(result[2][1]), &(result[2][2]));

		return H3DMat3(result);
	}

	template<>
	std::string value2Str<H3DMat3>(const H3DMat3& value)
	{
		char buf[100] = {0};
		sprintf(buf, "{%f, %f, %f, %f, %f, %f, %f, %f, %f}", value[0][0], value[0][1], value[0][2], value[1][0], value[1][1], value[1][2], value[2][0], value[2][1], value[2][2]);
		return buf;	
	}

	template<>
	H3DI::EH3DBEAMTYPE str2Value<H3DI::EH3DBEAMTYPE>(const std::string& v)
	{
		H3DI::EH3DBEAMTYPE beam_type = H3DI::E_H3D_BEAM_POST;
		if (stricmp("E_H3D_BEAM_POST", v.c_str()) == 0)
		{
			beam_type = H3DI::E_H3D_BEAM_POST;
		}
		else if (stricmp("E_H3D_BEAM_BOARD", v.c_str()) == 0)
		{
			beam_type = H3DI::E_H3D_BEAM_BOARD;
		}
		else if (stricmp("E_H3D_BEAM_VOL", v.c_str()) == 0)
		{
			beam_type = H3DI::E_H3D_BEAM_VOL;
		}
		else if (stricmp("E_H3D_BEAM_UE", v.c_str()) == 0)
		{
			beam_type = H3DI::E_H3D_BEAM_UE;
		}

		return beam_type;
	}

	template<>
	std::string value2Str<H3DI::EH3DBEAMTYPE>(const H3DI::EH3DBEAMTYPE& value)
	{
		std::string str = "E_H3D_BEAM_POST";
		switch(value)
		{
		case H3DI::E_H3D_BEAM_BOARD:
			str = "E_H3D_BEAM_BOARD";
			break;
		case H3DI::E_H3D_BEAM_VOL:
			str = "E_H3D_BEAM_VOL";
			break;
		case H3DI::E_H3D_BEAM_POST:
			str = "E_H3D_BEAM_POST";
			break;
		case H3DI::E_H3D_BEAM_UE:
			str = "E_H3D_BEAM_UE";
			break;
		default:
			break;
		}

		return str;
	}

	template<>
	SmoothType str2Value<SmoothType>(const std::string& v)
	{
		SmoothType smooth_type = SmoothType_NONE;
		if (stricmp("SmoothType_NONE", v.c_str()) == 0)
		{
			smooth_type = SmoothType_NONE;
		}
		else if (stricmp("SmoothType_AVERAGE_SMOOTH", v.c_str()) == 0)
		{
			smooth_type = SmoothType_AVERAGE_SMOOTH;
		}
		else if (stricmp("SmoothType_ON_ENTER", v.c_str()) == 0)
		{
			smooth_type = SmoothType_ON_ENTER;
		}
		else
		{
		}

		return smooth_type;	
	};

	template<>
	std::string value2Str<SmoothType>(const SmoothType& value)
	{
		std::string str ;
		switch(value)
		{
		case SmoothType_NONE:
			str = "SmoothType_NONE";
			break;
		case SmoothType_AVERAGE_SMOOTH:
			str = "SmoothType_AVERAGE_SMOOTH";
			break;
		case SmoothType_ON_ENTER:
			str = "SmoothType_ON_ENTER";
			break;

		default:
			str = "SmoothType_NONE";
			break;
		}

		return str;
	};	

	
	template<>
	TargetType str2Value<TargetType>(const std::string& v)
	{
		TargetType target_type = TargetType_ALL_TARGET;
		if (stricmp("TargetType_ALL_TARGET", v.c_str()) == 0)
		{
			target_type = TargetType_ALL_TARGET;
		}
		else if (stricmp("TargetType_SPECIFIED_TARGET", v.c_str()) == 0)
		{
			target_type = TargetType_SPECIFIED_TARGET;
		}
		else
		{
		}
		return target_type;	
	};

	template<>
	std::string value2Str<TargetType>(const TargetType& value)
	{
		std::string str ;
		switch(value)
		{
		case TargetType_ALL_TARGET:
			str = "TargetType_ALL_TARGET";
			break;
		case TargetType_SPECIFIED_TARGET:
			str = "TargetType_SPECIFIED_TARGET";
			break;
		default:
			str = "TargetType_ALL_TARGET";
			break;
		}
		return str;
	}

	template<>
	H3DI::LightAffectParam str2Value<H3DI::LightAffectParam>(const std::string& v)
	{
		H3DI::LightAffectParam target_type = H3DI::AFFECT_ALL;
		if (stricmp("AFFECT_ALL", v.c_str()) == 0)
		{
			target_type = H3DI::AFFECT_ALL;
		}
		else if (stricmp("AFFECT_SCENE_ONLY", v.c_str()) == 0)
		{
			target_type = H3DI::AFFECT_SCENE_ONLY;
		}
		else if (stricmp("AFFECT_ACTOR_ONLY", v.c_str()) == 0)
		{
			target_type = H3DI::AFFECT_ACTOR_ONLY;
		}
		else if (stricmp("LIGHTAFFECTPARAM_END", v.c_str()) == 0)
		{
			target_type = H3DI::LIGHTAFFECTPARAM_END;
		}
		else
		{
		}
		return target_type;	
	}

	template<>
	std::string value2Str<H3DI::LightAffectParam>(const H3DI::LightAffectParam& value)
	{
		std::string str ;
		switch(value)
		{
		case H3DI::AFFECT_ALL:
			str = "AFFECT_ALL";
			break;
		case H3DI::AFFECT_SCENE_ONLY:
			str = "AFFECT_SCENE_ONLY";
			break;
		case H3DI::AFFECT_ACTOR_ONLY:
			str = "AFFECT_ACTOR_ONLY";
			break;
		case H3DI::LIGHTAFFECTPARAM_END:
			str = "LIGHTAFFECTPARAM_END";
			break;
		default:
			str = "AFFECT_ALL";
			break;
		}
		return str;
	}


	//////////////////////////////////////////////////////////////////////////
	template<>
	H3DVec3_2 str2Value<H3DVec3_2>(const std::string& v)
	{
		H3DVec3_2 vec;
		sscanf(v.c_str(), "{%f, %f, %f}", &(vec.m_position.x), &(vec.m_position.y), &(vec.m_position.z));
		return vec;
	}

	template<>
	std::string value2Str<H3DVec3_2>(const H3DVec3_2& value)
	{
		char buf[100] = {0};
		sprintf(buf, "{%f,%f,%f}", value.m_position.x, value.m_position.y, value.m_position.z);
		return buf;
	}
	/////////////////////////////////////////////////////////////////////////////
	Actor* find_actor(Actor* actor, const ActorID& subactor_id)
	{
		ActorID actor_id = "";
		int begin_index = 1;
		while (true)
		{
			if ((begin_index >= subactor_id.length()) || (NULL == actor))
			{
				return actor;
			}

			int index = subactor_id.find('/', begin_index);
			if (index == std::string::npos)
			{
				actor_id = subactor_id.substr(begin_index, subactor_id.length()- begin_index);
				return actor->find_actor(actor_id.c_str());
			}
			else
			{
				actor_id = subactor_id.substr(begin_index, index - begin_index);
				actor = actor->find_actor(actor_id.c_str());
			}
			begin_index = index + 1;
		}

		return NULL;
	}
}
