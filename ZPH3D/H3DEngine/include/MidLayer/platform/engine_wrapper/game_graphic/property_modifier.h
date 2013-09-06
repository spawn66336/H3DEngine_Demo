/**
* @file property_modifier.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.17
*/

#ifndef _PROPERTY_MODIFIER_H_
#define _PROPERTY_MODIFIER_H_

#include "game_graphic_header.h"
#include "function_parameter.h"

class IPropertyCurvModifierStreamable;
struct CurvData;
static float s_DeltT = 0.01f;

/**
* @brief
*		插值方式
*/
enum InterpolatingType 
{
	INTERPOLATINGTYPE_POINT,
	INTERPOLATINGTYPE_LINE,
	INTERPOLATINGTYPE_BEZIER,
	INTERPOLATINGTYPE_UNKNOWN,
};

std::string InterpolatingType2Str(int type);

template<typename ValueT, typename ElementT>
void set_value(ValueT& value, const ElementT& elem, int index){}

/**
* @brief
*		计算器接口
*/
template<typename ValueT>
class CalculatorT : public ICalculator
{
public:
	virtual bool calculate(float t, ValueT& value, float* time_on_curve = NULL) = 0;
	virtual CurvData* get_curv_data() = 0;
};

/************************************************************************/
/* 
物体属性修改器
*/
/************************************************************************/
/// 修改器实现类，一次性修改
template<typename TClass, typename ApplierT, typename TPara0>
class PropertyModifierUsingTemplate : public IPropertyModifierStreamable
{
public:
	PropertyModifierUsingTemplate(TClass* owner, ApplierT* applier, int property_type)
		: m_applier(applier), m_property_type(MEDIA_PROPERTY_TYPE_INVALID)
	{
		m_owner = owner;
		applier->set_owner(owner);
		m_property_type = property_type;
	}
	virtual ~PropertyModifierUsingTemplate()
	{
		delete m_applier;
	}

	virtual int type()
	{
		return m_property_type;
	}

	TClass* get_owner()
	{
		return m_owner;
	}

	virtual void execute(bool isExcute, float t)
	{
		if (isExcute)
		{
			m_applier->apply(m_value);
		}
	}

	// 更新
	virtual void update(float t) {};
	// 通过字符串设置值
	virtual void set_value(const char* v) 
	{
		if ((m_input_value.has_string()) && (_stricmp(v, m_input_value.get_str_value().c_str()) == 0))
		{
			return ;
		}

		m_value = GameGraphicHelper::str2Value<TPara0>(v);
#ifdef _EDITOR
		m_input_value.set_value(v);
#endif
	};
	
	//设置二进制的值
	virtual void set_value(const BYTE* v, int length)
	{
		m_value = GameGraphicHelper::byte2Value<TPara0>(v, length);
#ifdef _EDITOR
		m_input_value.set_value(v, length);
#endif
	}

	// 获得表示值的字符串
	virtual const char* get_value()
	{
		//if (m_input_value.has_string())
		{
			//return m_input_value.get_str_value().c_str();
		}
		
		std::string tmp = GameGraphicHelper::value2Str<TPara0>(m_value);
#ifdef _EDITOR
		m_input_value.set_value(tmp);
#endif	// _EDITOR
		return m_input_value.get_str_value().c_str();
	}

	//获取二进制的值
	virtual const BYTE* get_value(int& length)
	{
		if (!m_input_value.has_binary())
		{
			int len = 0;
			BYTE* tmp = GameGraphicHelper::value2Byte<TPara0>(m_value, len);
#ifdef _EDITOR
			if (tmp != NULL)
			{
				m_input_value.set_value(tmp, len);
			}
#endif
		}

		length = m_input_value.get_binary_length();
		return m_input_value.get_binary_value();
	};

	virtual ApplierT* get_applier()
	{
		return m_applier;
	}

protected:
	ApplierT*				m_applier;
	TClass*		m_owner;
	TPara0					m_value;
	ByteString				m_input_value;
	int			m_property_type;
};

/************************************************************************/
/* 
*/
/************************************************************************/
/**
* @brief
*		属性序列修改器
* @function
*		1、更新属性序列
*		2、解析属性序列
*/

template<typename ValueT>
struct CurvNode
{
	float  m_t; //时间/帧数/或者其他
	ValueT m_value; //m_t对应的值
};

template<typename ValueT>
struct BezierCurvNode : public CurvNode<ValueT>
{
	float  m_pre_t; //相近的前一点的时间/帧数/或者其他
	ValueT m_pre_value; //相近的前一点的m_t对应的值

	float  m_next_t; //相近的后一点的时间/帧数/或者其他
	ValueT m_next_value; //相近的后一点的m_t对应的值
};

template<typename ValueT>
struct CurvNodes
{
	typedef std::list<CurvNode<ValueT>* >  NodeCollection;

	NodeCollection m_nodes;

public:
	virtual ~CurvNodes()
	{
		for (NodeCollection::iterator it = m_nodes.begin(); it != m_nodes.end();)
		{
			delete (*it++);
		}
	}

	void add_node(CurvNode<ValueT>* node)
	{
		m_nodes.push_back(node);
	}
};

struct CurvData
{
	int curv_interpolating_type;

	typedef std::list<CurvData*> CurvCollection;
	CurvCollection	m_curves;

	virtual ~CurvData()
	{
		for (CurvCollection::iterator it = m_curves.begin(); it != m_curves.end();)
		{
			delete (*it++);
		}
	}

	void add_curv(CurvData* curv)
	{
		m_curves.push_back(curv);
	}
};

template<typename ValueT>
struct CurvDataT : public CurvData
{
	CurvNodes<ValueT>* m_nodes;
public:
	CurvDataT()
	{
		m_nodes = NULL;
	}
	virtual ~CurvDataT()
	{
		delete m_nodes;
	}
};

template<typename ValueT>
bool is_two_bezier_point_curv_line(BezierCurvNode<ValueT>* cur, BezierCurvNode<ValueT>* next)
{
	bool is_line = (cur->m_next_value == (ValueT)((next->m_value - cur->m_value)* (1.0f * (cur->m_next_t - cur->m_t) * 1.0f / (next->m_t - cur->m_t)) + cur->m_value));
	if (!is_line)
	{
		return false;
	}

	is_line = (cur->m_next_value == (ValueT)((next->m_value - cur->m_value)* (1.0f * (next->m_pre_t - cur->m_t) * 1.0f / (next->m_t - cur->m_t)) + cur->m_value));
	return is_line;
}

template<>
bool is_two_bezier_point_curv_line(BezierCurvNode<float>* cur, BezierCurvNode<float>* next);

template<typename TClass, typename ApplierT, typename SerializerT, typename ValueT>
class PropertyCurvModifierStreamble : public IPropertyCurvModifierStreamable
{
public:
	PropertyCurvModifierStreamble(TClass* owner, ApplierT* applier, int property_type)
		: m_calculator(NULL), m_applier(applier), m_curv_data(NULL), 
		m_owner(NULL), m_enable(true), m_property_type(MEDIA_PROPERTY_TYPE_INVALID)
	{
		m_owner = owner;
		applier->set_owner(owner);
		m_property_type = property_type;
	}

	virtual ~PropertyCurvModifierStreamble(void)
	{
		delete m_calculator;
		delete m_applier;
		delete m_curv_data;
	}

	virtual int type()
	{
		return m_property_type;
	}

	TClass* get_owner()
	{
		return m_owner;
	}

	//设置是否启用
	virtual void set_enable(bool enable)
	{
		m_enable = enable;
	}

	//判断是否启用
	virtual bool get_enable()
	{
		return m_enable;
	}

	// 更新
	virtual void update(float t)
	{
		if ((NULL == m_calculator) || !m_enable)
		{//如果不存在曲线,或者禁止更新
			return;
		}

		ValueT value;
		m_applier->get_value(value);
		bool ret = m_calculator->calculate(t, value);
		if (!ret) //如果无法计算值或者不需要更新
		{
			return;
		}

		m_applier->apply(value); //应用值
	}

	// 获得t时刻属性值的字符串 t的单位为毫秒
	virtual const char* get_interpolating_value(float t)
	{
		ValueT value;
		bool ret = m_calculator->calculate(t, value);
		if (!ret) //如果无法计算值或者不需要更新
		{
			return NULL;
		}

		m_interpolating_str = GameGraphicHelper::value2Str<ValueT>(value);

		return m_interpolating_str.c_str();
	}

