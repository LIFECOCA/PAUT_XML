#include "PASolutionCreator.h"

#include <intrin.h>

#include <vector>
#include <algorithm>
#include <iostream>

#include "SoftwareUTInfoWriter.h"
#include "TransmitFocalLawInfoWriter.h"
#include "ReceiveFocalLawInfoWriter.h"

#pragma warning(disable: 4996)

struct PASolutionCreator::Impl
{
	//创建相控阵超声检测检测方案配置文件
	void createPAInspectSolution(const char* file);

	//创造透射式相控阵超声检测方案配置文件
	void createTRPAInspectSolution(const char* file, double c1, double c2, double pitch, double TH, double RH, double TAngle, double RAngle, double TFocus, double RFocus, int NChannel, int TAPSize, int TAPStart, int TAPEnd, int RAPSize, int RAPStart, int RAPEnd);

	//设置换能器、楔块、扫查方案等信息
	void setPAInspectInfo(ProbeInfo* probe, WedgeInfo* wedge, PositionInfo* position, ScanInfo* scan, int num);

	//获取超声参数
	void getPAUTInfo(EncoderInfo& encoder, FilterInfo& filter, double& AGain, double& DGain, double& width, int& divisor);

	//设置超声参数
	void setPAUTInfo(EncoderInfo encoder, FilterInfo filter, double AGain, double DGain, double width, int divisor);

	//设置闸门
	void setGateInfo(double start, double range, double threshold, int GateID, int ChannelID, int GroupID);

	//获得平板对接焊缝信息
	void getPlateWeldInfo(PlateWeldInfo& plate, GrooveInfo& groove, MaterialInfo& material);

	//将检测方案的被检测对象设置为平板对接焊缝
	void setPlateWeldInfo(PlateWeldInfo plate, GrooveInfo groove, MaterialInfo material);

	//获得角接焊缝信息
	void getFilletWeldInfo(FilletWeldInfo& fillet, MaterialInfo& material);

	//将检测方案的被检测对象设置为角接焊缝
	void setFilletWeldInfo(FilletWeldInfo fillet, MaterialInfo material);

	//构造函数
	Impl();

	//析构函数
	~Impl();

	//生成软件配置信息
	void CreateSoftwareUTInfo(const char* file);

	//生成声束发射聚焦法则配置信息
	void CreateBaseFocalLawInfo(const char* file);

	//生成声束接收聚焦法则配置信息
	void CreateAdvancedFocalLawInfo(const char* file);

	//已知声束路径上的两点，两点分别位于两不同的介质之上，求解该声束的在两点间传播所花费的时间
	double CalTransmitTime(double h1, double h2, double w, double v1, double v2);

	//计算某个通道的声束发射点中心距离试件表面的距离
	double CalHeight2Part(int element, int size, ProbeInfo pinfo, WedgeInfo winfo);

	//计算楔块延迟时间
	double CalWedgeDelay(int element, int size, double angle, double vw, double vp, ProbeInfo pinfo, WedgeInfo winfo);

	//显示进度
	void progressBar(int n);

	//已知检测通道求取该通道的延时聚焦法则
	std::vector<double> GenerateFocalLaw(int element, int size, double angle, double deep, double vw, double vp, ProbeInfo pinfo, WedgeInfo winfo);

	//各个组的探头信息
	std::vector<ProbeInfo> m_Probe;

	//各个组的楔块信息
	std::vector<WedgeInfo> m_Wedge;

	//各个组的检测位置信息
	std::vector<PositionInfo> m_Position;

	//各个组的扫查信息
	std::vector<ScanInfo> m_Scan;

	//各个组的闸门信息
	std::vector<std::vector<GateInfo>> m_Gate;

	//各个组的采集信号深度
	std::vector<double> m_AScanDepth;

	//材料信息
	MaterialInfo m_Material;

	//编码器信息
	EncoderInfo m_Encoder;

	//滤波器信息
	FilterInfo m_Filter;

	//试件几何信息
	PartType m_PartType;
	PlateWeldInfo m_Plate;
	GrooveInfo m_Groove;
	FilletWeldInfo m_FilletInfo;

	//组数
	int m_GroupNum;

	//shot数
	int m_ShotNum;

	//采样分频数
	int m_Divisor;

	//超声参数
	double m_AGain, m_DGain, m_Width;
};

//构造函数
PASolutionCreator::PASolutionCreator() : m_pImpl(new Impl())
{

}

//析构函数
PASolutionCreator::~PASolutionCreator()
{
	delete m_pImpl;
}

//创建相控阵超声检测检测方案配置文件
void PASolutionCreator::createPAInspectSolution(const char* file)
{
	m_pImpl->createPAInspectSolution(file);
}

//创造透射式相控阵超声检测方案配置文件
void PASolutionCreator::createTRPAInspectSolution(const char* file, double c1, double c2, double pitch, double TH, double RH, double TAngle, double RAngle, double TFocus, double RFocus, int NChannel, int TAPSize, int TAPStart, int TAPEnd, int RAPSize, int RAPStart, int RAPEnd)
{
	m_pImpl->createTRPAInspectSolution(file, c1, c2, pitch, TH, RH, TAngle, RAngle, TFocus, RFocus, NChannel, TAPSize, TAPStart, TAPEnd, RAPSize, RAPStart, RAPEnd);
}

//设置换能器、楔块、扫查方案等信息
void PASolutionCreator::setPAInspectInfo(ProbeInfo* probe, WedgeInfo* wedge, PositionInfo* position, ScanInfo* scan, int num)
{
	m_pImpl->setPAInspectInfo(probe, wedge, position, scan, num);
}

