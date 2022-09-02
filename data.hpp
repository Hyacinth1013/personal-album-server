#ifndef __MY_DATA__
#define __MY_DATA__
#include "util.hpp"
#include <cstdlib>
#include <mysql/mysql.h>

namespace image_sys
{
	#define HOST "127.0.0.1"
	#define USER "root"
	#define PASS "qqqforlsy"
	#define DBNAME "image_system"
	static MYSQL *MysqlInit()
	{
		MYSQL *mysql = mysql_init(NULL);
		if(mysql == NULL)
		{
			std::cout << "mysql init failed!\n";
			return NULL;
		}
		if(mysql_real_connect(mysql, HOST, USER, PASS, DBNAME, 0, NULL, 0) == NULL)
		{
			std::cout << "connect mysql server failed:";
			std::cout << mysql_error(mysql) << std::endl;
			mysql_close(mysql);
			return NULL;
		}
		int ret = mysql_set_character_set(mysql, "utf8");
		if(ret != 0)
		{
			std::cout << "set mysql client character failed:";
			std::cout << mysql_error(mysql) << std::endl;
			mysql_close(mysql);
			return NULL;
		}
		return mysql;
	}
	static void MysqlRelease(MYSQL *mysql)
	{
		if(mysql != NULL)
		{
			mysql_close(mysql);
		}
		return ;
	}
	static bool MysqlQuery(MYSQL *mysql, const std::string &sql)
	{
		int ret = mysql_query(mysql, sql.c_str());
		if(ret != 0)
		{
			std::cout << sql << std::endl;
			std::cout << mysql_error(mysql) << std::endl;
			return false;
		}
		return true;
	}
	class ImageTable
	{
		private:
			MYSQL *_mysql;
		public:
			ImageTable()
			{
				_mysql = MysqlInit();
				if(_mysql == NULL)
				{
					exit(-1);
				}
			}
			~ImageTable()
			{
				MysqlRelease(_mysql);
			}
			bool Insert(const Json::Value &image)
			{
				#define INSERT_IMAGE "insert image_table values(null, '%s', '%s', '%s')"
				char sql[4096] = {0};
				sprintf(sql, INSERT_IMAGE, image["info"].asCString(),
						image["url"].asCString(), 
						image["realpath"].asCString());
				/*
					std::stringstream ss;
					ss << "insert image_table values(null,";
					ss << "'" << image["info"].asCString() << "',";
					ss << "'" << image["url"].asCString() << "',";
					ss << "'" << image["realpath"].asCString() << "'),";
					return MysqlQuery(_mysql, sql);
				*/
				return MysqlQuery(_mysql, sql);
			}
			bool Delete(int image_id)
			{
				#define DELETE_IMAGE "delete from image_table where id = %d"
				char sql[4096] = {0};
				sprintf(sql, DELETE_IMAGE, image_id);
				return MysqlQuery(_mysql, sql);
			}
			bool Update(int image_id, const Json::Value &image)
			{
				#define UPDATE_IMAGE "update image_table set info='%s' where id='%d';"
				char sql[4096] = {0};
				sprintf(sql, UPDATE_IMAGE, image["info"].asCString(), image_id);
				return MysqlQuery(_mysql, sql);
			}
			bool GetAll(Json::Value *image)
			{
				#define GETALL_IMAGE "select id, info, url, realpath from image_table;"
				bool ret = MysqlQuery(_mysql, GETALL_IMAGE);
				if(ret == false)
				{
					return false;
				}
				MYSQL_RES *res = mysql_store_result(_mysql);
				if(res == NULL)
				{
					std::cout << "mysql store result failed!\n";
					std::cout << mysql_error(_mysql) << std::endl;
					return false;
				}
				int num = mysql_num_rows(res);
				for(int i=0; i<num; i++)
				{
					MYSQL_ROW row = mysql_fetch_row(res);
					Json::Value val;
					val["id"] = std::stoi(row[0]);
					val["info"] = row[1];
					val["url"] = row[2];
					val["realpath"] = row[3];
					image->append(val);
				}
				mysql_free_result(res);
				return true;
			}
			bool GetOne(int image_id, Json::Value *image)
			{
				#define GETONE_IMAGE "select id, info, url, realpath from image_table where id=%d;"
				char sql[4096] = {0};
				sprintf(sql, GETONE_IMAGE, image_id);
				bool ret = MysqlQuery(_mysql, sql);
				if(ret == false)
				{
					return false;
				}
				MYSQL_RES *res = mysql_store_result(_mysql);
				if(res == NULL)
				{
					std::cout << "mysql store result failed!\n";
					std::cout << mysql_error(_mysql) << std::endl;
					return false;
				}
				int num = mysql_num_rows(res);
				if(num == 0)
				{
					mysql_free_result(res);
					return false;
				}
				MYSQL_ROW row = mysql_fetch_row(res);
				(*image)["id"] = std::stoi(row[0]);
				(*image)["info"] = row[1];
				(*image)["url"] = row[2];
				(*image)["realpath"] = row[3];
				mysql_free_result(res);
				return true;
			}
	};
}

#endif
