#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "../idMath/dMathHeader.h"
#include "EffectCore_dx.h"
#include <algorithm>
#include <stdio.h>
using namespace  std;

/*测试逻辑
有共同贴图的实例的测试：
测试4（派生自TestCase_RefCompare）：
准备两个DML：dml1和dml2。这两个dml的LOD0的材质中至少有一张贴图相同（Tex0）--注：这里的资源只有固有色贴图相同。
以LOD0的方式加载这两个DML，并绘制5s。
修改磁盘上Tex0文件的内容。
Reload dml1。
效果上dml1和dml2应该一起变化。
*/

class ReloadTextureTwoDml1 :public HippoTestCase_RefCompare
{
public: 
	ReloadTextureTwoDml1(const char* casename):HippoTestCase_RefCompare(casename){}
	H3DI::IModel* m_pDml1;
	H3DI::IModel* m_pDml2;
	HippoLevelScene* m_pScene;
	Hippo_StateMachine stateMachine;
	bool bAutoEnd;
private:
	static HippoTestCaseBase* const testcase_;
public:

	virtual int GetMatLod(){return 0;}
	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4110003001_d.dds",
				"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/back.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4117007002_d.dds",
			    "../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4110003001_d.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4110003001_d.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4117007002_d.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/back.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4110003001_d.dds");
	}
	virtual void CreateModle()
	{
		m_pDml1=m_pScene->CreateDml("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/4110003.dml",GetMatLod());
		m_pDml2=m_pScene->CreateDml("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003002/4110003.dml",GetMatLod());
		m_pDml1->SetPosition(H3DVec3(-0.5f,0.f,0.f));
		m_pDml2->SetPosition(H3DVec3(0.5f,0.f,0.f));
	}

	//以LOD0的方式加载这两个DMl
	SCSTATE OnState0()
	{
		Hippo_WriteConsole(CC_WHITE,"CreateModle()");
		CreateModle();
		return STATE_CHANGE;
	}

	//绘制5s。
	SCSTATE OnState1()
	{
		if (Spin_N_Ms(5000))
		{
			Hippo_WriteConsole(CC_WHITE,"渲染5s");
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}

	//修改磁盘上Tex0文件的内容。
	SCSTATE OnState2()
	{
		Hippo_WriteConsole(CC_WHITE,"修改磁盘上Tex0文件的内容");
		ChangeDiskTex0First();
		return STATE_CHANGE;
	}

	//Reload dml1
	SCSTATE OnState3()
	{
		Hippo_WriteConsole(CC_WHITE,"Reload dml1");
		int res = m_pDml1->ReloadFromDisk(3);

		ChangeDiskTex0Second();
		bAutoEnd = true;

		return STATE_CHANGE;
	}

	virtual void InitScene()
	{
		bAutoEnd = false;
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		stateMachine.AddState(0,std::tr1::bind(&ReloadTextureTwoDml1::OnState0,this));
		stateMachine.AddState(1,std::tr1::bind(&ReloadTextureTwoDml1::OnState1,this));
		stateMachine.AddState(2,std::tr1::bind(&ReloadTextureTwoDml1::OnState2,this));
		stateMachine.AddState(3,std::tr1::bind(&ReloadTextureTwoDml1::OnState3,this));

	}
	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	virtual bool AutoRunExec()
	{
		if (bAutoEnd)
		{
			return true;
		}
		return ManualRunExec();
	}
	virtual bool ManualRunExec()
	{
		stateMachine.Run();
		return false;
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode() == MANUAL_RUN)
		{
			switch (key)
			{
			case VK_RETURN:
				m_state = CASE_END_EXECING;
				return 0;
			}
		}
		return -1;
	}
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml1)

/*测试逻辑
	逻辑同测试1，以lod1的方式进行
*/

class ReloadTextureTwoDml2 : public ReloadTextureTwoDml1
{
public:
	ReloadTextureTwoDml2(const char* casename):ReloadTextureTwoDml1(casename){}
	virtual int GetMatLod(){return 1;};
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml2);

/*测试逻辑
	逻辑同测试1，以lod2的方式进行
*/

class ReloadTextureTwoDml3: public ReloadTextureTwoDml1
{
public:
	ReloadTextureTwoDml3(const char* casename):ReloadTextureTwoDml1(casename){}
	virtual int GetMatLod(){return 2;};


