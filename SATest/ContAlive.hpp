#ifndef _Cont_Alive_HPP_
#define _Cont_Alive_HPP_

#include "SAModule.h"
#include "Constants.h"
#include <math.h>

/**
	@class ContAlive
	@brief Mission : バッテリー残量を一定に維持しようとする制御器
	<h1>Inputs</h1>
	<ul>
		<li>Input0: batLevelCAl: current Battery Level</li>
		<li>Input1 - 11: visionCAl[i]: 近傍の情報</li>
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
		InputやOutputなどを追加していく.
	 */
	ContAlive();
	/**
		@brief 実行時の動作.
		batteryLevelが低かったら，近傍のバッテリーチャージャーへ戻るよう信号を出す.
		@sa SAModule::Run()
	 */
	virtual void Run();
protected:
	/**
		@brief Batteryが足りないと判断するための閾値
		現在は0.2
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