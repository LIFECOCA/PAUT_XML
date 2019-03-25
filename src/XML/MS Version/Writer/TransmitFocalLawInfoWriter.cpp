#include "TransmitFocalLawInfoWriter.h"

#include <QMessageBox>
#include <QString>

//构造函数
CTransmitFocalLawInfoWriter::CTransmitFocalLawInfoWriter(const std::string& name)
{
	CoInitialize(NULL);

	m_FilePathName = name;

	//创建根节点
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (FAILED(hr))
	{
		QMessageBox* message = new QMessageBox(QMessageBox::Warning, "Failed", "Create Failed", QMessageBox::Ok);
		delete message;
		return;
	}

	//加载PoolInitInfo文档
	VARIANT_BOOL bXmlLoad = pDoc->load(m_FilePathName.c_str());
	if (!bXmlLoad) // 加载失败
	{
		QMessageBox* message = new QMessageBox(QMessageBox::Warning, "Failed", "Load Failed", QMessageBox::Ok);
		delete message;
		return;
	}

	//获得根节点
	pRoot = pDoc->GetdocumentElement();
}

//析构函数
CTransmitFocalLawInfoWriter::~CTransmitFocalLawInfoWriter()
{
	pDoc->save(m_FilePathName.c_str());
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//设置硬件信息
void CTransmitFocalLawInfoWriter::SetHardwareInfo(const HardwareInfo& hardware)
{
	QString str;

	MSXML2::IXMLDOMNodePtr pTransmitVoltage;
	pTransmitVoltage = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"TransmitVoltage", (_bstr_t)(char*)"");
	str = QString("%0").arg(hardware.m_TransmitVoltage);
	
	pTransmitVoltage->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pTransmitVoltage);

	MSXML2::IXMLDOMNodePtr pUTFrequency;
	pUTFrequency = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"UTFrequency", (_bstr_t)(char*)"");
	str = QString("%0").arg(hardware.m_UTFrequency);
	pUTFrequency->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pUTFrequency);

	MSXML2::IXMLDOMNodePtr pSamplingDivisor;
	pSamplingDivisor = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"SamplingDivisor", (_bstr_t)(char*)"");
	str = QString("%0").arg(hardware.m_SamplingDivisor);
	pSamplingDivisor->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pSamplingDivisor);
}

//设置总Shot数
void CTransmitFocalLawInfoWriter::SetShotNum(const int& ShotNum)
{
	QString str;

	MSXML2::IXMLDOMNodePtr pShotNum;
	pShotNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ShotNum", (_bstr_t)(char*)"");
	str = QString("%0").arg(ShotNum);
	pShotNum->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pShotNum);

	//创建相应于ShotNum个的Shot子节点
	MSXML2::IXMLDOMNodePtr pShot;
	for (int i = 0; i < ShotNum; i++)
	{
		str = QString("Shot%0").arg(i + 1);
		pShot = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str.toLatin1().constData(), (_bstr_t)(char*)"");
		pRoot->appendChild(pShot);
	}
}

//获得某一个shot的A扫深度
void CTransmitFocalLawInfoWriter::SetAScanDepth(const double& depth, const int& ShotID)
{
	QString str;

	MSXML2::IXMLDOMNodePtr pAScanDepth;
	pAScanDepth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"AScanDepth", (_bstr_t)(char*)"");
	str = QString("%0").arg(depth);
	pAScanDepth->Puttext((_bstr_t)str.toLatin1().constData());

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pShot = NULL;

	str = QString("Shot%0").arg(ShotID);
	pShot = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	pShot->appendChild(pAScanDepth);
}

//设置某一个shot所使用的阵元个数
void CTransmitFocalLawInfoWriter::SetElementNum(const int& ElementNum, const int& ShotID)
{
	QString str;

	MSXML2::IXMLDOMNodePtr pElementNum;
	pElementNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ElementNum", (_bstr_t)(char*)"");
	str = QString("%0").arg(ElementNum);
	pElementNum->Puttext((_bstr_t)str.toLatin1().constData());

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pShot = NULL;

	str = QString("Shot%0").arg(ShotID);
	pShot = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	pShot->appendChild(pElementNum);

	//创建相应于ElementNum个的Element子节点
	MSXML2::IXMLDOMNodePtr pElement;
	for (int i = 0; i < ElementNum; i++)
	{
		str = QString("Element%0").arg(i + 1);
		pElement = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str.toLatin1().constData(), (_bstr_t)(char*)"");
		pShot->appendChild(pElement);
	}
}

//设置某一个shot的某一个发射阵元在板卡上对应的阵元ID号
void CTransmitFocalLawInfoWriter::SetTransmitElementID(const int& ElementID, const int& ShotID, const int& ElementIndex)
{
	QString str;

	MSXML2::IXMLDOMNodePtr pElementID;
	pElementID = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"TransmitID", (_bstr_t)(char*)"");
	str = QString("%0").arg(ElementID);
	pElementID->Puttext((_bstr_t)str.toLatin1().constData());

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	str = QString("Shot%0").arg(ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<ElementIndex></ElementIndex>
	str = QString("Element%0").arg(ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	pNode->appendChild(pElementID);
}

//设置某一个shot的某一个阵元的发射延迟时间
void CTransmitFocalLawInfoWriter::SetTransmitDelay(const double& delay, const int& ShotID, const int& ElementIndex)
{
	QString str;

	MSXML2::IXMLDOMNodePtr pTransmitDelay;
	pTransmitDelay = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"TransmitDelay", (_bstr_t)(char*)"");
	str = QString("%0").arg(delay);
	pTransmitDelay->Puttext((_bstr_t)str.toLatin1().constData());

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	str = QString("Shot%0").arg(ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<ElementIndex></ElementIndex>
	str = QString("Element%0").arg(ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	pNode->appendChild(pTransmitDelay);
}

//获得某个shot的某个element的信息
void CTransmitFocalLawInfoWriter::SetElementInfo(const ElementInfo& element, const int& ShotID, const int& ElementIndex)
{
	QString str;

	//创建节点<PulseEnable></PulseEnable>
	MSXML2::IXMLDOMNodePtr pPulseWidth;
	pPulseWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"PulseWidth", (_bstr_t)(char*)"");
	str = QString("%0").arg(element.m_PulseWidth);
	pPulseWidth->Puttext((_bstr_t)str.toLatin1().constData());

	//创建节点<PulseEnable></PulseEnable>
	MSXML2::IXMLDOMNodePtr pGain;
	pGain = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Gain", (_bstr_t)(char*)"");
	str = QString("%0").arg(element.m_Gain);
	pGain->Puttext((_bstr_t)str.toLatin1().constData());

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	str = QString("Shot%0").arg(ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());
	//获取节点<ElementIndex></ElementIndex>
	str = QString("Element%0").arg(ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	pNode->appendChild(pPulseWidth);
	pNode->appendChild(pGain);
}
