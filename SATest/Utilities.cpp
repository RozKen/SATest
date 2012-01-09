#include "Utilities.h"
#include "Constants.h"
#include <iostream>
#include <sstream>

std::string logPathGenerator(){
	time_t now;
	///���ݎ������擾
	time(&now);
	///LogDirectoryPath��ݒ�
	std::string testLogDirectoryPath = "../../../../analysis/";
	testLogDirectoryPath.append(ctime(&now));
	testLogDirectoryPath.erase(testLogDirectoryPath.size() - 12, 1);
	testLogDirectoryPath.erase(testLogDirectoryPath.size() - 9, 1);
	testLogDirectoryPath.erase(testLogDirectoryPath.size() - 1, 1);
	std::string logPath = testLogDirectoryPath;
	return logPath;
}

int fieldGenerator(int* field){
	Random<boost::uniform_int<> > _numBatGen(15, 25);
	Random<boost::uniform_int<> > _batPos(1, LENGTH);

	int numberOfBatteries = _numBatGen();
	for(int i = 0; i < LENGTH; i++){
		field[i] = NORMAL;
		/**
		if(i % 10 == 3){
			field[i] = ONCHARGER;
		}else{
			field[i] = NORMAL;
		}
		*/
	}
	field[0] = ONSTART;
	field[LENGTH - 1] = ONGOAL;
	for(int i = 0; i < numberOfBatteries; ){
		int index = _batPos();
		if(field[index] != ONCHARGER && field[index] != ONSTART
			&& field[index] != ONGOAL){
			field[index] = ONCHARGER;
			i++;
		}
	}
	/*
	for(int i = 0; i < LENGTH; i++){
		std::cout << field[i] << "\t";
	}
	std::cout << std::endl;
	*/

	return numberOfBatteries;
}

/*
	int* field = new int[LENGTH];
	int numberOfFields = 1;
	for(int i = 0; i < numberOfFields; i++){
		//Field�̍쐬
		fieldGenerator(field);
		
		//�����Field�̍쐬
		
		//int field2[] = {1,0,0,0,0,0,3,0,0,0,0,0,0,0,0,
		//			0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,
		//			3,0,0,0,0,0,0,0,0,3,0,0,0,3,3,
		//			0,0,0,3,0,0,0,0,3,0,3,0,3,0,0,
		//			3,3,0,3,0,3,0,0,0,0,0,0,3,0,0,
		//			0,0,0,0,0,3,0,3,0,0,3,3,0,0,0,
		//			3,0,0,0,0,0,0,3,0,2};
		//field = field2;
		

		//FieldTester�̍쐬
		FieldTester ft = FieldTester((const int *)field);
		//FieldTest�̎��s
		ft.Test();
	}
	delete(field);
*/

std::string intToString5(int num){
	std::stringstream ss;
	int power = 100000;
	for(int i = 0; i < 5; i++){
		power /= 10;
		if(num / power == 0){
			ss << "0";
		}
	}
	ss << num;
	return ss.str();
}

std::wstring s2ws(const std::string& s){
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	//std::cout << "Length: " << len << std::endl;
	wchar_t * buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring result(buf);
	delete[] buf;
	return result;
}