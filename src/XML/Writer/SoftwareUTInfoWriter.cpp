#include "SoftwareUTInfoWriter.h"

#import "msxml6.dll"

#include <vector>

#include <map>
#include <string>

#pragma warning(disable: 4996)

struct CSoftwareUTInfoWriter::Impl
{
	//构造函数
	Impl(const std::string& name);

	//析构函数
	~Impl();

	//设置总组数
	void SetGroupNum(const int& GroupNum);

	//设置材料信息
	void SetMaterialInfo(const MaterialInfo& material);

	//设置某个组的探头信息
	void SetProbeInfo(const ProbeInfo& probe, const int& GroupID);

	//设置某个组的楔块信息
	void SetWedgeInfo(const WedgeInfo& wedge, const int& GroupID);

	//设置某个组的扫查信息
	void SetScanInfo(const ScanInfo& scan, const int& GroupID);

	//设置位置信息
	void SetPositionInfo(const PositionInfo& position, const int& GroupID);

	//设置某个组采集信息
	void SetGroupBasicInfo(const GroupBasicInfo& group, const int& GroupID);

	//设置某个组的声束信息
	void SetBeamBasicInfo(const BeamBasicInfo& beam, const int& GroupID, const int& ChannelID);

	//设置某个组的ACG信息
	void SetACGInfo(const double& delay, const double& gain, const int& GroupID, const int& ChannelID);

	//设置闸门信息
	void SetGateInfo(const GateInfo& gate, const int& GroupID, const int& GateID);

	//设置编码器信息
	void SetEncoderInfo(const EncoderInfo& encoder);

	//设置滤波器信息
	void SetFilterInfo(const FilterInfo& filter);

	//设置试件类型
	void SetPartType(const PartType& type);

	//设置平板对接试件的信息
	void SetPlateWeldInfo(const PlateWeldInfo& plate, const GrooveInfo& groove);

	//设置角接试件的信息
	void SetFilletWeldInfo(const FilletWeldInfo& fillet);

	//XML文档节点
	MSXML2::IXMLDOMDocumentPtr pDoc;

	//XML根节点
	MSXML2::IXMLDOMElementPtr pRoot;

	std::string m_FilePathName;
};

//构造函数
CSoftwareUTInfoWriter::CSoftwareUTInfoWriter(const char* name)
{
	m_pImpl = new Impl(name);
}

//析构函数
CSoftwareUTInfoWriter::~CSoftwareUTInfoWriter()
{
	delete m_pImpl;
}

//设置总组数
void CSoftwareUTInfoWriter::SetGroupNum(const int& GroupNum)
{
	m_pImpl->SetGroupNum(GroupNum);
}

//设置材料信息
void CSoftwareUTInfoWriter::SetMaterialInfo(const MaterialInfo& material)
{
	m_pImpl->SetMaterialInfo(material);
}

//设置某个组的探头信息
void CSoftwareUTInfoWriter::SetProbeInfo(const ProbeInfo& probe, const int& GroupID)
{
	m_pImpl->SetProbeInfo(probe, GroupID);
}

//设置某个组的楔块信息
void CSoftwareUTInfoWriter::SetWedgeInfo(const WedgeInfo& wedge, const int& GroupID)
{
	m_pImpl->SetWedgeInfo(wedge, GroupID);
}

//设置某个组的扫查信息
void CSoftwareUTInfoWriter::SetScanInfo(const ScanInfo& scan, const int& GroupID)
{
	m_pImpl->SetScanInfo(scan, GroupID);
}

//设置位置信息
void CSoftwareUTInfoWriter::SetPositionInfo(const PositionInfo& position, const int& GroupID)
{
	m_pImpl->SetPositionInfo(position, GroupID);
}

//设置某个组采集信息
void CSoftwareUTInfoWriter::SetGroupBasicInfo(const GroupBasicInfo& group, const int& GroupID)
{
	m_pImpl->SetGroupBasicInfo(group, GroupID);
}

//设置某个组的声束信息
void CSoftwareUTInfoWriter::SetBeamBasicInfo(const BeamBasicInfo& beam, const int& GroupID, const int& ChannelID)
{
	m_pImpl->SetBeamBasicInfo(beam, GroupID, ChannelID);
}

//设置某个组的ACG信息
void CSoftwareUTInfoWriter::SetACGInfo(const double& delay, const double& gain, const int& GroupID, const int& ChannelID)
{
	m_pImpl->SetACGInfo(delay, gain, GroupID, ChannelID);
}

//设置闸门信息
void CSoftwareUTInfoWriter::SetGateInfo(const GateInfo& gate, const int& GroupID, const int& GateID)
{
	m_pImpl->SetGateInfo(gate, GroupID, GateID);
}

//设置编码器信息
void CSoftwareUTInfoWriter::SetEncoderInfo(const EncoderInfo& encoder)
{
	m_pImpl->SetEncoderInfo(encoder);
}

//设置滤波器信息
void CSoftwareUTInfoWriter::SetFilterInfo(const FilterInfo& filter)
{
	m_pImpl->SetFilterInfo(filter);
}

//设置试件类型
void CSoftwareUTInfoWriter::SetPartType(const PartType& type)
{
	m_pImpl->SetPartType(type);
}

//设置平板对接试件的信息
void CSoftwareUTInfoWriter::SetPlateWeldInfo(const PlateWeldInfo& plate, const GrooveInfo& groove)
{
	m_pImpl->SetPlateWeldInfo(plate, groove);
}

//设置角接试件的信息
void CSoftwareUTInfoWriter::SetFilletWeldInfo(const FilletWeldInfo& fillet)
{
	m_pImpl->SetFilletWeldInfo(fillet);
}

//构造函数
CSoftwareUTInfoWriter::Impl::Impl(const std::string& name)
{
	CoInitialize(NULL);

	m_FilePathName = name;

	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (FAILED(hr))
	{
		printf("Create Failed\n");
		return;
	}

	//创建xml文档声明(或insertBefore根节点)
	//xml声明
	MSXML2::IXMLDOMProcessingInstructionPtr pDeclare;
	pDeclare = pDoc->createProcessingInstruction((_bstr_t)(char*)"xml", (_bstr_t)(char*)"version=\"1.0\" encoding=\"utf-8\"");
	pDoc->appendChild((MSXML2::IXMLDOMNode*)pDeclare);

	//创建根节点
	//<Software>
	pRoot =  pDoc->createElement((_bstr_t)(char*)"Software");
	pDoc->PutRefdocumentElement(pRoot);
}

//析构函数
CSoftwareUTInfoWriter::Impl::~Impl()
{
	pDoc->save(m_FilePathName.c_str());
	pDoc.Release();
	pRoot.Release();
	CoUninitialize();
}

