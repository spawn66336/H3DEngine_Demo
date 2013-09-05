#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/MatFileLoader.h"
#include <iostream>

using namespace std;
/*
测试逻辑
加载一个正常（非mask，无手动lod，允许自动lod）材质
检查lod0，1，2属性是否正确
检查lod0，1，2变量获取是否正确

*/
class H3DMaterialLodTest:public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pLevelScene;

	H3DI::IModel* gModel;
	H3DMaterialLodTest(const char* case_name) : HippoTestCaseBase(case_name){}
	
	bool CompareRS(H3DI::IMatRSCon* rs1,H3DI::IMatRSCon* rs2)
	{
		return false;
	}

	bool CompareMacro(H3DI::IMatMacroCon* r1,H3DI::IMatMacroCon* r2)
	{
		return false;
	}
	bool CompareNodeVar(H3DI::IMatNodeVarCon* nodevar1,H3DI::IMatNodeVarCon* nodevar2)
	{
		int n1=nodevar1->GetCount();
		int n2=nodevar2->GetCount();
		HIPPO_EXPECT_EQ(n1,n2);

		for(int idx=0;idx<n1;++idx)
		{
			H3DI::IMatNodeVar* pVar=nodevar1->GetVarByIdx(idx);
			bool b=nodevar2->FindEqualVar(pVar);
			HIPPO_EXPECT_EQ(b,true);
		}
		return false;
	}

	bool CompareLodData(H3DI::IMatLodData* pData1,H3DI::IMatLodData* pData2)
	{
		HIPPO_EXPECT_EQ(pData1->HasTransparent(),pData2->HasTransparent());
		HIPPO_EXPECT_EQ(pData1->IsUseAutoLod(),pData2->IsUseAutoLod());
		HIPPO_EXPECT_EQ(pData1->IsTwoSides(),pData2->IsTwoSides());
		HIPPO_EXPECT_EQ(pData1->GetAlphaTestRef(),pData2->GetAlphaTestRef());
		HIPPO_EXPECT_EQ(pData1->HasAlphaTest(),pData2->HasAlphaTest());
		HIPPO_EXPECT_EQ(pData1->GetEffectType(),pData2->GetEffectType());

		//var
		H3DI::IMatNodeVarCon* nodevar1=pData1->GetNodeVars();
		H3DI::IMatNodeVarCon* nodevar2=pData2->GetNodeVars();
		bool b1=CompareNodeVar(nodevar1,nodevar2);
		HIPPO_EXPECT_EQ(b1,true);


		//forward shading rs
		H3DI::IMatRSCon* rs1=pData1->GetRS(3);
		H3DI::IMatRSCon* rs2=pData2->GetRS(3);
		bool b2=CompareRS(rs1,rs2);
		HIPPO_EXPECT_EQ(b2,true);

		//macro
		H3DI::IMatMacroCon* macro1=pData1->GetMacros();
		H3DI::IMatMacroCon* macro2=pData2->GetMacros();
		bool b3=CompareMacro(macro1,macro2);
		HIPPO_EXPECT_EQ(b3,true);


		return true;
	}
	void TestMat1()
	{
		const char* fn="../resources/art/effect/item_effect/jz_rose/jz_rose.mat";
		
		//create 
		H3DI::IMatProxy* pMatContent=MatFileLoader::LoadMatFromFile(fn,"jz_rose");
		H3DI::IMatProxy* pShaderDec=Hippo_GetIProxyFactory()->GetGlobalEnv()->CreateIMatProxyFromFile(fn,"jz_rose");

		//compare
		//lod data数量
		HIPPO_EXPECT_EQ(pMatContent->GetMatLodDateNum(),pShaderDec->GetMatLodDateNum());
		for(int i=0;i<3;++i)
		{
			H3DI::IMatLodData* pData1=pMatContent->GetLodData(i);
			H3DI::IMatLodData* pData2=pShaderDec->GetLodData(i);
			bool b=CompareLodData(pData1,pData2);
			HIPPO_EXPECT_EQ(b,true);
		}


		HIPPO_EXPECT_EQ(pMatContent->HasTransparentInDof(),pShaderDec->HasTransparentInDof());
		HIPPO_EXPECT_EQ(pMatContent->IsBeam(),pShaderDec->IsBeam());
		HIPPO_EXPECT_EQ(pMatContent->RenderInReflect(),pShaderDec->RenderInReflect());
		HIPPO_EXPECT_EQ(pMatContent->HasCustomShadow(),pShaderDec->HasCustomShadow());
		HIPPO_EXPECT_EQ(pMatContent->IsReciveFog(),pShaderDec->IsReciveFog());
		HIPPO_EXPECT_EQ(pMatContent->IsReciveLight(),pShaderDec->IsReciveLight());
		HIPPO_EXPECT_EQ(pMatContent->IsReciveShadow(),pShaderDec->IsReciveShadow());
		HIPPO_EXPECT_EQ(pMatContent->IsCastShadow(),pShaderDec->IsCastShadow());
		HIPPO_EXPECT_EQ(pMatContent->IsDrawAfterDof(),pShaderDec->IsDrawAfterDof());
		HIPPO_EXPECT_EQ(pMatContent->IsReleaseTextureImmediately(),pShaderDec->IsReleaseTextureImmediately());
		HIPPO_EXPECT_EQ(pMatContent->IsUseVertexColor0(),pShaderDec->IsUseVertexColor0());
		HIPPO_EXPECT_EQ(pMatContent->IsUseVertexColor1(),pShaderDec->IsUseVertexColor1());
		HIPPO_EXPECT_EQ(pMatContent->GetLightMode(),pShaderDec->GetLightMode());
		HIPPO_EXPECT_EQ(pMatContent->HasNormalMap(),pShaderDec->HasNormalMap());
		HIPPO_EXPECT_EQ(pMatContent->HasBakeTexture(),pShaderDec->HasBakeTexture());


	}
	//对一个pShader进行检测
	void TestMat()
	{
		//H3DI::INewShader* pShader=
		//	Hippo_GetIRender()->GetNewShader("../resources/only_for_test/h3dmaterial_test/h3dmaterial_test_1.mat","test1");
		//H3DI::IMatProxy* pShaderDec=Hippo_GetIProxyFactory()->CreateIMatProxy(pShader);

		////检测是否有2个matlod
		//int matlodnum=pShaderDec->GetMatLodDateNum();
		//HIPPO_EXPECT_EQ(matlodnum,2);

		//const int lod=0;
		//bool bAtest=pShaderDec->HasAlphaTest(lod);
		//HIPPO_EXPECT_EQ(bAtest,true);

		//bool bTran=pShaderDec->HasTransparent(lod);
		//HIPPO_EXPECT_EQ(bTran,true);

		//bool bNorm=pShaderDec->HasNormalMap(lod);
		//HIPPO_EXPECT_EQ(bNorm,true);

		//bool bTs=pShaderDec->IsTwoSides(lod);
		//HIPPO_EXPECT_EQ(bTs,false);

		//bool bBkt=pShaderDec->HasBakeTexture();
		//HIPPO_EXPECT_EQ(bBkt,false);

		//int nRef=pShaderDec->GetAlphaTestRef(lod);
		//HIPPO_EXPECT_EQ(nRef,64);

		//int tEff=pShaderDec->GetEffectType(lod);
		//HIPPO_EXPECT_EQ(tEff,0);

		//pShaderDec->Release();
		//pShader->Release();

	}

	//
	virtual void InitScene()
	{
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();

		TestMat1();

	}

	 void CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);
	}

	//无论是lod0，1，2 都需要测试的
	void TestMatCommon(H3DI::INewShader* pShader,int lod)
	{

	}


	virtual bool AutoRunExec()
	{
		if(Spin_N_Frame(150)) //什么都不做，10s钟后切换到下一个状态
		{
			return true;
		}
		return false;
	}
	virtual bool ManualRunExec()
	{
		return false;
	}




private:
	static HippoTestCaseBase* const testcase_;
};

ADD_TESTCASE("H3DMaterialTest",H3DMaterialLodTest)

