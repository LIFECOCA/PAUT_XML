﻿#include "SoftwareUTInfoReader.h"

#import "msxml6.dll"

#include <map>
#include <string>

#pragma warning(disable: 4996)

struct CSoftwareUTInfoReader::Impl
{
	//公有构造函数
	Impl(const std::string& name);

	//公有析构函数
	~Impl();

	//获得总组数
	int GetGroupNum() const;

	//获得材料信息
	MaterialInfo GetMaterialInfo() const;

	//获得某个组的探头信息
	ProbeInfo GetProbeInfo(const int& GroupID) const;

	//获得某个组的楔块信息
	WedgeInfo GetWedgeInfo(const int& GroupID) const;

	//获得某个组的扫查信息
	ScanInfo GetScanInfo(const int& GroupID) const;

	//获得某个组的位置信息
	PositionInfo GetPositionInfo(const int& GroupID) const;

	//获得某个组采集信息
	GroupBasicInfo GetGroupBasicInfo(const int& GroupID) const;

	//获得某个组的声束信息
	BeamBasicInfo GetBeamBasicInfo(const int& GroupID, const int& ChannelID);

	//获得某个组的ACG信息
	void GetACGInfo(double& delay, double& gain, const int& GroupID, const int& ChannelID);

	//获得闸门信息
	GateInfo GetGateInfo(const int& GroupID, const int& GateID);

	//获得编码器信息
	EncoderInfo GetEncoderInfo() const;

	//获得滤波器信息
	FilterInfo GetFilterInfo() const;

	//获得试件类型
	PartType GetPartType();

	//获得平板对接试件的信息
	void GetPlateWeldInfo(PlateWeldInfo& plate, GrooveInfo& groove);

	//获得角接试件的信息
	void GetFilletWeldInfo(FilletWeldInfo& fillet);

	//XML文档节点
	MSXML2::IXMLDOMDocumentPtr pDoc;

	//XML根节点
	MSXML2::IXMLDOMElementPtr pRoot;

	//标志是否读取成功文档的变量
	bool m_bReadEnable;
};

//公有构造函数
CSoftwareUTInfoReader::CSoftwareUTInfoReader(const char* name)
{
	m_pImpl = new Impl(name);
}

//公有析构函数
CSoftwareUTInfoReader::~CSoftwareUTInfoReader()
{
	delete m_pImpl;
}

//获得总组数
int CSoftwareUTInfoReader::GetGroupNum() const
{
	return m_pImpl->GetGroupNum();
}

//获得材料信息
MaterialInfo CSoftwareUTInfoReader::GetMaterialInfo() const
{
	return m_pImpl->GetMaterialInfo();
}

//获得某个组的探头信息
ProbeInfo CSoftwareUTInfoReader::GetProbeInfo(const int& GroupID) const
{
	return m_pImpl->GetProbeInfo(GroupID);
}

//获得某个组的楔块信息
WedgeInfo CSoftwareUTInfoReader::GetWedgeInfo(const int& GroupID) const
{
	return m_pImpl->GetWedgeInfo(GroupID);
}

//获得某个组的扫查信息
ScanInfo CSoftwareUTInfoReader::GetScanInfo(const int& GroupID) const
{
	return m_pImpl->GetScanInfo(GroupID);
}

//获得某个组的位置信息
PositionInfo CSoftwareUTInfoReader::GetPositionInfo(const int& GroupID) const
{
	return m_pImpl->GetPositionInfo(GroupID);
}

//获得某个组采集信息
GroupBasicInfo CSoftwareUTInfoReader::GetGroupBasicInfo(const int& GroupID) const
{
	return m_pImpl->GetGroupBasicInfo(GroupID);
}

//获得某个组的声束信息
BeamBasicInfo CSoftwareUTInfoReader::GetBeamBasicInfo(const int& GroupID, const int& ChannelID)
{
	return m_pImpl->GetBeamBasicInfo(GroupID, ChannelID);
}

