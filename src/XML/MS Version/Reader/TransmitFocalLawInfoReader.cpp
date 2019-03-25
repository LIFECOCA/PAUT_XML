#include "TransmitFocalLawInfoReader.h"

#include <QMessageBox>
#include <QString>

//构造函数
CTransmitFocalLawInfoReader::CTransmitFocalLawInfoReader(const std::string& name)
{
	CoInitialize(NULL);

	m_bReadEnable = false;

	//创建根节点
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (FAILED(hr))
	{
		QMessageBox* message = new QMessageBox(QMessageBox::Warning, "Failed", "Create Failed", QMessageBox::Ok);
		delete message;
		return;
	}

	//加载PoolInitInfo文档
	VARIANT_BOOL bXmlLoad = pDoc->load(name.c_str());
	if (!bXmlLoad) // 加载失败
	{
		QMessageBox* message = new QMessageBox(QMessageBox::Warning, "Failed", "Load Failed", QMessageBox::Ok);
		delete message;
		return;
	}

	//获得根节点
	pRoot = pDoc->GetdocumentElement();

	m_bReadEnable = true;
}

//析构函数
CTransmitFocalLawInfoReader::~CTransmitFocalLawInfoReader()
{
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//获得硬件基本信息
HardwareInfo CTransmitFocalLawInfoReader::GetHardwareInfo() const
{
	HardwareInfo hardware;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode;

		//获取节点<TransmitVoltage></TransmitVoltage>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"TransmitVoltage")->GetfirstChild();

		//发射电压
		hardware.m_TransmitVoltage = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<UTFrequency></UTFrequency>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"UTFrequency")->GetfirstChild();

		//发射电压
		hardware.m_UTFrequency = atoi((char*)_bstr_t(pNode->GetnodeValue()));

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
int CTransmitFocalLawInfoReader::GetShotNum() const
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
int CTransmitFocalLawInfoReader::GetElementNum(const int& ShotID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<ElementNum></ElementNum>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"ElementNum")->GetfirstChild();

		//返回ElementNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的A扫深度
double CTransmitFocalLawInfoReader::GetAScanDepth(const int& ShotID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<AScanDepth></AScanDepth>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"AScanDepth")->GetfirstChild();

		//返回AScanDepth
		return atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个发射阵元在板卡上对应的阵元ID号
int CTransmitFocalLawInfoReader::GetTransmitElementID(const int& ShotID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<ElementIndex><ElementIndex>
		str = QString("Element%0").arg(ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<ID></ID>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"TransmitID")->GetfirstChild();

		//返回ID
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个阵元的发射延迟时间
double CTransmitFocalLawInfoReader::GetTransmitDelay(const int& ShotID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<ElementIndex><ElementIndex>
		str = QString("Element%0").arg(ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<TransmitDelay></TransmitDelay>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"TransmitDelay")->GetfirstChild();

		//返回TransmitDelay
		return atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某个shot的某个element的信息
ElementInfo CTransmitFocalLawInfoReader::GetElementInfo(const int& ShotID, const int& ElementIndex) const
{
	ElementInfo element;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<ElementIndex></ElementIndex>
		str = QString("Element%0").arg(ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

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
