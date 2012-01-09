#ifndef	_Cistream_CSV_HPP_
#define _Cistream_CSV_HPP_

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/**
	@class CistreamCSV
	@brief CSV読み込みのためのクラス
	抽出演算子 >> で、次のコンマまでの要素を読み込んで、コンマ自体は読み飛ばす．
	また，改行を吸収するマニピュレータ endl も定義
	@sa http://homepage2.nifty.com/well/Stream.html
	@author C++ Maniac
 */

class CistreamCSV {
	// マニピュレータ endl の定義
	friend CistreamCSV& endl(CistreamCSV& riscsv) {
		// 行末まで読み飛ばす。
		int iChar = 0;
		while ((iChar = riscsv.m_ris.get()) != '\n' && iChar != EOF) {}
		return riscsv;
	}
private:
	istream& m_ris;		// 入力に使う istream の参照
public:
	CistreamCSV(istream& ris) : m_ris(ris) {}
	CistreamCSV& operator >>(int& ri) {
		ri = 0;
		int iChar = 0;
		// 空白を読み飛ばす。
		while (isspace(iChar = m_ris.get()) && iChar != '\n') {}
		if (iChar == EOF ) { return *this; }
		string strValue;
		if (iChar == '\n') { m_ris.putback(iChar); iChar = 0; }
		else			   { strValue += iChar; 			  }
		// コンマまで読む。
		while ((iChar = m_ris.get()) != ',' && iChar != EOF && iChar != '\n') {
			strValue += iChar;
		}
		ri = atoi(strValue.c_str());
		if (iChar == EOF ) { return *this;		   }
		if (iChar == '\n') { m_ris.putback(iChar); }
		return *this;
	}
	// マニピュレータを受け入れるための挿入演算子
	CistreamCSV& operator >>(CistreamCSV& (*pmanipulatorfunction)(CistreamCSV&)) {
		return (*pmanipulatorfunction)(*this);
	}
};

#endif	//_Cistream_CSV_HPP_