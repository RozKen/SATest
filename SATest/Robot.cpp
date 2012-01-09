#include "Robot.h"

Robot::Robot(): numOfModules(0), numOfArbiters(0){
	this->modules = new std::vector<SAModule*>();
	this->arbiters = new std::vector<Arbiter*>();
	this->innerWireSrcType = new std::vector<int>();
	this->innerWireSrcIndex = new std::vector<int>();
	this->innerWireDestType = new std::vector<int>();
	this->innerWireDestIndex = new std::vector<int>();
}

Robot::Robot(std::string directoryPath, std::string fileName)
	: numOfModules(0), numOfArbiters(0)
{
	this->modules = new std::vector<SAModule*>();
	this->arbiters = new std::vector<Arbiter*>();
	this->innerWireSrcType = new std::vector<int>();
	this->innerWireSrcIndex = new std::vector<int>();
	this->innerWireDestType = new std::vector<int>();
	this->innerWireDestIndex = new std::vector<int>();
	this->innerMemory->setLogFilePath(directoryPath, fileName);
}

Robot::~Robot(){
	for(int i = 0; i < modules->size(); i++){
		delete modules->at(i);
	}
	for(int i = 0; i < arbiters->size(); i++){
		delete arbiters->at(i);
	}
	modules->clear();
	arbiters->clear();
	innerWireSrcType->clear();
	innerWireSrcIndex->clear();
	innerWireDestType->clear();
	innerWireDestIndex->clear();
	delete modules;
	delete arbiters;
	delete innerWireSrcType;
	delete innerWireSrcIndex;
	delete innerWireDestType;
	delete innerWireDestIndex;
}

void Robot::Initialize(){
}

void Robot::Run(){
	ProcessInputs();
	RunModules();
	//Log();				//Arbiter�̋����𒲂ׂ邽�߁C�_�u���O
	ProcessArbiters();
	ProcessOutputs();
	Log();
}

void Robot::RunModules(){
	for(int i = 0; i < modules->size(); i++){
		modules->at(i)->Run();
	}
}

void Robot::ProcessArbiters(){
	for(int i = 0; i < arbiters->size(); i++){
		arbiters->at(i)->Run();
	}
}

void Robot::ProcessInputs(){
	for(int i = 0; i < this->innerWireSrcType->size(); i++){
		//Source �� inputs:inputs��memory��
		if(this->innerWireSrcType->at(i) == 0){
			float signal = this->getInput(this->innerWireSrcIndex->at(i));
			//Destination �� iBoard: iBoard��innerMemory��
			if(this->innerWireDestType->at(i) == 2){
				innerMemory->setIBoard(this->innerWireDestIndex->at(i)
					, (int)(signal));
			}
			//Destination �� fBoard: fBoard��innerMemory��
			else if(this->innerWireDestType->at(i) == 3){
				innerMemory->setFBoard(this->innerWireDestIndex->at(i)
					, signal);
			}
		}
	}
}

void Robot::ProcessOutputs(){
	for(int i = 0; i < this->innerWireDestType->size(); i++){
		//Destination �� outputs: outputs��memory��
		if(this->innerWireDestType->at(i) == 1){
			//Source��iBoard: iBoard��innerMemory��
			if(this->innerWireSrcType->at(i) == 2){
				this->setOutput(this->innerWireDestIndex->at(i)
					, (float)(innerMemory->getIBoard(this->innerWireSrcIndex->at(i))));
			}
			//Source��fBoard: fBoard��innerMemory��
			else if(this->innerWireSrcType->at(i) == 3){
				this->setOutput(this->innerWireDestIndex->at(i)
					, innerMemory->getFBoard(this->innerWireSrcIndex->at(i)));
			}
		}
	}
}

