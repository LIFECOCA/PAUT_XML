#include "SoftwareUTInfoReader.h"

#include <QString>

#include <QMap>

#include <QtXml> 
#include <QFile>

//公有构造函数
CSoftwareUTInfoReader::CSoftwareUTInfoReader(const std::string& name)
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

//公有析构函数
CSoftwareUTInfoReader::~CSoftwareUTInfoReader()
{
}

//获得总组数
int CSoftwareUTInfoReader::GetGroupNum() const
{
	if (m_bReadEnable)
	{
		//获取节点<GroupNum></GroupNum>
		QDomNodeList node = root.elementsByTagName("GroupNum");

		//返回GroupNum
		return node.item(0).toElement().text().toInt();
	}
	return -1;
}

//获得材料信息
MaterialInfo CSoftwareUTInfoReader::GetMaterialInfo() const
{
	MaterialInfo material;

	if (m_bReadEnable)
	{
		//获取节点<MaterialID></MaterialID>
		QDomNodeList node = root.elementsByTagName("MaterialID");

		//MaterialID
		material.m_MaterialID = node.item(0).toElement().text().toInt();

		//获取节点<MaterialName></MaterialName>
		node = root.elementsByTagName("MaterialName");

		//MaterialID
		material.m_MaterialName = node.item(0).toElement().text();

		//获取节点<LongitudinalVelocity></LongitudinalVelocity>
		node = root.elementsByTagName("LongitudinalVelocity");

		//LongitudinalVelocity
		material.m_LongitudinalVelocity = node.item(0).toElement().text().toDouble();

		//获取节点<ShearVelocity></ShearVelocity>
		node = root.elementsByTagName("ShearVelocity");

		//LongitudinalVelocity
		material.m_ShearVelocity = node.item(0).toElement().text().toDouble();
	}

	return material;
}

//获得某个组的探头信息
ProbeInfo CSoftwareUTInfoReader::GetProbeInfo(const int& GroupID) const
{
	ProbeInfo probe;
	if (m_bReadEnable)
	{
		QString str;
		//获取节点<GroupID></GroupID>
		str = QString("Group%0").arg(GroupID);
		QDomNodeList node = root.elementsByTagName(str);
		

		//获取节点<ProbeID></ProbeID>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("ProbeID");

		//ProbeID
		probe.m_ProbeID = node1.item(0).toElement().text().toInt();

		//获取节点<ProbeName></ProbeName>
		node1 = node.item(0).toElement().elementsByTagName("ProbeName");

		//ProbeName
		probe.m_ProbeName = node1.item(0).toElement().text();

		//获取节点<ProbePitch></ProbePitch>
		node1 = node.item(0).toElement().elementsByTagName("ProbePitch");

		//ProbePitch
		probe.m_Pitch = node1.item(0).toElement().text().toDouble();

		//获取节点<ElementNum></ElementNum>
		node1 = node.item(0).toElement().elementsByTagName("ElementNum");

		//ElementNum
		probe.m_ElementNum = node1.item(0).toElement().text().toInt();

		//获取节点<ProbeFrequency></ProbeFrequency>
		node1 = node.item(0).toElement().elementsByTagName("ProbeFrequency");

		//ProbeFrequency
		probe.m_Frequency = node1.item(0).toElement().text().toDouble();

		//获取节点<ProbeHeight></ProbeHeight>
		node1 = node.item(0).toElement().elementsByTagName("ProbeHeight");

		//ProbeHeight
		probe.m_Height = node1.item(0).toElement().text().toDouble();

		//获取节点<ProbeD></ProbeD>
		node1 = node.item(0).toElement().elementsByTagName("ProbeD");

		//ProbeD
		probe.m_D = node1.item(0).toElement().text().toDouble();

		//获取节点<ProbeType></ProbeType>
		node1 = node.item(0).toElement().elementsByTagName("ProbeType");

		//ProbeType
		str = node1.item(0).toElement().text();

		QMap<QString, ProbeType> typemap;
		typemap["Linear"] = ProbeType::eLinear;
		typemap["Arc"] = ProbeType::eArc;

		probe.m_ProbeType = typemap[str];

		//获取节点<ProbeRadius></ProbeRadius>
		node1 = node.item(0).toElement().elementsByTagName("ProbeRadius");

		//ProbeRadius
		probe.m_Radius = node1.item(0).toElement().text().toDouble();

		//获取节点<FirstConnection></FirstConnection>
		node1 = node.item(0).toElement().elementsByTagName("FirstConnection");

		//FirstConnection
		probe.m_FirstConnection = node1.item(0).toElement().text().toInt();
	}
	return probe;
}

