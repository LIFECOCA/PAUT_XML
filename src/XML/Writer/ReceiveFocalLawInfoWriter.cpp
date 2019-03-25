#include "ReceiveFocalLawInfoWriter.h"

#import "msxml6.dll"

#include <string>

#pragma warning(disable: 4996)

struct CReceiveFocalLawInfoWriter::Impl
{
	//构造函数
	Impl(const std::string& name);

	//析构函数
	~Impl();

	//设置某一个shot所生成的A扫信号个数
	void SetSignalNum(const int SignalNum, const int& ShotID);

	//设置某一个shot的某个signal所使用的阵元个数
	void SetElementNum(const int& ElementNum, const int& ShotID, const int SignalID);

	//设置某一个shot的某一个Signal的某一个阵元在板卡上对应的阵元ID号
	void SetElementID(const int& element, const int& ShotID, const int& SignalID, const int& ElementIndex);

	//设置某一个shot所拥有的聚焦法则个数
	void SetFocalLawNum(const int& LawNum, const int& ShotID, const int& SignalID);

	//设置某一个Law起作用的时间长度
	void SetTotalFocalTime(const double& time, const int& ShotID, const int& SignalID, const int& LawID);

	//设置某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
	void SetElementDelay(const double& delay, const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex);

	//XML文档节点
	MSXML2::IXMLDOMDocumentPtr pDoc;

	//XML根节点
	MSXML2::IXMLDOMElementPtr pRoot;

	std::string m_FilePathName;
};

//构造函数
CReceiveFocalLawInfoWriter::CReceiveFocalLawInfoWriter(const char* name)
{
	m_pImpl = new Impl(name);
}

//析构函数
CReceiveFocalLawInfoWriter::~CReceiveFocalLawInfoWriter()
{
	delete m_pImpl;
}

//设置某一个shot所生成的A扫信号个数
void CReceiveFocalLawInfoWriter::SetSignalNum(const int SignalNum, const int& ShotID)
{
	m_pImpl->SetSignalNum(SignalNum, ShotID);
}

//设置某一个shot的某个signal所使用的阵元个数
void CReceiveFocalLawInfoWriter::SetElementNum(const int& ElementNum, const int& ShotID, const int SignalID)
{
	m_pImpl->SetElementNum(ElementNum, ShotID, SignalID);
}

//设置某一个shot的某一个Signal的某一个阵元在板卡上对应的阵元ID号
void CReceiveFocalLawInfoWriter::SetElementID(const int& element, const int& ShotID, const int& SignalID, const int& ElementIndex)
{
	m_pImpl->SetElementID(element, ShotID, SignalID, ElementIndex);
}

//设置某一个shot所拥有的聚焦法则个数
void CReceiveFocalLawInfoWriter::SetFocalLawNum(const int& LawNum, const int& ShotID, const int& SignalID)
{
	m_pImpl->SetFocalLawNum(LawNum, ShotID, SignalID);
}

//设置某一个Law起作用的时间长度
void CReceiveFocalLawInfoWriter::SetTotalFocalTime(const double& time, const int& ShotID, const int& SignalID, const int& LawID)
{
	m_pImpl->SetTotalFocalTime(time, ShotID, SignalID, LawID);
}

//设置某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
void CReceiveFocalLawInfoWriter::SetElementDelay(const double& delay, const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex)
{
	m_pImpl->SetElementDelay(delay, ShotID, SignalID, LawID, ElementIndex);
}

//构造函数
CReceiveFocalLawInfoWriter::Impl::Impl(const std::string& name)
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
CReceiveFocalLawInfoWriter::Impl::~Impl()
{
	pDoc->save(m_FilePathName.c_str());
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//设置某一个shot所生成的A扫信号个数
void CReceiveFocalLawInfoWriter::Impl::SetSignalNum(const int SignalNum, const int& ShotID)
{
	char str[100];
	MSXML2::IXMLDOMNodePtr pSignalNum;
	pSignalNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"SignalNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", SignalNum);
	pSignalNum->Puttext((_bstr_t)str);

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pShot = NULL;

	sprintf(str, "Shot%d", ShotID);
	pShot = pRoot->selectSingleNode((_bstr_t)str);

	pShot->appendChild(pSignalNum);

	//创建相应于ShotNum个的Shot子节点
	MSXML2::IXMLDOMNodePtr pSignal;
	for (int i = 0; i < SignalNum; i++)
	{
		sprintf(str, "Signal%d", i + 1);
		pSignal = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)"");
		pShot->appendChild(pSignal);
	}
}

