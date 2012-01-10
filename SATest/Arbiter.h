#ifndef _Arbiter_H_
#define _Arbiter_H_

#include "Random.hpp"
#include "SAModule.h"

/**
	@class Arbiter
	@brief Wire, Inhibitor, Suppressなどの記録
	Arbiterとしての統一表現を試みる
	@date 9th November 2011
	@author Kenichi Yorozu
 */
class Arbiter : public SAModule {
public:
	/**
		@brief Default Constructor
		とりあえずArbiterを作りたいときに利用.
	 */
	Arbiter();
	/**
		@brief factor = 0.0 - 1.0のランダムのArbiter
	 */
	Arbiter(SAModule* src, int srcPort, SAModule* dest, int destPort, int type);
	/**
		@brief factor固定のArbiter
	 */
	Arbiter(SAModule* src, int srcPort, SAModule* dest, int destPort, int type, float factor);
	/**
		@brief 指定範囲内factor固定のArbiter
	 */
	Arbiter(SAModule* src, int srcPort, SAModule* dest, int destPort, int type,
		float factor_min, float factor_max);
	/**
		@brief Destructor. Free Memories
	 */
	virtual ~Arbiter();
	/**
		@brief Transfer/Modify/Arbit Signal
		@sa generateSignal
	 */
	void Run();

	/**
		@brief set Source. addInputの代わりに利用
		@param src 接続元Module
		@param srcPort 接続元Module上の出力ポート番号
	 */
	void setSource(SAModule* src, int srcPort);
	/**
		@brief set Destination. addOutputの代わりに利用
		@param dest 接続先Module
		@param destPort 接続先Module上の入力ポート番号
	 */
	void setDestination(SAModule* dest, int destPort);
	
	/**
		@brief 接続先が元々持っている信号を得る
		@return 接続先が保持する信号
	 */
	float getDest() const;
	/**
		@brief 接続元が持っている信号を得る
		@return 接続元が保持する信号
	 */
	float getSrc() const;

	/**
		@brief 入力信号を得る．
		override SAModule::getInput
		@return input 入力信号の配列要素の値
		@sa SAModule::getInput
	 */
	float getInput() const;
	/**
		@brief 入力信号を得る．
		override SAModule::getInput
		@param index [disabled]得る配列要素のモジュール上の入力ポート番号
		@return signal 入力信号の配列要素の値
		@sa SAModule::getInput
	 */
	float getInput(int index) const;
	void setInput(int index, float signal);
	void setInput(float signal);
	/**
		@brief 出力信号を設定する
		override SAModule::setOutput
		@param signal 出力信号の配列要素へ代入する値
		@sa SAModule::setOutput
	 */
	void setOutput(float signal);
	/**
		@brief 出力信号を設定する
		override SAModule::setOutput
		@param signal 出力信号の配列要素へ代入する値
		@param index [disabled]設定する配列要素のモジュール上の出力ポート番号
		@sa SAModule::setOutput
	 */
	void setOutput(int index, float signal);
	float getOutput(int index) const;
	float getOutput() const;
	/**
		@brief Getter of factor
		@sa factor
		@return factor
	 */
	float getFactor() const;
	/**
		@brief get currentRatio
		@sa currentRatio
		@return currentRatio
	 */
	float getCurrentRatio() const;
protected:
	/**
		@brief Generate Signal
		@return signal
	 */
	double generateSignal();

	///接続元のModuleへのポインタ
	SAModule* source;
	///接続先のModuleへのポインタ
	SAModule* destination;
	///接続元のModuleの出力ポート番号
	int srcPort;
	///接続先のModuleの入力ポート番号
	int destPort;
	///一回のModifyが有効な時間．単位はclock. nullの場合もある
	int timeToModify;
	///Modifyするかしないかを決めたりする確率
	float factor;
	///現在の混ぜ具合. (1.0で上 100%, 0.0で下 100%)
	float currentRatio;
	///残りModify時間.
	int timeLeftModified;
	///ランダム一様分布生成器
	Random<boost::uniform_real<> > _rand;
	///ArbiterのType
	int type;
};

inline float Arbiter::getFactor() const {
	return factor;
}

inline float Arbiter::getCurrentRatio() const{
	return currentRatio;
}

#endif	//_Arbiter_H_