/**
* @file property_modifier_creator.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.23
*/

#ifndef _PROPERTY_MODIFIER_CREATOR_H_
#define _PROPERTY_MODIFIER_CREATOR_H_

#include "game_graphic_header.h"
#include "property_modifier.h"

/************************************************************************/
/* 
PropertyGetter的具体化类和帮助函数
*/
/************************************************************************/
//针对函数 ValueT(OwnerT::*pfunc)();
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
//针对函数 ValueT(OwnerT::*m_pfunc)(void) const;
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
//针对函数 const ValueT&(OwnerT::*m_pfunc)(void) const;
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
//针对函数 ValueT(*m_pfunc)(OwnerT*);
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
//针对函数 	ValueT(*m_pfunc)(OwnerT*, const TP0&, const TP1&);
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
Applie和一些具体化参数
*/
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
//针对函数TResult(OwnerT::*pfunc)(ValueT)的特化，这个类型不包括复合曲线
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
//针对函数TResult(OwnerT::*pfunc)(const ValueT&)的特化，这个类型包括复合曲线
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
//针对函数TResult(*pfunc)(OwnerT*, TP0)的特化，这个类型不包括复合曲线
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
//针对函数TResult(*pfunc)(OwnerT*, const TP0&)的特化，这个类型包括复合曲线
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
//针对函数TResult(*pfunc)(OwnerT*, TP0, TP1)的特化，这个类型不包括复合曲线
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
//针对函数TResult(*pfunc)(OwnerT*, TP0, const TP1&)的特化，这个类型包括复合曲线
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
//针对函数TResult(*pfunc)(OwnerT*, const TP0&, const TP1&)的特化，这个类型包括复合曲线
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
//针对函数TResult(OwnerT::*pfunc)(TPara0, TPara1, TPara2)的特化，这个类型不包括复合曲线
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
//针对函数TResult(*pfunc)(OwnerT*, TPara0, const TPara1&, TPara2)的特化，这个类型不包括复合曲线
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
//针对函数TResult(*pfunc)(OwnerT*, TPara0, const TPara1&, const TPara2&)的特化，这个类型包括复合曲线
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
//针对函数TResult(*pfunc)(OwnerT*, const TPara0&, const TPara1&, TPara2)的特化，这个类型不包括复合曲线
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
//针对函数TResult(*pfunc)(OwnerT*, const TPara0&, const TPara1&, const TPara2&)的特化，这个类型包括复合曲线
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
GetSetApplier的辅助函数
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
//Entity的其他一些基本属性
//资源属性和属性序列Modifer生成器
class ResourcePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_RESOURCE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

/************************************************************************/
/* 
属性和属性序列的生成器具体化部分
*/
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
//Pawn和Entity的一些基本属性

//可见性属性和属性序列Modifier生成器
class VisiblePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_VISIBLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//位置矩阵属性和属性序列Modifier生成器
class LocationPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LOCATION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//位置属性和属性序列Modifier生成器
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

//Rotation属性和属性序列Modifier生成器
class RotationPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ROTATION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Scale属性和属性序列Modifier生成器
class ScalePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SCALE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
	virtual IPropertyModifier* create_actor_property_modifier(Actor* actor, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

////////////////////////////////////////////////////////////////////////////
//相机的属性和属性序列
//Frustum属性和属性序列Modifer生成器
class CameraFrustumPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FRUSTUM;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};
//Frustum Fov属性和属性序列Modifer生成器
class CameraFovPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FOV;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};
//Frustum Ration属性和属性序列Modifer生成器
class CameraRatioPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_RATIO;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};
//Frustum fNear属性和属性序列Modifer生成器
class CameraNearPlanePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_NEAR_PLANE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Frustum fFar属性和属性序列Modifer生成器
class CameraFarPlanePropertyModifierCreator : public PropertyModifierCreator
{
public:
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FAR_PLANE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Focalplane属性和属性序列Modifer生成器
class CameraFocalplanePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FOCAL_PLANE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Dof属性和属性序列Modifer生成器
class CameraDofPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_DOF_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Confusion属性和属性序列Modifer生成器
class CameraConfusionPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_CONFUSION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//Lookat属性和属性序列Modifer生成器
class CameraLookatPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_LOOKAT;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//UpDirection属性和属性序列Modifer生成器
class CameraUpDirectionPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_UP;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//CameraAdjustType属性和属性序列Modifer生成器
class CameraAdjustTypePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_ADJUST_TYPE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//CameraTargetType属性和属性序列Modifer生成器
class CameraTargetTypePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_TARGET_TYPE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//CameraTargetes属性和属性序列Modifer生成器
class CameraTargetesPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_TARGETS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//相机后处理的属性和属性序列Modifer生成器
class CameraPostprocessPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_POST_PROCESS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//相机位置的属性和属性序列Modifer生成器
class CameraPostitionPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_POSITION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//////////////////////////////////////////////////////////////////////////
//模型的属性和属性序列创建
//开启模型阴影属性和属性序列Modifer生成器
class ModelShadelEnablePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ENABLE_MODEL_SHADOW;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//模型高亮属性和属性序列Modifer生成器
class ModelHighLightPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_HIGH_LIGHT_MODEL;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//模型材质属性和属性序列Modifer生成器
class ModelShaderVarPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SHADER_VAR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//模型振荡器属性和属性序列Modifer生成器
class ModelWaveFormPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SHADER_WAVEFORM;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

////////////////////////////////////////////////////////////////////////////
//灯光的属性和属性序列生成器

//灯光启用属性和属性序列Modifer生成器
class LightEnablePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_ENABLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};


