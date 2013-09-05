#pragma once
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/MatFileLoader.h"
#include <map>
#include <utility>
#include <sstream>
#include <vector>

using namespace std;


//辅助函数 
template <typename T>
T StringToNum(const string& str)
{
	T num;
	istringstream isstr(str);
	isstr >> num;
	return num;
}

template<typename T>
string NumToString(const T& t)
{
	ostringstream osstr;
	osstr << t;
	return osstr.str();
}

class HippoMatNodeVar ;
class HippoMatLod;
class SpecialShaderConst;
class Macro;

struct PSIN_Semantic;
struct VSIN_Semantic;
struct VSOUT_Semantic;
class HeaderCode;
class Technique;
class HippoShaderContent;
 


class AutoLod1
{
private:
	string fContrast_value;
	string fLightness_value;
	string fSpecStrength_value;
	string fSpecPower_value;

public:
	AutoLod1(string v1,string v2,string v3 ,string v4):fContrast_value(v1),fLightness_value(v2),fSpecStrength_value(v3),fSpecPower_value(v4)
	{

	}
	AutoLod1()
	{

	}
	void SetAutoLod1(string v1,string v2,string v3 ,string v4)
	{
		fContrast_value =v1;
		fLightness_value = v2;
		fSpecStrength_value = v3;
		fSpecPower_value = v4;
	}

};


//解析xml后对这里的元素赋值
class HippoMatProxy :public H3DI::IMatProxy
{
public:
	virtual void Release()
	{
		
	}
	virtual int GetMatLodDateNum()
	{
		return 0;
	}
	virtual H3DI::IMatLodData* GetLodData(int idx)
	{
		return 0;
	}
	virtual bool HasBakeTexture()
	{
		return 0;
	}
	virtual bool HasTransparentInDof()
	{
		return 0;
	}
	virtual bool IsBeam()
	{
		return 0;
	}
	virtual bool RenderInReflect()
	{
		return 0;
	}
	virtual bool HasCustomShadow()
	{
		return 0;
	}
   //是否接受阴影
	virtual bool IsReciveShadow()
	{
		return 0;
	}
//是否投射阴影
	virtual bool IsCastShadow()
	{
		return 0;
	}

	virtual bool IsReciveFog()
	{
		return 0;
	}
	virtual bool IsReciveLight()
	{
		return 0;
	}
	virtual bool IsDrawAfterDof()
	{
		return 0;
	}
	virtual bool IsReleaseTextureImmediately()
	{
		return 0;
	}
	virtual bool IsUseVertexColor0()
	{
		return 0;
	}
	virtual bool IsUseVertexColor1()
	{
		return 0;
	}
	virtual bool GetLightMode()
	{
		return 0;
	}
	virtual bool HasNormalMap()
	{
		return 0;
	}
	virtual const char* GetFxFileFullName(int shadercache)
	{
		return GetFxFileName();
	}
	int  GetSortKey()
	{
		return -1;
	}
	H3DI::IShaderProxy* GetCachedShaderBytype(int shadercache)
	{
		return 0;
	}

public:
	HippoMatProxy(void){};
	~HippoMatProxy(void){};

private:
	string fxFileName;
	//int UseAutoMatLod;
	string UseAutoMatLod;
public:
	string GetUseAutoMatLod()
	{
		return UseAutoMatLod;
	}
	void SetUseAutoMatLod(string val)
	{
		//UseAutoMatLod = StringToNum<int>(val);
		UseAutoMatLod = val;
	}


	const char* GetFxFileName()
	{
		return fxFileName.c_str();
	}

	void SetFxFileName(string name)
	{
		fxFileName = name;
	}


	//内部类
	class HippoShaderContent
	{
	private:
		map<string,Macro> mapMacro;
		map<string,SpecialShaderConst> mapSpecialSC;
		map<string,HeaderCode> mapHeaderCode;
		map<string,Technique> mapTechnique;

	public:
		HippoShaderContent()
		{
		 
		}
		map<string,Macro>& GetMacroMap()
		{
			return mapMacro;
		}
		map<string ,SpecialShaderConst>& GetSpecialSCMap()
		{
			return mapSpecialSC;
		}
		map<string ,HeaderCode>& GetHeaderCodeMap()
		{
			return mapHeaderCode;
		}

		map<string,Technique>& GetTechniqueMap()
		{
			return mapTechnique;
		}

	};

	private:

		map<string,HippoMatLod> mapMatLod;
		map<string,HippoMatNodeVar>  mapNodeVar;
		map<string,HippoMatNodeVar> mapGlobalVar;
		HippoShaderContent CastShadow_Shader;
		HippoShaderContent ForwardRenderingSM3_Shader;
		HippoShaderContent Lpp_PreLight_Shader;
		HippoShaderContent Lpp_Shading_Shader;
		AutoLod1 autoLod;

	public:

		AutoLod1& GetAutoLod1()
		{
			return autoLod;
		}
		map<string,HippoMatLod>& GetMatLodMap()
		{
			return mapMatLod;
		}
		map<string ,HippoMatNodeVar>& GetNodeVarMap()
		{
			return mapNodeVar;
		}
		map<string ,HippoMatNodeVar>& GetGlobalVarMap()
		{
			return mapGlobalVar;
		}

