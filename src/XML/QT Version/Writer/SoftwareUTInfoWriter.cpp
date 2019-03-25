#include "SoftwareUTInfoWriter.h"

#include <QString>

#include <QMap>

#include <QtXml> 
#include <QFile>

//构造函数
CSoftwareUTInfoWriter::CSoftwareUTInfoWriter(const std::string& name)
{
	//打开xml文件
	QFile *file = new QFile(name.c_str());
	if (!file->open(QIODevice::WriteOnly))
	{
		return;
	}
	doc.setContent(file);

	QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version = \'1.0\' encoding=\'UTF-8\'");
	doc.appendChild(instruction);

	//创建根节点根节点
	root = doc.createElement("Software");
	doc.appendChild(root);

	m_FilePathName = name;
}

//析构函数
CSoftwareUTInfoWriter::~CSoftwareUTInfoWriter()
{
	QFile f(m_FilePathName.c_str());
	if (!f.open(QFile::WriteOnly | QFile::Text))
		return ;
	QTextStream out(&f);
	doc.save(out, 4);
	f.close();
}

//获得总组数
void CSoftwareUTInfoWriter::SetGroupNum(const int& GroupNum)
{
	QString str;
	QDomElement pGroupNum = doc.createElement("GroupNum");
	str = QString("%0").arg(GroupNum);
	pGroupNum.appendChild(doc.createTextNode(str));
	root.appendChild(pGroupNum);

	//创建相应于组数个的Group子节点
	QDomElement pGroup;
	for (int i = 0; i < GroupNum; i++)
	{
		str = QString("Group%0").arg(i + 1);
		pGroup = doc.createElement(str);
		root.appendChild(pGroup);
	}
}

//设置材料信息
void CSoftwareUTInfoWriter::SetMaterialInfo(const MaterialInfo& material)
{
	QString str;

	//创建节点<MaterialID></MaterialID>
	QDomElement pMaterialID = doc.createElement("MaterialID");
	str = QString("%0").arg(material.m_MaterialID);
	pMaterialID.appendChild(doc.createTextNode(str));
	root.appendChild(pMaterialID);

	//创建节点<MaterialName></MaterialName>
	QDomElement pMaterialName = doc.createElement("MaterialName");
	str = material.m_MaterialName;
	pMaterialName.appendChild(doc.createTextNode(str));
	root.appendChild(pMaterialName);

	//创建节点<LongitudinalVelocity></LongitudinalVelocity>
	QDomElement pLongitudinalVelocity = doc.createElement("LongitudinalVelocity");
	str = QString("%0").arg(material.m_LongitudinalVelocity);
	pLongitudinalVelocity.appendChild(doc.createTextNode(str));
	root.appendChild(pLongitudinalVelocity);

	//创建节点<ShearVelocity></ShearVelocity>
	QDomElement pShearVelocity = doc.createElement("ShearVelocity");
	str = QString("%0").arg(material.m_ShearVelocity);
	pShearVelocity.appendChild(doc.createTextNode(str));
	root.appendChild(pShearVelocity);
}

//获得某个组的探头信息
void CSoftwareUTInfoWriter::SetProbeInfo(const ProbeInfo& probe, const int& GroupID)
{
	QString str;

	//创建节点<ProbeID></ProbePID>
	QDomElement pProbeID = doc.createElement("ProbeID");
	str = QString("%0").arg(probe.m_ProbeID);
	pProbeID.appendChild(doc.createTextNode(str));

	//创建节点<ProbeName></ProbeName>
	QDomElement pProbeName = doc.createElement("ProbeName");
	str = probe.m_ProbeName;
	pProbeName.appendChild(doc.createTextNode(str));

	//创建节点<ProbePitch></ProbePitch>
	QDomElement pProbePitch = doc.createElement("ProbePitch");
	str = QString("%0").arg(probe.m_Pitch);
	pProbePitch.appendChild(doc.createTextNode(str));

	//创建节点<ElementNum></ElementNum>
	QDomElement pElementNum = doc.createElement("ElementNum");
	str = QString("%0").arg(probe.m_ElementNum);
	pElementNum.appendChild(doc.createTextNode(str));

	//创建节点<ProbeFrequency></ProbeFrequency>
	QDomElement pFrequency = doc.createElement("ProbeFrequency");
	str = QString("%0").arg(probe.m_Frequency);
	pFrequency.appendChild(doc.createTextNode(str));

	//创建节点<ProbeHeight></ProbeHeight>
	QDomElement pHeight = doc.createElement("ProbeHeight");
	str = QString("%0").arg(probe.m_Height);
	pHeight.appendChild(doc.createTextNode(str));

	//创建节点<ProbeD></ProbeD>
	QDomElement pProbeD = doc.createElement("ProbeD");
	str = QString("%0").arg(probe.m_D);
	pProbeD.appendChild(doc.createTextNode(str));

	//创建节点<ProbeType></ProbeType>
	QDomElement pProbeType = doc.createElement("ProbeType");
	QMap<ProbeType, QString> probemap;
	probemap[ProbeType::eLinear] = "Linear";
	probemap[ProbeType::eArc] = "Arc";
	str = probemap[probe.m_ProbeType];
	pProbeType.appendChild(doc.createTextNode(str));

	//创建节点<ProbeRadius></ProbeRadius>
	QDomElement pProbeRadius = doc.createElement("ProbeRadius");
	str = QString("%0").arg(probe.m_Radius);
	pProbeRadius.appendChild(doc.createTextNode(str));

	//创建节点<FirstConnection></FirstConnection>
	QDomElement pConnection = doc.createElement("FirstConnection");
	str = QString("%0").arg(probe.m_FirstConnection);
	pConnection.appendChild(doc.createTextNode(str));

	//获取节点<GroupID></GroupID>
	str = QString("Group%0").arg(GroupID);
	QDomElement pGroup = root.elementsByTagName(str).item(0).toElement();

	pGroup.appendChild(pProbeID);
	pGroup.appendChild(pProbeName);
	pGroup.appendChild(pProbePitch);
	pGroup.appendChild(pElementNum);
	pGroup.appendChild(pFrequency);
	pGroup.appendChild(pHeight);
	pGroup.appendChild(pProbeD);
	pGroup.appendChild(pProbeType);
	pGroup.appendChild(pProbeRadius);
	pGroup.appendChild(pConnection);
}