//获得总组数
void CSoftwareUTInfoWriter::Impl::SetGroupNum(const int& GroupNum)
{
	char str[100];
	MSXML2::IXMLDOMNodePtr pGroupNum;
	pGroupNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GroupNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", GroupNum);
	pGroupNum->Puttext((_bstr_t)str);
	pRoot->appendChild(pGroupNum);

	//创建相应于组数个的Group子节点
	MSXML2::IXMLDOMNodePtr pGroup;
	for (int i = 0; i < GroupNum; i++)
	{
		sprintf(str, "Group%d", i + 1);
		pGroup = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)"");
		pRoot->appendChild(pGroup);
	}
}

//设置材料信息
void CSoftwareUTInfoWriter::Impl::SetMaterialInfo(const MaterialInfo& material)
{
	char str[100];

	//创建节点<MaterialID></MaterialID>
	MSXML2::IXMLDOMNodePtr pMaterialID;
	pMaterialID = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"MaterialID", (_bstr_t)(char*)"");
	sprintf(str, "%d", material.m_MaterialID);
	pMaterialID->Puttext((_bstr_t)str);
	pRoot->appendChild(pMaterialID);

	//创建节点<MaterialName></MaterialName>
	MSXML2::IXMLDOMNodePtr pMaterialName;
	pMaterialName = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"MaterialName", (_bstr_t)(char*)"");

	pMaterialName->Puttext((_bstr_t)material.m_MaterialName);
	pRoot->appendChild(pMaterialName);

	//创建节点<LongitudinalVelocity></LongitudinalVelocity>
	MSXML2::IXMLDOMNodePtr pLongitudinalVelocity;
	pLongitudinalVelocity = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"LongitudinalVelocity", (_bstr_t)(char*)"");
	sprintf(str, "%lf", material.m_LongitudinalVelocity);
	pLongitudinalVelocity->Puttext((_bstr_t)str);
	pRoot->appendChild(pLongitudinalVelocity);

	//创建节点<ShearVelocity></ShearVelocity>
	MSXML2::IXMLDOMNodePtr pShearVelocity;
	pShearVelocity = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ShearVelocity", (_bstr_t)(char*)"");
	sprintf(str, "%lf", material.m_ShearVelocity);
	pShearVelocity->Puttext((_bstr_t)str);
	pRoot->appendChild(pShearVelocity);
}

//获得某个组的探头信息
void CSoftwareUTInfoWriter::Impl::SetProbeInfo(const ProbeInfo& probe, const int& GroupID)
{
	char str[100];

	//创建节点<ProbeID></ProbePID>
	MSXML2::IXMLDOMNodePtr pProbeID;
	pProbeID = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbeID", (_bstr_t)(char*)"");
	sprintf(str, "%d", probe.m_ProbeID);
	pProbeID->Puttext((_bstr_t)str);

	//创建节点<ProbeName></ProbeName>
	MSXML2::IXMLDOMNodePtr pProbeName;
	pProbeName = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbeName", (_bstr_t)(char*)"");

	pProbeName->Puttext((_bstr_t)probe.m_ProbeName);

	//创建节点<ProbePitch></ProbePitch>
	MSXML2::IXMLDOMNodePtr pProbePitch;
	pProbePitch = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbePitch", (_bstr_t)(char*)"");
	sprintf(str, "%lf", probe.m_Pitch);
	pProbePitch->Puttext((_bstr_t)str);

	//创建节点<ElementNum></ElementNum>
	MSXML2::IXMLDOMNodePtr pElementNum;
	pElementNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ElementNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", probe.m_ElementNum);
	pElementNum->Puttext((_bstr_t)str);

	//创建节点<ProbeFrequency></ProbeFrequency>
	MSXML2::IXMLDOMNodePtr pFrequency;
	pFrequency = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbeFrequency", (_bstr_t)(char*)"");
	sprintf(str, "%lf", probe.m_Frequency);
	pFrequency->Puttext((_bstr_t)str);

	//创建节点<ProbeHeight></ProbeHeight>
	MSXML2::IXMLDOMNodePtr pHeight;
	pHeight = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbeHeight", (_bstr_t)(char*)"");
	sprintf(str, "%lf", probe.m_Height);
	pHeight->Puttext((_bstr_t)str);

	//创建节点<ProbeD></ProbeD>
	MSXML2::IXMLDOMNodePtr pProbeD;
	pProbeD = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbeD", (_bstr_t)(char*)"");
	sprintf(str, "%lf", probe.m_D);
	pProbeD->Puttext((_bstr_t)str);

	//创建节点<ProbeType></ProbeType>
	MSXML2::IXMLDOMNodePtr pProbeType;
	pProbeType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbeType", (_bstr_t)(char*)"");
	std::string str1;
	switch (probe.m_ProbeType)
	{
	case ProbeType::eLinear:
		str1 = "Linear";
		break;
	case ProbeType::eArc:
		str1 = "Arc";
		break;
	case ProbeType::eRing:
		str1 = "Ring";
		break;
	default:
		str1 = "Linear";
		break;
	}
	pProbeType->Puttext((_bstr_t)str1.c_str());

	//创建节点<ProbeRadius></ProbeRadius>
	MSXML2::IXMLDOMNodePtr pProbeRadius;
	pProbeRadius = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbeRadius", (_bstr_t)(char*)"");
	sprintf(str, "%lf", probe.m_Radius);
	pProbeRadius->Puttext((_bstr_t)str);

	//创建节点<Connection></Connection>
	MSXML2::IXMLDOMNodePtr pConnection;
	pConnection = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"FirstConnection", (_bstr_t)(char*)"");
	sprintf(str, "%d", probe.m_FirstConnection);
	pConnection->Puttext((_bstr_t)str);

	//获取节点<GroupID></GroupID>
	MSXML2::IXMLDOMNodePtr pGroup = NULL;
	sprintf(str, "Group%d", GroupID);
	pGroup = pRoot->selectSingleNode((_bstr_t)str);

	pGroup->appendChild(pProbeID);
	pGroup->appendChild(pProbeName);
	pGroup->appendChild(pProbePitch);
	pGroup->appendChild(pElementNum);
	pGroup->appendChild(pFrequency);
	pGroup->appendChild(pHeight);
	pGroup->appendChild(pProbeD);
	pGroup->appendChild(pProbeType);
	pGroup->appendChild(pProbeRadius);
	pGroup->appendChild(pConnection);
}

