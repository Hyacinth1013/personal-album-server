.PHONY:cloud
cloud:cloud.cpp  util.hpp
	g++ -std=c++17 $^ -o $@ -L./lib -lstdc++fs -ljsoncpp -lbundle -lpthread
