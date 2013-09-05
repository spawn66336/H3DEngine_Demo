#ifndef TINYXML_CONFIG_READER
#define TINYXML_CONFIG_READER

#include <platform/platform_shared/BiboFrame/BiboInterfaces.h>

#include "configdispatch.h"
//��̬��ȡconfig��ʱ����Ϊ1����
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
	//modify by neil:ĳЩ�ط���Ҫ��ʱˢ��һ��
	bool LoadConfig(bool init);
	void Update();
protected:

	void LoadMacros();
private:
	//�ַ���ָ��
	IConfigDispatch*	m_config_dispatch;
	//�ļ�ȫ�ƣ�������ַ
	std::string	m_file_name;
	//�ļ���һ���޸�ʱ��
	unsigned long long 		m_file_time_1;
	unsigned long long 		m_file_time_2;
	unsigned long long 		m_file_time_3;
	//�Ƿ�̬��ȡ
	CONFIG_UPDATE_MODE		m_update_mode;
	//���ֵ�
	IStatisticsCollector*	m_dict;
	//�ϴζ�ȡʱ��
	tick_t					m_last_update_config_time;
};

/**
����������Ϊ�����÷��㣬������ж�����¼������������������ɾ����һЩ�ӽڵ㡣
Ҫôָ��count��Ҫô�ӽڵ�ġ�count�����Զ�ȡ
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
