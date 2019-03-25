#pragma once

#include <string>

#include <QDomDocument>

//这是一个用于解析接收FocalLawInfoReader文档信息的类
class CReceiveFocalLawInfoReader
{
//公有数据访问方法
public:
	//获得总Shot数
	int GetShotNum() const;

	//获得某一个shot所生成的A扫信号个数
	int GetSignalNum(const int& ShotID) const;

	//获得某一个shot所使用的阵元个数
	int GetElementNum(const int& ShotID, const int& SignalID) const;

	//获得某一个shot的某一个Signal的某一个阵元在板卡上对应的阵元ID号
	int GetElementID(const int& ShotID, const int& SignalID, const int& ElementIndex) const;

	//获得某一个shot所拥有的聚焦法则个数
	int GetFocalLawNum(const int& ShotID, const int& SignalID) const;

	//获得某一个Law起作用的时间长度
	double GetTotalFocalTime(const int& ShotID, const int& SignalID, const int& LawID);

	//获得某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
	double GetElementDelay(const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex) const;

//公有构造及析构函数
public:
	//构造函数
	CReceiveFocalLawInfoReader(const std::string& name);

	//析构函数
	~CReceiveFocalLawInfoReader();

//私有成员变量
private:
	//XML文档节点
	QDomDocument doc;

	//XML根节点
	QDomElement root;

	//标志是否读取成功文档的变量
	bool m_bReadEnable;
};
