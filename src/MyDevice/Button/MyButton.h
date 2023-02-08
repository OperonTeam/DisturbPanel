//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyDevice/Button/MyButton.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYBUTTON_H
#define MYBUTTON_H

//--------------------------------------------------------------------------------------------------
// 物理キー及びジェスチャーセンサー・イベント（物理キーのコメントアウトについて）
//　機能操作は、WioTerminal内臓のボタンか、ジェスチャーセンサーを利用しており、このクラスでは
//	内蔵ボタンの操作処理を受け持っています。
//	クラスを作ったものの、残念ながら何も考えずにWioTerminal筐体にグルーガンで外部液晶パネルを
//	取り付けた為、ＡＢＣ３つの物理キーの押下・操作することが出来ません。
//	尚、ＡＢＣ３つの物理ボタンを利用した機能は、ジェスチャーセンサーが受け持っています。
//	ジェスチャーセンサーは、こちらー＞src/MyTask/MyI2C/PAJ7620	
//
// ＡＢＣキー ＋ 十字ボタン
//									<BUTTON>			<AXIS>
//  +--C--B--A--------------+		C : WIO_KEY_C		Z : WIO_5S_UP
//  |   +---------------+   |		B : WIO_KEY_B			WIO_5S_DOWN
//  |   |WioTemrinal    |   |		A : WIO_KEY_A			WIO_5S_LEFT
//  |   |               |   |								WIO_5S_RIGHT
//  |   |               |   |								WIO_5S_PRESS
//  |   +---------------+ Z |
//  +-----------------------+
//---
//--------------------------------------------------------------------------------------------------
// キー操作イベント
//--------------------------------------------------------------------------------------------------
#define	MY_KEYPAD_ALLMASK				(UINT)0xFFFF		// 全べてのマスク情報
#define	MY_KEYPAD_EVENT_NOTNOTIFY		(UINT)0x0000		// 発生イベント無し
//----------------------------------------------------------// WioTerminal ABCキー	| PAJ7620U2
#define	MY_KEYPAD_BTN_A_CLICK			(UINT)0x0001		// ボタンＡ				| Clockwise（時計回り）
#define	MY_KEYPAD_BTN_B_CLICK			(UINT)0x0002		// ボタンＢ				| Count Clockwise（反時計回り）
#define	MY_KEYPAD_BTN_C_CLICK			(UINT)0x0004		// ボタンＣ				| Wave（波）
//----------------------------------------------------------// WioTerminal 十字キー	| PAJ7620U2
#define	MY_KEYPAD_STICK_UP				(UINT)0x0100		// キー・ＵＰ			| UP（上へ）
#define	MY_KEYPAD_STICK_DOWN			(UINT)0x0200		// キー・ＤＯＷＮ		| DOWN（下へ）
#define	MY_KEYPAD_STICK_LEFT			(UINT)0x0400		// キー・ＬＥＦＴ		| LEFT（左へ）
#define	MY_KEYPAD_STICK_RIGHT			(UINT)0x0800		// キー・ＲＩＧＨＴ		| RIGHT（右へ）
#define	MY_KEYPAD_STICK_PUSH			(UINT)0x1000		// キー・ＰＵＳＨ		| FORWARD（近接）
#define	MY_KEYPAD_STICK_PULL			(UINT)0x2000		// -対象外-				| BACKWARD（遠離
//---
//--------------------------------------------------------------------------------------------------
// Ｂｕｔｔｏｎデバイス用クラス
//--------------------------------------------------------------------------------------------------
class   _MyButton {
	private:
		UINT		uiGesture;												// ジェスチャーセンサー情報
		UINT		MyButtonReleased(UINT uiMaskKeyID = MY_KEYPAD_ALLMASK);

	public:
		_MyButton();
		~_MyButton();
		//---
		//------------------------------------------------------------------------------------------
		// 手動操作（内蔵ボタン、ジェスチャーセンサ）情報・アクセスメソッド
		//	SetInterrupt				: 物理ボタン割り込み初期化
		//	SetManualOperationInfo		: 手動操作情報の退避
		//	GetManualOperationInfo		: 手動操作情報の取得
		//------------------------------------------------------------------------------------------
		VOID		SetInterrupt(VOID);
		VOID		SetManualOperationInfo(UINT uiGesture);
		UINT		GetManualOperationInfo(UINT uiMaskInfo = MY_KEYPAD_ALLMASK);
};
#endif
/*-- End Of File --*/
