#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <windows.h>	//for WIN32_FIND_DATA

#include "DataInDirectoryInvestigator.h"

DataInDirectoryInvestigator::DataInDirectoryInvestigator(std::string dirPath)
	:dirPath(dirPath)
{
	fileNames = new std::vector<std::string>();
}

std::vector<std::string>* DataInDirectoryInvestigator::SearchFiles(std::string fileType){
	this->fileType = fileType;

	std::string searchQuery = dirPath;
	searchQuery.append(fileType);
	
	std::wstring stemp = s2ws(searchQuery);
	LPCWSTR DIRPATH = stemp.c_str();
	
	std::cout << "Searching Files in " << dirPath << std::endl;
	std::vector<std::string> * acquiredFilePaths = new std::vector<std::string>();

	HANDLE hFind;
	WIN32_FIND_DATA w32fd;
	hFind = FindFirstFile(DIRPATH, &w32fd);
	if( hFind != INVALID_HANDLE_VALUE){
		do {
			//std::cout << "cAlternateFileName : " << ws2s(w32fd.cAlternateFileName) << std::endl;
			//printf("ファイル名：%S\n", w32fd.cFileName);
			std::cout << "ファイル名: " << ws2s(w32fd.cFileName) << std::endl;
			acquiredFilePaths->push_back(ws2s(w32fd.cFileName));
		} while (FindNextFile (hFind, &w32fd));
		FindClose(hFind);
	}
	return acquiredFilePaths;
}

std::vector<std::vector<std::string>* >* DataInDirectoryInvestigator::AcquireData(bool column, int index){
	std::vector<std::vector<std::string>* >* acquiredData = new std::vector<std::vector<std::string>* >();
	int maxLength = 0;
	if(fileNames->empty()){
		std::cerr << "'error: fileNames' is Empty";
	}else{
		if(column){	// == column
			///全ての該当するファイルについて行なう
			for(int currentFileIndex = 0; currentFileIndex < fileNames->size(); currentFileIndex++){
				std::vector<std::string>* currentFileData = new std::vector<std::string>();
				std::string currentFilePath = dirPath;
				std::string currentFileName = fileNames->operator[](currentFileIndex);
				currentFilePath.append(currentFileName);
				//OpenFile
				std::ifstream ifs;
				ifs.open(currentFilePath);
				//Process Each Line
				std::string line;

				int dataCount = 0;
				while( getline( ifs, line, '\n' ) != NULL ){
					//split line with ',' (comma)
					std::istringstream ss(line);
					std::string tmp;
					
					int count = 0;
					while( getline ( ss, tmp, ',' ) != NULL ){
						if(count == index){
							currentFileData->push_back(tmp);
							dataCount++;
						}
						count++;
					}
				}
				if(dataCount > maxLength){
					maxLength = dataCount;
				}
				//CloseFile
				ifs.close();
				//push to Acquired Data
				acquiredData->push_back(currentFileData);
			}
		}else{	//if(!column) == row
			///全ての該当するファイルについて行なう
			for(int currentFileIndex = 0; currentFileIndex < fileNames->size(); currentFileIndex++){
				std::vector<std::string>* currentFileData = new std::vector<std::string>();
				std::string currentFilePath = dirPath;
				std::string currentFileName = fileNames->operator[](currentFileIndex);
				currentFilePath.append(currentFileName);
				//OpenFile
				std::ifstream ifs;
				ifs.open(currentFilePath);
				//Process Each Line
				std::string line;
				int dataCount = 0;
				int count = 0;
				while( getline( ifs, line, '\n' ) != NULL ){
					if(count == index){
						//split line with ',' (comma)
						std::istringstream ss(line);
						std::string tmp;
						while( getline ( ss, tmp, ',' ) != NULL ){
							currentFileData->push_back(tmp);
							dataCount++;
						}
						if(dataCount > maxLength){
							maxLength = dataCount;
						}
					}
					count++;
				}
				//CloseFile
				ifs.close();
				//push to Acquired Data
				acquiredData->push_back(currentFileData);
			}
		}
	}
	this->dataLength = maxLength;
	return acquiredData;
}

std::vector<int>* DataInDirectoryInvestigator::CreateInvestigateFile(std::string outputPath){
	//Open File
	std::ofstream ofs;
	ofs.open(outputPath);
	std::vector<int>*			averageData = new std::vector<int>();
	int numberOfDataFiles = data->size();
	//一行目を書き込む
	ofs << "average";
	for(int i = 1; i < numberOfDataFiles + 1; i++){
		ofs << "," << i;
	}
	ofs << std::endl;
	//各行について処理
	std::string sTmp;
	int iTmp;

	for(int line = 0; line < this->dataLength; line++){
		std::vector<std::string>	lineStringData;
		if(line %3 == 1){	//一行目は無視：posとか書いてあるだけだから．	//3stepに一度しか，posは更新されない
			double average = 0.0;
			//各ファイルのデータを収集
			for(int file = 0; file < numberOfDataFiles; file++){
				int size = data->operator[](file)->size();
				if(line > size - 1){
					sTmp = data->operator[](file)->operator[](size - 1);					
				}else{
					sTmp = data->operator[](file)->operator[](line);
				}
				iTmp = atoi(sTmp.c_str());
				lineStringData.push_back(sTmp);
				average += (double)iTmp;
			}
			//平均を算出
			average /= (double)numberOfDataFiles;
			averageData->push_back(average);
			//平均を書き出し
			ofs << average;
			//その他のデータを書き出し
			for(int file = 0; file < numberOfDataFiles; file++){
				ofs << "," << lineStringData[file];
			}
			//改行コード書き出し
			ofs << std::endl;
		}
	}

	//Close File
	ofs.close();

	return averageData;
}

void DataInDirectoryInvestigator::Run(){
	this->fileNames = this->SearchFiles("*.csv");
	this->data = this->AcquireData(true, 1);
	std::string path = this->dirPath;
	path.append("Investigation.csv");
	this->CreateInvestigateFile(path);
	return;
}

std::wstring DataInDirectoryInvestigator::s2ws(const std::string& s){
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

std::string DataInDirectoryInvestigator::ws2s(const std::wstring& ws){
	int len;
	int wsLength = ((int)ws.length() + 1) * 2;
	len = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), wsLength, 0, 0, NULL, NULL);
	//std::cout << "Length: " << len << std::endl;
	char * buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, ws.c_str(), wsLength, buf, len, NULL, NULL);
	std::string result(buf);
	delete[] buf;
	return result;
}

void DataInDirectoryInvestigator::setDataLength(int length){
	this->dataLength = length;
}