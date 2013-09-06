/**
* @file error_code.h
* 这个文件的主要功能是： 错误代码定义文件
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       编码完成 by 程陈2009.12.28
*/

#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

#include "game_graphic_header.h"


//
// MessageId: ERROR_CLIENT_FAILED
//
// MessageText:
//
// The operation is failed.
//
#define ERROR_CLIENT_FAILED                    -1L

//
// MessageId: ERROR_CLIENT_BASE
//
// MessageText:
//
// The base of the error code for this client.
//
#define ERROR_CLIENT_BASE                    20000L


//
// MessageId: ERROR_CLIENT_SUCCESS
//
// MessageText:
//
// The operation completed successfully.
//
#define ERROR_CLIENT_SUCCESS                    0L


//
// MessageId: ERROR_NO_MORE_PROCESS
//
// MessageText:
//
// The operation needs no more operation now.
//
#define ERROR_NO_MORE_PROCESS                    ERROR_CLIENT_BASE + 1


//
// MessageId: ERROR_NEED_MORE_PROCESS
//
// MessageText:
//
// The operation needs no more operation now.
//
#define ERROR_NEED_MORE_PROCESS                    ERROR_CLIENT_BASE + 2

//
// MessageId: ERROR_ENTITY_ALREADY_EXIST
//
// MessageText:
//
// 要添加的可控实体已经存在.
//
#define ERROR_ENTITY_ALREADY_EXIST                    ERROR_CLIENT_BASE + 3

//
// MessageId: ERROR_NAME_CONFICT
//
// MessageText:
//
// 名字发生了冲突了。
//
#define ERROR_NAME_CONFICT                    ERROR_CLIENT_BASE + 4

/**
* @brief
*		错误码基值
*/
const int EDIT_SPECIAL_ERROR_CODE_BASE = 10001;

/**
* @brief
*		成功
*/
const int EDIT_SPECIAL_ERROR_SUCCESS = EDIT_SPECIAL_ERROR_CODE_BASE;

/**
* @brief
*		未知错误
*/
const int EDIT_SPECIAL_ERROR_UNKNOWN = EDIT_SPECIAL_ERROR_CODE_BASE + 1;

/**
* @brief
*		不支持的可控实体类型
*/
const int EDIT_SPECIAL_ERROR_UNSUPPORT_ENTITY = EDIT_SPECIAL_ERROR_UNKNOWN + 1;

/**
* @brief
*		不支持的属性
*/
const int EDIT_SPECIAL_ERROR_UNSUPPORT_ATTRIBUTE = EDIT_SPECIAL_ERROR_UNSUPPORT_ENTITY + 1;

/**
* @brief
*		参数不正确
*/
const int EDIT_SPECIAL_ERROR_INVALID_PARAMETERS = EDIT_SPECIAL_ERROR_UNSUPPORT_ATTRIBUTE + 1;


#endif //_ERROR_CODE_H_