//获得某个组的楔块信息
void CSoftwareUTInfoWriter::SetWedgeInfo(const WedgeInfo& wedge, const int& GroupID)
{
	QString str;

	//创建节点<WedgeID></WedgeID>
	QDomElement pWedgeID = doc.createElement("WedgeID");
	str = QString("%0").arg(wedge.m_WedgeID);
	pWedgeID.appendChild(doc.createTextNode(str));

	//创建节点<WedgeName></WedgeName>
	QDomElement pWedgeName = doc.createElement("WedgeName");
	str = wedge.m_WedgeName;
	pWedgeName.appendChild(doc.createTextNode(str));

	//创建节点<WedgeAngle></WedgeAngle>
	QDomElement pWedgeAngle = doc.createElement("WedgeAngle");
	str = QString("%0").arg(wedge.m_WedgeAngle);
	pWedgeAngle.appendChild(doc.createTextNode(str));

	//创建节点<WedgeVelocity></WedgeVelocity>
	QDomElement pVelocity = doc.createElement("WedgeVelocity");
	str = QString("%0").arg(wedge.m_Velocity);
	pVelocity.appendChild(doc.createTextNode(str));

	//创建节点<ProbeOffsetX></ProbeOffsetX>
	QDomElement pProbeOffsetX = doc.createElement("ProbeOffsetX");
	str = QString("%0").arg(wedge.m_ProbeOffsetX);
	pProbeOffsetX.appendChild(doc.createTextNode(str));

	//创建节点<ProbeOffsetY></ProbeOffsetY>
	QDomElement pProbeOffsetY = doc.createElement("ProbeOffsetY");
	str = QString("%0").arg(wedge.m_ProbeOffsetY);
	pProbeOffsetY.appendChild(doc.createTextNode(str));

	//创建节点<WedgeLength></WedgeLength>
	QDomElement pWedgeLength = doc.createElement("WedgeLength");
	str = QString("%0").arg(wedge.m_w1);
	pWedgeLength.appendChild(doc.createTextNode(str));

	//创建节点<WedgeHeight></WedgeHeight>
	QDomElement pWedgeHeight = doc.createElement("WedgeHeight");
	str = QString("%0").arg(wedge.m_h1);
	pWedgeHeight.appendChild(doc.createTextNode(str));

	//创建节点<OtherInfoNum></OtherInfoNum>
	QDomElement pOtherInfoNum = doc.createElement("OtherInfoNum");
	str = QString("%0").arg((int)wedge.m_OtherInfo.size());
	pOtherInfoNum.appendChild(doc.createTextNode(str));

	std::vector<QDomElement> Other;
	for (int i = 0; i < (int)wedge.m_OtherInfo.size(); i++)
	{
		str = QString("Info%0").arg(i + 1);
		Other.push_back(doc.createElement(str));
		str = QString("%0").arg(wedge.m_OtherInfo[i]);
		Other[i].appendChild(doc.createTextNode(str));
	}

	//获取节点<GroupID></GroupID>
	str = QString("Group%0").arg(GroupID);
	QDomElement pGroup = root.elementsByTagName(str).item(0).toElement();

	pGroup.appendChild(pWedgeID);
	pGroup.appendChild(pWedgeName);
	pGroup.appendChild(pWedgeAngle);
	pGroup.appendChild(pVelocity);
	pGroup.appendChild(pProbeOffsetX);
	pGroup.appendChild(pProbeOffsetY);
	pGroup.appendChild(pWedgeLength);
	pGroup.appendChild(pWedgeHeight);
	pGroup.appendChild(pOtherInfoNum);
	for (int i = 0; i < (int)wedge.m_OtherInfo.size(); i++)
	{
		pGroup.appendChild(Other[i]);
	}
}