		HippoShaderContent* GetShaderContent(string type)
		{
			if (type=="CastShadow_Shader")
			{
				return &CastShadow_Shader;
			}
			if ( type=="ForwardRenderingSM3_Shader" )
			{
				return &ForwardRenderingSM3_Shader;
			}
			if ( type== "Lpp_PreLight_Shader")
			{
				return &Lpp_PreLight_Shader;
			}
			if ( type== "Lpp_Shading_Shader")
			{
				return &Lpp_Shading_Shader;
			}
			return NULL;
		}
	
};

class Macro
{
private: 
	string name;
	string v;
public:
	Macro(string _name,string _v)
	{
		name = _name;
		v = _v;
	}
	Macro() 
	{

	}
};

//jiegou
class SpecialShaderConst
{
private:
	string Semantic;
	string share;
public:
	SpecialShaderConst(string sem,string share)
	{
		Semantic = sem;
		this->share = share;
	}
	SpecialShaderConst()
	{

	}
};




class HeaderCode
{

public:
	HeaderCode(string _name,string _type,string _semantic,string _AssoVar,string _AssoObject,string _AssoObjVar,string _share,string _private,string _bFrameWork,string _SamplerState)
	  :name(_name),type(_type),semantic(_semantic),AssoVar(_AssoVar),AssoObject(_AssoObject),AssoObjVar(_AssoObjVar),share(_share),private_(_private),bFrameWork(_bFrameWork),SamplerState(_SamplerState)
	  {

	  }
	HeaderCode()
	{

	}
private:
	string name;
	string type;
	string semantic;
	string AssoVar;
	string AssoObject;
	string AssoObjVar;
	string share;
	string private_;
	string bFrameWork;
	string SamplerState;

public:
	
		

};

class RenderState
{
public:
	RenderState(string varname,string typ,string val,string seman)
		:varName(varname),type(typ),value(val),sem(seman){}
	RenderState()
	{

	}
private:
	string varName;
	string type;
	string value;
	string sem;
};

struct VSIN_Semantic
{
public:
	VSIN_Semantic(string str)
	{
		Semantic = str;
	}
private:

	string Semantic;
};

struct VSOUT_Semantic
{
	string Semantic;
};

struct PSIN_Semantic
{
	string Semantic;
};

class Pass
{
private:
	string index;
	map<string,RenderState> mapRenderState;
	vector<VSIN_Semantic> vecVsIn;
	vector<VSOUT_Semantic> vecVsOut;
	vector<PSIN_Semantic> vecPsIn;


public:
	void setIndex(string val)
	{
		index = val;
	}
	map<string,RenderState>& GetRenderStateMap()
	{
		return mapRenderState;
	}
	vector<VSIN_Semantic>& GetVsInVec()
	{
		return vecVsIn;
	}
	vector<VSOUT_Semantic>& GetVsOutVec()
	{
		return vecVsOut;
	}
	vector<PSIN_Semantic>& GetPsInVec()
	{
		return vecPsIn;
	}

	~Pass()
	{

	}
};

class TechniqueCode
{
public:
	TechniqueCode(string _name,string passnum):name(_name),PassNum(passnum)
	{

	}
	TechniqueCode()
	{

	}
	void SetTechCode(string name,string pass)
	{
		this->name = name;
		PassNum = pass;
	}
private:
	string name;
	string PassNum;

};
class Technique
{
private:
	Pass pass;
	TechniqueCode tc;
public:
	 Pass* GetPass()
	 {
		 return &pass;
	 }

	 TechniqueCode* GetTechCode()
	 {
		 return &tc;
	 }

	 void SetTechniqueCode(string name, string pass)
	 {
		// tc = TechniqueCode(name,pass);
	 }
	 Technique()
	 {

	 }
	 
	
};

class HippoMatNodeVar : public H3DI::IMatNodeVar
{
public:
	HippoMatNodeVar(string _name,string _typ,string _sem,string _value,string _node):
	  varName(_name),type(_typ),sem(_sem),value(_value),Node(_node){ }
	  HippoMatNodeVar()
	  {

	  }

	  ~HippoMatNodeVar(void) { }

public:

	virtual const char* GetName()
	{
		return varName.c_str();
	}

	virtual const char* GetType()
	{
		return type.c_str();
	}

	virtual float GetValue_float()
	{
		if (string("DT_FLOAT") == type)
		{
			return StringToNum<float>(value);
		}
		
	}
	virtual void SetValue_float(float f)
	{
		if (string("DT_FLOAT") == type)
		{
			value = NumToString<float>(f);
		}

	}

	virtual int GetValue_int()
	{
		if (string("DT_INT") == type)
		{
			return StringToNum<int>(value);
		}
	}
	virtual void SetValue_int(int n)
	{
		if (string("DT_INT") == type)
		{
			value = NumToString<int>(n);
		}

	}

	virtual const char* GetValue_str()
	{
		if (string("DT_STRING") == type)
		{
			return value.c_str();
		}

	}
	virtual void SetValue_str(const char* str)
	{

		if (string("DT_STRING") == type)
		{
			value = string(str);
		}
	}

private:
	std::string varName;
	std::string type;
	std::string value;
	std::string Node;
	std::string sem;
};

class HippoMatLod
{
private:
	string level;
	string mat;
	string matball;
	H3DI::IMatProxy* mHippoMat;
public:
	HippoMatLod()
	{

	}
	~HippoMatLod()
	{
		//delete mHippoMat;
		mHippoMat = 0;
	}

	HippoMatLod(string _lev,string _mat,string _matball,H3DI::IMatProxy* mp):level(_lev),mat(_mat),matball(_matball),mHippoMat(mp)
	{

	}
};

