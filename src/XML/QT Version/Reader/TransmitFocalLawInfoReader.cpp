#include "TransmitFocalLawInfoReader.h"

#include <QString>

#include <QMap>

#include <QtXml> 
#include <QFile>

//构造函数
CTransmitFocalLawInfoReader::CTransmitFocalLawInfoReader(const std::string& name)
{
	m_bReadEnable = false;

	//打开xml文件
	QFile *file = new QFile(name.c_str());
	if (!file->open(QIODevice::ReadOnly))
	{
		return;
	}
	doc.setContent(file);

	//获取根节点
	root = doc.documentElement();

	m_bReadEnable = true;
}

//析构函数
CTransmitFocalLawInfoReader::~CTransmitFocalLawInfoReader()
{
}

//获得硬件基本信息
HardwareInfo CTransmitFocalLawInfoReader::GetHardwareInfo() const
{
	HardwareInfo hardware;
	if (m_bReadEnable)
	{
		//获取节点<TransmitVoltage></TransmitVoltage>
		QDomNodeList node = root.elementsByTagName("TransmitVoltage");

		//TransmitVoltage
		hardware.m_TransmitVoltage = node.item(0).toElement().text().toDouble();

		//获取节点<UTFrequency></UTFrequency>
		node = root.elementsByTagName("UTFrequency");

		//UTFrequency
		hardware.m_UTFrequency = node.item(0).toElement().text().toDouble();

		//获取节点<SamplingDivisor></SamplingDivisor>
		node = root.elementsByTagName("SamplingDivisor");

		//SamplingDivisor
		hardware.m_SamplingDivisor = node.item(0).toElement().text().toInt();

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
		//获取节点<ShotNum></ShotNum>
		QDomNodeList node = root.elementsByTagName("ShotNum");

		//返回ShotNum
		return node.item(0).toElement().text().toInt();
	}
	return -1;
}

//获得某一个shot所使用的阵元个数
int CTransmitFocalLawInfoReader::GetElementNum(const int& ShotID) const
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<ElementNum></ElementNum>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("ElementNum");

		//返回ElementNum
		return node1.item(0).toElement().text().toInt();
	}
	return -1;
}

//获得某一个shot的A扫深度
double CTransmitFocalLawInfoReader::GetAScanDepth(const int& ShotID) const
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<AScanDepth></AScanDepth>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("AScanDepth");

		//返回ElementNum
		return node1.item(0).toElement().text().toDouble();
	}
	return -1;
}

//获得某一个shot的某一个发射阵元在板卡上对应的阵元ID号
int CTransmitFocalLawInfoReader::GetTransmitElementID(const int& ShotID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<ElementIndex><ElementIndex>
		str = QString("Element%0").arg(ElementIndex);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<TransmitID></TransmitID>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("TransmitID");

		//返回TransmitID
		return node1.item(0).toElement().text().toInt();
	}
	return -1;
}

//获得某一个shot的某一个阵元的发射延迟时间
double CTransmitFocalLawInfoReader::GetTransmitDelay(const int& ShotID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<ElementIndex><ElementIndex>
		str = QString("Element%0").arg(ElementIndex);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<TransmitDelay></TransmitDelay>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("TransmitDelay");

		//返回ElementNum
		return node1.item(0).toElement().text().toDouble();
	}
	return -1;
}

//获得某个shot的某个element的信息
ElementInfo CTransmitFocalLawInfoReader::GetElementInfo(const int& ShotID, const int& ElementIndex) const
{
	ElementInfo element;
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<ElementIndex><ElementIndex>
		str = QString("Element%0").arg(ElementIndex);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<PulseWidth></PulseWidth>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("PulseWidth");

		element.m_PulseWidth = node1.item(0).toElement().text().toDouble();

		//获取节点<Gain></Gain>
		node1 = node.item(0).toElement().elementsByTagName("Gain");

		element.m_Gain = node1.item(0).toElement().text().toDouble();
	}
	return element;
}