	// 通过字符串设置值
	virtual void set_value(const char* v)
	{
		if ((m_curv_str.has_string()) && (_stricmp(v, m_curv_str.get_str_value().c_str()) == 0))
		{
			return ;
		}
		delete m_calculator;
		m_calculator = NULL;
		delete m_curv_data;
		m_curv_data = NULL;

		SerializerT			serializer;
		m_calculator = serializer.serialize(v);

		if(m_calculator == 0)
		{
			return;
		}
		else
		{
		}

		m_curv_str.set_value(v);
	}

	//设置二进制的值
	virtual void set_value(const BYTE* v, int length)
	{
		m_curv_str.set_value(v, length);
	}

	// 获得表示值的字符串
	virtual const char* get_value()
	{
		if(!m_calculator)
			return 0;

		SerializerT			serializer;
		if (!m_curv_data)
			m_curv_data = m_calculator->get_curv_data();
		std::string str = serializer.curvData2Str(dynamic_cast<CurvDataT<ValueT>* >(m_curv_data));

		m_curv_str.set_value(str);

		return m_curv_str.get_str_value().c_str();
	}

	//获取二进制的值
	virtual const BYTE* get_value(int& length)
	{
		if (m_curv_str.has_binary())
		{
			int len = 0;
			SerializerT			serializer;
			BYTE* b = serializer.curvData2Byte(dynamic_cast<CurvDataT<ValueT>* >(m_curv_data), len);
			if (b != NULL)
			{
				m_curv_str.set_value(b, len);
				delete b;
			}
		}

		length = m_curv_str.get_binary_length();
		return m_curv_str.get_binary_value();

		return NULL;
	};

	virtual ApplierT* get_applier()
	{
		return m_applier;
	}

	virtual ICalculator* get_calculator()
	{
		return m_calculator;
	}

	// 执行
	virtual void execute(bool isExcute, float t) {};

protected:
	ApplierT*				m_applier;
	CalculatorT<ValueT>*	m_calculator;
	CurvData*				m_curv_data;
	ByteString				m_curv_str;

	TClass*					m_owner;

	bool					m_enable; //是否启用
	std::string				m_interpolating_str;
	int						m_property_type;
};

template<typename TClass, typename ApplierT, typename SerializerT, typename ValueT>
class PropertyCurvModifierStreambleFrame : public PropertyCurvModifierStreamble<TClass, ApplierT, SerializerT, ValueT>
{
public:
	PropertyCurvModifierStreambleFrame(TClass* owner, ApplierT* applier, int property_type)
		: PropertyCurvModifierStreamble<TClass, ApplierT, SerializerT, ValueT>(owner, applier, property_type)
	{NULL;}

	// 更新
	virtual void update(float t)
	{
		float frame = t / CSystemInfo::instance()->frame_time();
		PropertyCurvModifierStreamble<TClass, ApplierT, SerializerT, ValueT>::update(frame);
	}

	// 获得t时刻属性值的字符串 t的单位为毫秒
	virtual const char* get_interpolating_value(float t)
	{
		float frame = t / CSystemInfo::instance()->frame_time();
		return PropertyCurvModifierStreamble<TClass, ApplierT, SerializerT, ValueT>::get_interpolating_value(frame);
	}
};

template<typename TClass, typename ApplierT, typename SerializerT, typename ValueT>
class PropertyCurvModifierStreambleTimeRelated : public IPropertyCurvModifierStreamable
{
public:
	PropertyCurvModifierStreambleTimeRelated(TClass* owner, ApplierT* applier, int property_type)
		: m_calculator(NULL), m_applier(applier), m_curv_data(NULL), m_owner(NULL),
		m_enable(true), m_property_type(MEDIA_PROPERTY_TYPE_INVALID)
	{
		m_owner = owner;
		applier->set_owner(owner);
		m_property_type = property_type;
		m_last_curv_time = 0.0f;
	}
	virtual ~PropertyCurvModifierStreambleTimeRelated(void)
	{
		delete m_calculator;
		delete m_applier;
		delete m_curv_data;
	}

	virtual int type()
	{
		return m_property_type;
	}

	TClass* get_owner()
	{
		return m_owner;
	}

	//设置是否启用
	virtual void set_enable(bool enable)
	{
		m_enable = enable;
	}

	//判断是否启用
	virtual bool get_enable()
	{
		return m_enable;
	}

	// 更新
	virtual void update(float t)
	{
		//if ((NULL == m_calculator) || !m_enable)
		//{//如果不存在曲线,或者禁止更新
		//	return;
		//}

		//ValueT value;
		//m_applier->get_value(value);
		//bool ret = m_calculator->calculate(t, value);
		//if (!ret) //如果无法计算值或者不需要更新
		//{
		//	return;
		//}

		//m_applier->apply(value); //应用值
	}

	// 获得t时刻属性值的字符串 t的单位为毫秒
	virtual const char* get_interpolating_value(float t)
	{
		ValueT value;
		bool ret = m_calculator->calculate(t, value);
		if (!ret) //如果无法计算值或者不需要更新
		{
			return NULL;
		}

		m_interpolating_str = GameGraphicHelper::value2Str<ValueT>(value);

		return m_interpolating_str.c_str();
	}

	// 通过字符串设置值
	virtual void set_value(const char* v)
	{
		if ((m_curv_str.has_string()) && (stricmp(v, m_curv_str.get_str_value().c_str()) == 0))
		{
			return ;
		}
		delete m_calculator;
		m_calculator = NULL;
		delete m_curv_data;
		m_curv_data = NULL;

		SerializerT			serializer;
		m_calculator = serializer.serialize(v);

		if(m_calculator == 0)
		{
			return;
		}
		else
		{
		}

		m_curv_str.set_value(v);
	}

	//设置二进制的值
	virtual void set_value(const BYTE* v, int length)
	{
		m_curv_str.set_value(v, length);
	}

	// 获得表示值的字符串
	virtual const char* get_value()
	{
		if(!m_calculator)
			return 0;

		SerializerT			serializer;
		if (!m_curv_data)
			m_curv_data = m_calculator->get_curv_data();
		std::string str = serializer.curvData2Str(dynamic_cast<CurvDataT<ValueT>* >(m_curv_data));

		m_curv_str.set_value(str);

		return m_curv_str.get_str_value().c_str();
	}

	//获取二进制的值
	virtual const BYTE* get_value(int& length)
	{
		if (m_curv_str.has_binary())
		{
			int len = 0;
			SerializerT			serializer;
			BYTE* b = serializer.curvData2Byte(dynamic_cast<CurvDataT<ValueT>* >(m_curv_data), len);
			if (b != NULL)
			{
				m_curv_str.set_value(b, len);
				delete b;
			}
		}

		length = m_curv_str.get_binary_length();
		return m_curv_str.get_binary_value();

		return NULL;
	};

	virtual ApplierT* get_applier()
	{
		return m_applier;
	}

	virtual ICalculator* get_calculator()
	{
		return m_calculator;
	}

	// 执行
	virtual void execute(bool isExcute, float t) {};

protected:
	ApplierT*				m_applier;
	CalculatorT<ValueT>*	m_calculator;
	CurvData*				m_curv_data;
	ByteString				m_curv_str;

	TClass*					m_owner;

	bool					m_enable; //是否启用
	std::string				m_interpolating_str;
	int						m_property_type;
	float					m_last_curv_time;
};

template<typename ValueT>
struct TimeRelatedValueT
{
	ValueT val;
	float delta_time;
};
template<typename TClass, typename ApplierT, typename SerializerT, typename ValueT>
class PropertyCurvModifierStreambleFrameTimeRelated : public PropertyCurvModifierStreambleTimeRelated<TClass, ApplierT, SerializerT, ValueT>
{
public:
	PropertyCurvModifierStreambleFrameTimeRelated(TClass* owner, ApplierT* applier, int property_type)
		: PropertyCurvModifierStreambleTimeRelated<TClass, ApplierT, SerializerT, ValueT>(owner, applier, property_type)
	{NULL;}

	virtual void update(float t)
	{

		float frame = t / CSystemInfo::instance()->frame_time();

		if ((NULL == m_calculator) || !m_enable)
		{//如果不存在曲线,或者禁止更新
			return;
		}

		ValueT value;
		//m_applier->get_value(value);

		//曲线上的帧数
		float frame_on_curve = 0.0f;
		bool ret = m_calculator->calculate(frame, value, &frame_on_curve);

		//由帧换算回时间
		float delta_t = -1.0f;
		if (!is_equal(frame_on_curve, m_last_curv_time))
		{
			float time_on_curve = frame_on_curve * CSystemInfo::instance()->frame_time();
			delta_t = t - time_on_curve;
			/*
			clAssert(delta_t > -0.0002f);
			if (delta_t < -0.0002f)
			{
				int a = 5;
			}
			*/
			if (delta_t < 0)
			{
				delta_t = 0.0f;
			}
		}

		m_last_curv_time = frame_on_curve;

		if (!ret) //如果无法计算值或者不需要更新
		{
			return;
		}

		TimeRelatedValueT<ValueT> value_time_related;
		value_time_related.val = value;
		value_time_related.delta_time = delta_t;

		m_applier->apply(value_time_related); //应用值
	}

