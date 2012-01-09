#include "World.h"
#include "RobotMAV.h"
#include "CistreamCSV.hpp"
World::World(){
	
}

World::World(std::string directoryPath, std::string fileName)
	:Robot(directoryPath, fileName){
	this->Initialize();
}

World::~World(){
}

void World::Initialize(){
	generateGeoField();//"geoField.csv");
#ifdef	CONSIDER_DELAY
	count = 0;
#endif	//CONSIDER_DELAY
}

void World::Run(){
	this->RunRobots();
	this->Update();
#ifdef	CONSIDER_DELAY
	if(count % 2 == 1){
#endif	//CONSIDER_DELAY
		//現在の状態を記録
		Log();
#ifdef	CONSIDER_DELAY
	}
	count++;
#endif	//CONSIDER_DELAY
}

void World::RunRobots(){
	for(int i = 0; i < this->modules->size(); i++){
		((RobotMAV*)(this->modules->at(i)))->Run();
	}
}

void World::Update(){
	RobotMAV* robot;
	for(int i = 0; i < this->modules->size(); i++){
		robot = this->getRobot(i);
		////////Battery////////
		float battery = robot->getBattery();
		if(onCharger(robot)){
			battery += (float)BAT_GAIN;
		}
		//Battery以外は，生きているときだけ，作動する
		if(isAlive(robot)){
			////////Position////////
			//fPosX = fPosX + fDeltaX
			float posX = robot->getPosX();
			if(posX == NO_SIGNAL){
				posX = 0.0f;
			}else if(posX == -1.0f){
				posX = 0.0f;
			}
			float dX = robot->getDX();
			float newX;
			if(dX != NO_SIGNAL){
				newX = posX + dX;
			}else{
				newX = posX;
			}
			if(newX < 0.0f){
				newX = 0.0f;
			}else if(newX > LENGTH){
				newX = LENGTH;
			}

			//set Position
			robot->setPosX(newX);

			//進んだ距離に応じてBatteryを減らす
			if(sqrt(dX) > 0){
				battery -= (float)BAT_LOSS * sqrt(dX);
			}

			////////Vision///////
			updateVision(robot);
			///////Others////////
			robot->setChargingFlag(robot->getChargingFlag());
			robot->setSteps(robot->getSteps());
#ifdef _DEBUG
		//	std::cout << "Robot[" << i << "] : " << posX + dX << ", " << posY + dY << std::endl;
#endif	//_DEBUG
		}

		////////MAX_BAT Condition////////
		if(battery > MAX_BAT){
			robot->setBattery(MAX_BAT);
			//robot->setInput(0, (float)MAX_BAT);
		}else if(battery < 0){
			robot->setBattery(0.0f);
			//robot->setInput(0, 0.0f);
		}else{
			robot->setBattery(battery);
			//robot->setInput(0, battery);
		}
		
		//For Debug//////////////////////////////////////////////////////////////////////////////////
		//robot->setInput(0, (float)MAX_BAT);
	}
}

	//geoField
void World::generateGeoField(){
	Random<boost::uniform_real<> > rand;
	for(int i = 0; i < LENGTH; i++){
		int field;
		if(rand() < P_CHARGER){
			field = ONCHARGER;
		}else{
			field = NORMAL;
		}
		for(int j = 0; j < NUM_ROBOTS; j++){
				geoField[i][j] = field;
		}
	}

	for(int j = 0; j < NUM_ROBOTS; j++){
		geoField[0][j] = ONSTART;
		geoField[LENGTH - 1][j] = ONGOAL;
	}

	////File Output
	std::string fileName = this->getLogDirectoryPath();
	fileName.append("/geoField.csv");
	std::ofstream ofsGeoField(fileName);
	//for(int i = LENGTH - 1; i >= 0; i--){
	for(int i = 0; i < LENGTH; i++){
		for(int j = 0; j < NUM_ROBOTS; j++){
			ofsGeoField << geoField[j][i] << ",";
		}
		ofsGeoField << std::endl;
	}
	ofsGeoField.close();
}

void World::generateGeoField(std::string filepath){
	////Read Data from File
	std::ifstream ifs(filepath);
	CistreamCSV csv(ifs);
	for(int i = 0; i < LENGTH; i++){
		int value;
		csv >> value;
		for(int j = 0; j < NUM_ROBOTS; j++){
			this->geoField[j][i] = value;
		}
		csv >> endl;
	}

	////File Output
	std::string fileName = this->getLogDirectoryPath();
	fileName.append("/geoField.csv");
	std::ofstream ofsGeoField(fileName);
	for(int i = LENGTH - 1; i >= 0; i--){
		for(int j = 0; j < NUM_ROBOTS; j++){
			ofsGeoField << geoField[j][i] << ",";
		}
		ofsGeoField << std::endl;
	}
	ofsGeoField.close();
}

bool World::onCharger(const RobotMAV* robot){
	bool result = false;
	int i = robot->getPosX();
	if(this->geoField[i][0] == ONCHARGER || this->geoField[i][0] == ONSTART){
		result = true;
	}
	return result;
}

bool World::isAlive(const RobotMAV* robot){
	bool result = true;
	if(robot->getBattery() < 1.0){
		result = false;
	}
	return result;
}

void World::updateVision(RobotMAV* robot){
	for(int i = 0; i < RANGE * 2 + 1; i++){
		int pos = robot->getPosX() - RANGE + i;
		if(pos >= 0 && pos <= LENGTH){
			robot->setVision(i, this->geoField[pos][0]);
		}else{
			robot->setVision(i, OUTOFAREA);
		}
	}
}