#ifndef _SAModule_H_
#define _SAModule_H_

//���M�����	NO_SIGNAL��include
#include "Constants.h"
#include "Blackboard.h"
#ifdef _DEBUG
#include <iostream>	//Debug���ɕ\�����邽��
#endif	//_DEBUG

/**
	@class SAModule
	@brief Module��Interface�N���X
	Module��State Machine�CActuator, Sensor��3��ނ�z��
	��{�Cinbox�̐M�����󂯎���āCRun����ƁCoutbox�ɓ����M�����o�͂���
*/
class SAModule {
public:
	/**
		@brief Constructor. Initialize std::vector
	*/
	SAModule();
	/**
		@brief Destructor. Delete std::vector
	 */
	virtual ~SAModule();
	/**
		@brief ���W���[���𓮍삳����D
		�������z�֐��D�p�������q�N���X�ł̒�`���K�{
	 */
	virtual void Run() = 0;
	/**
		@brief ���O���Ƃ�
	 */
	virtual void Log();
	/**
		@brief ���̓|�[�g��ǉ�����
		@param title ���̓|�[�g�̖��O
	 */
	virtual void addInput(std::string title);
	/**
		@brief �o�̓|�[�g��ǉ�����
		@param title �o�̓|�[�g�̖��O
	 */
	virtual void addOutput(std::string title);
	virtual void addIBoard(std::string title);
	virtual void addFBoard(std::string title);
	/**
		@brief memory�ɂ�����|�[�g�ԍ���ǉ�����
		@param index ���̓|�[�g�ԍ�
	 */
	virtual void addInputIndex(int index);
	/**
		@brief memory�ɂ�����|�[�g�ԍ���ǉ�����
		@param index �o�̓|�[�g�ԍ�
	 */
	virtual void addOutputIndex(int index);

	virtual void addIBoardIndex(int index);
	virtual void addFBoardIndex(int index);

	///////////////Setters and Getters///////////////
	/**
		@brief �����C����Module�̐e�ƂȂ�Module���w�肷��
		��{�I�ɁCBlackboard�́C�e�̂��̂𗘗p����
		@param parent �e���W���[��
		@sa Blackboard
	 */
	virtual void setParent(SAModule* parent);
	/**
		@brief �e���W���[���𓾂�
		@return �e���W���[��
	 */
	virtual SAModule* getParent() const;
	/**
		@brief ���̃��W���[���̓��̓|�[�g���̔z���Ԃ�
		@return ���̓|�[�g���̔z��ւ̃|�C���^
	 */
	virtual std::vector<std::string>* getInputTitles() const;
	/**
		@brief ���̃��W���[���̏o�̓|�[�g���̔z���Ԃ�
		@return �o�̓|�[�g���̔z��ւ̃|�C���^
	 */
	virtual std::vector<std::string>* getOutputTitles() const;
	/**
		@brief ���̃��W���[����indexOnModule�Ԗڂ̓��̓|�[�g���C
		memory��̂ǂ̏o�̓|�[�g�ƌq�����Ă��邩�H��Ԃ�
		@return memory��̏o�̓|�[�g�ԍ�
	 */
	virtual std::vector<std::string>* getIBoardTitles() const;
	virtual std::vector<std::string>* getFBoardTitles() const;

	virtual int	getInputIndex(int indexOnModule) const;
	/**
		@brief ���̃��W���[����indexOnModule�Ԗڂ̏o�̓|�[�g���C
		memory��̂ǂ̓��̓|�[�g�ƌq�����Ă��邩�H��Ԃ�
		@param indexOnModule ���W���[���̏o�̓|�[�g�ԍ�
		@return memory��̓��̓|�[�g�ԍ�
	 */
	virtual int getOutputIndex(int indexOnModule) const;
	virtual int getIBoardIndex(int indexOnModule) const;
	virtual int getFBoardIndex(int indexOnModule) const;
	/**
		@brief ���͐M����n���D
		@param index ����z��v�f�̃��W���[����̓��̓|�[�g�ԍ�
		@return input ���͐M���̔z��v�f�̒l
	 */
	virtual float getInput(int index) const;
	virtual void setInput(int index, float signal);
	virtual float getOutput(int index) const; 
	/**
		@brief �o�͐M����ݒ肷��
		@param index �ݒ肷��z��v�f�̃��W���[����̏o�̓|�[�g�ԍ�
		@param signal �o�͐M���̔z��v�f�֑������l
	 */
	virtual void setOutput(int index, float signal);
	
