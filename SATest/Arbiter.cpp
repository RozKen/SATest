#include "Arbiter.h"
#include <math.h>

Arbiter::Arbiter()
	:_rand(0, 1), timeToModify(0), timeLeftModified(0)
	, factor(NO_SIGNAL){
}

Arbiter::Arbiter(SAModule* src, int srcPort, SAModule* dest, int destPort)
	:_rand(0, 1), source(src), srcPort(srcPort), destination(dest), destPort(destPort),
	timeToModify(0), timeLeftModified(0), factor(NO_SIGNAL){
		addInput(src->getOutputTitles()->at(srcPort));
		addInputIndex(src->getOutputIndex(srcPort));
		addOutput(dest->getInputTitles()->at(destPort));
		addOutputIndex(dest->getInputIndex(destPort));
}

Arbiter::Arbiter(SAModule* src, int srcPort, SAModule* dest, int destPort, float factor)
	:_rand(0, 1),  source(src), srcPort(srcPort), destination(dest), destPort(destPort),
	timeToModify(0), timeLeftModified(0), factor(factor){
		addInput(src->getOutputTitles()->at(srcPort));
		addInputIndex(src->getOutputIndex(srcPort));
		addOutput(dest->getInputTitles()->at(destPort));
		addOutputIndex(dest->getInputIndex(destPort));
}

Arbiter::Arbiter(SAModule* src, int srcPort, SAModule* dest, int destPort, 
		float factor_min, float factor_max)
	: source(src), srcPort(srcPort), destination(dest), destPort(destPort), 
	_rand(factor_min, factor_max), timeToModify(0), timeLeftModified(0), factor(NO_SIGNAL){
		addInput(src->getOutputTitles()->at(srcPort));
		addInputIndex(src->getOutputIndex(srcPort));
		addOutput(dest->getInputTitles()->at(destPort));
		addOutputIndex(dest->getInputIndex(destPort));
}

Arbiter::~Arbiter(){
	delete source;
	delete destination;
}

void Arbiter::Run(){
	this->setOutput((float)generateSignal());
}

void Arbiter::setSource(SAModule* src, int srcPort){
	this->source = src;
	this->srcPort = srcPort;
	addInput(src->getOutputTitles()->at(srcPort));
	addInputIndex(src->getOutputIndex(srcPort));
}

void Arbiter::setDestination(SAModule* dest, int destPort){
	this->destination = dest;
	this->destPort = destPort;
	addOutput(dest->getInputTitles()->at(destPort));
	addOutputIndex(dest->getInputIndex(destPort));
}

float Arbiter::getInput() const{
		///Arbiterへの入力はmemoryの入力から入手※Moduleと異なる
	int result = memory->getInput(this->inputIndex->at(0));
	return result;
}
float Arbiter::getInput(int index) const{
	return getInput();
}

void Arbiter::setInput(float signal){
	memory->setInput(this->inputIndex->at(0), signal);
}
void Arbiter::setInput(int index, float signal){
	return setInput(signal);
}

float Arbiter::getOutput() const{
	int result = memory->getOutput(this->outputIndex->at(0));
	return result;
}
float Arbiter::getOutput(int index) const{
	return getOutput();
}

void Arbiter::setOutput(float signal){
	///Arbiterからの出力は，memoryの出力へ送信※Moduleと異なる
	memory->setOutput(this->outputIndex->at(0), signal);
}
void Arbiter::setOutput(int index, float signal){
	setOutput(signal);
}

float Arbiter::getDest() const{
	return memory->getOutput(this->outputIndex->at(0));
}

float Arbiter::getSrc() const{
	return memory->getInput(this->inputIndex->at(0));
}