//获得某个组的ACG信息
void CSoftwareUTInfoReader::GetACGInfo(double& delay, double& gain, const int& GroupID, const int& ChannelID)
{
	return m_pImpl->GetACGInfo(delay, gain, GroupID, ChannelID);
}

//获得闸门信息
GateInfo CSoftwareUTInfoReader::GetGateInfo(const int& GroupID, const int& GateID)
{
	return m_pImpl->GetGateInfo(GroupID, GateID);
}

//获得编码器信息
EncoderInfo CSoftwareUTInfoReader::GetEncoderInfo() const
{
	return m_pImpl->GetEncoderInfo();
}

//获得滤波器信息
FilterInfo CSoftwareUTInfoReader::GetFilterInfo() const
{
	return m_pImpl->GetFilterInfo();
}

//获得试件类型
PartType CSoftwareUTInfoReader::GetPartType()
{
	return m_pImpl->GetPartType();
}

//获得平板对接试件的信息
void CSoftwareUTInfoReader::GetPlateWeldInfo(PlateWeldInfo& plate, GrooveInfo& groove)
{
	m_pImpl->GetPlateWeldInfo(plate, groove);
}

//获得角接试件的信息
void CSoftwareUTInfoReader::GetFilletWeldInfo(FilletWeldInfo& fillet)
{
	m_pImpl->GetFilletWeldInfo(fillet);
}

//公有构造函数
CSoftwareUTInfoReader::Impl::Impl(const std::string& name)
{
	CoInitialize(NULL);

	m_bReadEnable = false;

	//创建根节点
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (FAILED(hr))
	{
		printf("Create Failed\n");
		return;
	}

	//加载PoolInitInfo文档
	VARIANT_BOOL bXmlLoad = pDoc->load(name.c_str());
	if (!bXmlLoad) // 加载失败
	{
		printf("Load Failed\n");
		return;
	}

	//获得根节点
	pRoot = pDoc->GetdocumentElement();

	m_bReadEnable = true;
}

//公有析构函数
CSoftwareUTInfoReader::Impl::~Impl()
{
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//获得总组数
int CSoftwareUTInfoReader::Impl::GetGroupNum() const
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		//获取节点<GroupNum></GroupNum>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GroupNum")->GetfirstChild();

		//返回GroupNum
		return atoi((char*)_bstr_t(pNode->GetnodeValue()));
	}
	return -1;
}

//获得材料信息
MaterialInfo CSoftwareUTInfoReader::Impl::GetMaterialInfo() const
{
	MaterialInfo material;

	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		//获取节点<MaterialID></MaterialID>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"MaterialID")->GetfirstChild();

		//MaterialID
		material.m_MaterialID = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<MaterialName></MaterialName>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"MaterialName")->GetfirstChild();

		//MaterialName
		strcpy(material.m_MaterialName, (char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<LongitudinalVelocity></LongitudinalVelocity>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"LongitudinalVelocity")->GetfirstChild();

		//LongitudinalVelocity
		material.m_LongitudinalVelocity = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<ShearVelocity></ShearVelocity>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"ShearVelocity")->GetfirstChild();

		//ShearVelocity
		material.m_ShearVelocity = atof((char*)_bstr_t(pNode->GetnodeValue()));
	}

	return material;
}

