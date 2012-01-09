#ifndef _DataInDirectoryInvestigator_H_
#define _DataInDirectoryInvestigator_H_

#include <vector>

/**
	@class DataInDirectoryInvestigator
	@brief ディレクトリ内のファイルを検索し，該当ファイルからデータを抽出し，まとめる
	@date 27th October 2011
	@author Kenichi Yorozu
 */
class DataInDirectoryInvestigator{
public:
	/**
		@brief Constructor
		@param dirPath 調査するフォルダへのパス ('\'は'\\'とエスケープする必要有り，最後は'\\')
	 */
	DataInDirectoryInvestigator(std::string dirPath);
	/**
		@brief fileTypeにMatchしたファイルを探索する
		@param fileType "*.csv"のような，フィルタ文字列
		@return ディレクトリ内のファイルの名前群
	 */
	std::vector<std::string>* SearchFiles(std::string fileType);
	/**
		@brief index番目の行(column = false)/列(column = true)のデータを取得
		@param column データを取る方向．縦の時true. 行(column = false)/列(column = true)
		@param index データをとる行や列の番号. 0から始まる
		@return データの2次元配列
	 */
	std::vector<std::vector<std::string>* >* AcquireData(bool column, int index);
	/**
		@brief データをまとめたファイルを作成する．今回は，各positionデータを集めて，更にそれらの平均を先頭の列(column)に挿入する
		@param outputPath 出力ファイルの名前
		@return 平均データ
	 */
	std::vector<int>* CreateInvestigateFile(std::string outputPath);

	/**
		@brief Execute DataInDirectoryInvestigator in normal way
	 */
	void Run();
	/**
		@brief setter of dataLength
	 */
	void setDataLength(int length);
protected:
	/**
		@brief std::stringをstd::wstringへ変換
	 */
	std::wstring s2ws(const std::string& s);
	/**
		@brief std::wstringをstd::stringへ変換
	 */
	std::string ws2s(const std::wstring& ws);
	///探索するディレクトリへのPath
	std::string dirPath;
	///探索するファイルの種類
	std::string fileType;
	///ディレクトリ内のファイルへの名前を格納する
	std::vector<std::string>* fileNames;
	// ///ディレクトリ内のファイルへのPathを格納する
	//std::vector<std::string>* filePaths;
	///データ列
	std::vector<std::vector<std::string>* >* data;
	///データの行の長さ
	int dataLength;
};

#endif //_DataInDirectoryInvestigator_H_