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
		<li>Vision Scope</li>
		<li>Position</li>
	</ul>
	<h1>Controllers</h1>
	<ul>
		<li>Alive</li>
		<li>Progress</li>
	</ul>
	<h1>Actuators</h1>
	<ul>
		<li>Position</li>
	</ul>
	@sa Actuators.h
	@sa Controllers.h
	@sa Sensors.h

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
		@brief get Vision Sensor Value
		@param index �ʒu�ɉ�����index
		@return Vision Sensor�l
	 */
	float getVision(int index) const;
	/**
		@brief set Vision Sensor Value
		@param index �ʒu�ɉ�����index
		@param value Vision Sensor�l
	 */
	void setVision(int index, float value);

	///get��Output����
	float getChargingFlag() const;
	///set��Input��
	void setChargingFlag(float value);

	///get��Output����
	float getSteps() const;
	///set��Input��
	void setSteps(float value);

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

	/**
		@brief ���ׂĂ�Arbiter�����s����
	 */
	virtual void ProcessArbiters();
protected:
	/**
		@brief Module�̓o�^�Ȃǂ��s�Ȃ��D
		Constructor����Ăяo����邱�Ƃ�z��
	 */
	void Initialize();
	///Member Variables

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
	SenseVision* sV;

	ContAlive* cAl;
	ContProgress* cP;
	
	ActPos* aP;

#ifdef CONSIDER_DELAY
	int count;
#endif //CONSIDER_DELAY
};

#endif	//_Robot_MAV_H_