	virtual int getIBoard(int index) const;
	virtual float getFBoard(int index) const;
	virtual void setIBoard(int index, int signal);
	virtual void setFBoard(int index, float signal);

#ifdef	IMPORTANCE_BASED
	/**
		@brief �M���̏d�v�x��^����
		@return �M���̏d�v�x
		@sa importance
	 */
	virtual float getImportance() const;
	/**
		@brief �M���̏d�v�x��ݒ肷��
		@param importance �M���̏d�v�x
		@sa importance
	 */
	virtual void setImportance(float importance);
	/**
		@brief ���p�̂悤�ȓ������������d�v�x���Z�o����
		�d�v�x����`�ł͂Ȃ��C���ɓʂƂ��邽�߂ɕK�v�D(���p�͏�ɓ�)
		���݂�y = x^4�̋Ȑ� 4 = IMPORTANCE_POWER
		@param value �d�v�x�Ɋ֌W����l��[0.0, 1.0]�F�傫���قǏd�v
		@return �d�v�x
		@sa importance
	 */
	virtual float calcImportance(float value);

#endif	//IMPORTANCE_BASED

	/**
		@brief ���͐M���|�[�g����Ԃ�
		@return ���͐M���|�[�g��
	 */
	int getNumOfInputPorts() const;
	/**
		@brief �o�͐M���|�[�g����Ԃ�
		@return �o�͐M���|�[�g��
	 */
	int getNumOfOutputPorts() const;
	/**
		@brief �e�̎��L���̈�ւ̃|�C���^��Ԃ�
		@return �e�̎��L���̈�ւ̃|�C���^
		@sa Blackbaord
	 */
	Blackboard* getMemory() const;
	/**
		@brief �����Ɏ��L���̈�ւ̃|�C���^��Ԃ�
		@return �����Ɏ��L���̈�
		@sa Blackboard
	 */
	Blackboard* getInnerMemory() const;

	/**
		@brief int�^�̕ϐ���string�^�ɕϊ����ĕς����֐�
		@param num �ϊ����������l
		@return string�^�ɂȂ������l
	 */
	std::string intToString(int num);
	/**
		@brief float�^�̒l���l�̌ܓ�����int�^�ɂ���
		@param value �l�̌ܓ��������l
		@return �l�̌ܓ����ꂽ�l
	 */
	int round(float value);
	/**
		@brief �������Z�o���� : SQRT(dx^2 + dy^2)
		@param dx x�̍���
		@param dy y�̍���
		@return ���� : SQRT(dx^2 + dy^2)
	 */
	float norm(const float dx, const float dy);
protected:
	///SAModule�̓��͒l���i�[����Ă���memory���Index
	std::vector<int> *inputIndex;
	///SAModule�̏o�͒l���i�[����Ă���memory���Index
	std::vector<int> *outputIndex;
	std::vector<int> *iBoardIndex;
	std::vector<int> *fBoardIndex;
	///Log�ɂ�����C���͒l��������閼�O
	std::vector<std::string> *inputTitles;
	///Log�ɂ�����C�o�͒l��������閼�O
	std::vector<std::string> *outputTitles;
	std::vector<std::string> *iBoardTitles;
	std::vector<std::string> *fBoardTitles;
	///���͐M���|�[�g��
	int numOfInputPorts;
	///�o�͐M���|�[�g��
	int numOfOutputPorts;
	///�����C�e�ɓ����郂�W���[��
	SAModule* parent;
	///�����̕ێ�����l���i�[���邽�߂́C�e�̋L���̈�
	Blackboard* memory;
	///�l�̓��o�͂ɓ�����C���p����L���̈�
	Blackboard* innerMemory;
#ifdef	IMPORTANCE_BASED
	///���̃��W���[����������M���̏d�v�x�D���p�ɉ����Ĕ������ׂ�
	float importance;
#endif	//IMPORTANCE_BASED
};

#endif //_SAModule_H_