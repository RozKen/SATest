#include <sstream>
#include <string>
#include "SAModule.h"

SAModule::SAModule():numOfInputPorts(0), numOfOutputPorts(0){
	this->inputIndex = new std::vector<int>();
	this->outputIndex = new std::vector<int>();
	this->iBoardIndex = new std::vector<int>();
	this->fBoardIndex = new std::vector<int>();
	this->inputTitles = new std::vector<std::string>();
	this->outputTitles = new std::vector<std::string>();
	this->iBoardTitles = new std::vector<std::string>();
	this->fBoardTitles = new std::vector<std::string>();
	//this->memory = new Blackboard();
	this->innerMemory = new Blackboard();

#ifdef	IMPORTANCE_BASED
	importance = NO_SIGNAL;
#endif	//IMPORTANCE_BASED
}

SAModule::~SAModule(){
	inputIndex->clear();
	outputIndex->clear();
	iBoardIndex->clear();
	fBoardIndex->clear();
	inputTitles->clear();
	outputTitles->clear();
	iBoardTitles->clear();
	fBoardTitles->clear();
	delete inputIndex;
	delete outputIndex;
	delete iBoardIndex;
	delete fBoardIndex;
	delete inputTitles;
	delete outputTitles;
	delete iBoardTitles;
	delete fBoardTitles;
	//delete memory;
	delete innerMemory;
}

void SAModule::Log(){
	this->innerMemory->Log();
}

void SAModule::addInput(std::string title){
	this->inputTitles->push_back(title);
	this->numOfInputPorts = this->inputTitles->size();
}

void SAModule::addOutput(std::string title){
	this->outputTitles->push_back(title);
	this->numOfOutputPorts = this->outputTitles->size();
}

void SAModule::addIBoard(std::string title){
	this->iBoardTitles->push_back(title);
}

void SAModule::addFBoard(std::string title){
	this->fBoardTitles->push_back(title);
}

void SAModule::addInputIndex(int index){
	this->inputIndex->push_back(index);
}

void SAModule::addOutputIndex(int index){
	this->outputIndex->push_back(index);
}

void SAModule::addIBoardIndex(int index){
	this->iBoardIndex->push_back(index);
}

void SAModule::addFBoardIndex(int index){
	this->fBoardIndex->push_back(index);
}

void SAModule::setParent(SAModule* parent){
	this->parent = parent;
	this->memory = parent->getInnerMemory();
}

SAModule* SAModule::getParent() const{
	return this->parent;
}

std::vector<std::string>* SAModule::getInputTitles() const{
	return inputTitles;
}

std::vector<std::string>* SAModule::getOutputTitles() const{
	return outputTitles;
}
std::vector<std::string>* SAModule::getIBoardTitles() const{
	return this->iBoardTitles;
}
std::vector<std::string>* SAModule::getFBoardTitles() const{
	return this->fBoardTitles;
}

float SAModule::getInput(int index) const{
	//moduleへの入力はmemoryの出力から入手
	float result = memory->getOutput(this->inputIndex->at(index));
	return result;
}
void SAModule::setInput(int index, float signal){
	//moduleへの入力memoryの出力へ設定する
	memory->setOutput(this->inputIndex->at(index), signal);
}

float SAModule::getOutput(int index) const{
	//moduleからの出力は，memoryの入力から得る
	float result = memory->getInput(this->outputIndex->at(index));
	return result;
}

void SAModule::setOutput(int index, float signal){
	//moduleからの出力は、memoryの入力へ送信
	memory->setInput(this->outputIndex->at(index), signal);
}

int SAModule::getIBoard(int index) const{
	int result = this->memory->getIBoard(this->iBoardIndex->at(index));
	return result;
}

float SAModule::getFBoard(int index) const{
	float result = this->memory->getFBoard(this->fBoardIndex->at(index));
	return result;
}

void SAModule::setIBoard(int index, int signal){
	this->memory->setIBoard(this->iBoardIndex->at(index), signal);
}

void SAModule::setFBoard(int index, float signal){
	this->memory->setFBoard(this->fBoardIndex->at(index), signal);
}
#ifdef	IMPORTANCE_BASED
float SAModule::getImportance() const{
	return this->importance;
}

void SAModule::setImportance(float importance){
	this->importance = importance;
}

float SAModule::calcImportance(float value){
	return pow(value, IMPORTANCE_POWER);
}

#endif	//IMPORTANCE_BASED

int SAModule::getNumOfInputPorts() const{
	return this->numOfInputPorts;
}

int SAModule::getNumOfOutputPorts() const{
	return this->numOfOutputPorts;
}

Blackboard* SAModule::getInnerMemory() const{
	return this->innerMemory;
}

Blackboard* SAModule::getMemory() const{
	return this->memory;
}

int SAModule::getInputIndex(int indexOnModule) const{
	return this->inputIndex->at(indexOnModule);
}

int SAModule::getOutputIndex(int indexOnModule) const{
	return this->outputIndex->at(indexOnModule);
}

int SAModule::getIBoardIndex(int indexOnModule) const{
	return this->iBoardIndex->at(indexOnModule);
}

int SAModule::getFBoardIndex(int indexOnModule) const{
	return this->fBoardIndex->at(indexOnModule);
}

std::string SAModule::intToString(int num){
	std::stringstream ss;
	ss << num;
	return ss.str();
}

int SAModule::round(float value){
	return (int)(value + 0.5);
}

float SAModule::norm(const float dx, const float dy){
	return sqrt(pow(dx, 2) + pow(dy, 2));
}