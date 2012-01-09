#ifndef _World_H_
#define _World_H_

#include "RobotMAV.h"

/**
	@class World
	@brief 世界をSimulateする．
	世界に存在するRobotを駆動させ，
	駆動に応じた反応としてセンサーへデータを送る
	@author Kenichi Yorozu
	@date 22th November 2011
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
		@brief indexが指し示すx, y座標を得るためのhash[x/y][index]
	 */
	int hash[2][HASH_MAX];
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
		@brief hash値を得る
		@param which x = 0/ y = 1を代入
		@param index 何番目の八種値がほしいのか．
	 */
	int getHash(int which, int index) const;
	/**
		@brief Fieldの地形情報を保持．
		geoField[x][y]
		<ul>
			<li>障害物: OUTOFAREA</li>
		</ul>
		※OpenGLから二次元配列にアクセスするためにPublicにしてある
	 */
	int geoField[FIELD_SIZE][FIELD_SIZE];/**
		@brief Fieldの固定的な意味情報(semantics)を保持
		semField[x][y];
		<ul>
			<li>ONSTART : 司令部</li>
			<li>ONCHARGER : 充電器(司令部近傍)</li>
		</ul>
		@sa semField
		※OpenGLから二次元配列にアクセスするためにPublicにしてある
	 */
	int semField[FIELD_SIZE][FIELD_SIZE];
	/**
		@brief Fieldの放射線量情報を保持
		radField[x][y];
		<ul>
			<li>放射線量: [0.0, 1.0]</li>
		</ul>
		※OpenGLから二次元配列にアクセスするためにPublicにしてある
	 */
	float radField[FIELD_SIZE][FIELD_SIZE];
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
	bool onVision(const RobotMAV* robot);
	/**
		@brief indexの該当するロボットのBatteryが切れていないかどうか，判別する
		@param robot robotへのpointer
		@return Batteryが切れていないか？
	 */
	bool isAlive(const RobotMAV* robot);
	/**
		@brief 各ロボットの近接センサへの入力を更新
		近くかどうかは，RANGE_DANGER (現在は3.0f)で決まる
		@param robot robotへのpointer
		@sa RANGE_DANGER
		@sa Constants.h
	 */
	void updateRange(RobotMAV* robot);
	/**
		@brief 各ロボットの放射線センサへの入力を更新
		近くかどうかは，MAX_RANGEで決まる
		@param robot robotへのpointer
		@sa MAX_RANGE
		@sa Constants.h
	 */
	void updateRadiation(RobotMAV* robot);
	/**
		@brief 各ロボットのNetworkセンサへの入力等を更新
		各ロボットの内部変数「近傍ロボットへのポインタの配列」も更新する
		@param robot robotへのpointer
		@sa SenseNet
		@sa WIFI_REACH
		@sa Constants.h
	 */
	void updateNetWork(RobotMAV* robot);
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
	/**
		@brief semFieldを司令部の初期位置に応じて生成する.
		@sa semField
	 */
	void generateSemField();
	/**
		@brief radFieldをRandomに生成する
		@sa radField
	 */
	void generateRadField();
#ifdef	CONSIDER_DELAY
	int count;
#endif	//CONSIDER_DELAY
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