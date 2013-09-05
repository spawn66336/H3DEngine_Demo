#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "engine_interface_internal.h"
#include "EffectCore_dx.h"
#include <vector>
#include <algorithm>
#include <memory>
using namespace  std;

/*测试逻辑
测试目的：测试GetDiff函数返回的有差异的资源 是否正确
1.确保前后两次引用计数一致
2.调用GetDiff接口 返回一个 新的结构体 HippoResStateSSDiff 指针
3.从测试用例端 验证 HippoResStateSSDiff
*/
class GetDiffTestcase : public HippoTestCaseBase
{
public:
	GetDiffTestcase(const char* casename):HippoTestCaseBase(casename){}
	HippoLevelScene* m_pScene;
	bool bAutoEnd;
	HippoResStateSnapShot* SnapShot_beg;
	HippoResStateSnapShot* SnapShot_end;
private:
	static HippoTestCaseBase* const testcase_;
public:

	virtual void  GenSnapShot()
	{
		SnapShot_beg = new HippoResStateSnapShot(5,HIPPO_TEX_RES);
		SnapShot_end = new HippoResStateSnapShot(5,HIPPO_TEX_RES);

		SnapShot_beg->m_pData[0].m_ref = 1;
		SnapShot_beg->m_pData[1].m_ref = 2;
		SnapShot_beg->m_pData[2].m_ref = 3;
		SnapShot_beg->m_pData[3].m_ref = 4;
		SnapShot_beg->m_pData[4].m_ref = 5;
		strcpy_s(SnapShot_beg->m_pData[0].m_name,256,"111111"); 
		strcpy_s(SnapShot_beg->m_pData[1].m_name,256,"222222"); 
		strcpy_s(SnapShot_beg->m_pData[2].m_name,256,"333333"); 
		strcpy_s(SnapShot_beg->m_pData[3].m_name,256,"444444"); 
		strcpy_s(SnapShot_beg->m_pData[4].m_name,256,"555555"); 

		strcpy_s(SnapShot_end->m_pData[0].m_name,256,"111111"); 
		strcpy_s(SnapShot_end->m_pData[1].m_name,256,"222222"); 
		strcpy_s(SnapShot_end->m_pData[2].m_name,256,"333333"); 
		strcpy_s(SnapShot_end->m_pData[3].m_name,256,"444444"); 
		strcpy_s(SnapShot_end->m_pData[4].m_name,256,"555555"); 
		SnapShot_end->m_pData[0].m_ref = 1;
		SnapShot_end->m_pData[1].m_ref = 2;
		SnapShot_end->m_pData[2].m_ref = 3;
		SnapShot_end->m_pData[3].m_ref = 4;
		SnapShot_end->m_pData[4].m_ref = 5;

	}

	//名字和数量都一致  返回true
	bool IsFindNameRef(HippoResStateSnapShot* sp, std::vector< _DiffResState >::iterator itr)
	{
		for (int i = 0;i<sp->m_res_count;i++)
		{
			if (strcmp(sp->m_pData[i].m_name,(*itr).m_name)==0 && sp->m_pData[i].m_ref == itr->m_ref_diff)
			{
				return true;
			}
		}
		return false;
	}

	//名字存在 就返回true
	bool IsFindName(HippoResStateSnapShot* sp, std::vector< _DiffResState >::iterator itr)
	{
		for (int i = 0;i<sp->m_res_count;i++)
		{
			if (strcmp(sp->m_pData[i].m_name,(*itr).m_name)==0)
			{
				return true;
			}
		}
		return false;

	}
	int GetRefByName(HippoResStateSnapShot* sp,const char* name)
	{
		int ref = -1;
		for (int i=0;i<sp->m_res_count;i++)
		{
			if (strcmp(sp->m_pData[i].m_name,name)==0)
			{
				ref = sp->m_pData[i].m_ref;
				break;
			}
		}
		return ref;
	}