//获得某个组的探头信息
ProbeInfo CSoftwareUTInfoReader::Impl::GetProbeInfo(const int& GroupID) const
{
	ProbeInfo probe;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;
		char str[100];
		//获取节点<GroupID></GroupID>
		sprintf(str, "Group%d", GroupID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<ProbeID></ProbeID>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbeID")->GetfirstChild();

		//ProbeID
		probe.m_ProbeID = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ProbeName></ProbeName>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbeName")->GetfirstChild();

		//ProbeName
		strcpy(probe.m_ProbeName, (char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ProbePitch></ProbePitch>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbePitch")->GetfirstChild();

		//ProbePitch
		probe.m_Pitch = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ElementNum></ElementNum>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ElementNum")->GetfirstChild();

		//ElementNum
		probe.m_ElementNum = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ProbeFrequency></ProbeFrequency>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbeFrequency")->GetfirstChild();

		//ProbeFrequency
		probe.m_Frequency = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ProbeHeight></ProbeHeight>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbeHeight")->GetfirstChild();

		//ProbeHeight
		probe.m_Height = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ProbeD></ProbeD>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbeD")->GetfirstChild();

		//ProbeD
		probe.m_D = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ProbeType></ProbeType>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbeType")->GetfirstChild();

		std::string str1 = (char*)_bstr_t(pNode1->GetnodeValue());
		if (str1 == "Linear")
		{
			probe.m_ProbeType = ProbeType::eLinear;
		}
		else if (str1 == "Arc")
		{
			probe.m_ProbeType = ProbeType::eArc;
		}
		else if (str1 == "Ring")
		{
			probe.m_ProbeType = ProbeType::eRing;
		}

		//获取节点<ProbeRadius></ProbeRadius>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbeRadius")->GetfirstChild();

		//ProbeRadius
		probe.m_Radius = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<FirstConnection></FirstConnection>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"FirstConnection")->GetfirstChild();

		//FirstConnection
		probe.m_FirstConnection = atoi((char*)_bstr_t(pNode1->GetnodeValue()));
	}
	return probe;
}

//获得某个组的楔块信息
WedgeInfo CSoftwareUTInfoReader::Impl::GetWedgeInfo(const int& GroupID) const
{
	WedgeInfo wedge;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;

		char str[100];
		//获取节点<GroupID></GroupID>
		sprintf(str, "Group%d", GroupID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<WedgeID></WedgeID>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"WedgeID")->GetfirstChild();

		//WedgeID
		wedge.m_WedgeID = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<WedgeName></WedgeName>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"WedgeName")->GetfirstChild();

		//WedgeName
		strcpy(wedge.m_WedgeName, (char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<WedgeAngle></WedgeAngle>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"WedgeAngle")->GetfirstChild();

		//WedgeAngle
		wedge.m_WedgeAngle = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<WedgeVelocity></WedgeVelocity>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"WedgeVelocity")->GetfirstChild();

		//WedgeVelocity
		wedge.m_Velocity = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ProbeOffsetX></ProbeOffsetX>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbeOffsetX")->GetfirstChild();

		//ProbeOffesetX
		wedge.m_ProbeOffsetX = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ProbeOffsetY></ProbeOffsetY>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ProbeOffsetY")->GetfirstChild();

		//ProbeOffesetY
		wedge.m_ProbeOffsetY = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<WedgeLength></WedgeLength>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"WedgeLength")->GetfirstChild();

		//WedgeLength
		wedge.m_w1 = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<WedgeHeight></WedgeHeight>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"WedgeHeight")->GetfirstChild();

		//WedgeHeight
		wedge.m_h1 = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<OtherInfoNum></OtherInfoNum>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"OtherInfoNum")->GetfirstChild();

		wedge.m_h2 = wedge.m_ProbeOffsetY - wedge.m_ProbeOffsetX * tan(wedge.m_WedgeAngle / 45 * atan(1.0));

		int num = atoi((char*)_bstr_t(pNode1->GetnodeValue()));
		for (int i = 0; i < num; i++)
		{
			sprintf(str, "Info%d", i + 1);
			pNode1 = pNode->selectSingleNode((_bstr_t)str)->GetfirstChild();
			wedge.m_OtherInfo.push_back(atof((char*)_bstr_t(pNode1->GetnodeValue())));
		}
	}
	return wedge;
}

