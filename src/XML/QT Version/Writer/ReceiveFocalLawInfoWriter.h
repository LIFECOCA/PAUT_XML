#pragma once

#include <string>

#include <QDomDocument>

class CReceiveFocalLawInfoWriter
{
//公有数据访问方法
public:
	//设置某一个shot所生成的A扫信号个数
	void SetSignalNum(const int SignalNum, const int& ShotID);

	//设置某一个shot的某个signal所使用的阵元个数
	void SetElementNum(const int& ElementNum, const int& ShotID, const int SignalID);

	//设置某一个shot的某一个Signal的某一个阵元在板卡上对应的阵元ID号
	void SetElementID(const int& element, const int& ShotID, const int& SignalID, const int& ElementIndex);

	//设置某一个shot所拥有的聚焦法则个数
	void SetFocalLawNum(const int& LawNum, const int& ShotID, const int& SignalID);

	//设置某一个Law起作用的时间长度
	void SetTotalFocalTime(const double& time, const int& ShotID, const int& SignalID, const int& LawID);

	//设置某一个shot的某一个聚焦法则的某一个阵元的接收延迟时间
	void SetElementDelay(const double& delay, const int& ShotID, const int& SignalID, const int& LawID, const int& ElementIndex);

//公有构造及析构函数
public:
	//构造函数
	CReceiveFocalLawInfoWriter(const std::string& name);

	//析构函数
	~CReceiveFocalLawInfoWriter();

//私有成员变量
private:
	//XML文档节点
	QDomDocument doc;

	//XML根节点
	QDomElement root;

	std::string m_FilePathName;
};
