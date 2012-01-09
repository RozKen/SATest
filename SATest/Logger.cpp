#include "Logger.h"
#include <time.h>
#include <sstream>
#include <iostream>
#include <direct.h>	//_mkdirに必要
#include <Shlwapi.h>	//PathIsDirectoryに必要
#pragma comment(lib, "Shlwapi.lib")

Logger::Logger(): count(0), logFilePath(""), logDirectoryPath("")
	, logFileName(""){
		logTitles = new std::vector<std::string>();
		logTypes = new std::vector<int>();
		logContents = new std::vector<void *>();
		logArrayIndex = new std::vector<int>();
}

Logger::Logger(std::string filePath)
	:count(0), logFilePath(filePath), logDirectoryPath(""), logFileName(""){

	Initialize();
}
Logger::Logger(std::string directoryPath, std::string fileName)
	:count(0), logFilePath(directoryPath.append("/").append(fileName))
	, logDirectoryPath(directoryPath), logFileName(fileName){
		if(!PathIsDirectory(s2ws(logDirectoryPath).c_str())){
			_mkdir(directoryPath.c_str());
		}else{
			std::cout << logDirectoryPath << " already exists." << std::endl;
		}
		Initialize();
}

Logger::~Logger(){
	ofs.close();
	logTitles->clear();
	logTypes->clear();
	logContents->clear();
	logArrayIndex->clear();
	delete logTitles;
	delete logTypes;
	delete logContents;
	delete logArrayIndex;
}

void Logger::Log(){
	if(count == 0){
		for(int i = 0; i < logTitles->size(); i++){
			ofs << logTitles->at(i) << ",";
		}
		ofs << std::endl;
	}
	for(int i = 0; i < logContents->size(); i++){
		switch(logTypes->at(i)){
		case 0:
			ofs << *((int *)(logContents->at(i))) << ",";
			break;
		case 1:
			ofs << *((float *)(logContents->at(i))) << ",";
			break;
		case 2:
			ofs << ((std::vector<int> *)logContents->at(i))->at(logArrayIndex->at(i)) << ",";
			break;
		case 3:
			////ERRORここでしばしばメモリリークが起きる logArrayIndexにおかしな値
			ofs << ((std::vector<float> *)logContents->at(i))->at(logArrayIndex->at(i)) << ",";
			break;
		default:
			ofs << "Invalid Type Value,";
		}
	}
	ofs << std::endl;
	count++;
}

void Logger::Initialize(){
	if(logFilePath.length() == 0){
		std::cerr << "Error:ログファイルの保存先(logFilePath)を指定して下さい" << std::endl;
	}else{
		///記録ファイルを設定
		this->ofs.open(logFilePath);
		this->add("count", &count);
	}
	if(logTitles != NULL){
		delete logTitles;
	}
	if(logTypes != NULL){
		delete logTypes;
	}
	if(logContents != NULL){
		delete logContents;
	}
	if(logArrayIndex != NULL){
		delete logArrayIndex;
	}
	logTitles = new std::vector<std::string>();
	logTypes = new std::vector<int>();
	logContents = new std::vector<void *>();
	logArrayIndex = new std::vector<int>();
}

void Logger::add(std::string title, int* pointer_to_integer_variable){
	logTitles->push_back(title);
	logTypes->push_back(0);
	logContents->push_back((void*) pointer_to_integer_variable);
	logArrayIndex->push_back(-1);
}

void Logger::add(std::string title, float* pointer_to_float_variable){
	logTitles->push_back(title);
	logTypes->push_back(1);
	logContents->push_back((void*) pointer_to_float_variable);
	logArrayIndex->push_back(-1);
}

void Logger::add(std::string title, std::vector<int>* vector, int index){
	logTitles->push_back(title);
	logTypes->push_back(2);
	logContents->push_back((void*) vector);
	logArrayIndex->push_back(index);
}

void Logger::add(std::string title, std::vector<float>* vector, int index){
	logTitles->push_back(title);
	logTypes->push_back(3);
	//logContentsには配列全体を入れておく
	logContents->push_back((void *) vector);
	//pushはしないけど，なにかしたいよね…
	logArrayIndex->push_back(index);
	if(index < 0){
		std::cout << "Wrong Index Value on Logger::add (std::vector<float>) : " << index << std::endl;
	}
}

std::string Logger::intToString(int number){
	std::string str = "";
	std::stringstream ss;
	ss << number;
	if(number / 1000 > 0){
		str = ss.str();
	}else if(number / 100 > 0){
		str = "0";
		str.append(ss.str());
	}else if(number / 10 > 0){
		str = "00";
		str.append(ss.str());
	}else{
		str = "000";
		str.append(ss.str());
	}
	return str;
}

void Logger::setFilePath(std::string filePath){
	logFilePath = filePath;
	Initialize();
}

void Logger::setFilePath(std::string fileDirectoryPath, std::string fileName){
	logDirectoryPath = fileDirectoryPath;
	logFileName = fileName;
	logFilePath = logDirectoryPath;
	logFilePath.append("/");
	logFilePath.append(fileName);
	if(!PathIsDirectory(s2ws(logDirectoryPath).c_str())){
		_mkdir(logDirectoryPath.c_str());
	}else{
		std::cout << logDirectoryPath << " already exists." << std::endl;
	}
	Initialize();
}

std::string Logger::getFilePath() const{
	return logFilePath;
}
std::string Logger::getDirectoryPath() const{
	return this->logDirectoryPath;
}
std::string Logger::getFileName() const{
	return this->logFileName;
}
std::wstring Logger::s2ws(const std::string& s){
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	//std::cout << "Length: " << len << std::endl;
	wchar_t * buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring result(buf);
	delete[] buf;
	return result;
}