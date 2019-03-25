#include "TransmitFocalLawInfoWriter.h"

#import "msxml6.dll"

#include <string>

#pragma warning(disable: 4996)

struct CTransmitFocalLawInfoWriter::Impl
{
	//构造函数
	Impl(const std::string& name);

	//析构函数
	~Impl();

	//设置硬件信息
	void SetHardwareInfo(const HardwareInfo& hardware);

	//设置总Shot数
	void SetShotNum(const int& ShotNum);

	//设置某一个shot所使用的阵元个数
	void SetElementNum(const int& ElementNum, const int& ShotID);

	//获得某一个shot的A扫深度
	void SetAScanDepth(const double& depth, const int& ShotID);

	//设置某一个shot的某一个发射阵元在板卡上对应的阵元ID号
	void SetTransmitElementID(const int& ElementID, const int& ShotID, const int& ElementIndex);

	//设置某一个shot的某一个阵元的接收延迟时间
	void SetTransmitDelay(const double& delay, const int& ShotID, const int& ElementIndex);

	//获得某个shot的某个element的信息
	void SetElementInfo(const ElementInfo& element, const int& ShotID, const int& ElementIndex);

	//XML文档节点
	MSXML2::IXMLDOMDocumentPtr pDoc;

	//XML根节点
	MSXML2::IXMLDOMElementPtr pRoot;

	std::string m_FilePathName;
};

//构造函数
CTransmitFocalLawInfoWriter::CTransmitFocalLawInfoWriter(const char* name)
{
	m_pImpl = new Impl(name);
}

//析构函数
CTransmitFocalLawInfoWriter::~CTransmitFocalLawInfoWriter()
{
	delete m_pImpl;
}

//设置硬件信息
void CTransmitFocalLawInfoWriter::SetHardwareInfo(const HardwareInfo& hardware)
{
	m_pImpl->SetHardwareInfo(hardware);
}

//设置总Shot数
void CTransmitFocalLawInfoWriter::SetShotNum(const int& ShotNum)
{
	m_pImpl->SetShotNum(ShotNum);
}

//设置某一个shot所使用的阵元个数
void CTransmitFocalLawInfoWriter::SetElementNum(const int& ElementNum, const int& ShotID)
{
	m_pImpl->SetElementNum(ElementNum, ShotID);
}

//获得某一个shot的A扫深度
void CTransmitFocalLawInfoWriter::SetAScanDepth(const double& depth, const int& ShotID)
{
	m_pImpl->SetAScanDepth(depth, ShotID);
}

//设置某一个shot的某一个发射阵元在板卡上对应的阵元ID号
void CTransmitFocalLawInfoWriter::SetTransmitElementID(const int& ElementID, const int& ShotID, const int& ElementIndex)
{
	m_pImpl->SetTransmitElementID(ElementID, ShotID, ElementIndex);
}

//设置某一个shot的某一个阵元的接收延迟时间
void CTransmitFocalLawInfoWriter::SetTransmitDelay(const double& delay, const int& ShotID, const int& ElementIndex)
{
	m_pImpl->SetTransmitDelay(delay, ShotID, ElementIndex);
}

//获得某个shot的某个element的信息
void CTransmitFocalLawInfoWriter::SetElementInfo(const ElementInfo& element, const int& ShotID, const int& ElementIndex)
{
	m_pImpl->SetElementInfo(element, ShotID, ElementIndex);
}

//构造函数
CTransmitFocalLawInfoWriter::Impl::Impl(const std::string& name)
{
	CoInitialize(NULL);

	m_FilePathName = name;

	//创建根节点
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (FAILED(hr))
	{
		printf("Create Failed\n");
		return;
	}

	//加载PoolInitInfo文档
	VARIANT_BOOL bXmlLoad = pDoc->load(m_FilePathName.c_str());
	if (!bXmlLoad) // 加载失败
	{
		printf("Load Failed\n");
		return;
	}

	//获得根节点
	pRoot = pDoc->GetdocumentElement();
}

//析构函数
CTransmitFocalLawInfoWriter::Impl::~Impl()
{
	pDoc->save(m_FilePathName.c_str());
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//设置硬件信息
void CTransmitFocalLawInfoWriter::Impl::SetHardwareInfo(const HardwareInfo& hardware)
{
	char str[100];

	MSXML2::IXMLDOMNodePtr pTransmitVoltage;
	pTransmitVoltage = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"TransmitVoltage", (_bstr_t)(char*)"");
	sprintf(str, "%lf", hardware.m_TransmitVoltage);
	pTransmitVoltage->Puttext((_bstr_t)str);
	pRoot->appendChild(pTransmitVoltage);

	MSXML2::IXMLDOMNodePtr pUTFrequency;
	pUTFrequency = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"UTFrequency", (_bstr_t)(char*)"");
	sprintf(str, "%lf", hardware.m_UTFrequency);
	pUTFrequency->Puttext((_bstr_t)str);
	pRoot->appendChild(pUTFrequency);

	MSXML2::IXMLDOMNodePtr pSamplingDivisor;
	pSamplingDivisor = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"SamplingDivisor", (_bstr_t)(char*)"");
	sprintf(str, "%d", hardware.m_SamplingDivisor);
	pSamplingDivisor->Puttext((_bstr_t)str);
	pRoot->appendChild(pSamplingDivisor);
}

