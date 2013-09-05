/**
* @file error_code.h
* ����ļ�����Ҫ�����ǣ� ������붨���ļ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       ������� by �̳�2009.12.28
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
// Ҫ��ӵĿɿ�ʵ���Ѿ�����.
//
#define ERROR_ENTITY_ALREADY_EXIST                    ERROR_CLIENT_BASE + 3

//
// MessageId: ERROR_NAME_CONFICT
//
// MessageText:
//
// ���ַ����˳�ͻ�ˡ�
//
#define ERROR_NAME_CONFICT                    ERROR_CLIENT_BASE + 4

/**
* @brief
*		�������ֵ
*/
const int EDIT_SPECIAL_ERROR_CODE_BASE = 10001;

/**
* @brief
*		�ɹ�
*/
const int EDIT_SPECIAL_ERROR_SUCCESS = EDIT_SPECIAL_ERROR_CODE_BASE;

/**
* @brief
*		δ֪����
*/
const int EDIT_SPECIAL_ERROR_UNKNOWN = EDIT_SPECIAL_ERROR_CODE_BASE + 1;

/**
* @brief
*		��֧�ֵĿɿ�ʵ������
*/
const int EDIT_SPECIAL_ERROR_UNSUPPORT_ENTITY = EDIT_SPECIAL_ERROR_UNKNOWN + 1;

/**
* @brief
*		��֧�ֵ�����
*/
const int EDIT_SPECIAL_ERROR_UNSUPPORT_ATTRIBUTE = EDIT_SPECIAL_ERROR_UNSUPPORT_ENTITY + 1;

/**
* @brief
*		��������ȷ
*/
const int EDIT_SPECIAL_ERROR_INVALID_PARAMETERS = EDIT_SPECIAL_ERROR_UNSUPPORT_ATTRIBUTE + 1;


#endif //_ERROR_CODE_H_