	// 获得t时刻属性值的字符串 t的单位为毫秒
	virtual const char* get_interpolating_value(float t)
	{
		float frame = t / CSystemInfo::instance()->frame_time();
		return PropertyCurvModifierStreambleTimeRelated<TClass, ApplierT, SerializerT, ValueT>::get_interpolating_value(frame);
	}
};
/**
* @brief
*		普通计算器
*/
template<typename ValueT>
class BaseCalculator : public CalculatorT<ValueT>
{
public:
	BaseCalculator(CurvNodes<ValueT>* nodes) : m_curv_nodes(nodes) 
	{
		m_delete_nodes = true;
	}

	virtual ~BaseCalculator()
	{
		if (m_delete_nodes)
			delete m_curv_nodes;
	}

	virtual CurvData* get_curv_data()
	{
		CurvDataT<ValueT>* curv_data = new CurvDataT<ValueT>;
		curv_data->m_nodes = m_curv_nodes;
		curv_data->curv_interpolating_type = get_interpolating_type(-1);

		m_delete_nodes = false;
		return curv_data; 
	}

	virtual int get_key_frames(int sub_index, float frames[], int& buffer_size)
	{
		if (sub_index != -1)
		{
			return 0;
		}

		if (buffer_size < m_curv_nodes->m_nodes.size())
		{
			buffer_size = m_curv_nodes->m_nodes.size();
			return 0;
		}

		buffer_size = m_curv_nodes->m_nodes.size();

		int i = 0;
		for (CurvNodes<ValueT>::NodeCollection::iterator it = m_curv_nodes->m_nodes.begin(); it != m_curv_nodes->m_nodes.end(); ++it)
		{
			frames[i++] = (*it)->m_t;
		}

		return buffer_size;
	}

	virtual void simplify()
	{
		if (m_curv_nodes->m_nodes.size() <= 1)
		{
			return;
		}
		
		CurvNode<ValueT>* node = *(m_curv_nodes->m_nodes.begin());
		CurvNodes<ValueT>::NodeCollection::iterator it = m_curv_nodes->m_nodes.begin();
		it++;

		bool equal = true;
		for (; it != m_curv_nodes->m_nodes.end(); ++it)
		{
			if (!is_node_data_equal(node, *it))
			{
				return;
			}
		}

		while (m_curv_nodes->m_nodes.size() > 1)
		{
			CurvNode<ValueT>* p = m_curv_nodes->m_nodes.back();
			m_curv_nodes->m_nodes.pop_back();
		}
	}

	virtual bool is_node_data_equal(CurvNode<ValueT>* node1, CurvNode<ValueT>* node2)
	{
		if (node1->m_value == node2->m_value)
		{
			return true;
		}
		
		return false;
	}

	virtual const char* get_interpolating_value(int sub_index, float t)
	{
		ValueT value;
		bool ret = calculate(t, value);
		if (!ret)
		{
			return NULL;
		}

		m_interpolating_value = GameGraphicHelper::value2Str(value);

		return m_interpolating_value.c_str();
	}

	virtual void set_key_frame_value(int sub_index, float frame, const char* value)
	{
		if (sub_index != -1)
		{
			return;
		}

		CurvNodes<ValueT>::NodeCollection::iterator it = m_curv_nodes->m_nodes.begin();
		for (; it != m_curv_nodes->m_nodes.end(); ++it)
		{
			if (is_equal((*it)->m_t, frame))
			{
				(*it)->m_value = GameGraphicHelper::str2Value<ValueT>(value);
				break;
			}
		}
	}

protected:
	CurvNodes<ValueT>* m_curv_nodes;
	std::string	 m_interpolating_value;

	bool		m_delete_nodes;
};


template<typename ValueT>
class LinearCalculator : public BaseCalculator<ValueT>
{
public:
	LinearCalculator(CurvNodes<ValueT>* nodes) : BaseCalculator<ValueT>(nodes){}
	virtual ~LinearCalculator(){}

	virtual bool calculate(float t, ValueT& value, float* time_on_curve = NULL)
	{
		CurvNodes<ValueT>::NodeCollection::iterator it = m_curv_nodes->m_nodes.begin();
		for (; it != m_curv_nodes->m_nodes.end(); ++it)
		{
			//if (t >= (*it)->m_t)
			if (is_greate_equal(t, (*it)->m_t))
			{
				CurvNodes<ValueT>::NodeCollection::iterator tmp_it = it;
				tmp_it++;
				if (tmp_it ==  m_curv_nodes->m_nodes.end())
				{
					value = (*it)->m_value;
					if (time_on_curve)
						(*time_on_curve) = (*it)->m_t;
					return true;
				}
				//if (t >= (*tmp_it)->m_t)
				if (is_greate_equal(t, (*tmp_it)->m_t))
				{
					continue;
				}

				//通过时间计算线性差值
				value = (ValueT)(((*tmp_it)->m_value - (*it)->m_value)* (1.0f * (t - (*it)->m_t) * 1.0f / ((*tmp_it)->m_t - (*it)->m_t)) + (*it)->m_value);
				if (time_on_curve)
					(*time_on_curve) = t;
				return true;
			}
			else
			{
				value = (*it)->m_value;
				if (time_on_curve)
					*time_on_curve = (*it)->m_t;
				return true;
			}
		}

		return false;
	}


	virtual int get_interpolating_type(int sub_index) 
	{
		return INTERPOLATINGTYPE_LINE;
	}
};

template<typename ValueT>
class PointCalculator : public BaseCalculator<ValueT>
{
public:
	PointCalculator(CurvNodes<ValueT>* nodes) : BaseCalculator<ValueT>(nodes){}
	virtual ~PointCalculator(){}

	virtual bool calculate(float t, ValueT& value, float* time_on_curve = NULL)
	{
		CurvNodes<ValueT>::NodeCollection::iterator it = m_curv_nodes->m_nodes.begin();
		for (; it != m_curv_nodes->m_nodes.end(); ++it)
		{
			//if (t >= (*it)->m_t)
			if (is_greate_equal(t, (*it)->m_t))
			{
				CurvNodes<ValueT>::NodeCollection::iterator tmp_it = it;
				tmp_it++;
				if (tmp_it ==  m_curv_nodes->m_nodes.end())
				{
					value = (*it)->m_value;
					if (time_on_curve)
						(*time_on_curve) = (*it)->m_t;
					return true;
				}
				//if (t >= (*tmp_it)->m_t)
				if (is_greate_equal(t, (*tmp_it)->m_t))
				{
					continue;
				}

				value = (*it)->m_value;
				if (time_on_curve)
					(*time_on_curve) = (*it)->m_t;
				return true;
			}
			else
			{
				value = (*it)->m_value;
				if (time_on_curve)
					(*time_on_curve) = (*it)->m_t;
				return true;
			}
		}

		return false;
	}

	virtual int get_interpolating_type(int sub_index) 
	{
		return INTERPOLATINGTYPE_POINT;
	}
};

template<typename ValueT>
class BezierCalculator : public BaseCalculator<ValueT>
{
public:
	BezierCalculator(CurvNodes<ValueT>* nodes) : BaseCalculator<ValueT>(nodes)
	{
		m_interpolating_type = INTERPOLATINGTYPE_BEZIER;
		m_last_update_time = 0.0f;
		m_last_update_value = ValueT();
	}

	virtual ~BezierCalculator(){}

