#include "ContAlive.hpp"
#include "RobotMAV.h"

void ContAlive::Run(){
	float batLevel = this->getInput(0);
	float signalX = NO_SIGNAL;
#ifdef	IMPORTANCE_BASED
	this->importance = NO_SIGNAL;
#endif	//IMPORTANCE_BASED
	if(batLevel != NO_SIGNAL){
		//充電中だったら，
		if(this->getIBoard(0) == 1 && this->getInput(RANGE + 1) == ONCHARGER){
			//充電完了の場合
			if(batLevel >= MAX_BAT){
				this->setIBoard(0, 0);
				signalX = NO_SIGNAL;
#ifdef	IMPORTANCE_BASED
				this->importance = NO_SIGNAL;
#endif	//IMPORTANCE_BASED
			}else{	//充電が完了していない場合：停止して充電する
				signalX = 0.0f;
#ifdef	IMPORTANCE_BASED
				this->importance = 1.0f;
#endif	//IMPORTANCE_BASED
			}
		}else{	//充電中じゃない場合
			this->setIBoard(0, 0);
			//電池が足りない場合
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
				
				//近くに充電器が無かった場合
				if(index == -1){
					signalX = 0.0f;
#ifdef	IMPORTANCE_BASED
					this->importance = 1.0f;
#endif	//IMPORTANCE_BASED
				}else{	//充電器が見つかったら
					if(index == RANGE){	//いま，充電器の上にいる場合
						signalX = 0.0f;
#ifdef	IMPORTANCE_BASED
						//Battery残量が少ないほど，重要度が増すようにする
						this->importance = this->calcImportance(1.0f - batLevel / ((float)MAX_BAT));
#endif	//IMPORTANCE_BASED
						//充電中にする
						this->setIBoard(0, 1);
					}else{				//充電器まで移動する必要がある場合
						signalX = (index - RANGE) / distance;
#ifdef	IMPORTANCE_BASED
						this->importance = 1.0f;
#endif	//IMPORTANCE_BASED
					}
				}

			}else{	//電池が足りている場合
				signalX = NO_SIGNAL;
#ifdef	IMPORTANCE_BASED
				this->importance = NO_SIGNAL;
#endif	//IMPORTANCE_BASED
			}
		}
	}

	//出力
	this->setOutput(0, signalX);
}