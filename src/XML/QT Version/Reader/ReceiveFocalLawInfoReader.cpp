#include "ReceiveFocalLawInfoReader.h"

#include <QString>

#include <QtXml> 
#include <QFile>

//构造函数
CReceiveFocalLawInfoReader::CReceiveFocalLawInfoReader(const std::string& name)
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
CReceiveFocalLawInfoReader::~CReceiveFocalLawInfoReader()
{
}

//获得总Shot数
int CReceiveFocalLawInfoReader::GetShotNum() const
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

//获得某一个shot所生成的A扫信号个数
int CReceiveFocalLawInfoReader::GetSignalNum(const int& ShotID) const
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<SignalNum></SignalNum>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("SignalNum");

		//返回SignalNum
		return node1.item(0).toElement().text().toInt();
	}
	return -1;
}

//获得某一个shot的某一个Signal所使用的阵元个数
int CReceiveFocalLawInfoReader::GetElementNum(const int& ShotID, const int& SignalID) const
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<ElementNum></ElementNum>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("ElementNum");

		//返回ElementNum
		return node1.item(0).toElement().text().toInt();
	}
	return -1;
}

//获得某一个shot的某一个Signal的某一个阵元在板卡上对应的阵元ID号
int CReceiveFocalLawInfoReader::GetElementID(const int& ShotID, const int& SignalID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<ElementIndex><ElementIndex>
		str = QString("Element%0").arg(ElementIndex);
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName(str);

		//返回ID
		return node1.item(0).toElement().text().toInt();
	}
	return -1;
}

//获得某一个shot所拥有的聚焦法则个数
int CReceiveFocalLawInfoReader::GetFocalLawNum(const int& ShotID, const int& SignalID) const
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<LawNum></LawNum>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("LawNum");

		//返回LawNum
		return node1.item(0).toElement().text().toInt();
	}
	return -1;
}

double CReceiveFocalLawInfoReader::GetTotalFocalTime(const int& ShotID, const int& SignalID, const int& LawID)
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<LawID><LawID>
		str = QString("Law%0").arg(LawID);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<TotalFocalTime><TotalFocalTime>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("TotalFocalTime");

		//返回TotalFocalTime
		return node1.item(0).toElement().text().toDouble();
	}
	return -1;
}

//获得某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
double CReceiveFocalLawInfoReader::GetElementDelay(const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex) const
{
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<ShotID></ShotID>
		str = QString("Shot%0").arg(ShotID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<SignalID></SignalID>
		str = QString("Signal%0").arg(SignalID);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<LawID><LawID>
		str = QString("Law%0").arg(LawID);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<Delay##ElementIndex><Delay##ElementIndex>
		str = QString("Delay%0").arg(ElementIndex);
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName(str);

		//返回TotalFocalTime
		return node1.item(0).toElement().text().toDouble();
	}
	return -1;
}