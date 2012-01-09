#include "ContAlive.hpp"
#include "RobotMAV.h"

void ContAlive::Run(){
	float batLevel = this->getInput(0);
	float signalX = NO_SIGNAL;
#ifdef	IMPORTANCE_BASED
	this->importance = NO_SIGNAL;
#endif	//IMPORTANCE_BASED
	if(batLevel != NO_SIGNAL){
		//�[�d����������C
		if(this->getIBoard(0) == 1 && this->getInput(RANGE + 1) == ONCHARGER){
			//�[�d�����̏ꍇ
			if(batLevel >= MAX_BAT){
				this->setIBoard(0, 0);
				signalX = NO_SIGNAL;
#ifdef	IMPORTANCE_BASED
				this->importance = NO_SIGNAL;
#endif	//IMPORTANCE_BASED
			}else{	//�[�d���������Ă��Ȃ��ꍇ�F��~���ď[�d����
				signalX = 0.0f;
#ifdef	IMPORTANCE_BASED
				this->importance = 1.0f;
#endif	//IMPORTANCE_BASED
			}
		}else{	//�[�d������Ȃ��ꍇ
			this->setIBoard(0, 0);
			//�d�r������Ȃ��ꍇ
			if( batLevel / ((float)MAX_BAT) < threshold){
				int distance = 1000;
				int index = -1;
				for(int i = 0; i < RANGE * 2 + 1; i++){
					int vision = this->getInput(i + 1);
					if(vision == ONCHARGER){
						int tmpD = sqrt(pow((double)(RANGE - i), 2));
						if(tmpD < distance){
							distance = tmpD;
							index = i;
						}
					}
				}
				
				//�߂��ɏ[�d�킪���������ꍇ
				if(index == -1){
					signalX = 0.0f;
#ifdef	IMPORTANCE_BASED
					this->importance = 1.0f;
#endif	//IMPORTANCE_BASED
				}else{	//�[�d�킪����������
					if(index == RANGE){	//���܁C�[�d��̏�ɂ���ꍇ
						signalX = 0.0f;
#ifdef	IMPORTANCE_BASED
						//Battery�c�ʂ����Ȃ��قǁC�d�v�x�������悤�ɂ���
						this->importance = this->calcImportance(1.0f - batLevel / ((float)MAX_BAT));
#endif	//IMPORTANCE_BASED
						//�[�d���ɂ���
						this->setIBoard(0, 1);
					}else{				//�[�d��܂ňړ�����K�v������ꍇ
						signalX = (index - RANGE) / distance;
#ifdef	IMPORTANCE_BASED
						this->importance = 1.0f;
#endif	//IMPORTANCE_BASED
					}
				}

			}else{	//�d�r������Ă���ꍇ
				signalX = NO_SIGNAL;
#ifdef	IMPORTANCE_BASED
				this->importance = NO_SIGNAL;
#endif	//IMPORTANCE_BASED
			}
		}
	}

	//�o��
	this->setOutput(0, signalX);
}