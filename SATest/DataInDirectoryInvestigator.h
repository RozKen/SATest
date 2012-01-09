#ifndef _DataInDirectoryInvestigator_H_
#define _DataInDirectoryInvestigator_H_

#include <vector>

/**
	@class DataInDirectoryInvestigator
	@brief �f�B���N�g�����̃t�@�C�����������C�Y���t�@�C������f�[�^�𒊏o���C�܂Ƃ߂�
	@date 27th October 2011
	@author Kenichi Yorozu
 */
class DataInDirectoryInvestigator{
public:
	/**
		@brief Constructor
		@param dirPath ��������t�H���_�ւ̃p�X ('\'��'\\'�ƃG�X�P�[�v����K�v�L��C�Ō��'\\')
	 */
	DataInDirectoryInvestigator(std::string dirPath);
	/**
		@brief fileType��Match�����t�@�C����T������
		@param fileType "*.csv"�̂悤�ȁC�t�B���^������
		@return �f�B���N�g�����̃t�@�C���̖��O�Q
	 */
	std::vector<std::string>* SearchFiles(std::string fileType);
	/**
		@brief index�Ԗڂ̍s(column = false)/��(column = true)�̃f�[�^���擾
		@param column �f�[�^���������D�c�̎�true. �s(column = false)/��(column = true)
		@param index �f�[�^���Ƃ�s���̔ԍ�. 0����n�܂�
		@return �f�[�^��2�����z��
	 */
	std::vector<std::vector<std::string>* >* AcquireData(bool column, int index);
	/**
		@brief �f�[�^���܂Ƃ߂��t�@�C�����쐬����D����́C�eposition�f�[�^���W�߂āC�X�ɂ����̕��ς�擪�̗�(column)�ɑ}������
		@param outputPath �o�̓t�@�C���̖��O
		@return ���σf�[�^
	 */
	std::vector<int>* CreateInvestigateFile(std::string outputPath);

	/**
		@brief Execute DataInDirectoryInvestigator in normal way
	 */
	void Run();
	/**
		@brief setter of dataLength
	 */
	void setDataLength(int length);
protected:
	/**
		@brief std::string��std::wstring�֕ϊ�
	 */
	std::wstring s2ws(const std::string& s);
	/**
		@brief std::wstring��std::string�֕ϊ�
	 */
	std::string ws2s(const std::wstring& ws);
	///�T������f�B���N�g���ւ�Path
	std::string dirPath;
	///�T������t�@�C���̎��
	std::string fileType;
	///�f�B���N�g�����̃t�@�C���ւ̖��O���i�[����
	std::vector<std::string>* fileNames;
	// ///�f�B���N�g�����̃t�@�C���ւ�Path���i�[����
	//std::vector<std::string>* filePaths;
	///�f�[�^��
	std::vector<std::vector<std::string>* >* data;
	///�f�[�^�̍s�̒���
	int dataLength;
};

#endif //_DataInDirectoryInvestigator_H_