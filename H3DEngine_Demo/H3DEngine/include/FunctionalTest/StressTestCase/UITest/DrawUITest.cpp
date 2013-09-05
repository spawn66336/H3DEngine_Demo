#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "engine_interface_internal.h"
#include "../CommonCode/HippoUtil/HippoScene.h"
#include "../CommonCode/HippoUtil/FileOperation.h"
#include "EffectCore_dx.h"
#include "engine_interface.h"
#include "idMath/dMathHeader.h"
#include "../../FunctionalTest/functional_test/HippoTestConfig.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <map>
#include <fstream>
#include <sstream>
#include "../RandomUtil.h"
#include <time.h>
using namespace  std;


struct sRect;
struct UirResource;

//get MaterialName
std::string GetMatrialName(std::string fileName)
{
	TiXmlDocument doc(fileName.c_str());
	if (!doc.LoadFile())
	{
		::MessageBox(0,"get mat name error","Error",0);
	}
	TiXmlElement* pRoot=doc.FirstChildElement("H3DMaterialInstanceRoot");
	if (!pRoot)
	{
		::MessageBox(0,"get mat name error","Error",0);
	}
	TiXmlElement* pElem=pRoot->FirstChildElement("H3DMaterialInstance");
	std::string matName;
	if (pElem)
	{
		matName=pElem->Attribute("MaterialName");
	}
	else
	{
		::MessageBox(0,"get mat name error","Error",0);
	}
	doc.Clear();
	return matName;
}

std::string GetMatPath(std::string fileName)
{
	int idx=fileName.find_last_of(".");
	if (idx!=-1)
		return fileName.substr(0,idx+1)+"mat";
}
//辅助函数 
template <typename T>
T StringToNum(const string& str)
{
	T num;
	istringstream isstr(str);
	isstr >> num;
	return num;
}




struct sRect
{
	float left;
	float top;
	float right;
	float bottom;
	sRect(float x,float y,float z,float w):left(x),top(y),right(z),bottom(w){}
};
struct UirResource
{
	std::string pngName;
	 float pngWidth;
	 float pngHight;
	std::string shaderName;
	std::map<std::string,sRect> rectCon;
	void Release(){rectCon.clear();}
};

sRect GetRandomRect()
{
	float left=GetRandomFloat(-100,100);
	float top=GetRandomFloat(-100,100);
	float width=GetRandomFloat(-100,100);
	float hight=GetRandomFloat(-100,100);
	return sRect(left,top,left+width,top-hight);
}

//解析uir文件
void ParseUirFile(const char* fileName,UirResource* res)
{
	ifstream infile;
	infile.open(fileName);
	if (!infile)
	{
		Hippo_WriteConsoleAndLog(CC_WHITE,"infile error");
		return ;
	}
	std::string line,word;
	int index=0;
	while(getline(infile,line))
	{
		istringstream _stream(line);
		if (index==0)
		{
			int idx=0;
			while(_stream>>word)
			{
				if (idx==1)
				{
					res->pngName=word;
				}
				idx++;
			}
		}
		else if (index==1)
		{
			int idx=0;
			while(_stream>>word)
			{
				if (idx==1)
				{
					res->pngWidth=StringToNum<float>(word);//类型转换
				}
				if (idx==3)
				{
					res->pngHight=StringToNum<float>(word);//类型转换
				}
				idx++;
			}
		}
		else if (index==2)
		{
			int idx=0;
			while(_stream>>word)
			{
				if (idx==1)
				{
					res->shaderName=word;
				}
				idx++;
			}
		}
		else if (index==3)
		{
		}
		else 
		{
			std::string ID;
			float posX,posY,width,hight;
			int idx=0;
			while(_stream>>word)
			{
				if (idx==0)
				{
					ID=word;
				}
				else if (idx==1)
				{
					posX=StringToNum<float>(word);
				}
				else if (idx==2)
				{
					posY=StringToNum<float>(word);
				}
				else if (idx==3)
				{
					width=StringToNum<float>(word);
				}
				else if (idx==4)
				{
					hight=StringToNum<float>(word);
				}
				idx++;
			}

			float left=1/res->pngWidth;
			float top=(res->pngHight-posY)/res->pngHight;
			float right=left + width/res->pngWidth;
			float bottom=top - hight/res->pngHight;

			res->rectCon.insert(make_pair(ID,sRect(left,top,right,bottom)));
		}
		index++;
		
	}
}


class DrawUITest:HippoTestCase_RefCompare
{
public:
public:
	DrawUITest(const char* casename):HippoTestCase_RefCompare(casename){}

