#ifndef _World_H_
#define _World_H_

#include "RobotMAV.h"

/**
	@class World
	@brief ���E��Simulate����D
	���E�ɑ��݂���Robot���쓮�����C
	�쓮�ɉ����������Ƃ��ăZ���T�[�փf�[�^�𑗂�
	@author Kenichi Yorozu
	@date 22th November 2011
	@sa Robot
	@sa SAModule
 */

class World : public Robot{
public:
	/**
		@brief Default Constructor.
		logFilePath���w�肵����CInitalize����K�v������.
	 */
	World();
	/**
		@brief Constructor
	 */
	World(std::string directoryPath, std::string fileName);
	/**
		@brief Destructor. Clear Memories.
	 */
	virtual ~World();
	/**
		@brief ���E��1clock�i�߂�
		<ol>
			<li>����Sensor�l���Z�b�g</li>
			<li>�e���{�b�g�����s</li>
			<li>�e���{�b�g�̍�����Sensor�l�����</li>
			<li>�V����Sensor�l���L�^</li>
		</ol>
	 */
	virtual void Run();
	/**
		@brief RobotModule�𒀎����s����
	 */
	virtual void RunRobots();
	/**
		@brief World��Robot��ǉ�����
	 */
	virtual void addRobot(RobotMAV* robot);
	//void moveRobot(int deltaX, int deltaY);
	//std::vector<int>* getRobotMap(int x, int y);
	/**
		@brief ���ː���Map�ɒl���L������
		@param x Robot��x���W
		@param y Robot��y���W
		@param data Robot�̕��ː��ʂɊւ���Sensor Data
	 */
	//void setRadiationMap(int x, int y, std::vector<float>* data);
	/**
		@brief ��_���Y�f��Map�ɒl���L������
		@param x Robot��x���W
		@param y Robot��y���W
		@param data Robot�̓�_���Y�f�ʂɊւ���Sensor Data
	 */
	//void setCOOMap(int x, int y, std::vector<float>* data);
	//std::vector<float>* getRadiationMap(int x, int y);
	//std::vector<float>* getCOOMap(int x, int y);

	/**
		@brief index���w������x, y���W�𓾂邽�߂�hash[x/y][index]
	 */
	int hash[2][HASH_MAX];
	/**
		@brief Initialize Fields
	 */
	void Initialize();
	/**
		@brief Robot�ւ̃|�C���^��Ԃ�
		@param index Robot��index
		@return Robot�ւ̃|�C���^
	 */
	RobotMAV* getRobot(const int index);
	/**
		@brief hash�l�𓾂�
		@param which x = 0/ y = 1����
		@param index ���Ԗڂ̔���l���ق����̂��D
	 */
	int getHash(int which, int index) const;
	/**
		@brief Field�̒n�`����ێ��D
		geoField[x][y]
		<ul>
			<li>��Q��: OUTOFAREA</li>
		</ul>
		��OpenGL����񎟌��z��ɃA�N�Z�X���邽�߂�Public�ɂ��Ă���
	 */
	int geoField[FIELD_SIZE][FIELD_SIZE];/**
		@brief Field�̌Œ�I�ȈӖ����(semantics)��ێ�
		semField[x][y];
		<ul>
			<li>ONSTART : �i�ߕ�</li>
			<li>ONCHARGER : �[�d��(�i�ߕ��ߖT)</li>
		</ul>
		@sa semField
		��OpenGL����񎟌��z��ɃA�N�Z�X���邽�߂�Public�ɂ��Ă���
	 */
	int semField[FIELD_SIZE][FIELD_SIZE];
	/**
		@brief Field�̕��ː��ʏ���ێ�
		radField[x][y];
		<ul>
			<li>���ː���: [0.0, 1.0]</li>
		</ul>
		��OpenGL����񎟌��z��ɃA�N�Z�X���邽�߂�Public�ɂ��Ă���
	 */
	float radField[FIELD_SIZE][FIELD_SIZE];
protected:
	/**
		@brief �V�����Ȃ������E�̏󋵂�Robot�ɓ`�B����.
	 */
	void Update();
	/**
		@brief index�̊Y�����郍�{�b�g���CBattery��Charge�����ǂ����𔻕ʂ���
		@param robot robot�ւ�pointer
		@return Battery�̏�ɂ��邩�ǂ����H
	 */
	bool onVision(const RobotMAV* robot);
	/**
		@brief index�̊Y�����郍�{�b�g��Battery���؂�Ă��Ȃ����ǂ����C���ʂ���
		@param robot robot�ւ�pointer
		@return Battery���؂�Ă��Ȃ����H
	 */
	bool isAlive(const RobotMAV* robot);
	/**
		@brief �e���{�b�g�̋ߐڃZ���T�ւ̓��͂��X�V
		�߂����ǂ����́CRANGE_DANGER (���݂�3.0f)�Ō��܂�
		@param robot robot�ւ�pointer
		@sa RANGE_DANGER
		@sa Constants.h
	 */
	void updateRange(RobotMAV* robot);
	/**
		@brief �e���{�b�g�̕��ː��Z���T�ւ̓��͂��X�V
		�߂����ǂ����́CMAX_RANGE�Ō��܂�
		@param robot robot�ւ�pointer
		@sa MAX_RANGE
		@sa Constants.h
	 */
	void updateRadiation(RobotMAV* robot);
	/**
		@brief �e���{�b�g��Network�Z���T�ւ̓��͓����X�V
		�e���{�b�g�̓����ϐ��u�ߖT���{�b�g�ւ̃|�C���^�̔z��v���X�V����
		@param robot robot�ւ�pointer
		@sa SenseNet
		@sa WIFI_REACH
		@sa Constants.h
	 */
	void updateNetWork(RobotMAV* robot);
	/**
		@brief �e���{�b�g��Vision�Z���T�ւ̓��͂Ȃǂ��X�V
		@param robot robot�ւ�pointer
		@sa SenseVision
	 */
	void updateVision(RobotMAV* robot);
	/**
		@brief geoField��Random�ɐ�������D
		@sa geoField
	 */
	void generateGeoField();
	/**
		@brief �t�@�C������ǂݍ���ŁCgeoField�𐶐�����
		@param filepath geoField�̃f�[�^���i�[����Ă���t�@�C���ւ�Path
		@sa geoField
	 */
	void generateGeoField(std::string filepath);
	/**
		@brief semField���i�ߕ��̏����ʒu�ɉ����Đ�������.
		@sa semField
	 */
	void generateSemField();
	/**
		@brief radField��Random�ɐ�������
		@sa radField
	 */
	void generateRadField();
#ifdef	CONSIDER_DELAY
	int count;
#endif	//CONSIDER_DELAY
};

inline void World::addRobot(RobotMAV* robot){
	//Robot�Œ�`����Ă���addModule�����s
	this->addModule(robot);
	robot->setBattery(MAX_BAT);
}

inline RobotMAV* World::getRobot(const int index){
	return ((RobotMAV*)this->modules->at(index));
}
#endif	//_World_H_