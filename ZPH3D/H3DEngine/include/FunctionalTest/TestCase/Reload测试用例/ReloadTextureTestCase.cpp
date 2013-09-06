#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/HippoStateMachine.h"
#include "../CommonCode/HippoUtil/HippoTaskVisitor.h"
#include "EffectCore_dx.h"
#include <algorithm>
#include <stdio.h>
using namespace  std;

/*测试逻辑
准备一个dml，其使用了带有贴图的材质实例lod0
以lod0方式加载此dml，并绘制5s
引用计数快照s1
在测试用例中通过代码修改dml的材质实例内容，给这个材质实例的贴图换一张
调用模型的reload Texturen函数，渲染5s
查看效果变化
执行clearmatlib
引用计数快照s2
s1和2的差别应该是一个贴图的差别
*/

class ReloadTexture1 : public HippoTestCaseBase
{
public:
	ReloadTexture1(const char* casename):HippoTestCaseBase(casename){}
	HippoLevelScene* m_pScene;
	H3DI::IModel* m_pDml;
	Hippo_StateMachine stateMachine;
	HippoResStateSnapShot* s1Tex;
	HippoResStateSnapShot* s1Mat;
	HippoResStateSnapShot* s1Tpl;

	HippoResStateSnapShot* s2Tex;
	HippoResStateSnapShot* s2Mat;
	HippoResStateSnapShot* s2Tpl;
	bool bAutoEnd;
private:
	static HippoTestCaseBase* const testcase_;
public:
	virtual int GetMatLod(){return 0;}
	virtual void ChangeNameFirst()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4110003001/4110003001.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4110003001/back.mat");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4110003001/__4110003001.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4110003001/4110003001.mat");
	}
	virtual void ChangeNameSecond()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4110003001/4110003001.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4110003001/__4110003001.mat");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4110003001/back.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4110003001/4110003001.mat");
	}
	virtual void Createdml()
	{
		m_pDml=m_pScene->CreateDml("../x52_test_resources/Reload测试用例资源/Reload贴图/4110003001/4110003.dml",GetMatLod());
	}

	virtual SCSTATE OnState0()
	{
		Hippo_WriteConsole(CC_WHITE,"CreateDml()");
		 Createdml();
		return STATE_CHANGE;
	}
	SCSTATE OnState1()
	{
		
		if (Spin_N_Ms(5000))
		{
			Hippo_WriteConsole(CC_WHITE,"渲染5s结束");
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}
	SCSTATE OnState2()
	{
		Hippo_WriteConsole(CC_WHITE,"引用计数快照s1");
		s1Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		s1Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		s1Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);
		return STATE_CHANGE;
	}



	//在测试用例中通过代码修改dml的材质实例内容，给这个材质实例的贴图换一张
	SCSTATE OnState3()
	{
		Hippo_WriteConsole(CC_WHITE,"给这个材质实例的贴图换一张");
		
		ChangeNameFirst();
		
		return STATE_CHANGE;
	}
	//调用模型的reload Texturen函数
	SCSTATE OnState4()
	{
		Hippo_WriteConsole(CC_WHITE,"ReloadFromDisk()");
		int res = m_pDml->ReloadAll();
		return STATE_CHANGE;
	}
	SCSTATE OnState5()
	{
		Hippo_WriteConsole(CC_WHITE,"记录引用计数并比较");
		Hippo_GetIRender()->ClearMaterialLib();
		s2Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		s2Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		s2Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);

		HippoResStateSSDiff* diff = RefCountCompare::GetSnapShotDiff(s1Tex,s2Tex);
		vector<_DiffResState> diffvec = diff->m_DiffVec;
		vector<_DiffResState>::size_type size = diffvec.size();
		//容器大小应该为2，因为只有一个贴图差异
 		HIPPO_EXPECT_EQ((size==2),true);
		vector<_DiffResState>::iterator itr = diffvec.begin();
		while (itr != diffvec.end())
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s   %d----%d",itr->m_name,itr->m_ref_diff,itr->m_position);
			bool flag = (*itr).m_position != BOTH; 
			HIPPO_EXPECT_EQ(flag,true);
			++itr;
		}
		
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s1Mat,s2Mat),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s2Tpl,s1Tpl),true);

		s1Tex->Release();
		s1Mat->Release();
		s1Tpl->Release();

		s2Tex->Release();
		s2Mat->Release();
		s2Tpl->Release();

		ChangeNameSecond();

		bAutoEnd = true;
		return STATE_CHANGE;
	}

	void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

		stateMachine.AddState(0,std::tr1::bind(&ReloadTexture1::OnState0,this));
		stateMachine.AddState(1,std::tr1::bind(&ReloadTexture1::OnState1,this));
		stateMachine.AddState(2,std::tr1::bind(&ReloadTexture1::OnState2,this));
		stateMachine.AddState(3,std::tr1::bind(&ReloadTexture1::OnState3,this));
		stateMachine.AddState(4,std::tr1::bind(&ReloadTexture1::OnState4,this));
		stateMachine.AddState(5,std::tr1::bind(&ReloadTexture1::OnState1,this));
		stateMachine.AddState(6,std::tr1::bind(&ReloadTexture1::OnState5,this));
		bAutoEnd = false;
	}

	void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	bool ManualRunExec()
	{

		stateMachine.Run();
		return false;
	}

	bool AutoRunExec()
	{
		if (bAutoEnd)
		{
			return true;
		}
		return ManualRunExec();
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

ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture1);

