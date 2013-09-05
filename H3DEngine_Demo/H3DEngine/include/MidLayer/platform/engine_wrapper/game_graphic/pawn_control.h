/**
* @file pawn_control.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.14
*/

#ifndef _PAWN_CONTROL_H_
#define _PAWN_CONTROL_H_

#include "game_graphic_header.h"

class PawnAnimationClip;
class IPawnControl;
class IPropertyModifier;
class IPropertyCurvModifierStreamable;

/**
* @brief
*		Pawn的通用动画控制器
* @function
*		1、更新动画
*		2、播放动画
*		3、管理动画：添加、删除、查找、遍历动画
* @comment
*		1、动画名称和PawnAnimationClip是一对多的关系
*/
class PawnAnimationControl : public IPawnControl
{
public:
	PawnAnimationControl();
	virtual ~PawnAnimationControl();
	// 设置需要修改的PAWN
	virtual void set_pawn(Pawn* pawn);
	//获取需要修改的PAWN
	virtual Pawn* get_pawn() = 0;
	// 更新
	virtual void update(int t);
	// 播放或停止动画控制器
	virtual void play(bool isplay, int anitime, PawnAnimationClip* aniclip);

	// 添加一个动画片段
	virtual bool add_animation_clip(const char* ani_name, PawnAnimationClip* aniclip);
	// 删除一个动画片段
	//virtual bool remove_animation_clip(PawnAnimationClip* aniclip);
	virtual bool remove_animation_clip(const char* ani_name);
	// 查找指定ID的动画片段
	virtual PawnAnimationClip* find_animation_clip(const char* ani_name);

	// 重置动画片段迭代器
	void reset_iterator();
	// 获得动画片段，迭代器指向下一个动画片段，如迭代器无效返回0
	PawnAnimationClip* next();

protected:
	Pawn*	m_pawn; //需要修改的pawn

	typedef std::map<std::string, PawnAnimationClip*> IDToPawnAnimationClipMap;
	IDToPawnAnimationClipMap m_ani_clip_collection; /// 所记录的动画片段容器
	IDToPawnAnimationClipMap::iterator m_cur_ani_clip_iterator; /// 当前动画片段的迭代器

	typedef std::set<PawnAnimationClip*> PawnAnimationClipCollection; 
	PawnAnimationClipCollection m_inplay_animation_clip_collection; /// 正在播放的动画片段
};

/// pawn的属性修改函数的类型ID
typedef std::string PawnPropertyTypeID;
/// pawn的类型ID
typedef std::string PawnTypeID;
/// 修改器调用的固定参数
typedef std::string ModifierFixParaInfo;
/// 子pawn的ID
typedef std::string SubPawnID;
/// 修改器的属性类型ID
struct ModifierTypeID
{
	PawnPropertyTypeID m_property_name; /// 所调用函数的类型ID
	PawnTypeID m_pawn_type_id; /// pawn的类型ID
	ModifierFixParaInfo m_fix_para_info; /// 修改器调用的固定参数
	SubPawnID m_sub_pawnid; /// 子pawn的ID
};


/**
* @brief
*		记录一个动画片段所需的属性修改器
* @function
*		1、属性修改器的管理：添加、删除、查找、遍历
*		2、播放时设置执行属性修改器
*		3、更新属性修改器
* @comment
*		1、属性修改器和属性是一一对应的
*/
class PawnAnimationClip
{
public:
	PawnAnimationClip();
	virtual ~PawnAnimationClip();

	// 添加属性修改器
	bool add_property_modifier(const ModifierTypeID& modifier_typeid, IPropertyModifier* modifier);
	// 移除属性修改器
	bool remove_property_modifier(const ModifierTypeID& modifier_typeid);
	// 查找指定类型ID的属性修改器
	IPropertyModifier* find_property_modifier(const ModifierTypeID& modifier_typeid);

	// 重置属性修改器迭代器
	void reset_iterator();
	// 获得属性修改器，迭代器指向下一个属性修改器，如迭代器无效返回0
	IPropertyModifier* next();

	// 播放或停止动画
	void play(bool isplay, int anitime);
	// 更新
	void update(int t);

protected:
	typedef std::map<ModifierTypeID, IPropertyModifier*> PropertyModifierCollection;
	PropertyModifierCollection m_property_modifier_collection;
	PropertyModifierCollection::iterator m_cur_property_modifier_itr;
};