//获得某个组的楔块信息
void CSoftwareUTInfoWriter::Impl::SetWedgeInfo(const WedgeInfo& wedge, const int& GroupID)
{
	char str[100];

	//创建节点<WedgeID></WedgeID>
	MSXML2::IXMLDOMNodePtr pWedgeID;
	pWedgeID = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WedgeID", (_bstr_t)(char*)"");
	sprintf(str, "%d", wedge.m_WedgeID);
	pWedgeID->Puttext((_bstr_t)str);

	//创建节点<WedgeName></WedgeName>
	MSXML2::IXMLDOMNodePtr pWedgeName;
	pWedgeName = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WedgeName", (_bstr_t)(char*)"");
	pWedgeName->Puttext((_bstr_t)wedge.m_WedgeName);

	//创建节点<WedgeAngle></WedgeAngle>
	MSXML2::IXMLDOMNodePtr pWedgeAngle;
	pWedgeAngle = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WedgeAngle", (_bstr_t)(char*)"");
	sprintf(str, "%lf", wedge.m_WedgeAngle);
	pWedgeAngle->Puttext((_bstr_t)str);

	//创建节点<WedgeVelocity></WedgeVelocity>
	MSXML2::IXMLDOMNodePtr pVelocity;
	pVelocity = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WedgeVelocity", (_bstr_t)(char*)"");
	sprintf(str, "%lf", wedge.m_Velocity);
	pVelocity->Puttext((_bstr_t)str);

	//创建节点<ProbeOffesetX></ProbeOffesetX>
	MSXML2::IXMLDOMNodePtr pProbeOffsetX;
	pProbeOffsetX = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbeOffsetX", (_bstr_t)(char*)"");
	sprintf(str, "%lf", wedge.m_ProbeOffsetX);
	pProbeOffsetX->Puttext((_bstr_t)str);

	//创建节点<ProbeOffesetY></ProbeOffesetY>
	MSXML2::IXMLDOMNodePtr pProbeOffsetY;
	pProbeOffsetY = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ProbeOffsetY", (_bstr_t)(char*)"");
	sprintf(str, "%lf", wedge.m_ProbeOffsetY);
	pProbeOffsetY->Puttext((_bstr_t)str);

	//创建节点<WedgeLength></WedgeLength>
	MSXML2::IXMLDOMNodePtr pWedgeLength;
	pWedgeLength = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WedgeLength", (_bstr_t)(char*)"");
	sprintf(str, "%lf", wedge.m_w1);
	pWedgeLength->Puttext((_bstr_t)str);

	//创建节点<WedgeHeight></WedgeHeight>
	MSXML2::IXMLDOMNodePtr pWedgeHeight;
	pWedgeHeight = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WedgeHeight", (_bstr_t)(char*)"");
	sprintf(str, "%lf", wedge.m_h1);
	pWedgeHeight->Puttext((_bstr_t)str);

	//创建节点<OtherInfoNum></OtherInfoNum>
	MSXML2::IXMLDOMNodePtr pOtherInfoNum;
	pOtherInfoNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"OtherInfoNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", (int)wedge.m_OtherInfo.size());
	pOtherInfoNum->Puttext((_bstr_t)str);
	std::vector<MSXML2::IXMLDOMNodePtr> Other;
	for (int i = 0; i < (int)wedge.m_OtherInfo.size(); i++)
	{
		sprintf(str, "Info%d", i + 1);
		Other.push_back(pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)""));
		sprintf(str, "%lf", wedge.m_OtherInfo[i]);
		Other[i]->Puttext((_bstr_t)str);
	}

	//获取节点<GroupID></GroupID>
	MSXML2::IXMLDOMNodePtr pGroup = NULL;
	sprintf(str, "Group%d", GroupID);
	pGroup = pRoot->selectSingleNode((_bstr_t)str);

	pGroup->appendChild(pWedgeID);
	pGroup->appendChild(pWedgeName);
	pGroup->appendChild(pWedgeAngle);
	pGroup->appendChild(pVelocity);
	pGroup->appendChild(pProbeOffsetX);
	pGroup->appendChild(pProbeOffsetY);
	pGroup->appendChild(pWedgeLength);
	pGroup->appendChild(pWedgeHeight);
	pGroup->appendChild(pOtherInfoNum);
	for (int i = 0; i < (int)wedge.m_OtherInfo.size(); i++)
	{
		pGroup->appendChild(Other[i]);
	}
}