	virtual bool calculate(float t, ValueT& value, float* time_on_curve = NULL)
	{
		CurvNodes<ValueT>::NodeCollection::iterator it = m_curv_nodes->m_nodes.begin();
		for (; it != m_curv_nodes->m_nodes.end(); ++it)
		{
			//if (t >= (*it)->m_t)
			if (is_greate_equal(t, (*it)->m_t))
			{
				CurvNodes<ValueT>::NodeCollection::iterator tmp_it = it;
				tmp_it++;
				if (tmp_it ==  m_curv_nodes->m_nodes.end())
				{
					value = (*it)->m_value;
					m_last_update_time = (*it)->m_t;
					m_last_update_value = (*it)->m_value;
					if (time_on_curve)
						(*time_on_curve) = (*it)->m_t;
					return true;
				}
				//if (t >= (*tmp_it)->m_t)
				if (is_greate_equal(t, (*tmp_it)->m_t))
				{
					continue;
				}

				if (INTERPOLATINGTYPE_BEZIER == m_interpolating_type)
				{
					BezierCurvNode<ValueT>* cur_node = (BezierCurvNode<ValueT>*)(*it);
					BezierCurvNode<ValueT>* next_node = (BezierCurvNode<ValueT>*)(*tmp_it);

					float percent = 0;
					int times = 0;
					int max_times = 3;
					while ((is_greate_equal(t,cur_node->m_t)) && !calculate_bezier_equation(cur_node->m_t, cur_node->m_next_t, next_node->m_pre_t, next_node->m_t, t, percent) && times < max_times)
					{
						t -= 0.01;
						times++;
					}

					if(times >= max_times)
					{
						if(is_less_equal(m_last_update_time, cur_node->m_t))
						{
							value = cur_node->m_value;
							m_last_update_time = cur_node->m_t;
							m_last_update_value = cur_node->m_value;
							return true;
						}
						else
						{
							value = m_last_update_value;
							m_last_update_time = t + 0.01 * times;							
							return true;
						}
					}
					

					value = calculate_bezier(percent, cur_node->m_value, cur_node->m_next_value, next_node->m_pre_value, next_node->m_value);
					m_last_update_time = t;
					m_last_update_value = value;
					if (time_on_curve)
						(*time_on_curve) = t;
					return true;
				}
				else if (INTERPOLATINGTYPE_LINE == m_interpolating_type)
				{
					value = (ValueT)(((*tmp_it)->m_value - (*it)->m_value)* (1.0f * (t - (*it)->m_t) * 1.0f / ((*tmp_it)->m_t - (*it)->m_t)) + (*it)->m_value);
					m_last_update_time = t;
					m_last_update_value = value;
					if (time_on_curve)
						(*time_on_curve) = t;
					return true;
				}

				return false;
			}
			else
			{
				value = (*it)->m_value;
				m_last_update_time = (*it)->m_t;
				m_last_update_value = (*it)->m_value;
				if (time_on_curve)
					(*time_on_curve) = (*it)->m_t;
				return true;
			}
		}

		return false;
	}

	virtual void simplify()
	{
		if (is_line_curv())
		{
			m_interpolating_type = INTERPOLATINGTYPE_LINE;
			BaseCalculator::simplify();
		}
	}

	virtual int get_interpolating_type(int sub_index) 
	{
		return m_interpolating_type;
	}

	virtual void set_key_frame_value(int sub_index, float frame, const char* value)
	{
		if (sub_index != -1)
		{
			return;
		}

		CurvNodes<ValueT>::NodeCollection::iterator it = m_curv_nodes->m_nodes.begin();
		for (; it != m_curv_nodes->m_nodes.end(); ++it)
		{
			if (is_equal((*it)->m_t, frame))
			{
				ValueT newVal = GameGraphicHelper::str2Value<ValueT>(value);
				ValueT gap = newVal - (*it)->m_value;
				(*it)->m_value = newVal;
				((BezierCurvNode<ValueT>*)(*it))->m_pre_value = ((BezierCurvNode<ValueT>*)(*it))->m_pre_value + gap;
				((BezierCurvNode<ValueT>*)(*it))->m_next_value = ((BezierCurvNode<ValueT>*)(*it))->m_next_value + gap;
				break;
			}
		}
	}

private:
	bool is_line_curv()
	{
		if (m_curv_nodes->m_nodes.size() <= 1)
		{
			return true;
		}

		CurvNodes<ValueT>::NodeCollection::iterator it = m_curv_nodes->m_nodes.begin();
		for (; it != m_curv_nodes->m_nodes.end(); ++it)
		{
			BezierCurvNode<ValueT>* cur_node = (BezierCurvNode<ValueT>*)(*it);
			CurvNodes<ValueT>::NodeCollection::iterator tmp_it = it;
			tmp_it++;
			if (tmp_it == m_curv_nodes->m_nodes.end())
			{
				break;
			}

			bool is_line = is_two_bezier_point_curv_line<ValueT>(cur_node, (BezierCurvNode<ValueT>*)(*tmp_it));

			if (!is_line)
			{
				return false;
			}
		}

		return true;
	}

private:
	InterpolatingType m_interpolating_type;
	ValueT m_last_update_value;
	float  m_last_update_time;
};

template<typename ValueT>
class PercentBezierCalculator : public BezierCalculator<ValueT>
{
public:
	PercentBezierCalculator(CurvNodes<ValueT>* nodes) : BezierCalculator<ValueT>(nodes){}
	virtual ~PercentBezierCalculator(){}

	virtual bool calculate(float t, ValueT& value)
	{
		CurvNodes<ValueT>::NodeCollection::iterator it = m_curv_nodes->m_nodes.begin();
		for (; it != m_curv_nodes->m_nodes.end(); ++it)
		{
			//if (t >= (*it)->m_t)
			if (is_greate_equal(t, (*it)->m_t))
			{
				CurvNodes<ValueT>::NodeCollection::iterator tmp_it = it;
				tmp_it++;
				if (tmp_it ==  m_curv_nodes->m_nodes.end())
				{
					value = (*it)->m_value;
					return true;
				}
				//if (t >= (*tmp_it)->m_t)
				if (is_greate_equal(t, (*tmp_it)->m_t))
				{
					continue;
				}

				BezierCurvNode<ValueT>* cur_node = (BezierCurvNode<ValueT>*)(*it);
				BezierCurvNode<ValueT>* next_node = (BezierCurvNode<ValueT>*)(*tmp_it);

				float percent = (t - cur_node->m_t) / (next_node->m_t - cur_node->m_t);

				value = calculate_bezier(percent, cur_node->m_value, cur_node->m_next_value, next_node->m_pre_value, next_node->m_value);
				return true;
			}
			else
			{
				value = (*it)->m_value;
				return true;
			}
		}

		return false;
	}
};

/**
* @brief
*		复合曲线计算器
*/
template<typename ValueT, typename ChildValueT>
class CompositeCalculator : public CalculatorT<ValueT>
{
public:
	CompositeCalculator(const std::list<ICalculator* >& calculates)
	{
		for (std::list<ICalculator*>::const_iterator it = calculates.begin(); it != calculates.end(); ++it)
		{
			m_calculates.push_back(dynamic_cast<CalculatorT<ChildValueT>*>(*it));
		}
	}
	virtual ~CompositeCalculator()
	{
		for (CalculatorCollection::iterator it = m_calculates.begin(); it != m_calculates.end(); )
		{
			CalculatorT<ChildValueT>* q = *it++;
			delete q;
		}
	}

	virtual bool calculate(float t, ValueT& value, float* time_on_curve = NULL)
	{
		int i = 0;
		for (CalculatorCollection::iterator it = m_calculates.begin(); it != m_calculates.end(); ++it)
		{
			ChildValueT elem;
			if ((*it)->calculate(t, elem, time_on_curve))
			{
				set_value(value, elem, i);
			}

			++i;
		}

		return true;
	}

	virtual CurvData* get_curv_data()
	{
		CurvData* curv_data = new CurvDataT<ValueT>;
		for (CalculatorCollection::iterator it = m_calculates.begin(); it != m_calculates.end(); ++it)
		{
			CurvData* curv = (*it)->get_curv_data();
			curv_data->add_curv(curv);
		}

		curv_data->curv_interpolating_type = INTERPOLATINGTYPE_BEZIER;
		
		return curv_data; 
	}

	virtual int get_interpolating_type(int sub_index)
	{
		if (-1 == sub_index)
		{
			return INTERPOLATINGTYPE_BEZIER;
		}

		CalculatorT<ChildValueT>* sub_calculator = get_sub_calculator(sub_index);
		if (NULL != sub_calculator)
		{
			return sub_calculator->get_interpolating_type(-1);
		}

		return INTERPOLATINGTYPE_UNKNOWN;
	}

	virtual void simplify()
	{
		m_key_frame_collection.clear();
		
		for (CalculatorCollection::iterator it = m_calculates.begin(); it != m_calculates.end(); ++it)
		{
			(*it)->simplify();
			int buffer_size = 0;
			int i = (*it)->get_key_frames(-1, NULL, buffer_size);
			if (buffer_size == 1)
			{
				m_key_frame_collection.insert(0);
				continue;
			}
			else if (buffer_size == 0)
			{
				continue;
			}

			float* buffer = new float[buffer_size + 1];
			if ((*it)->get_key_frames(-1, buffer, buffer_size))
			{
				for (int j = 0; j < buffer_size; ++j)
				{
					m_key_frame_collection.insert(buffer[j]);
				}
			}

			delete [] buffer;
		}
	}

