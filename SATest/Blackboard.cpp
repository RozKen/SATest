#include "Logger.h"
#include "Blackboard.h"
#include <iostream>

void Blackboard::Update(){
}

void Blackboard::Log(){
	///記録
	logger->Log();
}

int Blackboard::addInputPort(std::string title){
	///変数領域をinputs上に確保，登録
	inputs->push_back(NO_SIGNAL);
	///inputs内のindexを取得
	int index = inputs->size() - 1;
	///Blackboardの入力ポート数を更新
	numOfInputPorts = inputs->size();
	///Loggerに記録するよう設定
	logger->add(title, inputs, index);
	if(index < 0){
		std::cout << "Index has wrong value : " << index << ", Blackboard::input: " << title << std::endl;
	}
	return index;
}
int Blackboard::addOutputPort(std::string title){
	///変数領域をoutputs上に確保，登録
	outputs->push_back(NO_SIGNAL);
	///outputs内のindexを取得
	int index = outputs->size() - 1;
	///Blackboardの出力ポート数を更新
	numOfOutputPorts = outputs->size();
	///Loggerに記録するよう設定
	logger->add(title, outputs, index);
	if(index < 0){
		std::cout << "Index has wrong value : " << index << ", Blackboard::output: " << title << std::endl;
	}
	return index;
}

int Blackboard::addIntPort(std::string title){
	iBoard->push_back(NO_SIGNAL);
	int index = iBoard->size() - 1;
	logger->add(title, iBoard, index);
	if(index < 0){
		std::cout << "Wrong Index Value on Blackboard::addFloatPort: " << index << "," <<title << std::endl;
	}
	return index;
}

int Blackboard::addFloatPort(std::string title){
	fBoard->push_back(NO_SIGNAL);
	int index = fBoard->size() - 1;
	logger->add(title, fBoard, index);
	if(index < 0){
		std::cout << "Wrong Index Value on Blackboard::addFloatPort: " << index << "," <<title << std::endl;
	}
	return index;
}

void Blackboard::setLogFilePath(std::string logFilePath){
	this->logger->setFilePath(logFilePath);
}

void Blackboard::setLogFilePath(std::string fileDirectoryPath, std::string fileName){
	this->logger->setFilePath(fileDirectoryPath, fileName);
}

std::string Blackboard::getLogFilePath() const{
	return this->logger->getFilePath();
}

std::string Blackboard::getLogFileName() const{
	return this->logger->getFileName();
}

std::string Blackboard::getLogDirectoryPath() const{
	return this->logger->getDirectoryPath();
}

float Blackboard::getInput(int index) const{
	return inputs->at(index);
}
float Blackboard::getOutput(int index) const{
	return outputs->at(index);
}
void Blackboard::setInput(int index, float signal){
	inputs->at(index) = signal;
}
void Blackboard::setOutput(int index, float signal){
	outputs->at(index) = signal;
}
int Blackboard::getIBoard(int index) const{
	return iBoard->at(index);
}
float Blackboard::getFBoard(int index) const{
	return fBoard->at(index);
}
void Blackboard::setIBoard(int index, int signal){
	iBoard->at(index) = signal;
}
void Blackboard::setFBoard(int index, float signal){
	fBoard->at(index) = signal;
}

int Blackboard::getNumOfOutputs() const{
	return numOfOutputPorts;
}
int Blackboard::getNumOfInputs() const{
	return numOfInputPorts;
}
int Blackboard::getNumOfIBoards() const{
	return this->iBoard->size();
}
int Blackboard::getNumOfFBoards() const{
	return this->fBoard->size();
}