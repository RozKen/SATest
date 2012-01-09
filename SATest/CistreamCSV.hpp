#ifndef	_Cistream_CSV_HPP_
#define _Cistream_CSV_HPP_

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/**
	@class CistreamCSV
	@brief CSV�ǂݍ��݂̂��߂̃N���X
	���o���Z�q >> �ŁA���̃R���}�܂ł̗v�f��ǂݍ���ŁA�R���}���͓̂ǂݔ�΂��D
	�܂��C���s���z������}�j�s�����[�^ endl ����`
	@sa http://homepage2.nifty.com/well/Stream.html
	@author C++ Maniac
 */

class CistreamCSV {
	// �}�j�s�����[�^ endl �̒�`
	friend CistreamCSV& endl(CistreamCSV& riscsv) {
		// �s���܂œǂݔ�΂��B
		int iChar = 0;
		while ((iChar = riscsv.m_ris.get()) != '\n' && iChar != EOF) {}
		return riscsv;
	}
private:
	istream& m_ris;		// ���͂Ɏg�� istream �̎Q��
public:
	CistreamCSV(istream& ris) : m_ris(ris) {}
	CistreamCSV& operator >>(int& ri) {
		ri = 0;
		int iChar = 0;
		// �󔒂�ǂݔ�΂��B
		while (isspace(iChar = m_ris.get()) && iChar != '\n') {}
		if (iChar == EOF ) { return *this; }
		string strValue;
		if (iChar == '\n') { m_ris.putback(iChar); iChar = 0; }
		else			   { strValue += iChar; 			  }
		// �R���}�܂œǂށB
		while ((iChar = m_ris.get()) != ',' && iChar != EOF && iChar != '\n') {
			strValue += iChar;
		}
		ri = atoi(strValue.c_str());
		if (iChar == EOF ) { return *this;		   }
		if (iChar == '\n') { m_ris.putback(iChar); }
		return *this;
	}
	// �}�j�s�����[�^���󂯓���邽�߂̑}�����Z�q
	CistreamCSV& operator >>(CistreamCSV& (*pmanipulatorfunction)(CistreamCSV&)) {
		return (*pmanipulatorfunction)(*this);
	}
};

#endif	//_Cistream_CSV_HPP_