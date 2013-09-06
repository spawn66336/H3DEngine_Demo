/**
* @file i_Actor_control.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.11.9
*/

#ifndef _I_ACTOR_CONTROL_H_
#define _I_ACTOR_CONTROL_H_

#include "game_graphic_header.h"

class Actor;
class IPropertyModifier;
class ICalculator;

/**
* @brief
*		IActorControl�����Actor�Ŀ���
* @function
*		1������Actor����������������
*/
class IActorControl
{
public:
	virtual ~IActorControl(){}

	// ������Ҫ�޸ĵ�Actor
	virtual void set_actor(Actor* actor) = 0;
	//��ȡ��Ҫ�޸ĵ�Actor
	virtual Actor* get_actor() = 0;
	// ����
	virtual void update(float t) = 0;
	
	//��ȡ����
	virtual const char* get_name() const = 0;
	//��������
	virtual void set_name(const char* name) = 0;

	//�Ƿ���actor��update
	virtual bool in_charge_of_actor_update() = 0;
};

/**
* @brief
*		�޸�Actor��MediaEntity������
* @function
*		1���趨����ֵ
*		2����������ֵ
*/

class IPropertyModifier
{
public:
	virtual ~IPropertyModifier() {}
	// ִ��
	virtual void execute(bool isExcute, float t) = 0;
	// ����
	virtual void update(float t) = 0;

	virtual int type() = 0;
};

typedef unsigned char BYTE;

/**
* @brief
*		����ͨ���ַ������á��������ֵ�������޸���
* @function
*		1�������ַ�����������ֵ
*		2����ȡ����ֵ���ַ���
*/
class IPropertyModifierStreamable : public IPropertyModifier
{
public:
	virtual ~IPropertyModifierStreamable(){}

	// ͨ���ַ�������ֵ
	virtual void set_value(const char* v) = 0;
	//���ö����Ƶ�ֵ
	virtual void set_value(const BYTE* v, int length) = 0;

	// ��ñ�ʾֵ���ַ���
	virtual const char* get_value() = 0;
	//��ȡ�����Ƶ�ֵ
	virtual const BYTE* get_value(int& length) = 0;
};

/**
* @brief
*		���������޸���
* @function
*		1��������������
*		2��������������
* @comment
*		1��execute�����κβ���
*/
class IPropertyCurvModifierStreamable : public IPropertyModifierStreamable
{
public:
	virtual ~IPropertyCurvModifierStreamable(){}

	//�����Ƿ�����
	virtual void set_enable(bool enable) = 0;
	//�ж��Ƿ�����
	virtual bool get_enable() = 0;

	virtual ICalculator* get_calculator() = 0;
};

/**
* @brief
*		IPropertyModifier�ĳ��󹤳�
* @function
*		1�������������ƣ�������Ӧ�������޸���
*		2�������������ƣ�������Ӧ�����������޸���
*		3�����ݴ�������ݣ��������������޸���
*/
class IPropertyModifierFactory
{
public:
	virtual ~IPropertyModifierFactory(){}
	//������Ӧ������Modifier
	virtual IPropertyModifierStreamable* create_property_modifier(const char* property_name) = 0;
	//������Ӧ����������Modifier
	virtual IPropertyCurvModifierStreamable* create_property_curv_modifier(const char* property_name) = 0;
};

class ICalculator
{
public:
	virtual ~ICalculator() {NULL;}
	virtual int get_interpolating_type(int sub_index) = 0;
	virtual int get_key_frames(int sub_index, float frames[], int& buffer_size) = 0;
	virtual const char* get_interpolating_value(int sub_index, float t) = 0;
	virtual void simplify() = 0;
	virtual void set_key_frame_value(int sub_index, float frame, const char* value) = 0;
};

// ͨ���������Ч�ļ��ٽ�����
#endif //_I_ACTOR_CONTROL_H_