	HippoLevelScene* m_pScene;
	H3DI::INewShader* m_pNewshader;
	H3DI::IVB* m_pVB ;
	UirResource m_uirRes;

private:
	static HippoTestCaseBase* const testcase_;

public:
	virtual void InitScene()
	{
		srand((unsigned)time(NULL));

		m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();

		std::string uirName=Make_X52Resources_Path("art/ui/worldmap/x52_ui_wordmap2.uir");
		ParseUirFile(uirName.c_str(),&m_uirRes);
		std::string matPath=GetMatPath(uirName);

		CreateVbIb();
		
		m_pNewshader = Hippo_GetIRender()->GetNewShader(matPath.c_str(),GetMatrialName(matPath).c_str());
	}

	void CreateVbIb()
	{
		m_pVB = Hippo_GetIRender()->GetDynamicVB();
		if (!m_pVB)
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s","GetDynamicVB error!");
		}
		bool bCreateBuf = m_pVB->CreateBuffer(36 * 4096, 4096, - 1, - 1, 0, 0);
		if (!bCreateBuf)
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s","CreateBuffer error!");
		}

		float r = 1;
		float g = 1;
		float b = 1;
		float alpha = 1;

		std::map<std::string,sRect>::iterator itr=m_uirRes.rectCon.begin();
		float* pVertex = (float*)m_pVB->Lock(H3DI::VB_VERTEX_ARRAY,H3DI::VB_POS|H3DI::VB_COLOR|H3DI::VB_UV,36,4096);
		for (int i=0;itr!=m_uirRes.rectCon.end();itr++)
		{
			sRect& res=itr->second;
			sRect rect=GetRandomRect();
			
			pVertex[i++] = rect.left;		pVertex[i++] = 0;    pVertex[i++] = rect.top;
			pVertex[i++] = r;pVertex[i++] = g;pVertex[i++] = b;pVertex[i++] = alpha;
			pVertex[i++] = res.left;	pVertex[i++] = 1 - res.top;

			pVertex[i++] = rect.left;		pVertex[i++]=0;   pVertex[i++] = rect.bottom;
			pVertex[i++] = r; pVertex[i++] = g; pVertex[i++] = b; pVertex[i++] = alpha;
			pVertex[i++] = res.left;	pVertex[i++] = 1 - res.bottom;

			pVertex[i++] = rect.right;	pVertex[i++] =0;		pVertex[i++] = rect.top;
			pVertex[i++] = r; pVertex[i++]= g; pVertex[i++] = b; pVertex[i++]= alpha;
			pVertex[i++] = res.right;	pVertex[i++]= 1 - res.top;

			pVertex[i++] = rect.right;	pVertex[i++] =0;	pVertex[i++] = rect.bottom;
			pVertex[i++] = r; pVertex[i++]= g; pVertex[i++] = b; pVertex[i++]= alpha;
			pVertex[i++] = res.right;	pVertex[i++]= 1 - res.bottom;				
		}
		m_pVB->UnLock(H3DI::VB_VERTEX_ARRAY);

		unsigned short* pIndex = (unsigned short*)m_pVB->Lock(H3DI::VB_ELEMENT_ARRAY,-1,-1,4896);

		std::map<std::string,sRect>::iterator it=m_uirRes.rectCon.begin();
		for (int ibIdx=0;it!=m_uirRes.rectCon.end();it++)
		{
			pIndex[ibIdx++] = 0; pIndex[ibIdx++]= 1; pIndex[ibIdx++]=2; pIndex[ibIdx++] = 3;
		}
		m_pVB->UnLock(H3DI::VB_ELEMENT_ARRAY);
	}

	void DrawUir()
	{
		if (m_pNewshader)
		{
			int vbOffset = 0;
			int ibOffset = 0;

			std::map<std::string,sRect>::iterator itr=m_uirRes.rectCon.begin();
			for (;itr!=m_uirRes.rectCon.end();itr++)
			{
				Hippo_GetIRender()->SetNewShader(m_pNewshader);
				Hippo_GetIRender()->DrawIndexNewEx(H3DI::TRIANGLE_STRIP, 4, 4
				,vbOffset * sizeof(float), ibOffset * sizeof(unsigned short), -1, -1, -1, -1);
				vbOffset += 4 * 9;
				ibOffset += 4;
			}
			
		}
	}

	virtual void AfterRender()
	{
		if (Spin_N_Ms(5000))
		{
			m_pVB->Release();
			CreateVbIb();
		}
		DrawUir();
	}
	virtual bool AutoRunExec()
	{
		return false;
	}
	virtual bool ManualRunExec()
	{
		return false;

	}

	virtual void CleanUpScene()
	{
		m_uirRes.Release();
		m_pVB->Release();
		m_pVB=0;
		m_pScene->DelShader(m_pNewshader);
		m_pScene->CleanScene();
		Hippo_GetSceneManager()->DelScene(m_pScene);
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
ADD_TESTCASE("UIR_TEST",DrawUITest);