//获得某个组的扫查信息
ScanInfo CSoftwareUTInfoReader::Impl::GetScanInfo(const int& GroupID) const
{
	ScanInfo scan;

	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;

		char str[100];

		//获取节点<GroupID></GroupID>
		sprintf(str, "Group%d", GroupID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<AngleStart></AngleStart>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"AngleStart")->GetfirstChild();

		//AngleStart
		scan.m_AngleStart = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<AngleStop></AngleStop>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"AngleStop")->GetfirstChild();

		//AngleStart
		scan.m_AngleStop = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ChannelNum></ChannelNum>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ChannelNum")->GetfirstChild();

		//ChannelNum
		scan.m_ChannelNum = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ChannelSize></ChannelSize>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ChannelSize")->GetfirstChild();

		//ChannelSize
		scan.m_ChannelSize = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ElementStart></ElementStart>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ElementStart")->GetfirstChild();

		//ElementStart
		scan.m_ElementStart = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<ElementStop></ElementStop>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ElementStop")->GetfirstChild();

		//ElementStop
		scan.m_ElementStop = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<SendFocus></SendFocus>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"SendFocus")->GetfirstChild();

		//SendFocus
		scan.m_SendFocus = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<RecvFocusNum></RecvFocusNum>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"RecvFocusNum")->GetfirstChild();

		//RecvFocusNum
		scan.m_RecvFocusNum = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<RecvFocusStart></RecvFocusStart>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"RecvFocusStart")->GetfirstChild();

		//RecvFocusStart
		scan.m_RecvFocusStart = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<RecvFocusEnd></RecvFocusEnd>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"RecvFocusEnd")->GetfirstChild();

		//RecvFocusEnd
		scan.m_RecvFocusEnd = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<FocalType></FocalType>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"FocalType")->GetfirstChild();

		//FocalType
		scan.m_FocalType = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<Reflect></Reflect>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"Reflect")->GetfirstChild();

		//Reflect
		scan.m_Reflect = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<VelocityType></VelocityType>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"VelocityType")->GetfirstChild();
		std::map<std::string, VelocityType> velocitymap
		{
			{ "Longitudinal", VelocityType::eLongitudinal },
			{ "Shear", VelocityType::eShear }
		};
		//VelocityType
		scan.m_VelocityType = velocitymap[(char*)_bstr_t(pNode1->GetnodeValue())];
		//获取节点<Type></Type>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"Type")->GetfirstChild();
		std::map<std::string, ScanType> scanmap
		{
			{ "PAAngle", ScanType::ePAAngle },
			{ "PALinear", ScanType::ePALinear },
			{ "TOFD", ScanType::eTOFD },
			{ "Ring", ScanType::eRing }
		};
		//Type
		scan.m_Type = scanmap[(char*)_bstr_t(pNode1->GetnodeValue())];
	}

	return scan;
}

//获得某个组的位置信息
PositionInfo CSoftwareUTInfoReader::Impl::GetPositionInfo(const int& GroupID) const
{
	PositionInfo position;
	std::map<std::string, InspectPos> posmap
	{
		{ "WeldPDetectPos_Up", InspectPos::WeldPDetectPos_Up },
		{ "WeldPDetectPos_Down", InspectPos::WeldPDetectPos_Down },
		{ "WeldPDetectPos_UpL", InspectPos::WeldPDetectPos_UpL },
		{ "WeldPDetectPos_UpR", InspectPos::WeldPDetectPos_UpR },
		{ "WeldPDetectPos_DownL", InspectPos::WeldPDetectPos_DownL },
		{ "WeldPDetectPos_DownR", InspectPos::WeldPDetectPos_DownR },
		{ "WeldFDetectPos_WebUp", InspectPos::WeldFDetectPos_WebUp },
		{ "WeldFDetectPos_WebDown", InspectPos::WeldFDetectPos_WebDown },
		{ "WeldFDetectPos_MomUpL", InspectPos::WeldFDetectPos_MomUpL },
		{ "WeldFDetectPos_MomUpR", InspectPos::WeldFDetectPos_MomUpR },
		{ "WeldFDetectPos_MomDown", InspectPos::WeldFDetectPos_MomDown }
	};

	//GroupBasicInfo group;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;

		char str[100];
		std::string str1;

		//获取节点<GroupID></GroupID>
		sprintf(str, "Group%d", GroupID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<InspectPos></InspectPos>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"InspectPos")->GetfirstChild();
		str1 = (char*)_bstr_t(pNode1->GetnodeValue());

		position.m_InspectPos = posmap[str1];

		//获取节点<WedgetOffset></WedgetOffset>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"WedgeOffset")->GetfirstChild();

		//PartVelocity
		position.m_WedgeOffset = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		position.m_TofdCenterOffset = 0;
	}

	return position;
}

