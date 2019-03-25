#pragma once

#include "PADefinition.h"

#include <string>
 
#include <QDomDocument>



//这是一个用于解析SoftwareUTInof文档信息的类
class CSoftwareUTInfoReader
{
//公有数据访问方法
public:
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

//公有构造方法及析构方法
public:
	//公有构造函数
	CSoftwareUTInfoReader(const std::string& name);

	//公有析构函数
	~CSoftwareUTInfoReader();

//私有成员变量
private:
	//XML文档节点
	QDomDocument doc;

	//XML根节点
	QDomElement root;

	//标志是否读取成功文档的变量
	bool m_bReadEnable;
};
