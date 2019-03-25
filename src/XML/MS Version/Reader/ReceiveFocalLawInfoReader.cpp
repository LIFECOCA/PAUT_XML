#include "ReceiveFocalLawInfoReader.h"

#include <QMessageBox>
#include <QString>

//构造函数
CReceiveFocalLawInfoReader::CReceiveFocalLawInfoReader(const std::string& name)
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
CReceiveFocalLawInfoReader::~CReceiveFocalLawInfoReader()
{
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//获得总Shot数
int CReceiveFocalLawInfoReader::GetShotNum() const
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
int CReceiveFocalLawInfoReader::GetSignalNum(const int& ShotID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<SignalNum></SignalNum>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"SignalNum")->GetfirstChild();

		//返回SignalNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个Signal所使用的阵元个数
int CReceiveFocalLawInfoReader::GetElementNum(const int& ShotID, const int& SignalID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<ElementNum></ElementNum>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"ElementNum")->GetfirstChild();

		//返回ElementNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个Signal的某一个阵元在板卡上对应的阵元ID号
int CReceiveFocalLawInfoReader::GetElementID(const int& ShotID, const int& SignalID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<ElementIndex><ElementIndex>
		str = QString("Element%0").arg(ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData())->GetfirstChild();

		//返回ID
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot所拥有的聚焦法则个数
int CReceiveFocalLawInfoReader::GetFocalLawNum(const int& ShotID, const int& SignalID) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<LawNum></LawNum>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"LawNum")->GetfirstChild();

		//返回LawNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

double CReceiveFocalLawInfoReader::GetTotalFocalTime(const int& ShotID, const int& SignalID, const int& LawID)
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<LawID><LawID>
		str = QString("Law%0").arg(LawID);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<TotalFocalTime></TotalFocalTime>
		pNode = pNode->selectSingleNode((_bstr_t)(char*)"TotalFocalTime")->GetfirstChild();

		//返回ID
		return atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
double CReceiveFocalLawInfoReader::GetElementDelay(const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		pNode = pRoot->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<LawID><LawID>
		str = QString("Law%0").arg(LawID);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData());

		//获取节点<Delay##ElementIndex></Delay##ElementIndex>
		str = QString("Delay%0").arg(ElementIndex);
		pNode = pNode->selectSingleNode((_bstr_t)str.toLatin1().constData())->GetfirstChild();

		//返回Delay
		return atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}