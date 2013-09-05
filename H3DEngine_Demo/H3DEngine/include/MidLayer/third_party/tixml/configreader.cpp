#include "configreader.h"
#include "configdispatch.h"

ConfigReader::ConfigReader()
	:m_config_dispatch(0), m_update_mode(CUM_NO_UPDATE)
	,m_file_time_1(0), m_file_time_2(0), m_file_time_3(0)
	,m_last_update_config_time(get_tick_count())
{
	m_dict = GetBiboRegistry()->CreateStatisticsCollector();
	LoadMacros();
}

ConfigReader::~ConfigReader()
{
	m_dict->Release();
}

void ConfigReader::LoadMacros()
{
	ISimpleXMLDocument* doc = GetBiboRegistry()->LoadXMLFile("./config/macros.xml");
	if(doc)
	{
		ISimpleXMLNodeList* nodes = doc->GetElementsByTagName("macro");
		for(int i=0;i<nodes->GetLength();++i)
		{
			ISimpleXMLNode* macro = nodes->GetNode(i);
			const char* name = macro->GetAttribute("name");
			const char* value= macro->GetAttribute("value");
			m_dict->SetString(name, value);
		}
		nodes->Release();
		doc->Release();
	}
}

bool ConfigReader::Init(IConfigDispatch* dispatch, const char* filename, CONFIG_UPDATE_MODE update_mode)
{
	strongAssert(dispatch);
	m_config_dispatch = dispatch;
	m_file_name = filename;
	m_update_mode = update_mode;

	return LoadConfig(true);
}

void ConfigReader::Update()
{
	if (m_update_mode == CUM_UPDATE)
	{
		if (get_tick_count()-m_last_update_config_time > LoadConfigInterval)
		{
			LoadConfig(false);
			m_last_update_config_time = get_tick_count();
		}
	}
}

bool ConfigReader::LoadConfig(bool init)
{
	tick_t file_time_1 = (tick_t)get_file_time(m_file_name.c_str());
	tick_t file_time_2 = (tick_t)get_file_time("./config/globals.xml");
	tick_t file_time_3 = (tick_t)get_file_time("./config/macros.xml");

	if (!init)
	{
		if( file_time_1 == m_file_time_1
		  &&file_time_2 == m_file_time_2
		  &&file_time_3 == m_file_time_3
			)
		{	//文件没有更新
			return false;
		}
	}

	bool flag = false;

	m_file_time_1 = file_time_1;
	m_file_time_2 = file_time_2;
	m_file_time_3 = file_time_3;

	ISimpleXMLDocument* doc = GetBiboRegistry()->LoadXMLFile(m_file_name.c_str());

	if (doc)
	{
		//替换节点
		ISimpleXMLDocument* dict = GetBiboRegistry()->LoadXMLFile("./config/globals.xml");
		if(dict)
		{
			doc->ReplaceNodes(dict);
			dict->Release();
		}
		//替换宏
		doc->ReplaceMacros(m_dict);

		flag = m_config_dispatch->DispatchConfig(doc->GetRootNode(), doc->GetTagName(), init);
		doc->Release();
	}

	return flag;
}
