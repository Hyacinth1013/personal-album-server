main:main.cpp util.hpp data.hpp
	g++ -std=c++11 $^ -o $@ -L/usr/lib64/mysql -ljsoncpp -lmysqlclient -pthread