	virtual int get_key_frames(int sub_index, float frames[], int& buffer_size)
	{
		if (-1 == sub_index)
		{
			if (buffer_size < m_key_frame_collection.size())
			{
				buffer_size = m_key_frame_collection.size();
				return 0;
			}

			buffer_size = m_key_frame_collection.size();
			int i = 0;
			for (std::set<float>::iterator it = m_key_frame_collection.begin(); it != m_key_frame_collection.end(); ++it)
			{
				frames[i++] = (*it);
			}
			return buffer_size;
		}

		CalculatorT<ChildValueT>* sub_calculator = get_sub_calculator(sub_index);
		if (NULL != sub_calculator)
		{
			return sub_calculator->get_key_frames(-1, frames, buffer_size);
		}

		return -1;
	}

	virtual const char* get_interpolating_value(int sub_index, float t)
	{
		if (-1 == sub_index)
		{
			ValueT  value;
			bool ret = calculate(t, value);
			if (!ret)
			{
				return NULL;
			}

			m_interpolating_value = GameGraphicHelper::value2Str<ValueT>(value);
			return m_interpolating_value.c_str();
		}

		CalculatorT<ChildValueT>* sub_calculator = get_sub_calculator(sub_index);
		if (NULL != sub_calculator)
		{
			return sub_calculator->get_interpolating_value(-1, t);
		}

		return NULL;
	}

	virtual void set_key_frame_value(int sub_index, float frame, const char* value)
	{
		if (-1 == sub_index)
		{
			return;
		}

		CalculatorT<ChildValueT>* sub_calculator = get_sub_calculator(sub_index);
		if (NULL != sub_calculator)
		{
			return sub_calculator->set_key_frame_value(-1, frame, value);
		}
	}

protected:
	virtual CalculatorT<ChildValueT>* get_sub_calculator(int sub_index)
	{
		for (CalculatorCollection::iterator it = m_calculates.begin(); it != m_calculates.end(); ++it)
		{
			if (0 == sub_index)
			{
				return (*it);
			}
			sub_index--;
		}

		return NULL;
	}

protected:
	typedef std::list<CalculatorT<ChildValueT>*> CalculatorCollection;
	CalculatorCollection m_calculates;

	std::string m_interpolating_value;
	std::set<float> m_key_frame_collection;
};

template<typename ValueT>
class PercentCompositeCalculator : public CalculatorT<ValueT>
{
public:
	PercentCompositeCalculator(const std::list<ICalculator* >& calculates)
	{
		for (CalculatorCollection::const_iterator it = calculates.begin(); it != calculates.end(); ++it)
		{
			m_calculates.push_back(*it);
		}
	}
	virtual ~PercentCompositeCalculator()
	{
		for (CalculatorCollection::iterator it = m_calculates.begin(); it != m_calculates.end(); )
		{
			ICalculator* q = *it++;
			delete q;
		}
	}

	
	virtual bool calculate(float t, ValueT& value, float* time_on_curve = NULL)
	{
		CalculatorCollection::iterator it = m_calculates.begin();
		CalculatorT<ValueT>* value_calculator = dynamic_cast<CalculatorT<ValueT>*>(*it++);
		CalculatorT<float>* percent_calculator = dynamic_cast<CalculatorT<float>*>(*it);

		float p = 0;
		if (!percent_calculator->calculate(t, p, time_on_curve))
		{
			return false;
		}

		if (!value_calculator->calculate(p, value))
		{
			return false;
		}

		return true;
	}

	virtual const char* get_interpolating_value(int sub_index, float t)
	{
		if (-1 == sub_index)
		{
			ValueT  value;
			bool ret = calculate(t, value);
			if (!ret)
			{
				return NULL;
			}

			m_interpolating_value = GameGraphicHelper::value2Str<ValueT>(value);
			return m_interpolating_value.c_str();
		}

		if (sub_index != (m_calculates.size()))
		{
			CalculatorCollection::iterator latest = m_calculates.end();
			latest--;
			CalculatorT<float>* percent_calculator = dynamic_cast<CalculatorT<float>*>(*latest);
			float p = 0;
			if (!percent_calculator->calculate(t, p))
			{
				return NULL;
			}

			t = p;
		}

		ICalculator* sub_calculator = get_sub_calculator(sub_index);
		if (NULL != sub_calculator)
		{
			return sub_calculator->get_interpolating_value(-1, t);
		}

		return NULL;
	}

	virtual void set_key_frame_value(int sub_index, float frame, const char* value)
	{
		if (-1 == sub_index)
		{
			return;
		}

		ICalculator* sub_calculator = get_sub_calculator(sub_index);
		if (NULL != sub_calculator)
		{
			return sub_calculator->set_key_frame_value(-1, frame, value);
		}
	}

	virtual CurvData* get_curv_data()
	{
		CurvData* curv_data = new CurvDataT<ValueT>;
		CalculatorCollection::iterator it = m_calculates.begin();
		CalculatorT<ValueT>* value_calculator = dynamic_cast<CalculatorT<ValueT>*>(*it++);
		CalculatorT<float>* percent_calculator = dynamic_cast<CalculatorT<float>*>(*it);
		curv_data->add_curv(value_calculator->get_curv_data());
		curv_data->add_curv(percent_calculator->get_curv_data());

		curv_data->curv_interpolating_type = INTERPOLATINGTYPE_BEZIER;

		return curv_data; 
	}

	virtual int get_interpolating_type(int sub_index)
	{
		if (-1 == sub_index)
		{
			return INTERPOLATINGTYPE_BEZIER;
		}

		ICalculator* sub_calculator = get_sub_calculator(sub_index);
		if (NULL != sub_calculator)
		{
			return sub_calculator->get_interpolating_type(-1);
		}

		return INTERPOLATINGTYPE_UNKNOWN;
	}

	virtual void simplify()
	{
		NULL;
	}

	virtual int get_key_frames(int sub_index, float frames[], int& buffer_size)
	{
		if (-1 == sub_index)
		{
			return -1;
		}

		ICalculator* sub_calculator = get_sub_calculator(sub_index);
		if (NULL != sub_calculator)
		{
			return sub_calculator->get_key_frames(-1, frames, buffer_size);
		}

		return -1;
	}
protected:
	virtual ICalculator* get_sub_calculator(int sub_index)
	{
		for (CalculatorCollection::iterator it = m_calculates.begin(); it != m_calculates.end(); ++it)
		{
			if (0 == sub_index)
			{
				return (*it);
			}
			sub_index--;
		}

		return NULL;
	}

protected:
	typedef std::list<ICalculator*> CalculatorCollection;
	CalculatorCollection m_calculates;

	std::string m_interpolating_value;
};
template<>
bool PercentCompositeCalculator<H3DVec3_2>::calculate(float t, H3DVec3_2& value, float* time_on_curve)
{
	CalculatorCollection::iterator it = m_calculates.begin();
	CalculatorT<H3DVec3_2>* value_calculator = dynamic_cast<CalculatorT<H3DVec3_2>*>(*it++);
	CalculatorT<float>* percent_calculator = dynamic_cast<CalculatorT<float>*>(*it);

	float p = 0;
	if (!percent_calculator->calculate(t, p, time_on_curve))
	{
		return false;
	}

	if (!value_calculator->calculate(p, value))
	{
		return false;
	}

	H3DVec3_2 DeltValue;
	if (t - s_DeltT > 0)
	{
		float pp = 0;
		if (!percent_calculator->calculate(t - s_DeltT, pp))
		{
			return false;
		}			
		if (!value_calculator->calculate(pp, DeltValue))
		{
			return false;
		}

		value.m_tangent = value.m_position - DeltValue.m_position;
	}
	else if (t < 0.0001)
	{
		float pp = 0;
		if (!percent_calculator->calculate(s_DeltT, pp))
		{
			return false;
		}			
		if (!value_calculator->calculate(pp, DeltValue))
		{
			return false;
		}

		value.m_tangent = DeltValue.m_position - value.m_position;
	}

	return true;
}

/************************************************************************/
/* 
*/
/************************************************************************/
/**
* @brief
*		曲线数据解析接口
*/
template<typename ValueT>
class ICurvSerializer
{
public:
	virtual CalculatorT<ValueT>* serialize(const char* v) = 0;
	virtual std::string curvData2Str(CurvDataT<ValueT>* curv_data) = 0;
	virtual BYTE* curvData2Byte(CurvDataT<ValueT>* curv_data, int& len) = 0;
};

template<typename ValueT>
bool NodeCmpFunc (CurvNode<ValueT>* elem1,  CurvNode<ValueT>* elem2)
{
	return elem1->m_t < elem2->m_t;
}