//设置某个组的扫查信息
void CSoftwareUTInfoWriter::Impl::SetScanInfo(const ScanInfo& scan, const int& GroupID)
{
	char str[100];

	//创建节点<AngleStart></AngleStart>
	MSXML2::IXMLDOMNodePtr pAngleStart;
	pAngleStart = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"AngleStart", (_bstr_t)(char*)"");
	sprintf(str, "%lf", scan.m_AngleStart);
	pAngleStart->Puttext((_bstr_t)str);

	//创建节点<AngleStop></AngleStop>
	MSXML2::IXMLDOMNodePtr pAngleStop;
	pAngleStop = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"AngleStop", (_bstr_t)(char*)"");
	sprintf(str, "%lf", scan.m_AngleStop);
	pAngleStop->Puttext((_bstr_t)str);

	//创建节点<ChannelNum></ChannelNum>
	MSXML2::IXMLDOMNodePtr pChannelNum;
	pChannelNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ChannelNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", scan.m_ChannelNum);
	pChannelNum->Puttext((_bstr_t)str);

	//创建节点<ChannelSize></ChannelSize>
	MSXML2::IXMLDOMNodePtr pChannelSize;
	pChannelSize = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ChannelSize", (_bstr_t)(char*)"");
	sprintf(str, "%d", scan.m_ChannelSize);
	pChannelSize->Puttext((_bstr_t)str);

	//创建节点<ElementStart></ElementStart>
	MSXML2::IXMLDOMNodePtr pElementStart;
	pElementStart = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ElementStart", (_bstr_t)(char*)"");
	sprintf(str, "%d", scan.m_ElementStart);
	pElementStart->Puttext((_bstr_t)str);

	//创建节点<ElementStop></ElementStop>
	MSXML2::IXMLDOMNodePtr pElementStop;
	pElementStop = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ElementStop", (_bstr_t)(char*)"");
	sprintf(str, "%d", scan.m_ElementStop);
	pElementStop->Puttext((_bstr_t)str);

	//创建节点<SendFocus></SendFocus>
	MSXML2::IXMLDOMNodePtr pSendFocus;
	pSendFocus = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"SendFocus", (_bstr_t)(char*)"");
	sprintf(str, "%lf", scan.m_SendFocus);
	pSendFocus->Puttext((_bstr_t)str);

	//创建节点<RecvFocusStart></RecvFocusStart>
	MSXML2::IXMLDOMNodePtr pRecvFocusStart;
	pRecvFocusStart = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RecvFocusStart", (_bstr_t)(char*)"");
	sprintf(str, "%lf", scan.m_RecvFocusStart);
	pRecvFocusStart->Puttext((_bstr_t)str);

	//创建节点<RecvFocusEnd></RecvFocusEnd>
	MSXML2::IXMLDOMNodePtr pRecvFocusEnd;
	pRecvFocusEnd = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RecvFocusEnd", (_bstr_t)(char*)"");
	sprintf(str, "%lf", scan.m_RecvFocusEnd);
	pRecvFocusEnd->Puttext((_bstr_t)str);

	//创建节点<FocalType></FocalType>
	MSXML2::IXMLDOMNodePtr pFocalType;
	pFocalType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"FocalType", (_bstr_t)(char*)"");
	sprintf(str, "%d", scan.m_FocalType);
	pFocalType->Puttext((_bstr_t)str);

	//创建节点<RecvFocusNum></RecvFocusNum>
	MSXML2::IXMLDOMNodePtr pRecvFocusNum;
	pRecvFocusNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RecvFocusNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", scan.m_RecvFocusNum);
	pRecvFocusNum->Puttext((_bstr_t)str);

	//创建节点<Reflect></Reflect>
	MSXML2::IXMLDOMNodePtr pReflect;
	pReflect = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Reflect", (_bstr_t)(char*)"");
	sprintf(str, "%d", scan.m_Reflect);
	pReflect->Puttext((_bstr_t)str);

	//创建节点<VelocityType></VelocityType>
	MSXML2::IXMLDOMNodePtr pVelocityType;
	pVelocityType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"VelocityType", (_bstr_t)(char*)"");
	std::string str1;
	switch (scan.m_VelocityType)
	{
	case VelocityType::eLongitudinal:
		str1 = "Longitudinal";
		break;
	case VelocityType::eShear:
		str1 = "Shear";
		break;
	default:
		break;
	}
	pVelocityType->Puttext((_bstr_t)str1.c_str());

	//创建节点<Type></Type>
	MSXML2::IXMLDOMNodePtr pType;
	pType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Type", (_bstr_t)(char*)"");
	switch (scan.m_Type)
	{
	case ScanType::ePAAngle:
		str1 = "PAAngle";
		break;
	case ScanType::ePALinear:
		str1 = "PALinear";
		break;
	case ScanType::eTOFD:
		str1 = "TOFD";
		break;
	case ScanType::eFMC:
		str1 = "FMC";
		break;
	case ScanType::eRing:
		str1 = "Ring";
		break;
	case ScanType::eSpecial:
		str1 = "Special";
		break;
	default:
		break;
	}
	pType->Puttext((_bstr_t)str1.c_str());

	//获取节点<GroupID></GroupID>
	MSXML2::IXMLDOMNodePtr pGroup = NULL;
	sprintf(str, "Group%d", GroupID);
	pGroup = pRoot->selectSingleNode((_bstr_t)str);

	pGroup->appendChild(pAngleStart);
	pGroup->appendChild(pAngleStop);
	pGroup->appendChild(pChannelSize);
	pGroup->appendChild(pElementStart);
	pGroup->appendChild(pElementStop);
	pGroup->appendChild(pVelocityType);
	pGroup->appendChild(pType);
	pGroup->appendChild(pReflect);
}

//设置位置信息
void CSoftwareUTInfoWriter::Impl::SetPositionInfo(const PositionInfo& position, const int& GroupID)
{
	char str[100];
	std::string str1;

	//创建节点<InspectPos></InspectPos>
	MSXML2::IXMLDOMNodePtr pInspectPos;
	pInspectPos = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"InspectPos", (_bstr_t)(char*)"");
	switch (position.m_InspectPos)
	{
	case InspectPos::WeldPDetectPos_Up:
		str1 = "WeldPDetectPos_Up";
		break;
	case InspectPos::WeldPDetectPos_Down:
		str1 = "WeldPDetectPos_Down";
		break;
	case InspectPos::WeldPDetectPos_UpL:
		str1 = "WeldPDetectPos_UpL";
		break;
	case InspectPos::WeldPDetectPos_UpR:
		str1 = "WeldPDetectPos_UpR";
		break;
	case InspectPos::WeldPDetectPos_DownL:
		str1 = "WeldPDetectPos_DownL";
		break;
	case InspectPos::WeldPDetectPos_DownR:
		str1 = "WeldPDetectPos_DownR";
		break;
	case InspectPos::WeldFDetectPos_WebUp:
		str1 = "WeldFDetectPos_WebUp";
		break;
	case InspectPos::WeldFDetectPos_WebDown:
		str1 = "WeldFDetectPos_WebDown";
		break;
	case InspectPos::WeldFDetectPos_MomUpL:
		str1 = "WeldFDetectPos_MomUpL";
		break;
	case InspectPos::WeldFDetectPos_MomUpR:
		str1 = "WeldFDetectPos_MomUpR";
		break;
	case InspectPos::WeldFDetectPos_MomDown:
		str1 = "WeldFDetectPos_MomDown";
		break;
	default:
		break;
	}
	pInspectPos->Puttext((_bstr_t)str1.c_str());

	//创建节点<WedgeOffset></WedgeOffset>
	MSXML2::IXMLDOMNodePtr pWedgeOffset;
	pWedgeOffset = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WedgeOffset", (_bstr_t)(char*)"");
	sprintf(str, "%lf", position.m_WedgeOffset);
	pWedgeOffset->Puttext((_bstr_t)str);

	//获取节点<GroupID></GroupID>
	MSXML2::IXMLDOMNodePtr pGroup = NULL;
	sprintf(str, "Group%d", GroupID);
	pGroup = pRoot->selectSingleNode((_bstr_t)str);
	
	pGroup->appendChild(pInspectPos);
	pGroup->appendChild(pWedgeOffset);
}