//设置某个组的扫查信息
void CSoftwareUTInfoWriter::SetScanInfo(const ScanInfo& scan, const int& GroupID)
{
	QString str;

	//创建节点<AngleStart></AngleStart>
	QDomElement pAngleStart = doc.createElement("AngleStart");
	str = QString("%0").arg(scan.m_AngleStart);
	pAngleStart.appendChild(doc.createTextNode(str));

	//创建节点<AngleStop></AngleStop>
	QDomElement pAngleStop = doc.createElement("AngleStop");
	str = QString("%0").arg(scan.m_AngleStop);
	pAngleStop.appendChild(doc.createTextNode(str));

	//创建节点<ChannelNum></ChannelNum>
	QDomElement pChannelNum = doc.createElement("ChannelNum");
	str = QString("%0").arg(scan.m_ChannelNum);
	pChannelNum.appendChild(doc.createTextNode(str));

	//创建节点<ChannelSize></ChannelSize>
	QDomElement pChannelSize = doc.createElement("ChannelSize");
	str = QString("%0").arg(scan.m_ChannelSize);
	pChannelSize.appendChild(doc.createTextNode(str));

	//创建节点<ElementStart></ElementStart>
	QDomElement pElementStart = doc.createElement("ElementStart");
	str = QString("%0").arg(scan.m_ElementStart);
	pElementStart.appendChild(doc.createTextNode(str));

	//创建节点<ElementStop></ElementStop>
	QDomElement pElementStop = doc.createElement("ElementStop");
	str = QString("%0").arg(scan.m_ElementStop);
	pElementStop.appendChild(doc.createTextNode(str));

	//创建节点<SendFocus></SendFocus>
	QDomElement pSendFocus = doc.createElement("SendFocus");
	str = QString("%0").arg(scan.m_SendFocus);
	pSendFocus.appendChild(doc.createTextNode(str));

	//创建节点<RecvFocusStart></RecvFocusStart>
	QDomElement pRecvFocusStart = doc.createElement("RecvFocusStart");
	str = QString("%0").arg(scan.m_RecvFocusStart);
	pRecvFocusStart.appendChild(doc.createTextNode(str));

	//创建节点<RecvFocusEnd></RecvFocusEnd>
	QDomElement pRecvFocusEnd = doc.createElement("RecvFocusEnd");
	str = QString("%0").arg(scan.m_RecvFocusEnd);
	pRecvFocusEnd.appendChild(doc.createTextNode(str));

	//创建节点<RecvFocusNum></RecvFocusNum>
	QDomElement pRecvFocusNum = doc.createElement("RecvFocusNum");
	str = QString("%0").arg(scan.m_RecvFocusNum);
	pRecvFocusNum.appendChild(doc.createTextNode(str));

	//创建节点<Reflect></Reflect>
	QDomElement pReflect = doc.createElement("Reflect");
	str = QString("%0").arg(scan.m_Reflect);
	pReflect.appendChild(doc.createTextNode(str));

	//创建节点<VelocityType></VelocityType>
	QDomElement pVelocityType = doc.createElement("VelocityType");
	QMap<VelocityType, QString> velmap;
	velmap[VelocityType::eLongitudinal] = "Longitudinal";
	velmap[VelocityType::eShear] = "Shear";
	str = velmap[scan.m_VelocityType];
	pVelocityType.appendChild(doc.createTextNode(str));

	//创建节点<Type></Type>
	QDomElement pType = doc.createElement("Type");
	QMap<ScanType, QString> typemap;
	typemap[ScanType::ePAAngle] = "PAAngle";
	typemap[ScanType::ePALinear] = "PALinear";
	typemap[ScanType::eTOFD] = "TOFD";
	str = typemap[scan.m_Type];
	pType.appendChild(doc.createTextNode(str));

	//获取节点<GroupID></GroupID>
	str = QString("Group%0").arg(GroupID);
	QDomElement pGroup = root.elementsByTagName(str).item(0).toElement();

	pGroup.appendChild(pAngleStart);
	pGroup.appendChild(pAngleStop);
	pGroup.appendChild(pChannelSize);
	pGroup.appendChild(pElementStart);
	pGroup.appendChild(pElementStop);
	pGroup.appendChild(pVelocityType);
	pGroup.appendChild(pType);
	pGroup.appendChild(pReflect);
}

