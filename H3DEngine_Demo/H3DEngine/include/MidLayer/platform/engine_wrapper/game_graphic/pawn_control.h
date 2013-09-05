/**
* @file pawn_control.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.11.14
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
*		Pawn��ͨ�ö���������
* @function
*		1�����¶���
*		2�����Ŷ���
*		3������������ӡ�ɾ�������ҡ���������
* @comment
*		1���������ƺ�PawnAnimationClip��һ�Զ�Ĺ�ϵ
*/
class PawnAnimationControl : public IPawnControl
{
public:
	PawnAnimationControl();
	virtual ~PawnAnimationControl();
	// ������Ҫ�޸ĵ�PAWN
	virtual void set_pawn(Pawn* pawn);
	//��ȡ��Ҫ�޸ĵ�PAWN
	virtual Pawn* get_pawn() = 0;
	// ����
	virtual void update(int t);
	// ���Ż�ֹͣ����������
	virtual void play(bool isplay, int anitime, PawnAnimationClip* aniclip);

	// ���һ������Ƭ��
	virtual bool add_animation_clip(const char* ani_name, PawnAnimationClip* aniclip);
	// ɾ��һ������Ƭ��
	//virtual bool remove_animation_clip(PawnAnimationClip* aniclip);
	virtual bool remove_animation_clip(const char* ani_name);
	// ����ָ��ID�Ķ���Ƭ��
	virtual PawnAnimationClip* find_animation_clip(const char* ani_name);

	// ���ö���Ƭ�ε�����
	void reset_iterator();
	// ��ö���Ƭ�Σ�������ָ����һ������Ƭ�Σ����������Ч����0
	PawnAnimationClip* next();

protected:
	Pawn*	m_pawn; //��Ҫ�޸ĵ�pawn

	typedef std::map<std::string, PawnAnimationClip*> IDToPawnAnimationClipMap;
	IDToPawnAnimationClipMap m_ani_clip_collection; /// ����¼�Ķ���Ƭ������
	IDToPawnAnimationClipMap::iterator m_cur_ani_clip_iterator; /// ��ǰ����Ƭ�εĵ�����

	typedef std::set<PawnAnimationClip*> PawnAnimationClipCollection; 
	PawnAnimationClipCollection m_inplay_animation_clip_collection; /// ���ڲ��ŵĶ���Ƭ��
};

/// pawn�������޸ĺ���������ID
typedef std::string PawnPropertyTypeID;
/// pawn������ID
typedef std::string PawnTypeID;
/// �޸������õĹ̶�����
typedef std::string ModifierFixParaInfo;
/// ��pawn��ID
typedef std::string SubPawnID;
/// �޸�������������ID
struct ModifierTypeID
{
	PawnPropertyTypeID m_property_name; /// �����ú���������ID
	PawnTypeID m_pawn_type_id; /// pawn������ID
	ModifierFixParaInfo m_fix_para_info; /// �޸������õĹ̶�����
	SubPawnID m_sub_pawnid; /// ��pawn��ID
};


/**
* @brief
*		��¼һ������Ƭ������������޸���
* @function
*		1�������޸����Ĺ�����ӡ�ɾ�������ҡ�����
*		2������ʱ����ִ�������޸���
*		3�����������޸���
* @comment
*		1�������޸�����������һһ��Ӧ��
*/
class PawnAnimationClip
{
public:
	PawnAnimationClip();
	virtual ~PawnAnimationClip();

	// ��������޸���
	bool add_property_modifier(const ModifierTypeID& modifier_typeid, IPropertyModifier* modifier);
	// �Ƴ������޸���
	bool remove_property_modifier(const ModifierTypeID& modifier_typeid);
	// ����ָ������ID�������޸���
	IPropertyModifier* find_property_modifier(const ModifierTypeID& modifier_typeid);

	// ���������޸���������
	void reset_iterator();
	// ��������޸�����������ָ����һ�������޸��������������Ч����0
	IPropertyModifier* next();

	// ���Ż�ֹͣ����
	void play(bool isplay, int anitime);
	// ����
	void update(int t);

protected:
	typedef std::map<ModifierTypeID, IPropertyModifier*> PropertyModifierCollection;
	PropertyModifierCollection m_property_modifier_collection;
	PropertyModifierCollection::iterator m_cur_property_modifier_itr;
};


// Pawn������
class PawnManager
{
public:
	// �����Pawn
	bool add_pawn(Pawn* pawn);
	// �Ƴ���Pawn�����������٣�
	void remove_pawn(Pawn* pawn);
	// ����ָ�����Ƶ��ӷ�װʵ��
	Pawn* find_pawn(const char* pawn_name);
	// ����Pawn������
	void reset_iterator();
	// ���Pawn��������ָ����һ��Pawn�����������Ч����0
	Pawn* next();
};


//ʣ�µ���ƣ���ƶ�������
//���������еĽ�������
//�������еĸ��²���
//��������
//Ŀ�꣺�����ܵ����ʹ�������
//���룺������ַ�������xml/������
//�����Frame-Property��
//��ϸ��
//		1���������������߽���
//		2�����������������߽���
//		3�������ܵ���˵�ǶԼ����������͵Ľ���
//��������
//Ŀ�꣺��ʱ�����ʱ������������������
//���룺����ʱ�䡢Frame-Property��
//������������Է����仯
//��ϸ��
//		1�����Գ��˱༭��Property����һЩ�����Ϣ
//		2����Щ�����Ϣ������������������

/**
* @brief
*		���������޸���
* @function
*		1��������������
*		2��������������
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

	// ����
	virtual void update(float t)
	{
		if (NULL == m_calculator)
		{//�������������
			return;
		}

		ValueT value;
		bool ret = m_calculator->calculate(t, value);
		if (!ret) //����޷�����ֵ���߲���Ҫ����
		{
			return;
		}

		m_applier->apply(value); //Ӧ��ֵ
	}

	// ͨ���ַ�������ֵ
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
*		�������ݽ����ӿ�
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
	float  m_t; //ʱ��/֡��/��������
	ValueT m_value; //m_t��Ӧ��ֵ
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
*		��ͨ������
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
*		�������߼�����
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
*		��ֵ��ʽ
*/
enum InterpolatingType 
{
	INTERPOLATINGTYPE_POINT,
	INTERPOLATINGTYPE_LINE,
	INTERPOLATINGTYPE_BEZIER
};


/**
* @brief
*		�������ݽ����ӿ�
* @comment
*	���߸�ʽΪ
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
Applier�����Ͷ��岿��
*/
/************************************************************************/

/************************************************************************/
/* 
set_value��ƫ�ػ���������
*/
/************************************************************************/
//H3DVec3��ƫ�ػ�
void set_value(H3DVec3& value, const float& elem, int index);
//H3DColor��ƫ�ػ�
void set_value(H3DColor& value, const float& elem, int index);

#endif //_PAWN_CONTROL_H_