//获得某个组采集信息
void CSoftwareUTInfoWriter::Impl::SetGroupBasicInfo(const GroupBasicInfo& group, const int& GroupID)
{
	std::string str1;

	//创建节点<ScanType></ScanType>
	MSXML2::IXMLDOMNodePtr pScanType;
	pScanType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ScanType", (_bstr_t)(char*)"");
	switch (group.m_ScanType)
	{
	case ScanType::ePAAngle:
	case ScanType::ePALinear:
	case ScanType::ePA:
		str1 = "PA";
		break;
	case ScanType::eTOFD:
		str1 = "TOFD";
		break;
	case ScanType::eRing:
		str1 = "Ring";
		break;
	case ScanType::eSpecial:
		str1 = "Special";
		break;
	}
	pScanType->Puttext((_bstr_t)str1.c_str());


	//创建节点<TRMode></TRMode>
	MSXML2::IXMLDOMNodePtr pTRMode;
	pTRMode = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"TRMode", (_bstr_t)(char*)"");
	switch (group.m_TRMode)
	{
	case TRMode::eTransmitReceive:
		{
			str1 = "TransmitReceive";
			break;
		}
	case TRMode::eTransmit:
		{
			str1 = "Transmit";
			break;
		}
	}
	pTRMode->Puttext((_bstr_t)str1.c_str());

	char str[100];

	//创建节点<ChannelNum></ChannelNum>
	MSXML2::IXMLDOMNodePtr pChannelNum;
	pChannelNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ChannelNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", group.m_ChannelNum);
	pChannelNum->Puttext((_bstr_t)str);

	//创建节点<GateNum></GateNum>
	MSXML2::IXMLDOMNodePtr pGateNum;
	pGateNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GateNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", GateInfo::eMaxGateNum);
	pGateNum->Puttext((_bstr_t)str);

	//创建节点<PartVelocity></PartVelocity>
	MSXML2::IXMLDOMNodePtr pPartVelocity;
	pPartVelocity = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"PartVelocity", (_bstr_t)(char*)"");
	sprintf(str, "%lf", group.m_PartVelocity);
	pPartVelocity->Puttext((_bstr_t)str);

	//创建节点<SendFocus></SendFocus>
	MSXML2::IXMLDOMNodePtr pSendFocus;
	pSendFocus = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"SendFocus", (_bstr_t)(char*)"");
	sprintf(str, "%lf", group.m_SendFocus);
	pSendFocus->Puttext((_bstr_t)str);

	//创建节点<RecvFocusNum></RecvFocusNum>
	MSXML2::IXMLDOMNodePtr pRecvFocusNum;
	pRecvFocusNum = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RecvFocusNum", (_bstr_t)(char*)"");
	sprintf(str, "%d", group.m_RecvFocusNum);
	pRecvFocusNum->Puttext((_bstr_t)str);

	//创建节点<RecvFocusStart></RecFocusStart>
	MSXML2::IXMLDOMNodePtr pRecFocusStart;
	pRecFocusStart = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RecvFocusStart", (_bstr_t)(char*)"");
	sprintf(str, "%lf", group.m_RecvFocusStart);
	pRecFocusStart->Puttext((_bstr_t)str);

	//创建节点<RecvFocusEnd></RecFocusEnd>
	MSXML2::IXMLDOMNodePtr pRecFocusEnd;
	pRecFocusEnd = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RecvFocusEnd", (_bstr_t)(char*)"");
	sprintf(str, "%lf", group.m_RecvFocusEnd);
	pRecFocusEnd->Puttext((_bstr_t)str);

	//创建节点<FocalType></FocalType>
	MSXML2::IXMLDOMNodePtr pFocalType;
	pFocalType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"FocalType", (_bstr_t)(char*)"");
	sprintf(str, "%d", group.m_FocalType);
	pFocalType->Puttext((_bstr_t)str);

	//获取节点<GroupID></GroupID>
	MSXML2::IXMLDOMNodePtr pGroup = NULL;
	sprintf(str, "Group%d", GroupID);
	pGroup = pRoot->selectSingleNode((_bstr_t)str);

	pGroup->appendChild(pChannelNum);
	pGroup->appendChild(pScanType);
	pGroup->appendChild(pTRMode);
	pGroup->appendChild(pPartVelocity);
	pGroup->appendChild(pSendFocus);
	pGroup->appendChild(pRecvFocusNum);
	pGroup->appendChild(pRecFocusStart);
	pGroup->appendChild(pRecFocusEnd);
	pGroup->appendChild(pFocalType);

	//创建相应于ChannelNum个的Channel子节点
	MSXML2::IXMLDOMNodePtr pChannel;
	for (int i = 0; i < group.m_ChannelNum; i++)
	{
		sprintf(str, "Channel%d", i + 1);
		pChannel = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)"");
		pGroup->appendChild(pChannel);
	}

	//创建相应于GateNum个的Gate子节点
	MSXML2::IXMLDOMNodePtr pGae;
	for (int i = 0; i < GateInfo::eMaxGateNum; i++)
	{
		sprintf(str, "GateID%d", i);
		pGae = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)str, (_bstr_t)(char*)"");
		pGroup->appendChild(pGae);
	}
}

//获得某个组的声束信息
void CSoftwareUTInfoWriter::Impl::SetBeamBasicInfo(const BeamBasicInfo& beam, const int& GroupID, const int& ChannelID)
{
	char str[100];
	//创建节点<ScanAngle></ScanAngle>
	MSXML2::IXMLDOMNodePtr pScanAngle;
	pScanAngle = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ScanAngle", (_bstr_t)(char*)"");
	sprintf(str, "%lf", beam.m_BeamAngle);
	pScanAngle->Puttext((_bstr_t)str);

	//创建节点<CenterElement></CenterElement>
	MSXML2::IXMLDOMNodePtr pCenterElement;
	pCenterElement = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"CenterElement", (_bstr_t)(char*)"");
	sprintf(str, "%d", beam.m_CenterElementID);
	pCenterElement->Puttext((_bstr_t)str);

	//创建节点<WedgeDelay></WedgeDelay>
	MSXML2::IXMLDOMNodePtr pWedgeDelay;
	pWedgeDelay = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WedgeDelay", (_bstr_t)(char*)"");
	sprintf(str, "%lf", beam.m_WedgeDelay);
	pWedgeDelay->Puttext((_bstr_t)str);


	//获取节点<GroupID></GroupID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	sprintf(str, "Group%d", GroupID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);
	sprintf(str, "Channel%d", ChannelID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	pNode->appendChild(pScanAngle);
	pNode->appendChild(pCenterElement);
	pNode->appendChild(pWedgeDelay);
}

