#include "ReceiveFocalLawInfoWriter.h"

#include <QMessageBox>
#include <QString>

//构造函数
CReceiveFocalLawInfoWriter::CReceiveFocalLawInfoWriter(const std::string& name)
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
CReceiveFocalLawInfoWriter::~CReceiveFocalLawInfoWriter()
{
	pDoc->save(m_FilePathName.c_str());
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//设置某一个shot所生成的A扫信号个数
void CReceiveFocalLawInfoWriter::SetSignalNum(const int SignalNum, const int& ShotID)
{
	QString str;
	MSXML2::IXMLDOMNodePtr pSignalNum;
	pSignalNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"SignalNum", (_bstr_t)(char*)"");
	str = QString("%0").arg(SignalNum);
	pSignalNum->Puttext((_bstr_t)str.toLatin1().constData());

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pShot = NULL;

	str = QString("Shot%0").arg(ShotID);
	pShot = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	pShot->appendChild(pSignalNum);

	//创建相应于ShotNum个的Shot子节点
	MSXML2::IXMLDOMNodePtr pSignal;
	for (int i = 0; i < SignalNum; i++)
	{
		str = QString("Signal%0").arg(i + 1);
		pSignal = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str.toLatin1().constData(), (_bstr_t)(char*)"");
		pShot->appendChild(pSignal);
	}
}

//设置某一个shot所使用的阵元个数
void CReceiveFocalLawInfoWriter::SetElementNum(const int& ElementNum, const int& ShotID, const int SignalID)
{
	QString str;

	MSXML2::IXMLDOMNodePtr pElementNum;
	pElementNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ElementNum", (_bstr_t)(char*)"");
	str = QString("%0").arg(ElementNum);
	pElementNum->Puttext((_bstr_t)str.toLatin1().constData());

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	str = QString("Shot%0").arg(ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	pNode->appendChild(pElementNum);

	//创建相应于ElementNum个的element子节点
	MSXML2::IXMLDOMNodePtr pElement;
	for (int i = 0; i < ElementNum; i++)
	{
		str = QString("Element%0").arg(i + 1);
		pElement = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str.toLatin1().constData(), (_bstr_t)(char*)"");
		pNode->appendChild(pElement);
	}
}

//设置某一个shot的某一个聚焦法则的某一个阵元在板卡上对应的阵元ID号
void CReceiveFocalLawInfoWriter::SetElementID(const int& element, const int& ShotID, const int& SignalID, const int& ElementIndex)
{	
	QString str;

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	str = QString("Shot%0").arg(ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<ElementIndex></ElementIndex>
	str = QString("Element%0").arg(ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	str = QString("%0").arg(element);
	pNode->Puttext((_bstr_t)str.toLatin1().constData());
}

//设置某一个shot所拥有的聚焦法则个数
void CReceiveFocalLawInfoWriter::SetFocalLawNum(const int& LawNum, const int& ShotID, const int& SignalID)
{
	QString str;

	MSXML2::IXMLDOMNodePtr pLawNum;
	pLawNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"LawNum", (_bstr_t)(char*)"");

	str = QString("%0").arg(LawNum);
	pLawNum->Puttext((_bstr_t)str.toLatin1().constData());

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	str = QString("Shot%0").arg(ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	pNode->appendChild(pLawNum);
	
	//获取ElementNum
	int ElementNum = atoi((char*)_bstr_t(pNode->selectSingleNode((_bstr_t)(char*)"ElementNum")->GetfirstChild()->GetnodeValue()));

	//创建相应于LawNum个的Law子节点
	MSXML2::IXMLDOMNodePtr pLaw;
	MSXML2::IXMLDOMNodePtr pDelay;
	for (int i = 0; i < LawNum; i++)
	{
		str = QString("Law%0").arg(i + 1);
		pLaw = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str.toLatin1().constData(), (_bstr_t)(char*)"");
		pNode->appendChild(pLaw);
		for (int j = 0; j < ElementNum; j++)
		{
			str = QString("Delay%0").arg(j + 1);
			pDelay = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str.toLatin1().constData(), (_bstr_t)(char*)"");
			pLaw->appendChild(pDelay);
		}
	}
}

//设置某一个Law起作用的时间长度
void CReceiveFocalLawInfoWriter::SetTotalFocalTime(const double& time, const int& ShotID, const int& SignalID, const int& LawID)
{
	QString str;

	MSXML2::IXMLDOMNodePtr pTime;
	pTime = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"TotalFocalTime", (_bstr_t)(char*)"");

	str = QString("%0").arg(time);
	pTime->Puttext((_bstr_t)str.toLatin1().constData());

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	str = QString("Shot%0").arg(ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<LawID></LawID>
	str = QString("Law%0").arg(LawID);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	pNode->appendChild(pTime);
}

//设置某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
void CReceiveFocalLawInfoWriter::SetElementDelay(const double& delay, const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex)
{
	QString str;

	//获取节点<ShotID></ShotID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	str = QString("Shot%0").arg(ShotID);
	pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<LawID></LawID>
	str = QString("Law%0").arg(LawID);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	//获取节点<DelayID></DelayID>
	str = QString("Delay%0").arg(ElementIndex);
	pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

	str = QString("%0").arg(delay);
	pNode->Puttext((_bstr_t)str.toLatin1().constData());
}