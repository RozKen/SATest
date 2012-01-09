#include "RobotMAV.h"
#include "World.h"

RobotMAV::RobotMAV(){
	Initialize();
}

RobotMAV::RobotMAV(std::string directoryPath, std::string fileName)
	:Robot(directoryPath, fileName){
	Initialize();
}

RobotMAV::~RobotMAV(){
}

void RobotMAV::Initialize(){
	
	for(int i = 0; i < 3; i++){
		color[i] = 1.0f;
		for(int j = 0; j < 3; j++){
			modColor[i][j] = 1.0f;
		}
	}
	
	///Sensor��ǉ�
	///////Sensor�ɂ͂ǂ����ŏ����l���^����K�v������.
	///Module 00 : Battery�Z���T��ǉ�
	sB = new SenseBattery();
	this->addModule(sB);
	
	///Module 01 : �ʒu�Z���T��ǉ�
	sP = new SensePos();
	this->addModule(sP);

	///Module 02 : �[�d��Z���T��ǉ�
	sV = new SenseVision();
	this->addModule(sV);

	///Controller��ǉ�
	///////Controller�͔��������Ƃ��̕\���F���`����K�v������
	///Module 03 : Alive : Red
	cAl = new ContAlive();
	this->addModule(cAl);	
	modColor[0][0] = 1.0f;
	modColor[0][1] = 0.0f;
	modColor[0][2] = 0.0f;
	///Module 04 : Wander : Green
	cP = new ContProgress();
	this->addModule(cP);
	modColor[1][0] = 0.0f;
	modColor[1][1] = 1.0f;
	modColor[1][2] = 0.0f;

	/////Actuator��ǉ�
	///Module 05 : �ʒuActuator��ǉ�
	aP = new ActPos();
	this->addModule(aP);

	std::cout << "Number of Modules" << this->getNumOfModules() << std::endl;

	///////////////////Arbiter/////////////////////////////
	///Arbiter��ǉ�
	///���WireObjects
	
	///0 : Battery�Z���T->Avoid
	Arbiter* sBcAl;
	sBcAl = new Arbiter(sB, 0, cAl, 0, 2.0f);
	this->addArbiter(sBcAl);

	///1 - 11 :Vision�Z���T->Avoid
	Arbiter* sVcAl[RANGE * 2 + 1];
	for(int i = 0; i < RANGE * 2 + 1; i++){
		sVcAl[i] = new Arbiter(sV, i, cAl, i + 1, 2.0f);
		this->addArbiter(sVcAl[i]);
	}

	///12 : Position�Z���T->Progress
	Arbiter* sPcP;
	sPcP = new Arbiter(sP, 0, cP, 0, 2.0f);
	this->addArbiter(sPcP);

	///13 - 23 :Vision�Z���T->Progress
	Arbiter* sVcP[RANGE * 2 + 1];
	for(int i = 0; i < RANGE * 2 + 1; i++){
		sVcP[i] = new Arbiter(sV, i, cP, i + 1, 2.0f);
		this->addArbiter(sVcP[i]);
	}

	/////////�ȉ��̏��Ԃ͏d�v�D/////////
	/////////�K�w�̒Ⴂ�҂�����{���邽��////
	///24 Alive->�ʒuActuator	//Suppress���ꂽ�f�[�^�������Wire
	Arbiter* cAlaP;
	cAlaP = new Arbiter(cAl, 0, aP, 0, 2.0f);
	this->addArbiter(cAlaP);
#ifdef	IMPORTANCE_BASED	
	///25:Suppress Progress -> �ʒuActuator
	Arbiter* cPaP;
	cPaP = new Arbiter(cP, 0, aP, 0);
	this->addArbiter(cPaP);
#else	//IMPORTANCE_BASED	
	///25:Suppress Progress -> �ʒuActuator
	Arbiter* cPaP;
	cPaP = new Arbiter(cP, 0, aP, 0, 1.0f);
	this->addArbiter(cPaP);
#endif	//IMPORTANCE_BASED
	
	std::cout << "Number of Arbiters" << this->getNumOfArbiters() << std::endl;
	std::cout << "Number of Inputs" << this->getNumOfInputPorts() << std::endl;
	std::cout << "Number of Outputs" << this->getNumOfOutputPorts() << std::endl;
	std::cout << "Number of fBoards" << this->innerMemory->getNumOfFBoards() << std::endl;
	std::cout << "Number of iBoards" << this->innerMemory->getNumOfIBoards() << std::endl;

#ifdef	CONSIDER_DELAY
	count = 0;
#endif	//CONSIDER_DELAY
}