//设置位置信息
void CSoftwareUTInfoWriter::SetPositionInfo(const PositionInfo& position, const int& GroupID)
{
	QString str;

	//创建节点<InspectPos></InspectPos>
	QDomElement pInspectPos = doc.createElement("InspectPos");
	switch (position.m_InspectPos)
	{
	case InspectPos::WeldPDetectPos_Up:
		str = "WeldPDetectPos_Up";
		break;
	case InspectPos::WeldPDetectPos_Down:
		str = "WeldPDetectPos_Down";
		break;
	case InspectPos::WeldPDetectPos_UpL:
		str = "WeldPDetectPos_UpL";
		break;
	case InspectPos::WeldPDetectPos_UpR:
		str = "WeldPDetectPos_UpR";
		break;
	case InspectPos::WeldPDetectPos_DownL:
		str = "WeldPDetectPos_DownL";
		break;
	case InspectPos::WeldPDetectPos_DownR:
		str = "WeldPDetectPos_DownR";
		break;
	case InspectPos::WeldFDetectPos_WebUp:
		str = "WeldFDetectPos_WebUp";
		break;
	case InspectPos::WeldFDetectPos_WebDown:
		str = "WeldFDetectPos_WebDown";
		break;
	case InspectPos::WeldFDetectPos_MomUpL:
		str = "WeldFDetectPos_MomUpL";
		break;
	case InspectPos::WeldFDetectPos_MomUpR:
		str = "WeldFDetectPos_MomUpR";
		break;
	case InspectPos::WeldFDetectPos_MomDown:
		str = "WeldFDetectPos_MomDown";
		break;
	default:
		break;
	}
	pInspectPos.appendChild(doc.createTextNode(str));

	//创建节点<WedgeOffset></WedgeOffset>
	QDomElement pWedgeOffset = doc.createElement("WedgeOffset");
	str = QString("%0").arg(position.m_WedgeOffset);
	pWedgeOffset.appendChild(doc.createTextNode(str));

	//获取节点<GroupID></GroupID>
	str = QString("Group%0").arg(GroupID);
	QDomElement pGroup = root.elementsByTagName(str).item(0).toElement();
	
	pGroup.appendChild(pInspectPos);
	pGroup.appendChild(pWedgeOffset);
}

//获得某个组采集信息
void CSoftwareUTInfoWriter::SetGroupBasicInfo(const GroupBasicInfo& group, const int& GroupID)
{
	QString str;

	//创建节点<ScanType></ScanType>
	QDomElement pScanType = doc.createElement("ScanType");
	QMap<ScanType, QString> typemap;
	typemap[ScanType::ePAAngle] = "PA";
	typemap[ScanType::ePALinear] = "PA";
	typemap[ScanType::ePA] = "PA";
	typemap[ScanType::eTOFD] = "TOFD";
	typemap[ScanType::eSpecial] = "Special";
	str = typemap[group.m_ScanType];
	pScanType.appendChild(doc.createTextNode(str));

	//创建节点<TRMode></TRMode>
	QDomElement pTRMode = doc.createElement("TRMode");
	QMap<TRMode, QString> trmap;
	trmap[TRMode::eTransmitReceive] = "TransmitReceive";
	trmap[TRMode::eTransmit] = "Transmit";
	str = trmap[group.m_TRMode];
	pTRMode.appendChild(doc.createTextNode(str));

	//创建节点<ChannelNum></ChannelNum>
	QDomElement pChannelNum = doc.createElement("ChannelNum");
	str = QString("%0").arg(group.m_ChannelNum);
	pChannelNum.appendChild(doc.createTextNode(str));

	//创建节点<GateNum></GateNum>
	QDomElement pGateNum = doc.createElement("GateNum");
	str = QString("%0").arg(GroupInfo::eMaxGateNum);
	pGateNum.appendChild(doc.createTextNode(str));

	//创建节点<PartVelocity></PartVelocity>
	QDomElement pPartVelocity = doc.createElement("PartVelocity");
	str = QString("%0").arg(group.m_PartVelocity);
	pPartVelocity.appendChild(doc.createTextNode(str));

	//创建节点<SendFocus></SendFocus>
	QDomElement pSendFocus = doc.createElement("SendFocus");
	str = QString("%0").arg(group.m_SendFocus);
	pSendFocus.appendChild(doc.createTextNode(str));

	//创建节点<RecvFocusNum></RecvFocusNum>
	QDomElement pRecvFocusNum = doc.createElement("RecvFocusNum");
	str = QString("%0").arg(group.m_RecvFocusNum);
	pRecvFocusNum.appendChild(doc.createTextNode(str));

	//创建节点<RecvFocusStart></RecvFocusStart>
	QDomElement pRecvFocusStart = doc.createElement("RecvFocusStart");
	str = QString("%0").arg(group.m_RecvFocusStart);
	pRecvFocusStart.appendChild(doc.createTextNode(str));

	//创建节点<RecvFocusEnd></RecFocusEnd>
	QDomElement pRecFocusEnd = doc.createElement("RecvFocusEnd");
	str = QString("%0").arg(group.m_RecvFocusEnd);
	pRecFocusEnd.appendChild(doc.createTextNode(str));

	//获取节点<GroupID></GroupID>
	str = QString("Group%0").arg(GroupID);
	QDomElement pGroup = root.elementsByTagName(str).item(0).toElement();

	pGroup.appendChild(pChannelNum);
	pGroup.appendChild(pScanType);
	pGroup.appendChild(pTRMode);
	pGroup.appendChild(pPartVelocity);
	pGroup.appendChild(pSendFocus);
	pGroup.appendChild(pRecvFocusNum);
	pGroup.appendChild(pRecvFocusStart);
	pGroup.appendChild(pRecFocusEnd);

	//创建相应于ChannelNum个的Channel子节点
	QDomElement pChannel;
	for (int i = 0; i < group.m_ChannelNum; i++)
	{
		str = QString("Channel%0").arg(i + 1);
		pChannel = doc.createElement(str);
		pGroup.appendChild(pChannel);
	}

	//创建相应于GateNum个的Gate子节点
	QDomElement pGate;
	for (int i = 0; i < GroupInfo::eMaxGateNum; i++)
	{
		str = QString("GateID%0").arg(i);
		pGate = doc.createElement(str);
		pGroup.appendChild(pGate);
	}
}