//获取超声参数
void PASolutionCreator::getPAUTInfo(EncoderInfo& encoder, FilterInfo& filter, double& AGain, double& DGain, double& width, int& divisor)
{
	m_pImpl->getPAUTInfo(encoder, filter, AGain, DGain, width, divisor);
}

//设置超声参数
void PASolutionCreator::setPAUTInfo(EncoderInfo encoder, FilterInfo filter, double AGain, double DGain, double width, int divisor)
{
	m_pImpl->setPAUTInfo(encoder, filter, AGain, DGain, width, divisor);
}

//设置闸门
void PASolutionCreator::setGateInfo(double start, double range, double threshold, int GateID, int ChannelID, int GroupID)
{
	m_pImpl->setGateInfo(start, range, threshold, GateID, ChannelID, GroupID);
}

//获得平板对接焊缝信息
void PASolutionCreator::getPlateWeldInfo(PlateWeldInfo& plate, GrooveInfo& groove, MaterialInfo& material)
{
	m_pImpl->getPlateWeldInfo(plate, groove, material);
}

//将检测方案的被检测对象设置为平板对接焊缝
void PASolutionCreator::setPlateWeldInfo(PlateWeldInfo plate, GrooveInfo groove, MaterialInfo material)
{
	m_pImpl->setPlateWeldInfo(plate, groove, material);
}

//获得角接焊缝信息
void PASolutionCreator::getFilletWeldInfo(FilletWeldInfo& fillet, MaterialInfo& material)
{
	m_pImpl->getFilletWeldInfo(fillet, material);
}

//将检测方案的被检测对象设置为角接焊缝
void PASolutionCreator::setFilletWeldInfo(FilletWeldInfo fillet, MaterialInfo material)
{
	m_pImpl->setFilletWeldInfo(fillet, material);
}

//构造函数
PASolutionCreator::Impl::Impl()
{
	//默认参数
	m_Divisor = 1;
	m_AGain = 30;
	m_DGain = 5;
	m_Filter.m_FilterType = FilterType::eBandPassFilter;
	m_Filter.m_Frequency1 = 2e6;
	m_Filter.m_Frequency2 = 5e6;
	m_Encoder.m_ScanPointNum = 1;
	m_Encoder.m_IndexPointNum = 1;
	m_Encoder.m_ScanRange = 0;
	m_Encoder.m_IndexRange = 0;
	m_Encoder.m_ScanStep = 1;
	m_Encoder.m_IndexStep = 1;
	m_Encoder.m_EncoderResolution[0] = m_Encoder.m_EncoderResolution[1] = 200;
	m_Encoder.m_EncoderPolarity[0] = m_Encoder.m_EncoderPolarity[1] = 0;
	m_Encoder.m_ScanAxisID = 0;

	///////////////////////////////////工件信息////////////////////////////////////////
	//平板对接焊几何信息
	m_Plate.m_MotherWidth = 800;
	m_Plate.m_MotherThickness = 80;
	m_Plate.m_UpWeldWidth = 18;
	m_Plate.m_UpWeldHeight = 3;
	m_Plate.m_DownWeldWidth = 8;
	m_Plate.m_DownWeldHeight = 2;

	//平板对接焊坡口信息
	m_Groove.m_GrooveType = GrooveType::GrooveYY;
	m_Groove.m_GrooveIb = 18;
	m_Groove.m_GrooveVa = 18;
	m_Groove.m_GrooveVb = 8;
	m_Groove.m_GrooveYb = 8;
	m_Groove.m_GrooveYP = 8;
	m_Groove.m_GrooveYYb = 4;
	m_Groove.m_GrooveYYP = 6;
	m_Groove.m_GrooveVVb = 4;
	m_Groove.m_GrooveVVH = 15;

	//材料信息
	m_Material.m_MaterialID = 47;
	strcpy(m_Material.m_MaterialName, "STEEL MILD(软钢)");
	m_Material.m_LongitudinalVelocity = 5890;
	m_Material.m_ShearVelocity = 3240;
	m_Material.m_Thickness = 50;
	m_Material.m_Type = PartType::PWeld;
	m_PartType = PartType::PWeld;
}

//析构函数
PASolutionCreator::Impl::~Impl()
{

}

//创建相控阵超声检测检测方案配置文件
void PASolutionCreator::Impl::createPAInspectSolution(const char* file)
{
	CreateSoftwareUTInfo(file);
	CreateBaseFocalLawInfo(file);
	CreateAdvancedFocalLawInfo(file);
}

