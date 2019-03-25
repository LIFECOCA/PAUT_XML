#pragma once

#include "PADefinition.h"

#include <string>

#include <QDomDocument>

//这是一个用于解析基本聚焦法则文档信息的类
class CTransmitFocalLawInfoReader
{
//公有数据访问方法
public:
	//获得硬件基本信息
	HardwareInfo GetHardwareInfo() const;

	//获得总Shot数
	int GetShotNum() const;

	//获得某一个shot所使用的阵元个数
	int GetElementNum(const int& ShotID) const;

	//获得某一个shot的A扫深度
	double GetAScanDepth(const int& ShotID) const;

	//获得某一个shot的某一个发射阵元在板卡上对应的阵元ID号
	int GetTransmitElementID(const int& ShotID, const int& ElementIndex) const;

	//获得某一个shot的某一个阵元的发射延迟时间
	double GetTransmitDelay(const int& ShotID, const int& ElementIndex) const;

	//获得某个shot的某个element的信息
	ElementInfo GetElementInfo(const int& ShotID, const int& ElementIndex) const;

//公有构造及析构函数
public:
	//构造函数
	CTransmitFocalLawInfoReader(const std::string& name);

	//析构函数
	~CTransmitFocalLawInfoReader();

//私有成员变量
private:
	//XML文档节点
	QDomDocument doc;

	//XML根节点
	QDomElement root;

	//标志是否读取成功文档的变量
	bool m_bReadEnable;
};