// Pawn管理器
class PawnManager
{
public:
	// 添加子Pawn
	bool add_pawn(Pawn* pawn);
	// 移除子Pawn（不负责销毁）
	void remove_pawn(Pawn* pawn);
	// 查找指定名称的子封装实体
	Pawn* find_pawn(const char* pawn_name);
	// 重置Pawn迭代器
	void reset_iterator();
	// 获得Pawn，迭代器指向下一个Pawn，如迭代器无效返回0
	Pawn* next();
};


//剩下的设计，设计动画序列
//动画的序列的解析部分
//动画序列的更新部分
//动画解析
//目标：尽可能的灵活和代码重用
//输入：传入的字符串——xml/二进制
//输出：Frame-Property对
//详细：
//		1、解析包括简单曲线解析
//		2、解析包括复合曲线解析
//		3、解析总的来说是对几种数据类型的解析
//动画更新
//目标：当时间更新时，更新相关物体的属性
//输入：更新时间、Frame-Property对
//输出：物体属性发生变化
//详细：
//		1、属性除了编辑的Property还有一些相关信息
//		2、这些相关信息存在属性序列名称内

/**
* @brief
*		属性序列修改器
* @function
*		1、更新属性序列
*		2、解析属性序列
*/
template<typename ValueT, typename ApplierT, typename ParserT>
class PropertyCurvModifierStreamble : public IPropertyCurvModifierStreamable
{
public:
	PropertyCurvModifierStreamble(ApplierT* applier)
		: m_calculator(NULL), m_applier(applier)
	{NULL;}
	virtual ~PropertyCurvModifierStreamble(void)
	{
		delete m_calculator;
		delete m_calculator;
		delete m_applier;
	}

	// 更新
	virtual void update(float t)
	{
		if (NULL == m_calculator)
		{//如果不存在曲线
			return;
		}

		ValueT value;
		bool ret = m_calculator->calculate(t, value);
		if (!ret) //如果无法计算值或者不需要更新
		{
			return;
		}

		m_applier->apply(value); //应用值
	}

	// 通过字符串设置值
	virtual void set_value(const char* v)
	{
		ParserT			parser;
		m_calculator = parser.parse(v);
	}

private:
	ApplierT*				m_applier;
	ICalculator<ValueT>*	m_calculator;
};

/**
* @brief
*		曲线数据解析接口
*/
template<typename ValueT>
class ICurvParser
{
public:
	virtual ICalculator<ValueT>* parse(const char* v) = 0;
};

template<typename ValueT>
struct CurvNode
{
	float  m_t; //时间/帧数/或者其他
	ValueT m_value; //m_t对应的值
};

template<typename ValueT>
struct CurvNodes
{
	std::list<CurvNode<ValueT>* > m_curv_nodes;

public:
	void add_node(CurvNode<ValueT>* node)
	{
		m_curv_nodes.push_back(node);
	}
};

template<typename ValueT>
struct CurveData
{
	std::list<CurvNodes<ValueT>*> m_curves;
};

/**
* @brief
*		普通计算器
*/
template<typename ValueT>
class NormalCalculator : public ICalculator<ValueT>
{
public:
	NormalCalculator(CurvNodes<ValueT>* nodes) : m_curv_nodes(nodes) {}
	virtual ~NormalCalculator()
	{
		;
	}

	virtual bool calculate(float t, ValueT& value)
	{
		std::list<CurvNode<ValueT>* >::iterator it = m_curv_nodes->m_curv_nodes.begin();
		for (; it != m_curv_nodes->m_curv_nodes.end(); ++it)
		{
			if (t >= (*it)->m_t)
			{
				std::list<CurvNode<ValueT>* >::iterator tmp_it = it;
				tmp_it++;
				if (tmp_it ==  m_curv_nodes->m_curv_nodes.end())
				{
					value = (*it)->m_value;
					break;
				}
				if (t >= (*tmp_it)->m_t)
				{
					continue;
				}

				value = (ValueT)(((*tmp_it)->m_value - (*it)->m_value)* 1.0f * (t - (*it)->m_t) * 1.0f / ((*tmp_it)->m_t - (*it)->m_t) + (*it)->m_value);
				break;
			}
		}

		return true;
	}

private:
	CurvNodes<ValueT>* m_curv_nodes;
};

