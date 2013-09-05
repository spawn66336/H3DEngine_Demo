/*
add by eva
*/

#ifndef BIXML_HELPER_INCLUDED
#define BIXML_HELPER_INCLUDED

//#include "tinyxml.h"
#include "IXMLNode.h"
#include <list>
#include <map>

// ���ֹ�����tinyutil.h�еĹ����ظ�����Ҫ����ɾ��һ�������๦��
class BiXmlHelper
{
public:
	// ��ö�Ӧ�ڵ��ֵ����û�з���0
	static const char* get_value(BiXmlNode* node, const char* key);
	// ��ö�Ӧ�ڵ��attribute�����û�з���0
	static const char* get_attribute(BiXmlNode* node, const char* key);
	// ��ȡxml,ֻ��ȡ�ļ�,����������,ʹ֮ʹ�ø�xml�ļ�ʱ��,�������ļ��Ķ�ȡ��.
	// �����κ���������
	static bool LoadXMLDocument(const char* filename);
	//�鿴xml�ļ�,Ȼ����ݲ��������Ƿ���ļ�������ɾ��.Ĭ��ɾ��,��ʹ��һ��.
	//���remove����Ϊtrue,��֮����ͷŹ������ⲿִ��.
	static BiXmlDocument* PeekXMLDocument(const char* filename, bool remove = false);

	// �ӻ����ȡһ��xml�ĵ�, ���������û������ĵ�, �ͼ����ĵ�������
	static BiXmlDocument* GetXMLDocument(const char* filename);

	//ɾ������xml�ļ�
	static void ClearXMLDocument();
protected:
	typedef std::list<BiXmlDocument*> XmlDocumentList;
	static std::map<std::string, XmlDocumentList> m_xml_document_buf;
};

// �õ���Ӧ�ڵ��ֵ
template<class T>
inline bool tiny_parse_xml_value(T& v, const char* key, BiXmlNode* node)
{
	if(const char* str = BiXmlHelper::get_value(node, key))
	{
		v = str; // ����������������Է�ֹ��������ʹ��
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
