#ifndef _Sense_Battery_HPP_
#define _Sense_Battery_HPP_

#include "SAModule.h"

/**
	@class SenseBattery
	@brief バッテリー残量を検知し，出力するセンサ
	<h1>Output</h1>
	<ul><li>Output0: battery : バッテリーレベル</li></ul>
	<h1>FBoard</h1>
	<ul><li>fBoard0: fBattery : バッテリーレベル</li></ul>
	@author Kenichi Yorozu
	@date 29th November 2011
 */
class SenseBattery : public SAModule{
public:
	/**
		@brief Default Constructor
		Outputなどを追加していく.
		Sensorなので，Inputはない．
	 */
	SenseBattery();
	/**
		@brief 実行時の動作
		@sa SAModule::Run()
	 */
	virtual void Run();
};

inline SenseBattery::SenseBattery(){
	this->addFBoard("fBattery");
	this->addOutput("battery");
}

inline void SenseBattery::Run(){
	float batteryLevel = this->getFBoard(0);
#ifdef _DEBUG
	//std::cout << "batteryLevel: " << batteryLevel << std::endl;
#endif //_DEBUG
	this->setOutput(0, batteryLevel);
}

#endif	//_Sense_Battery_HPP_