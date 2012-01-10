#ifndef _World_H_
#define _World_H_

#include "RobotMAV.h"

/**
	@class World
	@brief ���E��Simulate����D
	���E�ɑ��݂���Robot���쓮�����C
	�쓮�ɉ����������Ƃ��ăZ���T�[�փf�[�^�𑗂�
	@author Kenichi Yorozu
	@date 9th January 2011
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
		@brief Field�̒n�`����ێ��D
		geoField[x]
		<ul>
			<li>�X�^�[�g�n�_: ONSTART</li>
			<li>�S�[���n�_: ONGOAL</li>
			<li>�[�d�n�_: ONCHARGER</li>
		</ul>
		��OpenGL����񎟌��z��ɃA�N�Z�X���邽�߂�Public�ɂ��Ă���
	 */
	int geoField[LENGTH][NUM_ROBOTS];/**
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
	bool onCharger(const RobotMAV* robot);
	/**
		@brief index�̊Y�����郍�{�b�g��Battery���؂�Ă��Ȃ����ǂ����C���ʂ���
		@param robot robot�ւ�pointer
		@return Battery���؂�Ă��Ȃ����H
	 */
	bool isAlive(RobotMAV* robot);

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