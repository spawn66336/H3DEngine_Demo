//#include "../../../platform/share/netengine/BiboFrame/BiboInterfaces.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tinyxmlhelper.h"

//#include "../client/i_log.h"
//#include "../client/global_share.h"
//#include "../client/imp/um_converter.h"
//void change_file_extension();
static const char BXMLEXT[] = "xm6";
static const char XMLEXT[] = "xml";
static const bool bBinFirst = true;
void get_file_extension(const char* filename, char* extension)
{
	if (!filename || !extension)
		return;
	memset(extension, 0, strlen(extension));
	int n = strlen(filename);
	for (int i = n; i > 0; --i)
	{
		if(filename[i - 1] == '.' && i < n)
		{
			strcpy(extension, (filename + i));
			break;
		}
	}
}
void change_file_extension(const char* filename, const char* extension, char* newfilename)
{
	if (!filename || !extension || !newfilename)
		return;
	memset(newfilename, 0, strlen(newfilename));
	int n = strlen(filename);
	for (int i = n; i > 0; --i)
	{
		if(filename[i - 1] == '.' && i < n)
		{
			for (int j = 0; j < i; ++j)
				newfilename[j] = filename[j];

			strcpy(newfilename+i, extension);
			break;
		}
	}
}
bool compare_without_LU(const char* str1, const char* str2)
{
	bool bRet = false;
	if (!str1 || !str2)
		return bRet;

	char* tempstr1 = strdup(str1);
	char* tempstr2 = strdup(str2);

	tempstr1 = strlwr(tempstr1);
	tempstr2 = strlwr(tempstr2);

	if (strcmp(tempstr1, tempstr2) == 0)
		bRet = true;

	free(tempstr1);
	free(tempstr2);

	return bRet;
}

const char* BiXmlHelper::get_value(BiXmlNode* node, const char* key)
{
	BiXmlNode *item = node->FirstChild(key);
	if(item)
	{
		BiXmlNode* subnode = item->FirstChild();
		if(!subnode)
			return 0;
		else
			return subnode->Value();
	}
	else
		return 0;
}

const char* BiXmlHelper::get_attribute(BiXmlNode* node, const char* key)
{
	BiXmlElement* ele = node->ToElement();
	if(ele)
	{
		return ele->Attribute(key);
	}
	else
		return 0;
}
//zzsa 新加,为了预先读取xml,并保存
std::map<std::string, BiXmlHelper::XmlDocumentList> BiXmlHelper::m_xml_document_buf;
bool BiXmlHelper::LoadXMLDocument(const char* filename)
{
	if (!filename) 
	{
#ifdef WIN32
//		LogWarning("BiXmlHelper::LoadXMLDocument filename is 0\n");
#endif
		return false;
	}

	//创建
	//BiXmlDocument* xml = new BiXmlDocument(filename);
	BiXmlDocument* xml = NULL;

	//如果二进制优先
	if (bBinFirst)
	{
		char newfilename[256] = {};
		
		change_file_extension(filename, BXMLEXT, newfilename);
		//先读二进制xml
		xml = XmlDocFactory::CreateDocument(DT_BIN);
		if (xml && xml->LoadFile(newfilename))
		{
			//二进制文件的名称也按照非二进制文件名称来索引。
			//因为目前所有的二进制xml文件均由工具生成。
			m_xml_document_buf[std::string(filename)].push_back(xml);
			return true;
		}
		XmlDocFactory::ReleaseDocument(xml);
		//二进制文件读取不成功，再读取文本xml
		xml = XmlDocFactory::CreateDocument();
		if (xml && xml->LoadFile(filename))
		{
			m_xml_document_buf[std::string(filename)].push_back(xml);
			return true;
		}

		XmlDocFactory::ReleaseDocument(xml);
		return false;
	}
	
	//获得文档扩展名
	char extension[256] = {};
	get_file_extension(filename, extension);

	//判断是否为二进制xml文档。
	bool bBin = compare_without_LU(extension, BXMLEXT);
	bool bTxt = compare_without_LU(extension, XMLEXT);

	//二进制
	if (bBin && !bTxt)
	{
		xml = XmlDocFactory::CreateDocument(DT_BIN);
	}
	else
	{
		xml = XmlDocFactory::CreateDocument();
	}
	//读取
	if (!xml || !xml->LoadFile(filename))
	{
		const char* errordesc = xml->ErrorDesc();
		if(errordesc)
		{
//			LogWarning("BiXmlHelper::LoadXMLDocument failed filename", filename, "cause:", errordesc);
		}
		else
		{
//			LogWarning("BiXmlHelper::LoadXMLDocument failed filename", filename, "no cause");
		}

//		LogWarning("BiXmlHelper::LoadXMLDocument failed filename", filename);

		//delete xml;
		XmlDocFactory::ReleaseDocument(xml);
		return false;
	}

	//保存
	m_xml_document_buf[std::string(filename)].push_back(xml);

#ifdef WIN32
//	LogWarning("BiXmlHelper::LoadXMLDocument load done for ", filename, (int)m_xml_document_buf[std::string(filename)].size());
#endif

	return true;
}
BiXmlDocument* BiXmlHelper::PeekXMLDocument(const char* filename, bool remove)
{
	if (!filename) 
		return 0;
	//查找对应名字
	std::map<std::string, XmlDocumentList>::iterator it0 = m_xml_document_buf.find(std::string(filename));
	if (it0 == m_xml_document_buf.end())
		return 0;
	//如果这个列表为空的话,清除掉他,返回0,这种情况应该不会存在的.
	XmlDocumentList& xmllist = it0->second;
	if (xmllist.empty())
	{
		m_xml_document_buf.erase(it0);
		return 0;
	}
	//对应名字的xml列表内取第一个
	BiXmlDocument* xml = xmllist.front();
	if (remove)
	{
		xmllist.pop_front();
		if (xmllist.empty())
			m_xml_document_buf.erase(it0);
	}
	//
	return xml;
}

BiXmlDocument* BiXmlHelper::GetXMLDocument(const char* filename)
{
	BiXmlDocument* doc = PeekXMLDocument(filename, false);
	if (!doc)
	{
		LoadXMLDocument(filename);
	}
	return PeekXMLDocument(filename);
}

void BiXmlHelper::ClearXMLDocument()
{
	std::map<std::string, XmlDocumentList>::iterator it0 = m_xml_document_buf.begin();
	for (; it0 != m_xml_document_buf.end(); it0++)
	{
		XmlDocumentList::iterator it1 = it0->second.begin();
		for (; it1 != it0->second.end(); it1++)
		{
			BiXmlDocument* xml = *it1;
			//delete xml;
			XmlDocFactory::ReleaseDocument(xml);
		}
	}
	m_xml_document_buf.clear();
}