	void TestDiff()
	{

		HippoResStateSSDiff* ff= RefCountCompare::GetSnapShotDiff(SnapShot_beg,SnapShot_end,true);
		std::vector< _DiffResState >::iterator it = ff->m_DiffVec.begin();
		while (it != ff->m_DiffVec.end())
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s   %d----%d",it->m_name,it->m_ref_diff,it->m_position);
			if (it->m_position == LEFT)
			{
				bool bflag = IsFindNameRef(SnapShot_beg,it) && !IsFindNameRef(SnapShot_end,it);
				HIPPO_EXPECT_EQ(bflag,true);
			}
			if (it->m_position == RIGHT)
			{
				bool bflag = !IsFindNameRef(SnapShot_beg,it) && IsFindNameRef(SnapShot_end,it);
				HIPPO_EXPECT_EQ(bflag,true);
			}
			if (it->m_position ==BOTH)
			{
				int diffRef = abs( GetRefByName(SnapShot_beg,it->m_name)-GetRefByName(SnapShot_end,it->m_name));
				bool bflag = IsFindName(SnapShot_beg,it) && IsFindName(SnapShot_end,it) &&(it->m_ref_diff ==diffRef);
				HIPPO_EXPECT_EQ(bflag,true);
			}
			it++;
		}
		ff->Release();
	}
	virtual void InitScene()
	{

		bAutoEnd = false;
		GenSnapShot();
		TestDiff();
		bAutoEnd = true;
	}


	virtual void CleanUpScene()
	{
		SnapShot_beg->Release();
		SnapShot_end->Release();
		if(m_pScene)
		{
			m_pScene->CleanScene();
			Hippo_GetSceneManager()->DelScene(m_pScene);
			m_pScene=0;
		}
	}


	virtual bool AutoRunExec()
	{
		if(bAutoEnd)
		{
			return true;
		}
		return false;
	}

	virtual bool ManualRunExec()
	{
		Hippo_GetIRender()->UpdateEngineThread();
		return false;
	}

	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch(key)
			{
			case   VK_RETURN:
				m_state = CASE_END_EXECING;
				return 1;
			}
		}
		return 0;
	}
};
ADD_TESTCASE("GETDIFF_INTERFACE_TEST",GetDiffTestcase);

/*测试逻辑
测试目的：测试GetDiff函数返回值 是否正确
1.确保前后两次引用计数 不一致 GetDiffTestcase
*/

