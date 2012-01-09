#include "Logger.h"
#include "Blackboard.h"
#include <iostream>

void Blackboard::Update(){
}

void Blackboard::Log(){
	///�L�^
	logger->Log();
}

int Blackboard::addInputPort(std::string title){
	///�ϐ��̈��inputs��Ɋm�ہC�o�^
	inputs->push_back(NO_SIGNAL);
	///inputs����index���擾
	int index = inputs->size() - 1;
	///Blackboard�̓��̓|�[�g�����X�V
	numOfInputPorts = inputs->size();
	///Logger�ɋL�^����悤�ݒ�
	logger->add(title, inputs, index);
	if(index < 0){
		std::cout << "Index has wrong value : " << index << ", Blackboard::input: " << title << std::endl;
	}
	return index;
}
int Blackboard::addOutputPort(std::string title){
	///�ϐ��̈��outputs��Ɋm�ہC�o�^
	outputs->push_back(NO_SIGNAL);
	///outputs����index���擾
	int index = outputs->size() - 1;
	///Blackboard�̏o�̓|�[�g�����X�V
	numOfOutputPorts = outputs->size();
	///Logger�ɋL�^����悤�ݒ�
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