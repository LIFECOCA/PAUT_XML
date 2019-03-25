#include "TransmitFocalLawInfoWriter.h"

#include <QString>

#include <QMap>

#include <QtXml> 
#include <QFile>

//构造函数
CTransmitFocalLawInfoWriter::CTransmitFocalLawInfoWriter(const std::string& name)
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
CTransmitFocalLawInfoWriter::~CTransmitFocalLawInfoWriter()
{
	QFile f(m_FilePathName.c_str());
	if (!f.open(QFile::WriteOnly | QFile::Text))
		return;
	QTextStream out(&f);
	doc.save(out, 4);
	f.close();
}

//设置硬件信息
void CTransmitFocalLawInfoWriter::SetHardwareInfo(const HardwareInfo& hardware)
{
	QString str;

	QDomElement pTransmitVoltage = doc.createElement("TransmitVoltage");
	str = QString("%0").arg(hardware.m_TransmitVoltage);
	pTransmitVoltage.appendChild(doc.createTextNode(str));
	root.appendChild(pTransmitVoltage);

	QDomElement pUTFrequency = doc.createElement("UTFrequency");
	str = QString("%0").arg(hardware.m_UTFrequency);
	pUTFrequency.appendChild(doc.createTextNode(str));
	root.appendChild(pUTFrequency);

	QDomElement pSamplingDivisor = doc.createElement("SamplingDivisor");
	str = QString("%0").arg(hardware.m_SamplingDivisor);
	pSamplingDivisor.appendChild(doc.createTextNode(str));
	root.appendChild(pSamplingDivisor);
}

//设置总Shot数
void CTransmitFocalLawInfoWriter::SetShotNum(const int& ShotNum)
{
	QString str;

	QDomElement pShotNum = doc.createElement("ShotNum");
	str = QString("%0").arg(ShotNum);
	pShotNum.appendChild(doc.createTextNode(str));
	root.appendChild(pShotNum);

	//创建相应于ShotNum个的Shot子节点
	QDomElement pShot;
	for (int i = 0; i < ShotNum; i++)
	{
		str = QString("Shot%0").arg(i + 1);
		pShot = doc.createElement(str);
		root.appendChild(pShot);
	}
}

//获得某一个shot的A扫深度
void CTransmitFocalLawInfoWriter::SetAScanDepth(const double& depth, const int& ShotID)
{
	QString str;

	QDomElement pAScanDepth = doc.createElement("AScanDepth");
	str = QString("%0").arg(depth);
	pAScanDepth.appendChild(doc.createTextNode(str));
	root.appendChild(pAScanDepth);

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pShot = root.elementsByTagName(str).item(0).toElement();

	pShot.appendChild(pAScanDepth);
}

//设置某一个shot所使用的阵元个数
void CTransmitFocalLawInfoWriter::SetElementNum(const int& ElementNum, const int& ShotID)
{
	QString str;

	QDomElement pElementNum = doc.createElement("ElementNum");
	str = QString("%0").arg(ElementNum);
	pElementNum.appendChild(doc.createTextNode(str));
	root.appendChild(pElementNum);

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pShot = root.elementsByTagName(str).item(0).toElement();

	pShot.appendChild(pElementNum);

	//创建相应于ElementNum个的Element子节点
	QDomElement pElement;
	for (int i = 0; i < ElementNum; i++)
	{
		str = QString("Element%0").arg(i + 1);
		pElement = doc.createElement(str);
		pShot.appendChild(pElement);
	}
}

//设置某一个shot的某一个发射阵元在板卡上对应的阵元ID号
void CTransmitFocalLawInfoWriter::SetTransmitElementID(const int& ElementID, const int& ShotID, const int& ElementIndex)
{
	QString str;

	QDomElement pElementID = doc.createElement("TransmitID");
	str = QString("%0").arg(ElementID);
	pElementID.appendChild(doc.createTextNode(str));
	root.appendChild(pElementID);

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();

	//获取节点<ElementIndex></ElementIndex>
	str = QString("Element%0").arg(ElementIndex);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	pNode.appendChild(pElementID);
}

//设置某一个shot的某一个阵元的发射延迟时间
void CTransmitFocalLawInfoWriter::SetTransmitDelay(const double& delay, const int& ShotID, const int& ElementIndex)
{
	QString str;

	QDomElement pTransmitDelay = doc.createElement("TransmitDelay");
	str = QString("%0").arg(delay);
	pTransmitDelay.appendChild(doc.createTextNode(str));
	root.appendChild(pTransmitDelay);

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();

	//获取节点<ElementIndex></ElementIndex>
	str = QString("Element%0").arg(ElementIndex);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	pNode.appendChild(pTransmitDelay);
}

//获得某个shot的某个element的信息
void CTransmitFocalLawInfoWriter::SetElementInfo(const ElementInfo& element, const int& ShotID, const int& ElementIndex)
{
	QString str;

	//创建节点<PulseWidth></PulseWidth>
	QDomElement pPulseWidth = doc.createElement("PulseWidth");
	str = QString("%0").arg(element.m_PulseWidth);
	pPulseWidth.appendChild(doc.createTextNode(str));
	root.appendChild(pPulseWidth);

	//创建节点<Gain></Gain>
	QDomElement pGain = doc.createElement("Gain");
	str = QString("%0").arg(element.m_Gain);
	pGain.appendChild(doc.createTextNode(str));
	root.appendChild(pGain);

	//获取节点<ShotID></ShotID>
	str = QString("Shot%0").arg(ShotID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();

	//获取节点<ElementIndex></ElementIndex>
	str = QString("Element%0").arg(ElementIndex);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	pNode.appendChild(pPulseWidth);
	pNode.appendChild(pGain);
}
