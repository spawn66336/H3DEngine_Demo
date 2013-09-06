#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "engine_interface_internal.h"
#include <iostream>

using namespace std;

/*
测试逻辑：
测试IShaderInterface，获取IShaderConst，并获取运行期值
*/

class TestIShaderInterface:public HippoTestCaseBase
{
public:
	HippoLevelScene* m_pLevelScene;

	H3DI::IActor* pactor;
	H3DI::IModel* gModel;
	ISpecialEffect* gSpe;
	Hippo_StateMachine sm;

	H3DI::IMatProxy* pActorMat;

	TestIShaderInterface(const char* case_name) : HippoTestCaseBase(case_name){}
	virtual void CreateActor()
	{
		pactor=m_pLevelScene->CreateActor(false,0);
		H3DI::INewShader* m_pNewShader = pactor->GetNewShaderByIndex(0);
		pActorMat=Hippo_GetIGlobalEnv()->CreateIMatProxyFromINewShader(m_pNewShader);

		//ChangeDress();
	}

	SCSTATE OnState0()
	{
		if(Spin_N_Frame(50)) //什么都不做，render 1s
		{
			CreateActor();
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}

	SCSTATE OnState1()
	{
		if(Spin_N_Frame(50)) //什么都不做，render 1s
		{
			m_pLevelScene->DelActor(pactor);
			Hippo_GetIRender()->ClearMaterialLib();
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}
	SCSTATE OnState2()
	{
		sm.m_current_state=-1; //hack it
		return STATE_CHANGE;
	}
	//
	virtual void InitScene()
	{
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		m_pLevelScene=Hippo_GetSceneManager()->CreateEmptyLevel();
		//sm.AddState(0,std::tr1::bind(&TestIShaderInterface::OnState0,this));
		//sm.AddState(1,std::tr1::bind(&TestIShaderInterface::OnState1,this));
		//sm.AddState(2,std::tr1::bind(&TestIShaderInterface::OnState2,this));
		CreateActor();
	}



	virtual void ChangeDress()
	{

		pactor->SetBodyPart("../resources/art/role/bodypart/female/socks/117013001/117013001.bpt");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/body/114001001/114001001.BPT");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/face/112002001/112002001.BPT");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/hair/113004001/113004001.BPT");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/hand/115002001/115002001.bpt");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/shoe/118006001/118006001.bpt");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/strippedbody/11a001.BPT");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/suit/119005001/1193005001/1193005001.bpt");
		pactor->SetBodyPart("../resources/art/role/bodypart/female/trousers/116004001/116004001.bpt");
	}

	void CleanUpScene()
	{
		m_pLevelScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pLevelScene);

	}


	bool ManualRunExec()
	{
		sm.Run();
		return false;
	}

	virtual bool AutoRunExec()
	{
		if(Spin_N_Frame(150)) //什么都不做，10s钟后切换到下一个状态
		{
			m_state=CASE_END_EXECING;
			return true;
		}
		return false;
	}

	void testInterface()
	{
		//static const char* szSCNames[] = 
		//{
		//	"TextureSample2D_3_TexFileName",
		//	"TextureSample2D_4_TexFileName",
		//	"TextureSample2D_5_TexFileName",
		//	"WorldMatrix",
		//	"RGBFactor",
		//	"SkinColor",
		//	"tmpAlpha",
		//	"fakespotlight00",
		//	"fakespotlight01",
		//	"fakespotlight02",
		//	"g_BoneMatrix",
		//	"g_InstancePose",
		//	0
		//};

		/// 没有绘制过的话，pshader是得不到任何内容的
		H3DI::IShaderProxy* pShader =  pActorMat->GetCachedShaderBytype(3);
		const char* filename = pShader->GetFxFileName();

		Hippo_WriteConsole(CC_WHITE,"fxfileName ……%s", filename);


		const char* allSCNames[32];
		memset(allSCNames, 0, 32*sizeof(int)	);
		unsigned int sccount = pShader->GetShaderConstCount(allSCNames, 32);

		unsigned int i = 0; 

		while(allSCNames[i])
		{
			H3DI::IShaderConstProxy* pshaderconst = pShader->GetShaderConstByName(allSCNames[i]);
			if (pshaderconst)
			{
				const char* type = pshaderconst->GetClassType();
				const char* name =  pshaderconst->GetName();
				H3DI::IShaderConstType itpye = pshaderconst->GetType();
				Hippo_WriteConsole(CC_WHITE,"	shaderconst …name:%s  type:%s", name, type);
				void* value = pshaderconst->GetValue();
				H3DI::ITexture* ptexture = 0;
				switch(itpye)
				{
				case H3DI::ISCT_INT:
					Hippo_WriteConsole(CC_WHITE,"	shaderconst …ISCT_INT:%d", pshaderconst->GetValue_int());
					break;
				case 	H3DI::ISCT_TEXTURE:
					ptexture = pshaderconst->GetValue_texture();
					//Hippo_WriteConsole(CC_WHITE,"	shaderconst …ISCT_INT:%d", pshaderconst->GetValue_int());
					break;
				case 	H3DI::ISCT_FLOAT1:
					Hippo_WriteConsole(CC_WHITE,"	shaderconst …ISCT_FLOAT1:%f", pshaderconst->GetValue_float1());
					break;
				case 	H3DI::ISCT_FLOAT4:
					//Hippo_WriteConsole(CC_WHITE,"	shaderconst …ISCT_INT:%d", pshaderconst->GetValue_int());
					break;
				default: 
					break;
				}
			}
			i++;
		}

		/// 测试析构：
		pShader->Release();
		//H3DI::IShaderConstProxy* pShaderConsts[]

	}

	virtual int  OnKeyDown(unsigned int key)
	{
		switch( key )
		{
		case 'T':
			{
				testInterface();

				return 0;
			}
		default:
			return 0;
		}
		return 0;
	}

private:
	static HippoTestCaseBase* const testcase_;
};




//class TestIShaderInterface2:public TestIShaderInterface
//{
//public:
//	TestIShaderInterface2(const char* case_name) : TestIShaderInterface(case_name){}
//	virtual void ChangeDress()
//	{
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)1,"../resources/art/role/link/female/4110/4110001001/4110001001.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)2,"../resources/art/role/link/female/4112/4112001001/4112001001.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)3,"../resources/art/role/link/female/4113/4113006002/4113006002.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)4,"../resources/art/role/link/female/4114/4114004003/4114004003.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)5,"../resources/art/role/link/female/4115/4115001001/4115001001.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)6,"../resources/art/role/link/female/4116/4116009002/4116009002.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)7,"../resources/art/role/link/female/4117/4117004001/4117004001.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)8,"../resources/art/role/link/female/4118/4118006001/4118006001.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)9,"../resources/art/role/link/female/4119/4119007001/4119007001.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)10,"../resources/art/role/link/female/4120/4120007001/4120007001.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)11,"../resources/art/role/link/female/4121/4121007001/4121007001.spe");
//		pactor->SetAdornment((H3DI::EActorAdornmentPosition)0,"../resources/art/role/link/female/4111/4111006001/4111006001.spe");
//	}
//
//private:
//	static HippoTestCaseBase* const testcase_;
//};

ADD_TESTCASE("TestIShaderInterface",TestIShaderInterface)
//ADD_TESTCASE("TestIShaderInterface2",TestIShaderInterface2)