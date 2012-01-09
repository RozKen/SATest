#ifndef _Constants_H_
#define _Constants_H_

//Robot内, Robot-World間情報処理の遅延を考慮する場合，宣言する
//#define CONSIDER_DELAY

//Importance-Basedでやるとき宣言
#define IMPORTANCE_BASED
#ifdef IMPORTANCE_BASED
#define IMPORTANCE_POWER	2
#define VERY_IMPORTANT		100.0f
#endif

/**
	NOW MODIFYING the meaning of SUPPRESSOR
	If using normal suppressor, Comment out this.
 */
#ifndef	IMPORTANCE_BASED
#define INVERSE_SUPPRESSOR
#endif

//削除予定
#define LENGTH				100
/**
	@brief Field1マス分の長さ [m]
	@sa WIFI_REACH
 */
#define FIELD_SCALE			1
/**
	@brief 1 step 分の時間の長さ [sec]
	Unmanned Ground Vehicleの速さが 1 m/sを想定
 */
#define TIME_SCALE			10
///Sensorの有効な範囲のCell数: マンハッタン距離で3
#define MAX_AREA			25
///Hashデータが持つ最大index ( > MAX_AREA )
#define HASH_MAX			25

///Field上の属性とその対応値(Hash)
#define NO_DATA_ON_FIELD	-99
#define OUTOFAREA		-1
#define NORMAL			0
#define ONSTART			1
#define ONGOAL			2
#define ONCHARGER		3
///全体における障害物の存在確率
#define P_BARRIER		0.05		//0.05

/////////ContAlive/////////////
///Batteryが低いと判断するしきい値
#ifdef	IMPORTANCE_BASED
#define BATTERY_LOW		0.8
#else	//IMPORTANCE_BASED
#define BATTERY_LOW		0.2
#endif	//IMPORTANCE_BASED
#define MAX_BAT			100
#define BAT_LOSS		3

/////////ContProgress/////////
#ifdef	IMPORTANCE_BASED
#define PROGRESS_LOW	0.8
#else	//IMPORTANCE_BASED
#define PROGRESS_LOW	0.2
#endif	//IMPORTANCE_BASED

/////////Vision Scope/////////////
/*	
	視野の奥行き:自分から2以上離れていれば，一度に1しか
	動かないロボット同士はぶつからない
*/
#define RANGE			5		//RANGE = (int)ceil(RANGE_DANGER);
#define RANGE_DANGER	5.0f

///無信号状態
#define NO_SIGNAL		-99.0f
#define	DISABLED		-999.0f

///円周率π : 1.15桁まで (double の有効数字は10進数で16桁未満)
#define PI				3.141592653589793

///Start地点の座標
#define START_X			0.0

///一度の動ける距離
#ifdef CONSIDER_DELAY
#define MAX_DRIVE		0.3333f
#else
#define MAX_DRIVE		1.0f
#endif	//CONSIDER_DELAY

///WorldをRunするInterval [millisecond]
#ifdef CONSIDER_DELAY
#define TIME_STEP		33.33
#else
#define TIME_STEP		100.0
#endif	//CONSIDER_DELAY

#endif //_Constants_H_