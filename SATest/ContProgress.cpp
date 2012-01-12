#include "ContProgress.hpp"
#include "RobotMAV.h"

void ContProgress::Run(){
	//�������ړ�
	for(int i = 4; i > 1; i--){
		this->innerMemory->setFBoard(i, this->innerMemory->getFBoard(i - 1));
	}
	//�ŐV��Ԃ��X�V
	this->innerMemory->setFBoard(0, this->getInput(0));

	int step = this->getIBoard(0);
	if(step == NO_SIGNAL){
		this->setIBoard(0, 0);
	}else{
		this->setIBoard(0, step + 1);
	}

	//�����l��NO_SIGNAL
	float signalX = NO_SIGNAL;
#ifdef	IMPORTANCE_BASED
	this->importance = NO_SIGNAL;
#endif	//IMPORTANCE_BASED
	
	//���ݒn
	float pos = this->getInput(0);
	//�i�x
	float progress = pos * 2 / (float)step;

	if(progress < PROGRESS_LOW){
		signalX = (float)MAX_DRIVE;
#ifdef	IMPORTANCE_BASED
		//�i�����x���قǁC�d�v�Ƃ���
		this->importance = 1.0f * this->calcImportance(1.0f - progress / PROGRESS_LOW);
#endif	//IMPORTANCE_BASED
	}else{
		signalX = NO_SIGNAL;
#ifdef	IMPORTANCE_BASED
	this->importance = NO_SIGNAL;
#endif	//IMPORTANCE_BASED
	}

	int goal = findGoal();
	if(goal != -1){
		if(goal != 0){
			signalX = (float)MAX_DRIVE;
#ifdef	IMPORTANCE_BASED
			this->importance = 50.0f;
#endif	//IMPORTANCE_BASED
		}
	}

	//�o��
	this->setOutput(0, signalX);
}

int ContProgress::findGoal(){
	int result = -1;
	for(int i = 0; i < RANGE * 2 + 1; i++){
		if(this->getInput(i + 1) == ONGOAL){
			return (i - RANGE);
		}
	}
	return -1;
}