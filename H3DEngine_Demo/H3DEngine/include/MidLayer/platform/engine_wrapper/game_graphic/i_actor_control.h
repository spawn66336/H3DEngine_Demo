/**
* @file i_Actor_control.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.9
*/

#ifndef _I_ACTOR_CONTROL_H_
#define _I_ACTOR_CONTROL_H_

#include "game_graphic_header.h"

class Actor;
class IPropertyModifier;
class ICalculator;

/**
* @brief
*		IActorControl负责对Actor的控制
* @function
*		1、更新Actor及其相关物体的属性
*/
class IActorControl
{
public:
	virtual ~IActorControl(){}

	// 设置需要修改的Actor
	virtual void set_actor(Actor* actor) = 0;
	//获取需要修改的Actor
	virtual Actor* get_actor() = 0;
	// 更新
	virtual void update(float t) = 0;
	
	//获取名称
	virtual const char* get_name() const = 0;
	//设置名称
	virtual void set_name(const char* name) = 0;

	//是否负责actor的update
	virtual bool in_charge_of_actor_update() = 0;
};

/**
* @brief
*		修改Actor或MediaEntity的属性
* @function
*		1、设定属性值
*		2、更新属性值
*/

class IPropertyModifier
{
public:
	virtual ~IPropertyModifier() {}
	// 执行
	virtual void execute(bool isExcute, float t) = 0;
	// 更新
	virtual void update(float t) = 0;

	virtual int type() = 0;
};

typedef unsigned char BYTE;

/**
* @brief
*		可以通过字符串设置、获得属性值的属性修改器
* @function
*		1、传入字符串，解析的值
*		2、获取属性值的字符串
*/
class IPropertyModifierStreamable : public IPropertyModifier
{
public:
	virtual ~IPropertyModifierStreamable(){}

	// 通过字符串设置值
	virtual void set_value(const char* v) = 0;
	//设置二进制的值
	virtual void set_value(const BYTE* v, int length) = 0;

	// 获得表示值的字符串
	virtual const char* get_value() = 0;
	//获取二进制的值
	virtual const BYTE* get_value(int& length) = 0;
};

/**
* @brief
*		属性序列修改器
* @function
*		1、更新属性序列
*		2、解析属性序列
* @comment
*		1、execute不做任何操作
*/
class IPropertyCurvModifierStreamable : public IPropertyModifierStreamable
{
public:
	virtual ~IPropertyCurvModifierStreamable(){}

	//设置是否启用
	virtual void set_enable(bool enable) = 0;
	//判断是否启用
	virtual bool get_enable() = 0;

	virtual ICalculator* get_calculator() = 0;
};

/**
* @brief
*		IPropertyModifier的抽象工厂
* @function
*		1、根据属性名称，创建对应的属性修改器
*		2、根据属性名称，创建对应的属性曲线修改器
*		3、根据传入的内容，创建属性曲线修改器
*/
class IPropertyModifierFactory
{
public:
	virtual ~IPropertyModifierFactory(){}
	//创建对应的属性Modifier
	virtual IPropertyModifierStreamable* create_property_modifier(const char* property_name) = 0;
	//创建对应的属性序列Modifier
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

// 通过宏可以有效的减少解析量
#endif //_I_ACTOR_CONTROL_H_
