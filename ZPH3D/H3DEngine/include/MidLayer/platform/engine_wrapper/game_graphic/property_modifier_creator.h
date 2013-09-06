/**
* @file property_modifier_creator.h
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.11.23
*/

#ifndef _PROPERTY_MODIFIER_CREATOR_H_
#define _PROPERTY_MODIFIER_CREATOR_H_

#include "game_graphic_header.h"
#include "property_modifier.h"

/************************************************************************/
/* 
PropertyGetter�ľ��廯��Ͱ�������
*/
/************************************************************************/
//��Ժ��� ValueT(OwnerT::*pfunc)();
//template<typename OwnerT, typename ValueT>
//class FuncGetter : public PropertyGetter<OwnerT, ValueT>
//{
//public:
//	FuncGetter(ValueT(OwnerT::*pfunc)())
//	{
//		m_pfunc = pfunc;
//	}
//
//	void get(ValueT& value)
//	{
//		value = (m_owner->*m_pfunc)();
//	}
//
//private:
//	ValueT(OwnerT::*m_pfunc)();
//};
//
//template<typename TClass, typename TResult>
//FuncGetter<TClass, TResult>* createGetter(TResult(TClass::*pfunc)())
//{
//	return new FuncGetter<TClass, TResult>(pfunc);
//}

//////////////////////////////////////////////////////////////////////////
//��Ժ��� ValueT(OwnerT::*m_pfunc)(void) const;
template<typename OwnerT, typename ValueT>
class FuncGetterConst : public PropertyGetter<OwnerT, ValueT>
{
public:
	FuncGetterConst(ValueT(OwnerT::*pfunc)(void) const)
	{
		m_pfunc = pfunc;
	}

	void get(ValueT& value)
	{
		value = (m_owner->*m_pfunc)();
	}

private:
	ValueT(OwnerT::*m_pfunc)(void) const;
};

