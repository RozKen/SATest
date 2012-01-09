#include "World.h"
#include "RobotMAV.h"
#include "CistreamCSV.hpp"
World::World(){
	//robotPos = new std::vector<position>();
	int tmp[2][HASH_MAX] = {
		{0,0,1,0,-1,0,1,2,1,0,-1,-2,-1,0,1,2,3,2,1,0,-1,-2,-3,-2,-1},
		{0,1,0,-1,0,2,1,0,-1,-2,-1,0,1,3,2,1,0,-1,-2,-3,-2,-1,0,1,2}
	};
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < HASH_MAX; j++){
			hash[i][j] = tmp[i][j];
		}
	}
}

World::World(std::string directoryPath, std::string fileName)
	:Robot(directoryPath, fileName){
	int tmp[2][HASH_MAX] = {
		{0,0,1,0,-1,0,1,2,1,0,-1,-2,-1,0,1,2,3,2,1,0,-1,-2,-3,-2,-1},
		{0,1,0,-1,0,2,1,0,-1,-2,-1,0,1,3,2,1,0,-1,-2,-3,-2,-1,0,1,2}
	};
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < HASH_MAX; j++){
			hash[i][j] = tmp[i][j];
		}
	}
	this->Initialize();
}

World::~World(){
}

void World::Initialize(){
	generateGeoField("geoField.csv");
	generateSemField();
	generateRadField();
#ifdef	CONSIDER_DELAY
	count = 0;
#endif	//CONSIDER_DELAY
}

int World::getHash(int which, int index) const{
	return hash[which][index];
}

