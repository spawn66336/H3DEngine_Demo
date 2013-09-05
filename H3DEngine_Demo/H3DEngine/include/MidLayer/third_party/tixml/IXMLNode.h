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
	*		清空node及其子节点
	*/
	virtual void Clear()=0;
	/**
	* @brief
	*		复制节点及其子元素
	*/
	virtual BiXmlNode* Clone() const = 0;

	/**
	* @brief
	*		返回第一个子节点
	*/
	virtual const BiXmlNode* FirstChild()const = 0;
	virtual BiXmlNode* FirstChild() = 0;

	/**
	* @brief
	*		返回名字为value的第一个子节点
	* @param
	*		value: 节点名称
	*/
	virtual const BiXmlNode* FirstChild( const char * value ) const = 0;
	virtual BiXmlNode* FirstChild( const char * value ) = 0;

	/**
	* @brief
	*		获得节点的第一个子元素
	*/
	virtual const BiXmlElement* FirstChildElement()	const = 0;
	virtual BiXmlElement* FirstChildElement() = 0;
	/**
	* @brief
	*		获得节点名称为value的第一个子元素
	* @value
	*		元素名称
	* @return
	*		成功返回第一个元素，否则返回null
	*/
	virtual const BiXmlElement* FirstChildElement( const char * value ) const = 0;
	virtual BiXmlElement* FirstChildElement( const char * value ) = 0;

	/**
	* @brief
	*		返回名字为_value的下一个兄弟节点
	* @return
	*		成功返回查找到的节点，失败返回null。
	*/
	virtual const BiXmlNode* NextSibling() const = 0;
	virtual BiXmlNode* NextSibling() = 0;

	/**
	* @brief
	*		返回名字为_value的下一个兄弟节点
	* @param
	*		value: 节点名称
	* @return
	*		成功返回查找到的节点，失败返回null。
	*/
	virtual const BiXmlNode* NextSibling( const char * _value) const = 0;
	virtual BiXmlNode* NextSibling( const char * _value) = 0;

	/**
	* @brief
	*		获得下一个兄弟元素
	*/
	virtual const BiXmlElement* NextSiblingElement() const = 0;
	virtual BiXmlElement* NextSiblingElement() = 0;

	/**
	* @brief
	*		获得节点名称为value的下一个兄弟元素
	* @value
	*		元素名称
	* @return
	*		成功返回查找到的元素，否则返回null
	*/
	virtual const BiXmlElement* NextSiblingElement( const char * _value) const = 0;
	virtual BiXmlElement* NextSiblingElement( const char * _value) = 0;

	/**
	* @brief
	*
	* @param
	*		value:节点名称
	*		previous:
	* @return
	*		成功返回节点，否则返回null
	*/

	virtual const BiXmlNode* IterateChildren( const char * value, const BiXmlNode* previous ) const = 0;
	virtual BiXmlNode* IterateChildren( const char * value, BiXmlNode* previous ) = 0;
	/**
	* @brief
	*		获得父节点
	*/
	virtual const BiXmlNode* Parent() const = 0;

	/**
	* @brief
	*		获得所属文档。
	*/
	virtual const BiXmlDocument* GetDocument() const = 0;
	virtual BiXmlDocument* GetDocument() = 0;
	/**
	* @brief
	*		删除子节点
	* @param
	*		removeThis:要删除的节点
	* @return
	*		删除成功返回true，否则返回false;
	*/
	virtual bool RemoveChild( BiXmlNode* removeThis ) = 0;

	/**
	* @brief
	*		插入名称为name的子元素。
	* @param
	*		name:节点元素的标签名
	* @return
	*		返回插入的节点
	*/
	virtual BiXmlNode* InsertEndChild( const BiXmlNode& addThis ) = 0;
	virtual BiXmlNode* InsertEndChild( const char* name ) = 0;

	/**
	* @brief
	*		将addthis挂接到该节点子元素列表末端
	* @param
	*		addThis:待挂接的元素
	* @return
	*		返回挂接成功的元素
	*/
	virtual BiXmlNode* LinkEndChild( BiXmlNode* addThis ) = 0;

	/**
	* @brief
	*		设置节点标签
	* @param
	*		_value:标签的名称
	*/
	virtual void SetValue(const char * _value)  = 0;
	/**
	* @brief
	*		获得节点标签名称
	*/
	virtual const char * Value() const = 0;

	/**
	* @brief
	*		获得节点类型
	* @return
	*		DT_XML：文本xml；DT_BIN:二进制XML。
	*/
	virtual DocType GetXmlType() = 0;

	/**
	* @brief
	*		将该node转换为element。
	* @return
	*		成功返回element,失败返回null
	*/
	virtual const BiXmlElement*     ToElement()const = 0;
	virtual BiXmlElement*           ToElement() = 0;

	
	/**
	* @brief
	*		将该node转换为text。
	* @return
	*		成功返回element,失败返回null
	*/
	virtual const BiXmlText*        ToText()const = 0;
	virtual BiXmlText*        ToText() = 0;
};

