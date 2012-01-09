#ifndef _Sense_Vision_HPP_
#define	_Sense_Vision_HPP_

#include "SAModule.h"

/**
	@class SenseVision
	@brief 自分の位置がBattery Chargerのある場所かどうかを与える
	<h1>Inputs</h1>
	No Inputs
	<h1>Outputs</h1>
	<ul>
		<li>Output0 : onBattery : 1.0 = true, 0.0 = false</li>
	</ul>
	<h1>IBoard</h1>
	<ul>
		<li>iBoard0 : iOnBattery : 1 = true, 0 = false</li>
	</ul>
	@author Kenichi Yorozu
	@date 9th January 2012
	@sa SAModule
 */
class SenseVision : public SAModule{
public:
	/**
		@brief Default Constructor
		Outputなどを追加していく．
		Sensorなので，Inputはない
	 */
	SenseVision();
	/**
		@brief 実行時の動作
		Sensing結果を出力
		@sa SAModule::Run
	 */
	virtual void Run();
};

inline SenseVision::SenseVision(){
	for(int i = 0; i < RANGE * 2 + 1; i++){
		std::string name = "Vision[";
		name.append(this->intToString(i));
		name.append("]");
		std::string iName = "i";
		iName.append(name);
		this->addOutput(name);
		this->addIBoard(iName);
	}
}

inline void SenseVision::Run(){
	int value;
	for(int i = 0; i < RANGE * 2 + 1; i++){
		value = this->getIBoard(i);
		this->setOutput(i, (float)value);
	}
}
#endif	//_Sense_Vision_HPP_