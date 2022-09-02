#ifndef __MY_UTIL__
#define __MY_UTIL__
#include <iostream>
#include <sstream>
#include <fstream>
#include <jsoncpp/json/json.h>

namespace image_sys
{
	class FileUtil
	{
		private:
			std::string _filename;
		public:
			FileUtil(const std::string &filename): _filename(filename){}
			bool Read(std::string *body)
			{
				std::ifstream ifs;
				ifs.open(_filename, std::ios::binary);
				if(ifs.is_open() == false)
				{
					perror("open file error");
					return false;
				}
				ifs.seekg(0, std::ios::end);
				size_t fsize = ifs.tellg();
				ifs.seekg(0, std::ios::beg);

				body->resize(fsize);
				ifs.read(&(*body)[0], fsize);
				if(ifs.good() == false)
				{
					std::cout << "read file content failed!\n";
					ifs.close();
					return false;
				}
				ifs.close();
				return true;
			}

			bool Write(const std::string &body)
			{
				std::ofstream ofs;
				ofs.open(_filename, std::ios::binary);
				if(ofs.is_open() == false)
				{
					std::cout << _filename << std::endl;
					perror("open file error");
					return false;
				}
				ofs.write(body.c_str(), body.size());
				if(ofs.good() == false)
				{
					std::cout << "write file content failed!\n";
					ofs.close();
					return false;
				}
				ofs.close();
				return true;
			}
	};

	class JsonUtil
	{
		public:
			static bool Serialize(Json::Value &root, std::string *body)
			{
				Json::StreamWriterBuilder swb;
				Json::StreamWriter *psw = swb.newStreamWriter();
				std::stringstream ss;
				int ret = psw->write(root, &ss);
				if(ret != 0)
				{
					std::cout << "Serialize failed!\n";
					return false;
				}
				*body = ss.str();
				return true;
			}

			static bool UnSerialize(const std::string &body, Json::Value *root)
			{
				Json::CharReaderBuilder crb;
				Json::CharReader *pcr = crb.newCharReader();

				std::string errs;
				bool ret = pcr->parse(body.c_str(), body.c_str() + body.size(), root, &errs);
				if(ret == false)
				{   
					std::cout << "parse failed: " << errs << std::endl;
					return -1;
				}
				return true;
			}
	};
}

#endif