//获得某个组的声束信息
void CSoftwareUTInfoWriter::SetBeamBasicInfo(const BeamBasicInfo& beam, const int& GroupID, const int& ChannelID)
{
	QString str;
	//创建节点<ScanAngle></ScanAngle>
	QDomElement pScanAngle = doc.createElement("ScanAngle");
	str = QString("%0").arg(beam.m_BeamAngle);
	pScanAngle.appendChild(doc.createTextNode(str));

	//创建节点<CenterElement></CenterElement>
	QDomElement pCenterElement = doc.createElement("CenterElement");
	str = QString("%0").arg(beam.m_CenterElementID);
	pCenterElement.appendChild(doc.createTextNode(str));

	//创建节点<WedgeDelay></WedgeDelay>
	QDomElement pWedgeDelay = doc.createElement("WedgeDelay");
	str = QString("%0").arg(beam.m_WedgeDelay);
	pWedgeDelay.appendChild(doc.createTextNode(str));

	//获取节点<GroupID></GroupID>
	str = QString("Group%0").arg(GroupID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();
	str = QString("Channel%0").arg(ChannelID);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	pNode.appendChild(pScanAngle);
	pNode.appendChild(pCenterElement);
	pNode.appendChild(pWedgeDelay);
}

//设置闸门信息
void CSoftwareUTInfoWriter::SetGateInfo(const GateInfo& gate, const int& GroupID, const int& GateID)
{
	QString str;
	//创建节点<GateIsUsed></GateIsUsed>
	QDomElement pGateIsUsed = doc.createElement("GateIsUsed");
	str = QString("%0").arg(gate.m_GateIsUsed ? 1 : 0);
	pGateIsUsed.appendChild(doc.createTextNode(str));

	//创建节点<GateType></GateType>
	QDomElement pGateType = doc.createElement("GateType");
	str = QString("%0").arg(gate.m_GateType);
	pGateType.appendChild(doc.createTextNode(str));

	//创建节点<GateStart></GateStart>
	QDomElement pGateStart = doc.createElement("GateStart");
	str = QString("%0").arg(gate.m_GateStart);
	pGateStart.appendChild(doc.createTextNode(str));

	//创建节点<GateRange></GateRange>
	QDomElement pGateRange = doc.createElement("GateRange");
	str = QString("%0").arg(gate.m_GateRange);
	pGateRange.appendChild(doc.createTextNode(str));

	//创建节点<GateThreshold></GateThreshold>
	QDomElement pGateThreshold = doc.createElement("GateThreshold");
	str = QString("%0").arg(gate.m_GateThreshold);
	pGateThreshold.appendChild(doc.createTextNode(str));

	//获取节点<GroupID></GroupID>
	str = QString("Group%0").arg(GroupID);
	QDomElement pNode = root.elementsByTagName(str).item(0).toElement();
	str = QString("GateID%0").arg(GateID);
	pNode = pNode.elementsByTagName(str).item(0).toElement();

	pNode.appendChild(pGateIsUsed);
	pNode.appendChild(pGateType);
	pNode.appendChild(pGateStart);
	pNode.appendChild(pGateRange);
	pNode.appendChild(pGateThreshold);

}

//获得编码器信息
void CSoftwareUTInfoWriter::SetEncoderInfo(const EncoderInfo& encoder)
{
	QString str;
	//创建节点<PointNumOfScanAxis></PointNumOfScanAxis>
	QDomElement pPointNumOfScanAxis = doc.createElement("PointNumOfScanAxis");
	str = QString("%0").arg(encoder.m_ScanPointNum);
	pPointNumOfScanAxis.appendChild(doc.createTextNode(str));
	root.appendChild(pPointNumOfScanAxis);

	//创建节点<ScanAxisStart></ScanAxisStart>
	QDomElement pPScanAxisStart = doc.createElement("ScanAxisStart");
	str = QString("%0").arg(encoder.m_ScanStart);
	pPScanAxisStart.appendChild(doc.createTextNode(str));
	root.appendChild(pPScanAxisStart);

	//创建节点<ScanAxisRange></ScanAxisRange>
	QDomElement pScanAxisRange = doc.createElement("ScanAxisRange");
	str = QString("%0").arg(encoder.m_ScanRange);
	pScanAxisRange.appendChild(doc.createTextNode(str));
	root.appendChild(pScanAxisRange);

	//创建节点<PointNumOfIndexAxis></PointNumOfIndexAxis>
	QDomElement pPointNumOfIndexAxis = doc.createElement("PointNumOfIndexAxis");
	str = QString("%0").arg(encoder.m_IndexPointNum);
	pPointNumOfIndexAxis.appendChild(doc.createTextNode(str));
	root.appendChild(pPointNumOfIndexAxis);

	//创建节点<IndexAxisStart></IndexAxisStart>
	QDomElement pIndexAxisStart = doc.createElement("IndexAxisStart");
	str = QString("%0").arg(encoder.m_IndexStart);
	pIndexAxisStart.appendChild(doc.createTextNode(str));
	root.appendChild(pIndexAxisStart);

	//创建节点<ScanAxisRange></ScanAxisRange>
	QDomElement pIndexAxisRange = doc.createElement("IndexAxisRange");
	str = QString("%0").arg(encoder.m_IndexRange);
	pIndexAxisRange.appendChild(doc.createTextNode(str));
	root.appendChild(pIndexAxisRange);

	//创建节点<Encoder1Resolution></Encoder1Resolution>
	QDomElement pEncoder1Resolution = doc.createElement("Encoder1Resolution");
	str = QString("%0").arg(encoder.m_EncoderResolution[0]);
	pEncoder1Resolution.appendChild(doc.createTextNode(str));
	root.appendChild(pEncoder1Resolution);

	//创建节点<Encoder2Resolution></Encoder2Resolution>
	QDomElement pEncoder2Resolution = doc.createElement("Encoder2Resolution");
	str = QString("%0").arg(encoder.m_EncoderResolution[1]);
	pEncoder2Resolution.appendChild(doc.createTextNode(str));
	root.appendChild(pEncoder2Resolution);

	//创建节点<Encoder1Porlarity></Encoder1Porlarity>
	QDomElement pEncoder1Porlarity = doc.createElement("Encoder1Porlarity");
	str = QString("%0").arg(encoder.m_EncoderPolarity[0]);
	pEncoder1Porlarity.appendChild(doc.createTextNode(str));
	root.appendChild(pEncoder1Porlarity);

	//创建节点<Encoder2Porlarity></Encoder2Porlarity>
	QDomElement pEncoder2Porlarity = doc.createElement("Encoder2Porlarity");
	str = QString("%0").arg(encoder.m_EncoderPolarity[1]);
	pEncoder2Porlarity.appendChild(doc.createTextNode(str));
	root.appendChild(pEncoder2Porlarity);

	//创建节点<ScanAxisID></ScanAxisID>
	QDomElement pScanAxisID = doc.createElement("ScanAxisID");
	str = QString("%0").arg(encoder.m_ScanAxisID);
	pScanAxisID.appendChild(doc.createTextNode(str));
	root.appendChild(pScanAxisID);
}

//设置滤波器信息
void CSoftwareUTInfoWriter::SetFilterInfo(const FilterInfo& filter)
{
	QString str;
	//创建节点<FilterType></FilterType>
	QDomElement pFilterType = doc.createElement("FilterType");
	QMap<FilterType, QString> filtermap;
	filtermap[FilterType::eNoFilter] = "No Filter";
	filtermap[FilterType::eBandPassFilter] = "Band Filter";
	filtermap[FilterType::eLowPassFilter] = "Lowpass Filter";
	filtermap[FilterType::eEnvellopFilter] = "Envelop Filter";
	str = filtermap[filter.m_FilterType];
	pFilterType.appendChild(doc.createTextNode(str));
	root.appendChild(pFilterType);

	//创建节点<Freq1></Freq1>
	QDomElement pFreq1 = doc.createElement("Freq1");
	str = QString("%0").arg(filter.m_Frequency1);
	pFreq1.appendChild(doc.createTextNode(str));
	root.appendChild(pFreq1);

	//创建节点<Freq2></Freq2>
	QDomElement pFreq2 = doc.createElement("Freq2");
	str = QString("%0").arg(filter.m_Frequency2);
	pFreq2.appendChild(doc.createTextNode(str));
	root.appendChild(pFreq2);
}

//设置试件类型
void CSoftwareUTInfoWriter::SetPartType(const PartType& type)
{
	QString str;
	//创建节点<PartType></PartType>
	QDomElement pPartType = doc.createElement("PartType");
	QMap<PartType, QString> typemap;
	typemap[PartType::PWeld] = "Plate Weld";
	typemap[PartType::FWeld] = "Fillet Weld";

	str = typemap[type];
	pPartType.appendChild(doc.createTextNode(str));
	root.appendChild(pPartType);
}

//设置平板对接试件的信息
void CSoftwareUTInfoWriter::SetPlateWeldInfo(const PlateWeldInfo& plate, const GrooveInfo& groove)
{
	QString str;
	//创建节点<MotherThickness></MotherThickness>
	QDomElement pMotherThickness = doc.createElement("MotherThickness");
	str = QString("%0").arg(plate.m_MotherThickness);
	pMotherThickness.appendChild(doc.createTextNode(str));
	root.appendChild(pMotherThickness);

	//创建节点<MotherWidth></MotherWidth>
	QDomElement pMotherWidth = doc.createElement("MotherWidth");
	str = QString("%0").arg(plate.m_MotherWidth);
	pMotherWidth.appendChild(doc.createTextNode(str));
	root.appendChild(pMotherWidth);

	//创建节点<UpWeldWidth></UpWeldWidth>
	QDomElement pUpWeldWidth = doc.createElement("UpWeldWidth");
	str = QString("%0").arg(plate.m_UpWeldWidth);
	pUpWeldWidth.appendChild(doc.createTextNode(str));
	root.appendChild(pUpWeldWidth);

	//创建节点<UpWeldHeight></UpWeldHeight>
	QDomElement pUpWeldHeight = doc.createElement("UpWeldHeight");
	str = QString("%0").arg(plate.m_UpWeldHeight);
	pUpWeldHeight.appendChild(doc.createTextNode(str));
	root.appendChild(pUpWeldHeight);

	//创建节点<DownWeldWidth></DownWeldWidth>
	QDomElement pDownWeldWidth = doc.createElement("DownWeldWidth");
	str = QString("%0").arg(plate.m_DownWeldWidth);
	pDownWeldWidth.appendChild(doc.createTextNode(str));
	root.appendChild(pDownWeldWidth);

	//创建节点<DownWeldHeight></DownWeldHeight>
	QDomElement pDownWeldHeight = doc.createElement("DownWeldHeight");
	str = QString("%0").arg(plate.m_DownWeldHeight);
	pDownWeldHeight.appendChild(doc.createTextNode(str));
	root.appendChild(pDownWeldHeight);

	//创建节点<GrooveType></GrooveType>
	QDomElement pGrooveType = doc.createElement("GrooveType");
	QMap<GrooveType, QString> groovemap;
	groovemap[GrooveType::GrooveI] = "GrooveI";
	groovemap[GrooveType::GrooveV] = "GrooveV";
	groovemap[GrooveType::GrooveVV] = "GrooveVV";
	groovemap[GrooveType::GrooveY] = "GrooveY";
	groovemap[GrooveType::GrooveYY] = "GrooveYY";
	str = groovemap[groove.m_GrooveType];
	pGrooveType.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveType);

	//创建节点<GrooveIb></GrooveIb>
	QDomElement pGrooveIb = doc.createElement("GrooveIb");
	str = QString("%0").arg(groove.m_GrooveIb);
	pGrooveIb.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveIb);

	//创建节点<GrooveYb></GrooveYb>
	QDomElement pGrooveYb = doc.createElement("GrooveYb");
	str = QString("%0").arg(groove.m_GrooveYb);
	pGrooveYb.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveYb);

	//创建节点<GrooveYP></GrooveYP>
	QDomElement pGrooveYP = doc.createElement("GrooveYP");
	str = QString("%0").arg(groove.m_GrooveYP);
	pGrooveYP.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveYP);

	//创建节点<GrooveVa></GrooveVa>
	QDomElement pGrooveVa = doc.createElement("GrooveVa");
	str = QString("%0").arg(groove.m_GrooveVa);
	pGrooveVa.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveVa);

	//创建节点<GrooveVb></GrooveVb>
	QDomElement pGrooveVb = doc.createElement("GrooveVb");
	str = QString("%0").arg(groove.m_GrooveVb);
	pGrooveVb.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveVb);

	//创建节点<GrooveYYb></GrooveYYb>
	QDomElement pGrooveYYb = doc.createElement("GrooveYYb");
	str = QString("%0").arg(groove.m_GrooveYYb);
	pGrooveYYb.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveYYb);

	//创建节点<GrooveYYP></GrooveYYP>
	QDomElement pGrooveYYP = doc.createElement("GrooveYYP");
	str = QString("%0").arg(groove.m_GrooveYYP);
	pGrooveYYP.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveYYP);

	//创建节点<GrooveVa></GrooveVVb>
	QDomElement pGrooveVVb = doc.createElement("GrooveVVb");
	str = QString("%0").arg(groove.m_GrooveVVb);
	pGrooveVVb.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveVVb);

	//创建节点<GrooveVb></GrooveVVH>
	QDomElement pGrooveVVH = doc.createElement("GrooveVVH");
	str = QString("%0").arg(groove.m_GrooveVVH);
	pGrooveVVH.appendChild(doc.createTextNode(str));
	root.appendChild(pGrooveVVH);
}