//获得某个组采集信息
GroupBasicInfo CSoftwareUTInfoReader::Impl::GetGroupBasicInfo(const int& GroupID) const
{
	GroupBasicInfo group;
	if (m_bReadEnable)
	{
		std::map<std::string, ScanType> scanmap
		{
			{ "PA", ScanType::ePA },
			{ "TOFD", ScanType::eTOFD },
			{ "FMC", ScanType::eFMC },
			{ "Ring", ScanType::eRing },
			{ "Special", ScanType::eSpecial }
		};

		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;

		char str[100];
		std::string str1;

		//获取节点<GroupID></GroupID>
		sprintf(str, "Group%d", GroupID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<ScanType></ScanType>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ScanType")->GetfirstChild();
		str1 = (char*)_bstr_t(pNode1->GetnodeValue());
		group.m_ScanType = scanmap[str1];

		//获取节点<TRMode></TRMode>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"TRMode")->GetfirstChild();
		str1 = (char*)_bstr_t(pNode1->GetnodeValue());
		if (str1 == "TransmitReceive")
		{
			group.m_TRMode = TRMode::eTransmitReceive;
		}
		else if (str1 == "Transmit")
		{
			group.m_TRMode = TRMode::eTransmit;
		}

		//获取节点<ChannelNum></ChannelNum>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ChannelNum")->GetfirstChild();

		//ChannelNum
		group.m_ChannelNum = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<PartVelocity></PartVelocity>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"PartVelocity")->GetfirstChild();

		//PartVelocity
		group.m_PartVelocity = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<SendFocus></SendFocus>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"SendFocus")->GetfirstChild();

		//SendFocus
		group.m_SendFocus = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<RecvFocusNum></RecvFocusNum>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"RecvFocusNum")->GetfirstChild();

		//RecvFocusNum
		group.m_RecvFocusNum = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<RecvFocusStart></RecvFocusStart>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"RecvFocusStart")->GetfirstChild();

		//RecvFocusStart
		group.m_RecvFocusStart = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<RecvFocusEnd></RecvFocusEnd>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"RecvFocusEnd")->GetfirstChild();

		//RecvFocusEnd
		group.m_RecvFocusEnd = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<FocalType></FocalType>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"FocalType")->GetfirstChild();

		//FocalType
		group.m_FocalType = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		group.m_DigitizingDelay = 0;

		group.m_WedgeDitizingDelay = 0;

		group.m_TRMode = TRMode::eTransmitReceive;
	}
	return group;
}

//获得某个组的声束信息
BeamBasicInfo CSoftwareUTInfoReader::Impl::GetBeamBasicInfo(const int& GroupID, const int& ChannelID)
{
	BeamBasicInfo beam;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;

		char str[100];

		//获取节点<GroupID></GroupID>
		sprintf(str, "Group%d", GroupID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<ChannelID></ChannelID>
		sprintf(str, "Channel%d", ChannelID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<ScanAngle></ScanAngle>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"ScanAngle")->GetfirstChild();

		//ScanAngle
		beam.m_BeamAngle = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<CenterElement></CenterElement>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"CenterElement")->GetfirstChild();

		//CenterElement
		beam.m_CenterElementID = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<WedgeDelay></WedgeDelay>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"WedgeDelay")->GetfirstChild();

		//返回WedgeDelay
		beam.m_WedgeDelay = atof((char*)_bstr_t(pNode1->GetnodeValue()));
	}
	return beam;
}

