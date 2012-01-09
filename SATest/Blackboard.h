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
		@param logFilePath ���O�t�@�C���ւ�path
	 */
	Blackboard(std::string logFilePath);
	/**
		@brief Constructor
		@param directoryPath ���O�t�@�C����ۑ�����f�B���N�g���ւ�Path
		@param fileName ���O�t�@�C����
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
		@brief ����Port��ǉ�
		@param title Port�̖��O
		@sa inputs
	 */
	int addInputPort(std::string title);
	/**
		@brief �o��Port��ǉ�
		@param title Port�̖��O
		@sa outputs
	 */
	int addOutputPort(std::string title);
	/**
		@brief int�l��ێ�����iBoard��Port��ǉ�
		@param title Port�̖��O
		@sa iBoard
	 */
	int addIntPort(std::string title);
	/**
		@brief float�l��ێ�����fBoard��Port��ǉ�
		@param title Port�̖��O
		@sa fBoard
	 */
	int addFloatPort(std::string title);
	////////////////Setters and Getters///////////////////
	/////////On LogFile/////////
	/**
		@brief ����Blackboard�ŗ��p���郍�O�t�@�C���ւ�Path��ݒ�
		@param logFilePath ���O�t�@�C���ւ�path
		@sa logger
	 */
	void setLogFilePath(std::string logFilePath);
	/**
		@brief ����Blackboard�ŗ��p���郍�O�t�@�C���ւ�Path��ݒ�
		@param fileDirectoryPath ���O�t�@�C����ۑ�����f�B���N�g���ւ�Path
		@param fileName ���O�t�@�C����
		@sa logger
	 */
	void setLogFilePath(std::string fileDirectoryPath, std::string fileName);
	
	std::string getLogFilePath() const;

	std::string getLogFileName() const;
	std::string getLogDirectoryPath() const;
	/////////On Data/////////
	/**
		@brief Blackboard�̓��͐M������肷��
		@param index ���肵�����M����Port�ԍ�
		@param �M���̒l
	 */
	float getInput(int index) const;
	/**
		@brief Blackboard�̏o�͐M������肷��
		@param index ���肵�����M����Port�ԍ�
		@return �M���̒l
	 */
	float getOutput(int index) const;
	/**
		@brief Blackboard�̓��͐M����ݒ肷��
		@param index �ݒ肵�����M����Port�ԍ�
		@param signal �ݒ肵�����M���̒l
	 */
	void setInput(int index, float signal);
	/**
		@brief Blackboard�̏o�͐M����ݒ肷��
		@param index �ݒ肵�����M����Port�ԍ�
		@param signal �ݒ肵�����M���̒l
	 */
	void setOutput(int index, float signal);
	/**
		@brief intBoard�̒l�𓾂�
		@param index �������M���̃|�[�g�ԍ�
		@return �M���̒l
	 */
	int getIBoard(int index) const;
	/**
		@brief floatBoard�̒l�𓾂�
		@param index �������M���̃|�[�g�ԍ�
		@return �M���̒l
	 */
	float getFBoard(int index) const;
	/**
		@brief intBoard�̒l��ݒ肷��
		@param index �ݒ肵�����M���̃|�[�g�ԍ�
		@param signal �M���̒l
	 */
	void setIBoard(int index, int signal);
	/**
		@brief floatBoard�̒l��ݒ肷��
		@param index �ݒ肵�����M���̃|�[�g�ԍ�
		@param signal �M���̒l
	 */
	void setFBoard(int index, float signal);
	/////////On Numbers of Inputs and Outputs /////////
	/**
		@brief ���̓|�[�g������肷��
		@return ���̓|�[�g�̐�
	 */
	int getNumOfInputs() const;
	/**
		@brief �o�̓|�[�g������肷��
		@return �o�̓|�[�g�̐�
	 */
	int getNumOfOutputs() const;
	/**
		@brief iBoard�|�[�g������肷��
		@return iBoard�|�[�g�̐�
	 */
	int getNumOfIBoards() const;
	/**
		@brief fBoard�|�[�g������肷��
		@return fBoard�|�[�g�̐�
	 */
	int getNumOfFBoards() const;

protected:
	///���͐M���pBuffer
	std::vector<float>	*inputs;
	///�o�͐M���pBuffer
	std::vector<float>	*outputs;
	///int�l��ێ�����ėp������
	std::vector<int>	*iBoard;
	///float�l��ێ�����ėp������
	std::vector<float>	*fBoard;

	///���͐M���|�[�g��
	int numOfInputPorts;
	///�o�͐M���|�[�g��
	int numOfOutputPorts;

	/**
		@brief �L�^���t�@�C���ɕۑ����郂�W���[��
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