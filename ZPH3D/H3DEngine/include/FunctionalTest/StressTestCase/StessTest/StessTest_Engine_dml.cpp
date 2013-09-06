#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/FileOperation.h"
#include "EffectCore_dx.h"
#include "idMath/dMathHeader.h"
#include "../../FunctionalTest/stress_test/HippoTestConfig.h"
#include "GenPathList.h"
using namespace  std;


/*测试逻辑
使用引擎：
1、遍历目录下所有的dml，生成加载列表
2、使用引擎同步加载dml，1次加载8个，并排放置。加载完成后渲染5s
3、每次加载dml前，先释放前面的模型，clearmatlib
4、加载完列表中所有的模型后，循环
*/
const int StaticModel_Num = 8;   //静态模型数

class StressTest_Engine_Sync:public HippoTestCase_RefCompare
{
public:
	StressTest_Engine_Sync(const char* casename):HippoTestCase_RefCompare(casename){}
	virtual void InitScene()
	{
		m_ErgodicDir = "art/role/link/female";
		if (!GenPathList(m_pDmlPathList,Make_X52Resources_Path(m_ErgodicDir)))
		{
			Hippo_WriteConsole(CC_RED,"产生列表文件错误，请检查");
			return ;
		}

		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		index = 0;
		CreateModel();
	}

	virtual void CreateModel()
	{
		if (!m_ModelList.empty())
		{

			vector<H3DI::IModel*>::iterator itr=m_ModelList.begin();
			while (itr!=m_ModelList.end())
			{
				(*itr)->Release();
				++itr;
			}
			m_ModelList.clear();
			Hippo_GetIRender()->ClearMaterialLib();	
		}
		H3DI::sCreateOp op;
		op.mat_lod=0;

		for (int i=0;i<StaticModel_Num;i++)
		{
		    H3DI::IModel* pdml=Hippo_GetIRender()->CreateModel(op,m_pDmlPathList.at(index).c_str());/*m_pScene->CreateDml(m_pDmlPathList.at(index).c_str())*/;
			if (pdml)
			{
				pdml->SetPosition(H3DVec3((float)(i-4),0.f,0.f));
				m_ModelList.push_back(pdml);
				index++;
				m_pScene->GetH3DScene()->AttachModel(pdml,H3DI::SL_DetailObj);
			}
			
			if (index == m_pDmlPathList.size())
			{
				index = 0;
			}
		}

	}
 
	virtual bool ManualRunExec()
	{
		if (Spin_N_Ms(5000))
		{
			CreateModel();
		}
		return false;
	}

	virtual bool AutoRunExec()
	{
		return ManualRunExec();
	}

	virtual void CleanUpScene()
	{
		if (m_pScene)
		{
			m_pScene->CleanScene();
			Hippo_GetSceneManager()->DelScene(m_pScene);
		}
	}
	virtual int OnKeyDown(unsigned int key)
	{
		if (HippoTestManager::GetInstance()->GetCurrentRunMode())
		{
			switch(key)
			{
			case  VK_RETURN:
				m_state = CASE_END_EXECING;
				return 0;
			}
		}
		return -1;
	}

private:
	static HippoTestCaseBase* const testcase_;

public:

	HippoScene* m_pScene;
	vector<std::string> m_pDmlPathList;
	vector<H3DI::IModel*> m_ModelList;
	
	int index;

	const char* m_ErgodicDir; //指定要遍历的目录

};
ADD_TESTCASE("STRESS_TEST",StressTest_Engine_Sync);

