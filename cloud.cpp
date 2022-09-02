#include "hot.hpp"
#include "server.hpp"
#include <thread>

void FileUtilTest()
{
	//cloud::FileUtil("./testdir/adir").CreateDirectory();
	//cloud::FileUtil("./testdir/a.txt").Write("hello bit\n");
	//std::string body;
	//cloud::FileUtil("./testdir/a.txt").Read(&body);
	//std::cout << body << std::endl;
	//std::cout << cloud::FileUtil("./testdir/a.txt").Size() << std::endl;
	//std::cout << cloud::FileUtil("./testdir/a.txt").MTime() << std::endl;
	//std::cout << cloud::FileUtil("./testdir/a.txt").ATime() << std::endl;
	std::vector<std::string>arry;
	cloud::FileUtil("./testdir").ScanDirectory(&arry);
	for(auto & a : arry)
	{
		std::cout << a << std::endl;
	}
}

void JsonTest()
{
	Json::Value val;
	val["姓名"]="张三";
	val["性别"]="男";
	val["年龄"]=18;
	val["成绩"].append(77.5);
	val["成绩"].append(78.5);
	val["成绩"].append(79.5);

	std::string body;
	cloud::JsonUtil::Serialize(val,&body);
	std::cout << body << std::endl;

	Json::Value root;
	cloud::JsonUtil::UnSerialize(body,&root);
	std::cout << root["姓名"].asString() << std::endl;
	std::cout << root["性别"].asString() << std::endl;
	std::cout << root["年龄"].asInt() << std::endl;
	std::cout << root["成绩"][0].asFloat() << std::endl;
	std::cout << root["成绩"][1].asFloat() << std::endl;
	std::cout << root["成绩"][2].asFloat() << std::endl;
}

void CompressTest()
{
	cloud::FileUtil("./hello.txt").Compress("hello.zip");
	cloud::FileUtil("./hello.zip").UnCompress("hi.txt");
}

void DataTest()
{
	cloud::DataManager data;
	
	std::vector<cloud::FileInfo> arry;
	data.SelectAll(&arry);
	
	for(auto &e : arry)
	{
		std::cout << e.filename << std::endl;
		std::cout << e.url_path << std::endl;
		std::cout << e.real_path << std::endl;
		std::cout << e.file_size << std::endl;
		std::cout << e.back_time << std::endl;
		std::cout << e.pack_flag << std::endl;
		std::cout << e.pack_path << std::endl;
	
	}
	
	/*	
	data.Insert("./backup_dir/hello.txt");
	data.UpdateStatus("./backup_dir/hello.txt",true);
	std::vector<cloud::FileInfo> arry;
	data.SelectAll(&arry);
	for(auto &e : arry)
	{
		std::cout << e.filename  << std::endl;
		std::cout << e.url_path  << std::endl;
		std::cout << e.real_path << std::endl;
		std::cout << e.file_size << std::endl;
		std::cout << e.back_time << std::endl;
		std::cout << e.pack_flag << std::endl;
		std::cout << e.pack_path << std::endl;
	}
	std::cout << "------------------------------\n";

	cloud::FileInfo e;
	data.SelectOne("/download/hello.txt",&e);
	std::cout << e.filename  << std::endl;
	std::cout << e.url_path  << std::endl;
	std::cout << e.real_path << std::endl;
	std::cout << e.file_size << std::endl;
	std::cout << e.back_time << std::endl;
	std::cout << e.pack_flag << std::endl;
	std::cout << e.pack_path << std::endl;
	std::cout << "------------delete------------\n";
	
	
	data.DeleteOne("/download/hello.txt");
	arry.clear();
	data.SelectAll(&arry);
	for(auto &e : arry)
	{
		std::cout << e.filename  << std::endl;
		std::cout << e.url_path  << std::endl;
		std::cout << e.real_path << std::endl;
		std::cout << e.file_size << std::endl;
		std::cout << e.back_time << std::endl;
		std::cout << e.pack_flag << std::endl;
		std::cout << e.pack_path << std::endl;	
	}
	*/	
}

cloud::DataManager *_data;
void HotTest()
{
	cloud::HotManager cloud;
	cloud.RunModule();
}

void ServerTest()
{
	cloud::Server srv;
	srv.RunModule();
}

int main()
{
	_data = new cloud::DataManager();
	//FileUtilTest();
	//JsonTest();
	//CompressTest();
	//DataTest();
	//HotTest();
	//ServerTest();
	std::thread hot_thread(HotTest);
	std::thread srv_thread(ServerTest);
	
	hot_thread.join();
	srv_thread.join();
	return 0;
}
