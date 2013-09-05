#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/FileOperation.h"
#include "EffectCore_dx.h"
#include "engine_interface.h"
#include "idMath/dMathHeader.h"
#include "../../FunctionalTest/functional_test/HippoTestConfig.h"
#include "GenPathList.h"
#include "middle_layer_interface.h"
#include "../CommonCode/HippoUtil/camera/CameraBase.h"
using namespace  std;
#pragma comment(lib, "winmm.lib")
/*测试逻辑
使用中间层 异步加载
1、遍历目录下所有的dml，生成加载列表
2、使用引擎异步加载dml，1次加载8个，并排放置。加载完成后渲染5s
3、每次加载dml前，先释放前面的模型，clearmatlib
4、加载完列表中所有的模型后，循环
*/

const int StaticModel_Num=8;
const int Model_Name_Length=64;

namespace H3DI
{
	class IMoveObject;
	class IPrePassLight;
	class IActor;
	class IAvatarSkeletonModel;
}
class ISpecialEffectManager;
class ActorManager;
class StressTest_MidLayer_Aync :public HippoTestCase_RefCompare
{
public:
	StressTest_MidLayer_Aync(const char* casename):HippoTestCase_RefCompare(casename){}

	virtual void InitScene()
	{
		m_ErgodicDir = "art/role/link/female";
		if (!GenPathList(m_pDmlPathList,Make_X52Resources_Path(m_ErgodicDir)))
		{
			Hippo_WriteConsole(CC_RED,"获取文件列表错误！");
			return ;
		}

		index = 0;
		m_pathlist_loop_count=0;

		m_pLight=Hippo_GetIRender()->CreatePrePassLight(H3DI::LIGHT_DIR);
		H3DVec3 lightDir(1.0f,1.0f,1.0f);
		lightDir.Normalize();
		m_pLight->SetDirection(lightDir);
		float color[4]={1.0f, 0.0f, 0.0f, 0.0f};
		m_pLight->SetColor(color);
		m_pLight->SetIntensity(11.0f);
		m_pLight->SetLightEnable(true);

		m_actormng = new ActorManager;
		m_actormng->initialize(Hippo_GetIRender(),NULL);
		m_level = m_actormng->get_level_entity();
		m_grapy_res_mng = m_actormng->get_graphic_res_mng();
		m_factory = m_grapy_res_mng->get_entity_factory();

		m_level->get_ILevel()->AttachModel(m_pLight,H3DI::SL_Lights);

		CreamteModel();
	}
	virtual void CreamteModel()
	{
		if (!m_ModelList.empty())
		{
		    ReleaseModel();
			Hippo_GetIRender()->ClearMaterialLib();
		}
		int posModel=0;
		for (int i = 0; i < StaticModel_Num; ++i)
		{
			char name[Model_Name_Length] = {0};
			sprintf(name, "static_model_%d", m_pathlist_loop_count * StaticModel_Num + i);
			CModelEntity* model_entity = m_factory->create_model(m_pDmlPathList[index], name, LL_ZERO, true);
			
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s--%s",name,m_pDmlPathList.at(index).c_str());

			if (model_entity && m_level)
			{
				model_entity->set_position(H3DVec3(float(posModel - 4),0.f,0.f));
				model_entity->update(0);
				m_level->add_to_level(model_entity);
				m_ModelList.push_back(model_entity);
			}
			index++;
			if (index == m_pDmlPathList.size())
				index = 0;
			posModel++;
		}
		m_pathlist_loop_count++;
	}

