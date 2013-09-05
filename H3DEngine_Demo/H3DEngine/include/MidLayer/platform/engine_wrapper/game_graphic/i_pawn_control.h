/**
* @file i_pawn_control.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.11.9
*/

#ifndef _I_PAWN_CONTROL_H_
#define _I_PAWN_CONTROL_H_

#include "game_graphic_header.h"

class Pawn;
class IPropertyModifier;

/**
* @brief
*		IPawnControl�����Pawn�Ŀ���
* @function
*		1������Pawn����������������
*/
class IPawnControl
{
public:
	virtual ~IPawnControl(){}
	// ������Ҫ�޸ĵ�PAWN
	virtual void set_pawn(Pawn* pawn) = 0;
	//��ȡ��Ҫ�޸ĵ�PAWN
	virtual Pawn* get_pawn() = 0;
	// ����
	virtual void update(int t) = 0;
};

/**
* @brief
*		�޸�Pawn��MediaEntity������
* @function
*		1���趨����ֵ
*		2����������ֵ
*/

class IPropertyModifier
{
public:
	virtual ~IPropertyModifier() {}
	// ִ��
	//virtual void execute() = 0;
	virtual void execute() {};
	// ����
	//virtual void update(float t) = 0;
	virtual void update(float t) {};
};


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
	// ��ñ�ʾֵ���ַ���
	//virtual const char* get_value() = 0;
	virtual const char* get_value(){ return "";};
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
	// ���tʱ������ֵ���ַ��� t�ĵ�λΪ����
	//virtual const char* get_value(float t) = 0;
	virtual const char* get_value(float t) {return"";};
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


// ͨ���������Ч�ļ��ٽ�����
// һ�����Ӧһ���࣬Ϊÿ���ຯ������һ��foo
// ��Ҫ����ʵ���䴴����
class IPropertyModifierCreator
{
public:
	virtual ~IPropertyModifierCreator(){}
	virtual IPropertyModifier* create(const std::string& fixparas) = 0;
};

/**
* @brief
*		�������ӿ�
*/
template<typename ValueT>
class ICalculator
{
public:
	virtual bool calculate(float t, ValueT& value) = 0;
};
#endif //_I_PAWN_CONTROL_H_
