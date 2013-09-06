#ifndef __BINXML_CONVERTER_H__93ksdf__
#define __BINXML_CONVERTER_H__93ksdf__

#include "../tinyxml/tinyxml.h"
#include "bin_xml.h"

class BinXMLConvert
{
	BinXML_Header m_header;
	std::string m_file;
	BinXMLWriter* m_writer;
	string_dictionary m_dict;

	char* m_dictbuf;
protected:
	void convert_attribs(BiXmlElementImpl* pElement, unsigned int indent);
	void convert_to_binary( BiXmlNodeImpl* pParent, int indent);
	void save_to_file(const char* file_name);
public:
	BinXMLConvert(const char* file);
	~BinXMLConvert();
	void convert_to_binary(const char* file_name);
	const char* GetBinBuffer()
	{
		return m_writer->buffer();
	}
	int GetLength()
	{
		return m_writer->position();
	}
	const char* GetDictBuffer()
	{
		return m_dictbuf;
	}
	int GetDictBuffLength()
	{
		return BinXMLReader::DICT_BUFLEN;
	}
	const string_dictionary& GetDictionary()
	{
		return m_dict;
	}
};

#endif