//灯光颜色属性和属性序列Modifer生成器
class LightColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光照强度属性和属性序列Modifer生成器
class LightIntensityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_INTENSITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//方向光高光强度属性和属性序列Modifer生成器
class LightSpecularIntensityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SPECULAR_INTENSITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//启用灯光阴影属性和属性序列Modifer生成器
class EnableLightShadowPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ENABLE_LIGHT_SHADOW;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//灯光阴影颜色属性和属性序列Modifer生成器
class LightShadowColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHADOW_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//灯光近范围属性和属性序列Modifer生成器
class LightNearRangePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_NEAR_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//灯光远范围属性和属性序列Modifer生成器
class LightFarRangePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_FAR_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//灯光影响范围属性和属性序列Modifer生成器
class LightAffectParamPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_AFFECT_PARAM;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑宽度属性和属性序列Modifer生成器
class LightGlareWidthPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_WIDTH;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑高度属性和属性序列Modifer生成器
class LightGlareHeightPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_HEIGHT;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑颜色属性和属性序列Modifer生成器
class LightGlareColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑贴图路径属性和属性序列Modifer生成器
class LightGlareTexturePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_TEXTURE_NAME;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑贴图路径属性和属性序列Modifer生成器
class LightGlareShaderLibPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_SHADER_LIB;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑材质名称属性和属性序列Modifer生成器
class LightGlareShaderNamePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_SHADER_NAME;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑距离属性和属性序列Modifer生成器
class LightGlareDistancePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_DISTANCE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑旋转速度属性和属性序列Modifer生成器
class LightGlareRotationVelocityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_ROTATION_VELOCITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//灯光光斑可见性属性和属性序列Modifer生成器
class LightGlareVisiblePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_VISIBLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑跟随摄像机转动属性和属性序列Modifer生成器
class LightGlareRotateWithCameraPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_ROTATE_WITH_CAMERA;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光斑面向摄像机属性和属性序列Modifer生成器
class LightGlareViewCameraPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_GLARE_VIEW_CAMERA;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光柱线绘制属性和属性序列Modifer生成器
class LightBeamLineDrawPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_DRAW_LIGHT_BEAM_LINE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//LightPriority属性和属性序列Modifer生成器
class LightPriorityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SET_LIGHT_PRIORITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//体积光开启属性和属性序列Modifer生成器
class LightShaftEnablePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_ENABLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//体积光颜色属性和属性序列Modifer生成器
class LightShaftColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//体积光强度属性和属性序列Modifer生成器
class LightShaftIntensityPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_INTENSITY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//体积光阀值属性和属性序列Modifer生成器
class LightShaftThresholdPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_THRESHOLD;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//体积光深度属性和属性序列Modifer生成器
class LightShaftDepthPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_DEPTH;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//////////////////////////////////////////////////////////////////////////
//锥光源支持的属性和属性序列