//获得某个组的DAC信息
void CSoftwareUTInfoReader::Impl::GetACGInfo(double& delay, double& gain, const int& GroupID, const int& ChannelID)
{
	delay = 0;
	gain = 0;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;

		char str[100];
		//获取节点<GroupID></GroupID>
		sprintf(str, "Group%d", GroupID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<ChannelID></ChannelID>
		sprintf(str, "Channel%d", ChannelID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<DACDelay></DACDelay>
		auto temp1 = pNode->selectSingleNode((_bstr_t)(char*)"ACGDelay");
		if (temp1)
		{
			pNode1 = temp1->GetfirstChild();
			delay = atof((char*)_bstr_t(pNode1->GetnodeValue()));
		}

		//获取节点<DACGain></DACGain>
		auto temp2 = pNode->selectSingleNode((_bstr_t)(char*)"ACGGain");
		if (temp2)
		{
			pNode1 = temp2->GetfirstChild();
			gain = atof((char*)_bstr_t(pNode1->GetnodeValue()));
		}
	}
}

//获得闸门信息
GateInfo CSoftwareUTInfoReader::Impl::GetGateInfo(const int& GroupID, const int& GateID)
{
	GateInfo gate;
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode, pNode1;

		char str[100];
		//获取节点<GroupID></GroupID>
		sprintf(str, "Group%d", GroupID);
		pNode = pRoot->selectSingleNode((_bstr_t)str);

		//获取节点<GateID></GateID>
		sprintf(str, "GateID%d", GateID);
		pNode = pNode->selectSingleNode((_bstr_t)str);

		//获取节点<GateIsUsed></GateIsUsed>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"GateIsUsed")->GetfirstChild();

		//GateIsUsed
		gate.m_GateIsUsed = (atoi((char*)_bstr_t(pNode1->GetnodeValue()))) ? true : false;

		//获取节点<GateType></GateType>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"GateType")->GetfirstChild();

		//GateType
		gate.m_GateType = atoi((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<GateStart></GateStart>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"GateStart")->GetfirstChild();

		//返回GateStart
		gate.m_GateStart = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<GateRange></GateRange>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"GateRange")->GetfirstChild();

		//返回GateRange
		gate.m_GateRange = atof((char*)_bstr_t(pNode1->GetnodeValue()));

		//获取节点<GateThreshold></GateThreshold>
		pNode1 = pNode->selectSingleNode((_bstr_t)(char*)"GateThreshold")->GetfirstChild();

		//返回GateThreshold
		gate.m_GateThreshold = atof((char*)_bstr_t(pNode1->GetnodeValue()));
	}
	return gate;
}

//获得编码器信息
EncoderInfo CSoftwareUTInfoReader::Impl::GetEncoderInfo() const
{
	EncoderInfo encoder;

	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		//获取节点<PointNumOfScanAxis></PointNumOfScanAxis>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"PointNumOfScanAxis")->GetfirstChild();

		//PointNumOfScanAxis
		encoder.m_ScanPointNum = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<ScanAxisStart></ScanAxisStart>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"ScanAxisStart")->GetfirstChild();

		//ScanAxisStart
		encoder.m_ScanStart = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<ScanAxisRange></ScanAxisRange>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"ScanAxisRange")->GetfirstChild();

		//ScanAxisRange
		encoder.m_ScanRange = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<PointNumOfIndexAxis></PointNumOfIndexAxis>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"PointNumOfIndexAxis")->GetfirstChild();

		//PointNumOfIndexAxis
		encoder.m_IndexPointNum = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<IndexAxisStart></IndexAxisStart>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"IndexAxisStart")->GetfirstChild();

		//IndexAxisStart
		encoder.m_IndexStart = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<IndexAxisRange></IndexAxisRange>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"IndexAxisRange")->GetfirstChild();

		//IndexAxisRange
		encoder.m_IndexRange = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<Encoder1Resolution></Encoder1Resolution>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"Encoder1Resolution")->GetfirstChild();

		//Encoder1Resolution
		encoder.m_EncoderResolution[0] = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<Encoder2Resolution></Encoder2Resolution>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"Encoder2Resolution")->GetfirstChild();

		//Encoder2Resolution
		encoder.m_EncoderResolution[1] = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<Encoder1Porlarity></Encoder1Porlarity>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"Encoder1Porlarity")->GetfirstChild();

		//Encoder1Porlarity
		encoder.m_EncoderPolarity[0] = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<Encoder2Porlarity></Encoder2Porlarity>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"Encoder2Porlarity")->GetfirstChild();

		//Encoder2Porlarity
		encoder.m_EncoderPolarity[1] = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<ScanAxisID></ScanAxisID>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"ScanAxisID")->GetfirstChild();

		//ScanAxisID
		encoder.m_ScanAxisID = atoi((char*)_bstr_t(pNode->GetnodeValue()));

		encoder.m_ScanStep = (encoder.m_ScanPointNum > 1) ? encoder.m_ScanRange / (encoder.m_ScanPointNum - 1) : 1;
		encoder.m_IndexStep = (encoder.m_IndexPointNum > 1) ? encoder.m_IndexRange / (encoder.m_IndexPointNum - 1) : 1;
	}

	return encoder;
}

