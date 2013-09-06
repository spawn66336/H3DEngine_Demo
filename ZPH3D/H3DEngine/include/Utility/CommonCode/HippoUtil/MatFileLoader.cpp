#include "MatFileLoader.h"
#include "engine_interface_internal.h"
#include "../../FunctionalTest/TestCase/HippoMatProxy.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <string>


using namespace std;


bool ParseTechniqueCodeTest(TiXmlElement* pTechniqueCode,HippoMatProxy* &matLoad,string shadertype,string techtype)
{
	const char* name = pTechniqueCode->Attribute("name");
	if (NULL ==name)
	{
		name = "";
	}
	const char* PassNum = pTechniqueCode->Attribute("PassNum");
	if (NULL == PassNum)
	{
		PassNum = "";
	}
	matLoad->GetShaderContent(shadertype)->GetTechniqueMap()[techtype].GetTechCode()->SetTechCode(name,PassNum);
	return true;
}

//解析pass
bool ParsePassTest(TiXmlElement* pPass,HippoMatProxy* &matLoad,string shadertype,string techtype)
{
	string idx = pPass->Attribute("idx");
	if (idx!="")
	{
		matLoad->GetShaderContent(shadertype)->GetTechniqueMap()[techtype].GetPass()->setIndex(idx);
	}
	

	TiXmlElement* pRenderState = pPass->FirstChildElement("RenderState");
	if (pRenderState == NULL)
	{
		return false;
	}
	while(pRenderState)
	{

		const char* varName =pRenderState->Attribute("varName");
		if (NULL == varName)
		{
			return false;
		}
		const char* type = pRenderState->Attribute("type");
		if (NULL == type)
		{
			return false;
		}
		const char* value= pRenderState->Attribute("value");
		if (NULL == value)
		{
			value = "";
		}
		const char* sem= pRenderState->Attribute("sem");
		if (NULL == sem)
		{
			sem = "";
		}

		RenderState rs =  RenderState(varName,type,value,sem);

		matLoad->GetShaderContent(shadertype)->GetTechniqueMap()[techtype].GetPass()->GetRenderStateMap()[varName] = rs;
		pRenderState = pRenderState->NextSiblingElement("RenderState");

	}

	TiXmlElement* pVsIn= pPass->FirstChildElement("VSIN_Semantic");
	while(pVsIn)
	{

		string Semantic =pVsIn->Attribute("Semantic");


		VSIN_Semantic vs_sem =VSIN_Semantic(Semantic);
		matLoad->GetShaderContent(shadertype)->GetTechniqueMap()[techtype].GetPass()->GetVsInVec().push_back(vs_sem);
		pVsIn = pVsIn->NextSiblingElement("VSIN_Semantic");
	}

	TiXmlElement* pVsOut= pPass->FirstChildElement("VSOUT_Semantic");
	while(pVsOut)
	{

		string Semantic =pVsOut->Attribute("Semantic");


		struct	VSOUT_Semantic vsout;
		vsout.Semantic = Semantic;
		matLoad->GetShaderContent(shadertype)->GetTechniqueMap()[techtype].GetPass()->GetVsOutVec().push_back(vsout);
		pVsOut = pVsOut->NextSiblingElement("VSOUT_Semantic");
	}

	TiXmlElement* pPsIn= pPass->FirstChildElement("PSIN_Semantic");
	while(pPsIn)
	{

		string Semantic =pPsIn->Attribute("Semantic");

		struct	 PSIN_Semantic psin;
		psin.Semantic = Semantic;
		matLoad->GetShaderContent(shadertype)->GetTechniqueMap()[techtype].GetPass()->GetPsInVec().push_back(psin);
		pPsIn = pPsIn->NextSiblingElement("PSIN_Semantic");
	}

	return true;
}