	//lod2情况下使用的是带后缀_lod2的贴图
	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4110003001_d_LOD2.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/back.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4117007002_d.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4110003001_d_LOD2.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4110003001_d_LOD2.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4117007002_d.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/back.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/4110003001/Texture/4110003001_d_LOD2.dds");
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml3);


/*测试逻辑
	逻辑同测试1，以lod0的方式进行--注：这里的资源带有mask材质 ， 固有色贴图 高光贴图和法线贴图都相同
	实例中修改的是固有色贴图，使用另一张固有色贴图进行替换
*/

class ReloadTextureTwoDml4 : public ReloadTextureTwoDml1
{
public:
	ReloadTextureTwoDml4(const char* casename):ReloadTextureTwoDml1(casename){}
	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_D.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/back.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4110003001_d.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_D.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_D.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4110003001_d.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/back.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_D.dds");
	}
	virtual void CreateModle()
	{
		m_pDml1=m_pScene->CreateChr("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/4015012.dml",GetMatLod());
		m_pDml2=m_pScene->CreateChr("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012002/4015012.dml",GetMatLod());
		m_pDml1->SetPosition(H3DVec3(-1.5f,0.f,0.f));
		m_pDml2->SetPosition(H3DVec3(1.5f,0.f,0.f));
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml4);


/*测试逻辑
	逻辑同测试4，以lod1的方式进行--注：这里的资源带有mask材质 ， 固有色贴图 高光贴图和法线贴图都相同
	实例中修改的是固有色贴图，使用另一张固有色贴图进行替换
*/

class ReloadTextureTwoDml5 : public ReloadTextureTwoDml4
{
public:
	ReloadTextureTwoDml5(const char* casename):ReloadTextureTwoDml4(casename){}
	virtual int GetMatLod(){return 1;}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml5);


/*测试逻辑
	逻辑同测试4，以lod2的方式进行--注：这里的资源带有mask材质 ， 固有色贴图 高光贴图和法线贴图都相同
	实例中修改的是固有色贴图，使用另一张固有色贴图进行替换
*/

class ReloadTextureTwoDml6 : public ReloadTextureTwoDml4
{
public:
	ReloadTextureTwoDml6(const char* casename):ReloadTextureTwoDml4(casename){}
	virtual int GetMatLod(){return 2;}

	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_d_LOD2.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/back.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4110003001_d.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_d_LOD2.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_d_LOD2.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4110003001_d.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/back.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_d_LOD2.dds");
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml6);


/*测试逻辑
	逻辑同测试4，以lod0的方式进行--注：这里的资源带有mask材质 ，mask贴图 固有色贴图 高光贴图和法线贴图都相同
	实例中修改的是mask贴图，使用另一张mask贴图进行替换
*/

class ReloadTextureTwoDml7 : public ReloadTextureTwoDml4
{
public:
	ReloadTextureTwoDml7(const char* casename):ReloadTextureTwoDml4(casename){}

	virtual void ChangeDiskTex0First()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_M.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/back.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/114006001_M.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_M.dds");
	}
	virtual void ChangeDiskTex0Second()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_M.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/114006001_M.dds");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/back.dds",
			"../x52_test_resources/Reload测试用例资源/Reload贴图/有共同贴图的实例测试/有mask材质/4015012001/Texture/4015012001_M.dds");
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml7);


/*测试逻辑
	逻辑同测试7，以lod1的方式进行--注：这里的资源带有mask材质 ，mask贴图 固有色贴图 高光贴图和法线贴图都相同
	实例中修改的是mask贴图，使用另一张mask贴图进行替换
*/

class ReloadTextureTwoDml8 : public ReloadTextureTwoDml7
{
public:
	ReloadTextureTwoDml8(const char* casename):ReloadTextureTwoDml7(casename){}

	virtual int GetMatLod(){return 1;}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml8);

/*测试逻辑
	逻辑同测试7，以lod2的方式进行--注：这里的资源带有mask材质 ，mask贴图 固有色贴图 高光贴图和法线贴图都相同
	实例中修改的是mask贴图，使用另一张mask贴图进行替换
*/

class ReloadTextureTwoDml9 : public ReloadTextureTwoDml7
{
public:
	ReloadTextureTwoDml9(const char* casename):ReloadTextureTwoDml7(casename){}

	virtual int GetMatLod(){return 2;}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTextureTwoDml9);
