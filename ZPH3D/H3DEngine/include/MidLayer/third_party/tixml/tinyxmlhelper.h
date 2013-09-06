/*
add by eva
*/

#ifndef BIXML_HELPER_INCLUDED
#define BIXML_HELPER_INCLUDED

//#include "tinyxml.h"
#include "IXMLNode.h"
#include <list>
#include <map>

// 部分功能与tinyutil.h中的功能重复，需要尽快删除一部分冗余功能
class BiXmlHelper
{
public:
	// 获得对应节点的值，如没有返回0
	static const char* get_value(BiXmlNode* node, const char* key);
	// 获得对应节点的attribute，如果没有返回0
	static const char* get_attribute(BiXmlNode* node, const char* key);
	// 读取xml,只读取文件,并保存起来,使之使用该xml文件时候,不进行文件的读取了.
	// 不做任何其他操作
	static bool LoadXMLDocument(const char* filename);
	//查看xml文件,然后根据参数决定是否从文件队列中删除.默认删除,即使用一次.
	//如果remove参数为true,则之后的释放工作由外部执行.
	static BiXmlDocument* PeekXMLDocument(const char* filename, bool remove = false);

	// 从缓存获取一个xml文档, 如果缓存里没有这个文档, 就加载文档到缓存
	static BiXmlDocument* GetXMLDocument(const char* filename);

	//删除所有xml文件
	static void ClearXMLDocument();
protected:
	typedef std::list<BiXmlDocument*> XmlDocumentList;
	static std::map<std::string, XmlDocumentList> m_xml_document_buf;
};

// 得到对应节点的值
template<class T>
inline bool tiny_parse_xml_value(T& v, const char* key, BiXmlNode* node)
{
	if(const char* str = BiXmlHelper::get_value(node, key))
	{
		v = str; // 这里会产生编译错误，以防止函数的误使用
		return true;
	}
	else
		return false;
}

template<>
inline bool tiny_parse_xml_value(int& v, const char* key, BiXmlNode* node)
{
	if(const char* str = BiXmlHelper::get_value(node, key))
	{
		v = atoi(str);
		return true;
	}
	else
		return false;
}

template<>
inline bool tiny_parse_xml_value(unsigned int& v, const char* key, BiXmlNode* node)
{
	if(const char* str = BiXmlHelper::get_value(node, key))
	{		
		v = atoi(str);
		return true;
	}
	else
		return false;
}

template<>
inline bool tiny_parse_xml_value(double& v, const char* key, BiXmlNode* node)
{
	if(const char* str = BiXmlHelper::get_value(node, key))
	{
		v = atof(str);
		return true;
	}
	else
		return false;
}

template<>
inline bool tiny_parse_xml_value(float& v, const char* key, BiXmlNode* node)
{
	if(const char* str = BiXmlHelper::get_value(node, key))
	{
		v = static_cast<float>(atof(str));
		return true;
	}
	else
		return false;
}


#endif // BIXML_HELPER_INCLUDED
