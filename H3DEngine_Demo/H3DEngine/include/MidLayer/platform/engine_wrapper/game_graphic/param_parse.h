/**
* @file param_parse.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.5.27
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
	*		将参数进行初次划分，切分为几个小参数
	*/
	int divid_param_simple(const std::string& big_param, ParamList& params);

	/**
	* @brief
	*		将几个小的参数字符串合成为一个大的字符串
	*/
	std::string merge_params_simple(const ParamList& params);

	/**
	* @brief
	*		将参数转换为一个H3DVec3类型的参数
	*/
	int to_vec3(const std::string& param, H3DVec3& vec3_value);

	/**
	* @brief
	*		将一个H3DVec3类型的参数转换为字符串
	*/
	std::string vec3_to_string(const H3DVec3& vec3_value);

	/**
	* @brief
	*		将参数转换为一个bool类型的参数
	*/
	int to_bool(const std::string& param, bool& bool_value);

	/**
	* @brief
	*		将一个bool类型的参数转换为字符串
	*/
	std::string bool_to_string(bool bool_value);

	/**
	* @brief
	*		将一个字符串转换为float类型的参数
	*/
	int to_float(const std::string& param, float& float_value);

	/**
	* @brief
	*		将一个float类型的参数转换为字符串
	*/
	std::string float_to_string(float float_value);


	/**
	* @brief
	*		将参数转换为一个int类型的参数
	*/
	int to_int(const std::string& param, int& int_value);

	/**
	* @brief
	*		将一个int类型的参数转换为字符串
	*/
	std::string int_to_string(int int_value);


	/**
	* @brief
	*		将参数转换为一个H3DColor类型的参数
	*/
	int to_H3DColor(const std::string& param, H3DColor& color_ret);

	/**
	* @brief
	*		将个H3DColor类型的参数转换为字符串
	*/
	std::string H3DColor_to_string(const H3DColor& color_ret);

	/**
	* @brief
	*		将一个字符串转换为H3dmat4类型的参数
	*/
	int to_mat44(const std::string& param, H3DMat4& mat_value);

	///**
	//* @brief
	//*		将一个字符串转换为float_array类型的参数
	//*/
	//int to_float_array(const std::string& param, FloatStruct& value);
	//
	///**
	//* @brief
	//*		将一个float_array类型的参数转换为字符串
	//*/
	//std::string  float_array_to_string(const FloatStruct& value);
private:
	/**
	* @brief
	*		判断字符串内是否存在分隔符
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