/*测试逻辑
1、遍历目录下所有的dml，生成加载列表
2、使用引擎同步加载dml，1次加载8个，并排放置。加载完成后渲染5s
3、释放前面的模型，不要clearmatlib
4、再加载这同样的8个模型。加载完成后渲染5s。
5、循环
*/
class StressTest_Engine_Sync_NoClear:public StressTest_Engine_Sync
{
public: 
	StressTest_Engine_Sync_NoClear(const char* casename):StressTest_Engine_Sync(casename){}
	virtual void CreateModel()
	{
		if (!m_ModelList.empty())
		{
			vector<H3DI::IModel*>::iterator itr=m_ModelList.begin();
			while (itr!=m_ModelList.end())
			{
				(*itr)->Release();
				++itr;
			}
			m_ModelList.clear();
		}
		H3DI::sCreateOp op;
		op.mat_lod=0;
		for (int i=0;i<StaticModel_Num;i++)
		{
			H3DI::IModel* pdml=Hippo_GetIRender()->CreateModel(op,m_pDmlPathList.at(index).c_str());
			if (pdml)
			{
				pdml->SetPosition(H3DVec3((float)(i-4),0.f,0.f));
				m_ModelList.push_back(pdml);
				index++;
				m_pScene->GetH3DScene()->AttachModel(pdml,H3DI::SL_DetailObj);
			}
		}
		index=0;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("STRESS_TEST",StressTest_Engine_Sync_NoClear);


/*测试逻辑
使用引擎：
1、遍历目录下所有的dml，生成加载列表
2、使用引擎异步加载dml，1次加载8个，并排放置。加载完成后渲染5s
3、每次加载dml前，先释放前面的模型，clearmatlib
4、加载完列表中所有的模型后，循环
*/

class StressTest_Engine_Asyn : public HippoTestCase_RefCompare
{
public:
	StressTest_Engine_Asyn(const char* casename):HippoTestCase_RefCompare(casename){}
	
	virtual void InitScene()
	{
		m_ErgodicDir = "art/role/link/female";
		if (!GenPathList(m_pDmlPathList,Make_X52Resources_Path(m_ErgodicDir)))
		{
			Hippo_WriteConsole(CC_RED,"产生列表文件错误，请检查");
			return ;
		}

		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		index = 0;
		CreateModel();
	}

	virtual void CreateModel()
	{
		if (m_ModelList.size() == StaticModel_Num)//
		{
			for (vector<H3DI::IModel*>::iterator itr=m_ModelList.begin();itr!=m_ModelList.end();itr++)
			{
				(*itr)->Release();
			}
			m_ModelList.clear();
			Hippo_GetIRender()->ClearMaterialLib();
		}
		H3DI::sCreateOp op;
		op.mat_lod=0;
		for (int i=0;i<StaticModel_Num;i++)
		{
			m_all_task.push_back(Hippo_GetIRender()->CreateModelAsyn(m_pDmlPathList.at(index).c_str(),op,0));
			index++;

			if (index == m_pDmlPathList.size())
			{
				index = 0;
			}
		}
	}
	void HandleTask()
	{
		Hippo_GetIRender()->UpdateEngineThread();
		static int pos=-4;
		for (vector<unsigned int>::iterator it=m_all_task.begin(); it!=m_all_task.end();)
		{
			unsigned int res = Hippo_GetIRender()->QueryTaskState(*it);
			if (res!=H3DI::TASK_OK && res!=H3DI::TASK_ERROR)
			{
				it++;
				continue;
			}

			H3DI::IModel* model = (H3DI::IModel*)Hippo_GetIRender()->GetModelAndFinishTask(*it);
			if (model)
			{
				model->SetPosition(H3DVec3(static_cast<float>(pos++),0.f,0.f));
				model->Update(0);
				m_ModelList.push_back(model);
				m_pScene->GetH3DScene()->AttachModel((H3DI::IMoveObject*)model,H3DI::SL_DetailObj);
			}
			it = m_all_task.erase(it);
		}
		if (pos == 4){ pos -= StaticModel_Num;}
	}
	virtual bool ManualRunExec()
	{
		HandleTask();
		if (Spin_N_Ms(5000))
		{
			CreateModel();
		}
		return false;
	}
	virtual bool AutoRunExec()
	{
		return ManualRunExec();
	}
	virtual void CleanUpScene()
	{
		if (m_pScene)
		{
			m_pScene->CleanScene();
			Hippo_GetSceneManager()->DelScene(m_pScene);
		}
	}
private:
	static HippoTestCaseBase* const testcase_;

public:

	HippoScene* m_pScene;
	vector<std::string> m_pDmlPathList;
	vector<H3DI::IModel*> m_ModelList;

	int index;

	vector<unsigned int> m_all_task;

	const char* m_ErgodicDir;
};
ADD_TESTCASE("STRESS_TEST",StressTest_Engine_Asyn);

/*测试逻辑
1、遍历目录下所有的dml，生成加载列表
2、使用引擎异步加载dml，1次加载8个，并排放置。加载完成后渲染5s
3、释放前面的模型，不要clearmatlib
4、再加载这同样的8个模型。加载完成后渲染5s。
5、循环
*/
class StressTest_Engine_Asyn_NoClear : public StressTest_Engine_Asyn
{
public:
	StressTest_Engine_Asyn_NoClear(const char* casename): StressTest_Engine_Asyn(casename){}

	virtual void CreateModel()
	{
		if (m_ModelList.size() == StaticModel_Num)//
		{
			for (vector<H3DI::IModel*>::iterator itr=m_ModelList.begin();itr!=m_ModelList.end();itr++)
			{
				(*itr)->Release();
			}
			m_ModelList.clear();
		}
		H3DI::sCreateOp op;
		op.mat_lod=0;
		for (int i=0;i<StaticModel_Num;i++)
		{
			m_all_task.push_back(Hippo_GetIRender()->CreateModelAsyn(m_pDmlPathList.at(index).c_str(),op,0));
			index++;
		}
		index = 0;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("STRESS_TEST",StressTest_Engine_Asyn_NoClear);