//创造透射式相控阵超声检测方案配置文件
void PASolutionCreator::Impl::createTRPAInspectSolution(const char* file, double c1, double c2, double pitch, double TH, double RH, double TAngle, double RAngle, double TFocus, double RFocus, int NChannel, int TAPSize, int TAPStart, int TAPEnd, int RAPSize, int RAPStart, int RAPEnd)
{
	//探头信息
	ProbeInfo probe;
	probe.m_ProbeID = 121;
	strcpy(probe.m_ProbeName, "5L64-A2");
	probe.m_Height = 5;
	probe.m_Pitch = pitch;
	probe.m_ElementNum = 128;
	probe.m_Frequency = 3.5;
	probe.m_FirstConnection = 1;

	WedgeInfo wedge;
	wedge.m_WedgeID = 544;
	strcpy(wedge.m_WedgeName, "SA2-N552-IHC dual 5L64");
	wedge.m_WedgeAngle = 0;
	wedge.m_h1 = 0;
	wedge.m_ProbeOffsetX = 11.73;
	wedge.m_ProbeOffsetY = 20;
	wedge.m_h2 = 0;
	wedge.m_w1 = 68.53;
	wedge.m_Velocity = c1;

	PositionInfo position;
	position.m_InspectPos = InspectPos::WeldPDetectPos_UpR;
	position.m_WedgeOffset = 20;

	ScanInfo scan;
	scan.m_Type = ScanType::ePALinear;
	scan.m_VelocityType = VelocityType::eLongitudinal;
	scan.m_Reflect = 0;
	scan.m_ChannelSize = TAPSize;
	scan.m_ChannelNum = NChannel;
	scan.m_AngleStart = 0;
	scan.m_AngleStop = 0;
	scan.m_ElementStart = 1;
	scan.m_ElementStop = 25;
	scan.m_SendFocus = TFocus;
	scan.m_RecvFocusStart = RFocus;
	scan.m_RecvFocusEnd = 0;
	scan.m_RecvFocusNum = 1;
	scan.m_FocalType = 3;

	m_Material.m_LongitudinalVelocity = c2;

	int TApertureSize = TAPSize;
	int RApertureSize = RAPSize;
	int TApertureStart = TAPStart;
	int TApertureStop = TAPEnd;
	int RApertureStart = RAPStart;
	int RApertureStop = RAPEnd;
	//int RApertureStart = 1;
	//int RApertureStop = 25;
	int ChannelStep = 1;
	double TWedgeH = TH;
	double RWedgeH = RH;
	double vw = wedge.m_Velocity;
	double vp = m_Material.m_LongitudinalVelocity;

	GateInfo gate;
	gate.m_GateIsUsed = false;

	double AScanDepth = 0;

	//总体
	{
		CSoftwareUTInfoWriter writer(file);
		//获得当前检测方案的总组数
		m_GroupNum = 1;
		//计算总shot数
		m_ShotNum = NChannel;
		writer.SetGroupNum(m_GroupNum);
		writer.SetMaterialInfo(m_Material);
		m_AScanDepth.resize(m_GroupNum);

		GroupBasicInfo group;
		BeamBasicInfo beam;

		group.m_ScanType = ScanType::ePA;
		group.m_ChannelNum = NChannel;
		group.m_PartVelocity = vp;
		group.m_TRMode = TRMode::eTransmitReceive;
		group.m_SendFocus = TFocus;
		group.m_RecvFocusNum = 1;
		group.m_RecvFocusStart = RFocus;
		group.m_RecvFocusEnd = RFocus;
		group.m_FocalType = scan.m_FocalType;
		writer.SetGroupBasicInfo(group, 1);
		writer.SetProbeInfo(probe, 1);
		writer.SetWedgeInfo(wedge, 1);
		writer.SetPositionInfo(position, 1);
		writer.SetScanInfo(scan, 1);

		//建立当前组每次检测角度数组
		std::vector<double> angle(NChannel);
		for (int i = 0; i < NChannel; i++)
		{
			angle[i] = TAngle;
		}
		std::vector<int> element(NChannel);
		for (int i = 0; i < NChannel; i++)
		{
			element[i] = TApertureStart + i;
		}

		//填写相应的信息
		for (int i = 1; i <= NChannel; i++)
		{
			beam.m_BeamAngle = angle[i - 1];
			beam.m_CenterElementID = element[i - 1] + TApertureSize / 2 - 1;
			beam.m_WedgeDelay = CalWedgeDelay(element[i - 1], TApertureSize, angle[i - 1], vw, vp, probe, wedge);
			writer.SetBeamBasicInfo(beam, 1, i);
		}

		//闸门信息
		for (int j = 0; j < GateInfo::eMaxGateNum; j++)
		{
			writer.SetGateInfo(gate, 1, j);
		}

		//编码器信息
		m_Encoder.m_ScanPointNum = (int)(m_Encoder.m_ScanRange / m_Encoder.m_ScanStep + 1);
		m_Encoder.m_IndexPointNum = (int)(m_Encoder.m_IndexRange / m_Encoder.m_IndexStep + 1);
		writer.SetEncoderInfo(m_Encoder);

		//滤波器信息
		writer.SetFilterInfo(m_Filter);

		//试件几何信息
		writer.SetPartType(m_PartType);
		writer.SetPlateWeldInfo(m_Plate, m_Groove);
	}

	//发射
	{
		const double PI = 4 * atan(1.0);
		CTransmitFocalLawInfoWriter writer(file);

		HardwareInfo hardware;
		hardware.m_SamplingDivisor = m_Divisor;
		hardware.m_TransmitVoltage = m_AGain;
		hardware.m_UTFrequency = 2000;
		writer.SetHardwareInfo(hardware);

		ElementInfo EE;
		EE.m_Gain = m_DGain;
		EE.m_PulseWidth = m_Width;

		//建立当前组每次检测角度数组
		std::vector<double> angle(NChannel);
		for (int i = 0; i < NChannel; i++)
		{
			angle[i] = TAngle;
		}
		std::vector<int> element(NChannel);
		for (int i = 0; i < NChannel; i++)
		{
			element[i] = TApertureStart + i;
		}

		//计算每条声波的采样深度 选择最大的那个进行设置
		double TempAScanDepth;
		AScanDepth = 0;
		for (int i = 0; i < NChannel; i++)
		{
			TempAScanDepth = m_Material.m_Thickness / cos(angle[i] / 45 * atan(1.0)) / vp * 2000;
			if (TempAScanDepth > AScanDepth)
			{
				AScanDepth = TempAScanDepth;
			}
		}
		//点数规约为32的倍数
		double temp = ((int)(AScanDepth * 100 / m_Divisor / 32)) * 32.0 * m_Divisor / 100;
		AScanDepth = (temp < AScanDepth) ? temp + 0.32 * m_Divisor : temp;

		writer.SetShotNum(NChannel);

		//计算聚焦法则
		for (int i = 1; i <= NChannel; i++)
		{
			writer.SetElementNum(TApertureSize, i);
			writer.SetAScanDepth(AScanDepth, i);

			//计算发射延时聚焦法则
			double FocusDeep;
			switch (scan.m_FocalType)
			{
			case 1:
				FocusDeep = scan.m_SendFocus / tan(angle[i - 1] / 180 * PI);
				break;
			case 2:
				FocusDeep = scan.m_SendFocus;
				break;
			case 3:
				FocusDeep = scan.m_SendFocus * cos(angle[i - 1] / 180 * PI);
				break;
			default:
				break;
			}

			std::vector<double> law = GenerateFocalLaw(element[i - 1], TApertureSize, angle[i - 1], FocusDeep, vw, vp, probe, wedge);

			//填写相应的信息
			for (int j = 1; j <= TApertureSize; j++)
			{
				writer.SetTransmitElementID(element[i - 1] + j - 1, i, j);
				//设置发射延迟时间
				writer.SetTransmitDelay(law[j - 1], i, j);
				writer.SetElementInfo(EE, i, j);
			}
		}
	}

	//接收
	{
		const double PI = 4 * atan(1.0);
		CReceiveFocalLawInfoWriter writer(file);

		//建立当前组每次检测角度数组
		std::vector<double> angle(NChannel);
		for (int i = 0; i < NChannel; i++)
		{
			angle[i] = RAngle;
		}
		std::vector<int> element(NChannel);
		for (int i = 0; i < NChannel; i++)
		{
			element[i] = RApertureStart + i;
		}

		//建立当前组的接收动态聚焦数组
		double focusstep = (scan.m_RecvFocusNum == 1) ? 0 : (scan.m_RecvFocusEnd - scan.m_RecvFocusStart) / (scan.m_RecvFocusNum - 1);
		std::vector<double> focus(scan.m_RecvFocusNum);
		for (int i = 0; i < scan.m_RecvFocusNum; i++)
		{
			focus[i] = scan.m_RecvFocusStart + i * focusstep;
		}

		for (int i = 1; i <= NChannel; i++)
		{
			writer.SetSignalNum(1, i);
			writer.SetElementNum(RApertureSize, i, 1);
			for (int j = 1; j <= RApertureSize; j++)
			{
				writer.SetElementID(element[i - 1] + j - 1, i, 1, j);
			}
			writer.SetFocalLawNum(scan.m_RecvFocusNum, i, 1);
			for (int j = 1; j <= scan.m_RecvFocusNum; j++)
			{
				//计算发射延时聚焦法则
				double FocusDeep;
				switch (scan.m_FocalType)
				{
				case 1:
					FocusDeep = focus[j - 1] / tan(angle[i - 1] / 180 * PI);
					break;
				case 2:
					FocusDeep = focus[j - 1];
					break;
				case 3:
					FocusDeep = focus[j - 1] * cos(angle[i - 1] / 180 * PI);
					break;
				default:
					break;
				}

				//计算聚焦法则
				std::vector<double> law = GenerateFocalLaw(element[i - 1], RApertureSize, angle[i - 1], FocusDeep, vw, vp, probe, wedge);
				for (int k = 1; k <= RApertureSize; k++)
				{
					writer.SetElementDelay(law[k - 1], i, 1, j, k);
				}
				writer.SetTotalFocalTime(law[RApertureSize], i, 1, j);
			}
		}
	}
}


