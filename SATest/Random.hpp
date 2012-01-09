#ifndef _Random_HPP_
#define _Random_HPP_

#include <boost/random.hpp>
#include <ctime>

/**
	@class Random
	@brief Random�����N���X.
	�I�u�W�F�N�g�𐶐�������C"�I�u�W�F�N�g��()"�Ń����_���Ȓl���͂��o��
	�����_���̐�����boost::mt19937�ɂ�郁���Z���k�c�C�X�^�D��l���z
	@author n_shuyo
	@sa http://d.hatena.ne.jp/n_shuyo/20100407
 */

template<class D, class G = boost::mt19937>
class Random{
public:
	///Default Constructor
	//Random() : _gen(static_cast<unsigned long>(time (0))),
	Random() : _gen(static_cast<unsigned long>(clock())),
		_Random(_gen, _dst){}
	///�悭������Ȃ����ǁC���Ԃ�͈͂��w�肵���R���X�g���N�^
	template<typename T1>
	//Random(T1 a1) : _gen(static_cast<unsigned long>(time(0))),
	Random(T1 a1) : _gen(static_cast<unsigned long>(clock())),
		_dst(a1), _Random(_gen, _dst){}
	///�͈͂��w�肵���R���X�g���N�^
	template<typename T1, typename T2>
	//Random(T1 a1, T2 a2): _gen(static_cast<unsigned long>(time(0))),
	Random(T1 a1, T2 a2): _gen(static_cast<unsigned long>(clock())),
		_dst(a1, a2), _Random(_gen, _dst){}

	///RandomObject��"()"��t����ƁC�����_���Ȑ��l���߂��Ă���
	typename D::result_type operator()(){
		return _Random();
	}
protected:
	///Random Generator
	G _gen;
	///Distribution Range
	D _dst;
	///Random Generator
	boost::variate_generator<G, D> _Random;
};

#endif //_Random_HPP_