bool ParseTechniqueTest(TiXmlElement* pTech,HippoMatProxy* &matLoad,string shaderType,string techType)
{
	TiXmlElement* pPass = pTech->FirstChildElement("Pass");
	if (pPass)
	{
		ParsePassTest(pPass,matLoad,shaderType,techType);

	}

	TiXmlElement* pTechniqueCode= pTech->FirstChildElement("TechniqueCode");
	if (pTechniqueCode)
	{
		ParseTechniqueCodeTest(pTechniqueCode,matLoad,shaderType,techType);

	}
	return true;

}



bool ParseSpecialSC(TiXmlElement* pSpicialSC,HippoMatProxy* &matLoad,string type)
{
	while(pSpicialSC)
	{
		const char* semantic = pSpicialSC->Attribute("Semantic");
		if (semantic == NULL)
		{
			semantic = "";
		}
		const char* share = pSpicialSC->Attribute("share");
		if (NULL == share)
		{
			share = "";
		}
		SpecialShaderConst ssc =  SpecialShaderConst(semantic,share);

		//matLoad->GetShaderContent("CastShadow_Shader")->GetSpecialSCMap().insert(make_pair(semantic,ssc));
		matLoad->GetShaderContent(type)->GetSpecialSCMap()[semantic] = ssc;
	
		pSpicialSC = pSpicialSC->NextSiblingElement("SpecialShaderConst");
	}

	return true;
}

bool ParseHeaderCode(TiXmlElement* pHeaderCode,HippoMatProxy* &matLoad,string Tp)
{
	//对于必须有的属性 返回false
	while(pHeaderCode)
	{
		TiXmlElement* pHeadCodeAtt = pHeaderCode->FirstChildElement("HeadCodeAtt");
		if (pHeadCodeAtt == NULL)
		{
			return false;
		}

		const char* name = pHeadCodeAtt->Attribute("name");
		if (NULL == name)
		{
			return false;
		}
		const char* type = pHeadCodeAtt->Attribute("type");
		if (NULL == type)
		{
			type = "";
		}
		const char* semantic = pHeadCodeAtt->Attribute("semantic");
		if (NULL ==semantic)
		{
			semantic = "";
		}
		const char* AssoVar = pHeadCodeAtt->Attribute("AssoVar");
		if (NULL == AssoVar)
		{
			AssoVar = "";
		}
		const char* AssoObject = pHeadCodeAtt->Attribute("AssoObject");
		if (NULL == AssoObject)
		{
			AssoObject = "";
		}
		const char* AssoObjVar = pHeadCodeAtt->Attribute("AssoObjVar");
		if (NULL == AssoObjVar)
		{
			AssoObjVar = "";
		}
		const char* share = pHeadCodeAtt->Attribute("share");
		if (NULL == share)
		{
			share = "";
		}
		const char* private_ = pHeadCodeAtt->Attribute("private");
		if (NULL == private_)
		{
			private_ = "";
		}
		const char* FinalAlphaMap = pHeadCodeAtt->Attribute("FinalAlphaMap");
		if (NULL == FinalAlphaMap)
		{
			FinalAlphaMap = "";
		}
		const char* bFrameWork = pHeadCodeAtt->Attribute("bFrameWork");
		if (bFrameWork == NULL)
		{
			bFrameWork = "";
		}
		const char* SamplerState = pHeadCodeAtt->Attribute("SamplerState");
		if (NULL == SamplerState)
		{
			SamplerState = "";
		}

		HeaderCode hcode = HeaderCode(name,type,semantic,AssoVar,AssoObject,AssoObjVar,share,private_,bFrameWork,SamplerState);
		matLoad->GetShaderContent(Tp)->GetHeaderCodeMap()[name] = hcode;

		pHeaderCode = pHeaderCode->NextSiblingElement("HeaderCode");
	}
	
	/*while(pHeadCodeAtt)
	{
		
		pHeadCodeAtt = pHeadCodeAtt->NextSiblingElement("");
	}*/
	return true;
}