template<typename ValueT, typename ElementT>
void set_value(ValueT& value, const ElementT& elem, int index){}

/**
* @brief
*		复合曲线计算器
*/
template<typename ValueT, typename ChildValueT>
class CompositeCalculator : public ICalculator<ValueT>
{
public:
	CompositeCalculator(const std::list<ICalculator<ChildValueT>* >& calculates)
	{
		for (std::list<ICalculator<ChildValueT>* >::const_iterator it = calculates.begin(); it != calculates.end(); ++it)
		{
			m_calculates.push_back(*it);
		}
	}
	virtual ~CompositeCalculator()
	{
		for (std::list<ICalculator<ChildValueT>* >::iterator it = m_calculates.begin(); it != m_calculates.end(); )
		{
			ICalculator<ChildValueT>* q = *it++;
			delete q;
		}
	}

	virtual bool calculate(float t, ValueT& value)
	{
		int i = 0;
		for (std::list<ICalculator<ChildValueT>* >::iterator it = m_calculates.begin(); it != m_calculates.end(); ++it)
		{
			ChildValueT elem;
			if (!(*it)->calculate(t, elem))
			{
				return false;
			}

			set_value(value, elem, i++);
		}

		return true;
	}

private:
	std::list<ICalculator<ChildValueT>*> m_calculates;
};


/**
* @brief
*		插值方式
*/
enum InterpolatingType 
{
	INTERPOLATINGTYPE_POINT,
	INTERPOLATINGTYPE_LINE,
	INTERPOLATINGTYPE_BEZIER
};


/**
* @brief
*		曲线数据解析接口
* @comment
*	曲线格式为
<PropertySequence>
<PropertyNode InterPolatingType="" frame_index="", value="">
<PropertySequence />
<PropertySequence/>
</PropertySequence>
*/
template<typename ValueT>
class XMLNormalCurvParser : ICurvParser<ValueT>
{
public:
	virtual ICalculator<ValueT>* parse(const char* v)
	{
		TiXmlDocument* xml = new TiXmlDocument;
		xml->Parse(v);
		if(xml->Error()) 
		{
			delete xml;
			assert(false);
			return false;
		}

		ICalculator<ValueT>* calculator = parse(xml->FirstChild("PropertySequence"));

		delete xml;
		return calculator;
	}

	virtual ICalculator<ValueT>* parse(TiXmlNode* sequence)
	{
		if (NULL == sequence)
		{
			return NULL;
		}

		int interpolating_type = get_interpolating_type(sequence);
		CurvNodes<ValueT>* curv_nodes = create_curv_nodes(interpolating_type);

		TiXmlNode* property_node = sequence->FirstChild("PropertyNode");
		while(NULL != property_node)
		{
			CurvNode<ValueT>* node= parse_node(property_node, interpolating_type);
			curv_nodes->add_node(node);

			property_node = property_node->NextSibling("PropertyNode");
		}

		ICalculator<ValueT>* calculator = create_calculator(interpolating_type, curv_nodes);

		return calculator;
	}

private:
	virtual CurvNodes<ValueT>* create_curv_nodes(int inter_polating_type)
	{
		switch(inter_polating_type)
		{
			;
		}

		return new CurvNodes<ValueT>;
	}

	virtual CurvNode<ValueT>* parse_node(TiXmlNode* property_node, int inter_polating_type)
	{
		CurvNode<ValueT>* node = NULL;
		if (inter_polating_type == INTERPOLATINGTYPE_BEZIER)
		{
		}
		else
		{
			node = new CurvNode<ValueT>;
			const char* v =  TinyXmlUtility::get_attribute(property_node, "Frame");
			node->m_t = atof(v);
			v = TinyXmlUtility::get_attribute(property_node, "Value");
			node->m_value = atof(v);
		}

		return node;
	}

