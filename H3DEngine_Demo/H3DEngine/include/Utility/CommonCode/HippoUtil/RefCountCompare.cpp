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
					Hippo_WriteConsole(CC_RED,	"%s Ref��%d �� %d", left->m_name, left->m_ref, right->m_ref);
					break;
				}
			}
		}
		if (bfind == 0)
		{
			Hippo_WriteConsole(CC_WHITE,	"%s Ref��1 �� %d", right->m_name,  right->m_ref);
			/// û�ҵ�
		}
	}

	for (int i=0;i<l->m_res_count;++i)
	{
		_SingleResState* left=l->m_pData+i;
		_SingleResState* right=r->m_pData+i;

		int name_r=strcmp(left->m_name,right->m_name);
		bool refc_r=left->m_ref == right->m_ref;

		if(name_r!=0 || !refc_r) //���ֺ�ref ֻҪ��һ������ ����Ϊ��ͬ
		{
			Hippo_WriteConsole(CC_RED,	"��Դ%s ���ü�����%d �� %d", left->m_name, left->m_ref, right->m_ref);
		}
	}
}


//�õ���ǰʱ�������ĳ����Դ������ɼ��ص���Դ����״̬
HippoResStateSnapShot* RefCountCompare::GetResState(HIPPO_RESTYPE t)
{
	H3DI::IRender* pRender=Hippo_GetIRender();
	int mng_type=t;
	void* pRes=pRender->QueryInfo(4,(void*)&mng_type,0,0,0,0); //��ȡ������Դ��ǰʱ�̵���Ŀ
	int count=(int)(*((int*)pRes));
	
	HippoResStateSnapShot* pResInfo=new HippoResStateSnapShot(count,t);		//����ռ�
	pRender->QueryInfo(5,(void*)&mng_type,(void*)(pResInfo->m_pData),0,0,0);//��ȡÿ����Դ����Ϣ

	return pResInfo;
}

//�Ƚ����������Ƿ����
bool RefCountCompare::SnapShotEqual(HippoResStateSnapShot* l,HippoResStateSnapShot* r)
{
	//���ȱȽ�����
	if(l->m_res_count != r->m_res_count)
		return false;

	//������Ӧ���������
	for (int i=0;i<l->m_res_count;++i)
	{
		_SingleResState* left=l->m_pData+i;
		_SingleResState* right=r->m_pData+i;

		int name_r=strcmp(left->m_name,right->m_name);
		bool refc_r=left->m_ref == right->m_ref;

		if(name_r!=0 || !refc_r) //���ֺ�ref ֻҪ��һ������ ����Ϊ��ͬ
			return false;
	}
	return true;
}


 HippoResStateSSDiff* RefCountCompare::GetSnapShotDiff(HippoResStateSnapShot* l,HippoResStateSnapShot* r,bool bLog)
 {

	 //���ȵõ�  type
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
				if (bName && !bRef) //������� �������� both
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
			if (bName)  //ֻҪ������� ������ ѭ��  
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