class BiXmlElement: public BiXmlNode
{
public:
	/**
	* @brief
	*		获得元素的某个属性值。
	* @param
	*		name: 属性名称
	* @return
	*		属性值
	*/
	virtual const char* Attribute(const char* name)const = 0;

	/**
	* @brief
	*		获得元素的某个属性的方便用法
	*/
	virtual const char* Attribute(const char* name, int* i)const = 0;
	virtual const char* Attribute(const char* name, double* d)const = 0;
	
	/**
	* @brief
	*		获得元素的文本
	* @return
	*		元素包含的文本值
	*/
	virtual const char* GetText() const = 0;
	/**
	* @brief
	*		设置元素的文本值。
	* @param
	*		strText:要设置的文本值
	* @return
	*		文本设置是否成功
	*/
	virtual bool SetText(const char* strText) = 0;
	/**
	* @brief
	*		给元素添加一个文本值。
	* @param
	*		strText:要添加的文本值
	* @return
	*		文本设置是否成功
	*/
	virtual bool AddText(const char* strText) = 0;

	/**
	* @brief
	*		设置元素属性
	* @param
	*		name:属性名称
	*		_value:属性值
	*/
	virtual void SetAttribute( const char* name, const char * _value ) = 0;
	/**
	* @brief
	*		设置元素属性的方便用法
	*/
	virtual void SetAttribute( const char * name, int value ) = 0;
	virtual void SetDoubleAttribute( const char * name, double value ) = 0;
	/**
	* @brief
	*		获取元素整型属性
	* @param
	*		name:属性名称
	*		_value:属性值
	*/
	virtual int QueryIntAttribute( const char* name, int* _value ) const = 0;
	/**
	* @brief
	*		获取元素浮点属性
	* @param
	*		name:属性名称
	*		_value:属性值
	*/
	virtual int QueryFloatAttribute( const char* name, float* _value ) const = 0;

	friend std::string& operator<< (std::string& out, const BiXmlElement* base );

};

class BiXmlText: public BiXmlNode
{
public:
	/**
	* @brief
	*		返回该文本节点是否为CDATA字段
	*/
	virtual bool CDATA() = 0;
	/**
	* @brief
	*		将该文本节点设置为CDATA字段
	* @param
	*		_cdata:是否为CDATA
	*/
	virtual void SetCDATA( bool _cdata ) = 0;
};

class BiXmlDocument: public BiXmlNode
{
public:
	/**
	* @brief
	*		载入文档
	* @param
	*		filename: 文件名称
	*		encoding: 编码方式，仅对文本xml有作用。
	*/
	virtual bool LoadFile( const char* filename, BiXmlEncoding encoding = BIXML_ENCODING_UNKNOWN ) = 0;
	virtual bool LoadFile( FILE* file, BiXmlEncoding encoding = BIXML_ENCODING_UNKNOWN) = 0;
	/**
	* @brief
	*		保存文档
	* @param
	*		filename:文档名称
	*/
	virtual bool SaveFile( const char * filename) = 0;
	/**
	* @brief
	*		获得错误描述，目前仅支持文本xml，二进制返回null
	*/
	virtual const char * ErrorDesc() const = 0;
	/**
	* @brief
	*		插入xml文档头，目前仅支持文本xml，二进制不进行任何操作
	* @param
	*		_version:xml版本信息
	*		_encoding:xml文档编码信息
	*		_standalone:值为"yes"或者"no"
	* @return
	*		插入是否成功。
	*/
	virtual bool InsertDeclaration(const char* _version, const char* _encoding, const char* _standalone) = 0;
	/**
	* @brief
	*		获得文档的根元素，用于访问整个文档使用。
	*/
	virtual const BiXmlElement* RootElement() const = 0;
	virtual BiXmlElement* RootElement() = 0;
	/**
	* @brief
	*		解析一段xml格式的文本,目前仅支持文本xml格式
	* @param
	*		strContent:待解析的字符串
	*/
	virtual const char* ParseXmlStr(const char* strContent) = 0;
	virtual const char* Parse(const char* strContent) = 0;
	/**
	* @brief
	*		载入xml过程是否出错，目前仅支持文本xml
	* @return
	*		出错返回true,否则返回false;
	*/
	virtual bool Error()const = 0;
};

class XmlDocFactory
{
public:
	/*
	* @brief
	*		创建文档对象
	* @param
	*		dt:创建类型。DT_XML（文本xml）、DT_BIN(二进制xml)
	* @return
	*		创建成功返回创建的文档，否则返回null
	*/
	static BiXmlDocument* CreateDocument(DocType dt = DT_XML);
	/**
	* @brief
	*		释放创建的文档
	*/
	static bool ReleaseDocument(BiXmlDocument* doc);
	/**
	* @brief
	*		创建元素
	* @param
	*		strName:元素名称
	*		dt:元素类型.DT_XML（文本xml）、DT_BIN(二进制xml)
	*/
	static BiXmlElement* CreateElement(const char* strName, DocType dt = DT_XML);/**
	* @brief
	*		释放创建的元素
	*/
	static bool ReleaseElement(BiXmlElement* elem);
};

#endif