	virtual ICalculator<ValueT>* create_calculator(int inter_polating_type, CurvNodes<ValueT>* curv_nodes)
	{
		switch(inter_polating_type)
		{
		case INTERPOLATINGTYPE_BEZIER:
			break;
		case INTERPOLATINGTYPE_LINE:
			break;
		case INTERPOLATINGTYPE_POINT:
			break;
		default:
			break;
		}

		ICalculator<ValueT>* calculator = new NormalCalculator<ValueT>(curv_nodes);

		return calculator;
	}

	virtual int get_interpolating_type(TiXmlNode* sequence)
	{
		const char* inter_polating_type = TinyXmlUtility::get_attribute(sequence, "InterpretationType");
		if (stricmp(inter_polating_type,  "Bezier") == 0)
		{
			return INTERPOLATINGTYPE_BEZIER;
		}
		if (stricmp(inter_polating_type,  "Point") == 0)
		{
			return INTERPOLATINGTYPE_POINT;
		}
		if (stricmp(inter_polating_type,  "Line") == 0)
		{
			return INTERPOLATINGTYPE_LINE;
		}
	}
};

template<typename ValueT, typename ChildValueT>
class XMLCompositeCurvParser : ICurvParser<ValueT>
{
public:
	virtual ICalculator<ValueT>* parse(const char* v)
	{
		TiXmlDocument* xml = new TiXmlDocument;
		xml->Parse(v);
		if(xml->Error()) 
		{
			delete xml;
			assert(false);
			return false;
		}

		XMLNormalCurvParser<ChildValueT> curv_parser;
		std::list<ICalculator<ChildValueT>*> calculator_collection;

		TiXmlNode* child_node = xml->FirstChild("PropertySequence")->FirstChild("PropertySequence");
		while(child_node)
		{
			ICalculator<ChildValueT>* calculator = curv_parser.parse(child_node);
			if (calculator != NULL)
			{
				calculator_collection.push_back(calculator);
			}

			child_node = child_node->NextSibling("PropertySequence");
		}

		ICalculator<ValueT>* calculator = new CompositeCalculator<ValueT, ChildValueT>(calculator_collection);

		delete xml;
		return calculator;
	}
};

template<typename ValueT, typename OwnerT>
class Applier
{
public:
	virtual void apply(const ValueT& value) = 0;
	virtual void set_owner(OwnerT* owner)
	{
		m_owner = owner;
	}

protected:
	OwnerT*		m_owner;
};

template<typename ValueT, typename OwnerT,  typename TResult>
class ApplierWithSingleParameter : public Applier<ValueT, OwnerT>
{
public:
	ApplierWithSingleParameter(TResult(OwnerT::*pfunc)(ValueT))
	{
		m_pfunc = pfunc;
	}
	virtual void apply(const ValueT& value)
	{
		(m_owner->*m_pfunc)(value);
	}

protected:
	TResult(OwnerT::*m_pfunc)(ValueT);
};

template<typename ValueT, typename OwnerT,  typename TResult>
class ApplierWithSingleParameterConst : public Applier<ValueT, OwnerT>
{
public:
	ApplierWithSingleParameterConst(TResult(OwnerT::*pfunc)(const ValueT&))
	{
		m_pfunc = pfunc;
	}
	virtual void apply(const ValueT& value)
	{
		(m_owner->*m_pfunc)(value);
	}

protected:
	TResult(OwnerT::*m_pfunc)(const ValueT&);
};

//template<typename ValueT, typename OwnerT,  typename TResult, typename Value2T>
//class ApplierTwoParameteres : public Applier<ValueT, OwnerT>
//{
//public:
//	ApplierSingleParameter(TResult(OwnerT::*pfunc)(ValueT))
//	{
//		m_pfunc = pfunc;
//	}
//	virtual void apply(const ValueT& value)
//	{
//		(m_owner->*m_pfunc)(value);
//	}
//
//protected:
//	TResult(OwnerT::*m_pfunc)(ValueT, Value2T);
//};

/************************************************************************/
/* 
Applier的类型定义部分
*/
/************************************************************************/

/************************************************************************/
/* 
set_value的偏特化声明部分
*/
/************************************************************************/
//H3DVec3的偏特化
void set_value(H3DVec3& value, const float& elem, int index);
//H3DColor的偏特化
void set_value(H3DColor& value, const float& elem, int index);

#endif //_PAWN_CONTROL_H_