//设置换能器、楔块、扫查方案等信息
void PASolutionCreator::Impl::setPAInspectInfo(ProbeInfo* probe, WedgeInfo* wedge, PositionInfo* position, ScanInfo* scan, int num)
{
	if (num > 0)
	{
		m_Probe.resize(num);
		m_Wedge.resize(num);
		m_Position.resize(num);
		m_Scan.resize(num);
		std::copy(probe, probe + num, std::begin(m_Probe));
		std::copy(wedge, wedge + num, std::begin(m_Wedge));
		std::copy(position, position + num, std::begin(m_Position));
		std::copy(scan, scan + num, std::begin(m_Scan));

		m_Gate.clear();
		m_Gate.resize(num, std::vector<GateInfo>(GateInfo::eMaxGateNum));
		for (int i = 0; i < num; i++)
		{
			for (int j = 0; j < GateInfo::eMaxGateNum; j++)
			{
				m_Gate[i][j].m_GateIsUsed = false;
				m_Gate[i][j].m_GateType = 0;
				m_Gate[i][j].m_GateStart = 0;
				m_Gate[i][j].m_GateRange = 0;
				m_Gate[i][j].m_GateThreshold = 0;
			}
		}
	}
}

//获取超声参数
void PASolutionCreator::Impl::getPAUTInfo(EncoderInfo& encoder, FilterInfo& filter, double& AGain, double& DGain, double& width, int& divisor)
{
	encoder = m_Encoder;
	filter = m_Filter;
	AGain = m_AGain;
	DGain = m_DGain;
	width = m_Width;
	divisor = m_Divisor;
}

//设置超声参数
void PASolutionCreator::Impl::setPAUTInfo(EncoderInfo encoder, FilterInfo filter, double AGain, double DGain, double width, int divisor)
{
	m_Encoder = encoder;
	m_Filter = filter;
	m_AGain = AGain;
	m_DGain = DGain;
	m_Width = width;
	m_Divisor = divisor;
}

//设置闸门
void PASolutionCreator::Impl::setGateInfo(double start, double range, double threshold, int GateID, int ChannelID, int GroupID)
{

}

//获得平板对接焊缝信息
void PASolutionCreator::Impl::getPlateWeldInfo(PlateWeldInfo& plate, GrooveInfo& groove, MaterialInfo& material)
{
	plate = m_Plate;
	groove = m_Groove;
	material = m_Material;
}

//将检测方案的被检测对象设置为平板对接焊缝
void PASolutionCreator::Impl::setPlateWeldInfo(PlateWeldInfo plate, GrooveInfo groove, MaterialInfo material)
{
	m_PartType = PartType::PWeld;
	m_Plate = plate;
	m_Groove = groove;
	m_Material = material;
}