void World::Run(){
	this->RunRobots();
	this->Update();
#ifdef	CONSIDER_DELAY
	if(count % 2 == 1){
#endif	//CONSIDER_DELAY
		//���݂̏�Ԃ��L�^
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
		if(onVision(robot)){
			battery += (float)BAT_GAIN;
		}
		//Battery�ȊO�́C�����Ă���Ƃ������C�쓮����
		if(isAlive(robot)){
			////////Position////////
			//fPosX = fPosX + fDeltaX
			float posX = robot->getPosX();
			float dX = robot->getDX();
			float newX;
			if(dX != NO_SIGNAL){
				newX = posX + dX;
			}else{
				newX = posX;
			}
			if(newX < 0.0f){
				newX = 0.0f;
			}else if(newX > FIELD_SIZE){
				newX = FIELD_SIZE;
			}
			
			//fPosY = fPosY + fDeltaY
			float posY = robot->getPosY();
			float dY = robot->getDY();
			float newY;
			if(dY != NO_SIGNAL){
				newY = posY + dY;
			}else{
				newY = posY;
			}
			if(newY < 0.0f){
				newY = 0.0f;
			}else if(newY > FIELD_SIZE){
				newY = FIELD_SIZE;
			}
			
			//��Q���ƏՓ˂��Ă�����C���肬��̂Ƃ���ɖ߂�
			for(int i = -1; i <= 1; i++){
				for(int j = -1; j <= 1; j++){
					int tmpX = (int)newX + i;
					int tmpY = (int)newY + j;
					if(tmpX >= 0 && tmpX < FIELD_SIZE && tmpY >= 0 && tmpY < FIELD_SIZE){
						//����傫���ꏊ�ɂ���Z������Q����������
						if(geoField[tmpX][tmpY] == OUTOFAREA){
							//��Q���ƂԂ���Ȃ��悤�Ɉړ�������
							newX = (int)newX - i;
							newY = (int)newY - j;
						}
					}
				}
			}

			//set Position
			robot->setPosX(newX);
			robot->setPosY(newY);
			//�i�񂾋����ɉ�����Battery�����炷
			if(sqrt(dX) > 0 || sqrt(dY) > 0){
				battery -= (float)BAT_LOSS * this->norm(dX, dY);
			}

			////////Direction////////
			double theta;
			if(dX != NO_SIGNAL || dY != NO_SIGNAL){
				//�P��pi : [-PI/2, PI/2]
				double phi;
				if(dX == 0.0f){
					if(dY > 0.0f){
						phi = PI / 2.0;
					}else{
						phi = -PI / 2.0;
					}
				}else{
					phi = atan( dY / dX );
				}
				theta = phi * 180.0 / PI;
			
				//�ی����Ƃɒǉ��̏������s�Ȃ�
				if(dX < 0){
					theta += 180.0;
				}else if(dY < 0){	//��l�ی�
					theta += 360.0f;
				}
			}else{
				theta = robot->getDirection();
			}
			robot->setDirection(theta);
			////////Range////////
			updateRange(robot);
			////////Radiation////////
			updateRadiation(robot);
			////////Vision///////
			updateVision(robot);
			////////Network////////
			updateNetWork(robot);
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
	for(int i = 0; i < FIELD_SIZE; i++){
		for(int j = 0; j < FIELD_SIZE; j++){
			if(rand() < P_BARRIER){
				geoField[i][j] = OUTOFAREA;
			}else{
				geoField[i][j] = NORMAL;
			}
		}
	}
	////File Output
	std::string fileName = this->getLogDirectoryPath();
	fileName.append("/geoField.csv");
	std::ofstream ofsGeoField(fileName);
	for(int i = FIELD_SIZE - 1; i >= 0; i--){
		for(int j = 0; j < FIELD_SIZE; j++){
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
	for(int i = 0; i < FIELD_SIZE; i++){
		for(int j = 0; j < FIELD_SIZE; j++){
			csv >> this->geoField[j][i];
		}
		csv >> endl;
	}

	////File Output
	std::string fileName = this->getLogDirectoryPath();
	fileName.append("/geoField.csv");
	std::ofstream ofsGeoField(fileName);
	for(int i = FIELD_SIZE - 1; i >= 0; i--){
		for(int j = 0; j < FIELD_SIZE; j++){
			ofsGeoField << geoField[j][i] << ",";
		}
		ofsGeoField << std::endl;
	}
	ofsGeoField.close();
}

void World::generateSemField(){
	//semField
	int startX = this->round((float)START_X);
	int startY = this->round((float)START_Y);
	double startR = START_R;
	for(int i = 0; i < FIELD_SIZE; i++){
		for(int j = 0; j < FIELD_SIZE; j++){
			if(startR > norm((float)(startX - i),(float)(startY - j))){
				if(i == startX && j == startY){
					semField[i][j] = ONSTART;
				}else{
					semField[i][j] = ONCHARGER;
				}
			}else{
				semField[i][j] = OUTOFAREA;
			}
		}
	}

	////File Output
	std::ofstream ofsSemField;
	std::string fileName = this->getLogDirectoryPath();
	fileName.append("/semField.csv");
	ofsSemField.open(fileName);
	for(int i = FIELD_SIZE - 1; i >= 0; i--){
		for(int j = 0; j < FIELD_SIZE; j++){
			ofsSemField << semField[j][i] << ",";
		}
		ofsSemField << std::endl;
	}
	ofsSemField.close();
}

void World::generateRadField(){
	
	//initialize as NO_DATA_ON_FIELD
	for(int i = 0; i < FIELD_SIZE; i++){
		for(int j = 0; j < FIELD_SIZE; j++){
			radField[i][j] = NO_DATA_ON_FIELD;
		}
	}

	//���ː������̓_�����𐶐�����̂ɗ��p
	Random<boost::uniform_int<> > number(MIN_RAD_POINT, MAX_RAD_POINT);
	//���ː������̓_���̏ꏊ�𐶐�����̂ɗ��p
	Random<boost::uniform_int<> > pos(0, FIELD_SIZE);
	//���ː������̓_���������ː��ʂ𐶐�����̂ɗ��p
	Random<boost::uniform_real<> > rand(0.0f, MAX_RAD_VALUE);


	int numberOfRadPoints = number();
	std::vector<int> emitPosX;
	std::vector<int> emitPosY;
	//Create Radiation Emitting Point
	int x, y;
	for(int i = 0; i < numberOfRadPoints; i++){
		x = pos();
		y = pos();
		radField[x][y] = rand();
		emitPosX.push_back(x);
		emitPosY.push_back(y);
	}
	//Calculate Radiation Volume at each point
	for(int i = 0; i < FIELD_SIZE; i++){
		for(int j = 0; j < FIELD_SIZE; j++){
			if(radField[i][j] == NO_DATA_ON_FIELD){
				radField[i][j] = 0.0f;
				//Sum Up Values derive from each emitting point.
				for(int k = 0; k < numberOfRadPoints; k++){
					float distance = this->norm(i - emitPosX.at(k), j - emitPosY.at(k));
					distance *= (float)FIELD_SCALE;
					radField[i][j] += radField[emitPosX.at(k)][emitPosY.at(k)] / pow(distance, 2);
				}
			}
		}
	}

	////File Output
	std::ofstream ofsRadField;
	std::string fileName = this->getLogDirectoryPath();
	fileName.append("/radField.csv");
	ofsRadField.open(fileName);

	//Write Out Conditions
	ofsRadField << "radPoints," << numberOfRadPoints << std::endl;
	ofsRadField << "point,posX,posY,radValue" << std::endl;
	for(int i = 0; i < numberOfRadPoints; i++){
		ofsRadField << i << "," << emitPosX.at(i) << ",";
		ofsRadField << emitPosY.at(i) << "," << radField[emitPosX.at(i)][emitPosY.at(i)] << std::endl;
	}
	//Write Out Rad Field Map
	for(int i = FIELD_SIZE - 1; i >= 0; i--){
		for(int j = 0; j < FIELD_SIZE; j++){
			ofsRadField << radField[j][i] << ",";
		}
		ofsRadField << std::endl;
	}

	emitPosX.clear();
	emitPosY.clear();

	ofsRadField.close();

}

bool World::onVision(const RobotMAV* robot){
	bool result = false;
	int i = robot->getPosX();
	int j = robot->getPosY();
	if(this->semField[i][j] == ONCHARGER || this->semField[i][j] == ONSTART){
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

void World::updateRadiation(RobotMAV* robot){
	//robot�̍��W (int)
	int x = round(robot->getPosX());
	int y = round(robot->getPosY());
	float value;
	//�T��������W
	int searchX;
	int searchY;
	for(int i = 0; i < MAX_AREA; i++){
		searchX = x + hash[0][i];
		searchY = y + hash[1][i];
		if(searchX >= 0 && searchX < FIELD_SIZE
			&& searchY >= 0 && searchY < FIELD_SIZE){
				value = radField[searchX][searchY];
				robot->setRad(i, value);
		}
	}
	return;
}

void World::updateRange(RobotMAV* robot){
	//robot�̍��W (int)
	int x = round(robot->getPosX());
	int y = round(robot->getPosY());
	/**
		@brief value[x][y] 
		x,y�͍��������_
	 */
	int value[RANGE * 2 + 1][RANGE * 2 + 1];
	//int value[7][7];

	for(int i = 0; i < RANGE * 2 + 1; i++){
		for(int j = 0; j < RANGE * 2 + 1; j++){
			value[i][j] = NORMAL;
		}
	}

	////////Field�O�̌��o////////
	int boundary[4];
	//north
	boundary[0] = FIELD_SIZE - 1 - round(y);
	//east
	boundary[1] = FIELD_SIZE - 1 - round(x);
	//south
	boundary[2] = y;
	//west
	boundary[3] = x;
	for(int i = 0; i < 4; i++){
		if(boundary[i] > RANGE){
			boundary[i] = RANGE;
		}else{
			for(int j = boundary[i]; j < RANGE; j++){
				switch(i){
				case 0:		//North Boundary
					for(int x = 0; x < RANGE * 2 + 1; x++){
						value[x][RANGE + 1 + j] = OUTOFAREA;
					}
					break;
				case 1:		//East Boundary
					for(int y = 0; y < RANGE * 2 + 1; y++){
						value[RANGE + 1 + j][y] = OUTOFAREA;
					}
					break;
				case 2:		//South Boundary
					for(int x = 0; x < RANGE * 2 + 1; x++){
						value[x][RANGE - 1 - j] = OUTOFAREA;
					}
					break;
				case 3:		//West Boundary
					for(int y = 0; y < RANGE * 2 + 1; y++){
						value[RANGE - 1 - j][y] = OUTOFAREA;
					}
					break;
				default:	//�z��O
					break;
				}
			}
		}
	}

	////////�ߗׂ̏�Q�����////////
	int searchX;	//����Search���Ă���ʒuX
	int searchY;	//����Search���Ă���ʒuY
	for(int i = -RANGE; i <= RANGE; i++){
		for(int j = -RANGE; j <= RANGE; j++){
			searchX = x + i;
			searchY = y + j;
			//Robot�̋ߖT��Field���̎�
			if( searchX >= 0 && searchX < FIELD_SIZE
				&& searchY >= 0 && searchY < FIELD_SIZE){
					//�n�`��񂪕��ʂ���Ȃ��Ƃ�
					if(geoField[searchX][searchY] != NORMAL){
						value[i + RANGE][j + RANGE] = geoField[searchX][searchY];
					}
			}
		}
	}
	////////�ߗׂ�robot��T��////////
	float robotX, robotY;
	for(int i = 0; i < this->numOfModules; i++){
		robotX = this->getRobot(i)->getPosX();
		robotY = this->getRobot(i)->getPosY();
		if(robotX != robot->getPosX() && robotY != robot->getPosY()){
			if( robotX < x + RANGE && robotX > x - RANGE
				&& robotY < y + RANGE && robotY > y - RANGE){
					value[(int)robotX - x + RANGE][(int)robotY - y + RANGE] = ROBOTEXIST;
			}
		}
	}
	////////RANGE��input////////
	////�ǂ�Range�֏o�͂��邩�����肷��
	/**
		@brief ����index�ɂ���value���ǂ�RANGE�ɑ����邩��ێ�����
		<ul>
			<li>Range0: 345 <= theta < 360 || 0 <= theta < 15 </li>
			<li>Range1: 15 <= theta < 45</li>
			<li>Range2: 45 <= theta < 75</li>
			<li>Range3: 75 <= theta < 105</li>
			<li>Range4: 105 <= theta < 135</li>
			<li>Range5: 135 <= theta < 165</li>
			<li>Range6: 165 <= theta < 195</li>
			<li>Range7: 195 <= theta < 225</li>
			<li>Range8: 225 <= theta < 255</li>
			<li>Range9: 255 <= theta < 285</li>
			<li>Range10: 285 <= theta < 315</li>
			<li>Range11: 315 <= theta < 345</li>
		</ul>
		@sa value
	 */
	int orientation[RANGE * 2 + 1][RANGE * 2 + 1];
	for(int i = 0; i < RANGE * 2 + 1; i++){
		for(int j = 0; j < RANGE * 2 + 1; j++){
			//�P��pi : [-PI/2, PI/2]
			double phi = atan( ((double)j - (double)RANGE) / ((double)i - (double)RANGE) );
			double theta = phi * 180.0 / PI;
			
			//�ی����Ƃɒǉ��̏������s�Ȃ�
			if(i - RANGE < 0){
				theta += 180.0;
			}else if(j - RANGE < 0){	//��l�ی�
				theta += 360.0f;
			}

			if( (theta >= 345 && theta < 360) || ( theta >= 0 && theta < 15) ){
				orientation[i][j] = 0;
			}else if ( theta >= 15 && theta < 45){
				orientation[i][j] = 1;
			}else if ( theta >= 45 && theta < 75){
				orientation[i][j] = 2;
			}else if ( theta >= 75 && theta < 105){
				orientation[i][j] = 3;
			}else if ( theta >= 105 && theta < 135){
				orientation[i][j] = 4;
			}else if ( theta >= 135 && theta < 165){
				orientation[i][j] = 5;
			}else if ( theta >= 165 && theta < 195){
				orientation[i][j] = 6;
			}else if ( theta >= 195 && theta < 225){
				orientation[i][j] = 7;
			}else if ( theta >= 225 && theta < 255){
				orientation[i][j] = 8;
			}else if ( theta >= 255 && theta < 285){
				orientation[i][j] = 9;
			}else if ( theta >= 285 && theta < 315){
				orientation[i][j] = 10;
			}else if ( theta >= 315 && theta < 345){
				orientation[i][j] = 11;
			}else{
				//orientation[RANGE][RANGE]�̂�
				orientation[i][j] = 0;
			}
		}
	}
	////�ł��߂�SensorValue�����߂�D
	/**
		@brief �e������RangeSensor�֑���M��
	 */
	float signal[RANGE_DIV];
	for(int i = 0; i < RANGE_DIV; i++){
		//�M���̏�����.�]�T�������đ傫�߂ɂ��Ă���D
		signal[i] = RANGE_DANGER * 2.0f;
	}
	//��ԋ߂���Q���̋����ŏ㏑�����Ă���
	for(int i = 0; i < RANGE * 2 + 1; i++){
		for(int j = 0; j < RANGE * 2 + 1; j++){
			if(value[i][j] != NORMAL){
				float distance = norm(((double)(i - RANGE)), ((double)(RANGE - j)));

				if( distance < signal[orientation[i][j]] ){
					signal[orientation[i][j]] = distance;
				}
			}
		}
	}

	////Range�֏o�͂���
	for(int i = 0; i < RANGE_DIV; i++){
		robot->setRange(i, signal[i]);
		//robot->setInput(4 + i, signal[i]);
	}

}

void World::updateNetWork(RobotMAV* robot){
	//���݈ʒu
	float x = robot->getPosX();
	float y = robot->getPosY();
	//���݈ʒu�ɋ߂����{�b�g��PickUp
	std::vector<RobotMAV*>* neighbors = new std::vector<RobotMAV*>();
	std::vector<float>* neighborsIndex = new std::vector<float>();
	for(int i = 0; i < this->numOfModules; i++){
		float xi = (this->getRobot(i))->getPosX();
		float yi = (this->getRobot(i))->getPosY();
		float distance = this->norm(xi - x, yi - y);
		//�������g�����O����
		if(distance < WIFI_REACH && distance > 0){
			//�d�r���Ȃ����̂́C�q����Ȃ��悤�ɂ���.
			if(this->getRobot(i)->getBattery() >= 1.0f){
				neighbors->push_back(this->getRobot(i));
				neighborsIndex->push_back(distance);
			}
		}
	}
	//�����Ƀ\�[�g
	std::sort(neighborsIndex->begin(), neighborsIndex->end());
	robot->clearNearest();
	for(int i = 0; i < WIFI_CONNECT && i < neighbors->size(); i++){
		int j = 0;// = neighborsIndex->at(i);
		while(j < neighbors->size()){
			float xj = neighbors->at(j)->getPosX();
			float yj = neighbors->at(j)->getPosY();
			if(this->norm(xj - x, yj - y) == neighborsIndex->at(i)){
				break;
			}else{
				j++;
			}
		}
		//nearest��ݒ�
		robot->pushNearest(neighbors->at(j));
		//robot��Input���蓮�Őݒ�
		robot->setRobot(i, neighbors->at(j)->getPosX() - x, true);
		robot->setRobot(i, neighbors->at(j)->getPosY() - y, false);
	}
	neighbors->clear();
	delete neighbors;
	neighborsIndex->clear();
	delete neighborsIndex;
}

void World::updateVision(RobotMAV* robot){
	robot->setVision(this->semField[round(robot->getPosX())][round(robot->getPosY())]);
}