bool ParseMacro(TiXmlElement* pMacro,HippoMatProxy* matLoad,string type)
{
	while(pMacro)
	{
		const char* name = pMacro->Attribute("name");
		if (name == NULL)
		{
			return false;
		}
		const char* v = pMacro->Attribute("v");
		if (NULL == v)
		{
			v = "";
		}
		Macro mac = Macro(name,v);

		matLoad->GetShaderContent(type)->GetMacroMap()[name] = mac;

		pMacro = pMacro->NextSiblingElement("Macro");
	}

	return true;
}

//shadercontent解析
bool ParseShaderContent(TiXmlElement* pCastShadow,HippoMatProxy* &matLoad,string type)
{

	TiXmlElement* pMacro = pCastShadow->FirstChildElement("Macro");
	if (pMacro)
	{
		ParseMacro(pMacro,matLoad,type);
	}

	TiXmlElement* pSpicialSC = pCastShadow->FirstChildElement("SpecialShaderConst");
	if (pSpicialSC)
	{
		ParseSpecialSC(pSpicialSC,matLoad,type);

	}

	TiXmlElement* pHeaderCode = pCastShadow->FirstChildElement("HeaderCode");
	if (pHeaderCode)
	{
		ParseHeaderCode(pHeaderCode,matLoad,type);

	}

	TiXmlElement*  pCastShadow_Technique =  pCastShadow->FirstChildElement("CASTSHADOW_TECHNIQUE");
	if (pCastShadow_Technique)
	{
		ParseTechniqueTest(pCastShadow_Technique,matLoad,type,"CASTSHADOW_TECHNIQUE");
	}
		

	TiXmlElement*  pSHADING_TECHNIQUE =  pCastShadow->FirstChildElement("SHADING_TECHNIQUE");
	if (pSHADING_TECHNIQUE)
	{
		ParseTechniqueTest(pSHADING_TECHNIQUE,matLoad,type,"SHADING_TECHNIQUE");
	}



	TiXmlElement*  pLIGHTMAP_TECHNIQUE =  pCastShadow->FirstChildElement("LIGHTMAP_TECHNIQUE");
	if (pLIGHTMAP_TECHNIQUE)
	{

		ParseTechniqueTest(pLIGHTMAP_TECHNIQUE,matLoad,type,"LIGHTMAP_TECHNIQUE");
	}


	TiXmlElement*  pINSTANCING_TECHNIQUE =  pCastShadow->FirstChildElement("INSTANCING_TECHNIQUE");
	if (pINSTANCING_TECHNIQUE)
	{
		ParseTechniqueTest(pINSTANCING_TECHNIQUE,matLoad,type,"INSTANCING_TECHNIQUE");
	}


	TiXmlElement*  pLPP_PRELIGHT_TECHNIQUE =  pCastShadow->FirstChildElement("LPP_PRELIGHT_TECHNIQUE");
	if (pLPP_PRELIGHT_TECHNIQUE)
	{

		ParseTechniqueTest(pLPP_PRELIGHT_TECHNIQUE,matLoad,type,"LPP_PRELIGHT_TECHNIQUE");

	}

	TiXmlElement*  pLPP_PRELIGHTINSTANCING_TECHNIQUE =  pCastShadow->FirstChildElement("LPP_PRELIGHTINSTANCING_TECHNIQUE");
	if (pLPP_PRELIGHTINSTANCING_TECHNIQUE)
	{
		ParseTechniqueTest(pLPP_PRELIGHTINSTANCING_TECHNIQUE,matLoad,type,"LPP_PRELIGHTINSTANCING_TECHNIQUE");
	}

	TiXmlElement*  pLPP_SHADING_TECHNIQUE =  pCastShadow->FirstChildElement("LPP_SHADING_TECHNIQUE");
	if (pLPP_SHADING_TECHNIQUE)
	{
		ParseTechniqueTest(pLPP_SHADING_TECHNIQUE,matLoad,type,"LPP_SHADING_TECHNIQUE");
	}

	TiXmlElement*  pLPP_SHADINGINSTANCING_TECHNIQUE =  pCastShadow->FirstChildElement("LPP_SHADINGINSTANCING_TECHNIQUE");
	if (pLPP_SHADINGINSTANCING_TECHNIQUE)
	{
		ParseTechniqueTest(pLPP_SHADINGINSTANCING_TECHNIQUE,matLoad,type,"LPP_SHADINGINSTANCING_TECHNIQUE");
	}
	return true;
}