//设置某一个shot所使用的阵元个数
void CReceiveFocalLawInfoWriter::Impl::SetElementNum(const int& ElementNum, const int& ShotID, const int SignalID)
{
	char str[100];

	MSXML2::IXMLDOMNodePtr pElementNum;
	pElementNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ElementNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", ElementNum);
	pElementNum->Puttext((_bstr_t)str);

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	sprintf(str, "Shot%d", ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);

	//获取节点<SignalID></SignalID>
	sprintf(str, "Signal%d", SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	pNode->appendChild(pElementNum);
	//创建相应于ElementNum个的element子节点
	MSXML2::IXMLDOMNodePtr pElement;
	for (int i = 0; i < ElementNum; i++)
	{
		sprintf(str, "Element%d", i + 1);
		pElement = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)"");
		pNode->appendChild(pElement);
	}
}

//设置某一个shot的某一个聚焦法则的某一个阵元在板卡上对应的阵元ID号
void CReceiveFocalLawInfoWriter::Impl::SetElementID(const int& element, const int& ShotID, const int& SignalID, const int& ElementIndex)
{	
	char str[100];

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	sprintf(str, "Shot%d", ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);

	//获取节点<SignalID></SignalID>
	sprintf(str, "Signal%d", SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	//获取节点<ElementIndex></ElementIndex>
	sprintf(str, "Element%d", ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	sprintf(str, "%d", element);
	pNode->Puttext((_bstr_t)str);
}

//设置某一个shot所拥有的聚焦法则个数
void CReceiveFocalLawInfoWriter::Impl::SetFocalLawNum(const int& LawNum, const int& ShotID, const int& SignalID)
{
	char str[100];

	MSXML2::IXMLDOMNodePtr pLawNum;
	pLawNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"LawNum", (_bstr_t)(char*)"");

	sprintf(str, "%d", LawNum);
	pLawNum->Puttext((_bstr_t)str);

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	sprintf(str, "Shot%d", ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);

	//获取节点<SignalID></SignalID>
	sprintf(str, "Signal%d", SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	pNode->appendChild(pLawNum);
	
	//获取ElementNum
	int ElementNum = atoi((char*)_bstr_t(pNode->selectSingleNode((_bstr_t)(char*)"ElementNum")->GetfirstChild()->GetnodeValue()));

	//创建相应于LawNum个的Law子节点
	MSXML2::IXMLDOMNodePtr pLaw;
	MSXML2::IXMLDOMNodePtr pDelay;
	for (int i = 0; i < LawNum; i++)
	{
		sprintf(str, "Law%d", i + 1);
		pLaw = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)"");
		pNode->appendChild(pLaw);
		for (int j = 0; j < ElementNum; j++)
		{
			sprintf(str, "Delay%d", j + 1);
			pDelay = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)"");
			pLaw->appendChild(pDelay);
		}
	}
}

//设置某一个Law起作用的时间长度
void CReceiveFocalLawInfoWriter::Impl::SetTotalFocalTime(const double& time, const int& ShotID, const int& SignalID, const int& LawID)
{
	char str[100];

	MSXML2::IXMLDOMNodePtr pTime;
	pTime = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"TotalFocalTime", (_bstr_t)(char*)"");

	sprintf(str, "%lf", time);
	pTime->Puttext((_bstr_t)str);

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	sprintf(str, "Shot%d", ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);

	//获取节点<SignalID></SignalID>
	sprintf(str, "Signal%d", SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	//获取节点<LawID></LawID>
	sprintf(str, "Law%d", LawID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	pNode->appendChild(pTime);
}

//设置某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
void CReceiveFocalLawInfoWriter::Impl::SetElementDelay(const double& delay, const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex)
{
	char str[100];

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	sprintf(str, "Shot%d", ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);

	//获取节点<SignalID></SignalID>
	sprintf(str, "Signal%d", SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	//获取节点<LawID></LawID>
	sprintf(str, "Law%d", LawID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	//获取节点<DelayID></DelayID>
	sprintf(str, "Delay%d", ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	sprintf(str, "%lf", delay);
	pNode->Puttext((_bstr_t)str);
}