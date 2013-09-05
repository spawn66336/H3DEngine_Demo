#include "../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "EffectCore_dx.h"
using namespace  std;

/*测试逻辑
 1.从外界读取一个mat创建1个inewshader
 2.使用inewshader，和drawindex绘制四边形并渲染5s
 3.释放，比较前后引用计数
*/

class MatRefCountNewshader : public HippoTestCaseBase
{
public:
	MatRefCountNewshader(const char* casename):HippoTestCaseBase(casename){}

	HippoLevelScene* m_pScene;
	H3DI::INewShader* m_pNewshader;

	enum ExeState
	{
		INIT,
		READ_MAT,
		REND_5_SEC,
		MAT_RELEASE,
		SNAPSHOT_COM,
		TEST_OVER
	};
	ExeState m_exeState;

	struct sSnapShot
	{
		HippoResStateSnapShot* SnapShot_Mat;
		HippoResStateSnapShot* SnapShot_Tex;
		HippoResStateSnapShot* SnapShot_Tpl;
	};
    sSnapShot beforSnapShot;
	sSnapShot afterSnapShot;

	H3DI::IVB* m_pVB ;
	


private:
	static HippoTestCaseBase* const testcase_;

public:
	virtual void InitScene()
	{
		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
		CreateQuad();
		m_pNewshader=0;
		m_exeState = INIT;

	}

	virtual void CleanUpScene()
	{
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
	}

	void CreateNewShader()
	{
		m_pNewshader = Hippo_GetIRender()->GetNewShader("../resources/art/role/link/female/4110/4110001001/4110001001.mat","4110001001");
	}

	void CreateQuad()
	{
		  m_pVB = Hippo_GetIRender()->GetDynamicVB();
		 if (!m_pVB)
		 {
			 Hippo_WriteConsoleAndLog(CC_WHITE,"%s","GetDynamicVB error!");
		 }
		 bool bCreateBuf = m_pVB->CreateBuffer(36 * 8192, 4096, - 1, - 1, 0, 0);
		 if (!bCreateBuf)
		 {
			  Hippo_WriteConsoleAndLog(CC_WHITE,"%s","CreateBuffer error!");
		 }

		 float points[12] = {0,0,-1, 1,0,-1, 1,1,1, 0,1,1};
		 float color[12] = {1,1,1, 1,1,1, 1,1,1, 1,1,1};
		 int index[6] = {0,1,2,0,2,3};
		 float* pVertex = (float*)m_pVB->Lock(H3DI::VB_VERTEX_ARRAY,H3DI::VB_POS|H3DI::VB_COLOR|H3DI::VB_UV,36,4);
		 int vpos = 0;
		 for (int i=0; i<4 ;i++)
		 {
			 pVertex[vpos++] = points[i*3];
			 pVertex[vpos++] = points[i*3+1];
			 pVertex[vpos++] = points[i*3+2];
			 pVertex[vpos++]= 1;
			 pVertex[vpos++]= 1;
			 pVertex[vpos++]= 1;
			 pVertex[vpos++]= 1;
			 pVertex[vpos++]= 1;
			 pVertex[vpos++]= 1;
		 }
		 m_pVB->UnLock(H3DI::VB_VERTEX_ARRAY);

		 unsigned short* pIndex = (unsigned short*)m_pVB->Lock(H3DI::VB_ELEMENT_ARRAY,-1,-1,6);
		 for (int i= 0;i<6 ;i++)
		 {
			 pIndex[i] = index[i];
		 }
		 m_pVB->UnLock(H3DI::VB_ELEMENT_ARRAY);

	}

	void DrawSquare()
	{
		Hippo_GetIRender()->FrameBegin();

		Hippo_GetIRender()->UpdatePhx();

		Hippo_GetIRender()->PushScene(m_pScene->GetH3DScene());
		Hippo_GetIRender()->Render();

		if (m_pNewshader)
		{
			Hippo_GetIRender()->SetNewShader(m_pNewshader);
			Hippo_GetIRender()->DrawIndexNew(H3DI::TRIANGLE_LIST,6,4,0,0,0,0);
		}

		Hippo_GetIRender()->FrameEnd();
	}

	void RelaseShader()
	{
		m_pScene->DelShader(m_pNewshader);
	}

	void GetNowSnapShot(sSnapShot & snapshot)
	{
		snapshot.SnapShot_Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
		snapshot.SnapShot_Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
		snapshot.SnapShot_Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);
	}

	void SnapShotCom(sSnapShot& before,sSnapShot& after)
	{
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(before.SnapShot_Mat,after.SnapShot_Mat),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(before.SnapShot_Tex,after.SnapShot_Tex),true);
		HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(before.SnapShot_Tpl,after.SnapShot_Tpl),true);
		before.SnapShot_Mat->Release();
		before.SnapShot_Tex->Release();
		before.SnapShot_Tpl->Release();
		after.SnapShot_Mat->Release();
		after.SnapShot_Tex->Release();
		after.SnapShot_Tpl->Release();
	}
	void NextState()
	{
		int* p = (int*)(&m_exeState);
		++(*p);
	}
	bool Executing()
	{
		switch(m_exeState)
		{
		case  INIT:
			GetNowSnapShot(beforSnapShot);
			NextState();
			break;
		case READ_MAT:
			CreateNewShader();
			NextState();
			break;
		case REND_5_SEC:
			if (Spin_N_Ms(5000))
			{
				NextState();
			}
			break;
		case MAT_RELEASE:
			RelaseShader();
			GetNowSnapShot(afterSnapShot);
			NextState();
			break;
		case SNAPSHOT_COM:
			SnapShotCom(beforSnapShot,afterSnapShot);
			NextState();
			break;
		}
		return false;
	}
	virtual void AfterRender()
	{
		DrawSquare();
	}
	virtual bool AutoRunExec()
	{
		if (m_exeState == TEST_OVER )
		{
			return true;
		}
		return Executing();
	}
	virtual bool ManualRunExec()
	{
		return Executing();

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
ADD_TESTCASE("MATERIAL_TEST",MatRefCountNewshader);