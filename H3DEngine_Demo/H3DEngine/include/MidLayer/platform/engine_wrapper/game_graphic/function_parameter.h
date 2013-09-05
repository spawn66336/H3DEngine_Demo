/**
* @file function_parameter.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.23
*/

#ifndef _FUNCTION_PARAMETER_H_
#define _FUNCTION_PARAMETER_H_

#include "game_graphic_header.h"

//定义一个类，用来进行二进制和string的复制
class ByteString
{
public:
	ByteString();
	ByteString(const std::string& v);
	ByteString(const BYTE* v, int length);
	~ByteString();

	void set_value(const std::string& v);
	void set_value(const BYTE* v, int length);

	const BYTE* get_binary_value();
	const std::string& get_str_value();
	int	  get_binary_length();
	bool has_binary();
	bool has_string();
private:
	void reset();

private:
	std::string m_str_value;
	BYTE*		m_binary_value;
	int			m_binary_len;

	bool		m_has_binary;
	bool		m_has_str;
};

//记录位置和切线方向
struct H3DVec3_2
{
	H3DVec3	m_position;
	H3DVec3 m_tangent;
	H3DVec3_2()	{
		m_position = H3DVec3(0.0f, 0.0f, 0.0f);
		m_tangent = H3DVec3(0.0f, 0.0f, 0.0f);
	};
	H3DVec3_2(H3DVec3_2& other)
	{
		m_position = other.m_position;
		m_tangent = other.m_tangent;
	}
	H3DVec3_2& operator = (H3DVec3_2& other)
	{
		m_position = other.m_position;
		m_tangent = other.m_tangent;
		return *this;
	}
	bool operator == (H3DVec3_2& other)
	{
		return m_position == other.m_position && m_tangent == other.m_tangent;
	}
	H3DVec3_2 operator * (float& other)
	{
		H3DVec3_2 vec;
		vec.m_position = m_position * other;
		vec.m_tangent = m_tangent * other;
		return vec;
	}
	/*H3DVec3_2 operator * (H3DVec3_2& other)
	{
		H3DVec3_2 vec;
		vec.m_position = m_position * other.m_position;
		vec.m_tangent = m_tangent * other.m_tangent;
		return vec;
	}*/
};

namespace GameGraphicHelper
{
	std::string to_lower(const std::string& str);
	std::string xml2Str(BiXmlElement* xml);
	
	template<typename T>
	T str2Value(const std::string& v)
	{
		T t = T();
		return t;
	}

	template<typename T>
	std::string value2Str(const T& value)
	{
		return "";
	}

	template<typename T>
	T byte2Value(const BYTE* v, int length)
	{
		T t = T();
		return t;
	}

	template<typename T>
	BYTE* value2Byte(const T& value, int& length)
	{
		return NULL;
	}

//////////////////////////////////////////////////////////////////////////
	template<>
	int str2Value<int>(const std::string& v);
	//{
	//	return atoi(v.c_str());
	//}

	template<>
	std::string value2Str<int>(const int& value);
// 	{
// 		char buf[20] = {0};
// 		itoa(value, buf, 10);
// 		return buf;
// 	}

//////////////////////////////////////////////////////////////////////////
	template<>
	float str2Value<float>(const std::string& v);
	//{
	//	return (float)atof(v.c_str());
	//}

	template<>
	std::string value2Str<float>(const float& value);
	//{
	//	char buf[30] = {0};
	//	sprintf(buf, "%f", value);
	//	return buf;	
	//}

//////////////////////////////////////////////////////////////////////////
	template<>
	bool str2Value<bool>(const std::string& v);
	//{
	//	if (stricmp(v.c_str(), "true") == 0)
	//	{
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}

	template<>
	std::string value2Str<bool>(const bool& value);
	//{
	//	if (value)
	//	{
	//		return "true";
	//	}

	//	return "false";
	//}

//////////////////////////////////////////////////////////////////////////
	template<>
	std::string str2Value<std::string>(const std::string& v);
// 	{
// 		return v;
// 	}

	template<>
	std::string value2Str<std::string>(const std::string& value);
// 	{
// 		return value;
// 	}

//////////////////////////////////////////////////////////////////////////
	template<>
	H3DVec3 str2Value<H3DVec3>(const std::string& v);
// 	{
// 		H3DVec3 vec;
// 		sscanf(v.c_str(), "{%f, %f, %f}", &(vec.x), &(vec.y), &(vec.z));
// 		return vec;
// 	}

	template<>
	std::string value2Str<H3DVec3>(const H3DVec3& value);
// 	{
// 		char buf[100] = {0};
// 		sprintf(buf, "{%f,%f,%f}", value.x, value.y, value.z);
// 		return buf;
// 	}

//////////////////////////////////////////////////////////////////////////
	template<>
	H3DColor str2Value<H3DColor>(const std::string& v);
// 	{
// 		float buf[4] = {0.0f};
// 		sscanf(v.c_str(), "{%f, %f, %f, %f}", &(buf[0]), &(buf[1]), &(buf[2]), &(buf[3]));
// 		H3DColor parameter = buf;
// 		return parameter;
// 	}

	template<>
	std::string value2Str<H3DColor>(const H3DColor& value);
// 	{
// 		char buf[100] = {0};
// 		sprintf(buf, "{%f,%f,%f,%f}", value.getRed(), value.getGreen(), value.getBlue(), value.getAlpha());
// 		return buf;		
// 	}

//////////////////////////////////////////////////////////////////////////
	template<>
	H3DVec4 str2Value<H3DVec4>(const std::string& v);

	template<>
	std::string value2Str<H3DVec4>(const H3DVec4& value);

//////////////////////////////////////////////////////////////////////////
	template<>
	H3DMat4 str2Value<H3DMat4>(const std::string& v);

	template<>
	std::string value2Str<H3DMat4>(const H3DMat4& value);

//////////////////////////////////////////////////////////////////////////
	template<>
	H3DMat3 str2Value<H3DMat3>(const std::string& v);

	template<>
	std::string value2Str<H3DMat3>(const H3DMat3& value);

//////////////////////////////////////////////////////////////////////////
	template<>
	H3DI::EH3DBEAMTYPE str2Value<H3DI::EH3DBEAMTYPE>(const std::string& v);

	template<>
	std::string value2Str<H3DI::EH3DBEAMTYPE>(const H3DI::EH3DBEAMTYPE& value);	

//////////////////////////////////////////////////////////////////////////
	template<>
	SmoothType str2Value<SmoothType>(const std::string& v);

	template<>
	std::string value2Str<SmoothType>(const SmoothType& value);	

	template<>
	TargetType str2Value<TargetType>(const std::string& v);

	template<>
	std::string value2Str<TargetType>(const TargetType& value);	

//////////////////////////////////////////////////////////////////////////
	template<>
	H3DI::LightAffectParam str2Value<H3DI::LightAffectParam>(const std::string& v);

	template<>
	std::string value2Str<H3DI::LightAffectParam>(const H3DI::LightAffectParam& value);	

//////////////////////////////////////////////////////////////////////////
	template<>
	H3DVec3_2 str2Value<H3DVec3_2>(const std::string& v);

	template<>
	std::string value2Str<H3DVec3_2>(const H3DVec3_2& value);	

//////////////////////////////////////////////////////////////////////////
	Actor* find_actor(Actor* actor, const ActorID& subactor_id);
}

#endif //_FUNCTION_PARAMETER_H_
