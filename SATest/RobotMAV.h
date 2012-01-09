#ifndef _Robot_MAV_H_
#define _Robot_MAV_H_

#include "Constants.h"
#include "Sensors.h"
#include "Controllers.h"
#include "Actuators.h"
#include "Robot.h"

/**
	@brief �T��Robot
	<h1>Sensors</h1>
	<ul>
		<li>Battery Level</li>
		<li>Position</li>
		<li>Range</li>
	</ul>
	<h1>Controllers</h1>
	<ul>
		<li>Avoid</li>
		<li>Alive</li>
		<li>Wander</li>
	</ul>
	<h1>Actuators</h1>
	<ul>
		<li>Position</li>
	</ul>
	@sa Actuators.h
	@sa Controllers.h
	@sa Sensors.h
	
	@sa SenseBattery
	@sa SenseRange
	@sa SenseRange
	
	@sa ContAvoid
	@sa ContAlive
	@sa ContWander

	@sa ActPos

 */
class RobotMAV : public Robot{
public:
	/**
		@brief Default Constructor
		Module��ǉ����Ă����D
	 */
	RobotMAV();
	RobotMAV(std::string directoryPath, std::string fileName);

	/**
		@brief Destructor. Clear Memories.
	 */
	virtual ~RobotMAV();

	/**
		@brief ���s���̓���
		@sa SAModule::Run()
	 */
	virtual void Run();
	/**
		@brief �����̒n�}���Ȃǂ��X�V����
		<ol>
			<li>updateInnerGeoMap</li>
			<li>updateInnerRadMap</li>
		</ol>
		@sa updateInnerGeoMap
		@sa updateInnerRadMap
	 */
	virtual void Update();

	/////////Getters and Setters/////////////
	///These are Created Only for Easy Code Reading
	///Each Methods are equivalent to [get/set]Inputs(index) excepts getColor[R,G,B]

	/**
		@brief getDX via Output
	 */
	float getDX() const;
	/**
		@brief getDY via Output
	 */
	float getDY() const;

	/**
		@brief getBatteryStatus via Input
	 */
	float getBattery() const;
	/**
		@brief setBatteryStatus via Input
	 */
	void setBattery(float value);
	/**
		@brief get X position via Input
	 */
	float getPosX() const;
	/**
		@brief set X position via Input
	 */
	void setPosX(float value);
	/**
		@brief get Y position via Input
	 */
	float getPosY() const;
	/**
		@brief set Y position via Input
	 */
	void setPosY(float value);
	/**
		@brief get Direction via Input
	 */
	float getDirection() const;
	/**
		@brief set Direction via Input
	 */
	void setDirection(float value);
	/**
		@brief get Range Sensor Value
		@param index �p�x�ɉ�����index
		@return Range Sensor�l
	 */
	float getRange(int index) const;
	/**
		@brief set Range Sensor Value
		@param index �p�x�ɉ�����index
		@param value Range Sensor�l
	 */
	void setRange(int index, float value);
	/**
		@brief get Radiation Sensor Value
		@param index Hash�ɉ�����index
		@return Radiation Sensor �l
	 */
	float getRad(int index) const;
	/**
		@brief set Radiation Sensor Value. World���痘�p�����
		@param index Hash�ɉ�����index
		@param value Radiation Sensor�l

	 */
	void setRad(int index, float value);
	/**
		@brief get the value of Battery Charger Sensor
		@return value of Battery Charger Sensor
	 */
	int getVision() const;
	/**
		@brief set the value of Battery Charger Sensor
		@param value the value of Battery Charger Sensr
		@sa World::updateVision(RobotMAV* robot)
	 */
	void setVision(float value);
	/**
		@brief Get Relative Root of this Robot
		@return relativeRoot
		@sa relativeRoot
	 */
	RobotMAV* getRelativeRoot();
	/**
		@brief set Relative Root of this Robot
		@param root RelativeRoot to be set
		@sa relativeRoot
	 */
	void setRelativeRoot(RobotMAV* root);
	
	/**
		@brief get Number of Hops to the Headquarters
		@return Number of Hops
	 */
	int getHop() const;
	/**
		@brief set Number of Hops to the Headquarters
		@param hop Number of Hops
	 */
	void setHop(int hop);
	
	/**
		@brief get Coordinate of Robot at index
		@param index which Robot
		@param x x-axis : true, y-axis. false
		@return Coordinate of the Robot
	 */
	float getRobot(int index, bool x);
	/**
		@brief set Coordinate of Robot at index
		@param index which Robot
		@param value Robot Position
		@param x x-axis : true, y-axis. false
	 */
	void setRobot(int index, float value, bool x);

