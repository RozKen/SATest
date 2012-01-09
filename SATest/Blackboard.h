#ifndef _Blackboard_H_
#define _Blackboard_H_

#include "Constants.h"
#include "Logger.h"

/**
	@class Blackboard
	@brief Base of Blackboard System. Hold Data and provide getter and setter.
	To provide semantic data to sensors, this class builds internal world
	from some data
	This System is inspired by Blackboard Application System. But this may not be
	exactly the same.

	@sa H. Penny Nii, Blackboard Application Systems
		and a Knowledge Engineering Perspective (1986 AI Magazine vol. 7)

	@date 9th November 2011
	@author Kenichi Yorozu
 */
class Blackboard {
public:
	/**
		@brief Constructor
	*/
	Blackboard();
	/**
		@brief Constructor
		@param logFilePath ログファイルへのpath
	 */
	Blackboard(std::string logFilePath);
	/**
		@brief Constructor
		@param directoryPath ログファイルを保存するディレクトリへのPath
		@param fileName ログファイル名
	 */
	Blackboard(std::string directoryPath, std::string fileName);
	/**
		@brief Destructor
		Delete pointers of std::vectors
	 */
	virtual ~Blackboard();
	/**
		@brief Update Internal World from inputs and set outputs according to the World
		////////<b>Not Used Currently</b>
	*/
	virtual void Update();

	/**
		@brief Log Blackboard Data into a File.
	 */
	void Log();
	
	/**
		@brief 入力Portを追加
		@param title Portの名前
		@sa inputs
	 */
	int addInputPort(std::string title);
	/**
		@brief 出力Portを追加
		@param title Portの名前
		@sa outputs
	 */
	int addOutputPort(std::string title);
	/**
		@brief int値を保持するiBoardにPortを追加
		@param title Portの名前
		@sa iBoard
	 */
	int addIntPort(std::string title);
	/**
		@brief float値を保持するfBoardにPortを追加
		@param title Portの名前
		@sa fBoard
	 */
	int addFloatPort(std::string title);
	////////////////Setters and Getters///////////////////
	/////////On LogFile/////////
	/**
		@brief このBlackboardで利用するログファイルへのPathを設定
		@param logFilePath ログファイルへのpath
		@sa logger
	 */
	void setLogFilePath(std::string logFilePath);
	/**
		@brief このBlackboardで利用するログファイルへのPathを設定
		@param fileDirectoryPath ログファイルを保存するディレクトリへのPath
		@param fileName ログファイル名
		@sa logger
	 */
	void setLogFilePath(std::string fileDirectoryPath, std::string fileName);
	
	std::string getLogFilePath() const;

	std::string getLogFileName() const;
	std::string getLogDirectoryPath() const;
	/////////On Data/////////
	/**
		@brief Blackboardの入力信号を入手する
		@param index 入手したい信号のPort番号
		@param 信号の値
	 */
	float getInput(int index) const;
	/**
		@brief Blackboardの出力信号を入手する
		@param index 入手したい信号のPort番号
		@return 信号の値
	 */
	float getOutput(int index) const;
	/**
		@brief Blackboardの入力信号を設定する
		@param index 設定したい信号のPort番号
		@param signal 設定したい信号の値
	 */
	void setInput(int index, float signal);
	/**
		@brief Blackboardの出力信号を設定する
		@param index 設定したい信号のPort番号
		@param signal 設定したい信号の値
	 */
	void setOutput(int index, float signal);
	/**
		@brief intBoardの値を得る
		@param index 得たい信号のポート番号
		@return 信号の値
	 */
	int getIBoard(int index) const;
	/**
		@brief floatBoardの値を得る
		@param index 得たい信号のポート番号
		@return 信号の値
	 */
	float getFBoard(int index) const;
	/**
		@brief intBoardの値を設定する
		@param index 設定したい信号のポート番号
		@param signal 信号の値
	 */
	void setIBoard(int index, int signal);
	/**
		@brief floatBoardの値を設定する
		@param index 設定したい信号のポート番号
		@param signal 信号の値
	 */
	void setFBoard(int index, float signal);
	/////////On Numbers of Inputs and Outputs /////////
	/**
		@brief 入力ポート数を入手する
		@return 入力ポートの数
	 */
	int getNumOfInputs() const;
	/**
		@brief 出力ポート数を入手する
		@return 出力ポートの数
	 */
	int getNumOfOutputs() const;
	/**
		@brief iBoardポート数を入手する
		@return iBoardポートの数
	 */
	int getNumOfIBoards() const;
	/**
		@brief fBoardポート数を入手する
		@return fBoardポートの数
	 */
	int getNumOfFBoards() const;

protected:
	///入力信号用Buffer
	std::vector<float>	*inputs;
	///出力信号用Buffer
	std::vector<float>	*outputs;
	///int値を保持する汎用メモリ
	std::vector<int>	*iBoard;
	///float値を保持する汎用メモリ
	std::vector<float>	*fBoard;

	///入力信号ポート数
	int numOfInputPorts;
	///出力信号ポート数
	int numOfOutputPorts;

	/**
		@brief 記録をファイルに保存するモジュール
		@sa Logger
	 */
	Logger* logger;
};

inline Blackboard::Blackboard(): numOfInputPorts(0), numOfOutputPorts(0){
	inputs = new std::vector<float>();
	outputs = new std::vector<float>();
	iBoard = new std::vector<int>();
	fBoard = new std::vector<float>();
	logger = new Logger();
}

inline Blackboard::Blackboard(std::string logFilePath)
	:numOfInputPorts(0), numOfOutputPorts(0){
	setLogFilePath(logFilePath);
	inputs = new std::vector<float>();
	outputs = new std::vector<float>();
	iBoard = new std::vector<int>();
	fBoard = new std::vector<float>();
	logger = new Logger(logFilePath);
}

inline Blackboard::Blackboard(std::string directoryPath, std::string fileName){
	inputs = new std::vector<float>();
	outputs = new std::vector<float>();
	iBoard = new std::vector<int>();
	fBoard = new std::vector<float>();
	logger = new Logger(directoryPath, fileName);
}

inline Blackboard::~Blackboard(){
	inputs->clear();
	outputs->clear();
	iBoard->clear();
	fBoard->clear();
	delete inputs;
	delete outputs;
	delete iBoard;
	delete fBoard;
	delete logger;
}

#endif //_Blackboard_H_