/**
* @brief
*		曲线数据解析接口
* @comment
*	曲线格式为
<PropertySequence>
<PropertyNode frame_index="", value="">
<PropertySequence />
<PropertySequence/>
</PropertySequence>
*/
template<typename ValueT>
class NormalCurvSerializer : ICurvSerializer<ValueT>
{
public:
	virtual CalculatorT<ValueT>* serialize(const char* v)
	{
		//BiXmlDocument* xml = new BiXmlDocument;
		BiXmlDocument* xml = XmlDocFactory::CreateDocument();
		//xml->Parse(v);
		xml->ParseXmlStr(v);
		if(xml->Error()) 
		{
			//delete xml;
			XmlDocFactory::ReleaseDocument(xml);
			assert(false);
			return false;
		}

		CalculatorT<ValueT>* calculator = serialize(const_cast<BiXmlNode*>(xml->FirstChild("PropertySequence")));

		//delete xml;
		XmlDocFactory::ReleaseDocument(xml);
		return calculator;
	}

	virtual CalculatorT<ValueT>* serialize(BiXmlNode* sequence)
	{
		if (NULL == sequence)
		{
			return NULL;
		}

		int interpolating_type = get_interpolating_type(sequence);
		CurvNodes<ValueT>* curv_nodes = create_curv_nodes(interpolating_type);

		BiXmlNode* property_node = const_cast<BiXmlNode*>(sequence->FirstChild("PropertyNode"));
		while(NULL != property_node)
		{
			CurvNode<ValueT>* node= parse_node(property_node, interpolating_type);
			curv_nodes->add_node(node);

			property_node = const_cast<BiXmlNode*>(property_node->NextSibling("PropertyNode"));
		}

		sort_nodes(curv_nodes->m_nodes);

		CalculatorT<ValueT>* calculator = create_calculator(interpolating_type, curv_nodes);

		return calculator;
	}

	virtual std::string curvData2Str(CurvDataT<ValueT>* curv_data)
	{

		BiXmlElement* head_elem = curvData2Xml(curv_data);

		std::string str = GameGraphicHelper::xml2Str(head_elem);
		//delete head_elem;
		XmlDocFactory::ReleaseElement(head_elem);
		
		return str;
	}

	virtual BiXmlElement* curvData2Xml(CurvDataT<ValueT>* curv_data)
	{
		//BiXmlElement* head_elem = new BiXmlElement("PropertySequence");
		BiXmlElement* head_elem = XmlDocFactory::CreateElement("PropertySequence");
		//设置插值方式
		head_elem->SetAttribute("InterpretationType", InterpolatingType2Str(curv_data->curv_interpolating_type).c_str());

		if (curv_data->curv_interpolating_type == INTERPOLATINGTYPE_BEZIER)
		{
			for (CurvNodes<ValueT>::NodeCollection::iterator it = curv_data->m_nodes->m_nodes.begin(); it != curv_data->m_nodes->m_nodes.end(); ++it)
			{
				BezierCurvNode<ValueT>* bezier_node = (BezierCurvNode<ValueT>*)(*it);

				//BiXmlElement node_elem("PropertyNode");
				BiXmlElement* node_elem = head_elem->InsertEndChild("PropertyNode")->ToElement();
				node_elem->SetAttribute("Frame", GameGraphicHelper::value2Str<float>(bezier_node->m_t).c_str());
				node_elem->SetAttribute("Value", GameGraphicHelper::value2Str<ValueT>(bezier_node->m_value).c_str());

				//BiXmlElement pre_node_elem("PreNode");
				BiXmlElement* pre_node_elem = node_elem->InsertEndChild("PreNode")->ToElement();
				pre_node_elem->SetAttribute("Frame", GameGraphicHelper::value2Str<float>(bezier_node->m_pre_t).c_str());
				pre_node_elem->SetAttribute("Value", GameGraphicHelper::value2Str<ValueT>(bezier_node->m_pre_value).c_str());
				//node_elem.InsertEndChild(pre_node_elem);

				//BiXmlElement next_node_elem("NextNode");
				BiXmlElement* next_node_elem = node_elem->InsertEndChild("NextNode")->ToElement();
				next_node_elem->SetAttribute("Frame", GameGraphicHelper::value2Str<float>(bezier_node->m_next_t).c_str());
				next_node_elem->SetAttribute("Value", GameGraphicHelper::value2Str<ValueT>(bezier_node->m_next_value).c_str());
				//node_elem.InsertEndChild(next_node_elem);

				//head_elem->InsertEndChild(node_elem);
			}
		}
		else
		{
			for (CurvNodes<ValueT>::NodeCollection::iterator it = curv_data->m_nodes->m_nodes.begin(); it != curv_data->m_nodes->m_nodes.end(); ++it)
			{
				//BiXmlElement node_elem("PropertyNode");
				BiXmlElement* node_elem = head_elem->InsertEndChild("PropertyNode")->ToElement();
				node_elem->SetAttribute("Frame", GameGraphicHelper::value2Str<float>((*it)->m_t).c_str());

				node_elem->SetAttribute("Value", GameGraphicHelper::value2Str<ValueT>((*it)->m_value).c_str());
				//head_elem->InsertEndChild(node_elem);
			}
		}

		return head_elem;
	}
	
	virtual BYTE* curvData2Byte(CurvDataT<ValueT>* curv_data, int& len)
	{
		return NULL;
	}

protected:
	typedef typename CurvNodes<ValueT>::NodeCollection CurvNodeCollection;
	typedef bool (*PFNNodeCmp)(CurvNode<ValueT>*, CurvNode<ValueT>*);
private:
	virtual void sort_nodes(CurvNodeCollection& node_collection)
	{
		node_collection.sort(NodeCmpFunc<ValueT>);
	}

	virtual CurvNodes<ValueT>* create_curv_nodes(int inter_polating_type)
	{
		return new CurvNodes<ValueT>;
	}

	virtual CurvNode<ValueT>* create_curv_node(int inter_polating_type)
	{
		if (inter_polating_type == INTERPOLATINGTYPE_BEZIER)
		{
			return (CurvNode<ValueT>*)(new BezierCurvNode<ValueT>);
		}

		return new CurvNode<ValueT>;
	}

	virtual CurvNode<ValueT>* parse_node(BiXmlNode* property_node, int inter_polating_type)
	{
		CurvNode<ValueT>* node = NULL;
		
		node = create_curv_node(inter_polating_type);

		const char* v =  BiXmlUtility::get_attribute(property_node, "Frame");
		if (NULL != v)
		{
			node->m_t = atof(v);
		}
		v = BiXmlUtility::get_attribute(property_node, "Value");
		node->m_value = GameGraphicHelper::str2Value<ValueT>(v);

		if (inter_polating_type == INTERPOLATINGTYPE_BEZIER)
		{
			BezierCurvNode<ValueT>* bezier_node = (BezierCurvNode<ValueT>*)(node);
			BiXmlNode* sub_node = property_node->FirstChild("PreNode");
			if (sub_node != NULL)
			{
				v =  BiXmlUtility::get_attribute(sub_node, "Frame");
				if (v != NULL)
				{
					bezier_node->m_pre_t = atof(v);
				}
				v = BiXmlUtility::get_attribute(sub_node, "Value");
				bezier_node->m_pre_value = GameGraphicHelper::str2Value<ValueT>(v);
			}
			sub_node = property_node->FirstChild("NextNode");
			if (sub_node != NULL)
			{
				v =  BiXmlUtility::get_attribute(sub_node, "Frame");
				if (NULL != v)
				{
					bezier_node->m_next_t = atof(v);
				}
				v = BiXmlUtility::get_attribute(sub_node, "Value");
				bezier_node->m_next_value = GameGraphicHelper::str2Value<ValueT>(v);
			}
		}

		return node;
	}

	virtual CalculatorT<ValueT>* create_calculator(int inter_polating_type, CurvNodes<ValueT>* curv_nodes)
	{
		CalculatorT<ValueT>* calculator = NULL;
		switch(inter_polating_type)
		{
		case INTERPOLATINGTYPE_BEZIER:
			calculator = new BezierCalculator<ValueT>(curv_nodes);
			break;
		case INTERPOLATINGTYPE_LINE:
			calculator = new LinearCalculator<ValueT>(curv_nodes);
			break;
		case INTERPOLATINGTYPE_POINT:
			calculator = new PointCalculator<ValueT>(curv_nodes);
			break;
		default:
			break;
		}

		return calculator;
	}

	virtual int get_interpolating_type(BiXmlNode* sequence)
	{
		const char* inter_polating_type = BiXmlUtility::get_attribute(sequence, "InterpretationType");
		if (_stricmp(inter_polating_type,  "Bezier") == 0)
		{
			return INTERPOLATINGTYPE_BEZIER;
		}
		if (_stricmp(inter_polating_type,  "Point") == 0)
		{
			return INTERPOLATINGTYPE_POINT;
		}
		if (_stricmp(inter_polating_type,  "Line") == 0)
		{
			return INTERPOLATINGTYPE_LINE;
		}

		return INTERPOLATINGTYPE_UNKNOWN;
	}
};
 
