//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/LGFX/MyLGFX.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
//#include "MyDevice.h"
//#include "MyJARVis.h"
#include "MyLGFX.h"

//--------------------------------------------------------------------------------------------------
// クラスインスタンスの初期処理
//--------------------------------------------------------------------------------------------------
_MyLGFX::_MyLGFX()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの終了処理
//--------------------------------------------------------------------------------------------------
_MyLGFX::~_MyLGFX()
{
}
/**/
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
VOID				_MyLGFX::LcdSetup(
_PanelType			PanelType,
INT					iRotateMode)												// パネル描画（回転：配置方向）
{
	//----------------------------------------------------------------------------------------------
	// 拡張パネル（メインパネル）の初期化を行う
	//----------------------------------------------------------------------------------------------
	this->MyPanel.freq_write		= MYSPI_PANEL_FREQ_WRITE;
	this->MyPanel.freq_fill			= MYSPI_PANEL_FREQ_FILL;
	this->MyPanel.freq_read			= MYSPI_PANEL_FREQ_READ;
	this->MyPanel.memory_height		= MYSPI_PANEL_MEMORY_HEIGHT;
	this->MyPanel.spi_dc			= MYSPI_PANEL_SPI_DC;
//---
	//----------------------------------------------------------------------------------------------
	// 生成を行うパネルの種別は？
	//----------------------------------------------------------------------------------------------
	if (PanelType == _MyLGFX::_PanelType::PanelMain) {
		this->MyPanel.spi_cs		= MYSPI_PANEL_A_SPI_CS;								// メインパネル
		this->MyPanel.gpio_rst		= MYSPI_PANEL_A_GPIO_RST;							// メインパネル
	} else {
		this->MyPanel.spi_cs		= MYSPI_PANEL_B_SPI_CS;								// サイドパネル
		this->MyPanel.gpio_rst		= MYSPI_PANEL_B_GPIO_RST;							// サイドパネル
	}
//---
	this->MyPanel.reverse_invert	= MYSPI_PANEL_REVERSE_INVERT;
	this->MyPanel.rotation			= iRotateMode;
//---
	//----------------------------------------------------------------------------------------------
	// バックライト制御の設定を行う。
	// 本来はパネル毎に設定可能だが、今回は同一信号線で結線したのでここは操作不要）
	//----------------------------------------------------------------------------------------------
	this->MyPanel.gpio_bl			= /*lgfx::samd51::PORT_B |*/ MYSPI_PANEL_BACKLIGHT;	// backlight pin number
    this->MyPanel.pwm_ch_bl			= /*lgfx::samd51::PORT_B |*/ MYSPI_PANEL_BACKLIGHT;	// backlight PWM channel number
	this->MyPanel.backlight_level	= true;
//---
	//----------------------------------------------------------------------------------------------
	// パネル制御のピン情報をダンプ
	//----------------------------------------------------------------------------------------------
#ifndef	MY_SPI_PANEL_DONOT_TEST
	MY_LOG("MISO = [%4x]\n", ((g_APinDescription[PIN_SPI_MISO].ulPort << 8)		| g_APinDescription[PIN_SPI_MISO].ulPin));	// 0x0100
	MY_LOG("MOSI = [%4x]\n", ((g_APinDescription[PIN_SPI_MOSI].ulPort << 8)		| g_APinDescription[PIN_SPI_MOSI].ulPin));	// 0x0102
	MY_LOG("SCLK = [%4x]\n", ((g_APinDescription[PIN_SPI_SCK].ulPort << 8)		| g_APinDescription[PIN_SPI_SCK].ulPin));	// 0x0103
	MY_LOG("--\n");
	MY_LOG("CS   = [%4x]\n", ((g_APinDescription[PIN_SPI_SS].ulPort << 8)		| g_APinDescription[PIN_SPI_SS].ulPin));	// 0x0101
	MY_LOG("DC-D3= [%4x]\n", ((g_APinDescription[D3].ulPort << 8)				| g_APinDescription[D3].ulPin));			// 0x0104
	MY_LOG("RSTD4= [%4x]\n", ((g_APinDescription[D4].ulPort << 8)				| g_APinDescription[D4].ulPin));			// 0x0105
	MY_LOG("BL-D6= [%4x]\n", ((g_APinDescription[D6].ulPort << 8)				| g_APinDescription[D6].ulPin));			// 0x0004
	MY_LOG("--\n");
	MY_LOG("CS 2 = [%4x]\n", ((g_APinDescription[D7].ulPort << 8)				| g_APinDescription[D7].ulPin));			// 0x0107
	MY_LOG("RSTD5= [%4x]\n", ((g_APinDescription[D5].ulPort << 8)				| g_APinDescription[D5].ulPin));			// 0x0106
#endif
//---
#ifdef	MY_NEWGFX_INTERNAL_PANEL
	//----------------------------------------------------------------------------------------------
	// Wioterminal内蔵パネルを誤って割ってしまったので、残念ながら使わないロジック、、、、。
	//----------------------------------------------------------------------------------------------
	MyPanelB.spi_cs		= 0x0115;		// PORTB 21  (PORTB=0x0100 | 21=0x0015)
	MyPanelB.spi_dc		= 0x0206;		// PORTC  6  (PORTC=0x0200 |  6=0x0006)
	MyPanelB.gpio_rst	= 0x0207;		// PORTC  7  (PORTC=0x0200 |  7=0x0007)
	MyPanelB.gpio_bl	= 0x0205;		// PORTC  5  (PORTC=0x0200 |  5=0x0005)
	MyPanelB.freq_fill	= 75000000;
	MyPanelB.freq_write	= 60000000;
	MyPanelB.freq_read	= 20000000;
	MyPanelB.rotation	= 1;
	MyLcdB.setPanel(&MyPanelB);
	MyLcdB.init();
	this->MySidePanel.Begin(&MyLcdB);
#endif
//---
	//----------------------------------------------------------------------------------------------
	// Wioterminal内蔵パネルを誤って割ってしまったので、残念ながら使わないロジック、、、、。
	//----------------------------------------------------------------------------------------------
	this->MyLcd.setPanel(&(this->MyPanel));
	this->MyLcd.init();
//
	//----------------------------------------------------------------------------------------------
	// パネルを初期化する
	//----------------------------------------------------------------------------------------------
	this->MyLcd.setRotation(iRotateMode);										// パネル描画（回転：配置方向）
	this->LcdSetConfig(MY_LCDFONT_eSPI4, TFT_YELLOW, TFT_BLACK, 1.0, 1.0, textdatum_t::bottom_right);
	this->MyLcd.setCursor(0, 0);												// カーソル位置を初期化
	this->MyLcd.clearDisplay(TFT_BLACK);										// パネル初期化時の描画・背景色
	this->MyLcd.setTextWrap(MY_FALSE);											// Warp Mode Off
	this->MyLcd.setColorDepth(8);												// 色深度
//---
#ifndef	MY_SPI_PANEL_DONOT_TEST
	//----------------------------------------------------------------------------------------------
	// パネルのセルフテストを行う
	//----------------------------------------------------------------------------------------------
	this->MyPanelUtilSide.LcdTest();
#endif
}
/**/
//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
LovyanGFX*			_MyLGFX::GetLcdHandle(VOID)
{
	return	&(this->MyLcd);
}
/**/
//--------------------------------------------------------------------------------------------------
// オフスクリーン領域の確保と、カラーパレットの登録を行います
//--------------------------------------------------------------------------------------------------
VOID				_MyLGFX::SpriteCreate(
INT					iWidth,
INT					iHeight,
INT					iColorDepth,
BOOL				bColorAutoPalette)
{
	//----------------------------------------------------------------------------------------------
	// オフスクリーン・バッファ領域を作成する
	//----------------------------------------------------------------------------------------------
	this->MyLgfxSprite.setColorDepth(iColorDepth);													// ビット数で色深度を設定する
	this->MyLgfxSprite.createPalette();																// ビット数で色深度を設定する
	this->MyLgfxSprite.createSprite(iWidth, iHeight);												// iWidth X iHeight
//--
	//----------------------------------------------------------------------------------------------
	// カラー・パレットの登録を行う
	//----------------------------------------------------------------------------------------------
	if (bColorAutoPalette == MY_TRUE) {
		//------------------------------------------------------------------------------------------
		// カラー・パレットを色深度に合わせて自動設定する
		// 少し嫌らしい実装だけれど、iColorDepth = 8の場合はパレットを利用しない。
		//------------------------------------------------------------------------------------------
		if (iColorDepth >= 8) {
			this->MyLgfxSprite.deletePalette();
		} else {
			if (iColorDepth >= 1) {
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_BLACK,	_MY_COLORMAP::MY_COLORMAP_BLACK);		// Palette 0
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_YELLOW, 	_MY_COLORMAP::MY_COLORMAP_YELLOW);		// Palette 1
			}
			//--
			if (iColorDepth >= 2) {
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_DARKGREEN,_MY_COLORMAP::MY_COLORMAP_DARKGREEN);	// Palette 2
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_RED,		_MY_COLORMAP::MY_COLORMAP_RED);			// Palette 3
			}
			//--
			if (iColorDepth >= 4) {
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_ORANGE,	_MY_COLORMAP::MY_COLORMAP_GREENYELLOW);	// Palette 4
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_WHITE,	_MY_COLORMAP::MY_COLORMAP_WHITE);		// Palette 5
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_LIGHTGRAY,_MY_COLORMAP::MY_COLORMAP_LIGHTGREY);	// Palette 6
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_NAVY,		_MY_COLORMAP::MY_COLORMAP_NAVY);		// Palette 7
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_BLUE,		_MY_COLORMAP::MY_COLORMAP_BLUE);		// Palette 8
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_GREEN,	_MY_COLORMAP::MY_COLORMAP_GREEN);		// Palette 9
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_DARKCYAN,	_MY_COLORMAP::MY_COLORMAP_DARKCYAN);	// Palette 10
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_MAROON,	_MY_COLORMAP::MY_COLORMAP_MARON);		// Palette 11
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_PURPLE,	_MY_COLORMAP::MY_COLORMAP_PURPLE);		// Palette 12
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_OLIVE,	_MY_COLORMAP::MY_COLORMAP_OLIVE);		// Palette 13
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_DARKGRAY,	_MY_COLORMAP::MY_COLORMAP_DARKGREY);	// Palette 14
				this->MyLgfxSprite.setPaletteColor(MY_PALETTE_CYAN,		_MY_COLORMAP::MY_COLORMAP_CYAN);		// Palette 15
			}
		}
	} else {
		//------------------------------------------------------------------------------------------
		// カラー・パレットを手動設定したいのだけれど、サーモにしか使ってないから結局決め打ちです。
		// AMG8833センサー・プログラムを公開している、"のるLAB"さん設定が良い感じで、そのまま使っています。
		// -> src/MyTask/MyI2C/AMG8833を参照願います！
		//------------------------------------------------------------------------------------------
		this->MyLgfxSprite.setPaletteColor( 0,   0,   0, 192);
		this->MyLgfxSprite.setPaletteColor( 1,   0,  48, 160);
		this->MyLgfxSprite.setPaletteColor( 2,  48,  76, 176);
		this->MyLgfxSprite.setPaletteColor( 3,  80, 128, 192);
		this->MyLgfxSprite.setPaletteColor( 4, 112, 160, 208);
		this->MyLgfxSprite.setPaletteColor( 5, 144, 176, 224);
		this->MyLgfxSprite.setPaletteColor( 6, 160, 224, 240);
		this->MyLgfxSprite.setPaletteColor( 7, 192, 255, 255);
		this->MyLgfxSprite.setPaletteColor( 8, 255, 255, 112);
		this->MyLgfxSprite.setPaletteColor( 9, 255, 192,   0);
		this->MyLgfxSprite.setPaletteColor(10, 255, 144,   0);
		this->MyLgfxSprite.setPaletteColor(11, 240, 112,   0);
		this->MyLgfxSprite.setPaletteColor(12, 240,  80,   0);
		this->MyLgfxSprite.setPaletteColor(13, 208,  48,   0);
		this->MyLgfxSprite.setPaletteColor(14, 160,   0,   0);
		this->MyLgfxSprite.setPaletteColor(15, 255,   0,   0);
	}
}
/**/
//	MyPanelB.drawRect(69, 0+17,		 	 60, 18, GxEPD_BLACK);		MyPanelB.setCursor(70, 14+17);			MyPanelB.printf("  12");	// Weather 気温(℃)
/**/
//
//
//
VOID				_MyLGFX::SpriteDrawStringFormat(
STRING				strDrawString,
INT					iX,
INT					iY,
_MY_LCD_FONT		_MyFontID,
USHORT				_usColorFG,
FLOAT				_fWMag,
FLOAT				_fHMag,
textdatum_t			_TextDatum)
{
	//----------------------------------------------------------------------------------------------
	// 描画FONT設定
	// 利用可能な日本語FONTの詳細は、LovyanGFXを見ること。以下抜粋
	// IPAフォントをコンバートした日本語フォントが４種類ｘ９サイズ = 36通りプリセットされています。
	// 末尾の数字がサイズを表しており、8, 12, 16, 20, 24, 28, 32, 36, 40 が用意されています。
	//----------------------------------------------------------------------------------------------
	switch (_MyFontID) {
//		case	MY_LCDFONT_eSPI0:		this->MyLgfxSprite.setFont(&fonts::Font0);				break;
//		case	MY_LCDFONT_eSPI2:		this->MyLgfxSprite.setFont(&fonts::Font2);				break;
		case	MY_LCDFONT_eSPI4:		this->MyLgfxSprite.setFont(&fonts::Font4);				break;
//		case	MY_LCDFONT_eSPI6:		this->MyLgfxSprite.setFont(&fonts::Font6);				break;
		case	MY_LCDFONT_eSPI7:		this->MyLgfxSprite.setFont(&fonts::Font7);				break;
//		case	MY_LCDFONT_eSPI8:		this->MyLgfxSprite.setFont(&fonts::Font8);				break;
#ifdef	MY_USE_JAPANESE_FONT
		case	MY_LCDFONT_eSPIKANJI:	this->MyLgfxSprite.setFont(&fonts::lgfxJapanGothic_24);	break;
#endif
		default:
			break;
	}

	//----------------------------------------------------------------------------------------------
	// 描画COLOR設定
	//----------------------------------------------------------------------------------------------
	this->MyLgfxSprite.setTextColor(_usColorFG, MY_PALETTE_BLACK);

	//----------------------------------------------------------------------------------------------
	// 描画時のテキスト表示位置設定
	//----------------------------------------------------------------------------------------------
	this->MyLgfxSprite.setTextDatum(_TextDatum);

	//----------------------------------------------------------------------------------------------
	// 描画時のテキスト倍率設定
	//----------------------------------------------------------------------------------------------
	this->MyLgfxSprite.setTextSize(_fWMag, _fHMag);

	//----------------------------------------------------------------------------------------------
	// 文字列を描画
	//----------------------------------------------------------------------------------------------
	this->MyLgfxSprite.drawString(strDrawString, iX, iY);
}
//---
//--------------------------------------------------------------------------------------------------
// テキスト描画環境設定（ＬＧＦＸ＿Ｓｐｒｉｔｅ）
//--------------------------------------------------------------------------------------------------
VOID				_MyLGFX::SpriteSetConfig(
_MY_LCD_FONT		_MyFontID,
USHORT				_usColorFG,
USHORT				_usColorBG,
FLOAT				_fWMag,
FLOAT				_fHMag,
textdatum_t			_TextDatum)
{
	//----------------------------------------------------------------------------------------------
	// 描画FONT設定
	//
	// 利用可能な日本語FONTの詳細は、LovyanGFXを見ること。以下抜粋
	// IPAフォントをコンバートした日本語フォントが４種類ｘ９サイズ = 36通りプリセットされています。
	// 末尾の数字がサイズを表しており、8, 12, 16, 20, 24, 28, 32, 36, 40 が用意されています。
	//								Program FLASH size
	// Non KanjiFont				41.6% (used 211088 bytes from 507904 bytes)
	// fonts::lgfxJapanMincho_12	64.6% (used 328304 bytes from 507904 bytes)
	// fonts::lgfxJapanMinchoP_16   73.9% (used 375464 bytes from 507904 bytes)
	// fonts::lgfxJapanGothic_20 	84.6% (used 429832 bytes from 507904 bytes)
	// fonts::lgfxJapanGothicP_24   96.3% (used 489264 bytes from 507904 bytes)
	//
	// 20 Flash: [========= ]  88.5% (used 449696 bytes from 507904 bytes)
	// 24 Flash: [==========]  99.2% (used 503720 bytes from 507904 bytes)
	//----------------------------------------------------------------------------------------------
	switch (_MyFontID) {
//		case	MY_LCDFONT_eSPI0:		this->MyLgfxSprite.setFont(&fonts::Font0);					break;
//		case	MY_LCDFONT_eSPI2:		this->MyLgfxSprite.setFont(&fonts::Font2);					break;
		case	MY_LCDFONT_eSPI4:		this->MyLgfxSprite.setFont(&fonts::Font4);					break;
//		case	MY_LCDFONT_eSPI6:		this->MyLgfxSprite.setFont(&fonts::Font6);					break;
		case	MY_LCDFONT_eSPI7:		this->MyLgfxSprite.setFont(&fonts::Font7);					break;
//		case	MY_LCDFONT_eSPI8:		this->MyLgfxSprite.setFont(&fonts::Font8);					break;
#ifdef	MY_USE_JAPANESE_FONT
		case	MY_LCDFONT_eSPIKANJI:	this->MyLgfxSprite.setFont(&fonts::lgfxJapanGothic_24);		break;
#endif
		default:
			break;
	}

	//----------------------------------------------------------------------------------------------
	// 描画COLOR設定
	//----------------------------------------------------------------------------------------------
	this->MyLgfxSprite.setColor(_usColorFG);
	this->MyLgfxSprite.setTextColor(_usColorFG, _usColorBG);

	//----------------------------------------------------------------------------------------------
	// 描画時のテキスト表示位置設定
	//----------------------------------------------------------------------------------------------
	this->MyLgfxSprite.setTextDatum(_TextDatum);

	//----------------------------------------------------------------------------------------------
	// 描画時のテキスト倍率設定
	//----------------------------------------------------------------------------------------------
		this->MyLgfxSprite.setTextSize(_fWMag, _fHMag);
//	}
}
/**/
//--------------------------------------------------------------------------------------------------
// オフスクリーンバッファをＬＣＤパネルに出力（描画）する（ＬＧＦＸ＿Ｓｐｒｉｔｅ）
//--------------------------------------------------------------------------------------------------
VOID				_MyLGFX::SpritePush(
INT					iXPos,
INT					iYPos,
BOOL				bSpriteDeleteFlag,
INT					iTranspareColor)
{
	//----------------------------------------------------------------------------------------------
	// オフスクリーンバッファに生成した領域を、ＬＣＤパネルに出力する
	//----------------------------------------------------------------------------------------------
	if (iTranspareColor < 0) {
		//------------------------------------------------------------------------------------------
		// 描画：透過色指定無し
		//------------------------------------------------------------------------------------------
		this->MyLgfxSprite.pushSprite(&(this->MyLcd), iXPos, iYPos);
	} else {
		//------------------------------------------------------------------------------------------
		// 描画：透過色指定あり
		//------------------------------------------------------------------------------------------
		this->MyLgfxSprite.pushSprite(&(this->MyLcd), iXPos, iYPos, iTranspareColor);
	}
//---
	//----------------------------------------------------------------------------------------------
	// 描画したオフスクリーンバッファ解放が指示されているか？（デフォルト＝削除指示）
	//----------------------------------------------------------------------------------------------
	if (bSpriteDeleteFlag == MY_TRUE) {
		this->MyLgfxSprite.deleteSprite();
	}
}
/**/
/**/
//--------------------------------------------------------------------------------------------------
// テキスト描画環境設定（ＬＧＦＸ＿Ｄｅｖｉｃｅ）
//--------------------------------------------------------------------------------------------------
VOID				_MyLGFX::LcdSetConfig(
_MY_LCD_FONT		_MyFontID,
USHORT				_usColorFG,
USHORT				_usColorBG,
FLOAT				_fWMag,
FLOAT				_fHMag,
textdatum_t			_TextDatum)
{
	//----------------------------------------------------------------------------------------------
	// 描画FONT設定
	//----------------------------------------------------------------------------------------------
	switch (_MyFontID) {
//--	case	MY_LCDFONT_eSPI0:		this->MyLcd.setFont(&fonts::Font0);					break;
//--	case	MY_LCDFONT_eSPI2:		this->MyLcd.setFont(&fonts::Font2);					break;
		case	MY_LCDFONT_eSPI4:		this->MyLcd.setFont(&fonts::Font4);					break;
//--	case	MY_LCDFONT_eSPI6:		this->MyLcd.setFont(&fonts::Font6);					break;
		case	MY_LCDFONT_eSPI7:		this->MyLcd.setFont(&fonts::Font7);					break;
//--	case	MY_LCDFONT_eSPI8:		this->MyLcd.setFont(&fonts::Font8);					break;
#ifdef	MY_USE_JAPANESE_FONT
		case	MY_LCDFONT_eSPIKANJI:	this->MyLcd.setFont(&fonts::lgfxJapanGothic_24);	break;
#endif
		default:
			break;
	}
	//----------------------------------------------------------------------------------------------
	// 描画COLOR設定
	//----------------------------------------------------------------------------------------------
	this->MyLcd.setColor(_usColorFG);
	this->MyLcd.setTextColor(_usColorFG, _usColorBG);

	//----------------------------------------------------------------------------------------------
	// 描画時のテキスト表示位置設定
	//----------------------------------------------------------------------------------------------
	this->MyLcd.setTextDatum(_TextDatum);

	//----------------------------------------------------------------------------------------------
	// 描画時のテキスト倍率設定
	//----------------------------------------------------------------------------------------------
	this->MyLcd.setTextSize(_fWMag, _fHMag);
}
/**/
//--------------------------------------------------------------------------------------------------
// パネルの表示テストを行う（LovyanGFX : 2_spi_settingのまま）
//--------------------------------------------------------------------------------------------------
VOID				_MyLGFX::LcdTest(VOID)
{
	//----------------------------------------------------------------------------------------------
	// パネルを初期化して、現在回転方向を取得
	//----------------------------------------------------------------------------------------------
	this->MyLcd.clear();										// 表示を初期化
	INT	iRotateANow = this->MyLcd.getRotation();

	//----------------------------------------------------------------------------------------------
	// パネルの表示テストを行う（LovyanGFX : 2_spi_settingのまま）
	//----------------------------------------------------------------------------------------------
	for (INT iRotate = 0; iRotate < 3; iRotate++) {
		this->MyLcd.setRotation(iRotate);						// パネルの回転方向を設定する
		random(0xFFFFFF);
		for (INT iX = 0; iX < this->MyLcd.height(); iX++) {
			this->MyLcd.drawGradientLine( 0, 0, this->MyLcd.width(), iX, rand(), rand());
		}
	}
	//----------------------------------------------------------------------------------------------
	// パネルの回転方向を復元する
	//----------------------------------------------------------------------------------------------
	this->MyLcd.setRotation(iRotateANow);						// パネルの回転方向を元に戻す
}
//
/*-- End Of File --*/
