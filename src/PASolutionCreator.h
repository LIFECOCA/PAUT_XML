#pragma once

#include "PADefinition.h"

//相控阵检测方案生成子
class PASolutionCreator
{
public:
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

public:
	//构造函数
	PASolutionCreator();

	//析构函数
	~PASolutionCreator();

private:
	struct Impl;
	Impl* const m_pImpl;

	//禁止拷贝构造以及赋值
	PASolutionCreator(const PASolutionCreator& another) = delete;
	PASolutionCreator(PASolutionCreator&& another) = delete;
	PASolutionCreator& operator=(const PASolutionCreator& another) = delete;
	PASolutionCreator& operator=(PASolutionCreator&& another) = delete;
};