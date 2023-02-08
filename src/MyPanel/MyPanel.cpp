//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/MyPanel.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyJARVis.h"
#include "MySDWriter.h"
#include "MyPanel.h"
#include "Boot/MyBootsplash.h"

//--------------------------------------------------------------------------------------------------
// 外部拡張パネルの環境（ＳＰＩ接続ピン）定義
// ２．０インチＴＦＴ　２４０ｘ３２０ピクセルＳＴ７７８９コントローラＳＰＩ
//	＜外部拡張パネル共有ピン＞
//		・SPI_MOSI	: Master
//		・SPI_MISO	: Not Use (現在利用しているパネルには、TouchPanel機能が無い)
//		・SPI_SCLK	: Serial Clock
//		・DC  		: Data Command
//	＜外部拡張パネル単独ピン＞
//		・(SPI)CS	: Chip Select
//		・RST		: Reset
//--------------------------------------------------------------------------------------------------
/**/
#ifdef	MY_NEWGFX_INTERNAL_PANEL
//--------------------------------------------------------------------------------------------------
// 内臓パネル（WioTemrinal）：	IlI9341
//--------------------------------------------------------------------------------------------------
struct LGFX_Config
{
	static constexpr int sercom_index = 7;
	static constexpr int sercom_clksrc = 0;   // -1=notchange / 0=select GCLK0
	static constexpr int sercom_clkfreq = F_CPU;
	static constexpr int spi_miso = 0x0100 | PIN_PA18;	// PORTB 18 (PORTB=0x0100 | 18=0x0012)
	static constexpr int spi_mosi = 0x0100 | PIN_PA19;	// PORTB 19 (PORTB=0x0100 | 19=0x0013)
	static constexpr int spi_sclk = 0x0100 | PIN_PA20;	// PORTB 20 (PORTB=0x0100 | 20=0x0014)
	static constexpr SercomSpiTXPad pad_mosi = SPI_PAD_3_SCK_1;  // PAD_SPI3_TX;
	static constexpr SercomRXPad    pad_miso = SERCOM_RX_PAD_2;  // PAD_SPI3_RX;
};
static lgfx::LGFX_SPI<LGFX_Config> MyLcd;
static lgfx::Panel_ILI9341 MyPanel;
#endif
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの初期処理
//--------------------------------------------------------------------------------------------------
_MyPanel::_MyPanel()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの終了処理
//--------------------------------------------------------------------------------------------------
_MyPanel::~_MyPanel()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// TFT液晶パネル・デバイスを初期化する
//--------------------------------------------------------------------------------------------------
VOID				_MyPanel::Initialize(_MyDevice *pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// ＴＦＴパネルセットアップ情報を取得する
	//----------------------------------------------------------------------------------------------
	this->DrawLayout	= _DrawLayout::None;										// メインパネル（全体）	
	this->bDrawInitFlag = MY_TRUE;													// 初期化フラグ
	this->lTFTSleepTime = MyGlobalTool.MyConfigFile.lTFTSleepTime;					// ＴＦＴ・パネル自動消灯時間（秒）
//--
	//----------------------------------------------------------------------------------------------
	// 液晶（メイン）パネルの初期化を行う
	//----------------------------------------------------------------------------------------------
	this->MyLGFXMain.LcdSetup(_MyLGFX::_PanelType::PanelMain, MYSPI_PANEL_A_ROTATION);
//--
	//----------------------------------------------------------------------------------------------
	// 液晶（サイド）パネルの初期化を行う
	//----------------------------------------------------------------------------------------------
	this->MyLGFXSide.LcdSetup(_MyLGFX::_PanelType::PanelSide, MYSPI_PANEL_B_ROTATION);
}
/**/
//--------------------------------------------------------------------------------------------------
// ＴＦＴパネル・デバイスをセットアップする
//--------------------------------------------------------------------------------------------------
VOID				_MyPanel::DrawBootsplash(
_MyDevice			*pMyDevice,
_MyJARVis			*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// 液晶（メイン・サイド）パネルで起動ロゴの描画を行う
	//----------------------------------------------------------------------------------------------
	_MyBootsplash		MyBootsplash;
	MyBootsplash.DrawBootLogo(&this->MyLGFXMain, &this->MyLGFXSide);
	MyBootsplash.DrawBooting(&this->MyLGFXMain, &this->MyLGFXSide, pMyDevice);
}
/**/
//--------------------------------------------------------------------------------------------------
// （ＬＣＤ表示領域全体）情報表示スクリーン処理
//--------------------------------------------------------------------------------------------------
BOOL			_MyPanel::DrawPanel(
_MyDevice		*pMyDevice,
_MyJARVis		*pMyJARVis)
{
	//----------------------------------------------------------------------------------------------
	// 入力されたジェスチャー情報から、次回描画するパネル・レイアウトを求めます
	//----------------------------------------------------------------------------------------------
	BOOL	bDrawFlag = this->QueNextLayout(pMyDevice, this->DrawLayout);
	if (bDrawFlag) {
		//------------------------------------------------------------------------------------------
		// 描画するパネル・レイアウトが変更された場合は、表示パネルを初期化する
		//------------------------------------------------------------------------------------------
		this->MyLGFXMain.LcdClear();
		this->MyLGFXSide.LcdClear();
	}
	//---
BOOL	bKeep = this->bDrawInitFlag;
	//---
	//----------------------------------------------------------------------------------------------
	// 描画を行うオブジェクトのパネル全体をスリープ(Blank)描画以外の場合
	//----------------------------------------------------------------------------------------------
	switch(this->DrawLayout) {
		//------------------------------------------------------------------------------------------
		// メインパネル（右側）のパネル描画
		//------------------------------------------------------------------------------------------
		case	_DrawLayout::MainArea:									// メインパネル（全体）	
			this->MyPanelMain.MyUpdateAll(this->bDrawInitFlag, &this->MyLGFXMain, pMyDevice, pMyJARVis);
			this->DrawLayout = _DrawLayout::SideTopConsole;
			break;
	//---
		//------------------------------------------------------------------------------------------
		// サイドパネル（左側上部）のパネル描画
		//------------------------------------------------------------------------------------------
		case	_DrawLayout::SideTopConsole:							// サイドパネル・上層領域（メッセージ）
			this->MyPanelSide.DrawTopSystemConsole(this->bDrawInitFlag, &this->MyLGFXSide, pMyDevice, pMyJARVis);
			this->DrawLayout = _DrawLayout::SideTopMarker;
			break;

		case	_DrawLayout::SideTopMarker:								// サイドパネル・上層領域（ステータス）
			this->MyPanelSide.DrawTopSystemMarker(this->bDrawInitFlag, &this->MyLGFXSide, pMyDevice);
			this->DrawLayout = _DrawLayout::SideLowArea; 
			break;

		//------------------------------------------------------------------------------------------
		// サイドパネル（左側下部）のパネル描画
		//------------------------------------------------------------------------------------------
		case	_DrawLayout::SideLowArea:								// サイドパネル・下層領域
			this->MyPanelSide.DrawLowSystemArea(this->bDrawInitFlag, &this->MyLGFXSide, pMyDevice, pMyJARVis);
			this->DrawLayout = _DrawLayout::MainArea;
			this->bDrawInitFlag = MY_FALSE;								// 初期化フラグ・オフ
			break;

		case	_DrawLayout::AllBlank:									// パネルブランク
		default:
			this->bDrawInitFlag = MY_FALSE;								// 初期化フラグ・オフ
			break;
	}

//MY_LOG("I AM DrawPanel(%d) DrawInitFlag = %d -> %d\n", this->DrawLayout, bKeep, this->bDrawInitFlag);

	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// 入力されたジェスチャー情報から、次回表示するパネルレイアウト情報を返却する
//---
// 表示パネルの全体像
//	InternetでポチったST7789(320x240) 2枚（左がSidePanel,右がMainPanel）
//		+-----------------------+	+-----------------------+
//		|	MyPanelSide(Left)	|	|	MyPanelMain(Right)	|
//		|						|	|						|
//		|Panel_ST7789(320x240)	|	|Panel_ST7789(320x240)	|
//		+-----------------------+	+-----------------------+
//---
// 物理キー及びジェスチャーセンサー・イベント
//	->詳細は、MyButton.hに記載した。
//---
// 表示パネルのレイアウト遷移図
//												+-------------------------------------------------------+
//												|											   			|PUSH and Standstill
//	+-------------------+			+-------------------+			+-------------------+		+-------------------+		
//	|					|RIGHT->	|(After BOOT)		|	  <-LEFT|					|		|					|
//	|MyLayout::TPMS		|			|MyLayout::Main		|			|MyLayout::Weather	|		|MyLayout::LapTimer	|
//	|					|	  <-LEFT|					|RIGHT->	|					|		|					|
//	+-------------------+			+-------------------+			+-------------------+		+-------------------+
//			|DOWN							|	|DOWN							|DOWN					|
//			|								|	|								|						|
//			|							UP	|	|								|						|
//			|						+-------------------+						|						|
//			+-----------------------|					|-----------------------+						|
//									|MyLayout::Blank	|												|
//									|					|-----------------------------------------------+
//									+-------------------+PUSH and Standstill
//
//--------------------------------------------------------------------------------------------------
BOOL			_MyPanel::QueNextLayout(_MyDevice *pMyDevice, _DrawLayout DrawLayout)
{
	//----------------------------------------------------------------------------------------------
	// 操作系デバイスの状態を取得する
	//----------------------------------------------------------------------------------------------
	UINT	uiGestureDir = pMyDevice->GetButtonInfo(MY_KEYPAD_ALLMASK);

	//----------------------------------------------------------------------------------------------
	// ジェスチャー情報から、パネル表示するレイアウトを取得する
	//----------------------------------------------------------------------------------------------
	switch(DrawLayout) {
		case	_DrawLayout::None:											// メインパネル（全体）	
			this->DrawLayout = _DrawLayout::MainArea;
			break;
		case	_DrawLayout::MainArea:										// メインパネル（全体）	
		case	_DrawLayout::SideTopConsole:								// サイドパネル・上層領域（メッセージ）
		case	_DrawLayout::SideTopMarker:									// サイドパネル・上層領域（ステータス）
		case	_DrawLayout::SideLowArea:									// サイドパネル・下層領域
			if (uiGestureDir == MY_KEYPAD_STICK_DOWN)	this->DrawLayout = _DrawLayout::AllBlank;
			break;
		case	_DrawLayout::AllBlank:										// パネルブランク
		default:
			if (uiGestureDir == MY_KEYPAD_STICK_UP) 	this->DrawLayout = _DrawLayout::MainArea;
			break;
	}
//---
	BOOL	bDrawFlag = MY_FALSE;
	if (this->DrawLayout != DrawLayout) {
		//------------------------------------------------------------------------------------------
		// パネル表示レイアウトが切り替わった際は、時刻を保存
		//------------------------------------------------------------------------------------------
		this->ulPanelChangedTime = millis();
		bDrawFlag = MY_TRUE;
	} else {
		if (this->DrawLayout != _DrawLayout::AllBlank) {
			//--------------------------------------------------------------------------------------
			// 通常描画レイアウトの場合は、スクリーンセイバー・タイムアウト状態を判断する
			//--------------------------------------------------------------------------------------
			if ((millis() - this->ulPanelChangedTime) / 1000 > this->lTFTSleepTime) {
				//----------------------------------------------------------------------------------
				// タイムアウトを検知した場合、パネルブランク状態に自動遷移する
				//----------------------------------------------------------------------------------
				this->DrawLayout = _DrawLayout::AllBlank;
				bDrawFlag = MY_TRUE;
			}
		}
	}
	return	bDrawFlag;
}
/*-- End Of File --*/