template<typename ValueT>
class DistanceCalculator
{
public:
	float calculate(const ValueT& begin, const ValueT& end)
	{
		return 0;
	};
};

template<>
class DistanceCalculator<H3DVec3>
{
public:
	float calculate(const H3DVec3& begin, const H3DVec3& end)
	{
		return (end - begin).Length();
	}
};
template<>
class DistanceCalculator<H3DVec3_2>
{
public:
	float calculate(const H3DVec3_2& begin, const H3DVec3_2& end)
	{
		return (end.m_position - begin.m_position).Length();
	}
};


template<typename ValueT>
class PercentNormalCurvSerializer : public NormalCurvSerializer<ValueT>
{
public:
	virtual BiXmlElement* curvData2Xml(CurvDataT<ValueT>* curv_data)
	{
		//BiXmlElement* head_elem = new BiXmlElement("PropertySequence");
		BiXmlElement* head_elem = XmlDocFactory::CreateElement("PropertySequence");
		//设置插值方式
		head_elem->SetAttribute("InterpretationType", InterpolatingType2Str(curv_data->curv_interpolating_type).c_str());

		if (curv_data->curv_interpolating_type == INTERPOLATINGTYPE_BEZIER)
		{
			for (CurvNodes<ValueT>::NodeCollection::iterator it = curv_data->m_nodes->m_nodes.begin(); it != curv_data->m_nodes->m_nodes.end(); ++it)
			{
				BezierCurvNode<ValueT>* bezier_node = (BezierCurvNode<ValueT>*)(*it);
				//BiXmlElement node_elem("PropertyNode");
				BiXmlElement* node_elem = head_elem->InsertEndChild("PropertyNode")->ToElement();
				node_elem->SetAttribute("Value", GameGraphicHelper::value2Str<ValueT>(bezier_node->m_value).c_str());

				//BiXmlElement pre_node_elem("PreNode");
				BiXmlElement* pre_node_elem = node_elem->InsertEndChild("PreNode")->ToElement();
				pre_node_elem->SetAttribute("Value", GameGraphicHelper::value2Str<ValueT>(bezier_node->m_pre_value).c_str());
				//node_elem.InsertEndChild(pre_node_elem);

				//BiXmlElement next_node_elem("NextNode");
				BiXmlElement* next_node_elem = node_elem->InsertEndChild("NextNode")->ToElement();
				next_node_elem->SetAttribute("Value", GameGraphicHelper::value2Str<ValueT>(bezier_node->m_next_value).c_str());
				//node_elem.InsertEndChild(next_node_elem);

				//head_elem->InsertEndChild(node_elem);
			}
		}
		else
		{
			for (CurvNodes<ValueT>::NodeCollection::iterator it = curv_data->m_nodes->m_nodes.begin(); it != curv_data->m_nodes->m_nodes.end(); ++it)
			{
				//BiXmlElement node_elem("PropertyNode");
				BiXmlElement* node_elem = head_elem->InsertEndChild("PropertyNode")->ToElement();
				node_elem->SetAttribute("Value", GameGraphicHelper::value2Str<ValueT>((*it)->m_value).c_str());
				//head_elem->InsertEndChild(node_elem);
			}
		}

		return head_elem;
	}

private:
	virtual CalculatorT<ValueT>* create_calculator(int inter_polating_type, CurvNodes<ValueT>* curv_nodes)
	{
		CalculatorT<ValueT>* calculator = NULL;
		switch(inter_polating_type)
		{
		case INTERPOLATINGTYPE_BEZIER:
			calculator = new PercentBezierCalculator<ValueT>(curv_nodes);
			break;
		case INTERPOLATINGTYPE_LINE:
			calculator = new LinearCalculator<ValueT>(curv_nodes);
			break;
		case INTERPOLATINGTYPE_POINT:
			calculator = new PointCalculator<ValueT>(curv_nodes);
			break;
		default:
			break;
		}

		return calculator;
	}

	virtual void sort_nodes(CurvNodeCollection& node_collection)
	{
		if (node_collection.size() == 0)
		{
			return;
		}

		float total_length = 0.0f;
		DistanceCalculator<ValueT> dis_cal;

		CurvNodes<ValueT>::NodeCollection::iterator it = node_collection.begin(); 
		(*it)->m_t = 0.0f;

		for (it = node_collection.begin(); it != node_collection.end(); ++it)
		{
			CurvNodes<ValueT>::NodeCollection::iterator tmp = it;
			tmp++;
			if (tmp == node_collection.end())
			{
				break;
			}

			total_length += dis_cal.calculate((*it)->m_value, (*tmp)->m_value);
		}

		if (0.0f == total_length)
		{
			return;
		}

		it = node_collection.begin();
		for (it = node_collection.begin(); it != node_collection.end(); ++it)
		{
			CurvNodes<ValueT>::NodeCollection::iterator tmp = it;
			tmp++;
			if (tmp == node_collection.end())
			{
				(*it)->m_t = 1.0f;
				break;
			}

			(*tmp)->m_t = dis_cal.calculate((*it)->m_value, (*tmp)->m_value) / total_length + (*it)->m_t;
		}

		//首先计算所有节点的数据，找出m_t
		node_collection.sort(NodeCmpFunc<ValueT>);
	}
};

template<typename ValueT, typename ChildValueT>
class CompositeCurvSerializer : ICurvSerializer<ValueT>
{
public:
	virtual CalculatorT<ValueT>* serialize(const char* v)
	{
		//BiXmlDocument* xml = new BiXmlDocument;
		BiXmlDocument* xml = XmlDocFactory::CreateDocument();
		xml->ParseXmlStr(v);
		if(xml->Error()) 
		{
			//delete xml;
			XmlDocFactory::ReleaseDocument(xml);
			assert(false);
			return false;
		}

		NormalCurvSerializer<ChildValueT> curv_serializer;
		std::list<ICalculator*> calculator_collection;

		BiXmlNode* child_node = xml->FirstChild("PropertySequence")->FirstChild("PropertySequence");
		while(child_node)
		{
			CalculatorT<ChildValueT>* calculator = curv_serializer.serialize(child_node);
			if (calculator != NULL)
			{
				calculator_collection.push_back(calculator);
			}

			child_node = child_node->NextSibling("PropertySequence");
		}

		CalculatorT<ValueT>* calculator = create_calculator(calculator_collection);

		XmlDocFactory::ReleaseDocument(xml);
		return calculator;
	}

	virtual BYTE* curvData2Byte(CurvDataT<ValueT>* curv_data, int& len)
	{
		return NULL;
	}

	virtual std::string curvData2Str(CurvDataT<ValueT>* curv_data)
	{

		BiXmlElement* head_elem = curvData2Xml(curv_data);

		std::string str = GameGraphicHelper::xml2Str(head_elem);
		//delete head_elem;
		XmlDocFactory::ReleaseElement(head_elem);

		return str;
	}

	virtual BiXmlElement* curvData2Xml(CurvDataT<ValueT>* curv_data)
	{
		//BiXmlElement* head_elem = new BiXmlElement("PropertySequence");
		BiXmlElement* head_elem = XmlDocFactory::CreateElement("PropertySequence");
		//设置插值方式
		head_elem->SetAttribute("InterpretationType", InterpolatingType2Str(curv_data->curv_interpolating_type).c_str());

		NormalCurvSerializer<ChildValueT> curv_serializer;

		for (CurvDataT<ValueT>::CurvCollection::iterator it = curv_data->m_curves.begin(); it != curv_data->m_curves.end(); ++it)
		{
			BiXmlElement* node_elem = curv_serializer.curvData2Xml(dynamic_cast<CurvDataT<ChildValueT>* >(*it));
			//head_elem->InsertEndChild(*node_elem);
			head_elem->LinkEndChild(node_elem);
			//delete node_elem;
		}

		return head_elem;
	}

private:
	virtual CalculatorT<ValueT>* create_calculator(const std::list<ICalculator*>& calculator_collection)
	{
		return new CompositeCalculator<ValueT, ChildValueT>(calculator_collection);
	}
};