//获得滤波器信息
FilterInfo CSoftwareUTInfoReader::Impl::GetFilterInfo() const
{
	FilterInfo filter;

	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		//获取节点<FilterType></FilterType>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"FilterType")->GetfirstChild();

		//FilterType
		std::map<std::string, FilterType> filtermap
		{
			{ "No Filter", FilterType::eNoFilter },
			{ "Band Filter", FilterType::eBandPassFilter },
			{ "Lowpass Filter", FilterType::eLowPassFilter },
			{ "Envelop Filter", FilterType::eEnvellopFilter },
		};
		filter.m_FilterType = filtermap[(char*)_bstr_t(pNode->GetnodeValue())];

		//获取节点<Freq1></Freq1>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"Freq1")->GetfirstChild();

		//Freq1
		filter.m_Frequency1 = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<Freq2></Freq2>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"Freq2")->GetfirstChild();

		//Freq2
		filter.m_Frequency2 = atof((char*)_bstr_t(pNode->GetnodeValue()));
	}

	return filter;
}


//获得试件类型
PartType CSoftwareUTInfoReader::Impl::GetPartType()
{
	PartType type;
	std::map<std::string, PartType> typemap{ { "Plate Weld", PartType::PWeld }, { "Fillet Weld", PartType::FWeld } };

	std::string str;

	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		//获取节点<PartType></PartType>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"PartType")->GetfirstChild();

		str = (char*)_bstr_t(pNode->GetnodeValue());

		//PartType
		type = typemap[str];
	}

	return type;
}

//获得平板对接试件的信息
void CSoftwareUTInfoReader::Impl::GetPlateWeldInfo(PlateWeldInfo& plate, GrooveInfo& groove)
{
	std::string str;

	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		//获取节点<MotherThickness></MotherThickness>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"MotherThickness")->GetfirstChild();
		//MotherThickness
		plate.m_MotherThickness = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<MotherWidth></MotherWidth>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"MotherWidth")->GetfirstChild();
		//MotherWidth
		plate.m_MotherWidth = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<UpWeldWidth></UpWeldWidth>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"UpWeldWidth")->GetfirstChild();
		//UpWeldWidth
		plate.m_UpWeldWidth = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<UpWeldHeight></UpWeldHeight>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"UpWeldHeight")->GetfirstChild();
		//UpWeldHeight
		plate.m_UpWeldHeight = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<DownWeldWidth></DownWeldWidth>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"DownWeldWidth")->GetfirstChild();
		//DownWeldWidth
		plate.m_DownWeldWidth = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<DownWeldHeight></DownWeldHeight>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"DownWeldHeight")->GetfirstChild();
		//DownWeldHeight
		plate.m_DownWeldHeight = atof((char*)_bstr_t(pNode->GetnodeValue()));

		std::map<std::string, GrooveType> groovemap{ { "GrooveI", GrooveType::GrooveI }, { "GrooveV", GrooveType::GrooveV }, { "GrooveVV", GrooveType::GrooveVV }, { "GrooveY", GrooveType::GrooveY }, { "GrooveYY", GrooveType::GrooveYY } };
		//获取节点<GrooveType></GrooveType>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveType")->GetfirstChild();
		str = (char*)_bstr_t(pNode->GetnodeValue());
		groove.m_GrooveType = groovemap[str];

		//获取节点<GrooveIb></GrooveIb>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveIb")->GetfirstChild();
		//GrooveIb
		groove.m_GrooveIb = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<GrooveYb></GrooveYb>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveYb")->GetfirstChild();
		//GrooveYb
		groove.m_GrooveYb = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<GrooveYP></GrooveYP>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveYP")->GetfirstChild();
		//GrooveYP
		groove.m_GrooveYP = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<GrooveVa></GrooveVa>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveVa")->GetfirstChild();
		//GrooveYb
		groove.m_GrooveVa = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<GrooveVb></GrooveVb>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveVb")->GetfirstChild();
		//GrooveVb
		groove.m_GrooveVb = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<GrooveYYb></GrooveYYb>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveYYb")->GetfirstChild();
		//GrooveYb
		groove.m_GrooveYYb = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<GrooveYYP></GrooveYYP>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveYYP")->GetfirstChild();
		//GrooveYYP
		groove.m_GrooveYYP = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<GrooveVVb></GrooveVVb>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveVVb")->GetfirstChild();
		//GrooveVVb
		groove.m_GrooveVVb = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<GrooveVVH></GrooveVVH>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"GrooveVVH")->GetfirstChild();
		//GrooveVVH
		groove.m_GrooveVVH = atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
}