/*测试逻辑
使用lod1加载一个dml，其它同上
*/
class ReloadTexture2 : public ReloadTexture1
{
public:
	ReloadTexture2(const char* casename):ReloadTexture1(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture2);

/*测试逻辑
使用lod2加载一个dml，其它同上
*/
class ReloadTexture3 : public ReloadTexture1
{
public:
	ReloadTexture3(const char* casename):ReloadTexture1(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture3);

/*测试逻辑
使用lod0加载4115001001 下的dml，其它同上
*/
class ReloadTexture4 : public ReloadTexture1
{
public:
	ReloadTexture4(const char* casename):ReloadTexture1(casename){}
	virtual int GetMatLod(){return 0;}
	virtual void ChangeNameFirst()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4115001001/4115001001.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4115001001/back.mat");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4115001001/__4115001001.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4115001001/4115001001.mat");
	}
	virtual void ChangeNameSecond()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4115001001/4115001001.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4115001001/__4115001001.mat");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4115001001/back.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4115001001/4115001001.mat");
	}
	virtual void Createdml()
	{
		m_pDml=m_pScene->CreateDml("../x52_test_resources/Reload测试用例资源/Reload贴图/4115001001/4115001.dml",GetMatLod());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture4);


/*测试逻辑
使用lod1加载4115001001 下的dml，其它同上
*/
class ReloadTexture5 : public ReloadTexture4
{
public:
	ReloadTexture5(const char* casename):ReloadTexture4(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture5);


/*测试逻辑
使用lod2加载4115001001 下的dml，其它同上
*/
class ReloadTexture6 : public ReloadTexture4
{
public:
	ReloadTexture6(const char* casename):ReloadTexture4(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture6);


/*测试逻辑
使用lod0加载4115ddd 下的dml，其它同上
*/
class ReloadTexture7 : public ReloadTexture1
{
public:
	ReloadTexture7(const char* casename):ReloadTexture1(casename){}
	virtual int GetMatLod(){return 0;}
	virtual void ChangeNameFirst()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4115ddd/4115001001.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4115ddd/back.mat");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4115ddd/__4115001001.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4115ddd/4115001001.mat");
	}
	virtual void ChangeNameSecond()
	{
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4115ddd/4115001001.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4115ddd/__4115001001.mat");
		rename("../x52_test_resources/Reload测试用例资源/Reload贴图/4115ddd/back.mat","../x52_test_resources/Reload测试用例资源/Reload贴图/4115ddd/4115001001.mat");
	}
	virtual void Createdml()
	{
		m_pDml=m_pScene->CreateDml("../x52_test_resources/Reload测试用例资源/Reload贴图/4115ddd/4115001.dml",GetMatLod());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture7);


/*测试逻辑
使用lod1加载4115ddd 下的dml，其它同上
*/
class ReloadTexture8 : public ReloadTexture7
{
public:
	ReloadTexture8(const char* casename):ReloadTexture7(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture8);


/*测试逻辑
使用lod2加载4115ddd 下的dml，其它同上
*/
class ReloadTexture9 : public ReloadTexture7
{
public:
	ReloadTexture9(const char* casename):ReloadTexture7(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_TESTCASE",ReloadTexture9);




