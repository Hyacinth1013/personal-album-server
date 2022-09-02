#include "data.hpp"
#include "httplib.h"

void FileTest()
{
	image_sys::FileUtil("./test.txt").Write("Hello World!\n");
	std::string body;
	image_sys::FileUtil("./test.txt").Read(&body);
	std::cout << body << std::endl;
}

void JsonTest()
{
	Json::Value root;
	root["姓名"] = "张三";
	root["年龄"] = 18;
	root["成绩"].append(77);
	root["成绩"].append(88);
	root["成绩"].append(99.5);

	std::string body;
	image_sys::JsonUtil::Serialize(root, &body);
	std::cout << body << std::endl;

	Json::Value val;
	image_sys::JsonUtil::UnSerialize(body, &val);
	std::cout << val["姓名"].asString() << std::endl;
	std::cout << val["年龄"].asInt() << std::endl;
	for(int i=0; i<val["成绩"].size(); ++i)
	{
		std::cout << val["成绩"][i].asFloat() << std::endl;
	}
}

void DataTest()
{
	image_sys::ImageTable *image = new image_sys::ImageTable();
	Json::Value val;
	val["info"] = "这是一只狗";
	val["url"] = "/images/dog.png";
	val["realpath"] = "./wwwroot/images/dog.png";

	//image->Insert(val);
	//image->Update(1,val);
	image->Delete(4);
	Json::Value root;
	image->GetAll(&root);
	//image->GetOne(2, &root);
	std::string body;
	image_sys::JsonUtil::Serialize(root, &body);
	std::cout << body << std::endl;
}

#define WWWROOT "./wwwroot/"
#define IMAGE_URL_PREFIX "/images/"
#define IMAGE_REAL_PATH "./wwwroot/images/"

image_sys::ImageTable *g_image = NULL; 
void DeleteImage(const httplib::Request &req, httplib::Response &rsp)
{
	// 获取图片ID
	int image_id = std::stoi(req.matches[1]);
	// 从数据库中获取指定图片信息，获取实际存储路径
	Json::Value image;
	bool ret = g_image->GetOne(image_id, &image);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Failed to get the one information from the database";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 500;
		return ;
	}
	// 获取图片的实际存储路径，然后删除图片文件
	std::string realpath = image["realpath"].asString();
	remove(realpath.c_str());
	ret = g_image->Delete(image_id);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Failed to delete information from the database";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 500;
		return ;
	}
	rsp.status = 200;
}

void UpdateImage(const httplib::Request &req, httplib::Response &rsp)
{
	// 获取要修改的图片ID以及新的图片信息
	int image_id = std::stoi(req.matches[1]);
	Json::Value image;
	bool ret = image_sys::JsonUtil::UnSerialize(req.body, &image);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Text information parsing failed";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 400;
		return ;
	}
	ret = g_image->Update(image_id, image);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Failed to modify database picture information";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 500;
		return ;
	}
	rsp.status = 200;
}

void GetOneImage(const httplib::Request &req, httplib::Response &rsp)
{
	// 获取图片ID
	int image_id = std::stoi(req.matches[1]);
	Json::Value image;
	bool ret = g_image->GetOne(image_id, &image);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Failed to get the one information from the database";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 500;
		return ;	
	}
	ret = image_sys::JsonUtil::Serialize(image, &rsp.body);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Failed to serialize picture information";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 500;
		return ;		
	}
	rsp.status = 200;
	rsp.set_header("Content-Type", "application/json");
	return ;
}

void GetAllImage(const httplib::Request &req, httplib::Response &rsp)
{
	// 从数据库获取所有图片信息，然后组织正文相应
	Json::Value image;
	bool ret = g_image->GetAll(&image);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Failed to get all information from the database";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 500;
		return ;	
	}
	ret = image_sys::JsonUtil::Serialize(image, &rsp.body);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Failed to serialize picture information";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 500;
		return ;		
	}
	rsp.status = 200;
	rsp.set_header("Content-Type", "application/json");
	return ;
}

void InsertImage(const httplib::Request &req, httplib::Response &rsp)
{
	const httplib::MultipartFormData &image = req.get_file_value("image_file");
	const httplib::MultipartFormData &info = req.get_file_value("image_info");
	// image.filename 文件名	image.content 文件数据
	// info.content 图片描述信息	
	std::string realpath = IMAGE_REAL_PATH + image.filename;
	bool ret = image_sys::FileUtil(realpath).Write(image.content);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Failed to store picture data to disk";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 500;
		return ;
	}
	Json::Value json_image;
	json_image["info"] = info.content;
	json_image["url"] = IMAGE_URL_PREFIX + image.filename;	// /images/a.png
	json_image["realpath"] = realpath;	// ./wwwroot/images/a.png
	ret = g_image->Insert(json_image);
	if(ret == false)
	{
		Json::Value err;
		err["result"] = false;
		err["reason"] = "Failed to insert information into the database";
		image_sys::JsonUtil::Serialize(err, &rsp.body);
		rsp.set_header("Content-Type", "application/json");
		rsp.status = 500;
		return ;
	}
	rsp.set_redirect("/", 303);
}

int main()
{
	//JsonTest();
	//DataTest();

	g_image = new image_sys::ImageTable();
	httplib::Server server;		// 设置静态资源根目录
	// /index.html -> ./wwwroot/index/html
	server.set_mount_point("/", "./wwwroot/");

	server.Delete("/image/(\\d+)", DeleteImage);
	server.Put("/image/(\\d+)", UpdateImage);
	server.Get("/image/(\\d+)", GetOneImage);
	server.Get("/image", GetAllImage);
	server.Post("/image", InsertImage);

	server.listen("0.0.0.0", 9090);
	return 0;
}
