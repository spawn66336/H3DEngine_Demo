/**
* @file pawn_control.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.11.14
*/

#include "game_graphic_body.h"

/// ModifierTypeID的比较器
bool operator<(const ModifierTypeID& a, const ModifierTypeID& b)
{
	return std::make_pair(a.m_property_name, std::make_pair(a.m_pawn_type_id, std::make_pair(a.m_fix_para_info, a.m_sub_pawnid))) 
		< std::make_pair(b.m_property_name, std::make_pair(b.m_pawn_type_id, std::make_pair(b.m_fix_para_info, b.m_sub_pawnid)));
}

PawnAnimationControl::PawnAnimationControl()
{
	m_pawn = 0;
	reset_iterator();
}

PawnAnimationControl::~PawnAnimationControl()
{
	for(IDToPawnAnimationClipMap::iterator pos = m_ani_clip_collection.begin();
		pos != m_ani_clip_collection.end(); ++pos)
	{
		delete pos->second;
	}
}

// 设置需要修改的PAWN
void PawnAnimationControl::set_pawn(Pawn* pawn)
{
	m_pawn = pawn;
}
//获取需要修改的PAWN
Pawn* PawnAnimationControl::get_pawn()
{
	return m_pawn;
}
// 更新
void PawnAnimationControl::update(int t)
{
	for(PawnAnimationClipCollection::iterator pos = m_inplay_animation_clip_collection.begin();
		pos != m_inplay_animation_clip_collection.end(); ++pos)
	{
		PawnAnimationClip* aniclip = *pos;
		aniclip->update(t);
	}
}
// 播放或停止动画控制器
void PawnAnimationControl::play(bool isplay, int anitime, PawnAnimationClip* aniclip)
{
	aniclip->play(isplay, anitime);

	if(isplay)
		m_inplay_animation_clip_collection.insert(aniclip);
	else
		m_inplay_animation_clip_collection.erase(aniclip);
}

// 添加一个动画片段
bool PawnAnimationControl::add_animation_clip(const char* ani_name, PawnAnimationClip* aniclip)
{
	if(m_ani_clip_collection.find(ani_name) != m_ani_clip_collection.end())
		return false;

	m_ani_clip_collection[ani_name] = aniclip;
	reset_iterator();

	return true;
}
// 删除一个动画片段
bool PawnAnimationControl::remove_animation_clip(const char* ani_name)
{
	IDToPawnAnimationClipMap::iterator pos = m_ani_clip_collection.find(ani_name);
	if(pos == m_ani_clip_collection.end())
		return false;

	m_inplay_animation_clip_collection.erase(pos->second);
	m_ani_clip_collection.erase(pos);
	reset_iterator();

	return true;
}
// 查找指定ID的动画片段
PawnAnimationClip* PawnAnimationControl::find_animation_clip(const char* ani_name)
{
	IDToPawnAnimationClipMap::iterator pos = m_ani_clip_collection.find(ani_name);

	if(pos == m_ani_clip_collection.end())
		return false;
	else
		return pos->second;
}

// 重置动画片段迭代器
void PawnAnimationControl::reset_iterator()
{
	m_cur_ani_clip_iterator = m_ani_clip_collection.begin();
}
// 获得动画片段，迭代器指向下一个动画片段，如迭代器无效返回0
PawnAnimationClip* PawnAnimationControl::next()
{
	PawnAnimationClip* aniclip = 0;
	
	if(m_cur_ani_clip_iterator != m_ani_clip_collection.end())
	{
		aniclip = m_cur_ani_clip_iterator->second;
		++m_cur_ani_clip_iterator;
	}

	return aniclip;
}


//////////////////////////////////////////////////////////////////////////
PawnAnimationClip::PawnAnimationClip()
{
	reset_iterator();
}

PawnAnimationClip::~PawnAnimationClip()
{
	for(PropertyModifierCollection::iterator pos = m_property_modifier_collection.begin();
		pos != m_property_modifier_collection.end(); ++pos)
	{
		delete pos->second;
	}
}