//设置总Shot数
void CTransmitFocalLawInfoWriter::Impl::SetShotNum(const int& ShotNum)
{
	char str[100];

	MSXML2::IXMLDOMNodePtr pShotNum;
	pShotNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ShotNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", ShotNum);
	pShotNum->Puttext((_bstr_t)str);
	pRoot->appendChild(pShotNum);

	//创建相应于ShotNum个的Shot子节点
	MSXML2::IXMLDOMNodePtr pShot;
	for (int i = 0; i < ShotNum; i++)
	{
		sprintf(str, "Shot%d", i + 1);
		pShot = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)"");
		pRoot->appendChild(pShot);
	}
}

//获得某一个shot的A扫深度
void CTransmitFocalLawInfoWriter::Impl::SetAScanDepth(const double& depth, const int& ShotID)
{
	char str[100];

	MSXML2::IXMLDOMNodePtr pAScanDepth;
	pAScanDepth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"AScanDepth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", depth);
	pAScanDepth->Puttext((_bstr_t)str);

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pShot = NULL;
	sprintf(str, "Shot%d", ShotID);
	pShot = pRoot->selectSingleNode((_bstr_t)str);

	pShot->appendChild(pAScanDepth);
}

//设置某一个shot所使用的阵元个数
void CTransmitFocalLawInfoWriter::Impl::SetElementNum(const int& ElementNum, const int& ShotID)
{
	char str[100];

	MSXML2::IXMLDOMNodePtr pElementNum;
	pElementNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ElementNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", ElementNum);
	pElementNum->Puttext((_bstr_t)str);

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pShot = NULL;
	sprintf(str, "Shot%d", ShotID);
	pShot = pRoot->selectSingleNode((_bstr_t)str);

	pShot->appendChild(pElementNum);

	//创建相应于ElementNum个的Element子节点
	MSXML2::IXMLDOMNodePtr pElement;
	for (int i = 0; i < ElementNum; i++)
	{
		sprintf(str, "Element%d", i + 1);
		pElement = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)"");
		pShot->appendChild(pElement);
	}
}

//设置某一个shot的某一个发射阵元在板卡上对应的阵元ID号
void CTransmitFocalLawInfoWriter::Impl::SetTransmitElementID(const int& ElementID, const int& ShotID, const int& ElementIndex)
{
	char str[100];

	MSXML2::IXMLDOMNodePtr pElementID;
	pElementID = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"TransmitID", (_bstr_t)(char*)"");
	sprintf(str, "%d", ElementID);
	pElementID->Puttext((_bstr_t)str);

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	sprintf(str, "Shot%d", ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);

	//获取节点<ElementIndex></ElementIndex>
	sprintf(str, "Element%d", ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	pNode->appendChild(pElementID);
}

//设置某一个shot的某一个阵元的发射延迟时间
void CTransmitFocalLawInfoWriter::Impl::SetTransmitDelay(const double& delay, const int& ShotID, const int& ElementIndex)
{
	char str[100];

	MSXML2::IXMLDOMNodePtr pTransmitDelay;
	pTransmitDelay = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"TransmitDelay", (_bstr_t)(char*)"");
	sprintf(str, "%lf", delay);
	pTransmitDelay->Puttext((_bstr_t)str);

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	sprintf(str, "Shot%d", ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);

	//获取节点<ElementIndex></ElementIndex>
	sprintf(str, "Element%d", ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	pNode->appendChild(pTransmitDelay);
}

//获得某个shot的某个element的信息
void CTransmitFocalLawInfoWriter::Impl::SetElementInfo(const ElementInfo& element, const int& ShotID, const int& ElementIndex)
{
	char str[100];

	//创建节点<PulseEnable></PulseEnable>
	MSXML2::IXMLDOMNodePtr pPulseWidth;
	pPulseWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"PulseWidth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", element.m_PulseWidth);
	pPulseWidth->Puttext((_bstr_t)str);

	//创建节点<PulseEnable></PulseEnable>
	MSXML2::IXMLDOMNodePtr pGain;
	pGain = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Gain", (_bstr_t)(char*)"");
	sprintf(str, "%lf", element.m_Gain);
	pGain->Puttext((_bstr_t)str);

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	sprintf(str, "Shot%d", ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);
	//获取节点<ElementIndex></ElementIndex>
	sprintf(str, "Element%d", ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	pNode->appendChild(pPulseWidth);
	pNode->appendChild(pGain);
}