//设置某个组的DAC信息
void CSoftwareUTInfoWriter::Impl::SetACGInfo(const double& delay, const double& gain, const int& GroupID, const int& ChannelID)
{
	char str[100];
	//创建节点<DACDelay></DACDelay>
	MSXML2::IXMLDOMNodePtr pDACDelay;
	pDACDelay = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ACGDelay", (_bstr_t)(char*)"");
	sprintf(str, "%lf", delay);
	pDACDelay->Puttext((_bstr_t)str);

	//创建节点<DACGain></DACGain>
	MSXML2::IXMLDOMNodePtr pDACGain;
	pDACGain = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ACGGain", (_bstr_t)(char*)"");
	sprintf(str, "%lf", gain);
	pDACGain->Puttext((_bstr_t)str);


	//获取节点<GroupID></GroupID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	sprintf(str, "Group%d", GroupID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);
	sprintf(str, "Channel%d", ChannelID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	pNode->appendChild(pDACDelay);
	pNode->appendChild(pDACGain);
}

//设置闸门信息
void CSoftwareUTInfoWriter::Impl::SetGateInfo(const GateInfo& gate, const int& GroupID, const int& GateID)
{
	//原本是str[100]在执行过程中可能会出现栈溢出 改成str[1000]以后就没问题了
	char str[1000];
	//创建节点<GateIsUsed></GateIsUsed>
	MSXML2::IXMLDOMNodePtr pGateIsUsed;
	pGateIsUsed = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GateIsUsed", (_bstr_t)(char*)"");
	sprintf(str, "%d", gate.m_GateIsUsed ? 1 : 0);
	pGateIsUsed->Puttext((_bstr_t)str);

	//创建节点<GateType></GateType>
	MSXML2::IXMLDOMNodePtr pGateType;
	pGateType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GateType", (_bstr_t)(char*)"");
	sprintf(str, "%d", gate.m_GateType);
	pGateType->Puttext((_bstr_t)str);

	//创建节点<GateStart></GateStart>
	MSXML2::IXMLDOMNodePtr pGateStart;
	pGateStart = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GateStart", (_bstr_t)(char*)"");
	sprintf(str, "%lf", gate.m_GateIsUsed ? gate.m_GateStart : 0);
	pGateStart->Puttext((_bstr_t)str);

	//创建节点<GateRange></GateRange>
	MSXML2::IXMLDOMNodePtr pGateRange;
	pGateRange = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GateRange", (_bstr_t)(char*)"");
	sprintf(str, "%lf", gate.m_GateIsUsed ? gate.m_GateRange : 0);
	pGateRange->Puttext((_bstr_t)str);

	//创建节点<GateThreshold></GateThreshold>
	MSXML2::IXMLDOMNodePtr pGateThreshold;
	pGateThreshold = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GateThreshold", (_bstr_t)(char*)"");
	sprintf(str, "%lf", gate.m_GateIsUsed ? gate.m_GateThreshold : 0);
	pGateThreshold->Puttext((_bstr_t)str);


	//获取节点<GroupID></GroupID>
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	sprintf(str, "Group%d", GroupID);
	pNode = pRoot->selectSingleNode((_bstr_t)str);
	sprintf(str, "GateID%d", GateID);
	pNode = pNode->selectSingleNode((_bstr_t)str);

	pNode->appendChild(pGateIsUsed);
	pNode->appendChild(pGateType);
	pNode->appendChild(pGateStart);
	pNode->appendChild(pGateRange);
	pNode->appendChild(pGateThreshold);
}

//获得编码器信息
void CSoftwareUTInfoWriter::Impl::SetEncoderInfo(const EncoderInfo& encoder)
{
	char str[100];
	//创建节点<PointNumOfScanAxis></PointNumOfScanAxis>
	MSXML2::IXMLDOMNodePtr pPointNumOfScanAxis;
	pPointNumOfScanAxis = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"PointNumOfScanAxis", (_bstr_t)(char*)"");
	sprintf(str, "%d", encoder.m_ScanPointNum);
	pPointNumOfScanAxis->Puttext((_bstr_t)str);
	pRoot->appendChild(pPointNumOfScanAxis);

	//创建节点<ScanAxisStart></ScanAxisStart>
	MSXML2::IXMLDOMNodePtr pPScanAxisStart;
	pPScanAxisStart = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ScanAxisStart", (_bstr_t)(char*)"");
	sprintf(str, "%lf", encoder.m_ScanStart);
	pPScanAxisStart->Puttext((_bstr_t)str);
	pRoot->appendChild(pPScanAxisStart);

	//创建节点<ScanAxisRange></ScanAxisRange>
	MSXML2::IXMLDOMNodePtr pScanAxisRange;
	pScanAxisRange = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ScanAxisRange", (_bstr_t)(char*)"");
	sprintf(str, "%lf", encoder.m_ScanRange);
	pScanAxisRange->Puttext((_bstr_t)str);
	pRoot->appendChild(pScanAxisRange);

	//创建节点<PointNumOfIndexAxis></PointNumOfIndexAxis>
	MSXML2::IXMLDOMNodePtr pPointNumOfIndexAxis;
	pPointNumOfIndexAxis = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"PointNumOfIndexAxis", (_bstr_t)(char*)"");
	sprintf(str, "%d", encoder.m_IndexPointNum);
	pPointNumOfIndexAxis->Puttext((_bstr_t)str);
	pRoot->appendChild(pPointNumOfIndexAxis);

	//创建节点<IndexAxisStart></IndexAxisStart>
	MSXML2::IXMLDOMNodePtr pIndexAxisStart;
	pIndexAxisStart = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"IndexAxisStart", (_bstr_t)(char*)"");
	sprintf(str, "%lf", encoder.m_IndexStart);
	pIndexAxisStart->Puttext((_bstr_t)str);
	pRoot->appendChild(pIndexAxisStart);

	//创建节点<ScanAxisRange></ScanAxisRange>
	MSXML2::IXMLDOMNodePtr pIndexAxisRange;
	pIndexAxisRange = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"IndexAxisRange", (_bstr_t)(char*)"");
	sprintf(str, "%lf", encoder.m_IndexRange);
	pIndexAxisRange->Puttext((_bstr_t)str);
	pRoot->appendChild(pIndexAxisRange);

	//创建节点<Encoder1Resolution></Encoder1Resolution>
	MSXML2::IXMLDOMNodePtr pEncoder1Resolution;
	pEncoder1Resolution = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Encoder1Resolution", (_bstr_t)(char*)"");
	sprintf(str, "%d", encoder.m_EncoderResolution[0]);
	pEncoder1Resolution->Puttext((_bstr_t)str);
	pRoot->appendChild(pEncoder1Resolution);

	//创建节点<Encoder2Resolution></Encoder2Resolution>
	MSXML2::IXMLDOMNodePtr pEncoder2Resolution;
	pEncoder2Resolution = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Encoder2Resolution", (_bstr_t)(char*)"");
	sprintf(str, "%d", encoder.m_EncoderResolution[1]);
	pEncoder2Resolution->Puttext((_bstr_t)str);
	pRoot->appendChild(pEncoder2Resolution);

	//创建节点<Encoder1Porlarity></Encoder1Porlarity>
	MSXML2::IXMLDOMNodePtr pEncoder1Porlarity;
	pEncoder1Porlarity = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Encoder1Porlarity", (_bstr_t)(char*)"");
	sprintf(str, "%d", encoder.m_EncoderPolarity[0]);
	pEncoder1Porlarity->Puttext((_bstr_t)str);
	pRoot->appendChild(pEncoder1Porlarity);

	//创建节点<Encoder2Porlarity></Encoder2Porlarity>
	MSXML2::IXMLDOMNodePtr pEncoder2Porlarity;
	pEncoder2Porlarity = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Encoder2Porlarity", (_bstr_t)(char*)"");
	sprintf(str, "%d", encoder.m_EncoderPolarity[1]);
	pEncoder2Porlarity->Puttext((_bstr_t)str);
	pRoot->appendChild(pEncoder2Porlarity);

	//创建节点<ScanAxisID></ScanAxisID>
	MSXML2::IXMLDOMNodePtr pScanAxisID;
	pScanAxisID = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"ScanAxisID", (_bstr_t)(char*)"");
	sprintf(str, "%d", encoder.m_ScanAxisID);
	pScanAxisID->Puttext((_bstr_t)str);
	pRoot->appendChild(pScanAxisID);
}

