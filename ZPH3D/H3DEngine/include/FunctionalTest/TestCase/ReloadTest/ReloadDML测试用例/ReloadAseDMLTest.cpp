#include "../../../HippoTestManager/HippoTestManager.h"
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
#include <Windows.h>
#include "../../FunctionalTest/functional_test/HippoTestConfig.h"
using namespace std;

/*�����߼�
����Ŀ�ģ�����dml������ase�ı�� reload

׼��һ��dml
��lod0��ʽ���ش�dml��������5s
���ü�������s1
�ڲ���������ͨ�������޸�dml��ase���ݣ�--�޸���ase�в�����Ϣ�Ĺ���ֵ
����ģ�͵�reload Texturen������
��Ⱦ5s �鿴Ч���仯
ִ��clearmatlib
���ü�������s2
*/


/*//��ע��

*/
class RelaodAseDMLTest1 : public HippoTestCase_RefCompare
{
public:
	RelaodAseDMLTest1(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;
	H3DI::IModel* m_pDml;
	bool bAutoEnd;

	HippoResStateSnapShot* s1Tex;
	HippoResStateSnapShot* s1Mat;
	HippoResStateSnapShot* s1Tpl;

	HippoResStateSnapShot* s2Tex;
	HippoResStateSnapShot* s2Mat;
	HippoResStateSnapShot* s2Tpl;

	Hippo_StateMachine stateMachine;
private:
	static HippoTestCaseBase* const testcase_;
public:

	virtual int GetMatLod(){return 0;}
	virtual void CopyOrDel(bool bCopy)
	{
		if (bCopy)
		{
			CopyFile(Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/back/4110001.ase").c_str(),Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/4110001.ase").c_str(),TRUE);
			CopyFile(Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/back/changed.ase").c_str(),Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/changed.ase").c_str(),TRUE);
		}
		else
		{
			//ÿ�γ����˳�ǰ ���ʹ�õ�mat�ļ�
			DeleteFile(Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/4110001.ase").c_str());
			DeleteFile(Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/changed.ase").c_str());
			DeleteFile(Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/back.ase").c_str());
		}
	}
	virtual void ChangeAse()
	{
		rename(Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/4110001.ase").c_str(),Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/back.ase").c_str());
		rename(Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/changed.ase").c_str(),Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/4110001.ase").c_str());
	}

	SCSTATE OnState0()
	{
		Hippo_WriteConsole(CC_WHITE,"CreateDml()");
		CopyOrDel(true);
		m_pDml = m_pScene->CreateDml(Make_HippoTestResource_Path("Reload����������Դ/RelaodDML/RelaodAse/4110001001/4110001.dml").c_str(),GetMatLod());
		return STATE_CHANGE;
	}
	SCSTATE OnState1()
	{
		if (Spin_N_Ms(5000))
		{
			return STATE_CHANGE;
		}
		return STATE_NOT_CHANGE;
	}
	SCSTATE OnState2()//���ü�������s1
	{
		Hippo_WriteConsole(CC_WHITE,"���ü�������s1");
		s1Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		s1Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		s1Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);
		return STATE_CHANGE;
	}
	SCSTATE OnState3()
	{
		ChangeAse();
		Hippo_WriteConsole(CC_WHITE,"ReloadAll()");
		m_pDml->ReloadAll();
		CopyOrDel(false);
		return STATE_CHANGE;
	}
	SCSTATE OnState4()
	{
		Hippo_WriteConsole(CC_WHITE,"��¼���ü������Ƚ�");
		Hippo_GetIRender()->ClearMaterialLib();
		s2Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		s2Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		s2Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);

		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s1Tex,s2Tex),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s1Mat,s2Mat),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s1Tpl,s2Tpl),true);

		s1Tex->Release();
		s1Mat->Release();
		s1Tpl->Release();

		s2Tex->Release();
		s2Mat->Release();
		s2Tpl->Release();

		bAutoEnd = true;

		return STATE_CHANGE;
	}
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

		stateMachine.AddState(0,std::tr1::bind(&RelaodAseDMLTest1::OnState0,this));//create dml
		stateMachine.AddState(1,std::tr1::bind(&RelaodAseDMLTest1::OnState1,this));//render
		stateMachine.AddState(2,std::tr1::bind(&RelaodAseDMLTest1::OnState2,this));//snapshot s1
		stateMachine.AddState(3,std::tr1::bind(&RelaodAseDMLTest1::OnState3,this));//reload
		stateMachine.AddState(4,std::tr1::bind(&RelaodAseDMLTest1::OnState1,this));//render
		stateMachine.AddState(5,std::tr1::bind(&RelaodAseDMLTest1::OnState4,this));//s2 and compare
		bAutoEnd = false;

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
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
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
ADD_TESTCASE("RELOAD_DML_TEST",RelaodAseDMLTest1);

/*�����߼�
��lod1����dml������ͬ����1
*/
class RelaodAseDMLTest2 :public RelaodAseDMLTest1
{
public:
	RelaodAseDMLTest2(const char* casename):RelaodAseDMLTest1(casename){}
	virtual int GetMatLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_DML_TEST",RelaodAseDMLTest2);

/*�����߼� 
��lod2 ����dml������ͬ����1
*/
class RelaodAseDMLTest3:public RelaodAseDMLTest1
{
public:
	RelaodAseDMLTest3(const char* casename):RelaodAseDMLTest1(casename){}
	virtual int GetMatLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_DML_TEST",RelaodAseDMLTest3);