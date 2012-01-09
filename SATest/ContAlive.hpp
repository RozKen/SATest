#ifndef _Cont_Alive_HPP_
#define _Cont_Alive_HPP_

#include "SAModule.h"
#include "Constants.h"
#include <math.h>

/**
	@class ContAlive
	@brief Mission : �o�b�e���[�c�ʂ����Ɉێ����悤�Ƃ��鐧���
	<h1>Inputs</h1>
	<ul>
		<li>Input0: batLevelCAl: current Battery Level</li>
		<li>Input1 - 11: visionCAl[i]: �ߖT�̏��</li>
	</ul>
	<h1>Outputs</h1>
	<ul>
		<li>Output0: dXCAl: delta X - differential of Motion X</li>
	</ul>

	<h1>IBoard</h1>
	<ul>
		<li>IBoard0 : ChargingFlag - 0 = NOT CHARGING, 1 = CHARGING</li>
	</ul>
	@author Kenichi Yorozu
	@date 29th November 2011
 */
class ContAlive : public SAModule{
public:
	/**
		@brief Default Constructor
		Input��Output�Ȃǂ�ǉ����Ă���.
	 */
	ContAlive();
	/**
		@brief ���s���̓���.
		batteryLevel���Ⴉ������C�ߖT�̃o�b�e���[�`���[�W���[�֖߂�悤�M�����o��.
		@sa SAModule::Run()
	 */
	virtual void Run();
protected:
	/**
		@brief Battery������Ȃ��Ɣ��f���邽�߂�臒l
		���݂�0.2
		@sa BATTERY_LOW
	 */
	float threshold;
};

inline ContAlive::ContAlive(): threshold((float)BATTERY_LOW){
	this->addInput("batLevelCAl");
	for(int i = 0; i < RANGE * 2 + 1; i++){
		std::string name = "visionCAl[";
		name.append(this->intToString(i));
		name.append("]");
		std::string iName = "i";
		iName.append(name);
		this->addOutput(name);
		this->addIBoard(iName);
	}

	this->addIBoard("ChargingFlag");

	this->addOutput("dXCAl");
}

#endif	//_Cont_Alive_HPP_