//设置角接试件的信息
void CSoftwareUTInfoWriter::SetFilletWeldInfo(const FilletWeldInfo& fillet)
{
	/*QString str;

	//创建节点<MotherWidth></MotherWidth>
	MSXML2::IXMLDOMNodePtr pMotherWidth;
	pMotherWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"MotherWidth", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_MotherWidth);
	pMotherWidth->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pMotherWidth);

	//创建节点<MotherThickness></MotherThickness>
	MSXML2::IXMLDOMNodePtr pMotherThickness;
	pMotherThickness = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"MotherThickness", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_MotherThickness);
	pMotherThickness->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pMotherThickness);

	//创建节点<WebAngle></WebAngle>
	MSXML2::IXMLDOMNodePtr pWebAngle;
	pWebAngle = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WebAngle", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_WebAngle);
	pWebAngle->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pWebAngle);

	//创建节点<WebWidth></WebWidth>
	MSXML2::IXMLDOMNodePtr pWebWidth;
	pWebWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WebWidth", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_WebWidth);
	pWebWidth->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pWebWidth);

	//创建节点<WebThickness></WebThickness>
	MSXML2::IXMLDOMNodePtr pWebThickness;
	pWebThickness = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WebThickness", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_WebThickness);
	pWebThickness->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pWebThickness);

	//创建节点<BlunteEdgeOffset></BlunteEdgeOffset>
	MSXML2::IXMLDOMNodePtr pBlunteEdgeOffset;
	pBlunteEdgeOffset = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"BlunteEdgeOffset", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_BlunteEdgeOffset);
	pBlunteEdgeOffset->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pBlunteEdgeOffset);

	//创建节点<BlunteEdgeHeight></BlunteEdgeHeight>
	MSXML2::IXMLDOMNodePtr pBlunteEdgeHeight;
	pBlunteEdgeHeight = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"BlunteEdgeHeight", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_BlunteEdgeHeight);
	pBlunteEdgeHeight->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pBlunteEdgeHeight);

	//创建节点<BlunteEdgeWidth></BlunteEdgeWidth>
	MSXML2::IXMLDOMNodePtr pBlunteEdgeWidth;
	pBlunteEdgeWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"BlunteEdgeWidth", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_BlunteEdgeWidth);
	pBlunteEdgeWidth->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pBlunteEdgeWidth);

	//创建节点<LeftWeldAngle></LeftWeldAngle>
	MSXML2::IXMLDOMNodePtr pLeftWeldAngle;
	pLeftWeldAngle = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"LeftWeldAngle", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_LeftWeldAngle);
	pLeftWeldAngle->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pLeftWeldAngle);

	//创建节点<RightWeldAngle></RightWeldAngle>
	MSXML2::IXMLDOMNodePtr pRightWeldAngle;
	pRightWeldAngle = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RightWeldAngle", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_RightWeldAngle);
	pRightWeldAngle->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pRightWeldAngle);

	//创建节点<LeftWeldWidth></LeftWeldWidth>
	MSXML2::IXMLDOMNodePtr pLeftWeldWidth;
	pLeftWeldWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"LeftWeldWidth", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_LeftWeldWidth);
	pLeftWeldWidth->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pLeftWeldWidth);

	//创建节点<RightWeldWidth></RightWeldWidth>
	MSXML2::IXMLDOMNodePtr pRightWeldWidth;
	pRightWeldWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RightWeldWidth", (_bstr_t)(char*)"");
	str = QString("%0").arg(fillet.m_RightWeldWidth);
	pRightWeldWidth->Puttext((_bstr_t)str.toLatin1().constData());
	pRoot->appendChild(pRightWeldWidth);*/
}
