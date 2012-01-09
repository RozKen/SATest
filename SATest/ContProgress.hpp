#ifndef _Cont_Progress_HPP_
#define _Cont_Progress_HPP_

#include <math.h>
#include "Random.hpp"
#include "SAModule.h"
#include "Constants.h"

/**
	@class ContProgress
	@brief Mission : うろつき行動をしようとする制御器
	<h1>Inputs</h1>
	<ul>
		<li>Input0: posXCP : current Position - X</li>
		<li>Input1 - 11 : visionCP[i]</li>
	</ul>
	<h1>Outputs</h1>
	<ul>
		<li>Output0: dXCP: delta X - differential of Motion X</li>
	</ul>
	<h1>IBoard</h1>
	<ul>
		<li>IBoard0: steps: Current Time Steps</li>
	</ul>

	<h1>Inner FBoard : None-Logged Values</h1>
	<ul>
		<li>innerFBoard0: fPosX0 : current Position[t]</li>
		<li>innerFBoard1: fPosX1 : Position[t-1]</li>
		<li>innerFBoard2: fPosX2 : Position[t-2]</li>
		<li>innerFBoard3: fPosX3 : Position[t-3]</li>
		<li>innerFBoard4: fPosX4 : Position[t-4]</li>
	</ul>
	@author Kenichi Yorozu
	@date 9th January 2012
 */

class ContProgress : public SAModule{
public:
	/**
		@brief Default Constructor
		Input, Outputなどを追加していく.
	 */
	ContProgress();
	/**
		@brief 実行時の動作.
		<ul>
			<li>あんまり進んでいないとき，進む</li>
			<li>その他の時，NO_SIGNAL</li>
		</ul>
		@sa SAModule::Run()
	 */
	virtual void Run();
protected:
	int findGoal();
	///Random Generator
	//Random<boost::uniform_real<> > _rand;
};

inline ContProgress::ContProgress(): _rand(0, 1){
	this->addInput("posXCP");
	for(int i = 0; i < RANGE * 2 + 1; i++){
		std::string name = "visionCP[";
		name.append(this->intToString(i));
		name.append("]");
		std::string iName = "i";
		iName.append(name);
		this->addOutput(name);
		this->addIBoard(iName);
	}

	this->addOutput("dXCP");

	std::string temp;
	for(int i = 0; i < 5; i++){
		temp = "fPosX";
		temp.append(this->intToString(i/2));
		this->innerMemory->addFloatPort(temp);
	}
	this->addIBoard("steps");
}

#endif	//_Cont_Progress_HPP_