	/**
		@brief get Red Color of Current Robot
	 */
	float getColorR() const;
	/**
		@brief get Green Color of Current Robot
	 */
	float getColorG() const;
	/**
		@brief get Blue Color of Current Robot
	 */
	float getColorB() const;

	///////�\���p///////
	/**
		@brief get x coordinate of Objective on ContExplore
		@return x coordinate of Objective on ContExplore
	 */
	float getObjectiveX() const;
	/**
		@brief get x coordinate of Objective on ContExplore
		@return x coordinate of Objective on ContExplore
	 */
	float getObjectiveY() const;

	/**
		@brief ���ׂĂ�Arbiter�����s����
	 */
	virtual void ProcessArbiters();

	/**
		@brief SenseNet�ւ̃|�C���^��Ԃ�. World����̌Ăяo���ɗ��p����
		@return SenseNet ���W���[���ւ̃|�C���^
		@sa SenseNet
	 */
	SenseNet* getSenseNet();

	/**
		@brief �ߖT��Robot�ւ̃|�C���^������ϐ���Push����
		@param robot �ߖT��Robot
	 */
	void pushNearest(RobotMAV* robot);
	/**
		@brief �ߖT��Robot�ւ̃|�C���^����肷��
		@param index ���Ԗڂ�Robot��?
		@return �ߖT��Robot�ւ̃|�C���^
	 */
	RobotMAV* getNearestAt(int index);
	/**
		@brief �ߖT��Robot���i�[���Ă���Vector�ւ̃|�C���^����肷��
		@return �ߖT��Robot���i�[���Ă���Vector
	 */
	std::vector<RobotMAV *>* getNearest();
	/**
		@brief �ߖT��Robot���i�[���Ă���Vector����ɂ���
	 */
	void clearNearest();
	/**
		@brief �ߖT�̃��{�b�g������肷��
	 */
	int getNumOfNearestRobots() const;
	///ContExplore����Access���邽�߁Cpublic�ɂ��Ă���
	/**
		@brief �n�`�}�b�v.�e���{�b�g���Ǝ��ɏ��L����.
	 */
	int geoMap[FIELD_SIZE][FIELD_SIZE];
	/**
		@brief ���ː��ʃ}�b�v.�e���{�b�g���Ǝ��ɏ��L����.
	 */
	float radMap[FIELD_SIZE][FIELD_SIZE];
	/**
		@brief �Ӗ�Map�D�e���{�b�g���Ǝ��ɏ��L����.
		Battery Charger�̗L���Ȃǂ��L�^����
	 */
	int semMap[FIELD_SIZE][FIELD_SIZE];
	/**
		@brief �i�ߕ��֘A�����Ƃ�ۂɁC���p����㗬��Robot
		@sa ContLinkToHQ
	 */
	RobotMAV* relativeRoot;
protected:
	/**
		@brief Module�̓o�^�Ȃǂ��s�Ȃ��D
		Constructor����Ăяo����邱�Ƃ�z��
	 */
	void Initialize();
	/**
		@brief �e���{�b�g���ێ����Ă���n�}�������o��
	 */
	void logMaps();
	/**
		@brief geoMap���X�V����
		@sa geoMap
	 */
	void updateInnerGeoMap();
	/**
		@brief radMap���X�V����
		@sa radMap
	 */
	void updateInnerRadMap();
	/**
		@brief semMap���X�V����
		@sa semMap
	 */
	void updateInnerSemMap();
	///Member Variables
	/**
		@brief �ߖT��Robot�ւ̃|�C���^�z��
	 */
	std::vector<RobotMAV*>* nearest;

	/**
		@brief Robot��Color
		�ǂ̃��W���[�����������Ă��邩�ɂ���āC�ω�����
	 */
	float color[3];
	/**
		@brief Module���Ƃ�Color(R,G,B)
		modColor[module][r/g/b]
		@sa NUM_OF_CONTROLLERS
	 */
	float modColor[NUM_OF_LAYERS][3];
	/**
		@brief �ߖT��Robot�̐�
	 */
	int numOfNearestRobots;
private:
	SenseBattery* sB;
	SensePos* sP;
	SenseDirection* sD;
	SenseRange* sR;
	SenseRadiation* sRad;
	SenseVision* sBc;
	SenseNet* sN;

	ContAvoid* cAv;
	ContAlive* cAl;
	ContWander* cW;
	ContSmartAlive* cSa;
	ContConnect* cC;
	ContSpread* cSp;
	ContLinkToHQ* cL2HQ;
	ContExplore* cE;
	ContArbitrateDestination* cAd;
	
	ActPos* aP;

	std::ofstream geoLog;
	std::ofstream radLog;
	std::ofstream semLog;
#ifdef CONSIDER_DELAY
	int count;
#endif //CONSIDER_DELAY
};

#endif	//_Robot_MAV_H_