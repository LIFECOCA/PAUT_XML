#include <windows.h>

#include "tinyxml2.h"

#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <iostream>


//测试XML写入
void testWriteXML(const char* xmlPath)
{	
	using std::vector;
	using std::string;
	using std::to_string;
	using std::for_each;
	using std::fill;
	using std::begin;
	using std::end;
	using tinyxml2::XMLDocument;

	//信息字典
	string ScanTypeDict[2] = { "PA", "TOFD" };
	string TRModeDict[2] = { "Pusle-Echo", "TransmitReceive" };

	//General信息
	int GroupNum = 3;
	int ChannelNum[] = { 15, 30, 20 };
	int ScanType[] = { 0, 0, 0 };
	int TRMode[] = { 0, 0, 0 };
	vector<vector<double>> ScanAngle(GroupNum);
	vector<vector<int>> CenterElement(GroupNum);
	vector<vector<double>> WedgeDelay(GroupNum);
	for (int i = 0; i < GroupNum; i++)
	{
		ScanAngle[i].resize(ChannelNum[i]);
		CenterElement[i].resize(ChannelNum[i]);
		WedgeDelay[i].resize(ChannelNum[i]);
	}
	//初始化不同组各通道的角度值
	fill(begin(ScanAngle[0]), end(ScanAngle[0]), 10);
	fill(begin(ScanAngle[1]), end(ScanAngle[1]), 30);
	fill(begin(ScanAngle[2]), end(ScanAngle[2]), 40);
	//初始化不同组各通道的中心阵元
	fill(begin(CenterElement[0]), end(CenterElement[0]), 16);
	fill(begin(CenterElement[1]), end(CenterElement[1]), 32);
	fill(begin(CenterElement[2]), end(CenterElement[2]), 64);
	//初始化不同组各通道的楔块延时
	fill(begin(WedgeDelay[0]), end(WedgeDelay[0]), 0.3);
	fill(begin(WedgeDelay[1]), end(WedgeDelay[1]), 0.4);
	fill(begin(WedgeDelay[2]), end(WedgeDelay[2]), 0.5);

	//Transmit信息
	int ShotNum = 91;
	vector<int> TransmitElementNum(ShotNum);
	//设置所有声束的发射阵元数量均为32
	fill(begin(TransmitElementNum), end(TransmitElementNum), 32);
	vector<vector<double>> TransmitDelay(ShotNum);
	for (int i = 0; i < ShotNum; i++)
	{
		TransmitDelay[i].resize(TransmitElementNum[i]);
		//延时设置为0
		fill(begin(TransmitDelay[i]), end(TransmitDelay[i]), 0);
	}
	//Receive信息
	vector<int> ReceiveFocalPointNum(ShotNum);
	//设置所有声束的接收聚焦点数均为3
	fill(begin(ReceiveFocalPointNum), end(ReceiveFocalPointNum), 3);
	vector<int> ReceiveElementNum(ShotNum);
	//设置所有声束的接收数量均为32
	fill(begin(ReceiveElementNum), end(ReceiveElementNum), 16);
	vector<vector<vector<double>>> ReceiveDelay(ShotNum);
	vector<vector<double>> ReceiveTotalFocalTime(ShotNum);
	for (int i = 0; i < ShotNum; i++)
	{
		ReceiveDelay[i].resize(ReceiveFocalPointNum[i]);
		ReceiveTotalFocalTime[i].resize(ReceiveFocalPointNum[i]);
		//设置各个接收聚焦点的接收延时
		for (int j = 0; j < ReceiveFocalPointNum[i]; j++)
		{
			ReceiveDelay[i][j].resize(ReceiveElementNum[i]);
			fill(begin(ReceiveDelay[i][j]), end(ReceiveDelay[i][j]), 0.01);
		}
		//设置TotalFocalTime
		fill(begin(ReceiveTotalFocalTime[i]), end(ReceiveTotalFocalTime[i]), 10.3);
	}


	//构建xml文档
	const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
	XMLDocument doc;
	doc.Parse(declaration);

	//定义减少书写量的lambda
	auto newNode = [&doc](string name) { return doc.NewElement(name.c_str()); };
	auto newIDNode = [&doc](string name, int id) { return doc.NewElement((name + to_string(id)).c_str()); };
	auto newNodeWithText = [&doc](string name, string text)
	{
		auto node = doc.NewElement(name.c_str());
		node->InsertEndChild(doc.NewText(text.c_str()));
		return node;
	};
	auto newNodeWithValue = [&doc](string name, auto val)
	{
		auto node = doc.NewElement(name.c_str());
		node->InsertEndChild(doc.NewText(to_string(val).c_str()));
		return node;
	};
	auto newIDNodeWithValue = [&doc](string name, int id, auto val)
	{
		auto node = doc.NewElement((name + to_string(id)).c_str());
		node->InsertEndChild(doc.NewText(to_string(val).c_str()));
		return node;
	};
	auto newNodeWithVector = [&doc](string name, auto vec)
	{
		auto node = doc.NewElement(name.c_str());
		string text;
		for (auto val : vec) { text += to_string(val) + " "; }
		text.pop_back();
		node->InsertEndChild(doc.NewText(text.c_str()));
		return node;
	};
	auto newIDNodeWithVector = [&doc](string name, int id, auto vec)
	{
		auto node = doc.NewElement((name.c_str() + to_string(id)).c_str());
		string text;
		for (auto val : vec) { text += to_string(val) + " "; }
		text.pop_back();
		node->InsertEndChild(doc.NewText(text.c_str()));
		return node;
	};

	//<root>
	auto root = newNode("root");
	doc.InsertEndChild(root);

	/////////////////////////////设置General Info//////////////////////////////////
	//<GroupNum>
	root->InsertEndChild(newNodeWithValue("GroupNum", GroupNum));
	for (int i = 0; i < GroupNum; i++)
	{
		//<Groupi+1>
		auto pGroup = newIDNode("Group", i + 1);
		root->InsertEndChild(pGroup);
		//<ChannelNum>
		pGroup->InsertEndChild(newNodeWithValue("ChannelNum", ChannelNum[i]));
		//<ScanType>
		pGroup->InsertEndChild(newNodeWithText("ScanType", ScanTypeDict[ScanType[i]]));
		//<TRMode>
		pGroup->InsertEndChild(newNodeWithText("TRMode", TRModeDict[TRMode[i]]));
		//<PartVelocity>
		pGroup->InsertEndChild(newNodeWithValue("PartVelocity", 5900));
		//<SendFocus>
		pGroup->InsertEndChild(newNodeWithValue("SendFocus", 10));
		//<RecvFocusNum>
		pGroup->InsertEndChild(newNodeWithValue("RecvFocusNum", 10));
		//<RecvFocusStart>
		pGroup->InsertEndChild(newNodeWithValue("RecvFocusStart", 1));
		//<RecvFocusEnd>
		pGroup->InsertEndChild(newNodeWithValue("RecvFocusEnd", 10));
		//<FocalType>
		pGroup->InsertEndChild(newNodeWithValue("FocalType", 3));

		//<ScanAngle>
		pGroup->InsertEndChild(newNodeWithVector("ScanAngle", ScanAngle[i]));
		pGroup->InsertEndChild(newNodeWithVector("CenterElement", CenterElement[i]));
		pGroup->InsertEndChild(newNodeWithVector("WedgeDelay", WedgeDelay[i]));
	}

	//设置Transmit和ReceiveInfo
	root->InsertEndChild(newNodeWithValue("ShotNum", ShotNum));
	for (int i = 0; i < ShotNum; i++)
	{
		//<Shotj+1>
		auto pShot = newIDNode("Shot", i + 1);
		root->InsertEndChild(pShot);
		//<TransmitElementNum>
		pShot->InsertEndChild(newNodeWithValue("TransmitElementNum", TransmitElementNum[i]));
		//<ReceiveElementNum>
		pShot->InsertEndChild(newNodeWithValue("ReceiveElementNum", ReceiveElementNum[i]));
		//<ReceiveFocalNum>
		pShot->InsertEndChild(newNodeWithValue("ReceiveFocalPointNum", ReceiveFocalPointNum[i]));
		//<TransmitDelay>
		pShot->InsertEndChild(newNodeWithVector("TransmitDelay", TransmitDelay[i]));
		for (int j = 0; j < ReceiveFocalPointNum[i]; j++)
		{
			//<ReceiveDelayi+1>
			pShot->InsertEndChild(newIDNodeWithVector("ReceiveDelay", j + 1, ReceiveDelay[i][j]));
		}
		//<TotalFocalTime>
		pShot->InsertEndChild(newNodeWithVector("ReceiveTotalFocalTime", ReceiveTotalFocalTime[i]));
	}

	//存储
	doc.SaveFile(xmlPath);
}