//设置滤波器信息
void CSoftwareUTInfoWriter::Impl::SetFilterInfo(const FilterInfo& filter)
{
	char str[100];
	//创建节点<FilterType></FilterType>
	MSXML2::IXMLDOMNodePtr pFilterType;
	pFilterType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"FilterType", (_bstr_t)(char*)"");

	std::map<FilterType, std::string> filtermap
	{
		{ FilterType::eNoFilter, "No Filter" },
		{ FilterType::eBandPassFilter, "Band Filter" },
		{ FilterType::eLowPassFilter, "Lowpass Filter" },
		{ FilterType::eEnvellopFilter, "Envelop Filter" }
	};

	std::string str1 = filtermap[filter.m_FilterType];
	pFilterType->Puttext((_bstr_t)str1.c_str());
	pRoot->appendChild(pFilterType);

	//创建节点<Freq1></Freq1>
	MSXML2::IXMLDOMNodePtr pFreq1;
	pFreq1 = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Freq1", (_bstr_t)(char*)"");
	sprintf(str, "%lf", filter.m_Frequency1);
	pFreq1->Puttext((_bstr_t)str);
	pRoot->appendChild(pFreq1);

	//创建节点<Freq2></Freq2>
	MSXML2::IXMLDOMNodePtr pFreq2;
	pFreq2 = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"Freq2", (_bstr_t)(char*)"");
	sprintf(str, "%lf", filter.m_Frequency2);
	pFreq2->Puttext((_bstr_t)str1.c_str());
	pRoot->appendChild(pFreq2);
}

//设置试件类型
void CSoftwareUTInfoWriter::Impl::SetPartType(const PartType& type)
{
	std::string str;
	//创建节点<PartType></PartType>
	MSXML2::IXMLDOMNodePtr pPartType;
	pPartType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"PartType", (_bstr_t)(char*)"");
	switch (type)
	{
	case PartType::PWeld:
		str = "Plate Weld";
		break;
	case PartType::FWeld:
		str = "Fillet Weld";
		break;
	default:
		break;
	}
	pPartType->Puttext((_bstr_t)str.c_str());
	pRoot->appendChild(pPartType);
}

//设置平板对接试件的信息
void CSoftwareUTInfoWriter::Impl::SetPlateWeldInfo(const PlateWeldInfo& plate, const GrooveInfo& groove)
{
	char str[100];
	//创建节点<MotherThickness></MotherThickness>
	MSXML2::IXMLDOMNodePtr pMotherThickness;
	pMotherThickness = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"MotherThickness", (_bstr_t)(char*)"");
	sprintf(str, "%lf", plate.m_MotherThickness);
	pMotherThickness->Puttext((_bstr_t)str);
	pRoot->appendChild(pMotherThickness);

	//创建节点<MotherWidth></MotherWidth>
	MSXML2::IXMLDOMNodePtr pMotherWidth;
	pMotherWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"MotherWidth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", plate.m_MotherWidth);
	pMotherWidth->Puttext((_bstr_t)str);
	pRoot->appendChild(pMotherWidth);

	//创建节点<UpWeldWidth></UpWeldWidth>
	MSXML2::IXMLDOMNodePtr pUpWeldWidth;
	pUpWeldWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"UpWeldWidth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", plate.m_UpWeldWidth);
	pUpWeldWidth->Puttext((_bstr_t)str);
	pRoot->appendChild(pUpWeldWidth);

	//创建节点<UpWeldHeight></UpWeldHeight>
	MSXML2::IXMLDOMNodePtr pUpWeldHeight;
	pUpWeldHeight = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"UpWeldHeight", (_bstr_t)(char*)"");
	sprintf(str, "%lf", plate.m_UpWeldHeight);
	pUpWeldHeight->Puttext((_bstr_t)str);
	pRoot->appendChild(pUpWeldHeight);

	//创建节点<DownWeldWidth></DownWeldWidth>
	MSXML2::IXMLDOMNodePtr pDownWeldWidth;
	pDownWeldWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"DownWeldWidth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", plate.m_DownWeldWidth);
	pDownWeldWidth->Puttext((_bstr_t)str);
	pRoot->appendChild(pDownWeldWidth);

	//创建节点<DownWeldHeight></DownWeldHeight>
	MSXML2::IXMLDOMNodePtr pDownWeldHeight;
	pDownWeldHeight = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"DownWeldHeight", (_bstr_t)(char*)"");
	sprintf(str, "%lf", plate.m_DownWeldHeight);
	pDownWeldHeight->Puttext((_bstr_t)str);
	pRoot->appendChild(pDownWeldHeight);


	//创建节点<GrooveType></GrooveType>
	std::string str1;
	MSXML2::IXMLDOMNodePtr pGrooveType;
	pGrooveType = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveType", (_bstr_t)(char*)"");
	switch (groove.m_GrooveType)
	{
	case GrooveType::GrooveI:
		str1 = "GrooveI";
		break;
	case GrooveType::GrooveV:
		str1 = "GrooveV";
		break;
	case GrooveType::GrooveVV:
		str1 = "GrooveVV";
		break;
	case GrooveType::GrooveY:
		str1 = "GrooveY";
		break;
	case GrooveType::GrooveYY:
		str1 = "GrooveYY";
		break;
	default:
		break;
	}
	pGrooveType->Puttext((_bstr_t)str1.c_str());
	pRoot->appendChild(pGrooveType);

	//创建节点<GrooveIb></GrooveIb>
	MSXML2::IXMLDOMNodePtr pGrooveIb;
	pGrooveIb = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveIb", (_bstr_t)(char*)"");
	sprintf(str, "%lf", groove.m_GrooveIb);
	pGrooveIb->Puttext((_bstr_t)str);
	pRoot->appendChild(pGrooveIb);

	//创建节点<GrooveYb></GrooveYb>
	MSXML2::IXMLDOMNodePtr pGrooveYb;
	pGrooveYb = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveYb", (_bstr_t)(char*)"");
	sprintf(str, "%lf", groove.m_GrooveYb);
	pGrooveYb->Puttext((_bstr_t)str);
	pRoot->appendChild(pGrooveYb);

	//创建节点<GrooveYP></GrooveYP>
	MSXML2::IXMLDOMNodePtr pGrooveYP;
	pGrooveYP = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveYP", (_bstr_t)(char*)"");
	sprintf(str, "%lf", groove.m_GrooveYP);
	pGrooveYP->Puttext((_bstr_t)str);
	pRoot->appendChild(pGrooveYP);

	//创建节点<GrooveVa></GrooveVa>
	MSXML2::IXMLDOMNodePtr pGrooveVa;
	pGrooveVa = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveVa", (_bstr_t)(char*)"");
	sprintf(str, "%lf", groove.m_GrooveVa);
	pGrooveVa->Puttext((_bstr_t)str);
	pRoot->appendChild(pGrooveVa);

	//创建节点<GrooveVb></GrooveVb>
	MSXML2::IXMLDOMNodePtr pGrooveVb;
	pGrooveVb = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveVb", (_bstr_t)(char*)"");
	sprintf(str, "%lf", groove.m_GrooveVb);
	pGrooveVb->Puttext((_bstr_t)str);
	pRoot->appendChild(pGrooveVb);

	//创建节点<GrooveYYb></GrooveYYb>
	MSXML2::IXMLDOMNodePtr pGrooveYYb;
	pGrooveYYb = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveYYb", (_bstr_t)(char*)"");
	sprintf(str, "%lf", groove.m_GrooveYYb);
	pGrooveYYb->Puttext((_bstr_t)str);
	pRoot->appendChild(pGrooveYYb);

	//创建节点<GrooveYYP></GrooveYYP>
	MSXML2::IXMLDOMNodePtr pGrooveYYP;
	pGrooveYYP = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveYYP", (_bstr_t)(char*)"");
	sprintf(str, "%lf", groove.m_GrooveYYP);
	pGrooveYYP->Puttext((_bstr_t)str);
	pRoot->appendChild(pGrooveYYP);

	//创建节点<GrooveVa></GrooveVVb>
	MSXML2::IXMLDOMNodePtr pGrooveVVb;
	pGrooveVVb = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveVVb", (_bstr_t)(char*)"");
	sprintf(str, "%lf", groove.m_GrooveVVb);
	pGrooveVVb->Puttext((_bstr_t)str);
	pRoot->appendChild(pGrooveVVb);

	//创建节点<GrooveVb></GrooveVVH>
	MSXML2::IXMLDOMNodePtr pGrooveVVH;
	pGrooveVVH = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"GrooveVVH", (_bstr_t)(char*)"");
	sprintf(str, "%lf", groove.m_GrooveVVH);
	pGrooveVVH->Puttext((_bstr_t)str);
	pRoot->appendChild(pGrooveVVH);
}