//锥光源的内角度属性和属性序列Modifer生成器
class LightInnerAnglePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_INNER_ANGLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//锥光源的外角度属性和属性序列Modifer生成器
class LightOutterAnglePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_OUTTER_ANGLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//灯光投影启用属性和属性序列Modifer生成器
class LightProjTextureEnablePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_ENABLE_PROJ_TEXTURE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//灯光投影贴图名称属性和属性序列Modifer生成器
class LightProjectTextureNamePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_PROJ_TEXTURE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光柱可见性属性和属性序列Modifer生成器
class LightBeamVisiblePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_VISIBLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光柱的亮度属性和属性序列Modifer生成器
class LightBeamBrightnessPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_BRIGHTNESS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光柱的类型属性和属性序列
class LightBeamTypePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_TYPE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光柱的内半径属性和属性序列
class LightBeamInnerRadiusPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_INNER_RADIUS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光柱的近范围属性和属性序列
class LightBeamNearRangePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_NEAR_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光柱的远范围属性和属性序列
class LightBeamFarRangePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_FAR_RANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光柱的内角度属性和属性序列
class LightBeamInnerAnglePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_INNER_ANGLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//光柱的外角度属性和属性序列
class LightBeamOutterAnglePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_BEAM_OUTTER_ANGLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//后处理光柱的模糊系数属性和属性序列
class LightPostBeamBlurCoeffPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHT_POST_BEAM_BLUR_COEFF;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//锥光灯是否使用光斑属性和属性序列
class FakeLightPostPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_FAKE_LIGHT_SPOT_ENABLE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};


//////////////////////////////////////////////////////////////////////////
//特效的属性和属性序列生成器

//特效透明度属性和属性序列Modifer生成器
class EffectTransparencePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_EFFECT_TRANSPARENCE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//特效播放的属性和属性序列Modifer生成器
class EffectPlayPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_PLAY;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//////////////////////////////////////////////////////////////////////////
//系统设置
class SystemAmbientColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SYSTEM_AMBIENT_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

///系统设置 人物环境光
class SystemAmbientActorColorPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SYSTEM_AMBIENT_ACTOR_COLOR;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

/* 
	lightmap 的调整为 (ColorA * a) * lightmap + (ColorB * b)
	因此设置4个属性进行记录，分别为：
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
//相机是否根据客户端屏幕长宽比进行自动调整
class CameraFovAutoChangePropertyModifierCreator: public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_CAMERA_FOV_AUTO_CHANGE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

//////////////////////////////////////////////////////////////////////////
//useless,这只不过是个trick
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
//Avatar性别属性和属性Modifer生成器
class AvatarIsMalePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_IS_MALE;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

/*
*@breif avatar的bodypart属性modifer生成器
*/
class AvatarBodyPartPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_BODY_PART;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

/*
*@breif avatar的Action属性modifer生成器
*/
class AvatarActionPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ACTION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

/*
*@breif avatar的ActionLoop动作是否循环，属性modifer生成器
*/
class AvatarActionLoopPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ACTION_LOOP;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

/*
*@breif avatar的ActionList动作列表，实现连续动作设置，属性modifer生成器
*/
class AvatarActionListPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ACTION_LIST;};
private:
	//此处由于AvatarEntity封装时的层次不是继承自对应的CSkeletonModelEntity，导致要通过动态转换绕过去。
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
	virtual IPropertyModifier* create_entity_property_modifier(AvatarEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
	virtual IPropertyModifier* create_entity_property_modifier(CSkeletonModelEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

//绕Z轴旋转角度属性
class ZRotationByDegreePropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_ZROTATION;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};


////音效播放的属性和属性序列Modifer生成器
//class SoundPlayPropertyModifierCreator : public PropertyModifierCreator
//{
//public:
//	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SOUND_PLAY;};
//private:
//	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
//};
//
////音效的资源属性和属性序列Modifer生成器
//class SoundResourcePropertyModifierCreator : public PropertyModifierCreator
//{
//public:
//	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_SOUND_RESOURCE;};
//private:
//	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
//};

//模型是否接受光斑
class ReciveFakeLightSpotPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_RECEIVE_FAKE_LIGHT_SPOT;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};


/*
*@breif skeleton model的设置动作过度时间属性
*/
class TransitionTimeFromCurrentPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_TRANSITION_TIME_FROM_CURRENT;};
private:
	//此处由于AvatarEntity封装时的层次不是继承自对应的CSkeletonModelEntity，导致要通过动态转换绕过去。
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

class TransitionTimeToNextPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_TRANSITION_TIME_TO_NEXT;};
private:
	//此处由于AvatarEntity封装时的层次不是继承自对应的CSkeletonModelEntity，导致要通过动态转换绕过去。
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv);
};

//-------------------仅编辑器使用
//lightmap阴影指数
class LightMapShadowIndexPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHTMAP_SHADOW_INDEX;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};

//lightmap光源半径
class LightMapRadiusPropertyModifierCreator : public PropertyModifierCreator
{
public:
	virtual int property_type()const {return MEDIA_PROPERTY_TYPE_LIGHTMAP_RADIUS;};
private:
	virtual IPropertyModifier* create_entity_property_modifier(CMediaEndEntity* entity, const ActorPropertyType& type, const std::list<ModifierFixParaInfo>& fixed_params, bool is_curv) ;
};
#endif //_PROPERTY_MODIFIER_CREATOR_H_
