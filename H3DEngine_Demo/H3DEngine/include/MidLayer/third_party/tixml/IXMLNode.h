#ifndef __H_I_XML_NODE_H__
#define __H_I_XML_NODE_H__
#include <stdio.h>
#include <string>
// Used by the parsing routines.
enum BiXmlEncoding
{
	BIXML_ENCODING_UNKNOWN,
	BIXML_ENCODING_UTF8,
	BIXML_ENCODING_LEGACY
};
enum DocType
{
	DT_BIN = 0x1,
	DT_XML = 0x1<<1,
};
class BiXmlNode;
class BiXmlElement;
class BiXmlDocument;
class BiXmlText;

class BiXmlNode
{
public:

	/**
	* @brief
	*		���node�����ӽڵ�
	*/
	virtual void Clear()=0;
	/**
	* @brief
	*		���ƽڵ㼰����Ԫ��
	*/
	virtual BiXmlNode* Clone() const = 0;

	/**
	* @brief
	*		���ص�һ���ӽڵ�
	*/
	virtual const BiXmlNode* FirstChild()const = 0;
	virtual BiXmlNode* FirstChild() = 0;

	/**
	* @brief
	*		��������Ϊvalue�ĵ�һ���ӽڵ�
	* @param
	*		value: �ڵ�����
	*/
	virtual const BiXmlNode* FirstChild( const char * value ) const = 0;
	virtual BiXmlNode* FirstChild( const char * value ) = 0;

	/**
	* @brief
	*		��ýڵ�ĵ�һ����Ԫ��
	*/
	virtual const BiXmlElement* FirstChildElement()	const = 0;
	virtual BiXmlElement* FirstChildElement() = 0;
	/**
	* @brief
	*		��ýڵ�����Ϊvalue�ĵ�һ����Ԫ��
	* @value
	*		Ԫ������
	* @return
	*		�ɹ����ص�һ��Ԫ�أ����򷵻�null
	*/
	virtual const BiXmlElement* FirstChildElement( const char * value ) const = 0;
	virtual BiXmlElement* FirstChildElement( const char * value ) = 0;

	/**
	* @brief
	*		��������Ϊ_value����һ���ֵܽڵ�
	* @return
	*		�ɹ����ز��ҵ��Ľڵ㣬ʧ�ܷ���null��
	*/
	virtual const BiXmlNode* NextSibling() const = 0;
	virtual BiXmlNode* NextSibling() = 0;

	/**
	* @brief
	*		��������Ϊ_value����һ���ֵܽڵ�
	* @param
	*		value: �ڵ�����
	* @return
	*		�ɹ����ز��ҵ��Ľڵ㣬ʧ�ܷ���null��
	*/
	virtual const BiXmlNode* NextSibling( const char * _value) const = 0;
	virtual BiXmlNode* NextSibling( const char * _value) = 0;

	/**
	* @brief
	*		�����һ���ֵ�Ԫ��
	*/
	virtual const BiXmlElement* NextSiblingElement() const = 0;
	virtual BiXmlElement* NextSiblingElement() = 0;

	/**
	* @brief
	*		��ýڵ�����Ϊvalue����һ���ֵ�Ԫ��
	* @value
	*		Ԫ������
	* @return
	*		�ɹ����ز��ҵ���Ԫ�أ����򷵻�null
	*/
	virtual const BiXmlElement* NextSiblingElement( const char * _value) const = 0;
	virtual BiXmlElement* NextSiblingElement( const char * _value) = 0;

	/**
	* @brief
	*
	* @param
	*		value:�ڵ�����
	*		previous:
	* @return
	*		�ɹ����ؽڵ㣬���򷵻�null
	*/

	virtual const BiXmlNode* IterateChildren( const char * value, const BiXmlNode* previous ) const = 0;
	virtual BiXmlNode* IterateChildren( const char * value, BiXmlNode* previous ) = 0;
	/**
	* @brief
	*		��ø��ڵ�
	*/
	virtual const BiXmlNode* Parent() const = 0;