//获得角接焊缝信息
void PASolutionCreator::Impl::getFilletWeldInfo(FilletWeldInfo& fillet, MaterialInfo& material)
{
	fillet = m_FilletInfo;
	material = m_Material;
}

//将检测方案的被检测对象设置为角接焊缝
void PASolutionCreator::Impl::setFilletWeldInfo(FilletWeldInfo fillet, MaterialInfo material)
{
	m_PartType = PartType::FWeld;
	m_FilletInfo = fillet;
	m_Material = material;
}

//生成软件配置信息
void PASolutionCreator::Impl::CreateSoftwareUTInfo(const char* file)
{
	CSoftwareUTInfoWriter writer(file);
	//获得当前检测方案的总组数
	m_GroupNum = (int)m_Scan.size();
	//计算总shot数
	m_ShotNum = 0;
	for (int i = 0; i < m_GroupNum; i++)
	{
		if (m_Scan[i].m_Type == ScanType::eTOFD)
		{
			m_ShotNum += 1;
		}
		else
		{
			m_ShotNum += m_Scan[i].m_ChannelNum;
		}
	}
	writer.SetGroupNum(m_GroupNum);
	writer.SetMaterialInfo(m_Material);
	m_AScanDepth.resize(m_GroupNum);

	GroupBasicInfo group;
	BeamBasicInfo beam;

	for (int i = 0; i < m_GroupNum; i++)
	{
		double vp;
		switch (m_Scan[i].m_VelocityType)
		{
		case VelocityType::eLongitudinal:
			vp = m_Material.m_LongitudinalVelocity;
			break;
		case VelocityType::eShear:
			vp = m_Material.m_ShearVelocity;
			break;
		}

		if (m_Scan[i].m_Type == ScanType::eTOFD)
		{
			group.m_ScanType = ScanType::eTOFD;
			group.m_ChannelNum = 1;
		}
		else
		{
			group.m_ScanType = ScanType::ePA;
			group.m_ChannelNum = m_Scan[i].m_ChannelNum;
		}

		group.m_PartVelocity = vp;
		group.m_TRMode = TRMode::eTransmitReceive;
		group.m_SendFocus = m_Scan[i].m_SendFocus;
		group.m_RecvFocusNum = m_Scan[i].m_RecvFocusNum;
		group.m_RecvFocusStart = m_Scan[i].m_RecvFocusStart;
		group.m_RecvFocusEnd = m_Scan[i].m_RecvFocusEnd;
		group.m_FocalType = m_Scan[i].m_FocalType;
		writer.SetGroupBasicInfo(group, i + 1);
		writer.SetProbeInfo(m_Probe[i], i + 1);
		writer.SetWedgeInfo(m_Wedge[i], i + 1);
		writer.SetPositionInfo(m_Position[i], i + 1);
		writer.SetScanInfo(m_Scan[i], i + 1);

		if (m_Scan[i].m_Type == ScanType::eTOFD)
		{
			beam.m_BeamAngle = 0;
			beam.m_CenterElementID = 1;
			beam.m_WedgeDelay = 0;
			writer.SetBeamBasicInfo(beam, i + 1, 1);
		}
		else
		{
			//建立当前组每次检测角度数组
			double anglestep = (m_Scan[i].m_ChannelNum == 1) ? 0 : (m_Scan[i].m_AngleStop - m_Scan[i].m_AngleStart) / (m_Scan[i].m_ChannelNum - 1);
			std::vector<double> angle(m_Scan[i].m_ChannelNum);
			for (int j = 0; j < m_Scan[i].m_ChannelNum; j++) angle[j] = m_Scan[i].m_AngleStart + j * anglestep;
			//建立当前组每次检测起始振源数组
			int elementstep = (m_Scan[i].m_ChannelNum == 1) ? 0 : (m_Scan[i].m_ElementStop - m_Scan[i].m_ElementStart) / (m_Scan[i].m_ChannelNum - 1);
			std::vector<int> element(m_Scan[i].m_ChannelNum);
			for (int j = 0; j < m_Scan[i].m_ChannelNum; j++) element[j] = m_Scan[i].m_ElementStart + j * elementstep;
			//声束发射点距离试件表面距离
			//填写相应的信息
			for (int j = 1; j <= m_Scan[i].m_ChannelNum; j++)
			{
				beam.m_BeamAngle = angle[j - 1];
				beam.m_CenterElementID = element[j - 1] + m_Scan[i].m_ChannelSize / 2 - 1;
				beam.m_WedgeDelay = CalWedgeDelay(element[j - 1], m_Scan[i].m_ChannelSize, angle[j - 1], m_Wedge[i].m_Velocity, vp, m_Probe[i], m_Wedge[i]);
				writer.SetBeamBasicInfo(beam, i + 1, j);
			}
		}

		//闸门信息
		for (int j = 0; j < GateInfo::eMaxGateNum; j++)
		{
			writer.SetGateInfo(m_Gate[i][j], i + 1, j);
		}
	}

	//编码器信息
	m_Encoder.m_ScanPointNum = (int)(m_Encoder.m_ScanRange / m_Encoder.m_ScanStep + 1);
	m_Encoder.m_IndexPointNum = (int)(m_Encoder.m_IndexRange / m_Encoder.m_IndexStep + 1);
	writer.SetEncoderInfo(m_Encoder);

	//滤波器信息
	writer.SetFilterInfo(m_Filter);

	//试件几何信息
	writer.SetPartType(m_PartType);
	switch (m_PartType)
	{
	case PartType::PWeld:
		writer.SetPlateWeldInfo(m_Plate, m_Groove);
		break;
	case PartType::FWeld:
		writer.SetFilletWeldInfo(m_FilletInfo);
		break;
	default:
		break;
	}
}

