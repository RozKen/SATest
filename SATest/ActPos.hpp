#ifndef _Act_Pos_HPP_
#define _Act_Pos_HPP_

#include "SAModule.h"
#include <iostream>
/**
	@class ActPos
	@brief Robot�̈ʒu���ړ�������A�N�`���G�[�^�D
	�o�͂͌��݂���̑��΋���
	<h1>Inputs</h1>
	<ul>
		<li>Input0: controlX - X�����ړ��ʂ̎w��</li>
	</ul>
	<h1>Outputs</h1>
	No Outputs
	<h1>fBoard</h1>
	<ul>
		<li>fBoard0: fDeltaX : X�����ړ���</li>
	</ul>
	@author Kenichi Yorozu
	@date 25th November 2011
 */
class ActPos : public SAModule{
public:
	/**
		@brief Default Constructor
		Input�Ȃǂ�ǉ����Ă���.
		Actuator�Ȃ̂ŁCOutput�͂Ȃ��D
	 */
	ActPos();
	/**
		@brief ���s���̓���
		@sa SAModule::Run()
	 */
	virtual void Run();
};

inline ActPos::ActPos(){
	this->addInput("controlX");
	this->addFBoard("fDeltaX");
}

inline void ActPos::Run(){
	//�M�������
	float dX = this->getInput(0);
#ifdef _DEBUG
	//std::cout << "getInput0: " << this->getInput(0) << ": dX : " << dX << std::endl;
#endif //_DEBUG
	//Robot�̓����ɏo�͐M�����i�[
	this->setFBoard(0, dX);
}

#endif	//_Act_Pos_HPP_