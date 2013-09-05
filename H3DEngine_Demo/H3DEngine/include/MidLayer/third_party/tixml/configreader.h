#ifndef TINYXML_CONFIG_READER
#define TINYXML_CONFIG_READER

#include <platform/platform_shared/BiboFrame/BiboInterfaces.h>

#include "configdispatch.h"
//动态读取config的时间间隔为1分钟
enum
{
	LoadConfigInterval = 60000
};
enum CONFIG_UPDATE_MODE
{
	CUM_NO_UPDATE,
	CUM_UPDATE,
};

class ConfigReader
{
public:
	ConfigReader();
	~ConfigReader();
public:
	bool Init(IConfigDispatch* dispatch, const char* filename, CONFIG_UPDATE_MODE update_mode);
	//modify by neil:某些地方需要即时刷新一下
	bool LoadConfig(bool init);
	void Update();
protected:

	void LoadMacros();
private:
	//分发器指针
	IConfigDispatch*	m_config_dispatch;
	//文件全称，包含地址
	std::string	m_file_name;
	//文件上一次修改时间
	unsigned long long 		m_file_time_1;
	unsigned long long 		m_file_time_2;
	unsigned long long 		m_file_time_3;
	//是否动态读取
	CONFIG_UPDATE_MODE		m_update_mode;
	//宏字典
	IStatisticsCollector*	m_dict;
	//上次读取时间
	tick_t					m_last_update_config_time;
};

/**
帮助函数，为了配置方便，如果遇有多条记录，则可以用这个函数“删除”一些子节点。
要么指定count，要么从节点的“count”属性读取
*/
//by LYH
inline void ChopTreeNode(ISimpleXMLNode* node, const char* child_name, int count=-1)
{
	if(node==0)
		return;

	if(count==-1)
	{
		const char* str_count = node->GetAttribute("count");
		count = atoi(str_count);
		if(count==0)
			count=1;
	}

	std::string new_name = child_name;
	new_name = "__chopped_" + new_name;

	int i=0;
	for(ISimpleXMLNode* child=node->FirstChildNode(child_name);
		child;child=child->NextSibling(child_name))
	{
		if(i>=count)
		{
			child->SetValue(new_name.c_str());
		}
		++i;
	}
}

#endif