	/**
	* @brief
	*		��������ĵ���
	*/
	virtual const BiXmlDocument* GetDocument() const = 0;
	virtual BiXmlDocument* GetDocument() = 0;
	/**
	* @brief
	*		ɾ���ӽڵ�
	* @param
	*		removeThis:Ҫɾ���Ľڵ�
	* @return
	*		ɾ���ɹ�����true�����򷵻�false;
	*/
	virtual bool RemoveChild( BiXmlNode* removeThis ) = 0;

	/**
	* @brief
	*		��������Ϊname����Ԫ�ء�
	* @param
	*		name:�ڵ�Ԫ�صı�ǩ��
	* @return
	*		���ز���Ľڵ�
	*/
	virtual BiXmlNode* InsertEndChild( const BiXmlNode& addThis ) = 0;
	virtual BiXmlNode* InsertEndChild( const char* name ) = 0;

	/**
	* @brief
	*		��addthis�ҽӵ��ýڵ���Ԫ���б�ĩ��
	* @param
	*		addThis:���ҽӵ�Ԫ��
	* @return
	*		���عҽӳɹ���Ԫ��
	*/
	virtual BiXmlNode* LinkEndChild( BiXmlNode* addThis ) = 0;

	/**
	* @brief
	*		���ýڵ��ǩ
	* @param
	*		_value:��ǩ������
	*/
	virtual void SetValue(const char * _value)  = 0;
	/**
	* @brief
	*		��ýڵ��ǩ����
	*/
	virtual const char * Value() const = 0;

	/**
	* @brief
	*		��ýڵ�����
	* @return
	*		DT_XML���ı�xml��DT_BIN:������XML��
	*/
	virtual DocType GetXmlType() = 0;

	/**
	* @brief
	*		����nodeת��Ϊelement��
	* @return
	*		�ɹ�����element,ʧ�ܷ���null
	*/
	virtual const BiXmlElement*     ToElement()const = 0;
	virtual BiXmlElement*           ToElement() = 0;

	
	/**
	* @brief
	*		����nodeת��Ϊtext��
	* @return
	*		�ɹ�����element,ʧ�ܷ���null
	*/
	virtual const BiXmlText*        ToText()const = 0;
	virtual BiXmlText*        ToText() = 0;
};

class BiXmlElement: public BiXmlNode
{
public:
	/**
	* @brief
	*		���Ԫ�ص�ĳ������ֵ��
	* @param
	*		name: ��������
	* @return
	*		����ֵ
	*/
	virtual const char* Attribute(const char* name)const = 0;

	/**
	* @brief
	*		���Ԫ�ص�ĳ�����Եķ����÷�
	*/
	virtual const char* Attribute(const char* name, int* i)const = 0;
	virtual const char* Attribute(const char* name, double* d)const = 0;
	
	/**
	* @brief
	*		���Ԫ�ص��ı�
	* @return
	*		Ԫ�ذ������ı�ֵ
	*/
	virtual const char* GetText() const = 0;
	/**
	* @brief
	*		����Ԫ�ص��ı�ֵ��
	* @param
	*		strText:Ҫ���õ��ı�ֵ
	* @return
	*		�ı������Ƿ�ɹ�
	*/
	virtual bool SetText(const char* strText) = 0;
	/**
	* @brief
	*		��Ԫ�����һ���ı�ֵ��
	* @param
	*		strText:Ҫ��ӵ��ı�ֵ
	* @return
	*		�ı������Ƿ�ɹ�
	*/
	virtual bool AddText(const char* strText) = 0;

	/**
	* @brief
	*		����Ԫ������
	* @param
	*		name:��������
	*		_value:����ֵ
	*/
	virtual void SetAttribute( const char* name, const char * _value ) = 0;
	/**
	* @brief
	*		����Ԫ�����Եķ����÷�
	*/
	virtual void SetAttribute( const char * name, int value ) = 0;
	virtual void SetDoubleAttribute( const char * name, double value ) = 0;
	/**
	* @brief
	*		��ȡԪ����������
	* @param
	*		name:��������
	*		_value:����ֵ
	*/
	virtual int QueryIntAttribute( const char* name, int* _value ) const = 0;
	/**
	* @brief
	*		��ȡԪ�ظ�������
	* @param
	*		name:��������
	*		_value:����ֵ
	*/
	virtual int QueryFloatAttribute( const char* name, float* _value ) const = 0;