	void ReleaseModel()
	{
		std::vector<CModelEntity*>::iterator itr = m_ModelList.begin();
		while(itr != m_ModelList.end())
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"****  remove ****%s",(*itr)->get_name());
			(*itr)->stop();
			if ((*itr)->is_added_to_level() && m_level)
				m_level->remove_from_level(*itr);
			m_factory->release(*itr);
			*itr++=NULL;
		}
		m_ModelList.clear();
	}

	void DoRender()
	{
		if(m_level)
			Hippo_GetIRender()->PushScene(m_level->get_ILevel());
	}
	void DoUpdate()
	{
		Hippo_GetIRender()->UpdateEngineThread();

		UpdateCamera();

		static DWORD current_t=0;
		static DWORD last_t=0;
		last_t=current_t;
		current_t=::timeGetTime();
		DWORD escape=current_t-last_t;

		std::vector<CModelEntity*>::iterator itr = m_ModelList.begin();
		for(; itr != m_ModelList.end(); ++itr)
		{
			(*itr)->update(escape);
		}

		if (Spin_N_Ms(5000))
		{
			CreamteModel();
		}
	}

	void UpdateCamera()
	{
		//Hippo_WriteConsoleAndLog(CC_RED,"update camera");

		m_level->get_ILevel()->SetCurrentCamera(Hippo_GetIRender());
		CameraBase* pCamera=HippoTestManager::GetInstance()->GetCamera();
		H3DVec3 CamPos = pCamera->GetPos();
		H3DVec3 CamLook = pCamera->GetViewAtDir();

		float fov, ratio, fNear, fFar;
		Hippo_GetIRender()->GetFrustum(fov, ratio, fNear, fFar, H3DI::ECT_Render);
		Hippo_GetIRender()->LookAt(CamPos, CamLook, H3DVec3(0, 0, 1));
		Hippo_GetIRender()->UpdateCamera();

		m_level->get_ILevel()->SetFrustum(fov, ratio, fNear, fFar);
		m_level->get_ILevel()->LookAt(CamPos,CamLook, H3DVec3(0, 0, 1));
		m_level->get_ILevel()->UpdateCamera();
	}

	virtual void CleanUpScene()
	{
		ReleaseModel();

		if (m_pLight != NULL)
		{
			m_pLight->Release();
			m_pLight = NULL;
		}
		m_factory=NULL;
		m_level=NULL;
		m_grapy_res_mng=NULL;
		if (m_actormng)
		{
			delete m_actormng;
			m_actormng=NULL;
		}
	}

	virtual bool AutoRunExec()
	{
		return ManualRunExec();
	}
	virtual bool ManualRunExec()
	{
		return false;
	}
	virtual void BeforeRender()
	{
		DoUpdate();
		DoRender();
	}

private:
	static HippoTestCaseBase* const testcase_;

public:
	ActorManager* m_actormng;
	CGraphicsResMng* m_grapy_res_mng;
	CLevelEntity*		m_level;
	CEntityFactory*		m_factory;
	vector<CModelEntity*> m_ModelList;

	H3DI::IPrePassLight* m_pLight;

	const char* m_ErgodicDir; //要遍历的目录
	vector<string> m_pDmlPathList;//存放dml路径的列表
	int index;//m_pDmlPathList的当前索引
	unsigned long long  m_pathlist_loop_count;//一次创建8个模型，循环的次数
};

ADD_TESTCASE("STRESS_TEST",StressTest_MidLayer_Aync);

class StressTest_MidLayer_Aync_NoClear:public StressTest_MidLayer_Aync
{
public:
	StressTest_MidLayer_Aync_NoClear(const char* casename):StressTest_MidLayer_Aync(casename){}
	virtual void CreamteModel()
	{
		if (!m_ModelList.empty())
		{
			ReleaseModel();
		}
		int posModel=0;
		for (int i = 0; i < StaticModel_Num; ++i)
		{
			char name[64] = {0};
			sprintf(name, "static_model_%d", m_pathlist_loop_count * StaticModel_Num + i);
			CModelEntity* model_entity = m_factory->create_model(m_pDmlPathList[index], name, LL_ZERO, false);

			if (model_entity && m_level)
			{
				model_entity->set_position(H3DVec3(float(posModel - 4),0.f,0.f));
				model_entity->update(0);
				m_level->add_to_level(model_entity);
			}
			m_ModelList.push_back(model_entity);
			posModel++;
			index++;
		}
		index=0;
		m_pathlist_loop_count++;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("STRESS_TEST",StressTest_MidLayer_Aync_NoClear);