void RobotMAV::Run(){
#ifdef	CONSIDER_DELAY
	if(count == 0){
#endif	//CONSIDER_DELAY
		//Robot�ւ�Input����������
		ProcessInputs();
#ifdef	CONSIDER_DELAY
		count = 1;
	}else if(count == 1){
		//Robot�̊eModule�𓮂���
		RunModules();
		RunModules();
#endif	//CONSIDER_DELAY
		RunModules();
		//Arbiter���쓮������
		ProcessArbiters();

		//Robot�����Output����������
		ProcessOutputs();
		//�����̏����X�V����
		//Update();
		//Log�����
		Log();
		//�n�}���L�^����
		//logMaps();

#ifdef	CONSIDER_DELAY
		count = 0;
	}else if(count == 2){
#endif	//CONSIDER_DELAY

#ifdef	CONSIDER_DELAY
		count = 0;
	}
#endif	//CONSIDER_DELAY
}

void RobotMAV::Update(){
	return;
}

float RobotMAV::getDX() const {
	return this->getOutput(2);
}

float RobotMAV::getBattery() const{
	return this->getInput(0);
}
void RobotMAV::setBattery(float value){
	this->setInput(0, value);
}
float RobotMAV::getPosX() const{
	return this->getInput(1);
}
void  RobotMAV::setPosX(float value){
	this->setInput(1, value);
}
float RobotMAV::getVision(int index) const{
	float value;
	if(index < RANGE * 2 + 1){
		value = this->getInput(2 + index);
	}else{
		value = -9999;	//NO_SIGNAL;
	}
	return value;
}
void RobotMAV::setVision(int index, float value){
	if(index < RANGE * 2 + 1){
		this->setInput(index, value);
	}else{
		this->setInput(index, NO_SIGNAL);
	}
}

///get��Output����
float RobotMAV::getChargingFlag() const{
	return this->getOutput(0);
}
///set��Input��
void RobotMAV::setChargingFlag(float value){
	this->setInput(13, value);
}

///get��Output����
float RobotMAV::getSteps() const{
	return this->getOutput(1);
}
///set��Input��
void RobotMAV::setSteps(float value){
	this->setInput(14, value);
}

float RobotMAV::getColorR() const{
	return this->color[0];
}
float RobotMAV::getColorG() const{
	return this->color[1];
}
float RobotMAV::getColorB() const{
	return this->color[2];
}

void RobotMAV::ProcessArbiters(){
	float ratios[NUM_OF_LAYERS - 1];
	for(int i = 0; i < arbiters->size(); i++){
		arbiters->at(i)->Run();
		switch(i){
		case 25:	//Alive Suppress Avoid and ActPos
			ratios[0] = arbiters->at(i)->getCurrentRatio();
			break;
		default:
			break;
		}
	}

	///Set RobotColor According to Suppress

	if(this->getPosX() == NO_SIGNAL){
		color[0] = 0.3f;
		color[1] = 0.3f;
		color[2] = 0.3f;
	}else{
		for(int j = 0; j < 3; j++){
			color[j] = modColor[0][j];
		}
		for(int i = 0; i < NUM_OF_LAYERS - 1; i++){
			for(int j = 0; j < 3; j++){
#ifdef	INVERSE_SUPPRESSOR
				color[j] = color[j] * ratios[i] + modColor[i+1][j] * (1.0f - ratios[i]);
#else	//INVERSE_SUPPRESSOR
				color[j] = color[j] * (1.0f - ratios[i]) + modColor[i+1][j] * ratios[i];
#endif	//INVERSE_SUPPRESSOR
			}
		}
	}
}