double Arbiter::generateSignal(){
	///現Stepにおける，Arbiterの挙動を決める因子
	double currentFactor;
	///factorが指定されていなければ，
	if(factor == NO_SIGNAL){
#ifdef	IMPORTANCE_BASED
		//重要度に応じてcurrentFactorを設定する
		//currentFactorはsrc/ dstの影響率を0.0f - 1.0fの範囲で指定する
		//INVERSE SUPPRESSORの時，currentFactorが大きいほど, dstの影響が大きくなる
		//NORMAL SUPPRESSORの時, currentFactorが大きいほど，srcの影響が大きくなる

		///importanceを入手
		float impSrc = source->getImportance();
		float impDst = destination->getImportance();

		//信号がNO_SIGNALの場合，重要度もNO_SIGNALとする
		if(getSrc() == NO_SIGNAL){
			impSrc = NO_SIGNAL;
		}
		if(getDest() == NO_SIGNAL){
			impDst = NO_SIGNAL;
		}

		if((impSrc != NO_SIGNAL && impDst == NO_SIGNAL) || impSrc == VERY_IMPORTANT){
#ifdef	INVERSE_SUPPRESSOR
			currentFactor = 0.0f;	//srcが強い
#else	//INVERSE_SUPPRESSOR
			currentFactor = 1.0f;	//srcが強い
#endif	//INVERSE_SUPPRESSOR
			//destinationの重要度を設定する
			destination->setImportance(impSrc);
		}else if((impSrc == NO_SIGNAL && impDst != NO_SIGNAL) || impDst == VERY_IMPORTANT){
#ifdef	INVERSE_SUPPRESSOR
			currentFactor = 1.0f;	//dstが強い
#else	//INVERSE_SUPPRESSOR
			currentFactor = 0.0f;	//dstが強い
#endif	//INVERSE_SUPPRESSOR
			//下は何もしないのと同じ
			destination->setImportance(impDst);
		}else if(impSrc != NO_SIGNAL && impDst != NO_SIGNAL){
#ifdef	INVERSE_SUPPRESSOR
			currentFactor = impDst / (impSrc + impDst);	//大きいほど強いのはdst
#else	//INVERSE_SUPPRESSOR
			currentFactor = impSrc / (impSrc + impDst);	//大きいほど強いのはsrc
#endif	//INVERSE_SUPPRESSOR
			if(currentFactor > 1.0f){
				currentFactor = 1.0f;
			}else if(currentFactor < 0.0f){
				currentFactor = 0.0f;
			}
			//destination->setImportance(currentFactor);
			destination->setImportance((impSrc + impDst) / 2.0f);
		}else{	//impSrc == NO_SIGNAL && impDst == NO_SIGNAL
			///乱数で生成
			//currentFactor = _rand();
			currentFactor = 0.0f;	//dst, srcのどちらにしろNO_SIGNAL;
			destination->setImportance(NO_SIGNAL);
		}
#else	//IMPORTANCE_BASED
		///乱数で生成
		currentFactor = _rand();
#endif	//IMPORTANCE_BASED
	}else{
		///factorが指定されていれば，その値を利用
		currentFactor = (double)factor;
	}

	double destRatio;	///接続先(下層)の信号の影響率
	double sourceRatio;	///接続元(上層)の信号の影響率

	double magnitude;	///信号全体の強さの係数

	if(currentFactor >= 0.0 && currentFactor <= 1.0){
		//Act like Suppressor, Selecter or Superposer
		magnitude = 1.0;
		
#ifdef INVERSE_SUPPRESSOR
		if(getDest() != NO_SIGNAL){
			destRatio = 0.5 * ( cos ( 0.5 * PI * ( cos ( (double) currentFactor * PI ) + 1.0 ) ) + 1.0 );
			sourceRatio = 1.0 - destRatio;
		}else{
			//getDest() == NO_SIGNAL
			destRatio = 0.0;
			sourceRatio = 1.0;
		}
#else	// if NORMAL_SUPRESSOR
		if(getSrc() != NO_SIGNAL){
			sourceRatio = 0.5 * ( cos ( 0.5 * PI * ( cos ( (double) currentFactor * PI ) + 1.0 ) ) + 1.0 );
			destRatio = 1.0 - sourceRatio;
		}else{
			//getSrc() == NO_SIGNAL
			destRatio = 1.0;
			sourceRatio = 0.0;
		}
#endif	//INVERSE_SUPPRESSOR
	}else if(currentFactor < 0.0 && currentFactor >= -1.0){
		//Act like Inhibitor
		sourceRatio = 0.0;
		destRatio = 1.0;
		magnitude = 1.0 - 0.5 * ( cos( 0.5 * PI * ( cos( (double)(currentFactor + 1.0) * PI ) + 1.0 ) ) + 1.0 );
	}else{	//Act as Wire
		sourceRatio = 1.0;
		destRatio = 0.0;
		magnitude = 1.0;
#ifdef	IMPORTANCE_BASED
		//////////////////////////ここがおかしい///////////////////
		////無理矢理dst = ActPosの時だけ，importanceを上書きするように設定
		float imp = source->getImportance();
		if(getSrc() == NO_SIGNAL){
			imp = NO_SIGNAL;
		}
		if(imp != NO_SIGNAL){	//Srcの重要度がNO_SIGNALでないとき
			destination->setImportance(imp);	//Srcの重要度をそのまま，Dstの重要度にするよう設定
		}else{					//Srcの重要度がNO_SIGNALの時，
			if(destination->getNumOfInputPorts() == 2
				&& destination->getNumOfOutputPorts() == 0
				&& destination->getFBoardTitles()->size() == 2
				&& destination->getIBoardTitles()->size() == 0){	//つまり，destinationがActPosの時
					destination->setImportance(NO_SIGNAL);	//Dstの重要度もNO_SIGNALとする
			}else{
				//currentFactor = destination->getImportance();
				float impDst = destination->getImportance();
				if(getDest() == NO_SIGNAL){
					impDst = NO_SIGNAL;
				}
				destination->setImportance(impDst);
			}
		}
#endif	//IMPORTANCE_BASED
	}

#ifdef	INVERSE_SUPPRESSOR
	this->currentRatio = destRatio;
#else	//INVERSE_SUPPRESSOR
	this->currentRatio = sourceRatio;
#endif	//INVERSE_SUPPRESSOR

	double valDest = (double)getDest();
	double valSrc = (double)getSrc();
	double signal = (double)NO_SIGNAL;
	if(valDest != NO_SIGNAL || valSrc != NO_SIGNAL){
		signal = magnitude * (destRatio * valDest + sourceRatio * valSrc);
	}
	
#ifdef _DEBUG
	//std::cout << "=========================" << std::endl;
	//std::cout << "magnitude: " << magnitude << std::endl;
	//std::cout << "dest: " << this->outputTitles->at(0) << ":" << valDest << std::endl;
	//std::cout << "src: " << this->inputTitles->at(0) << ":" << valSrc << std::endl;
#ifdef	IMPORTANCE_BASED
	//	std::cout << "impDst: " << destination->getImportance() << std::endl;
	//	std::cout << "impSrc: " << source->getImportance() << std::endl;
#endif	//IMPORTANCE_BASED
	//std::cout << "signal: " << signal << std::endl;
	
#endif
	return signal;
}