//设置角接试件的信息
void CSoftwareUTInfoWriter::Impl::SetFilletWeldInfo(const FilletWeldInfo& fillet)
{
	char str[100];

	//创建节点<MotherWidth></MotherWidth>
	MSXML2::IXMLDOMNodePtr pMotherWidth;
	pMotherWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"MotherWidth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_MotherWidth);
	pMotherWidth->Puttext((_bstr_t)str);
	pRoot->appendChild(pMotherWidth);

	//创建节点<MotherThickness></MotherThickness>
	MSXML2::IXMLDOMNodePtr pMotherThickness;
	pMotherThickness = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"MotherThickness", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_MotherThickness);
	pMotherThickness->Puttext((_bstr_t)str);
	pRoot->appendChild(pMotherThickness);

	//创建节点<WebAngle></WebAngle>
	MSXML2::IXMLDOMNodePtr pWebAngle;
	pWebAngle = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WebAngle", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_WebAngle);
	pWebAngle->Puttext((_bstr_t)str);
	pRoot->appendChild(pWebAngle);

	//创建节点<WebWidth></WebWidth>
	MSXML2::IXMLDOMNodePtr pWebWidth;
	pWebWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WebWidth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_WebWidth);
	pWebWidth->Puttext((_bstr_t)str);
	pRoot->appendChild(pWebWidth);

	//创建节点<WebThickness></WebThickness>
	MSXML2::IXMLDOMNodePtr pWebThickness;
	pWebThickness = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"WebThickness", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_WebThickness);
	pWebThickness->Puttext((_bstr_t)str);
	pRoot->appendChild(pWebThickness);

	//创建节点<BlunteEdgeOffset></BlunteEdgeOffset>
	MSXML2::IXMLDOMNodePtr pBlunteEdgeOffset;
	pBlunteEdgeOffset = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"BlunteEdgeOffset", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_BlunteEdgeOffset);
	pBlunteEdgeOffset->Puttext((_bstr_t)str);
	pRoot->appendChild(pBlunteEdgeOffset);

	//创建节点<BlunteEdgeHeight></BlunteEdgeHeight>
	MSXML2::IXMLDOMNodePtr pBlunteEdgeHeight;
	pBlunteEdgeHeight = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"BlunteEdgeHeight", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_BlunteEdgeHeight);
	pBlunteEdgeHeight->Puttext((_bstr_t)str);
	pRoot->appendChild(pBlunteEdgeHeight);

	//创建节点<BlunteEdgeWidth></BlunteEdgeWidth>
	MSXML2::IXMLDOMNodePtr pBlunteEdgeWidth;
	pBlunteEdgeWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"BlunteEdgeWidth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_BlunteEdgeWidth);
	pBlunteEdgeWidth->Puttext((_bstr_t)str);
	pRoot->appendChild(pBlunteEdgeWidth);

	//创建节点<LeftWeldAngle></LeftWeldAngle>
	MSXML2::IXMLDOMNodePtr pLeftWeldAngle;
	pLeftWeldAngle = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"LeftWeldAngle", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_LeftWeldAngle);
	pLeftWeldAngle->Puttext((_bstr_t)str);
	pRoot->appendChild(pLeftWeldAngle);

	//创建节点<RightWeldAngle></RightWeldAngle>
	MSXML2::IXMLDOMNodePtr pRightWeldAngle;
	pRightWeldAngle = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RightWeldAngle", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_RightWeldAngle);
	pRightWeldAngle->Puttext((_bstr_t)str);
	pRoot->appendChild(pRightWeldAngle);

	//创建节点<LeftWeldWidth></LeftWeldWidth>
	MSXML2::IXMLDOMNodePtr pLeftWeldWidth;
	pLeftWeldWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"LeftWeldWidth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_LeftWeldWidth);
	pLeftWeldWidth->Puttext((_bstr_t)str);
	pRoot->appendChild(pLeftWeldWidth);

	//创建节点<RightWeldWidth></RightWeldWidth>
	MSXML2::IXMLDOMNodePtr pRightWeldWidth;
	pRightWeldWidth = pDoc->createNode((_variant_t)(long)NODE_ELEMENT, (_bstr_t)(char*)"RightWeldWidth", (_bstr_t)(char*)"");
	sprintf(str, "%lf", fillet.m_RightWeldWidth);
	pRightWeldWidth->Puttext((_bstr_t)str);
	pRoot->appendChild(pRightWeldWidth);
}
