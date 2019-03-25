#include <windows.h>

#include "tinyxml2.h"

#include <string>
#include <cstdio>


//测试XML写入
void testWriteXML(const char* xmlPath)
{	
	using std::string;
	using std::to_string;
	using tinyxml2::XMLDocument;

	//信息字典
	string ScanTypeDict[2] = { "PA", "TOFD" };
	string TRModeDict[2] = { "Pusle-Echo", "TransmitReceive" };

	//检测信息
	int GroupNum = 3;
	int ChannelNum[] = { 15, 30, 20 };
	int ScanType[] = { 0, 0, 0 };
	int TRMode[] = { 0, 0, 0 };

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
	

	//<root>
	auto root = newNode("root");
	doc.InsertEndChild(root);

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

		for (int j = 0; j < ChannelNum[i]; j++)
		{
			//<Channelj+1>
			auto pChannel = newIDNode("Channel", j + 1);
			pGroup->InsertEndChild(pChannel);
			//<ScanAngle>
			pChannel->InsertEndChild(newNodeWithValue("ScanAngle", 10));
			//<CenterElement>
			pChannel->InsertEndChild(newNodeWithValue("CenterElement", 5.2));
			//<WedgeDelay>
			pChannel->InsertEndChild(newNodeWithValue("WedgeDelay", 3.3));
			//<ACGDelay>
			pChannel->InsertEndChild(newNodeWithValue("ACGDelay", 4.4));
			//<ACGGain>
			pChannel->InsertEndChild(newNodeWithValue("ACGGain", 5.5));
		}
	}

	//存储
	doc.SaveFile(xmlPath);
}


//测试XML读出
void testReadXML(const char* xmlPath)
{
	using std::string;
	using std::stoi;
	using std::stod;
	using std::to_string;
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

	int GroupNum = stoi(getNodeText(root, "GroupNum"));
	for (int i = 0; i < GroupNum; i++)
	{
		auto pGroup = getIDNode(root, "Group", i + 1);
		int ChannelNum = stoi(getNodeText(pGroup, "ChannelNum"));
		printf("Group%d: ChannelNum: %d\n", i + 1, ChannelNum);
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
	auto loadfile = path + "default.xml";


	testWriteXML(savefile.c_str());

	//testReadXML(loadfile.c_str());

	return 0;
}