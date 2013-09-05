/**
* @file param_parse.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.5.27
*/

#include "game_graphic_body.h"

int CParamParse::divid_param_simple(const std::string& big_param, CParamParse::ParamList& params)
{
	if (big_param.length() == 0)
	{
		return EDIT_SPECIAL_ERROR_CODE_BASE;
	}

	int left_brace_count = 0;
	int start_index = 0;

	for (int i = 0; i < big_param.length(); ++i)
	{
		if ('{' == big_param[i])
		{
			left_brace_count++;
		}
		else if ('}' == big_param[i])
		{
			left_brace_count--;
			if (left_brace_count < 0)
			{
				return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
			}
		}

		if (',' == big_param[i] || ((big_param.length() - 1) == i))
		{
			if (0 == left_brace_count)
			{
				while(start_index < i)
				{
					if (' ' == big_param[start_index])
						start_index++;
					else
						break;
				}

				int length = (',' == big_param[i]) ? (i - start_index) : (length = i - start_index + 1);

				std::string param = big_param.substr(start_index, length);
				if (param.length() > 0)
				{
					if ('{' == param[0])
					{
						if ('}' == param[param.length() - 1])
						{
							param = param.substr(1, param.length() - 2);
						}
						else
						{
							return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
						}
					}
				}

				if (param.length() == 0)
				{
					return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
				}

				params.push_back(param);
				start_index = i + 1;
			}
		}	}

	return EDIT_SPECIAL_ERROR_SUCCESS;
}

std::string CParamParse::merge_params_simple(const CParamParse::ParamList& params)
{
	std::string str = "";
	for (int i = 0; i < params.size(); ++i)
	{
		if (i > 0)
		{
			str += ",";
		}

		if (is_exist_separator(params[i]))
		{
			str += "{";
			str += params[i];
			str += "}";
		}
		else
		{
			str += params[i];
		}
	}

	return str;
}

bool CParamParse::is_exist_separator(const std::string& str)
{
	int index = str.find(',', 0);
	if (std::string::npos != index)
	{
		return true;
	}

	return false;
}

int CParamParse::to_bool(const std::string& param, bool& bool_value)
{
	if (stricmp(param.c_str(), "true") == 0)
	{
		bool_value = true;
		return EDIT_SPECIAL_ERROR_SUCCESS;
	}
	else if (stricmp(param.c_str(), "false") == 0)
	{
		bool_value = false;
		return EDIT_SPECIAL_ERROR_SUCCESS;
	}

	return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
}

std::string CParamParse::bool_to_string(bool bool_value)
{
	if (bool_value)
	{
		return "true";
	}
	else
	{
		return "false";
	}
}

int CParamParse::to_float(const std::string& param, float& float_value)
{
	float_value = atof(param.c_str());
	return EDIT_SPECIAL_ERROR_SUCCESS;
}

std::string CParamParse::float_to_string(float float_value)
{
	char buf[100] = {0};
	sprintf(buf, "%f", float_value);
	return buf;
}

int CParamParse::to_int(const std::string& param, int& int_value)
{
	int_value = atoi(param.c_str());
	return EDIT_SPECIAL_ERROR_SUCCESS;
}

std::string CParamParse::int_to_string(int int_value)
{
	char buf[100] = {0};
	itoa(int_value, buf, 10);

	return buf;
}

int CParamParse::to_vec3(const std::string& param, H3DVec3& vec3_value)
{
	ParamList params;
	int n_ret = divid_param_simple(param, params);
	if (EDIT_SPECIAL_ERROR_SUCCESS != n_ret)
	{
		return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
	}

	if (params.size() != 3)
	{
		return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
	}

	float float_value[3] = {0};
	for (int i = 0; i < 3; ++i)
	{
		if (EDIT_SPECIAL_ERROR_SUCCESS != to_float(params[i], float_value[i]))
		{
			return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
		}
	}

	vec3_value = H3DVec3(float_value[0], float_value[1], float_value[2]);

	return EDIT_SPECIAL_ERROR_SUCCESS;
}

