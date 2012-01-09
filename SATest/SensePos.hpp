#ifndef _Sense_Pos_HPP_
#define _Sense_Pos_HPP_

#include "SAModule.h"

/**
	@class SensePos
	@brief 現在位置を検知し，出力する(GPS)センサ
	<h1>Outputs</h1>
	<ul>
		<li>Output0 : posX - X Position</li>
		<li>Output1 : posY - Y Position</li>
	</ul>
	<h1>FBoard</h1>
	<ul>
		<li>fBoard0 : fPosX - X Position</li>
		<li>fBoard1 : fPosY - Y Position</li>
	</ul>
	@author Kenichi Yorozu
	@date 29th November 2011
 */
class SensePos : public SAModule{
public:
	/**
		@brief Default Constructor
		Outputなどを追加していく.
		Sensorなので，Inputはない．
	 */
	SensePos();
	/**
		@brief 実行時の動作
		@sa SAModule::Run()
	 */
	virtual void Run();
};

inline SensePos::SensePos(){
	this->addFBoard("fPosX");
	this->addOutput("posX");
}

inline void SensePos::Run(){
	//Sensor情報を入手
	float posX = this->getFBoard(0);
	//Sensor情報を出力
	this->setOutput(0, posX);
}

#endif	//_Sense_Pos_HPP_