//生成声束发射聚焦法则配置信息
void PASolutionCreator::Impl::CreateBaseFocalLawInfo(const char* file)
{
	const double PI = 4 * atan(1.0);
	CTransmitFocalLawInfoWriter writer(file);

	HardwareInfo hardware;
	hardware.m_SamplingDivisor = m_Divisor;
	hardware.m_TransmitVoltage = m_AGain;
	hardware.m_UTFrequency = 2000;
	writer.SetHardwareInfo(hardware);

	ElementInfo EE;
	EE.m_Gain = m_DGain;
	EE.m_PulseWidth = m_Width;

	/////////////////////////////////聚焦法则计算相关变量定义///////////////////////////
	writer.SetShotNum(m_ShotNum);
	int index = 1;
	for (int p = 0; p < m_GroupNum; p++)
	{
		double vp;
		//根据工件声速及厚度计算采样深度
		switch (m_Scan[p].m_VelocityType)
		{
		case VelocityType::eLongitudinal:
			vp = m_Material.m_LongitudinalVelocity;
			break;
		case VelocityType::eShear:
			vp = m_Material.m_ShearVelocity;
			break;
		}

		if (m_Scan[p].m_Type == ScanType::eTOFD)
		{
			//计算每条声波的采样深度 选择最大的那个进行设置
			m_AScanDepth[p] = m_Material.m_Thickness / vp * 2000;
			//点数规约为32的倍数
			double temp = ((int)(m_AScanDepth[p] * 100 / m_Divisor / 32)) * 32.0 * m_Divisor / 100;
			m_AScanDepth[p] = (temp < m_AScanDepth[p]) ? temp + 0.32 * m_Divisor : temp;
			writer.SetElementNum(1, index);
			writer.SetAScanDepth(m_AScanDepth[p], index);
			writer.SetTransmitElementID(1, index, 1);
			writer.SetElementInfo(EE, index, 1);
			//设置发射延迟时间
			writer.SetTransmitDelay(0, index, 1);
			index++;
		}
		else
		{
			//确定楔块声速
			double vw = m_Wedge[p].m_Velocity;

			//建立当前组每次检测角度数组
			double anglestep = (m_Scan[p].m_ChannelNum == 1) ? 0 : (m_Scan[p].m_AngleStop - m_Scan[p].m_AngleStart) / (m_Scan[p].m_ChannelNum - 1);
			std::vector<double> angle(m_Scan[p].m_ChannelNum);
			for (int j = 0; j < m_Scan[p].m_ChannelNum; j++) angle[j] = m_Scan[p].m_AngleStart + j * anglestep;
			//建立当前组每次检测起始振源数组
			int elementstep = (m_Scan[p].m_ChannelNum == 1) ? 0 : (m_Scan[p].m_ElementStop - m_Scan[p].m_ElementStart) / (m_Scan[p].m_ChannelNum - 1);
			std::vector<int> element(m_Scan[p].m_ChannelNum);
			for (int j = 0; j < m_Scan[p].m_ChannelNum; j++) element[j] = m_Scan[p].m_ElementStart + j * elementstep;

			//计算每条声波的采样深度 选择最大的那个进行设置
			double TempAScanDepth;
			m_AScanDepth[p] = 0;
			for (int i = 0; i < m_Scan[p].m_ChannelNum; i++)
			{
				TempAScanDepth = m_Material.m_Thickness / cos(angle[i] / 45 * atan(1.0)) / vp * 2000;
				if (TempAScanDepth > m_AScanDepth[p])
				{
					m_AScanDepth[p] = TempAScanDepth;
				}
			}
			//点数规约为32的倍数
			double temp = ((int)(m_AScanDepth[p] * 100 / m_Divisor / 32)) * 32.0 * m_Divisor / 100;
			m_AScanDepth[p] = (temp < m_AScanDepth[p]) ? temp + 0.32 * m_Divisor : temp;

			//计算聚焦法则
			for (int i = 1; i <= m_Scan[p].m_ChannelNum; i++)
			{
				writer.SetElementNum(m_Scan[p].m_ChannelSize, index);
				writer.SetAScanDepth(m_AScanDepth[p], index);


				//计算发射延时聚焦法则
				double FocusDeep;
				switch (m_Scan[p].m_FocalType)
				{
				case 1:
					FocusDeep = m_Scan[p].m_SendFocus / tan(angle[i - 1] / 180 * PI);
					break;
				case 2:
					FocusDeep = m_Scan[p].m_SendFocus;
					break;
				case 3:
					FocusDeep = m_Scan[p].m_SendFocus * cos(angle[i - 1] / 180 * PI);
					break;
				default:
					break;
				}

				std::vector<double> law = GenerateFocalLaw(element[i - 1], m_Scan[p].m_ChannelSize, angle[i - 1], FocusDeep, vw, vp, m_Probe[p], m_Wedge[p]);

				//填写相应的信息
				for (int j = 1; j <= m_Scan[p].m_ChannelSize; j++)
				{
					writer.SetTransmitElementID(element[i - 1] + j - 1, index, j);
					//设置发射延迟时间
					writer.SetTransmitDelay(law[j - 1], index, j);
					writer.SetElementInfo(EE, index, j);
				}
				index++;
			}
		}
	}
}

