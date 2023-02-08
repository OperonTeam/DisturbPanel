//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyDevice/Button/MyButton.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyButton.h"

//--------------------------------------------------------------------------------------------------
// 物理キーボード・情報
//--------------------------------------------------------------------------------------------------
MY_PRIVATE	UINT	uiMyButtonInfo = MY_KEYPAD_EVENT_NOTNOTIFY;
/*
*/
//--------------------------------------------------------------------------------------------------
// 物理キーボード・操作割り込み
// ・ボタン操作は、押したボタンを【離した】操作を行った際のタイミングで、操作イベントを処理します。
// ・WioTemrinalの筐体位置が上下逆向きになる為、十字キーの方向を変更しています。
//--------------------------------------------------------------------------------------------------
#ifndef	MY_BUTTON_INVALIDATION
//--------------------------------------------------------------------------------------------------
// WioTemrinalの筐体上部には３つの物理ボタンが存在しますが、外部液晶パネルを取り付ける際に、誤って
// 潰した為、使い勝手の良いボタンでしたがＡＢＣの３つボタンの扱いを、ソフト的にも潰しておきます。
//--------------------------------------------------------------------------------------------------
MY_PRIVATE	VOID	MyButtonKeyAPush(VOID)	{	uiMyButtonInfo |= MY_KEYPAD_BTN_A_CLICK;	}
MY_PRIVATE	VOID	MyButtonKeyBPush(VOID)	{	uiMyButtonInfo |= MY_KEYPAD_BTN_B_CLICK;	}
MY_PRIVATE	VOID	MyButtonKeyCPush(VOID)	{	uiMyButtonInfo |= MY_KEYPAD_BTN_C_CLICK;	}
#endif
MY_PRIVATE	VOID	MyButton5SUp(VOID)		{	uiMyButtonInfo |= MY_KEYPAD_STICK_DOWN;		}
MY_PRIVATE	VOID	MyButton5SDown(VOID)	{	uiMyButtonInfo |= MY_KEYPAD_STICK_UP;		}
MY_PRIVATE	VOID	MyButton5SRight(VOID)	{	uiMyButtonInfo |= MY_KEYPAD_STICK_LEFT;		}
MY_PRIVATE	VOID	MyButton5SLeft(VOID)	{	uiMyButtonInfo |= MY_KEYPAD_STICK_RIGHT;	}
MY_PRIVATE	VOID	MyButton5SPush(VOID)	{	uiMyButtonInfo |= MY_KEYPAD_STICK_PUSH;		}
/*
*/
//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyButton::_MyButton()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyButton::~_MyButton()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// （このメソッドを呼び出す迄に）ボタンが押されたか？状態を取得する
//--------------------------------------------------------------------------------------------------
UINT		_MyButton::MyButtonReleased(
UINT		uiMaskKeyID)
{
	//----------------------------------------------------------------------------------------------
	// ボタン・オンの精算状態から、指示されたキーIDが一度でも押下されたか？判断する
	//----------------------------------------------------------------------------------------------
	UINT	uiButtonInfoBackup = uiMyButtonInfo & uiMaskKeyID;		// 現状状態を退避
	uiMyButtonInfo = MY_KEYPAD_EVENT_NOTNOTIFY;						// 「発生イベント無し」をセット
	return	uiButtonInfoBackup;										// 退避情報を返す
}
/**/
//--------------------------------------------------------------------------------------------------
// 物理ボタン操作に対する割り込みを登録する
//--------------------------------------------------------------------------------------------------
VOID		_MyButton::SetInterrupt(VOID)
{
	//----------------------------------------------------------------------------------------------
	// 物理ボタン（ＡＢＣ＋十字ボタン）デバイスを初期化する
	// 今回WIO_KEY_Aを利用しませんが、開発時点では本家ライブラリ側のバグで、WIO_KEY_AとWIO_5S_UPの
	// 割り込みの同時登録が出来ませんでした。
	//----------------------------------------------------------------------------------------------
#ifndef	MY_BUTTON_INVALIDATION
	pinMode(WIO_KEY_A,		INPUT_PULLUP);							// 正面から見て、上部右側のボタン
	pinMode(WIO_KEY_B,		INPUT_PULLUP);							// 正面から見て、上部中央のボタン
	pinMode(WIO_KEY_C,		INPUT_PULLUP);							// 正面から見て、上部左側のボタン
#endif
	pinMode(WIO_5S_UP,		INPUT_PULLUP);							// 十字キーの上方向ボタン
	pinMode(WIO_5S_DOWN,	INPUT_PULLUP);							// 十字キーの下方向ボタン
	pinMode(WIO_5S_LEFT,	INPUT_PULLUP);							// 十字キーの左方向ボタン
	pinMode(WIO_5S_RIGHT,	INPUT_PULLUP);							// 十字キーの右方向ボタン
	pinMode(WIO_5S_PRESS,	INPUT_PULLUP);							// 十字キーの押下ボタン
//
	//----------------------------------------------------------------------------------------------
	// 物理ボタンの割り込みを定義（開発者の趣味で,RISINGを利用します）
	//----------------------------------------------------------------------------------------------
	//　LOW		： ピンの状態がLOWのとき（ボタンを押下している最中）に割り込みが発生
	//　RISING	： ピンの状態がLOW  -> HIGH（押されているボタンを戻した時）に変化したときに割り込みが発生
	//　FALLING	： ピンの状態がHIGH -> LOW（ボタンを押した時）に変化したときに割り込みが発生
	//　CHANGE	： ピンの状態がLOW  -> HIGH、HIGH -> LOW（ボタンを押した時と戻した時）どちらでも、割り込みが発生
	//----------------------------------------------------------------------------------------------
#ifndef	MY_BUTTON_INVALIDATION
	attachInterrupt(digitalPinToInterrupt(WIO_KEY_A),	MyButtonKeyAPush,	RISING);
	attachInterrupt(digitalPinToInterrupt(WIO_KEY_B),	MyButtonKeyBPush,	RISING);
	attachInterrupt(digitalPinToInterrupt(WIO_KEY_C),	MyButtonKeyCPush,	RISING);
#endif
	attachInterrupt(digitalPinToInterrupt(WIO_5S_UP),	MyButton5SUp,		RISING);
	attachInterrupt(digitalPinToInterrupt(WIO_5S_DOWN),	MyButton5SDown,		RISING);
	attachInterrupt(digitalPinToInterrupt(WIO_5S_RIGHT),MyButton5SRight,	RISING);
	attachInterrupt(digitalPinToInterrupt(WIO_5S_LEFT),	MyButton5SLeft,		RISING);
	attachInterrupt(digitalPinToInterrupt(WIO_5S_PRESS),MyButton5SPush,		RISING);
}
/**/
//--------------------------------------------------------------------------------------------------
// ジェスチャー(APDS9960)情報アクセスメソッド
//	SetManualOperationInfo		: ジェスチャー情報の設定
//	SetManualOperationInfo		: ジェスチャー情報の取得
//--------------------------------------------------------------------------------------------------
VOID		_MyButton::SetManualOperationInfo(UINT uiGesture)
{
	//----------------------------------------------------------------------------------------------
	// 入力パラメータ（ボタンイベント）情報をORで保存する。
	//----------------------------------------------------------------------------------------------
	this->uiGesture = this->uiGesture | uiGesture;
}
/**/
UINT		_MyButton::GetManualOperationInfo(UINT uiMask)
{
	//----------------------------------------------------------------------------------------------
	// 現在の操作情報と物理ボタン情報をORで取得する。（保存情報はリセットする）
	//----------------------------------------------------------------------------------------------
	UINT	uiGestureBackup = this->uiGesture | this->MyButtonReleased(uiMask);
	this->uiGesture = MY_KEYPAD_EVENT_NOTNOTIFY;
	return	uiGestureBackup;
}
/*-- End Of File --*/