class GetDiffTestcase1 : public GetDiffTestcase
{
public:
	GetDiffTestcase1(const char* casename):GetDiffTestcase(casename){}
	virtual void GenSnapShot()
	{
		SnapShot_beg = RefCountCompare::GetResState(HIPPO_MAT_RES);
		

		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
		ActorUtil::InitActorDress("../resources/config/shared/item/dress.xml");

		H3DI::IModel* m_pDml = m_pScene->CreateDml("../resources/art/role/link/female/4110/4110001001/4110001.dml");
		H3DI::IActor* m_pActor = m_pScene->CreateActor(false,0);
		m_pActor->SetBodyPart("../resources/art/role/bodypart/female/body/114001001/114001001.BPT");
		H3DI::sCreateOp op;
		op.mat_lod = 0;
		op.geo_lod = 0;
		Hippo_GetIRender()->LoadBodypartAsyn("../resources/art/role/bodypart/female/body/114001001/114001001.BPT",H3DI::ACTOR_HUMAN,op,0);

		SnapShot_end = RefCountCompare::GetResState(HIPPO_MAT_RES);


	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("GETDIFF_INTERFACE_TEST",GetDiffTestcase1)


/*测试逻辑
测试目的：测试GetDiff函数返回值 是否正确
1.确保前后两次引用计数  引用计数名字相同 但是数量不一样
*/

class GetDiffTestcase2 : public GetDiffTestcase
{
public:
	GetDiffTestcase2(const char* casename):GetDiffTestcase(casename){}
	virtual void GenSnapShot()
	{
		SnapShot_beg = new HippoResStateSnapShot(5,HIPPO_TEX_RES);
		SnapShot_end = new HippoResStateSnapShot(5,HIPPO_TEX_RES);

		SnapShot_beg->m_pData[0].m_ref = 1;
		SnapShot_beg->m_pData[1].m_ref = 2;
		SnapShot_beg->m_pData[2].m_ref = 3;
		SnapShot_beg->m_pData[3].m_ref = 4;
		SnapShot_beg->m_pData[4].m_ref = 5;
		strcpy_s(SnapShot_beg->m_pData[0].m_name,256,"111111"); 
		strcpy_s(SnapShot_beg->m_pData[1].m_name,256,"222222"); 
		strcpy_s(SnapShot_beg->m_pData[2].m_name,256,"333333"); 
		strcpy_s(SnapShot_beg->m_pData[3].m_name,256,"444444"); 
		strcpy_s(SnapShot_beg->m_pData[4].m_name,256,"555555"); 

		strcpy_s(SnapShot_end->m_pData[0].m_name,256,"111111"); 
		strcpy_s(SnapShot_end->m_pData[1].m_name,256,"222222"); 
		strcpy_s(SnapShot_end->m_pData[2].m_name,256,"333333"); 
		strcpy_s(SnapShot_end->m_pData[3].m_name,256,"444444"); 
		strcpy_s(SnapShot_end->m_pData[4].m_name,256,"555555"); 
		SnapShot_end->m_pData[0].m_ref = 1;
		SnapShot_end->m_pData[1].m_ref = 2;
		SnapShot_end->m_pData[2].m_ref = 3;
		SnapShot_end->m_pData[3].m_ref = 6;
		SnapShot_end->m_pData[4].m_ref = 7;
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("GETDIFF_INTERFACE_TEST",GetDiffTestcase2)

/*测试逻辑
测试目的：测试GetDiff函数返回值 是否正确
1.确保前后两次引用计数  引用计数名字不同 且数量不一样
*/

class GetDiffTestcase3 : public GetDiffTestcase
{
public:
	GetDiffTestcase3(const char* casename):GetDiffTestcase(casename){}
	virtual void GenSnapShot()
	{
		SnapShot_beg = new HippoResStateSnapShot(5,HIPPO_TEX_RES);
		SnapShot_end = new HippoResStateSnapShot(5,HIPPO_TEX_RES);

		SnapShot_beg->m_pData[0].m_ref = 1;
		SnapShot_beg->m_pData[1].m_ref = 2;
		SnapShot_beg->m_pData[2].m_ref = 3;
		SnapShot_beg->m_pData[3].m_ref = 4;
		SnapShot_beg->m_pData[4].m_ref = 5;
		strcpy_s(SnapShot_beg->m_pData[0].m_name,256,"111111"); 
		strcpy_s(SnapShot_beg->m_pData[1].m_name,256,"222222"); 
		strcpy_s(SnapShot_beg->m_pData[2].m_name,256,"333333"); 
		strcpy_s(SnapShot_beg->m_pData[3].m_name,256,"444444"); 
		strcpy_s(SnapShot_beg->m_pData[4].m_name,256,"555555"); 

		strcpy_s(SnapShot_end->m_pData[0].m_name,256,"666666"); 
		strcpy_s(SnapShot_end->m_pData[1].m_name,256,"777777"); 
		strcpy_s(SnapShot_end->m_pData[2].m_name,256,"888888"); 
		strcpy_s(SnapShot_end->m_pData[3].m_name,256,"999999"); 
		strcpy_s(SnapShot_end->m_pData[4].m_name,256,"000000"); 
		SnapShot_end->m_pData[0].m_ref = 9;
		SnapShot_end->m_pData[1].m_ref = 8;
		SnapShot_end->m_pData[2].m_ref = 0;
		SnapShot_end->m_pData[3].m_ref = 6;
		SnapShot_end->m_pData[4].m_ref = 7;
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("GETDIFF_INTERFACE_TEST",GetDiffTestcase3)


/*测试逻辑
测试目的：测试GetDiff函数返回值 是否正确
1.确保前后两次引用计数 中 有名字相同数量不一致的 有名字不同的 有名字相同数量一致的
*/

class GetDiffTestcase4 : public GetDiffTestcase
{
public:
	GetDiffTestcase4(const char* casename):GetDiffTestcase(casename){}
	virtual void GenSnapShot()
	{
		SnapShot_beg = new HippoResStateSnapShot(4,HIPPO_TEX_RES);
		SnapShot_end = new HippoResStateSnapShot(5,HIPPO_TEX_RES);

		SnapShot_beg->m_pData[0].m_ref = 1;
		SnapShot_beg->m_pData[1].m_ref = 2;
		SnapShot_beg->m_pData[2].m_ref = 3;
		SnapShot_beg->m_pData[3].m_ref = 4;
		strcpy_s(SnapShot_beg->m_pData[0].m_name,256,"111111"); 
		strcpy_s(SnapShot_beg->m_pData[1].m_name,256,"222222"); 
		strcpy_s(SnapShot_beg->m_pData[2].m_name,256,"333333"); 
		strcpy_s(SnapShot_beg->m_pData[3].m_name,256,"444444"); 


		strcpy_s(SnapShot_end->m_pData[0].m_name,256,"666666"); 
		strcpy_s(SnapShot_end->m_pData[1].m_name,256,"777777"); 
		strcpy_s(SnapShot_end->m_pData[2].m_name,256,"888888"); 
		strcpy_s(SnapShot_end->m_pData[3].m_name,256,"222222"); 
		strcpy_s(SnapShot_end->m_pData[4].m_name,256,"111111"); 
		SnapShot_end->m_pData[0].m_ref = 9;
		SnapShot_end->m_pData[1].m_ref = 8;
		SnapShot_end->m_pData[2].m_ref = 0;
		SnapShot_end->m_pData[3].m_ref = 2;
		SnapShot_end->m_pData[4].m_ref = 7;
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("GETDIFF_INTERFACE_TEST",GetDiffTestcase4)


/*测试逻辑
测试目的：测试GetDiff函数返回值 是否正确
1.确保前后两次引用计数  左边的有内容 右边的为空
*/

class GetDiffTestcase5 : public GetDiffTestcase
{
public:
	GetDiffTestcase5(const char* casename):GetDiffTestcase(casename){}
	virtual void GenSnapShot()
	{
		SnapShot_beg = new HippoResStateSnapShot(4,HIPPO_TEX_RES);
		SnapShot_end = new HippoResStateSnapShot(5,HIPPO_TEX_RES);


		strcpy_s(SnapShot_end->m_pData[0].m_name,256,"666666"); 
		strcpy_s(SnapShot_end->m_pData[1].m_name,256,"777777"); 
		strcpy_s(SnapShot_end->m_pData[2].m_name,256,"888888"); 
		strcpy_s(SnapShot_end->m_pData[3].m_name,256,"222222"); 
		strcpy_s(SnapShot_end->m_pData[4].m_name,256,"111111"); 
		SnapShot_end->m_pData[0].m_ref = 9;
		SnapShot_end->m_pData[1].m_ref = 8;
		SnapShot_end->m_pData[2].m_ref = 0;
		SnapShot_end->m_pData[3].m_ref = 2;
		SnapShot_end->m_pData[4].m_ref = 7;
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("GETDIFF_INTERFACE_TEST",GetDiffTestcase5)

	/*测试逻辑
	测试目的：测试GetDiff函数返回值 是否正确
	1.确保前后两次引用计数  左边的为空 右边有内容
	*/

class GetDiffTestcase6 : public GetDiffTestcase
{
public:
	GetDiffTestcase6(const char* casename):GetDiffTestcase(casename){}
	virtual void GenSnapShot()
	{

		SnapShot_beg = new HippoResStateSnapShot(4,HIPPO_TEX_RES);
		SnapShot_end = new HippoResStateSnapShot(0,HIPPO_TEX_RES);

		SnapShot_beg->m_pData[0].m_ref = 1;
		SnapShot_beg->m_pData[1].m_ref = 2;
		SnapShot_beg->m_pData[2].m_ref = 3;
		SnapShot_beg->m_pData[3].m_ref = 4;
		strcpy_s(SnapShot_beg->m_pData[0].m_name,256,"111111"); 
		strcpy_s(SnapShot_beg->m_pData[1].m_name,256,"222222"); 
		strcpy_s(SnapShot_beg->m_pData[2].m_name,256,"333333"); 
		strcpy_s(SnapShot_beg->m_pData[3].m_name,256,"444444"); 
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("GETDIFF_INTERFACE_TEST",GetDiffTestcase6)