template<typename TClass, typename TResult>
FuncGetterConst<TClass, TResult>* createGetter(TResult(TClass::*pfunc)(void) const)
{
	return new FuncGetterConst<TClass, TResult>(pfunc);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ��� const ValueT&(OwnerT::*m_pfunc)(void) const;
template<typename OwnerT, typename ValueT>
class ConstFuncGetterConst : public PropertyGetter<OwnerT, ValueT>
{
public:
	ConstFuncGetterConst(const ValueT&(OwnerT::*pfunc)(void) const)
	{
		m_pfunc = pfunc;
	}

	void get(ValueT& value)
	{
		value = (m_owner->*m_pfunc)();
	}

private:
	const ValueT&(OwnerT::*m_pfunc)(void) const;
};

template<typename TClass, typename TResult>
ConstFuncGetterConst<TClass, TResult>* createGetter(const TResult&(TClass::*pfunc)(void) const)
{
	return new ConstFuncGetterConst<TClass, TResult>(pfunc);
}


//////////////////////////////////////////////////////////////////////////
//��Ժ��� ValueT(*m_pfunc)(OwnerT*);
template<typename OwnerT, typename ValueT>
class HelpFuncGetter : public PropertyGetter<OwnerT, ValueT>
{
public:
	HelpFuncGetter(ValueT(*pfunc)(OwnerT*))
	{
		m_pfunc = pfunc;
	}

	void get(ValueT& value)
	{
		value = m_pfunc(get_owner());
	}

private:
	ValueT(*m_pfunc)(OwnerT*);
};

template<typename TClass, typename TResult>
HelpFuncGetter<TClass, TResult>* createGetter(TResult(*pfunc)(TClass*))
{
	return new HelpFuncGetter<TClass, TResult>(pfunc);
}
//////////////////////////////////////////////////////////////////////////
template<typename OwnerT, typename ValueT, typename TP0>
class HelpFuncGetterFix1 : public PropertyGetter<OwnerT, ValueT>
{
public:
	HelpFuncGetterFix1 (ValueT(*pfunc)(OwnerT*, TP0), TP0 param0)
	{
		m_pfunc = pfunc;
		m_param0 = param0;
	}

	void get(ValueT& value)
	{
		value = m_pfunc(get_owner(), m_param0);
	}

private:
	ValueT(*m_pfunc)(OwnerT*, TP0);

	TP0	m_param0;
};

template<typename TClass, typename TResult, typename TP0>
HelpFuncGetterFix1 <TClass, TResult, TP0>* createGetter(TResult(*pfunc)(TClass*, TP0), TP0 param0)
{
	return new HelpFuncGetterFix1<TClass, TResult, TP0>(pfunc, param0);
}


//////////////////////////////////////////////////////////////////////////
//��Ժ��� 	ValueT(*m_pfunc)(OwnerT*, const TP0&, const TP1&);
template<typename OwnerT, typename ValueT, typename TP0, typename TP1>
class HelpFuncGetterFix2Const12 : public PropertyGetter<OwnerT, ValueT>
{
public:
	HelpFuncGetterFix2Const12(ValueT(*pfunc)(OwnerT*, const TP0&, const TP1&), const TP0& param0, const TP1& param1)
	{
		m_pfunc = pfunc;
		m_param0 = param0;
		m_param1 = param1;
	}

	void get(ValueT& value)
	{
		value = m_pfunc(get_owner(), m_param0, m_param1);
	}

private:
	ValueT(*m_pfunc)(OwnerT*, const TP0&, const TP1&);

	TP0	m_param0;
	TP1 m_param1;
};

template<typename TClass, typename TResult, typename TP0, typename TP1>
HelpFuncGetterFix2Const12<TClass, TResult, TP0, TP1>* createGetter(TResult(*pfunc)(TClass*, const TP0&, const TP1&), const TP0& param0, const TP1& param1)
{
	return new HelpFuncGetterFix2Const12<TClass, TResult, TP0, TP1>(pfunc, param0, param1);
}

/************************************************************************/
/* 
Applie��һЩ���廯����
*/
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(OwnerT::*pfunc)(ValueT)���ػ���������Ͳ�������������
template<typename OwnerT, typename TResult,  typename ValueT>
class FuncApplier : public Applier<OwnerT, ValueT>
{
public:
	FuncApplier(TResult(OwnerT::*pfunc)(ValueT))
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

template<typename TClass, typename TResult, typename TPara0>
FuncApplier<TClass, TResult, TPara0>* createApplier(TResult(TClass::*pfunc)(TPara0))
{
	return new FuncApplier<TClass, TResult, TPara0>(pfunc);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createPropertyModifier(ActorT* owner, FuncApplier<TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass, FuncApplier<TClass, TResult, TPara0>, TPara0>(dynamic_cast<TClass*>(owner), applier, property_type); 
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createNormalCurvPropertyModifier(ActorT* owner, FuncApplier<TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, FuncApplier<TClass, TResult, TPara0>, NormalCurvSerializer<TPara0>, TPara0>(dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(OwnerT::*pfunc)(const ValueT&)���ػ���������Ͱ�����������
template<typename OwnerT, typename TResult,  typename ValueT>
class FuncApplierConst : public Applier<OwnerT, ValueT>
{
public:
	FuncApplierConst(TResult(OwnerT::*pfunc)(const ValueT&))
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

template<typename TClass, typename TResult, typename TPara0>
FuncApplierConst<TClass, TResult, TPara0>* createApplier(TResult(TClass::*pfunc)(const TPara0&))
{
	return new FuncApplierConst<TClass, TResult, TPara0>(pfunc);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createPropertyModifier(ActorT* owner, FuncApplierConst<TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass, FuncApplierConst<TClass, TResult, TPara0>, TPara0>(dynamic_cast<TClass*>(owner), applier, property_type); 
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createNormalCurvPropertyModifier(ActorT* owner, FuncApplierConst<TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, FuncApplierConst<TClass, TResult, TPara0>, NormalCurvSerializer<TPara0>, TPara0>(dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename ValueT>
IPropertyModifier* createCompositeCurvPropertyModifierFloat(ActorT* owner, FuncApplierConst< TClass, TResult, ValueT>* applier, int property_type)
{
	return createCompositeCurvPropertyModifier<ActorT, TClass, TResult, ValueT, float>(owner, applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename ElemT>
IPropertyModifier* createCompositeCurvPropertyModifier(ActorT* owner, FuncApplierConst< TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, FuncApplierConst<TClass, TResult, TPara0>, CompositeCurvSerializer<TPara0, ElemT>, TPara0>(dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(*pfunc)(OwnerT*, TP0)���ػ���������Ͳ�������������
template<typename OwnerT, typename TResult, typename TP0>
class HelpFuncApplier : public Applier<OwnerT, TP0>
{
public:
	HelpFuncApplier(TResult(*pfunc)(OwnerT*, TP0))
	{
		m_pfunc = pfunc;
	}
	virtual void apply(const TP0& value)
	{
		m_pfunc(get_owner(), value);
	}

protected:
	TResult(*m_pfunc)(OwnerT*, TP0);
};

template<typename TClass, typename TResult, typename TPara0>
HelpFuncApplier<TClass, TResult, TPara0>* createApplier(TResult(*pfunc)(TClass*, TPara0))
{
	return new HelpFuncApplier<TClass, TResult, TPara0>(pfunc);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createPropertyModifier(ActorT* owner, HelpFuncApplier<TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass, HelpFuncApplier<TClass, TResult, TPara0>, TPara0>(dynamic_cast<TClass*>(owner), applier, property_type); 
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createNormalCurvPropertyModifier(ActorT* owner, HelpFuncApplier<TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, HelpFuncApplier<TClass, TResult, TPara0>, NormalCurvSerializer<TPara0>, TPara0>(dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createNormalCurvPropertyModifierTimeRelated(
	ActorT* owner, HelpFuncApplier<TClass, TResult, TimeRelatedValueT<TPara0> >* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrameTimeRelated<TClass, 
		HelpFuncApplier<TClass, TResult, TimeRelatedValueT<TPara0> >, NormalCurvSerializer<TPara0>, TPara0>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}
//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(*pfunc)(OwnerT*, const TP0&)���ػ���������Ͱ�����������
template<typename OwnerT, typename TResult, typename TP0>
class HelpFuncApplierConst : public Applier<OwnerT, TP0>
{
public:
	HelpFuncApplierConst(TResult(*pfunc)(OwnerT*, const TP0&))
	{
		m_pfunc = pfunc;
	}
	virtual void apply(const TP0& value)
	{
		m_pfunc(get_owner(), value);
	}

protected:
	TResult(*m_pfunc)(OwnerT*, const TP0&);
};

template<typename TClass, typename TResult, typename TPara0>
HelpFuncApplierConst<TClass, TResult, TPara0>* createApplier(TResult(*pfunc)(TClass*, const TPara0&))
{
	return new HelpFuncApplierConst<TClass, TResult, TPara0>(pfunc);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createPropertyModifier(
	ActorT* owner, HelpFuncApplierConst<TClass, TResult, TPara0>* applier,int property_type)
{
	return new PropertyModifierUsingTemplate<TClass, HelpFuncApplierConst<TClass, TResult, TPara0>, TPara0>(
		dynamic_cast<TClass*>(owner), applier, property_type); 
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createNormalCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierConst<TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConst<TClass, TResult, TPara0>, NormalCurvSerializer<TPara0>, TPara0>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename ValueT>
IPropertyModifier* createCompositeCurvPropertyModifierFloat(
	ActorT* owner, HelpFuncApplierConst< TClass, TResult, ValueT>* applier, int property_type)
{
	return createCompositeCurvPropertyModifier<ActorT, TClass, TResult, ValueT, float>(owner, applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename ElemT>
IPropertyModifier* createCompositeCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierConst< TClass, TResult, TPara0>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConst<TClass, TResult, TPara0>, CompositeCurvSerializer<TPara0, ElemT>, TPara0>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(*pfunc)(OwnerT*, TP0, TP1)���ػ���������Ͳ�������������
template<typename OwnerT,  typename TResult, typename TP0, typename TP1>
class HelpFuncApplierFix1 : public Applier<OwnerT, TP1>
{
public:
	HelpFuncApplierFix1(TResult(*pfunc)(OwnerT*, TP0, TP1), const TP0& param0)
	{
		m_pfunc = pfunc;
		m_fix_paramer0 = param0;
	}
	virtual void apply(const TP1& value)
	{
		m_pfunc(get_owner(), m_fix_paramer0, value);
	}

protected:
	TResult(*m_pfunc)(OwnerT*, TP0, TP1);
	TP0    m_fix_paramer0;
};

template<typename TClass, typename TResult, typename TPara0, typename TPara1>
HelpFuncApplierFix1<TClass, TResult, TPara0, TPara1>* createApplier(TResult(*pfunc)(TClass*, TPara0, TPara1), TPara0 param0)
{
	return new HelpFuncApplierFix1<TClass, TResult, TPara0, TPara1>(pfunc, param0);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1>
IPropertyModifier* createPropertyModifier(
	ActorT* owner, HelpFuncApplierFix1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass,
		HelpFuncApplierFix1<TClass, TResult, TPara0, TPara1>, TPara1>(
		dynamic_cast<TClass*>(owner), applier, property_type); 
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1>
IPropertyModifier* createNormalCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierFix1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierFix1<TClass, TResult, TPara0, TPara1>, NormalCurvSerializer<TPara1>, TPara1>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(*pfunc)(OwnerT*, TP0, const TP1&)���ػ���������Ͱ�����������
template<typename OwnerT,  typename TResult, typename TP0, typename TP1>
class HelpFuncApplierConstFix1 : public Applier<OwnerT, TP1>
{
public:
	HelpFuncApplierConstFix1(TResult(*pfunc)(OwnerT*, TP0, const TP1&), TP0 param0)
	{
		m_pfunc = pfunc;
		m_fix_paramer0 = param0;
	}
	virtual void apply(const TP1& value)
	{
		m_pfunc(get_owner(), m_fix_paramer0, value);
	}

protected:
	TResult(*m_pfunc)(OwnerT*, TP0, const TP1&);
	TP0    m_fix_paramer0;
};

template<typename TClass, typename TResult, typename TPara0, typename TPara1>
HelpFuncApplierConstFix1<TClass, TResult, TPara0, TPara1>* createApplier(TResult(*pfunc)(TClass*, TPara0, const TPara1&), TPara0 param0)
{
	return new HelpFuncApplierConstFix1<TClass, TResult, TPara0, TPara1>(pfunc, param0);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1>
IPropertyModifier* createPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass,
		HelpFuncApplierConstFix1<TClass, TResult, TPara0, TPara1>, TPara1>(
		dynamic_cast<TClass*>(owner), applier, property_type); 
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1>
IPropertyModifier* createNormalCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConstFix1<TClass, TResult, TPara0, TPara1>, NormalCurvSerializer<TPara1>, TPara1>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1>
IPropertyModifier* createCompositeCurvPropertyModifierFloat(
	ActorT* owner, HelpFuncApplierConstFix1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return createCompositeCurvPropertyModifier<ActorT, TClass, TResult, TPara0, TPara1, float>(
		owner, applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename ElemT>
IPropertyModifier* createCompositeCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConstFix1<TClass, TResult, TPara0, TPara1>, CompositeCurvSerializer<TPara1, ElemT>, TPara1>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(*pfunc)(OwnerT*, const TP0&, const TP1&)���ػ���������Ͱ�����������
template<typename OwnerT,  typename TResult, typename TP0, typename TP1>
class HelpFuncApplierConstFix1Const1 : public Applier<OwnerT, TP1>
{
public:
	HelpFuncApplierConstFix1Const1(TResult(*pfunc)(OwnerT*, const TP0&, const TP1&), const TP0& param0)
	{
		m_pfunc = pfunc;
		m_fix_paramer0 = param0;
	}
	virtual void apply(const TP1& value)
	{
		m_pfunc(get_owner(), m_fix_paramer0, value);
	}

protected:
	TResult(*m_pfunc)(OwnerT*, const TP0&, const TP1&);
	TP0    m_fix_paramer0;
};

template<typename TClass, typename TResult, typename TPara0, typename TPara1>
HelpFuncApplierConstFix1Const1<TClass, TResult, TPara0, TPara1>* createApplier(TResult(*pfunc)(TClass*, const TPara0&, const TPara1&), const TPara0& param0)
{
	return new HelpFuncApplierConstFix1Const1<TClass, TResult, TPara0, TPara1>(pfunc, param0);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1>
IPropertyModifier* createPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix1Const1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass,
		HelpFuncApplierConstFix1Const1<TClass, TResult, TPara0, TPara1>, TPara1>(
		dynamic_cast<TClass*>(owner), applier, property_type); 
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1>
IPropertyModifier* createNormalCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix1Const1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConstFix1Const1<TClass, TResult, TPara0, TPara1>, NormalCurvSerializer<TPara1>, TPara1>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1>
IPropertyModifier* createCompositeCurvPropertyModifierFloat(
	ActorT* owner, HelpFuncApplierConstFix1Const1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return createCompositeCurvPropertyModifier<ActorT, 
		TClass, TResult, TPara0, TPara1, float>(owner, applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename ElemT>
IPropertyModifier* createCompositeCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix1Const1<TClass, TResult, TPara0, TPara1>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConstFix1Const1<TClass, TResult, TPara0, TPara1>, CompositeCurvSerializer<TPara1, ElemT>, TPara1>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(OwnerT::*pfunc)(TPara0, TPara1, TPara2)���ػ���������Ͳ�������������
template<typename OwnerT,  typename TResult, typename TPara0, typename TPara1, typename TPara2>
class FuncApplierFix2 : public Applier<OwnerT, TPara2>
{
public:
	FuncApplierFix2(TResult(OwnerT::*pfunc)(TPara0, TPara1, TPara2), TPara0 para0, TPara1 para1)
	{
		m_pfunc = pfunc;
		m_para0 = para0;
		m_para1 = para1;
	}
	virtual void apply(const TPara2& value)
	{
		(m_owner->*m_pfunc)(m_para0, m_para1, value);
	}

protected:
	TResult(OwnerT::*m_pfunc)(TPara0, TPara1, TPara2);
	TPara0 m_para0;
	TPara1 m_para1;
};


template<typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
FuncApplierFix2<TClass, TResult, TPara0, TPara1, TPara2>* createApplier(TResult(TClass::*pfunc)(TPara0, TPara1, TPara2), TPara0 para0, TPara1 para1)
{
	return new FuncApplierFix2<TClass, TResult, TPara0, TPara1, TPara2>(pfunc, para0, para1);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createPropertyModifier(
	ActorT* owner, FuncApplierFix2<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass,
		FuncApplierFix2<TClass, TResult, TPara0, TPara1, TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type); 
}


template<typename OwnerT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createNormalCurvPropertyModifier(
	OwnerT* owner, FuncApplierFix2<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		FuncApplierFix2<TClass, TResult, TPara0, TPara1, TPara2>, NormalCurvSerializer<TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(*pfunc)(OwnerT*, TPara0, const TPara1&, TPara2)���ػ���������Ͳ�������������
template<typename OwnerT,  typename TResult, typename TPara0, typename TPara1, typename TPara2>
class HelpFuncApplierFix2Const2 : public Applier<OwnerT, TPara2>
{
public:
	HelpFuncApplierFix2Const2(TResult(*pfunc)(OwnerT*, TPara0, const TPara1&, TPara2), TPara0 para0, const TPara1& para1)
	{
		m_pfunc = pfunc;
		m_para0 = para0;
		m_para1 = para1;
	}
	virtual void apply(const TPara2& value)
	{
		m_pfunc(get_owner(), m_para0, m_para1, value);
	}

protected:
	TResult(*m_pfunc)(OwnerT*, TPara0, const TPara1&, TPara2);
	TPara0 m_para0;
	TPara1 m_para1;
};


template<typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
HelpFuncApplierFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>* createApplier(TResult(*pfunc)(TClass*, TPara0, const TPara1&, TPara2), TPara0 para0, const TPara1& para1)
{
	return new HelpFuncApplierFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>(pfunc, para0, para1);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createPropertyModifier(
	ActorT* owner, HelpFuncApplierFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass,
		HelpFuncApplierFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type); 
}


template<typename OwnerT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createNormalCurvPropertyModifier(
	OwnerT* owner, HelpFuncApplierFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>, NormalCurvSerializer<TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(*pfunc)(OwnerT*, TPara0, const TPara1&, const TPara2&)���ػ���������Ͱ�����������
template<typename OwnerT,  typename TResult, typename TPara0, typename TPara1, typename TPara2>
class HelpFuncApplierConstFix2Const2 : public Applier<OwnerT, TPara2>
{
public:
	HelpFuncApplierConstFix2Const2(TResult(*pfunc)(OwnerT*, TPara0, const TPara1&,const TPara2&), TPara0 para0, const TPara1& para1)
	{
		m_pfunc = pfunc;
		m_para0 = para0;
		m_para1 = para1;
	}
	virtual void apply(const TPara2& value)
	{
		m_pfunc(get_owner(), m_para0, m_para1, value);
	}

protected:
	TResult(*m_pfunc)(OwnerT*, TPara0, const TPara1&, const TPara2&);
	TPara0 m_para0;
	TPara1 m_para1;
};


template<typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
HelpFuncApplierConstFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>* createApplier(TResult(*pfunc)(TClass*, TPara0, const TPara1&, const TPara2&), TPara0 para0, const TPara1& para1)
{
	return new HelpFuncApplierConstFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>(pfunc, para0, para1);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass,
		HelpFuncApplierConstFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type); 
}


template<typename OwnerT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createNormalCurvPropertyModifier(
	OwnerT* owner, HelpFuncApplierConstFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConstFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>, NormalCurvSerializer<TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createCompositeCurvPropertyModifierFloat(
	ActorT* owner, HelpFuncApplierConstFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return createCompositeCurvPropertyModifier<ActorT, 
		TClass, TResult, TPara0, TPara1, TPara2, float>(owner, applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2, typename ElemT>
IPropertyModifier* createCompositeCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConstFix2Const2<TClass, TResult, TPara0, TPara1, TPara2>, CompositeCurvSerializer<TPara2, ElemT>, 
		TPara2>(dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(*pfunc)(OwnerT*, const TPara0&, const TPara1&, TPara2)���ػ���������Ͳ�������������
template<typename OwnerT,  typename TResult, typename TPara0, typename TPara1, typename TPara2>
class HelpFuncApplierFix2Const12 : public Applier<OwnerT, TPara2>
{
public:
	HelpFuncApplierFix2Const12(TResult(*pfunc)(OwnerT*, const TPara0&, const TPara1&, TPara2), const TPara0& para0, const TPara1& para1)
	{
		m_pfunc = pfunc;
		m_para0 = para0;
		m_para1 = para1;
	}
	virtual void apply(const TPara2& value)
	{
		m_pfunc(get_owner(), m_para0, m_para1, value);
	}

protected:
	TResult(*m_pfunc)(OwnerT*, const TPara0&, const TPara1&, TPara2);
	TPara0 m_para0;
	TPara1 m_para1;
};


template<typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
HelpFuncApplierFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>* createApplier(TResult(*pfunc)(TClass*, const TPara0&, const TPara1&, TPara2), const TPara0& para0, const TPara1& para1)
{
	return new HelpFuncApplierFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>(pfunc, para0, para1);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createPropertyModifier(
	ActorT* owner, HelpFuncApplierFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass, 
		HelpFuncApplierFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type); 
}


template<typename OwnerT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createNormalCurvPropertyModifier(
	OwnerT* owner, HelpFuncApplierFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>, NormalCurvSerializer<TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//��Ժ���TResult(*pfunc)(OwnerT*, const TPara0&, const TPara1&, const TPara2&)���ػ���������Ͱ�����������
template<typename OwnerT,  typename TResult, typename TPara0, typename TPara1, typename TPara2>
class HelpFuncApplierConstFix2Const12 : public Applier<OwnerT, TPara2>
{
public:
	HelpFuncApplierConstFix2Const12(TResult(*pfunc)(OwnerT*, const TPara0&, const TPara1&,const TPara2&), const TPara0& para0, const TPara1& para1)
	{
		m_pfunc = pfunc;
		m_para0 = para0;
		m_para1 = para1;
	}
	virtual void apply(const TPara2& value)
	{
		m_pfunc(get_owner(), m_para0, m_para1, value);
	}

protected:
	TResult(*m_pfunc)(OwnerT*, const TPara0&, const TPara1&, const TPara2&);
	TPara0 m_para0;
	TPara1 m_para1;
};


template<typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
HelpFuncApplierConstFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>* createApplier(TResult(*pfunc)(TClass*, const TPara0&, const TPara1&, const TPara2&), const TPara0& para0, const TPara1& para1)
{
	return new HelpFuncApplierConstFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>(pfunc, para0, para1);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyModifierUsingTemplate<TClass,
		HelpFuncApplierConstFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type); 
}


template<typename OwnerT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createNormalCurvPropertyModifier(
	OwnerT* owner, HelpFuncApplierConstFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass, 
		HelpFuncApplierConstFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>, NormalCurvSerializer<TPara2>, TPara2>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createCompositeCurvPropertyModifierFloat(
	ActorT* owner, HelpFuncApplierConstFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return createCompositeCurvPropertyModifier<ActorT,
		TClass, TResult, TPara0, TPara1, TPara2, float>(owner, applier, property_type);
}

template<typename ActorT, typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2, typename ElemT>
IPropertyModifier* createCompositeCurvPropertyModifier(
	ActorT* owner, HelpFuncApplierConstFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass,
		HelpFuncApplierConstFix2Const12<TClass, TResult, TPara0, TPara1, TPara2>, CompositeCurvSerializer<TPara2, ElemT>,
		TPara2>(dynamic_cast<TClass*>(owner), applier, property_type);
}


/************************************************************************/
/* 
GetSetApplier�ĸ�������
*/
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
template<typename TClass, typename TPara0>
GetSetApplier<TClass, TPara0>* createGetSetApplier(Applier<TClass, TPara0>* setter, PropertyGetter<TClass, TPara0>* getter)
{
	return new GetSetApplier<TClass, TPara0>(getter, setter);
}

template<typename ActorT, typename TClass, typename TPara0, typename ElemT>
IPropertyModifier* createCompositeCurvPropertyModifier(
	ActorT* owner, GetSetApplier< TClass, TPara0>* applier, int property_type)
{
	return new PropertyCurvModifierStreambleFrame<TClass,
		GetSetApplier<TClass, TPara0>, CompositeCurvSerializer<TPara0, ElemT>, TPara0>(
		dynamic_cast<TClass*>(owner), applier, property_type);
}

template<typename ActorT, typename TClass, typename ValueT>
IPropertyModifier* createCompositeCurvPropertyModifierFloat(
	ActorT* owner, GetSetApplier<TClass, ValueT>* applier, int property_type)
{
	return createCompositeCurvPropertyModifier<ActorT, TClass, ValueT, float>(owner, applier, property_type);
}

//////////////////////////////////////////////////////////////////////////
//Entity������һЩ��������
//��Դ���Ժ���������Modifer������
class ResourcePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_RESOURCE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

/************************************************************************/
/* 
���Ժ��������е����������廯����
*/
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
//Pawn��Entity��һЩ��������

//�ɼ������Ժ���������Modifier������
class VisiblePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_VISIBLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//λ�þ������Ժ���������Modifier������
class LocationPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LOCATION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//λ�����Ժ���������Modifier������
class PositionPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_POSITION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};
class PathPositionPropertyModifierCreator: public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_PATH_POSITION;};

private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Rotation���Ժ���������Modifier������
class RotationPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ROTATION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Scale���Ժ���������Modifier������
class ScalePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SCALE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

////////////////////////////////////////////////////////////////////////////
//��������Ժ���������
//Frustum���Ժ���������Modifer������
class CameraFrustumPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FRUSTUM;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};
//Frustum Fov���Ժ���������Modifer������
class CameraFovPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FOV;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};
//Frustum Ration���Ժ���������Modifer������
class CameraRatioPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_RATIO;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};
//Frustum fNear���Ժ���������Modifer������
class CameraNearPlanePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_NEAR_PLANE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Frustum fFar���Ժ���������Modifer������
class CameraFarPlanePropertyModifierCreator : public PropertyModifierCreator
{
public:
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FAR_PLANE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Focalplane���Ժ���������Modifer������
class CameraFocalplanePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FOCAL_PLANE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Dof���Ժ���������Modifer������
class CameraDofPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_DOF_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Confusion���Ժ���������Modifer������
class CameraConfusionPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_CONFUSION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Lookat���Ժ���������Modifer������
class CameraLookatPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_LOOKAT;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//UpDirection���Ժ���������Modifer������
class CameraUpDirectionPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_UP;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//CameraAdjustType���Ժ���������Modifer������
class CameraAdjustTypePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_ADJUST_TYPE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//CameraTargetType���Ժ���������Modifer������
class CameraTargetTypePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_TARGET_TYPE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//CameraTargetes���Ժ���������Modifer������
class CameraTargetesPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_TARGETS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����������Ժ���������Modifer������
class CameraPostprocessPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_POST_PROCESS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//���λ�õ����Ժ���������Modifer������
class CameraPostitionPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_POSITION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//////////////////////////////////////////////////////////////////////////
//ģ�͵����Ժ��������д���
//����ģ����Ӱ���Ժ���������Modifer������
class ModelShadelEnablePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ENABLE_MODEL_SHADOW;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//ģ�͸������Ժ���������Modifer������
class ModelHighLightPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_HIGH_LIGHT_MODEL;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//ģ�Ͳ������Ժ���������Modifer������
class ModelShaderVarPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SHADER_VAR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//ģ���������Ժ���������Modifer������
class ModelWaveFormPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SHADER_WAVEFORM;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

////////////////////////////////////////////////////////////////////////////
//�ƹ�����Ժ���������������

//�ƹ��������Ժ���������Modifer������
class LightEnablePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_ENABLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};


//�ƹ���ɫ���Ժ���������Modifer������
class LightColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//����ǿ�����Ժ���������Modifer������
class LightIntensityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_INTENSITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����߹�ǿ�����Ժ���������Modifer������
class LightSpecularIntensityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SPECULAR_INTENSITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//���õƹ���Ӱ���Ժ���������Modifer������
class EnableLightShadowPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ENABLE_LIGHT_SHADOW;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�ƹ���Ӱ��ɫ���Ժ���������Modifer������
class LightShadowColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHADOW_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�ƹ����Χ���Ժ���������Modifer������
class LightNearRangePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_NEAR_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�ƹ�Զ��Χ���Ժ���������Modifer������
class LightFarRangePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_FAR_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�ƹ�Ӱ�췶Χ���Ժ���������Modifer������
class LightAffectParamPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_AFFECT_PARAM;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//��߿�����Ժ���������Modifer������
class LightGlareWidthPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_WIDTH;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//��߸߶����Ժ���������Modifer������
class LightGlareHeightPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_HEIGHT;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����ɫ���Ժ���������Modifer������
class LightGlareColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����ͼ·�����Ժ���������Modifer������
class LightGlareTexturePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_TEXTURE_NAME;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����ͼ·�����Ժ���������Modifer������
class LightGlareShaderLibPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_SHADER_LIB;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//��߲����������Ժ���������Modifer������
class LightGlareShaderNamePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_SHADER_NAME;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//��߾������Ժ���������Modifer������
class LightGlareDistancePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_DISTANCE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����ת�ٶ����Ժ���������Modifer������
class LightGlareRotationVelocityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_ROTATION_VELOCITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�ƹ��߿ɼ������Ժ���������Modifer������
class LightGlareVisiblePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_VISIBLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//��߸��������ת�����Ժ���������Modifer������
class LightGlareRotateWithCameraPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_ROTATE_WITH_CAMERA;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//���������������Ժ���������Modifer������
class LightGlareViewCameraPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_VIEW_CAMERA;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����߻������Ժ���������Modifer������
class LightBeamLineDrawPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_DRAW_LIGHT_BEAM_LINE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//LightPriority���Ժ���������Modifer������
class LightPriorityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SET_LIGHT_PRIORITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//����⿪�����Ժ���������Modifer������
class LightShaftEnablePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_ENABLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�������ɫ���Ժ���������Modifer������
class LightShaftColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����ǿ�����Ժ���������Modifer������
class LightShaftIntensityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_INTENSITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//����ֵⷧ���Ժ���������Modifer������
class LightShaftThresholdPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_THRESHOLD;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����������Ժ���������Modifer������
class LightShaftDepthPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_DEPTH;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//////////////////////////////////////////////////////////////////////////
//׶��Դ֧�ֵ����Ժ���������

//׶��Դ���ڽǶ����Ժ���������Modifer������
class LightInnerAnglePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_INNER_ANGLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//׶��Դ����Ƕ����Ժ���������Modifer������
class LightOutterAnglePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_OUTTER_ANGLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�ƹ�ͶӰ�������Ժ���������Modifer������
class LightProjTextureEnablePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_ENABLE_PROJ_TEXTURE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�ƹ�ͶӰ��ͼ�������Ժ���������Modifer������
class LightProjectTextureNamePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_PROJ_TEXTURE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����ɼ������Ժ���������Modifer������
class LightBeamVisiblePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_VISIBLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�������������Ժ���������Modifer������
class LightBeamBrightnessPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_BRIGHTNESS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�������������Ժ���������
class LightBeamTypePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_TYPE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�������ڰ뾶���Ժ���������
class LightBeamInnerRadiusPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_INNER_RADIUS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�����Ľ���Χ���Ժ���������
class LightBeamNearRangePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_NEAR_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//������Զ��Χ���Ժ���������
class LightBeamFarRangePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_FAR_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//�������ڽǶ����Ժ���������
class LightBeamInnerAnglePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_INNER_ANGLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//��������Ƕ����Ժ���������
class LightBeamOutterAnglePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_OUTTER_ANGLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//���������ģ��ϵ�����Ժ���������
class LightPostBeamBlurCoeffPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_POST_BEAM_BLUR_COEFF;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//׶����Ƿ�ʹ�ù�����Ժ���������
class FakeLightPostPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_FAKE_LIGHT_SPOT_ENABLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};


//////////////////////////////////////////////////////////////////////////
//��Ч�����Ժ���������������

//��Ч͸�������Ժ���������Modifer������
class EffectTransparencePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_EFFECT_TRANSPARENCE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//��Ч���ŵ����Ժ���������Modifer������
class EffectPlayPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_PLAY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//////////////////////////////////////////////////////////////////////////
//ϵͳ����
class SystemAmbientColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SYSTEM_AMBIENT_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

///ϵͳ���� ���ﻷ����
class SystemAmbientActorColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SYSTEM_AMBIENT_ACTOR_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

/* 
	lightmap �ĵ���Ϊ (ColorA * a) * lightmap + (ColorB * b)
	�������4�����Խ��м�¼���ֱ�Ϊ��
		SystemLightMapColorFactorA
		SystemLightMapColorFactorB
		SystemLightMapColorFactorAIntensity
		SystemLightMapColorFactorBIntensity

*/
class SystemLightMapColorFactorAPropertyModifierCreator:public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_COLOR_FACTOR_A;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};
class SystemLightMapColorFactorBPropertyModifierCreator:public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_COLOR_FACTOR_B;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};
class SystemLightMapColorFactorAIntensityPropertyModifierCreator:public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_COLOR_FACTOR_A_INTENSITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};
class SystemLightMapColorFactorBIntensityPropertyModifierCreator:public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_COLOR_FACTOR_B_INTENSITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};



////////////////////////////////////////////////////////////////////////////
//����Ƿ���ݿͻ�����Ļ����Ƚ����Զ�����
class CameraFovAutoChangePropertyModifierCreator: public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FOV_AUTO_CHANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

//////////////////////////////////////////////////////////////////////////
//useless,��ֻ�����Ǹ�trick
inline std::string operator - (const std::string& first_value, const std::string& second_value)
{
	return first_value;
}

template<typename T>
inline std::string operator * (const std::string& first_value, T second_value)
{
	return first_value;
}

inline std::string operator / (const std::string& first_value, const std::string& second_value)
{
	return first_value;
}

//////////////////////////////////////////
//Avatar�Ա����Ժ�����Modifer������
class AvatarIsMalePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_IS_MALE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

/*
*@breif avatar��bodypart����modifer������
*/
class AvatarBodyPartPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_BODY_PART;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

/*
*@breif avatar��Action����modifer������
*/
class AvatarActionPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ACTION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

/*
*@breif avatar��ActionLoop�����Ƿ�ѭ��������modifer������
*/
class AvatarActionLoopPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ACTION_LOOP;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

/*
*@breif avatar��ActionList�����б�ʵ�������������ã�����modifer������
*/
class AvatarActionListPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ACTION_LIST;};
private:
	//�˴�����AvatarEntity��װʱ�Ĳ�β��Ǽ̳��Զ�Ӧ��CSkeletonModelEntity������Ҫͨ����̬ת���ƹ�ȥ��
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
	virtual IPropertyModifier* create_entity_property_modifier(AvatarEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
	virtual IPropertyModifier* create_entity_property_modifier(CSkeletonModelEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

//��Z����ת�Ƕ�����
class ZRotationByDegreePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ZROTATION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};


////��Ч���ŵ����Ժ���������Modifer������
//class SoundPlayPropertyModifierCreator : public PropertyModifierCreator
//{
//public:
//	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SOUND_PLAY;};
//private:
//	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
//};
//
////��Ч����Դ���Ժ���������Modifer������
//class SoundResourcePropertyModifierCreator : public PropertyModifierCreator
//{
//public:
//	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SOUND_RESOURCE;};
//private:
//	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
//};

//ģ���Ƿ���ܹ��
class ReciveFakeLightSpotPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_RECEIVE_FAKE_LIGHT_SPOT;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};


/*
*@breif skeleton model�����ö�������ʱ������
*/
class TransitionTimeFromCurrentPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_TRANSITION_TIME_FROM_CURRENT;};
private:
	//�˴�����AvatarEntity��װʱ�Ĳ�β��Ǽ̳��Զ�Ӧ��CSkeletonModelEntity������Ҫͨ����̬ת���ƹ�ȥ��
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

class TransitionTimeToNextPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_TRANSITION_TIME_TO_NEXT;};
private:
	//�˴�����AvatarEntity��װʱ�Ĳ�β��Ǽ̳��Զ�Ӧ��CSkeletonModelEntity������Ҫͨ����̬ת���ƹ�ȥ��
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

//-------------------���༭��ʹ��
//lightmap��Ӱָ��
class LightMapShadowIndexPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHTMAP_SHADOW_INDEX;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//lightmap��Դ�뾶
class LightMapRadiusPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHTMAP_RADIUS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};
#endif //_PROPERTY_MODIFIER_CREATOR_H_