bool ParseNodeVar(TiXmlElement* pNodeElem,HippoMatProxy* &matLoad)
{
	while(pNodeElem)
	{
		const char*  varName=pNodeElem->Attribute("varName");
		const char*  type=pNodeElem->Attribute("type");
		const char*  value=pNodeElem->Attribute("value");
		const char*  sem=pNodeElem->Attribute("sem");

		if (sem == NULL)
		{
			sem = "";
		}
		const char* node = pNodeElem->Attribute("node");
		if (node == NULL)
		{
			node = "";
		}

		HippoMatNodeVar nodeVar = HippoMatNodeVar(varName,type,sem,value,node);
		matLoad->GetNodeVarMap()[varName] = nodeVar;

		pNodeElem=pNodeElem->NextSiblingElement("NODE_VAR");
	}

	return true;
}

bool ParseGlobalVar(TiXmlElement* pGlobalElem,HippoMatProxy* &matLoad)
{
	while (pGlobalElem)
	{
		const char*  varName=pGlobalElem->Attribute("varName");
		const char*  type=pGlobalElem->Attribute("type");
		const char*  value=pGlobalElem->Attribute("value");
		const char*  sem=pGlobalElem->Attribute("sem");
		if (sem == NULL)
		{
			sem = "";
		}
		const char* node = pGlobalElem->Attribute("node");
		if (node == NULL)
		{
			node = "";
		}

		HippoMatNodeVar nodeVar = HippoMatNodeVar(varName,type,sem,value,node);
		matLoad->GetGlobalVarMap()[varName] = nodeVar;

		pGlobalElem=pGlobalElem->NextSiblingElement("GLOBAL_VAR");
	}
	return true;
}

