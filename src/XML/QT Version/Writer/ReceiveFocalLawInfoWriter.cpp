#include "ReceiveFocalLawInfoWriter.h"

#include <QString>

#include <QMap>

#include <QtXml> 
#include <QFile>

//构造函数
CReceiveFocalLawInfoWriter::CReceiveFocalLawInfoWriter(const std::string& name)
{
	//打开xml文件
	QFile *file = new QFile(name.c_str());
	if (!file->open(QIODevice::ReadWrite))
	{
		return;
	}
	doc.setContent(file);

	//获取根节点
	root = doc.documentElement();

	m_FilePathName = name;
}

//析构函数
CReceiveFocalLawInfoWriter::~CReceiveFocalLawInfoWriter()
{
	QFile f(m_FilePathName.c_str());
	if (!f.open(QFile::WriteOnly | QFile::Text))
		return;
	QTextStream out(&f);
	doc.save(out, 4);
	f.close();
}

//设置某一个shot所生成的A扫信号个数
void CReceiveFocalLawInfoWriter::SetSignalNum(const int SignalNum, const int& ShotID)
{
	QString str;

	QDomElement pSignalNum = doc.createElement("SignalNum");
	str = QString("%0").arg(SignalNum);
	pSignalNum.appendChild(doc.createTextNode(str));

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pShot = root.elementsByTagName(str).item(0).toElement();

	pShot.appendChild(pSignalNum);

	//创建相应于SignalNum个的Signal子节点
	QDomElement pSignal;
	for (int i = 0; i < SignalNum; i++)
	{
		str = QString("Signal%0").arg(i + 1);
		pSignal = doc.createElement(str);
		pShot.appendChild(pSignal);
	}
}

//设置某一个shot所使用的阵元个数
void CReceiveFocalLawInfoWriter::SetElementNum(const int& ElementNum, const int& ShotID, const int SignalID)
{
	QString str;

	QDomElement pElementNum = doc.createElement("ElementNum");
	str = QString("%0").arg(ElementNum);
	pElementNum.appendChild(doc.createTextNode(str));

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	pNode.appendChild(pElementNum);

	//创建相应于ElementNum个的element子节点
	QDomElement pElement;
	for (int i = 0; i < ElementNum; i++)
	{
		str = QString("Element%0").arg(i + 1);
		pElement = doc.createElement(str);
		pNode.appendChild(pElement);
	}
}

//设置某一个shot的某一个聚焦法则的某一个阵元在板卡上对应的阵元ID号
void CReceiveFocalLawInfoWriter::SetElementID(const int& element, const int& ShotID, const int& SignalID, const int& ElementIndex)
{	
	QString str;

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	//获取节点<ElementIndex></ElementIndex>
	str = QString("Element%0").arg(ElementIndex);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	str = QString("%0").arg(element);
	pNode.appendChild(doc.createTextNode(str));
}

//设置某一个shot所拥有的聚焦法则个数
void CReceiveFocalLawInfoWriter::SetFocalLawNum(const int& LawNum, const int& ShotID, const int& SignalID)
{
	QString str;

	QDomElement pLawNum = doc.createElement("LawNum");
	str = QString("%0").arg(LawNum);
	pLawNum.appendChild(doc.createTextNode(str));

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	pNode.appendChild(pLawNum);
	
	//获取ElementNum
	int ElementNum = pNode.elementsByTagName("ElementNum").item(0).toElement().text().toInt();

	//创建相应于LawNum个的Law子节点
	QDomElement pLaw;
	QDomElement pDelay;
	for (int i = 0; i < LawNum; i++)
	{
		str = QString("Law%0").arg(i + 1);
		pLaw = doc.createElement(str);
		pNode.appendChild(pLaw);
		for (int j = 0; j < ElementNum; j++)
		{
			str = QString("Delay%0").arg(j + 1);
			pDelay = doc.createElement(str);
			pLaw.appendChild(pDelay);
		}
	}
}

//设置某一个Law起作用的时间长度
void CReceiveFocalLawInfoWriter::SetTotalFocalTime(const double& time, const int& ShotID, const int& SignalID, const int& LawID)
{
	QString str;

	QDomElement pTime = doc.createElement("TotalFocalTime");
	str = QString("%0").arg(time);
	pTime.appendChild(doc.createTextNode(str));

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	//获取节点<LawID></LawID>
	str = QString("Law%0").arg(LawID);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	pNode.appendChild(pTime);
}

//设置某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
void CReceiveFocalLawInfoWriter::SetElementDelay(const double& delay, const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex)
{
	QString str;

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();

	//获取节点<SignalID></SignalID>
	str = QString("Signal%0").arg(SignalID);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	//获取节点<LawID></LawID>
	str = QString("Law%0").arg(LawID);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	//获取节点<DelayID></DelayID>
	str = QString("Delay%0").arg(ElementIndex);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	str = QString("%0").arg(delay);
	pNode.appendChild(doc.createTextNode(str));
}