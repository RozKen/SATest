#ifndef _Sense_Pos_HPP_
#define _Sense_Pos_HPP_

#include "SAModule.h"

/**
	@class SensePos
	@brief ���݈ʒu�����m���C�o�͂���(GPS)�Z���T
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
		Output�Ȃǂ�ǉ����Ă���.
		Sensor�Ȃ̂ŁCInput�͂Ȃ��D
	 */
	SensePos();
	/**
		@brief ���s���̓���
		@sa SAModule::Run()
	 */
	virtual void Run();
};

inline SensePos::SensePos(){
	this->addFBoard("fPosX");
	this->addOutput("posX");
}

inline void SensePos::Run(){
	//Sensor�������
	float posX = this->getFBoard(0);
	//Sensor�����o��
	this->setOutput(0, posX);
}

#endif	//_Sense_Pos_HPP_