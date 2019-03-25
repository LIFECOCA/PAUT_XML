#pragma once

#include "PADefinition.h"

#include <string>

#include <QDomDocument>

class CTransmitFocalLawInfoWriter
{
//公有数据访问方法
public:
	//设置硬件信息
	void SetHardwareInfo(const HardwareInfo& hardware);

	//设置总Shot数
	void SetShotNum(const int& ShotNum);

	//设置某一个shot所使用的阵元个数
	void SetElementNum(const int& ElementNum, const int& ShotID);

	//获得某一个shot的A扫深度
	void SetAScanDepth(const double& depth, const int& ShotID);

	//设置某一个shot的某一个发射阵元在板卡上对应的阵元ID号
	void SetTransmitElementID(const int& ElementID, const int& ShotID, const int& ElementIndex);

	//设置某一个shot的某一个阵元的接收延迟时间
	void SetTransmitDelay(const double& delay, const int& ShotID, const int& ElementIndex);

	//获得某个shot的某个element的信息
	void SetElementInfo(const ElementInfo& element, const int& ShotID, const int& ElementIndex);

//公有构造及析构函数
public:
	//构造函数
	CTransmitFocalLawInfoWriter(const std::string& name);

	//析构函数
	~CTransmitFocalLawInfoWriter();

//私有成员变量
private:
	//XML文档节点
	QDomDocument doc;

	//XML根节点
	QDomElement root;

	std::string m_FilePathName;
};
