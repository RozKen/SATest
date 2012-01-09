#ifndef _Robot_MAV_H_
#define _Robot_MAV_H_

#include "Constants.h"
#include "Sensors.h"
#include "Controllers.h"
#include "Actuators.h"
#include "Robot.h"

/**
	@brief 探索Robot
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
		Moduleを追加していく．
	 */
	RobotMAV();
	RobotMAV(std::string directoryPath, std::string fileName);

	/**
		@brief Destructor. Clear Memories.
	 */
	virtual ~RobotMAV();

	/**
		@brief 実行時の動作
		@sa SAModule::Run()
	 */
	virtual void Run();
	/**
		@brief 内部の地図情報などを更新する
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
		@param index 角度に応じたindex
		@return Range Sensor値
	 */
	float getRange(int index) const;
	/**
		@brief set Range Sensor Value
		@param index 角度に応じたindex
		@param value Range Sensor値
	 */
	void setRange(int index, float value);
	/**
		@brief get Radiation Sensor Value
		@param index Hashに応じたindex
		@return Radiation Sensor 値
	 */
	float getRad(int index) const;
	/**
		@brief set Radiation Sensor Value. Worldから利用される
		@param index Hashに応じたindex
		@param value Radiation Sensor値

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

	///////表示用///////
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
		@brief すべてのArbiterを実行する
	 */
	virtual void ProcessArbiters();

	/**
		@brief SenseNetへのポインタを返す. Worldからの呼び出しに利用する
		@return SenseNet モジュールへのポインタ
		@sa SenseNet
	 */
	SenseNet* getSenseNet();

	/**
		@brief 近傍のRobotへのポインタを内部変数にPushする
		@param robot 近傍のRobot
	 */
	void pushNearest(RobotMAV* robot);
	/**
		@brief 近傍のRobotへのポインタを入手する
		@param index 何番目のRobotか?
		@return 近傍のRobotへのポインタ
	 */
	RobotMAV* getNearestAt(int index);
	/**
		@brief 近傍のRobotを格納しているVectorへのポインタを入手する
		@return 近傍のRobotを格納しているVector
	 */
	std::vector<RobotMAV *>* getNearest();
	/**
		@brief 近傍のRobotを格納しているVectorを空にする
	 */
	void clearNearest();
	/**
		@brief 近傍のロボット数を入手する
	 */
	int getNumOfNearestRobots() const;
	///ContExploreからAccessするため，publicにしてある
	/**
		@brief 地形マップ.各ロボットが独自に所有する.
	 */
	int geoMap[FIELD_SIZE][FIELD_SIZE];
	/**
		@brief 放射線量マップ.各ロボットが独自に所有する.
	 */
	float radMap[FIELD_SIZE][FIELD_SIZE];
	/**
		@brief 意味Map．各ロボットが独自に所有する.
		Battery Chargerの有無などを記録する
	 */
	int semMap[FIELD_SIZE][FIELD_SIZE];
	/**
		@brief 司令部へ連絡をとる際に，利用する上流のRobot
		@sa ContLinkToHQ
	 */
	RobotMAV* relativeRoot;
protected:
	/**
		@brief Moduleの登録などを行なう．
		Constructorから呼び出されることを想定
	 */
	void Initialize();
	/**
		@brief 各ロボットが保持している地図を書き出す
	 */
	void logMaps();
	/**
		@brief geoMapを更新する
		@sa geoMap
	 */
	void updateInnerGeoMap();
	/**
		@brief radMapを更新する
		@sa radMap
	 */
	void updateInnerRadMap();
	/**
		@brief semMapを更新する
		@sa semMap
	 */
	void updateInnerSemMap();
	///Member Variables
	/**
		@brief 近傍のRobotへのポインタ配列
	 */
	std::vector<RobotMAV*>* nearest;

	/**
		@brief RobotのColor
		どのモジュールが発現しているかによって，変化する
	 */
	float color[3];
	/**
		@brief ModuleごとのColor(R,G,B)
		modColor[module][r/g/b]
		@sa NUM_OF_CONTROLLERS
	 */
	float modColor[NUM_OF_LAYERS][3];
	/**
		@brief 近傍のRobotの数
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