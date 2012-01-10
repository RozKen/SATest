#ifndef _World_H_
#define _World_H_

#include "RobotMAV.h"

/**
	@class World
	@brief 世界をSimulateする．
	世界に存在するRobotを駆動させ，
	駆動に応じた反応としてセンサーへデータを送る
	@author Kenichi Yorozu
	@date 9th January 2011
	@sa Robot
	@sa SAModule
 */

class World : public Robot{
public:
	/**
		@brief Default Constructor.
		logFilePathを指定した後，Initalizeする必要がある.
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
		@brief 世界を1clock進める
		<ol>
			<li>黒板にSensor値をセット</li>
			<li>各ロボットを実行</li>
			<li>各ロボットの黒板からSensor値を入手</li>
			<li>新たなSensor値を記録</li>
		</ol>
	 */
	virtual void Run();
	/**
		@brief RobotModuleを逐次実行する
	 */
	virtual void RunRobots();
	/**
		@brief WorldにRobotを追加する
	 */
	virtual void addRobot(RobotMAV* robot);
	//void moveRobot(int deltaX, int deltaY);
	//std::vector<int>* getRobotMap(int x, int y);
	/**
		@brief 放射線量Mapに値を記入する
		@param x Robotのx座標
		@param y Robotのy座標
		@param data Robotの放射線量に関するSensor Data
	 */
	//void setRadiationMap(int x, int y, std::vector<float>* data);
	/**
		@brief 二酸化炭素量Mapに値を記入する
		@param x Robotのx座標
		@param y Robotのy座標
		@param data Robotの二酸化炭素量に関するSensor Data
	 */
	//void setCOOMap(int x, int y, std::vector<float>* data);
	//std::vector<float>* getRadiationMap(int x, int y);
	//std::vector<float>* getCOOMap(int x, int y);

	/**
		@brief Initialize Fields
	 */
	void Initialize();
	/**
		@brief Robotへのポインタを返す
		@param index Robotのindex
		@return Robotへのポインタ
	 */
	RobotMAV* getRobot(const int index);

	/**
		@brief Fieldの地形情報を保持．
		geoField[x]
		<ul>
			<li>スタート地点: ONSTART</li>
			<li>ゴール地点: ONGOAL</li>
			<li>充電地点: ONCHARGER</li>
		</ul>
		※OpenGLから二次元配列にアクセスするためにPublicにしてある
	 */
	int geoField[LENGTH][NUM_ROBOTS];/**
protected:
	/**
		@brief 新しくなった世界の状況をRobotに伝達する.
	 */
	void Update();
	/**
		@brief indexの該当するロボットが，BatteryをCharge中かどうかを判別する
		@param robot robotへのpointer
		@return Batteryの上にいるかどうか？
	 */
	bool onCharger(const RobotMAV* robot);
	/**
		@brief indexの該当するロボットのBatteryが切れていないかどうか，判別する
		@param robot robotへのpointer
		@return Batteryが切れていないか？
	 */
	bool isAlive(RobotMAV* robot);

	/**
		@brief 各ロボットのVisionセンサへの入力などを更新
		@param robot robotへのpointer
		@sa SenseVision
	 */
	void updateVision(RobotMAV* robot);
	/**
		@brief geoFieldをRandomに生成する．
		@sa geoField
	 */
	void generateGeoField();
	/**
		@brief ファイルから読み込んで，geoFieldを生成する
		@param filepath geoFieldのデータが格納されているファイルへのPath
		@sa geoField
	 */
	void generateGeoField(std::string filepath);
};

inline void World::addRobot(RobotMAV* robot){
	//Robotで定義されているaddModuleを実行
	this->addModule(robot);
	robot->setBattery(MAX_BAT);
}

inline RobotMAV* World::getRobot(const int index){
	return ((RobotMAV*)this->modules->at(index));
}
#endif	//_World_H_