bool PawnAnimationClip::add_property_modifier(const ModifierTypeID& modifier_typeid, IPropertyModifier* modifier)
{
	if(m_property_modifier_collection.find(modifier_typeid) != m_property_modifier_collection.end())
		return false;

	m_property_modifier_collection[modifier_typeid] = modifier;
	reset_iterator();
	return true;
}

bool PawnAnimationClip::remove_property_modifier(const ModifierTypeID& modifier_typeid)
{
	PropertyModifierCollection::iterator pos = m_property_modifier_collection.find(modifier_typeid);
	if(pos == m_property_modifier_collection.end())
		return false;

	m_property_modifier_collection.erase(modifier_typeid);
	reset_iterator();
	return true;
}

IPropertyModifier* PawnAnimationClip::find_property_modifier(const ModifierTypeID& modifier_typeid)
{
	PropertyModifierCollection::iterator pos = m_property_modifier_collection.find(modifier_typeid);
	if(pos == m_property_modifier_collection.end())
		return false;
	else
		return pos->second;
}

void PawnAnimationClip::reset_iterator()
{
	m_cur_property_modifier_itr = m_property_modifier_collection.begin();
}

IPropertyModifier* PawnAnimationClip::next()
{
	IPropertyModifier* modifier = 0;
	if(m_cur_property_modifier_itr != m_property_modifier_collection.end())
	{
		modifier = m_cur_property_modifier_itr->second;
		++m_cur_property_modifier_itr;
	}

	return modifier;
}

void PawnAnimationClip::play(bool isplay, int anitime)
{
	for(PropertyModifierCollection::iterator pos = m_property_modifier_collection.begin();
		pos != m_property_modifier_collection.end(); ++pos)
	{
		IPropertyModifier* modifier = pos->second;
		//modifier->execute(isplay, anitime); //编译没通过，暂时注释掉
	}
}

void PawnAnimationClip::update(int t)
{
	for(PropertyModifierCollection::iterator pos = m_property_modifier_collection.begin();
		pos != m_property_modifier_collection.end(); ++pos)
	{
		IPropertyModifier* modifier = pos->second;
		modifier->update(t);
	}
}

// 由谁来负责销毁，需要为了不拉不拉添加一个创建器


// 具体的实现类
/*
class PositionPropertyModifierCreator : public IPropertyModifierCreator
{
public:
	virtual IPropertyModifier* create(const std::string& fixparas)
	{
		return createPropertyModifier(&TestE::foo, fixparas);
	}
};
*/

class PropertyModifierStreamableFactory : public IPropertyModifierFactory
{
public:
	PropertyModifierStreamableFactory();
	~PropertyModifierStreamableFactory() {};

	IPropertyModifier* create_property_modifier(const ModifierTypeID& modifier_typeid);
	IPropertyModifier* create_property_curve_modifier(const ModifierTypeID& modifier_typeid);

protected:
	typedef std::pair<PawnTypeID, PawnPropertyTypeID> CreatorKey;
	typedef std::map<CreatorKey, IPropertyModifierCreator*> CreatorMap;
	CreatorMap m_creator_map;
};


/// 参数类：提供函数参数的读写和记录功能
template<typename T>
struct FunctionParameter
{
	T parameter;

	void parse(const std::string& v)
	{
		// 编译错误
		void error[];
	}
};

template<>
struct FunctionParameter<int>
{
	int parameter;

	void parse(const std::string& v)
	{
		parameter = atoi(v.c_str());
	}
};

template<>
struct FunctionParameter<float>
{
	float parameter;

	void parse(const std::string& v)
	{
		parameter = (float)atof(v.c_str());
	}
};

template<>
struct FunctionParameter<std::string>
{
	std::string parameter;

	void parse(const std::string& v)
	{
		parameter = v;
	}
};

template<>
struct FunctionParameter<H3DVec3>
{
	H3DVec3 parameter;

	void parse(const std::string& v)
	{
		// 继续分割，然后调用三个atof
	}
};

/// 修改器实现类，一次性修改
template<typename TClass, typename TResult, typename TPara0>
class PropertyModifierUsingTemplate_1 : public IPropertyModifier
{
public:
	PropertyModifierUsingTemplate_1(TClass* t, TResult(TClass::*pfunc)(TPara0))
		: m_pfunc(pfunc)
	{
	}