//生成声束接收聚焦法则配置信息
void PASolutionCreator::Impl::CreateAdvancedFocalLawInfo(const char* file)
{
	const double PI = 4 * atan(1.0);
	CReceiveFocalLawInfoWriter writer(file);
	int index = 1;
	for (int p = 0; p < m_GroupNum; p++)
	{
		if (m_Scan[p].m_Type == ScanType::eTOFD)
		{
			writer.SetSignalNum(1, index);
			writer.SetElementNum(1, index, 1);
			writer.SetElementID(2, index, 1, 1);
			writer.SetFocalLawNum(1, index, 1);
			writer.SetElementDelay(0, index, 1, 1, 1);
			writer.SetTotalFocalTime(0, index, 1, 1);
			index++;
		}
		else
		{
			//确定楔块声速
			double vw = m_Wedge[p].m_Velocity;

			//确定工件声速
			double vp;
			switch (m_Scan[p].m_VelocityType)
			{
			case VelocityType::eLongitudinal:
				vp = m_Material.m_LongitudinalVelocity;
				break;
			case VelocityType::eShear:
				vp = m_Material.m_ShearVelocity; 
				break;
			}

			//建立当前组的接收动态聚焦数组
			double focusstep = (m_Scan[p].m_RecvFocusNum == 1) ? 0 : (m_Scan[p].m_RecvFocusEnd - m_Scan[p].m_RecvFocusStart) / (m_Scan[p].m_RecvFocusNum - 1);
			std::vector<double> focus(m_Scan[p].m_RecvFocusNum);
			for (int i = 0; i < m_Scan[p].m_RecvFocusNum; i++)
			{
				focus[i] = m_Scan[p].m_RecvFocusStart + i * focusstep;
			}

			//建立当前组每次检测角度数组
			double anglestep = (m_Scan[p].m_ChannelNum == 1) ? 0 : (m_Scan[p].m_AngleStop - m_Scan[p].m_AngleStart) / (m_Scan[p].m_ChannelNum - 1);
			std::vector<double> angle(m_Scan[p].m_ChannelNum);
			for (int j = 0; j < m_Scan[p].m_ChannelNum; j++) angle[j] = m_Scan[p].m_AngleStart + j * anglestep;
			//建立当前组每次检测起始振源数组
			int elementstep = (m_Scan[p].m_ChannelNum == 1) ? 0 : (m_Scan[p].m_ElementStop - m_Scan[p].m_ElementStart) / (m_Scan[p].m_ChannelNum - 1);
			std::vector<int> element(m_Scan[p].m_ChannelNum);
			for (int j = 0; j < m_Scan[p].m_ChannelNum; j++) element[j] = m_Scan[p].m_ElementStart + j * elementstep;

			for (int i = 1; i <= m_Scan[p].m_ChannelNum; i++)
			{
				writer.SetSignalNum(1, index);
				writer.SetElementNum(m_Scan[p].m_ChannelSize, index, 1);
				for (int j = 1; j <= m_Scan[p].m_ChannelSize; j++)
				{
					writer.SetElementID(element[i - 1] + j - 1, index, 1, j);
				}
				writer.SetFocalLawNum(m_Scan[p].m_RecvFocusNum, index, 1);
				for (int j = 1; j <= m_Scan[p].m_RecvFocusNum; j++)
				{
					//计算发射延时聚焦法则
					double FocusDeep;
					switch (m_Scan[p].m_FocalType)
					{
					case 1:
						FocusDeep = focus[j - 1] / tan(angle[i - 1] / 180 * PI);
						break;
					case 2:
						FocusDeep = focus[j - 1];
						break;
					case 3:
						FocusDeep = focus[j - 1] * cos(angle[i - 1] / 180 * PI);
						break;
					default:
						break;
					}

					//计算聚焦法则
					std::vector<double> law = GenerateFocalLaw(element[i - 1], m_Scan[p].m_ChannelSize, angle[i - 1], FocusDeep, vw, vp, m_Probe[p], m_Wedge[p]);
					for (int k = 1; k <= m_Scan[p].m_ChannelSize; k++)
					{
						writer.SetElementDelay(law[k - 1], index, 1, j, k);
					}
					writer.SetTotalFocalTime(law[m_Scan[p].m_ChannelSize], index, 1, j);
				}
				//progressBar(index * 100 / m_ShotNum);
				index++;
			}
		}
	}
}

//已知声束路径上的两点，两点分别位于两不同的介质之上，求解该声束的在两点间传播所花费的时间
double PASolutionCreator::Impl::CalTransmitTime(double h1, double h2, double w, double v1, double v2)
{
	/*const double PI = 4 * atan(1.0);
	const double angle_step = 0.01;

	int direct = (w < 0) ? -1 : 1;
	double _w = abs(w);
	double time;
	double dist1, dist2;
	double angle;
	double min = 1e32;
	//算法有待该进
	for (int i = 0; i < 90 / angle_step; i++)
	{
		angle = i * angle_step / 180 * PI;
		dist1 = h1 / cos(angle);
		dist2 = abs(_w - h1 * tan(angle));
		dist2 = sqrt(dist2 * dist2 + h2 * h2);
		time = dist1 / v1 + dist2 / v2;
		if (time < min) min = time;
	}
	return min;*/

	const int count = 1024;
	const float step = 1.0f / (count - 1);
	__m256 _w = _mm256_set1_ps((float)(w * w));
	__m256 _h1 = _mm256_set1_ps((float)(h1 * h1));
	__m256 _h2 = _mm256_set1_ps((float)(h2 * h2));
	__m256 c1 = _mm256_set1_ps((float)v1);
	__m256 c2 = _mm256_set1_ps((float)v2);
	__m256 dt = _mm256_set1_ps(8 * step);
	__m256 MinValue = _mm256_set1_ps(1e10f);
	__m256 t1 = _mm256_setr_ps(0, step, 2 * step, 3 * step, 4 * step, 5 * step, 6 * step, 7 * step);
	//__m256 t2 = _mm256_setr_ps(1, 1 - step, 1 - 2 * step, 1 - 3 * step, 1 - 4 * step, 1 - 5 * step, 1 - 6 * step, 1 - 7 * step);
	__m256 t2 = _mm256_sub_ps(_mm256_set1_ps(1.0f), t1);
	__m256 discance1, discance2, time;
	for (int i = 0; i < count; i += 8)
	{
		//_w * t^2
		discance1 = _mm256_mul_ps(_w, _mm256_mul_ps(t1, t1));
		//sqrt(discance1 + _h1)
		discance1 = _mm256_sqrt_ps(_mm256_add_ps(discance1, _h1));
		//_w * (1 - t)^2
		discance2 = _mm256_mul_ps(_w, _mm256_mul_ps(t2, t2));
		//sqrt(discance2 + _h2)
		discance2 = _mm256_sqrt_ps(_mm256_add_ps(discance2, _h2));
		//time = discance1 / c1 + distance2 / c2
		time = _mm256_add_ps(_mm256_div_ps(discance1, c1), _mm256_div_ps(discance2, c2));
		//比较大小保留最小的那个
		MinValue = _mm256_min_ps(MinValue, time);
		//步进
		t1 = _mm256_add_ps(t1, dt);
		t2 = _mm256_sub_ps(t2, dt);
	}

	//将计算的结果拷贝至一个大小为8的float数组 定义导出的数组的时候要求内存对齐32(即一个AVX512寄存器所占据的字节总数) 从而能进一步提升效率
	__declspec(align(32)) float result[8];
	_mm256_store_ps(result, MinValue);

	//返回最小值
	return (*std::min_element(result, result + 8));
}

