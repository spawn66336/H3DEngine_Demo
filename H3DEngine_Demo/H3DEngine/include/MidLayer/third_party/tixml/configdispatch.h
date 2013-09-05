#ifndef TINYXML_CONFIG_DISPATCH
#define TINYXML_CONFIG_DISPATCH

struct ISimpleXMLNode;

struct IConfigDispatch
{
	//3个参数分别表示文件数据，读取的节点名称，是否初始化
	virtual bool DispatchConfig(ISimpleXMLNode* nodedata,const char* name,bool init) = 0;
};

#endif
