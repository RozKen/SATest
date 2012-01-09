#ifndef _Robot_H_
#define _Robot_H_

#include <vector>
#include "SAModule.h"
#include "Blackboard.h"
#include "Arbiter.h"

/**
	@class Robot
	@brief ロボット一体の動きを制御するモジュール.
	以前のSAServerに近い役割をするが，Blackboard Systemを実装してある

	inputs: sensorへの入力
	outputs: actuatorによる出力
	@sa SAModule
 */
class Robot : public SAModule{
public:
	/**
		@brief Default Constructor
	 */
	Robot();
	/**
		@brief Constructor with Log File Path
		@param directoryPath	ログを保存するディレクトリへのパス
		@param fileName			ログファイル名
	 */
	Robot(std::string directoryPath, std::string fileName);

	/**
		@brief Destructor. delete vectors
	 */
	virtual ~Robot();
	virtual void Run();
	/**
		@brief モジュールを登録する
		@sa SAModule
	 */
	void addModule(SAModule *module);
	/**
		@brief コネクターを登録する
		@sa Arbiter
	 */
	void addArbiter(Arbiter *arbiter);
	/**
		@brief	ログを保存する場所を設定する
		@param directoryPath	ログを保存するディレクトリへのパス
		@param fileName			ログファイル名
	 */
	void setLogDirectoryPath(std::string directoryPath, std::string fileName);
	std::string getLogDirectoryPath() const;
	std::string getLogFileName() const;
	std::string getLogFilePath() const;
	/**
		@brief get number of Modules
		@return number of Registered Modules
	 */
	int getNumOfModules() const;
	/**
		@brief get number of Arbiters
		@return number of Registered Arbiters
	 */
	int getNumOfArbiters() const;
	/**
		@brief get Module at index
		@param index 登録された順番
		@return pointer to the Module
	 */
	SAModule*	getModule(int index);
	/**
		@brief get Arbiter at index
		@param index 登録された順番
		@return pointer to the Arbiter
	 */
	Arbiter*	getArbiter(int index);
protected:
	/**
		@brief ロボット内のModuleをすべて実行する
	 */
	virtual void RunModules();
	/**
		@brief すべてのArbiterを実行する
	 */
	virtual void ProcessArbiters();
	/**
		@brief Inner Wireの入力に関わる部分を処理
		Robotへの入力(inputs)を，RobotのinnerMemoryのiBoard/fBoardへコピーする
	 */
	virtual void ProcessInputs();
	/**
		@brief Inner Wireの出力に関わる部分を処理
		Robotからの出力(outputs)を，RobotのinnerMemoryのiBoard/fBoardからコピーする
	 */
	virtual void ProcessOutputs();
	/**
		@brief Initialize States
		環境の構築,ログの初期化
		@sa Blackboard
		@sa Log();
	*/
	virtual void Initialize();
	/**
		@brief 管理されているModule
		Moduleの登録された順に格納されている.
	*/
	std::vector<SAModule*> *modules;

	/**
		@brief 登録されているArbiter
		Modifierの登録された順に格納されている.
	 */
	std::vector<Arbiter*> *arbiters;

	/*
		iBoard/fBoardとRobotのinput/outputを結ぶ
		Inner Wireの定義
	 */
	/**
		@brief
		Source Type
		<ul>
			<li>0: input</li>
			<li>1: output</li>
			<li>2: iBoard</li>
			<li>3: fBoard</li>
		</ul>
	 */
	std::vector<int> *innerWireSrcType;
	std::vector<int> *innerWireSrcIndex;
	std::vector<int> *innerWireDestType;
	std::vector<int> *innerWireDestIndex;
	int numOfModules;
	int numOfArbiters;
};

#endif _Robot_H_