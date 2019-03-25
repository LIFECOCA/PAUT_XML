#include "ReceiveFocalLawInfoReader.h"

#import "msxml6.dll"

#include <string>

#pragma warning(disable: 4996)

struct CReceiveFocalLawInfoReader::Impl
{
	//构造函数
	Impl(const std::string& name);

	//析构函数
	~Impl();

	//获得总Shot数
	int GetShotNum() const;

	//获得某一个shot所生成的A扫信号个数
	int GetSignalNum(const int& ShotID) const;

	//获得某一个shot所使用的阵元个数
	int GetElementNum(const int& ShotID, const int& SignalID) const;

	//获得某一个shot的某一个Signal的某一个阵元在板卡上对应的阵元ID号
	int GetElementID(const int& ShotID, const int& SignalID, const int& ElementIndex) const;

	//获得某一个shot所拥有的聚焦法则个数
	int GetFocalLawNum(const int& ShotID, const int& SignalID) const;

	//获得某一个Law起作用的时间长度
	double GetTotalFocalTime(const int& ShotID, const int& SignalID, const int& LawID);

	//获得某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
	double GetElementDelay(const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex) const;

	//XML文档节点
	MSXML2::IXMLDOMDocumentPtr pDoc;

	//XML根节点
	MSXML2::IXMLDOMElementPtr pRoot;

	//标志是否读取成功文档的变量
	bool m_bReadEnable;
};

//构造函数
CReceiveFocalLawInfoReader::CReceiveFocalLawInfoReader(const char* name)
{
	m_pImpl = new Impl(name);
}

//析构函数
CReceiveFocalLawInfoReader::~CReceiveFocalLawInfoReader()
{
	delete m_pImpl;
}

//获得总Shot数
int CReceiveFocalLawInfoReader::GetShotNum() const
{
	return m_pImpl->GetShotNum();
}

//获得某一个shot所生成的A扫信号个数
int CReceiveFocalLawInfoReader::GetSignalNum(const int& ShotID) const
{
	return m_pImpl->GetSignalNum(ShotID);
}

//获得某一个shot所使用的阵元个数
int CReceiveFocalLawInfoReader::GetElementNum(const int& ShotID, const int& SignalID) const
{
	return m_pImpl->GetElementNum(ShotID, SignalID);
}

//获得某一个shot的某一个Signal的某一个阵元在板卡上对应的阵元ID号
int CReceiveFocalLawInfoReader::GetElementID(const int& ShotID, const int& SignalID, const int& ElementIndex) const
{
	return m_pImpl->GetElementID(ShotID, SignalID, ElementIndex);
}

//获得某一个shot所拥有的聚焦法则个数
int CReceiveFocalLawInfoReader::GetFocalLawNum(const int& ShotID, const int& SignalID) const
{
	return m_pImpl->GetFocalLawNum(ShotID, SignalID);
}

//获得某一个Law起作用的时间长度
double CReceiveFocalLawInfoReader::GetTotalFocalTime(const int& ShotID, const int& SignalID, const int& LawID)
{
	return m_pImpl->GetTotalFocalTime(ShotID, SignalID, LawID);
}

//获得某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
double CReceiveFocalLawInfoReader::GetElementDelay(const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex) const
{
	return m_pImpl->GetElementDelay(ShotID, SignalID, LawID, ElementIndex);
}

//构造函数
CReceiveFocalLawInfoReader::Impl::Impl(const std::string& name)
{
	CoInitialize(NULL);

	m_bReadEnable = false;

	//创建根节点
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (FAILED(hr))
	{
		printf("Create Failed\n");
		return;
	}

	//加载PoolInitInfo文档
	VARIANT_BOOL bXmlLoad = pDoc->load(name.c_str());
	if (!bXmlLoad) // 加载失败
	{
		printf("Load Failed\n");
		return;
	}

	//获得根节点
	pRoot = pDoc->GetdocumentElement();

	m_bReadEnable = true;
}

//析构函数
CReceiveFocalLawInfoReader::Impl::~Impl()
{
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//获得总Shot数
int CReceiveFocalLawInfoReader::Impl::GetShotNum() const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		//获取节点<ShotNum></ShotNum>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"ShotNum")->GetfirstChild();

		//返回ShotNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot所生成的A扫信号个数
int CReceiveFocalLawInfoReader::Impl::GetSignalNum(const int& ShotID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		//获取节点<ShotID></ShotID>
		char str[100];
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<SignalNum></SignalNum>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"SignalNum")->GetfirstChild();

		//返回SignalNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个Signal所使用的阵元个数
int CReceiveFocalLawInfoReader::Impl::GetElementNum(const int& ShotID, const int& SignalID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<SignalID></SignalID>
		sprintf(str, "Signal%d", SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<ElementNum></ElementNum>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"ElementNum")->GetfirstChild();

		//返回ElementNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个Signal的某一个阵元在板卡上对应的阵元ID号
int CReceiveFocalLawInfoReader::Impl::GetElementID(const int& ShotID, const int& SignalID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<SignalID></SignalID>
		sprintf(str, "Signal%d", SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<ElementIndex><ElementIndex>
		sprintf(str, "Element%d", ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str)->GetfirstChild();

		//返回ID
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot所拥有的聚焦法则个数
int CReceiveFocalLawInfoReader::Impl::GetFocalLawNum(const int& ShotID, const int& SignalID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<SignalID></SignalID>
		sprintf(str, "Signal%d", SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<LawNum></LawNum>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"LawNum")->GetfirstChild();

		//返回LawNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

double CReceiveFocalLawInfoReader::Impl::GetTotalFocalTime(const int& ShotID, const int& SignalID, const int& LawID)
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<SignalID></SignalID>
		sprintf(str, "Signal%d", SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<LawID></LawID>
		sprintf(str, "Law%d", LawID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<TotalFocalTime></TotalFocalTime>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"TotalFocalTime")->GetfirstChild();

		//返回ID
		return atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
double CReceiveFocalLawInfoReader::Impl::GetElementDelay(const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<SignalID></SignalID>
		sprintf(str, "Signal%d", SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<LawID></LawID>
		sprintf(str, "Law%d", LawID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<Delay##ElementIndex></Delay##ElementIndex>
		sprintf(str, "Delay%d", ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str)->GetfirstChild();

		//返回Delay
		return atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}