//获得某个组的楔块信息
WedgeInfo CSoftwareUTInfoReader::GetWedgeInfo(const int& GroupID) const
{
	WedgeInfo wedge;
	if (m_bReadEnable)
	{
		QString str;
		//获取节点<GroupID></GroupID>
		str = QString("Group%0").arg(GroupID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<WedgeID></WedgeID>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("WedgeID");

		//WedgeID
		wedge.m_WedgeID = node1.item(0).toElement().text().toInt();

		//获取节点<WedgeName></WedgeName>
		node1 = node.item(0).toElement().elementsByTagName("WedgeName");

		//WedgeName
		wedge.m_WedgeName = node1.item(0).toElement().text();

		//获取节点<WedgeAngle></WedgeAngle>
		node1 = node.item(0).toElement().elementsByTagName("WedgeAngle");

		//WedgeAngle
		wedge.m_WedgeAngle = node1.item(0).toElement().text().toDouble();

		//获取节点<WedgeVelocity></WedgeVelocity>
		node1 = node.item(0).toElement().elementsByTagName("WedgeVelocity");

		//WedgeVelocity
		wedge.m_Velocity = node1.item(0).toElement().text().toDouble();

		//获取节点<ProbeOffsetX></ProbeOffsetX>
		node1 = node.item(0).toElement().elementsByTagName("ProbeOffsetX");

		//ProbeOffsetX
		wedge.m_ProbeOffsetX = node1.item(0).toElement().text().toDouble();

		//获取节点<ProbeOffsetY></ProbeOffsetY>
		node1 = node.item(0).toElement().elementsByTagName("ProbeOffsetY");

		//ProbeOffsetY
		wedge.m_ProbeOffsetY = node1.item(0).toElement().text().toDouble();

		//获取节点<WedgeLength></WedgeLength>
		node1 = node.item(0).toElement().elementsByTagName("WedgeLength");

		//ProbeOffsetY
		wedge.m_w1 = node1.item(0).toElement().text().toDouble();

		//获取节点<WedgeHeight></WedgeHeight>
		node1 = node.item(0).toElement().elementsByTagName("WedgeHeight");

		//WedgeHeight
		wedge.m_h1 = node1.item(0).toElement().text().toDouble();

		wedge.m_h2 = wedge.m_ProbeOffsetY - wedge.m_ProbeOffsetX * tan(wedge.m_WedgeAngle / 45 * atan(1.0));

		//获取节点<OtherInfoNum></OtherInfoNum>
		node1 = node.item(0).toElement().elementsByTagName("OtherInfoNum");

		int num = node1.item(0).toElement().text().toInt();
		for (int i = 0; i < num; i++)
		{
			str = QString("Info%0").arg(i + 1);
			node1 = node.item(0).toElement().elementsByTagName(str);
			wedge.m_OtherInfo.push_back(node1.item(0).toElement().text().toDouble());
		}
	}
	return wedge;
}

//获得某个组的扫查信息
ScanInfo CSoftwareUTInfoReader::GetScanInfo(const int& GroupID) const
{
	ScanInfo scan;

	if (m_bReadEnable)
	{
		QString str;
		//获取节点<GroupID></GroupID>
		str = QString("Group%0").arg(GroupID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<AngleStart></AngleStart>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("AngleStart");

		//AngleStart
		scan.m_AngleStart = node1.item(0).toElement().text().toDouble();

		//获取节点<AngleStop></AngleStop>
		node1 = node.item(0).toElement().elementsByTagName("AngleStop");

		//AngleStop
		scan.m_AngleStop = node1.item(0).toElement().text().toDouble();

		//获取节点<ChannelNum></ChannelNum>
		node1 = node.item(0).toElement().elementsByTagName("ChannelNum");

		//ChannelNum
		scan.m_ChannelNum = node1.item(0).toElement().text().toInt();

		//获取节点<ChannelSize></ChannelSize>
		node1 = node.item(0).toElement().elementsByTagName("ChannelSize");

		//ChannelSize
		scan.m_ChannelSize = node1.item(0).toElement().text().toInt();

		//获取节点<ElementStart></ElementStart>
		node1 = node.item(0).toElement().elementsByTagName("ElementStart");

		//ElementStart
		scan.m_ElementStart = node1.item(0).toElement().text().toInt();

		//获取节点<ElementStop></ElementStop>
		node1 = node.item(0).toElement().elementsByTagName("ElementStop");

		//ElementStop
		scan.m_ElementStop = node1.item(0).toElement().text().toInt();

		//获取节点<SendFocus></SendFocus>
		node1 = node.item(0).toElement().elementsByTagName("SendFocus");

		//SendFocus
		scan.m_SendFocus = node1.item(0).toElement().text().toDouble();

		//获取节点<RecvFocusNum></RecvFocusNum>
		node1 = node.item(0).toElement().elementsByTagName("RecvFocusNum");

		//RecvFocusNum
		scan.m_RecvFocusNum = node1.item(0).toElement().text().toInt();

		//获取节点<RecvFocusStart></RecvFocusStart>
		node1 = node.item(0).toElement().elementsByTagName("RecvFocusStart");

		//RecvFocusStart
		scan.m_RecvFocusStart = node1.item(0).toElement().text().toDouble();

		//获取节点<RecvFocusEnd></RecvFocusEnd>
		node1 = node.item(0).toElement().elementsByTagName("RecvFocusEnd");

		//RecvFocusEnd
		scan.m_RecvFocusEnd = node1.item(0).toElement().text().toDouble();

		//获取节点<Reflect></Reflect>
		node1 = node.item(0).toElement().elementsByTagName("Reflect");

		//Reflect
		scan.m_Reflect = node1.item(0).toElement().text().toInt();

		//获取节点<VelocityType></VelocityType>
		node1 = node.item(0).toElement().elementsByTagName("VelocityType");

		QMap<QString, VelocityType> velocitymap;
		velocitymap["Longitudinal"] = VelocityType::eLongitudinal;
		velocitymap["Shear"] = VelocityType::eShear;

		//VelocityType
		scan.m_VelocityType = velocitymap[node1.item(0).toElement().text()];


		//获取节点<Type></Type>
		node1 = node.item(0).toElement().elementsByTagName("Type");

		QMap<QString, ScanType> scanmap;
		scanmap["PAAngle"] = ScanType::ePAAngle;
		scanmap["PALinear"] = ScanType::ePALinear;
		scanmap["TOFD"] = ScanType::eTOFD;

		//Type
		scan.m_Type = scanmap[node1.item(0).toElement().text()];
	}

	return scan;
}

//获得某个组的位置信息
PositionInfo CSoftwareUTInfoReader::GetPositionInfo(const int& GroupID) const
{
	PositionInfo position;

	QMap<QString, InspectPos> posmap;

	posmap["WeldPDetectPos_Up"] = InspectPos::WeldPDetectPos_Up;
	posmap["WeldPDetectPos_Down"] = InspectPos::WeldPDetectPos_Down;
	posmap["WeldPDetectPos_UpL"] = InspectPos::WeldPDetectPos_UpL;
	posmap["WeldPDetectPos_UpR"] = InspectPos::WeldPDetectPos_UpR;
	posmap["WeldPDetectPos_DownL"] = InspectPos::WeldPDetectPos_DownL;
	posmap["WeldPDetectPos_DownR"] = InspectPos::WeldPDetectPos_DownR;
	posmap["WeldFDetectPos_WebUp"] = InspectPos::WeldFDetectPos_WebUp;
	posmap["WeldFDetectPos_WebDown"] = InspectPos::WeldFDetectPos_WebDown;
	posmap["WeldFDetectPos_MomUpL"] = InspectPos::WeldFDetectPos_MomUpL;
	posmap["WeldFDetectPos_MomUpR"] = InspectPos::WeldFDetectPos_MomUpR;
	posmap["WeldFDetectPos_MomDown"] = InspectPos::WeldFDetectPos_MomDown;

	GroupBasicInfo group;
	if (m_bReadEnable)
	{
		QString str;
		//获取节点<GroupID></GroupID>
		str = QString("Group%0").arg(GroupID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<InspectPos></InspectPos>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("InspectPos");

		position.m_InspectPos = posmap[node1.item(0).toElement().text()];

		//获取节点<WedgeOffset></WedgeOffset>
		node1 = node.item(0).toElement().elementsByTagName("WedgeOffset");

		//WedgetOffset
		position.m_WedgeOffset = node1.item(0).toElement().text().toDouble();
	}

	return position;
}

//获得某个组采集信息
GroupBasicInfo CSoftwareUTInfoReader::GetGroupBasicInfo(const int& GroupID) const
{
	GroupBasicInfo group;
	if (m_bReadEnable)
	{
		QString str;
		//获取节点<GroupID></GroupID>
		str = QString("Group%0").arg(GroupID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<ScanType></ScanType>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("InspectPos");
		QMap<QString, ScanType> scantypemap;
		scantypemap["PA"] = ScanType::ePA;
		scantypemap["TOFD"] = ScanType::eTOFD;
		scantypemap["Special"] = ScanType::eSpecial;
		group.m_ScanType = scantypemap[node1.item(0).toElement().text()];
		

		//获取节点<TRMode></TRMode>
		node1 = node.item(0).toElement().elementsByTagName("InspectPos");
		QMap<QString, TRMode> trmap;
		trmap["TransmitReceive"] = TRMode::eTransmitReceive;
		trmap["Transmit"] = TRMode::eTransmit;
		group.m_TRMode = trmap[node1.item(0).toElement().text()];

		//获取节点<ChannelNum></ChannelNum>
		node1 = node.item(0).toElement().elementsByTagName("ChannelNum");

		//ChannelNum
		group.m_ChannelNum = node1.item(0).toElement().text().toInt();

		//获取节点<PartVelocity></PartVelocity>
		node1 = node.item(0).toElement().elementsByTagName("PartVelocity");

		//PartVelocity
		group.m_PartVelocity = node1.item(0).toElement().text().toDouble();

		//获取节点<SendFocus></SendFocus>
		node1 = node.item(0).toElement().elementsByTagName("SendFocus");

		//SendFocus
		group.m_SendFocus = node1.item(0).toElement().text().toDouble();

		//获取节点<RecvFocusNum></RecvFocusNum>
		node1 = node.item(0).toElement().elementsByTagName("RecvFocusNum");

		//RecvFocusNum
		group.m_RecvFocusNum = node1.item(0).toElement().text().toInt();

		//获取节点<RecvFocusStart></RecvFocusStart>
		node1 = node.item(0).toElement().elementsByTagName("RecvFocusStart");

		//RecvFocusStart
		group.m_RecvFocusStart = node1.item(0).toElement().text().toDouble();

		//获取节点<RecvFocusEnd></RecvFocusEnd>
		node1 = node.item(0).toElement().elementsByTagName("RecvFocusEnd");

		//RecvFocusEnd
		group.m_RecvFocusEnd = node1.item(0).toElement().text().toDouble();

		group.m_DigitizingDelay = 0;

		group.m_TRMode = TRMode::eTransmitReceive;
	}
	return group;
}

//获得某个组的声束信息
BeamBasicInfo CSoftwareUTInfoReader::GetBeamBasicInfo(const int& GroupID, const int& ChannelID)
{
	BeamBasicInfo beam;
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<GroupID></GroupID>
		str = QString("Group%0").arg(GroupID);
		QDomNodeList node = root.elementsByTagName(str);
		
		//获取节点<ChannelID></ChannelID>
		str = QString("Channel%0").arg(ChannelID);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<ScanAngle></ScanAngle>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("ScanAngle");

		//ScanAngle
		beam.m_BeamAngle = node1.item(0).toElement().text().toDouble();

		//获取节点<CenterElement></CenterElement>
		node1 = node.item(0).toElement().elementsByTagName("CenterElement");

		//CenterElement
		beam.m_CenterElementID = node1.item(0).toElement().text().toInt();

		//获取节点<WedgeDelay></WedgeDelay>
		node1 = node.item(0).toElement().elementsByTagName("WedgeDelay");

		//WedgeDelay
		beam.m_WedgeDelay = node1.item(0).toElement().text().toDouble();
	}
	return beam;
}

//获得闸门信息
GateInfo CSoftwareUTInfoReader::GetGateInfo(const int& GroupID, const int& GateID)
{
	GateInfo gate;
	if (m_bReadEnable)
	{
		QString str;

		//获取节点<GroupID></GroupID>
		str = QString("Group%0").arg(GroupID);
		QDomNodeList node = root.elementsByTagName(str);

		//获取节点<GateID></GateID>
		str = QString("GateID%0").arg(GateID);
		node = node.item(0).toElement().elementsByTagName(str);

		//获取节点<GateIsUsed></GateIsUsed>
		QDomNodeList node1 = node.item(0).toElement().elementsByTagName("GateIsUsed");

		//GateIsUsed
		gate.m_GateIsUsed = (bool)node1.item(0).toElement().text().toInt();

		//获取节点<GateType></GateType>
		node1 = node.item(0).toElement().elementsByTagName("GateType");

		//GateType
		gate.m_GateType = node1.item(0).toElement().text().toInt();

		//获取节点<GateStart></GateStart>
		node1 = node.item(0).toElement().elementsByTagName("GateStart");

		//GateStart
		gate.m_GateStart = node1.item(0).toElement().text().toDouble();

		//获取节点<GateRange></GateRange>
		node1 = node.item(0).toElement().elementsByTagName("GateRange");

		//GateRange
		gate.m_GateRange = node1.item(0).toElement().text().toDouble();

		//获取节点<GateThreshold></GateThreshold>
		node1 = node.item(0).toElement().elementsByTagName("GateThreshold");

		//GateThreshold
		gate.m_GateThreshold = node1.item(0).toElement().text().toDouble();
	}
	return gate;
}

//获得编码器信息
EncoderInfo CSoftwareUTInfoReader::GetEncoderInfo() const
{
	EncoderInfo encoder;

	if (m_bReadEnable)
	{
		//获取节点<PointNumOfScanAxis></PointNumOfScanAxis>
		QDomNodeList node = root.elementsByTagName("PointNumOfScanAxis");

		//PointNumOfScanAxis
		encoder.m_ScanPointNum = node.item(0).toElement().text().toInt();

		//获取节点<ScanAxisStart></ScanAxisStart>
		node = root.elementsByTagName("ScanAxisStart");

		//ScanAxisStart
		encoder.m_ScanStart = node.item(0).toElement().text().toDouble();

		//获取节点<ScanAxisRange></ScanAxisRange>
		node = root.elementsByTagName("ScanAxisRange");

		//ScanAxisRange
		encoder.m_ScanRange = node.item(0).toElement().text().toDouble();

		//获取节点<PointNumOfIndexAxis></PointNumOfIndexAxis>
		node = root.elementsByTagName("PointNumOfIndexAxis");

		//ScanAxisRange
		encoder.m_IndexPointNum = node.item(0).toElement().text().toInt();

		//获取节点<IndexAxisStart></IndexAxisStart>
		node = root.elementsByTagName("IndexAxisStart");

		//IndexAxisStart
		encoder.m_IndexStart = node.item(0).toElement().text().toDouble();

		//获取节点<IndexAxisRange></IndexAxisRange>
		node = root.elementsByTagName("IndexAxisRange");

		//IndexAxisStart
		encoder.m_IndexRange = node.item(0).toElement().text().toDouble();

		//获取节点<Encoder1Resolution></Encoder1Resolution>
		node = root.elementsByTagName("Encoder1Resolution");

		//Encoder1Resolution
		encoder.m_EncoderResolution[0] = node.item(0).toElement().text().toInt();

		//获取节点<Encoder2Resolution></Encoder2Resolution>
		node = root.elementsByTagName("Encoder2Resolution");

		//Encoder2Resolution
		encoder.m_EncoderResolution[1] = node.item(0).toElement().text().toInt();

		//获取节点<Encoder1Porlarity></Encoder1Porlarity>
		node = root.elementsByTagName("Encoder1Porlarity");

		//Encoder1Porlarity
		encoder.m_EncoderPolarity[0] = node.item(0).toElement().text().toInt();

		//获取节点<Encoder2Porlarity></Encoder2Porlarity>
		node = root.elementsByTagName("Encoder2Porlarity");

		//Encoder2Porlarity
		encoder.m_EncoderPolarity[1] = node.item(0).toElement().text().toInt();

		//获取节点<ScanAxisID></ScanAxisID>
		node = root.elementsByTagName("ScanAxisID");

		//Encoder2Porlarity
		encoder.m_ScanAxisID = node.item(0).toElement().text().toInt();

		encoder.m_ScanStep = (encoder.m_ScanPointNum > 1) ? encoder.m_ScanRange / (encoder.m_ScanPointNum - 1) : 1;
		encoder.m_IndexStep = (encoder.m_IndexPointNum > 1) ? encoder.m_IndexRange / (encoder.m_IndexPointNum - 1) : 1;
	}

	return encoder;
}

//获得滤波器信息
FilterInfo CSoftwareUTInfoReader::GetFilterInfo() const
{
	FilterInfo filter;

	if (m_bReadEnable)
	{
		//获取节点<FilterType></FilterType>
		QDomNodeList node = root.elementsByTagName("FilterType");

		//FilterType
		QMap<QString, FilterType> filtermap;
		filtermap["No Filter"] = FilterType::eNoFilter;
		filtermap["Band Filter"] = FilterType::eBandPassFilter;
		filtermap["Lowpass Filter"] = FilterType::eLowPassFilter;
		filtermap["Envelop Filter"] = FilterType::eEnvellopFilter;
		filter.m_FilterType = filtermap[node.item(0).toElement().text()];

		//获取节点<Freq1></Freq1>
		node = root.elementsByTagName("Freq1");

		//Freq1
		filter.m_Frequency1 = node.item(0).toElement().text().toDouble();

		//获取节点<Freq2></Freq2>
		node = root.elementsByTagName("Freq2");

		//Freq2
		filter.m_Frequency2 = node.item(0).toElement().text().toDouble();
	}

	return filter;
}


//获得试件类型
PartType CSoftwareUTInfoReader::GetPartType()
{
	PartType type;

	QMap<QString, PartType> typemap;
	typemap["Plate Weld"] = PartType::PWeld;
	typemap["Fillet Weld"] = PartType::FWeld;

	QString str;

	if (m_bReadEnable)
	{
		//获取节点<PartType></PartType>
		QDomNodeList node = root.elementsByTagName("PartType");

		//PartType
		type = typemap[node.item(0).toElement().text()];
	}

	return type;
}

//获得平板对接试件的信息
void CSoftwareUTInfoReader::GetPlateWeldInfo(PlateWeldInfo& plate, GrooveInfo& groove)
{
	QString str;

	if (m_bReadEnable)
	{
		QString str;

		//获取节点<MotherThickness></MotherThickness>
		QDomNodeList node = root.elementsByTagName("MotherThickness");
		//MotherThickness
		plate.m_MotherThickness = node.item(0).toElement().text().toDouble();
		
		//获取节点<MotherWidth></MotherWidth>
		node = root.elementsByTagName("MotherWidth");
		//MotherWidth
		plate.m_MotherWidth = node.item(0).toElement().text().toDouble();

		//获取节点<UpWeldWidth></UpWeldWidth>
		node = root.elementsByTagName("UpWeldWidth");
		//UpWeldWidth
		plate.m_UpWeldWidth = node.item(0).toElement().text().toDouble();

		//获取节点<UpWeldHeight></UpWeldHeight>
		node = root.elementsByTagName("UpWeldHeight");
		//UpWeldHeight
		plate.m_UpWeldHeight = node.item(0).toElement().text().toDouble();

		//获取节点<DownWeldWidth></DownWeldWidth>
		node = root.elementsByTagName("DownWeldWidth");
		//DownWeldWidth
		plate.m_DownWeldWidth = node.item(0).toElement().text().toDouble();

		//获取节点<DownWeldHeight></DownWeldHeight>
		node = root.elementsByTagName("DownWeldHeight");
		//DownWeldHeight
		plate.m_DownWeldHeight = node.item(0).toElement().text().toDouble();

		QMap<QString, GrooveType> groovemap;
		groovemap["GrooveI"] = GrooveType::GrooveI;
		groovemap["GrooveV"] = GrooveType::GrooveV;
		groovemap["GrooveVV"] = GrooveType::GrooveVV;
		groovemap["GrooveY"] = GrooveType::GrooveY;
		groovemap["GrooveYY"] = GrooveType::GrooveYY;
		//获取节点<GrooveType></GrooveType>
		node = root.elementsByTagName("GrooveType");
		groove.m_GrooveType = groovemap[node.item(0).toElement().text()];

		//获取节点<GrooveIb></GrooveIb>
		node = root.elementsByTagName("GrooveIb");
		//GrooveIb
		groove.m_GrooveIb = node.item(0).toElement().text().toDouble();

		//获取节点<GrooveYb></GrooveYb>
		node = root.elementsByTagName("GrooveYb");
		//GrooveYb
		groove.m_GrooveYb = node.item(0).toElement().text().toDouble();

		//获取节点<GrooveYP></GrooveYP>
		node = root.elementsByTagName("GrooveYP");
		//GrooveYP
		groove.m_GrooveYP = node.item(0).toElement().text().toDouble();

		//获取节点<GrooveVa></GrooveVa>
		node = root.elementsByTagName("GrooveVa");
		//GrooveVa
		groove.m_GrooveVa = node.item(0).toElement().text().toDouble();

		//获取节点<GrooveVb></GrooveVb>
		node = root.elementsByTagName("GrooveVb");
		//GrooveVb
		groove.m_GrooveVb = node.item(0).toElement().text().toDouble();

		//获取节点<GrooveYYb></GrooveYYb>
		node = root.elementsByTagName("GrooveYYb");
		//GrooveYYb
		groove.m_GrooveYYb = node.item(0).toElement().text().toDouble();

		//获取节点<GrooveYYP></GrooveYYP>
		node = root.elementsByTagName("GrooveYYP");
		//GrooveYYP
		groove.m_GrooveYYP = node.item(0).toElement().text().toDouble();

		//获取节点<GrooveVVb></GrooveVVb>
		node = root.elementsByTagName("GrooveVVb");
		//GrooveVVb
		groove.m_GrooveVVb = node.item(0).toElement().text().toDouble();

		//获取节点<GrooveVVH></GrooveVVH>
		node = root.elementsByTagName("GrooveVVH");
		//GrooveVVH
		groove.m_GrooveVVH = node.item(0).toElement().text().toDouble();
	}
}

//获得角接试件的信息
void CSoftwareUTInfoReader::GetFilletWeldInfo(FilletWeldInfo& fillet)
{
	QString str;

	if (m_bReadEnable)
	{
		//获取节点<MotherWidth></MotherWidth>
		QDomNodeList node = root.elementsByTagName("MotherWidth");
		//MotherWidth
		fillet.m_MotherWidth = node.item(0).toElement().text().toDouble();

		//获取节点<MotherThickness></MotherThickness>
		node = root.elementsByTagName("MotherThickness");
		//MotherThickness
		fillet.m_MotherThickness = node.item(0).toElement().text().toDouble();

		//获取节点<WebAngle></WebAngle>
		node = root.elementsByTagName("WebAngle");
		//WebAngle
		fillet.m_WebAngle = node.item(0).toElement().text().toDouble();

		//获取节点<WebWidth></WebWidth>
		node = root.elementsByTagName("WebWidth");
		//WebWidth
		fillet.m_WebWidth = node.item(0).toElement().text().toDouble();

		//获取节点<WebThickness></WebThickness>
		node = root.elementsByTagName("WebThickness");
		//WebThickness
		fillet.m_WebThickness = node.item(0).toElement().text().toDouble();

		//获取节点<BlunteEdgeOffset></BlunteEdgeOffset>
		node = root.elementsByTagName("BlunteEdgeOffset");
		//BlunteEdgeOffset
		fillet.m_BlunteEdgeOffset = node.item(0).toElement().text().toDouble();

		//获取节点<BlunteEdgeHeight></BlunteEdgeHeight>
		node = root.elementsByTagName("BlunteEdgeHeight");
		//BlunteEdgeHeight
		fillet.m_BlunteEdgeHeight = node.item(0).toElement().text().toDouble();

		//获取节点<BlunteEdgeWidth></BlunteEdgeWidth>
		node = root.elementsByTagName("BlunteEdgeWidth");
		//BlunteEdgeWidth
		fillet.m_BlunteEdgeWidth = node.item(0).toElement().text().toDouble();

		//获取节点<LeftWeldAngle></LeftWeldAngle>
		node = root.elementsByTagName("LeftWeldAngle");
		//LeftWeldAngle
		fillet.m_LeftWeldAngle = node.item(0).toElement().text().toDouble();

		//获取节点<RightWeldAngle></RightWeldAngle>
		node = root.elementsByTagName("RightWeldAngle");
		//RightWeldAngle
		fillet.m_RightWeldAngle = node.item(0).toElement().text().toDouble();

		//获取节点<LeftWeldWidth></LeftWeldWidth>
		node = root.elementsByTagName("LeftWeldWidth");
		//LeftWeldWidth
		fillet.m_LeftWeldWidth = node.item(0).toElement().text().toDouble();

		//获取节点<RightWeldWidth></RightWeldWidth>
		node = root.elementsByTagName("RightWeldWidth");
		//RightWeldWidth
		fillet.m_RightWeldWidth = node.item(0).toElement().text().toDouble();
	}
}