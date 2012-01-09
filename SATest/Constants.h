#ifndef _Constants_H_
#define _Constants_H_

//Robot��, Robot-World�ԏ�񏈗��̒x�����l������ꍇ�C�錾����
//#define CONSIDER_DELAY

//Importance-Based�ł��Ƃ��錾
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

//�폜�\��
#define LENGTH				100
/**
	@brief Field1�}�X���̒��� [m]
	@sa WIFI_REACH
 */
#define FIELD_SCALE			1
/**
	@brief 1 step ���̎��Ԃ̒��� [sec]
	Unmanned Ground Vehicle�̑����� 1 m/s��z��
 */
#define TIME_SCALE			10
///Sensor�̗L���Ȕ͈͂�Cell��: �}���n�b�^��������3
#define MAX_AREA			25
///Hash�f�[�^�����ő�index ( > MAX_AREA )
#define HASH_MAX			25

///Field��̑����Ƃ��̑Ή��l(Hash)
#define NO_DATA_ON_FIELD	-99
#define OUTOFAREA		-1
#define NORMAL			0
#define ONSTART			1
#define ONGOAL			2
#define ONCHARGER		3
///�S�̂ɂ������Q���̑��݊m��
#define P_BARRIER		0.05		//0.05

/////////ContAlive/////////////
///Battery���Ⴂ�Ɣ��f���邵�����l
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
	����̉��s��:��������2�ȏ㗣��Ă���΁C��x��1����
	�����Ȃ����{�b�g���m�͂Ԃ���Ȃ�
*/
#define RANGE			5		//RANGE = (int)ceil(RANGE_DANGER);
#define RANGE_DANGER	5.0f

///���M�����
#define NO_SIGNAL		-99.0f
#define	DISABLED		-999.0f

///�~������ : 1.15���܂� (double �̗L��������10�i����16������)
#define PI				3.141592653589793

///Start�n�_�̍��W
#define START_X			0.0

///��x�̓����鋗��
#ifdef CONSIDER_DELAY
#define MAX_DRIVE		0.3333f
#else
#define MAX_DRIVE		1.0f
#endif	//CONSIDER_DELAY

///World��Run����Interval [millisecond]
#ifdef CONSIDER_DELAY
#define TIME_STEP		33.33
#else
#define TIME_STEP		100.0
#endif	//CONSIDER_DELAY

#endif //_Constants_H_