bool ParseMatLod(TiXmlElement* pMatLod,HippoMatProxy* &matLoad)
{
	string level ;
	string mat ;
	string matball;

	while(pMatLod)
	{
		TiXmlElement* pMTLod = pMatLod->FirstChildElement("MTLod");
		if (pMTLod)
		{
			 level = pMTLod->Attribute("level");
			 mat = pMTLod->Attribute("mat");
			 matball = pMTLod->Attribute("matball");

			 H3DI::IMatProxy* temp = MatFileLoader::LoadMatFromFile(mat.c_str(),matball.c_str());
			 
			HippoMatLod matl =  HippoMatLod(level,mat,matball,temp);
			matLoad->GetMatLodMap()[level] = matl;
			//matLoad->GetMatLodMap().insert(make_pair(level,matl));
		}

		pMatLod = pMatLod->NextSiblingElement("MaterialLod");
	}

	return true;
}
bool ParseAutoLod1(TiXmlElement* pAutoLod1,HippoMatProxy* &matLoad)
{
	string v1;
	string v2;
	string v3;
	string v4;
	TiXmlElement* pfContrast = pAutoLod1->FirstChildElement("pfContrast");
	if (pfContrast)
	{
		v1  = pfContrast->Attribute("value");
	}
	TiXmlElement* pfLightness = pAutoLod1->FirstChildElement("fLightness");
	if (pfLightness)
	{
		v2 = pfLightness->Attribute("value");

	}
	TiXmlElement* pfSpecStrength = pAutoLod1->FirstChildElement("fSpecStrength");
	if (pfSpecStrength)
	{
		v3 = pfSpecStrength->Attribute("value");
	}
	TiXmlElement* pfSpecPower = pAutoLod1->FirstChildElement("fSpecPower");
	if (pfSpecPower)
	{
		v4 = pfSpecPower->Attribute("value");
	}

	matLoad->GetAutoLod1().SetAutoLod1(v1,v2,v3,v4);
	return true;
}
bool Load(TiXmlElement* Root,HippoMatProxy* &matLoad,bool loadmat)
{
	if (loadmat)
	{
		TiXmlElement* pMatLod = Root->FirstChildElement("MaterialLod");
		if (pMatLod)
		{
			ParseMatLod(pMatLod,matLoad);
		}

		TiXmlElement* pAutoLod1 = Root->FirstChildElement("AutoLod1Param");
		if (pAutoLod1)
		{
			ParseAutoLod1(pAutoLod1,matLoad);
		}
	}
	

	TiXmlElement* useAutoMatLod = Root->FirstChildElement("UseAutoMatLod");
	if (useAutoMatLod)
	{
		matLoad->SetUseAutoMatLod(useAutoMatLod->Attribute("value"));
	}


	TiXmlElement* FxFileName = Root->FirstChildElement("FxFileName");
	if (FxFileName)
	{
		matLoad->SetFxFileName(FxFileName->Attribute("str"));

	}

	//解析global节点
	TiXmlElement* pGlobalElem=Root->FirstChildElement("GLOBAL_VAR");
	if (pGlobalElem)
	{
		ParseGlobalVar(pGlobalElem,matLoad);
	}

	//解析nodevar节点
	TiXmlElement* pNodeElem = Root->FirstChildElement("NODE_VAR");
	if (pNodeElem)
	{
		ParseNodeVar(pNodeElem,matLoad);
	}
	//Function_Code

	//CastShadow_Shader
	TiXmlElement* pCastShadow = Root->FirstChildElement("CastShadow_Shader");
	if (pCastShadow)
	{
		ParseShaderContent(pCastShadow,matLoad,"CastShadow_Shader");
	}


	TiXmlElement* pForward = Root->FirstChildElement("ForwardRenderingSM3_Shader");
	if (pForward)
	{
		ParseShaderContent(pForward,matLoad,"ForwardRenderingSM3_Shader");
	}


	TiXmlElement* pPreLignt = Root->FirstChildElement("Lpp_PreLight_Shader");
	if (pPreLignt)
	{
		ParseShaderContent(pPreLignt,matLoad,"Lpp_PreLight_Shader");
	}


	TiXmlElement* pShading = Root->FirstChildElement("Lpp_Shading_Shader");
	if (pShading)
	{
		ParseShaderContent(pShading,matLoad,"Lpp_Shading_Shader");
	}
	return true;
}



H3DI::IMatProxy* MatFileLoader::LoadMatFromFile(const char* filename,const char* ball)
{

	
		TiXmlDocument* xmlDoc = new TiXmlDocument(filename);

		//加载mat文件
		if (! xmlDoc->LoadFile())
		{
			return 0;
		}
		TiXmlElement* root = xmlDoc->RootElement();
		TiXmlElement* pNode  = root->FirstChildElement("H3DMaterialInstance");
		while(pNode)
		{
			if (strcmp(pNode->Attribute("MaterialName"),ball)==0)
			{
				break;
			}
			pNode=pNode->NextSiblingElement("H3DMaterialInstance");
		}
		//找到指定的mat
		string tplPath;
		if (NULL == pNode)
		{
			return false;
		}
		tplPath = pNode->Attribute("TemplateFile");
		//加载tpl文件
		TiXmlDocument* subDoc = new TiXmlDocument(tplPath.c_str());
		subDoc->LoadFile();
		if (subDoc == NULL)
		{
			return 0;
		}
		TiXmlElement* Root = subDoc->RootElement();


		HippoMatProxy* matLoad = new HippoMatProxy();


		//tpl
		Load(Root,matLoad,false);
		//mat
		Load(pNode,matLoad,true);


		//关闭xml
		delete xmlDoc;
		delete subDoc;

		//注意matload的释放,如果是手动lod，其中还包含一个H3DI::IMatProxy*指针
	return matLoad;
}