std::string CParamParse::vec3_to_string(const H3DVec3& vec3_value)
{
	ParamList params;
	const float* f = vec3_value.ToFloatPtr();

	for (int i = 0; i < 3; i++)
	{
		std::string str = float_to_string(f[i]);
		if (str.length() == 0)
		{
			return "";
		}

		params.push_back(str);
	}

	return merge_params_simple(params);
}

int CParamParse::to_H3DColor(const std::string& param, H3DColor& color_ret)
{
	ParamList params;
	int n_ret = divid_param_simple(param, params);
	if (EDIT_SPECIAL_ERROR_SUCCESS != n_ret)
	{
		return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
	}

	if (params.size() != 4 )
	{
		return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
	}

	float float_value[4] = {0};
	for (int i = 0; i < 4; ++i)
	{
		if (EDIT_SPECIAL_ERROR_SUCCESS != to_float(params[i], float_value[i]))
		{
			return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
		}
	}

	color_ret = H3DColor(float_value[0], float_value[1], float_value[2], float_value[3]);

	return EDIT_SPECIAL_ERROR_SUCCESS;
}

std::string CParamParse::H3DColor_to_string(const H3DColor& color_value)
{
	ParamList params;
	const float* f = color_value.getColor();

	for (int i = 0; i < 4; i++)
	{
		std::string str = float_to_string(f[i]);
		if (str.length() == 0)
		{
			return "";
		}

		params.push_back(str);
	}

	return merge_params_simple(params);
}

int CParamParse::to_mat44(const std::string& param, H3DMat4& mat_value)
{
	ParamList params;
	int n_ret = divid_param_simple(param, params);
	if (EDIT_SPECIAL_ERROR_SUCCESS != n_ret)
	{
		return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
	}

	if (params.size() != 16 )
	{
		return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
	}

	float float_value[16] = {0};
	for (int i = 0; i < 16; ++i)
	{
		if (EDIT_SPECIAL_ERROR_SUCCESS != to_float(params[i], float_value[i]))
		{
			return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
		}
	}

	mat_value = H3DMat4(float_value[0], float_value[1], float_value[2], float_value[3], float_value[4], float_value[5], float_value[6], float_value[7], float_value[8], float_value[9], float_value[10], float_value[11], float_value[12], float_value[13], float_value[14], float_value[15]);

	return EDIT_SPECIAL_ERROR_SUCCESS;
}

//int CParamParse::to_float_array(const std::string& param, FloatStruct& value)
//{
//	ParamList params;
//	int n_ret = divid_param_simple(param, params);
//	if (EDIT_SPECIAL_ERROR_SUCCESS != n_ret)
//	{
//		return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
//	}
//
//	for (int i = 0; i < params.size(); ++i)
//	{
//		if (EDIT_SPECIAL_ERROR_SUCCESS != to_float(params[i], value.m_f[i]))
//		{
//			return EDIT_SPECIAL_ERROR_INVALID_PARAMETERS;
//		}
//	}
//
//	value.m_size = params.size();
//
//	return EDIT_SPECIAL_ERROR_SUCCESS;
//}
//
//std::string  CParamParse::float_array_to_string(const FloatStruct& value)
//{
//	ParamList params;
//
//	for (int i = 0; i < value.m_size; i++)
//	{
//		std::string str = float_to_string(value.m_f[i]);
//		if (str.length() == 0)
//		{
//			return "";
//		}
//
//		params.push_back(str);
//	}
//
//	return merge_params_simple(params);
//}

bool StringHelper::divid_string(const std::string& source, const std::string& separator, StringHelper::StringList& sub_strs)
{
	int index = 0;
	int separator_length = separator.length();
	while(index < source.length())
	{
		int next_index = source.find_first_of(separator, index);
		if (std::string::npos == next_index)
		{
			sub_strs.push_back(source.substr(index));
			break;
		}

		sub_strs.push_back(source.substr(index, next_index - index));
		index = next_index + separator.length();
	}

	return true;
}
