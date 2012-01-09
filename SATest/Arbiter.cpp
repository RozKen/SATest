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
		///Arbiter�ւ̓��͂�memory�̓��͂�����聦Module�ƈقȂ�
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
	///Arbiter����̏o�͂́Cmemory�̏o�֑͂��M��Module�ƈقȂ�
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
	///��Step�ɂ�����CArbiter�̋��������߂���q
	double currentFactor;
	///factor���w�肳��Ă��Ȃ���΁C
	if(factor == NO_SIGNAL){
#ifdef	IMPORTANCE_BASED
		//�d�v�x�ɉ�����currentFactor��ݒ肷��
		//currentFactor��src/ dst�̉e������0.0f - 1.0f�͈̔͂Ŏw�肷��
		//INVERSE SUPPRESSOR�̎��CcurrentFactor���傫���ق�, dst�̉e�����傫���Ȃ�
		//NORMAL SUPPRESSOR�̎�, currentFactor���傫���قǁCsrc�̉e�����傫���Ȃ�

		///importance�����
		float impSrc = source->getImportance();
		float impDst = destination->getImportance();

		//�M����NO_SIGNAL�̏ꍇ�C�d�v�x��NO_SIGNAL�Ƃ���
		if(getSrc() == NO_SIGNAL){
			impSrc = NO_SIGNAL;
		}
		if(getDest() == NO_SIGNAL){
			impDst = NO_SIGNAL;
		}

		if((impSrc != NO_SIGNAL && impDst == NO_SIGNAL) || impSrc == VERY_IMPORTANT){
#ifdef	INVERSE_SUPPRESSOR
			currentFactor = 0.0f;	//src������
#else	//INVERSE_SUPPRESSOR
			currentFactor = 1.0f;	//src������
#endif	//INVERSE_SUPPRESSOR
			//destination�̏d�v�x��ݒ肷��
			destination->setImportance(impSrc);
		}else if((impSrc == NO_SIGNAL && impDst != NO_SIGNAL) || impDst == VERY_IMPORTANT){
#ifdef	INVERSE_SUPPRESSOR
			currentFactor = 1.0f;	//dst������
#else	//INVERSE_SUPPRESSOR
			currentFactor = 0.0f;	//dst������
#endif	//INVERSE_SUPPRESSOR
			//���͉������Ȃ��̂Ɠ���
			destination->setImportance(impDst);
		}else if(impSrc != NO_SIGNAL && impDst != NO_SIGNAL){
#ifdef	INVERSE_SUPPRESSOR
			currentFactor = impDst / (impSrc + impDst);	//�傫���قǋ����̂�dst
#else	//INVERSE_SUPPRESSOR
			currentFactor = impSrc / (impSrc + impDst);	//�傫���قǋ����̂�src
#endif	//INVERSE_SUPPRESSOR
			if(currentFactor > 1.0f){
				currentFactor = 1.0f;
			}else if(currentFactor < 0.0f){
				currentFactor = 0.0f;
			}
			//destination->setImportance(currentFactor);
			destination->setImportance((impSrc + impDst) / 2.0f);
		}else{	//impSrc == NO_SIGNAL && impDst == NO_SIGNAL
			///�����Ő���
			//currentFactor = _rand();
			currentFactor = 0.0f;	//dst, src�̂ǂ���ɂ���NO_SIGNAL;
			destination->setImportance(NO_SIGNAL);
		}
#else	//IMPORTANCE_BASED
		///�����Ő���
		currentFactor = _rand();
#endif	//IMPORTANCE_BASED
	}else{
		///factor���w�肳��Ă���΁C���̒l�𗘗p
		currentFactor = (double)factor;
	}

	double destRatio;	///�ڑ���(���w)�̐M���̉e����
	double sourceRatio;	///�ڑ���(��w)�̐M���̉e����

	double magnitude;	///�M���S�̂̋����̌W��

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
		//////////////////////////��������������///////////////////
		////�����dst = ActPos�̎������Cimportance���㏑������悤�ɐݒ�
		float imp = source->getImportance();
		if(getSrc() == NO_SIGNAL){
			imp = NO_SIGNAL;
		}
		if(imp != NO_SIGNAL){	//Src�̏d�v�x��NO_SIGNAL�łȂ��Ƃ�
			destination->setImportance(imp);	//Src�̏d�v�x�����̂܂܁CDst�̏d�v�x�ɂ���悤�ݒ�
		}else{					//Src�̏d�v�x��NO_SIGNAL�̎��C
			if(destination->getNumOfInputPorts() == 2
				&& destination->getNumOfOutputPorts() == 0
				&& destination->getFBoardTitles()->size() == 2
				&& destination->getIBoardTitles()->size() == 0){	//�܂�Cdestination��ActPos�̎�
					destination->setImportance(NO_SIGNAL);	//Dst�̏d�v�x��NO_SIGNAL�Ƃ���
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