//获得角接试件的信息
void CSoftwareUTInfoReader::Impl::GetFilletWeldInfo(FilletWeldInfo& fillet)
{
	if (m_bReadEnable)
	{
		//定义节点变量
		MSXML2::IXMLDOMNodePtr pNode = NULL;

		//获取节点<MotherWidth></MotherWidth>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"MotherWidth")->GetfirstChild();
		//MotherWidth
		fillet.m_MotherWidth = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<MotherThickness></MotherThickness>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"MotherThickness")->GetfirstChild();
		//MotherThickness
		fillet.m_MotherThickness = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<WebAngle></WebAngle>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"WebAngle")->GetfirstChild();
		//WebAngle
		fillet.m_WebAngle = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<WebWidth></WebWidth>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"WebWidth")->GetfirstChild();
		//WebWidth
		fillet.m_WebWidth = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<WebThickness></WebThickness>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"WebThickness")->GetfirstChild();
		//WebThickness
		fillet.m_WebThickness = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<BlunteEdgeOffset></BlunteEdgeOffset>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"BlunteEdgeOffset")->GetfirstChild();
		//BlunteEdgeOffset
		fillet.m_BlunteEdgeOffset = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<BlunteEdgeHeight></BlunteEdgeHeight>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"BlunteEdgeHeight")->GetfirstChild();
		//BlunteEdgeHeight
		fillet.m_BlunteEdgeHeight = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<BlunteEdgeWidth></BlunteEdgeWidth>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"BlunteEdgeWidth")->GetfirstChild();
		//BlunteEdgeWidth
		fillet.m_BlunteEdgeWidth = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<LeftWeldAngle></LeftWeldAngle>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"LeftWeldAngle")->GetfirstChild();
		//LeftWeldAngle
		fillet.m_LeftWeldAngle = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<RightWeldAngle></RightWeldAngle>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"RightWeldAngle")->GetfirstChild();
		//RightWeldAngle
		fillet.m_RightWeldAngle = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<LeftWeldWidth></LeftWeldWidth>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"LeftWeldWidth")->GetfirstChild();
		//LeftWeldWidth
		fillet.m_LeftWeldWidth = atof((char*)_bstr_t(pNode->GetnodeValue()));

		//获取节点<RightWeldWidth></RightWeldWidth>
		pNode = pRoot->selectSingleNode((_bstr_t)(char*)"RightWeldWidth")->GetfirstChild();
		//RightWeldWidth
		fillet.m_RightWeldWidth = atof((char*)_bstr_t(pNode->GetnodeValue()));
	}
}