void Robot::addModule(SAModule *module){
	//module��robot�ɓo�^
	modules->push_back(module);
	numOfModules = modules->size();
	//module�̐e�Ƃ��āCrobot���g��o�^
	module->setParent(this);
	int index;
	//module�̓��͂��C���̃��{�b�g��innerMemory�̏o�͂Ɛڑ�
	for(int i = 0; i < module->getNumOfInputPorts(); i++){
		index = this->innerMemory->addOutputPort(module->getInputTitles()->at(i));
		if(index < 0){
			std::cout << "Index has wrong value : " << index << ", Robot::Input: " << module->getInputTitles()->at(i) << std::endl;
		}
		module->addInputIndex(index);
	}

	//module�̏o�͂��C���̃��{�b�g��innerMemory�̓��͂Ɛڑ�
	for(int i = 0; i < module->getNumOfOutputPorts(); i++){
		index = this->innerMemory->addInputPort(module->getOutputTitles()->at(i));
		if(index < 0){
			std::cout << "Index has wrong value : " << index << ", Robot::Output: " << module->getInputTitles()->at(i) << std::endl;
		}
		module->addOutputIndex(index);
	}
	
	//Module��Controller(Robot�̂悤�ɒP�̂œ��͂��o�͂������ꍇ������)
	int flag = 0;
	//Sensor�̎�
	if(module->getNumOfInputPorts() == 0
		&& module->getNumOfOutputPorts() > 0){
		flag = 1;		//Module��Sensor
	}
	//Actuator�̎�
	if(module->getNumOfInputPorts() > 0 
		&& module->getNumOfOutputPorts() == 0){
		flag = 2;		//Module��Actuator
	}

	//module��int���o�͂��A���̃��{�b�g��innerMemory��iBoard�Ɛڑ�
	std::vector<std::string>* titles = module->getIBoardTitles();
	for(int i = 0; i < titles->size(); i++){
		index = this->innerMemory->addIntPort(titles->at(i));
		module->addIBoardIndex(index);
		if(flag != 2){
			std::string title = titles->at(i);
			this->addInput(title);
			//this->addIBoard(title.append("_i"));
			//InnerWire�̐ڑ����`
			this->innerWireSrcType->push_back(0);
			this->innerWireSrcIndex->push_back(this->inputTitles->size() - 1);
			this->innerWireDestType->push_back(2);
			this->innerWireDestIndex->push_back(index);
		}
		if(flag != 1){
			std::string title = titles->at(i);
			this->addOutput(title);
			//this->addIBoard(title.append("_i"));
			//InnerWire�̐ڑ����`
			this->innerWireSrcType->push_back(2);
			this->innerWireSrcIndex->push_back(index);
			this->innerWireDestType->push_back(1);
			this->innerWireDestIndex->push_back(this->outputTitles->size() - 1);
		}
	}
	//module��float���o�͂��A���̃��{�b�g��innerMemory��fBoard�Ɛڑ�
	titles = (module->getFBoardTitles());
	for(int i = 0; i < titles->size(); i++){
		index = this->innerMemory->addFloatPort(titles->at(i));
		module->addFBoardIndex(index);
		if(flag != 2){
			std::string title = titles->at(i);
			this->addInput(title);
			//this->addFBoard(title.append("_i"));
			//InnerWire�̐ڑ����`
			this->innerWireSrcType->push_back(0);
			this->innerWireSrcIndex->push_back(this->inputTitles->size() - 1);
			this->innerWireDestType->push_back(3);
			this->innerWireDestIndex->push_back(index);
		}
		if(flag != 1){
			std::string title = titles->at(i);
			this->addOutput(title);
			//this->addFBoard(title.append("_i"));
			//InnerWire�̐ڑ����`
			this->innerWireSrcType->push_back(3);
			this->innerWireSrcIndex->push_back(index);
			this->innerWireDestType->push_back(1);
			this->innerWireDestIndex->push_back(this->outputTitles->size() - 1);
		}
	}
	delete(titles);
}

void Robot::addArbiter(Arbiter *arbiter){
	//arbiter��robot�ɓo�^
	arbiters->push_back(arbiter);
	numOfArbiters = arbiters->size();
	//arbiter�̐e�Ƃ��āCrobot���g��o�^
	arbiter->setParent(this);
	//int index;
	///Arbiter�ɂ�memory�̊��蓖�ĕs�v
	/**
		Arbiter�ɂ́CsetSource, setDestination�̎��_�ŁC
		inputIndex, outputIndex���\�z����Ă���
	 */
}

void Robot::setLogDirectoryPath(std::string directoryPath, std::string fileName){
	this->innerMemory->setLogFilePath(directoryPath, fileName);
}

std::string Robot::getLogDirectoryPath() const{
	return this->innerMemory->getLogDirectoryPath();
}
std::string Robot::getLogFileName() const{
	return this->innerMemory->getLogFileName();
}

std::string Robot::getLogFilePath() const{
	return this->innerMemory->getLogFilePath();
}

int Robot::getNumOfModules() const{
	return numOfModules;
}

int Robot::getNumOfArbiters() const{
	return numOfArbiters;
}

SAModule*	Robot::getModule(int index){
	return this->modules->at(index);
}
Arbiter*	Robot::getArbiter(int index){
	return this->arbiters->at(index);
}