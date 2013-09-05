#include "RefCountCompare.h"
#include "engine_interface.h"
#include "HippoGlobal.h"
#include <math.h>

HippoResStateSnapShot::HippoResStateSnapShot(int count,HIPPO_RESTYPE t)
{
	m_pData=new _SingleResState[count];
	m_res_count=count;
	m_restype=t;
}


void HippoResStateSnapShot::Release()
{
	delete[] m_pData;
	delete this;
}


//add for snapshot diff
HippoResStateSSDiff::HippoResStateSSDiff(HIPPO_RESTYPE t )
{
	m_restype=t;
}
void HippoResStateSSDiff::Release( )
{
	delete this;
}


void RefCountCompare::GetCurrentResRefChange(HippoResStateSnapShot* l,HippoResStateSnapShot* r)
{
	for (int i =0; i < r->m_res_count; ++i)
	{
		int bfind = 0;
		_SingleResState* right=r->m_pData+i;

		for (int j = 0; j < l->m_res_count; ++j)
		{
			_SingleResState* left=l->m_pData+j;

			int name_r=strcmp(left->m_name,right->m_name);
			bool refc_r=left->m_ref == right->m_ref;
			
			if (name_r == 0)
			{
				bfind = 1;
				if( !refc_r ) 
				{
					Hippo_WriteConsole(CC_RED,	"%s Ref由%d 到 %d", left->m_name, left->m_ref, right->m_ref);
					break;
				}
			}
		}
		if (bfind == 0)
		{
			Hippo_WriteConsole(CC_WHITE,	"%s Ref由1 到 %d", right->m_name,  right->m_ref);
			/// 没找到
		}
	}

	for (int i=0;i<l->m_res_count;++i)
	{
		_SingleResState* left=l->m_pData+i;
		_SingleResState* right=r->m_pData+i;

		int name_r=strcmp(left->m_name,right->m_name);
		bool refc_r=left->m_ref == right->m_ref;

		if(name_r!=0 || !refc_r) //名字和ref 只要有一个不等 就认为不同
		{
			Hippo_WriteConsole(CC_RED,	"资源%s 引用计数由%d 到 %d", left->m_name, left->m_ref, right->m_ref);
		}
	}
}


//得到当前时刻引擎的某种资源（已完成加载的资源）的状态
HippoResStateSnapShot* RefCountCompare::GetResState(HIPPO_RESTYPE t)
{
	H3DI::IRender* pRender=Hippo_GetIRender();
	int mng_type=t;
	void* pRes=pRender->QueryInfo(4,(void*)&mng_type,0,0,0,0); //获取该类资源当前时刻的数目
	int count=(int)(*((int*)pRes));
	
	HippoResStateSnapShot* pResInfo=new HippoResStateSnapShot(count,t);		//分配空间
	pRender->QueryInfo(5,(void*)&mng_type,(void*)(pResInfo->m_pData),0,0,0);//获取每个资源的信息

	return pResInfo;
}

//比较两个快照是否相等
bool RefCountCompare::SnapShotEqual(HippoResStateSnapShot* l,HippoResStateSnapShot* r)
{
	//首先比较数量
	if(l->m_res_count != r->m_res_count)
		return false;

	//容器内应该是有序的
	for (int i=0;i<l->m_res_count;++i)
	{
		_SingleResState* left=l->m_pData+i;
		_SingleResState* right=r->m_pData+i;

		int name_r=strcmp(left->m_name,right->m_name);
		bool refc_r=left->m_ref == right->m_ref;

		if(name_r!=0 || !refc_r) //名字和ref 只要有一个不等 就认为不同
			return false;
	}
	return true;
}


 HippoResStateSSDiff* RefCountCompare::GetSnapShotDiff(HippoResStateSnapShot* l,HippoResStateSnapShot* r,bool bLog)
 {

	 //首先得到  type
	 HIPPO_RESTYPE type = l->m_restype;
	 HippoResStateSSDiff* pResInfo = new HippoResStateSSDiff(type);

	 for (int i = 0;i<l->m_res_count;i++)
	 {
		  _SingleResState* left = l->m_pData+i;
		  if (bLog)
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s   %d----",left->m_name,left->m_ref);

			int j=0;
			for (;j<r->m_res_count;j++)
			{
				_SingleResState* right = r->m_pData+j;

				bool bName = strcmp(left->m_name,right->m_name)==0;
				bool bRef = left->m_ref == right->m_ref;

				if (bName && bRef)
				{
					break;
				}
				if (bName && !bRef) //名字相等 数量不等 both
				{
					_DiffResState dBoth;
					strcpy_s(dBoth.m_name,256,left->m_name);
					dBoth.m_ref_diff = abs(left->m_ref-right->m_ref);
					dBoth.m_position = BOTH;
					pResInfo->m_DiffVec.push_back(dBoth);
					break;
				} 
			}
			if (j ==r->m_res_count )
			{
				_DiffResState dLeft;
				strcpy_s(dLeft.m_name,256,left->m_name);
				dLeft.m_ref_diff = left->m_ref;
				dLeft.m_position = LEFT;
				pResInfo->m_DiffVec.push_back(dLeft);
			}
	 }

	 for (int i = 0;i<r->m_res_count;i++)
	 {
		 _SingleResState* right = r->m_pData+i;
		 if (bLog)
			Hippo_WriteConsoleAndLog(CC_WHITE,"%s   %d----",right->m_name,right->m_ref);
		int j=0;
		for (;j<l->m_res_count;j++)
		{
			_SingleResState* left = l->m_pData+j;
			bool bName = strcmp(left->m_name,right->m_name)==0;
			if (bName)  //只要名字相等 就跳出 循环  
			{
				break;
			}
		}
		if (j ==l->m_res_count )
		{
			_DiffResState dRight;
			strcpy_s(dRight.m_name,256,right->m_name);
			dRight.m_ref_diff = right->m_ref;
			dRight.m_position = RIGHT;
			pResInfo->m_DiffVec.push_back(dRight);
		}
	 }

	 return pResInfo;
 }