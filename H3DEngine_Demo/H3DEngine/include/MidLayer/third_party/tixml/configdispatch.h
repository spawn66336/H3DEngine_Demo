#ifndef TINYXML_CONFIG_DISPATCH
#define TINYXML_CONFIG_DISPATCH

struct ISimpleXMLNode;

struct IConfigDispatch
{
	//3�������ֱ��ʾ�ļ����ݣ���ȡ�Ľڵ����ƣ��Ƿ��ʼ��
	virtual bool DispatchConfig(ISimpleXMLNode* nodedata,const char* name,bool init) = 0;
};

#endif
