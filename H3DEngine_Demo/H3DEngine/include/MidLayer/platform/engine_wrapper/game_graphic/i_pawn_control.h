/**
* @file i_pawn_control.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.9
*/

#ifndef _I_PAWN_CONTROL_H_
#define _I_PAWN_CONTROL_H_

#include "game_graphic_header.h"

class Pawn;
class IPropertyModifier;

/**
* @brief
*		IPawnControl负责对Pawn的控制
* @function
*		1、更新Pawn及其相关物体的属性
*/
class IPawnControl
{
public:
	virtual ~IPawnControl(){}
	// 设置需要修改的PAWN
	virtual void set_pawn(Pawn* pawn) = 0;
	//获取需要修改的PAWN
	virtual Pawn* get_pawn() = 0;
	// 更新
	virtual void update(int t) = 0;
};

/**
* @brief
*		修改Pawn或MediaEntity的属性
* @function
*		1、设定属性值
*		2、更新属性值
*/

class IPropertyModifier
{
public:
	virtual ~IPropertyModifier() {}
	// 执行
	//virtual void execute() = 0;
	virtual void execute() {};
	// 更新
	//virtual void update(float t) = 0;
	virtual void update(float t) {};
};


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
	// 获得表示值的字符串
	//virtual const char* get_value() = 0;
	virtual const char* get_value(){ return "";};
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
	// 获得t时刻属性值的字符串 t的单位为毫秒
	//virtual const char* get_value(float t) = 0;
	virtual const char* get_value(float t) {return"";};
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


// 通过宏可以有效的减少解析量
// 一个宏对应一个类，为每个类函数调用一次foo
// 主要就是实现其创建器
class IPropertyModifierCreator
{
public:
	virtual ~IPropertyModifierCreator(){}
	virtual IPropertyModifier* create(const std::string& fixparas) = 0;
};

/**
* @brief
*		计算器接口
*/
template<typename ValueT>
class ICalculator
{
public:
	virtual bool calculate(float t, ValueT& value) = 0;
};
#endif //_I_PAWN_CONTROL_H_