	void update(TClass* t)
	{
		// 调用属性修改函数
		(t->*m_pfunc)(m_p0.parameter);
	}

	virtual void setValue(const char* value)
	{
		m_value = value;
		m_p0.parse(value);
	}

	virtual const char* getValue() { return m_value.c_str(); }

protected:
	TResult(TClass::*m_pfunc)(TPara0);
	FunctionParameter<TPara0> m_p0;
	std::string m_value;
};

template<typename TClass, typename TResult, typename TPara0, typename TPara1>
class PropertyModifierUsingTemplate_2 : public IPropertyModifier
{
public:
	PropertyModifierUsingTemplate_2(TClass* t, TResult(TClass::*pfunc)(TPara0, TPara1), const std::string& fixedpara)
		: m_pfunc(pfunc)
	{
		parseFixedPara(fixedpara.c_str());
	}

	void update(TClass* t)
	{
		// 调用属性修改函数
		(t->*m_pfunc)(m_p0.parameter, m_p1.parameter);
	}

	virtual void setValue(const char* value)
	{
		m_value = value;
		m_p1.parse(value);
	}

	virtual const char* getValue() { return m_value.c_str(); }

protected:
	void parseFixedPara(const char* fixedpara)
	{
		m_p0.parse(fixedpara);
	}


protected:
	TResult(TClass::*m_pfunc)(TPara0, TPara1);
	FunctionParameter<TPara0> m_p0;
	FunctionParameter<TPara1> m_p1;
	std::string m_value;
};

/**
@brief 萃取器函数的参数类型，创建相应的修改器实例
@param TClass类的类型
@param TResult类成员函数的返回值
@param TPara0类成员函数的第一个参数类型
@param pfunc类成员函数指针
@param fixed_parameters类成员函数调用时的固定参数
@return 相应的属性修改器
*/

template<typename TClass, typename TResult, typename TPara0>
IPropertyModifier* createPropertyModifier(Pawn* pawn, const ModifierTypeID& modifierid, TResult(TClass::*pfunc)(TPara0))
{
	return new PropertyModifierUsingTemplate_1<TClass, TResult, TPara0>(t, pfunc);
}
template<typename TClass, typename TResult, typename TPara0, typename TPara1>
IPropertyModifier* createPropertyModifier(Pawn* pawn, const ModifierTypeID& modifierid, TResult(TClass::*pfunc)(TPara0, TPara1), const std::string& fixed_parameters)
{
	return new PropertyModifierUsingTemplate_2<TClass, TResult, TPara0, TPara1>(t, pfunc, fixed_parameters);
}
template<typename TClass, typename TResult, typename TPara0, typename TPara1, typename TPara2>
IPropertyModifier* createPropertyModifier(Pawn* pawn, const ModifierTypeID& modifierid, TResult(TClass::*pfunc)(TPara0, TPara1, TPara2), const std::string& fixed_parameters)
{
	return new PropertyModifierUsingTemplate_3<TClass, TResult, TPara0, TPara1, TPara2>(t, pfunc, fixed_parameters);
}

PropertyModifierStreamableFactory::PropertyModifierStreamableFactory()
{
}

IPropertyModifier* PropertyModifierStreamableFactory::create_property_modifier(const ModifierTypeID& modifier_type)
{
	return 0;
}

/************************************************************************/
/* 
set_value的偏特化部分
*/
/************************************************************************/
void set_value(H3DVec3& value, const float& elem, int index)
{
	switch(index)
	{
	case 0:
		value.x = elem;
		break;
	case 1:
		value.y = elem;
		break;
	case 2:
		value.z = elem;
		break;
	default:
		break;
	}
}

void set_value(H3DColor& value, const float& elem, int index)
{
	switch(index)
	{
	case 0:
		value.setRed(elem);
		break;
	case 1:
		value.setGreen(elem);
		break;
	case 2:
		value.setBlue(elem);
		break;
	case 3:
		value.setAlpha(elem);
		break;
	default:
		break;
	}
}