	friend std::string& operator<< (std::string& out, const BiXmlElement* base );

};

class BiXmlText: public BiXmlNode
{
public:
	/**
	* @brief
	*		���ظ��ı��ڵ��Ƿ�ΪCDATA�ֶ�
	*/
	virtual bool CDATA() = 0;
	/**
	* @brief
	*		�����ı��ڵ�����ΪCDATA�ֶ�
	* @param
	*		_cdata:�Ƿ�ΪCDATA
	*/
	virtual void SetCDATA( bool _cdata ) = 0;
};

class BiXmlDocument: public BiXmlNode
{
public:
	/**
	* @brief
	*		�����ĵ�
	* @param
	*		filename: �ļ�����
	*		encoding: ���뷽ʽ�������ı�xml�����á�
	*/
	virtual bool LoadFile( const char* filename, BiXmlEncoding encoding = BIXML_ENCODING_UNKNOWN ) = 0;
	virtual bool LoadFile( FILE* file, BiXmlEncoding encoding = BIXML_ENCODING_UNKNOWN) = 0;
	/**
	* @brief
	*		�����ĵ�
	* @param
	*		filename:�ĵ�����
	*/
	virtual bool SaveFile( const char * filename) = 0;
	/**
	* @brief
	*		��ô���������Ŀǰ��֧���ı�xml�������Ʒ���null
	*/
	virtual const char * ErrorDesc() const = 0;
	/**
	* @brief
	*		����xml�ĵ�ͷ��Ŀǰ��֧���ı�xml�������Ʋ������κβ���
	* @param
	*		_version:xml�汾��Ϣ
	*		_encoding:xml�ĵ�������Ϣ
	*		_standalone:ֵΪ"yes"����"no"
	* @return
	*		�����Ƿ�ɹ���
	*/
	virtual bool InsertDeclaration(const char* _version, const char* _encoding, const char* _standalone) = 0;
	/**
	* @brief
	*		����ĵ��ĸ�Ԫ�أ����ڷ��������ĵ�ʹ�á�
	*/
	virtual const BiXmlElement* RootElement() const = 0;
	virtual BiXmlElement* RootElement() = 0;
	/**
	* @brief
	*		����һ��xml��ʽ���ı�,Ŀǰ��֧���ı�xml��ʽ
	* @param
	*		strContent:���������ַ���
	*/
	virtual const char* ParseXmlStr(const char* strContent) = 0;
	virtual const char* Parse(const char* strContent) = 0;
	/**
	* @brief
	*		����xml�����Ƿ����Ŀǰ��֧���ı�xml
	* @return
	*		������true,���򷵻�false;
	*/
	virtual bool Error()const = 0;
};

class XmlDocFactory
{
public:
	/*
	* @brief
	*		�����ĵ�����
	* @param
	*		dt:�������͡�DT_XML���ı�xml����DT_BIN(������xml)
	* @return
	*		�����ɹ����ش������ĵ������򷵻�null
	*/
	static BiXmlDocument* CreateDocument(DocType dt = DT_XML);
	/**
	* @brief
	*		�ͷŴ������ĵ�
	*/
	static bool ReleaseDocument(BiXmlDocument* doc);
	/**
	* @brief
	*		����Ԫ��
	* @param
	*		strName:Ԫ������
	*		dt:Ԫ������.DT_XML���ı�xml����DT_BIN(������xml)
	*/
	static BiXmlElement* CreateElement(const char* strName, DocType dt = DT_XML);/**
	* @brief
	*		�ͷŴ�����Ԫ��
	*/
	static bool ReleaseElement(BiXmlElement* elem);
};

#endif

