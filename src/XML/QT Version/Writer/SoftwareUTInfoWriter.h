#pragma once

#include "PADefinition.h"

#include <string>

#include <QDomDocument>

class CSoftwareUTInfoWriter
{
//公有数据访问方法
public:
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

//公有构造及析构方法
public:
	//构造函数
	CSoftwareUTInfoWriter(const std::string& name);

	//析构函数
	~CSoftwareUTInfoWriter();

//私有成员变量
private:
	//XML文档节点
	QDomDocument doc;

	//XML根节点
	QDomElement root;

	std::string m_FilePathName;
};