template<typename ValueT, typename ChildValueT>
class PercentCompositeCurvSerializer : public CompositeCurvSerializer<ValueT, ChildValueT>
{
public:
	virtual CalculatorT<ValueT>* serialize(const char* v)
	{
		//BiXmlDocument* xml = new BiXmlDocument;
		BiXmlDocument* xml = XmlDocFactory::CreateDocument();
		//xml->Parse(v);
		xml->ParseXmlStr(v);
		if(xml->Error()) 
		{
			//delete xml;
			XmlDocFactory::ReleaseDocument(xml);
			assert(false);
			return false;
		}

		std::list<ICalculator*> calculator_collection;
		BiXmlNode* child_node = xml->FirstChild("PropertySequence")->FirstChild("PropertySequence");
		if (child_node == NULL)
		{
			return false;
		}

		PercentNormalCurvSerializer<ValueT> curv_serializer;
		CalculatorT<ValueT>* value_calculator = curv_serializer.serialize(child_node);
		if (value_calculator == NULL)
		{
			return false;
		}
		calculator_collection.push_back(value_calculator);

		child_node = child_node->NextSibling("PropertySequence");
		if (child_node == NULL)
		{
			return false;
		}

		NormalCurvSerializer<float> percent_curv_serializer;
		CalculatorT<float>* percent_calculator = percent_curv_serializer.serialize(child_node);
		if (percent_calculator == NULL)
		{
			return false;
		}
		calculator_collection.push_back(percent_calculator);

		CalculatorT<ValueT>* calculator = create_calculator(calculator_collection);

		//delete xml;
		XmlDocFactory::ReleaseDocument(xml);
		return calculator;
	}

	virtual BiXmlElement* curvData2Xml(CurvDataT<ValueT>* curv_data)
	{
		//BiXmlElement* head_elem = new BiXmlElement("PropertySequence");
		BiXmlElement* head_elem = XmlDocFactory::CreateElement("PropertySequence");
		//设置插值方式
		head_elem->SetAttribute("InterpretationType", InterpolatingType2Str(curv_data->curv_interpolating_type).c_str());

		int i = 0;
		for (CurvDataT<ValueT>::CurvCollection::iterator it = curv_data->m_curves.begin(); it != curv_data->m_curves.end(); ++it, ++i)
		{
			if (0 == i)
			{
				PercentNormalCurvSerializer<ValueT> curv_serializer;
				BiXmlElement* node_elem = curv_serializer.curvData2Xml(dynamic_cast<CurvDataT<ValueT>* >(*it));
				//head_elem->InsertEndChild(*node_elem);
				head_elem->LinkEndChild(node_elem);
				//delete node_elem;
			}
			else if (1 == i)
			{
				NormalCurvSerializer<float> curv_serializer;
				BiXmlElement* node_elem = curv_serializer.curvData2Xml(dynamic_cast<CurvDataT<float>* >(*it));
				//head_elem->InsertEndChild(*node_elem);
				head_elem->LinkEndChild(node_elem);
				//delete node_elem;
			}
		}

		return head_elem;
	}

private:
	virtual CalculatorT<ValueT>* create_calculator(const std::list<ICalculator*>& calculator_collection)
	{
		return new PercentCompositeCalculator<ValueT>(calculator_collection);
	}
};

template<typename OwnerT, typename ValueT>
class Applier
{
public:
	virtual ~Applier() {}
	virtual void apply(const ValueT& value) = 0;

	virtual void set_owner(OwnerT* owner)
	{
		m_owner = owner;
	}

	virtual	OwnerT*	get_owner()
	{
		return m_owner;
	}

	virtual void get_value(ValueT& value)
	{
		NULL;
	}

protected:
	OwnerT*		m_owner;
};

template <typename OwnerT, typename ValueT>
class PropertyGetter
{
public:
	virtual ~PropertyGetter() {NULL;}
	virtual void get(ValueT& value) = 0;

	virtual void set_owner(OwnerT* owner)
	{
		m_owner = owner;
	}

	virtual	OwnerT*	get_owner()
	{
		return m_owner;
	}

protected:
	OwnerT*		m_owner;
};

template<typename OwnerT, typename ValueT>
class GetSetApplier : public Applier<OwnerT, ValueT>
{
public:
	GetSetApplier(PropertyGetter<OwnerT, ValueT>* getter, Applier<OwnerT, ValueT>* setter)
	{
		assert(getter);
		assert(setter);

		m_getter = getter;
		m_setter = setter;
	}

	virtual ~GetSetApplier()
	{
		delete m_getter;
		delete m_setter;
	}

	virtual void apply(const ValueT& value)
	{
		m_setter->apply(value);
	}

	virtual void get_value(ValueT& value)
	{
		m_getter->get(value);
	}

	virtual void set_owner(OwnerT* owner)
	{
		Applier<OwnerT, ValueT>::set_owner(owner);
		m_getter->set_owner(owner);
		m_setter->set_owner(owner);
	}

	virtual	OwnerT*	get_owner()
	{
		return Applier<OwnerT, ValueT>::get_owner();
	}

protected:
	PropertyGetter<OwnerT,ValueT>*	m_getter; //值获取器
	Applier<OwnerT, ValueT>*		m_setter; //值设置器
};

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
template<>
void set_value<H3DVec3, float>(H3DVec3& value, const float& elem, int index);
//H3DColor的偏特化
template<>
void set_value<H3DColor, float>(H3DColor& value, const float& elem, int index);
//H3DVec4的偏特化
template<>
void set_value<H3DVec4, float>(H3DVec4& value, const float& elem, int index);
//H3DMat4的偏特化
template<>
void set_value<H3DMat4, float>(H3DMat4& value, const float& elem, int index);
//H3DColor的偏特化
template<>
void set_value<H3DMat3, float>(H3DMat3& value, const float& elem, int index);


/************************************************************************/
/* 
*/
/************************************************************************/
/**
@brief 萃取器函数的参数类型，创建相应的修改器实例
@param TClass类的类型
@param TResult类成员函数的返回值
@param TPara0类成员函数的第一个参数类型
@param pfunc类成员函数指针
@param fixed_parameters类成员函数调用时的固定参数
@return 相应的属性修改器
*/

//template<typename TClass, typename TResult, typename TPara0, typename TPara1>
//IPropertyModifier* createPropertyModifier(Actor* Actor, const PropertyModifierType& modifierid, TResult(TClass::*pfunc)(TPara0, TPara1), const std::string& fixed_parameters)
//{
//	return new PropertyModifierUsingTemplate_2<TClass, TResult, TPara0, TPara1>(t, pfunc, fixed_parameters);
//}
//template<typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
//IPropertyModifier* createPropertyModifier(Actor* Actor, const PropertyModifierType& modifierid, TResult(TClass::*pfunc)(TPara0, TPara1, TPara2), const std::string& fixed_parameters)
//{
//	return new PropertyModifierUsingTemplate_3<TClass, TResult, TPara0, TPara1, TPara2>(t, pfunc, fixed_parameters);
//}

class IPropertyModifierCreator
{
public:
	virtual ~IPropertyModifierCreator() {}
	virtual IPropertyModifier* create_modifier(Actor* actor, const PropertyModifierType& type) = 0;
	virtual bool release_modifier(IPropertyModifier* property) = 0;
	virtual int property_type() const = 0;
};

class PropertyModifierCreator : public IPropertyModifierCreator
{
public:
	virtual ~PropertyModifierCreator() {}
	virtual IPropertyModifier* create_modifier(Actor* actor, const PropertyModifierType& type);
	virtual int property_type() const{ return MEDIA_PROPERTY_TYPE_INVALID;};
	virtual bool release_modifier(IPropertyModifier* property);

private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;

	virtual Actor* find_actor(Actor* actor, const ActorID& subactor_id);
	virtual CMediaEndEntity* find_entity(Actor* owner, const SubEntityID& entity_id);
	virtual void divide_fixed_params(const ModifierFixParaInfo& fixed_params, std::list<std::string>& params);
};

class PropertyModifierFactory
{
public:
	PropertyModifierFactory();
	virtual ~PropertyModifierFactory();
	virtual IPropertyModifier* create_modifier(Actor* actor, const PropertyModifierType& type);
	virtual bool releaseModifier(IPropertyModifier* modifier);

	virtual void add_creator(const ActorPropertyType& propertyid, IPropertyModifierCreator* creator);
	virtual void remove_creator(const ActorPropertyType& propertyid);

protected:
	void init_creator_map();
	

private:
	typedef std::map<ActorPropertyType, IPropertyModifierCreator*> ModifierCreatorCollection;
	typedef ModifierCreatorCollection::iterator ModifierCreatorCollectionItr;

	ModifierCreatorCollection m_modifier_creator_map; 

	typedef std::map<int, IPropertyModifierCreator*> ModifierCreatorTypeMap;
	typedef ModifierCreatorTypeMap::iterator ModifierCreatorTypeMapItr;
	ModifierCreatorTypeMap m_modifier_creator_type_map;
};


//


#endif //_PROPERTY_MODIFIER_H_
