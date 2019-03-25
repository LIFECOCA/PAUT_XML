#include "TransmitFocalLawInfoReader.h"

#import "msxml6.dll"

#include <string>

#pragma warning(disable: 4996)

struct CTransmitFocalLawInfoReader::Impl
{
	//构造函数
	Impl(const std::string& name);

	//析构函数
	~Impl();

	//获得硬件基本信息
	HardwareInfo GetHardwareInfo() const;

	//获得总Shot数
	int GetShotNum() const;

	//获得某一个shot所使用的阵元个数
	int GetElementNum(const int& ShotID) const;

	//获得某一个shot的A扫深度
	double GetAScanDepth(const int& ShotID) const;

	//获得某一个shot的某一个发射阵元在板卡上对应的阵元ID号
	int GetTransmitElementID(const int& ShotID, const int& ElementIndex) const;

	//获得某一个shot的某一个阵元的发射延迟时间
	double GetTransmitDelay(const int& ShotID, const int& ElementIndex) const;

	//获得某个shot的某个element的信息
	ElementInfo GetElementInfo(const int& ShotID, const int& ElementIndex) const;

	//XML文档节点
	MSXML2::IXMLDOMDocumentPtr pDoc;

	//XML根节点
	MSXML2::IXMLDOMElementPtr pRoot;

	//标志是否读取成功文档的变量
	bool m_bReadEnable;
};

//构造函数
CTransmitFocalLawInfoReader::CTransmitFocalLawInfoReader(const char* name)
{
	m_pImpl = new Impl(name);
}

//析构函数
CTransmitFocalLawInfoReader::~CTransmitFocalLawInfoReader()
{
	delete m_pImpl;
}

//获得硬件基本信息
HardwareInfo CTransmitFocalLawInfoReader::GetHardwareInfo() const
{
	return m_pImpl->GetHardwareInfo();
}

//获得总Shot数
int CTransmitFocalLawInfoReader::GetShotNum() const
{
	return m_pImpl->GetShotNum();
}

//获得某一个shot所使用的阵元个数
int CTransmitFocalLawInfoReader::GetElementNum(const int& ShotID) const
{
	return m_pImpl->GetElementNum(ShotID);
}

//获得某一个shot的A扫深度
double CTransmitFocalLawInfoReader::GetAScanDepth(const int& ShotID) const
{
	return m_pImpl->GetAScanDepth(ShotID);
}

//获得某一个shot的某一个发射阵元在板卡上对应的阵元ID号
int CTransmitFocalLawInfoReader::GetTransmitElementID(const int& ShotID, const int& ElementIndex) const
{
	return m_pImpl->GetTransmitElementID(ShotID, ElementIndex);
}

//获得某一个shot的某一个阵元的发射延迟时间
double CTransmitFocalLawInfoReader::GetTransmitDelay(const int& ShotID, const int& ElementIndex) const
{
	return m_pImpl->GetTransmitDelay(ShotID, ElementIndex);
}

//获得某个shot的某个element的信息
ElementInfo CTransmitFocalLawInfoReader::GetElementInfo(const int& ShotID, const int& ElementIndex) const
{
	return m_pImpl->GetElementInfo(ShotID, ElementIndex);
}

//构造函数
CTransmitFocalLawInfoReader::Impl::Impl(const std::string& name)
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
CTransmitFocalLawInfoReader::Impl::~Impl()
{
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//获得硬件基本信息
HardwareInfo CTransmitFocalLawInfoReader::Impl::GetHardwareInfo() const
{
	HardwareInfo hardware;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode;

		//获取节点<TransmitVoltage></TransmitVoltage>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"TransmitVoltage")->GetfirstChild();

		//发射电压
		hardware.m_TransmitVoltage = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<UTFrequency></UTFrequency>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"UTFrequency")->GetfirstChild();

		//发射电压
		hardware.m_UTFrequency = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<SamplingDivisor></SamplingDivisor>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"SamplingDivisor")->GetfirstChild();

		//分频数
		hardware.m_SamplingDivisor = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		//默认扫查频率为200
		hardware.m_ScanningFrequency = 200;
	}
	return hardware;
}

//获得总Shot数
int CTransmitFocalLawInfoReader::Impl::GetShotNum() const
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

//获得某一个shot所使用的阵元个数
int CTransmitFocalLawInfoReader::Impl::GetElementNum(const int& ShotID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<ElementNum></ElementNum>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"ElementNum")->GetfirstChild();

		//返回ElementNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的A扫深度
double CTransmitFocalLawInfoReader::Impl::GetAScanDepth(const int& ShotID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<AScanDepth></AScanDepth>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"AScanDepth")->GetfirstChild();

		//返回AScanDepth
		return atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个发射阵元在板卡上对应的阵元ID号
int CTransmitFocalLawInfoReader::Impl::GetTransmitElementID(const int& ShotID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<ElementIndex><ElementIndex>
		sprintf(str, "Element%d", ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<ID></ID>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"TransmitID")->GetfirstChild();

		//返回ID
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个阵元的发射延迟时间
double CTransmitFocalLawInfoReader::Impl::GetTransmitDelay(const int& ShotID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<ElementIndex><ElementIndex>
		sprintf(str, "Element%d", ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<TransmitDelay></TransmitDelay>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"TransmitDelay")->GetfirstChild();

		//返回TransmitDelay
		return atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某个shot的某个element的信息
ElementInfo CTransmitFocalLawInfoReader::Impl::GetElementInfo(const int& ShotID, const int& ElementIndex) const
{
	ElementInfo element;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;

		char str[100];

		//获取节点<ShotID></ShotID>
		sprintf(str, "Shot%d", ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<ElementIndex><ElementIndex>
		sprintf(str, "Element%d", ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<PulseWidth></PulseWidth>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"PulseWidth")->GetfirstChild();

		//PulseWidth
		element.m_PulseWidth = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<Gain></Gain>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"Gain")->GetfirstChild();

		//Gain
		element.m_Gain = atof((char*)_bstr_t(pNode1->GetnodeValue()));
	}
	return element;
}
