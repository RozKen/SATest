#ifndef _Logger_H_
#define _Logger_H_

#include <string>
#include <vector>
#include <fstream>

/**
	@class Logger
	@brief 変数値をファイルに記録するオブジェクト．
	扱える変数は
	<ul>
		<li>int型</li>
		<li>float型</li>
		<li>std::vector<int>*型の内部の要素(int)</li>
		<li>std::vector<float>*型の内部の要素(float)</li>
	</ul>
	Blackboard に保管されている変数にアクセスして、その時々の値を記録する．
	@author Kenichi Yorozu
	@date 11th November 2011
	@sa Blackboard
 */

class Logger{
public:
	/**
		@brief Default Constructor: <b>filePathを後で指定する必要がある</b>
	 */
	Logger();
	/**
		@brief Constructor
		@param filePath ログファイルを書き出すFileへのpath
		@sa logFilePath
	 */
	Logger(std::string filePath);
	/**
		@brief Constructor
		@param directoryPath ログファイルを書き出すDirectoryへのpath
		@param fileName
		@sa logDirectoryPath ログファイル名
	 */
	Logger(std::string directoryPath, std::string fileName);

	/**
		@brief Destructor
	 */
	virtual ~Logger();
	/**
		@brief <b>現在の状態(変数)を記録</b>
	 */
	void Log();

	/**
		@brief 内部変数の初期化. ofstreamの設定など
	 */
	void Initialize();

	/**
		@brief int型の変数を記録するよう登録する
		@param title 変数を説明するタイトル
		@param pointer_to_integer_variable int型変数へのポインタ
		@sa logTitles
		@sa logTypes
		@sa logContents
	 */
	void add(std::string title, int* pointer_to_integer_variable);
	/**
		@brief float型の変数を記録するよう登録する
		@param title 変数を説明するタイトル
		@param pointer_to_float_variable float型変数へのポインタ
	 */
	void add(std::string title, float* pointer_to_float_variable);
	/**
		@brief int型のvectorに含まれている値を記録するよう登録する
		@param title 変数を説明するタイトル
		@param vector 値が含まれているvector
		@param index vectorにおける，index
	 */
	void add(std::string title, std::vector<int>* vector, int index);
	/**
		@brief float型のvectorに含まれている値を記録するよう登録する
		@param title 変数を説明するタイトル
		@param vector 値が含まれているvector
		@param index vectorにおける，index
	 */
	void add(std::string title, std::vector<float>* vector, int index);
	
	/**
		@brief logFilePathを設定する
		@param filePath logFilePath
		@sa logFilePath
	 */
	void setFilePath(std::string filePath);
	/**
		@brief logFilePathを設定する
		@param fileDirectoryPath ログファイルを保存するフォルダへのパス
		@param fileName ログファイル名
		@sa logFilePath
		@sa logDirectoryPath
		@sa logFileName
	 */
	void setFilePath(std::string fileDirectoryPath, std::string fileName);
	/**
		@brief logFilePathを与える
		@return logFilePath
		@sa logFilePath
	 */
	std::string getFilePath() const;
	std::string getDirectoryPath() const;
	std::string getFileName() const;
	/**
		@brief 整数型の数を，四桁の数字の文字列へ変換する
		@param number 変換したい整数型の数
		@return 四桁の数字の文字列
	 */
	std::string intToString(int number);
protected:
	///ログファイルを保存するディレクトリへのPath
	std::string logDirectoryPath;
	///ログファイル名前
	std::string logFileName;
	///ログファイルを書き出す場所へのpath
	std::string logFilePath;
	///ログの種類を説明するタイトル．一行目に表示される
	std::vector<std::string> *logTitles;
	/**
		@brief ログの変数の種類
		<ul>
			<li>0 : int型</li>
			<li>1 : float型</li>
			<li>2 : std::vector<float*> 型配列中の
				logArrayIndexに書いてあるindexの要素</li>
			<li>3 : std::vector<int*> 型配列中の
				logArrayIndexに書いてあるindexの要素</li>
		</ul>
	 */
	std::vector<int>	*logTypes;
	///記録する変数へのポインタ
	std::vector<void *> *logContents;
	std::vector<int>	*logArrayIndex;
	///Logした回数のカウンタ
	int count;
	/**
		@brief File Output Stream
	 */
	std::ofstream ofs;

	/**
		@brief String to Wide String
	 */
	std::wstring s2ws(const std::string& s);
};

#endif	//_Logger_H_