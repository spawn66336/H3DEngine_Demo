/**
* @file param_parse.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.5.27
*/

#ifndef _PARAM_PARSE_H_
#define _PARAM_PARSE_H_

#include "game_graphic_header.h"

class CParamParse
{
public:
	typedef std::vector<std::string> ParamList;

	/**
	* @brief
	*		���������г��λ��֣��з�Ϊ����С����
	*/
	int divid_param_simple(const std::string& big_param, ParamList& params);

	/**
	* @brief
	*		������С�Ĳ����ַ����ϳ�Ϊһ������ַ���
	*/
	std::string merge_params_simple(const ParamList& params);

	/**
	* @brief
	*		������ת��Ϊһ��H3DVec3���͵Ĳ���
	*/
	int to_vec3(const std::string& param, H3DVec3& vec3_value);

	/**
	* @brief
	*		��һ��H3DVec3���͵Ĳ���ת��Ϊ�ַ���
	*/
	std::string vec3_to_string(const H3DVec3& vec3_value);

	/**
	* @brief
	*		������ת��Ϊһ��bool���͵Ĳ���
	*/
	int to_bool(const std::string& param, bool& bool_value);

	/**
	* @brief
	*		��һ��bool���͵Ĳ���ת��Ϊ�ַ���
	*/
	std::string bool_to_string(bool bool_value);

	/**
	* @brief
	*		��һ���ַ���ת��Ϊfloat���͵Ĳ���
	*/
	int to_float(const std::string& param, float& float_value);

	/**
	* @brief
	*		��һ��float���͵Ĳ���ת��Ϊ�ַ���
	*/
	std::string float_to_string(float float_value);


	/**
	* @brief
	*		������ת��Ϊһ��int���͵Ĳ���
	*/
	int to_int(const std::string& param, int& int_value);

	/**
	* @brief
	*		��һ��int���͵Ĳ���ת��Ϊ�ַ���
	*/
	std::string int_to_string(int int_value);


	/**
	* @brief
	*		������ת��Ϊһ��H3DColor���͵Ĳ���
	*/
	int to_H3DColor(const std::string& param, H3DColor& color_ret);

	/**
	* @brief
	*		����H3DColor���͵Ĳ���ת��Ϊ�ַ���
	*/
	std::string H3DColor_to_string(const H3DColor& color_ret);

	/**
	* @brief
	*		��һ���ַ���ת��ΪH3dmat4���͵Ĳ���
	*/
	int to_mat44(const std::string& param, H3DMat4& mat_value);

	///**
	//* @brief
	//*		��һ���ַ���ת��Ϊfloat_array���͵Ĳ���
	//*/
	//int to_float_array(const std::string& param, FloatStruct& value);
	//
	///**
	//* @brief
	//*		��һ��float_array���͵Ĳ���ת��Ϊ�ַ���
	//*/
	//std::string  float_array_to_string(const FloatStruct& value);
private:
	/**
	* @brief
	*		�ж��ַ������Ƿ���ڷָ���
	*/
	bool is_exist_separator(const std::string& str);
};

class StringHelper
{
public:
	typedef std::vector<std::string> StringList;

	static bool divid_string(const std::string& source, const std::string& separator, StringList& sub_strs);
};

#endif //_PARAM_PARSE_H_
