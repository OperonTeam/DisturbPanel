//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/Utility/MyDrawScroll.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDrawScroll.h"

//--------------------------------------------------------------------------------------------------
// クラスインスタンスの初期処理
//--------------------------------------------------------------------------------------------------
_MyDrawScroll::_MyDrawScroll()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの終了処理
//--------------------------------------------------------------------------------------------------
_MyDrawScroll::~_MyDrawScroll()
{
}
/**/
//	MyPanelB.drawRect(69, 0+17,		 	 60, 18, GxEPD_BLACK);		MyPanelB.setCursor(70, 14+17);			MyPanelB.printf("  12");	// Weather 気温(℃)
/**/
//
//
//
/**/
VOID				_MyDrawScroll::SpriteDrawScrollStringInit(INT iW, INT iH, INT iX, INT iY, String strMessage)
{
	//----------------------------------------------------------------------------------------------
	// 現在描画中のメッセージと内容が変わっていたら、スクロール描画用情報を初期化する
	//----------------------------------------------------------------------------------------------
	if (strMessage.compareTo(this->strScrollMessage)) {
		this->strScrollMessage = strMessage;
		this->iScrollMode = 0;											// スクロールモード
		this->iSystemDrawAreaScroll = 0;								// メッセージスクロールカウンタ
		this->ulDrawJARVisMessageTime = (ULONG)0;						// 前回描画時刻（millis）
		this->iScrollW				= iW;
		this->iScrollH				= iH;
		this->iScrollX				= iX;
		this->iScrollY				= iY;
	}
}
/**/
BOOL				_MyDrawScroll::SpriteDrawSolidStringRun(
_MyLGFX				*pMyLGFX)
{
	//----------------------------------------------------------------------------------------------
	// 描画が必要なタイミングをチェックする
	//----------------------------------------------------------------------------------------------
	#define	MY_SCROLLFONT_SIZEX			1.0
	#define	MY_SCROLLFONT_SIZEY			2.0
//
	//----------------------------------------------------------------------------------------------
	// オフスクリーン・バッファ領域を作成する
	//----------------------------------------------------------------------------------------------
	BOOL	bReturn = MY_FALSE;
	pMyLGFX->SpriteCreate(this->iScrollW, this->iScrollH, 2);

	//----------------------------------------------------------------------------------------------
	// 描画文字列を表示して指定秒数だけホールド
	//----------------------------------------------------------------------------------------------
	pMyLGFX->SpriteDrawStringFormat(this->strScrollMessage, this->iScrollW - 2, 2, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, MY_SCROLLFONT_SIZEX, MY_SCROLLFONT_SIZEY, textdatum_t::top_right);

	//----------------------------------------------------------------------------------------------
	// 描画を行う
	//----------------------------------------------------------------------------------------------
	pMyLGFX->SpriteDrawRect(0, 0, this->iScrollW, this->iScrollH, MY_PALETTE_DARKGREEN);
	pMyLGFX->SpritePush(this->iScrollX, this->iScrollY, MY_TRUE);
//
	return	bReturn;
}
/**/
//--------------------------------------------------------------------------------------------------
// 執事メッセージをスクロール描画する
//	+---------------------------+	サイドパネル上の配置座標は、(0, 0) - (319, 52)
//	| JARVis String				|
//	|							|
//	|							|
//	|							|
//	+---------------------------+
//--------------------------------------------------------------------------------------------------
BOOL				_MyDrawScroll::SpriteDrawScrollStringRun(
_MyLGFX				*pMyLGFX)
{
	//----------------------------------------------------------------------------------------------
	// 描画が必要なタイミングをチェックする
	//----------------------------------------------------------------------------------------------
	#define	MY_SCROLLFONT_SIZEX			1.0
	#define	MY_SCROLLFONT_SIZEY			2.0
	#define	MY_SCROLL_EXPANSION_SIZE	50
	#define	MY_SCROLL_SHRINK_SIZE		16
	#define	MY_SCROLL_HOLDTIME			3000
//
	//----------------------------------------------------------------------------------------------
	// オフスクリーン・バッファ領域を作成する
	//----------------------------------------------------------------------------------------------
	BOOL	bReturn = MY_FALSE;
	pMyLGFX->SpriteCreate(this->iScrollW, this->iScrollH, 2);

	//----------------------------------------------------------------------------------------------
	// 動的オブジェクトを生成する
	//----------------------------------------------------------------------------------------------
	switch(this->iScrollMode) {
		case	0:
			//--------------------------------------------------------------------------------------
			// クリップ領域を（左端固定：右端を拡張）することで、描画文字列が広がっていく様に見せる
			//--------------------------------------------------------------------------------------
			pMyLGFX->SpriteDrawRect(0, 0, this->iScrollW, this->iScrollH, MY_PALETTE_DARKGREEN);
			pMyLGFX->SpriteSetClipRect(0, 0, this->iSystemDrawAreaScroll, this->iScrollH);
			pMyLGFX->SpriteDrawStringFormat(this->strScrollMessage, 2, 2, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, MY_SCROLLFONT_SIZEX, MY_SCROLLFONT_SIZEY, textdatum_t::top_left);
			//
			this->iSystemDrawAreaScroll += MY_SCROLL_EXPANSION_SIZE;				// スクロール・スライド値
			if (this->iSystemDrawAreaScroll > this->iScrollW) {
				this->iSystemDrawAreaScroll = 0;
				this->iScrollMode = 1;
				this->ulDrawJARVisMessageTime = millis();
			}
			break;
		case	1:
			//--------------------------------------------------------------------------------------
			// 描画文字列を表示して指定秒数だけホールド
			//--------------------------------------------------------------------------------------
			pMyLGFX->SpriteDrawStringFormat(this->strScrollMessage, 2, 2, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, MY_SCROLLFONT_SIZEX, MY_SCROLLFONT_SIZEY, textdatum_t::top_left);
			if (this->ulDrawJARVisMessageTime + MY_SCROLL_HOLDTIME < millis()) {
				this->iScrollMode = 2;
			}
			break;
		case	2:
			//--------------------------------------------------------------------------------------
			// 文字列の描画位置を左にシフトさせることで、描画文字列が右にスライド表示する様に見せる
			//--------------------------------------------------------------------------------------
			this->iSystemDrawAreaScroll -= MY_SCROLL_SHRINK_SIZE;					// 描画座標のシフト値
			pMyLGFX->SpriteDrawStringFormat(this->strScrollMessage, this->iSystemDrawAreaScroll, 2, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, MY_SCROLLFONT_SIZEX, MY_SCROLLFONT_SIZEY, textdatum_t::top_left);
			if (abs(this->iSystemDrawAreaScroll) > pMyLGFX->SpriteTextWidth(this->strScrollMessage)) {
				this->iScrollMode = 3;
			}
			break;
		case	3:
			//--------------------------------------------------------------------------------------
			// 
			//--------------------------------------------------------------------------------------
			this->iSystemDrawAreaScroll = 0;
			this->iScrollMode = 0;
			bReturn = MY_TRUE;
			break;
	}
	//----------------------------------------------------------------------------------------------
	// 描画を行う
	//----------------------------------------------------------------------------------------------
	pMyLGFX->SpriteDrawRect(0, 0, this->iScrollW, this->iScrollH, MY_PALETTE_DARKGREEN);
	pMyLGFX->SpritePush(this->iScrollX, this->iScrollY, MY_TRUE);
//
	return	bReturn;
}
/*-- End Of File --*/