//计算某个通道的声束发射点中心距离试件表面的距离
double PASolutionCreator::Impl::CalHeight2Part(int element, int size, ProbeInfo pinfo, WedgeInfo winfo)
{
	const double PI = 4 * atan(1.0);

	std::vector<double> h(size);
	for (int i = 0; i < size; i++) h[i] = pinfo.m_Pitch * (element + i) * sin(winfo.m_WedgeAngle / 180 * PI) + winfo.m_ProbeOffsetY;
	return (size == 1) ? h[0] : (h[size / 2 - 1] + h[size / 2]) / 2;
}

//计算楔块延迟时间
double PASolutionCreator::Impl::CalWedgeDelay(int element, int size, double angle, double vw, double vp, ProbeInfo pinfo, WedgeInfo winfo)
{
	const double PI = 4 * atan(1.0);

	double h = CalHeight2Part(element, size, pinfo, winfo);
	/*snell定律*/
	double _anglep = angle / 180 * PI;
	double _anglew = asin(sin(_anglep) * vw / vp);
	//计算楔块延迟时间
	return h / cos(_anglew) / vw / 1e-3;
}

//已知检测通道求取该通道的延时聚焦法则
std::vector<double> PASolutionCreator::Impl::GenerateFocalLaw(int element, int size, double angle, double deep, double vw, double vp, ProbeInfo pinfo, WedgeInfo winfo)
{
	const double PI = 4 * atan(1.0);
	
	//计算提离高度
	std::vector<double> h(size);
	for (int i = 0; i < size; i++) h[i] = pinfo.m_Pitch * (element + i) * sin(winfo.m_WedgeAngle / 180 * PI) + winfo.m_ProbeOffsetY;
	double _h = (size == 1) ? h[0] : (h[size / 2 - 1] + h[size / 2]) / 2;

	//计算入射角以及入射点
	double _anglep = angle / 180 * PI;
	double _anglew = asin(sin(_anglep) * vw / vp);
	double _w = _h * tan(_anglew) + deep * tan(_anglep);

	//计算传播时间
	std::vector<double> w(size);
	std::vector<double> law(size);
	for (int i = 0; i < size; i++)
	{
		double w = _w - (i - size / 2 + 0.5) * pinfo.m_Pitch * cos(winfo.m_WedgeAngle * atan(1.0) / 45);
		law[i] = CalTransmitTime(h[i], deep, w, vw, vp) / 1e-3;
	}

	//计算度越时间(DDF需要设置的参数)
	double fimeoffilght = (size == 1) ? law[0] : (law[size / 2 - 1] + law[size / 2]) / 2;

	//最大传播时间
	double maxtime = *std::max_element(std::begin(law), std::end(law));
	//计算各阵元延迟时间
	std::for_each(std::begin(law), std::end(law), [maxtime](double& time){ time = maxtime - time; });
	law.push_back(fimeoffilght * 2);
	
	//以右值引用的形式返回
	return law;
}

void PASolutionCreator::Impl::progressBar(int n)
{
	using std::cout;
	using std::endl;
	if (n != 1)
	{
		if (n % 4 == 0)
		{
			for (int j = 1; j < (50 - ((n - 1) / 2) + 9); j++)
				cout << "\b";
			cout << "█";
			for (int j = 1; j <= (50 - (n / 2)); j++)
				cout << "_";
			cout << "|   ";
			if (n < 10)
				cout << " " << n << "%";
			else
				cout << n << "%";
		}

		else if (n % 2 == 0)
		{
			for (int j = 1; j <= (50 - ((n - 1) / 2) + 7); j++)
				cout << "\b";
			cout << "▌";
			for (int j = 1; j < (50 - (n / 2)); j++)
				cout << "_";
			cout << "|   ";
			if (n < 10)
				cout << " " << n << "%";
			else
				cout << n << "%";
		}
		else
		{
			cout << "\b\b\b";
			if (n < 10)
				cout << " " << n << "%";
			else
				cout << n << "%";
		}
	}
	else
	{
		cout << "检测方案生成进度：|";
		for (int j = 1; j < (50 - (n / 2)); j++)
			cout << "_";
		cout << "|     " << n << "%";
	}
	if (n == 100)
		cout << endl;
}