//测试XML读出
void testReadXML(const char* xmlPath)
{
	using std::vector;
	using std::string;
	using std::stoi;
	using std::stod;
	using std::to_string;
	using std::for_each;
	using std::begin;
	using std::end;
	using tinyxml2::XMLDocument;

	//载入XML文档
	XMLDocument doc;
	doc.LoadFile(xmlPath);

	//获取root
	auto root = doc.RootElement();

	//定义减少书写量的lambda
	auto getNode = [](auto parent, string name) { return parent->FirstChildElement(name.c_str()); };
	auto getIDNode = [](auto parent, string name, int id) { return parent->FirstChildElement((name + to_string(id)).c_str()); };
	auto getNodeText = [](auto parent, string name) { return parent->FirstChildElement(name.c_str())->GetText(); };
	auto getIDNodeText = [](auto parent, string name, int id) { return parent->FirstChildElement((name + to_string(id)).c_str())->GetText(); };

	auto splitInt = [](string text)
	{
		vector<int> result;
		int pos = text.find_first_of(" ");
		while (pos != string::npos)
		{
			result.emplace_back(stoi(text.substr(0, pos)));
			text = text.substr(pos + 1, text.length());
			pos = text.find_first_of(" ");
		}
		result.emplace_back(stoi(text));
		return result;
	};
	auto splitDouble = [](string text) 
	{
		vector<double> result;
		int pos = text.find_first_of(" ");
		while (pos != string::npos)
		{
			result.emplace_back(stod(text.substr(0, pos)));
			text = text.substr(pos + 1, text.length());
			pos = text.find_first_of(" ");
		}
		result.emplace_back(stod(text));
		return result;
	};
	auto printIntNode = [](const char* s, const auto& vec)
	{
		printf("%s", s);
		for (auto val : vec)
		{
			printf("%d ", val);
		}
		printf("\n");
	};
	auto printFloatNode = [](const char* s, const auto& vec)
	{
		printf("%s", s);
		for (auto val : vec)
		{
			printf("%lf ", val);
		}
		printf("\n");
	};


	int GroupNum = stoi(getNodeText(root, "GroupNum"));
	for (int i = 0; i < GroupNum; i++)
	{
		auto pGroup = getIDNode(root, "Group", i + 1);
		int ChannelNum = stoi(getNodeText(pGroup, "ChannelNum"));
		printf("Group%d:\nChannelNum: %d\n", i + 1, ChannelNum);
		printFloatNode("ScanAngle:", splitDouble(getNodeText(pGroup, "ScanAngle")));
		printIntNode("CenterElement: ", splitInt(getNodeText(pGroup, "CenterElement")));
	}

}


int main(int argc, char* argv[])
{
	//获得资源路径
	char result[MAX_PATH];
	std::string path(result, GetModuleFileName(NULL, result, MAX_PATH));
	auto pos = path.find("\\bin");
	path = path.substr(0, pos) + "\\resource\\";

	auto savefile = path + "1.xml";
	auto loadfile = path + "1.xml";


	//testWriteXML(savefile.c_str());

	testReadXML(loadfile.c_str());

	return 0;
}