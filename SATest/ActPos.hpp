#ifndef _Act_Pos_HPP_
#define _Act_Pos_HPP_

#include "SAModule.h"
#include <iostream>
/**
	@class ActPos
	@brief Robotの位置を移動させるアクチュエータ．
	出力は現在からの相対距離
	<h1>Inputs</h1>
	<ul>
		<li>Input0: controlX - X方向移動量の指示</li>
	</ul>
	<h1>Outputs</h1>
	No Outputs
	<h1>fBoard</h1>
	<ul>
		<li>fBoard0: fDeltaX : X方向移動量</li>
	</ul>
	@author Kenichi Yorozu
	@date 25th November 2011
 */
class ActPos : public SAModule{
public:
	/**
		@brief Default Constructor
		Inputなどを追加していく.
		Actuatorなので，Outputはない．
	 */
	ActPos();
	/**
		@brief 実行時の動作
		@sa SAModule::Run()
	 */
	virtual void Run();
};

inline ActPos::ActPos(){
	this->addInput("controlX");
	this->addFBoard("fDeltaX");
}

inline void ActPos::Run(){
	//信号を入手
	float dX = this->getInput(0);
#ifdef _DEBUG
	//std::cout << "getInput0: " << this->getInput(0) << ": dX : " << dX << std::endl;
#endif //_DEBUG
	//Robotの内部に出力信号を格納
	this->setFBoard(0, dX);
}

#endif	//_Act_Pos_HPP_