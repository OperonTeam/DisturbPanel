//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/LGFX/MyLGFX.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYLGFX_H
#define MYLGFX_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#define	LGFX_USE_V0
#include <LovyanGFX.h>

//--------------------------------------------------------------------------------------------------
// パネルの描画領域サイズ
//--------------------------------------------------------------------------------------------------
#define MY_PANEL_MAX_WIDTH		320									// 利用パネルの描画最大領域(X)
#define MY_PANEL_MAX_HEIGHT		240									// 利用パネルの描画最大領域(Y)

//--------------------------------------------------------------------------------------------------
// String描画時のFONT指定
//--------------------------------------------------------------------------------------------------
typedef	enum {
	MY_LCDFONT_NON				= -1,
//	MY_LCDFONT_eSPI0			= 1,								// TFT_eSPI互換用
//	MY_LCDFONT_eSPI2			= 2,								// TFT_eSPI互換用	
	MY_LCDFONT_eSPI4			= 3,								// TFT_eSPI互換用
//	MY_LCDFONT_eSPI6			= 4,								// TFT_eSPI互換用（時計用フォント）
	MY_LCDFONT_eSPI7			= 5,								// TFT_eSPI互換用（７セグメント液晶風）
//	MY_LCDFONT_eSPI8			= 6,								// TFT_eSPI互換用（数字のみ）
#ifdef	MY_USE_JAPANESE_FONT
	MY_LCDFONT_eSPIKANJI		= 7									// 拡張日本語
#endif
} _MY_LCD_FONT;
//---
//--------------------------------------------------------------------------------------------------
// カラー描画時のパレット番号指定
// ＠適当に並べたダケなので、試走を含めた評価が必要（特に、速度推移や、タイヤ表面温度に注意）
//--------------------------------------------------------------------------------------------------
enum _MY_PALETTE_COLOR {
	MY_PALETTE_BLACK		= 0,									// Color Depth 2
	MY_PALETTE_YELLOW		= 1,
	MY_PALETTE_DARKGREEN	= 2,
	MY_PALETTE_RED			= 3,
//---
	MY_PALETTE_ORANGE		= 4,									// Color Depth 4
	MY_PALETTE_WHITE		= 5,
	MY_PALETTE_LIGHTGRAY	= 6,
	MY_PALETTE_NAVY			= 7,
	MY_PALETTE_BLUE			= 8,
	MY_PALETTE_GREEN		= 9,
	MY_PALETTE_DARKCYAN		= 10,
	MY_PALETTE_MAROON		= 11,
	MY_PALETTE_PURPLE		= 12,
	MY_PALETTE_OLIVE		= 13,
	MY_PALETTE_DARKGRAY		= 14,
	MY_PALETTE_CYAN			= 15,
//---
	MY_PALETTE_MAGENTA		= 16,									// Color Depth 8
	MY_PALETTE_PINK			= 17,
	MY_PALETTE_GREENYELLOW	= 18,
	MY_PALETTE_BROWN		= 19,
	MY_PALETTE_NONE			= -1,									// 透過色指定無し
};
//---
//--------------------------------------------------------------------------------------------------
// LCDパネル描画時のカラーマップ
// 描画ユーザは、_MY_PALETTE_COLOR::カラーパレット番号を利用すること！
//--------------------------------------------------------------------------------------------------
enum _MY_COLORMAP {
	MY_COLORMAP_BLACK		= 0x000000U,							// TFT_BLACK        |   0,   0,   0 |
	MY_COLORMAP_FAKEBLACK	= 0x000100U,							// -                |   0,   1,   0 |
	MY_COLORMAP_NAVY		= 0x000080U,							// TFT_NAVY         |   0,   0, 128 |
	MY_COLORMAP_DARKGREEN	= 0x008000U,							// TFT_DARKGREEN    |   0, 128,   0 |
	MY_COLORMAP_DARKCYAN	= 0x008080U,							// TFT_DARKCYAN     |   0, 128, 128 |
	MY_COLORMAP_MARON		= 0x800000U,							// TFT_MAROON       | 128,   0,   0 |
	MY_COLORMAP_PURPLE		= 0x800080U,							// TFT_PURPLE       | 128,   0, 128 |
	MY_COLORMAP_OLIVE		= 0x808000U,							// TFT_OLIVE        | 128, 128,   0 |
	MY_COLORMAP_LIGHTGREY	= 0xD3D3D3U,							// TFT_LIGHTGREY    | 211, 211, 211 |
	MY_COLORMAP_DARKGREY	= 0x808080U,							// TFT_DARKGREY     | 128, 128, 128 |
	MY_COLORMAP_BLUE		= 0x0000FFU,							// TFT_BLUE         |   0,   0, 255 |
	MY_COLORMAP_GREEN		= 0x00FF00U,							// TFT_GREEN        |   0, 255,   0 |
	MY_COLORMAP_CYAN		= 0x00FFFFU,							// TFT_CYAN         |   0, 255, 255 |
	MY_COLORMAP_RED			= 0xFF0000U,							// TFT_RED          | 255,   0,   0 |
	MY_COLORMAP_MAGENTA		= 0xFF00FFU,							// TFT_MAGENTA      | 255,   0, 255 |
	MY_COLORMAP_YELLOW		= 0xFFFF00U,							// TFT_YELLOW       | 255, 255,   0 |
	MY_COLORMAP_WHITE		= 0xFFFFFFU,							// TFT_WHITE        | 255, 255, 255 |
	MY_COLORMAP_ORANGE		= 0xFFF000U,							// TFT_ORANGE       | 255, 180,   0 |
	MY_COLORMAP_GREENYELLOW	= 0xF0FF00U,							// TFT_GREENYELLOW  | 180, 255,   0 |
	MY_COLORMAP_PINK		= 0xFFC0CBU,							// TFT_PINK         | 255, 192, 203 |
	MY_COLORMAP_BROWN		= 0x009A60U								// TFT_BROWN        | 150,  75,   0 |
};
//
//--------------------------------------------------------------------------------------------------
// LovyanGFX SPI Busコントロール用の構造体
//--------------------------------------------------------------------------------------------------
typedef struct _MyLgfxControl {
	MY_PRIVATE constexpr INT sercom_index			= MYSPI_PANEL_SERCOM_INDEX;
	MY_PRIVATE constexpr INT sercom_clksrc			= MYSPI_PANEL_SERCOM_CLKSRC;
	MY_PRIVATE constexpr INT sercom_clkfreq			= MYSPI_PANEL_SERCOM_CLKFREQ;
	MY_PRIVATE constexpr INT spi_miso				= MYSPI_PANEL_SPI_MISO;
	MY_PRIVATE constexpr INT spi_mosi				= MYSPI_PANEL_SPI_MOSI;
	MY_PRIVATE constexpr INT spi_sclk				= MYSPI_PANEL_SPI_SCLK;
	MY_PRIVATE constexpr SercomSpiTXPad	pad_mosi	= MYSPI_PANEL_PAD_MOSI;
	MY_PRIVATE constexpr SercomRXPad pad_miso		= MYSPI_PANEL_PAD_MISO;
};
//
//--------------------------------------------------------------------------------------------------
// 情報表示パネル用基底クラス
//--------------------------------------------------------------------------------------------------
class   _MyLGFX {
	private:
		//------------------------------------------------------------------------------------------
		// 表示パネル・オブジェクト
		//------------------------------------------------------------------------------------------
		LGFX_SPI<_MyLgfxControl>	MyLcd;							// 
		lgfx::Panel_ST7789			MyPanel;						// パネル・実態
		LGFX_Sprite					MyLgfxSprite;					// オフスクリーン描画用オブジェクト
//---
	public:
		//------------------------------------------------------------------------------------------
		// メインどころのメソッド
		//------------------------------------------------------------------------------------------
		_MyLGFX();
		~_MyLGFX();
//
		//------------------------------------------------------------------------------------------
		// ＧＦＸ描画ペン・アクセスメソッド
		//------------------------------------------------------------------------------------------
		// 	+---------------------------------------------------------------+
		//	| 生成パネル種別情報[Flexibility]					 			|
		//	|	・メイン	:	Main				 						|
		//	|	・サイド	:	Side				 						|
		// 	+---------------------------------------------------------------+
		enum	_PanelType {										// 生成パネル種別
			PanelMain	= 0,										// 　メイン・パネル
			PanelSide	= 1											// 　サイド・パネル
		};
		VOID					LcdSetup(_PanelType, INT iRotateMode);
		LovyanGFX*				GetLcdHandle(VOID);
		VOID					LcdTest(VOID);
		VOID					LcdSetConfig(_MY_LCD_FONT _MyFontID, USHORT _usColorFG, USHORT _usColorBG = TFT_BLACK, FLOAT _fWMag = 1.0, FLOAT _fHMag = 1.0, textdatum_t TextDatum = textdatum_t::bottom_left);
		VOID					SpriteDrawStringFormat(STRING strDrawString, INT iX, INT iY, _MY_LCD_FONT _MyFontID, USHORT _usColorFG = MY_PALETTE_YELLOW, FLOAT _fWMag = 1.0, FLOAT _fHMag = 1.0, textdatum_t TextDatum = textdatum_t::bottom_left);
		//----
		//------------------------------------------------------------------------------------------
		// Ｌｃｄ描画・インライン・アクセスメソッド（ラッパー）
		//------------------------------------------------------------------------------------------
		INLINE	VOID			LcdClear(VOID) {
									this->MyLcd.clear();
		};
		INLINE	VOID			LcdFillScreen(INT iDrawColor) {
									this->MyLcd.fillScreen(iDrawColor);
		};

		//------------------------------------------------------------------------------------------
		// Ｓｐｒｉｔｅ描画・アクセスメソッド
		//------------------------------------------------------------------------------------------
		VOID					SpriteCreate(INT iWidth, INT iHeight, INT iColorDepth, BOOL bColorAutoPalette = MY_TRUE);
		VOID					SpriteSetConfig(_MY_LCD_FONT _MyFontID, USHORT _usColorFG, USHORT _usColorBG = TFT_BLACK, FLOAT _fWMag = 1.0, FLOAT _fHMag = 1.0, textdatum_t TextDatum = textdatum_t::bottom_left);
		VOID					SpritePush(INT iXPos, INT iYPos, BOOL bSpriteDeleteFlag, INT iTranspareColor = -1);
		//---
		//------------------------------------------------------------------------------------------
		// Ｓｐｒｉｔｅ描画・インライン・アクセスメソッド（ラッパー）
		//
		//	角度指定：SpriteFillArc()dえ、開始・終了角度を指定する場合、時計回りで指定する
		//			270	
		//			|
		//			|
		// 180 -----+----- 0(360)
		//			|
		//			|
		//			90
		//------------------------------------------------------------------------------------------
		INLINE	INT				SpriteTextWidth(String strText) {
									return	this->MyLgfxSprite.textWidth(strText);
		};
		INLINE	VOID			SpriteClearDisplay(INT iPaletteNo) {
									this->MyLgfxSprite.clearDisplay(iPaletteNo);
		};
		INLINE	VOID			SpriteSetClipRect(INT iX1, INT iY1, INT iWidth, INT iHeight) {
									this->MyLgfxSprite.setClipRect(iX1, iY1, iWidth, iHeight);
		};
		INLINE	VOID			SpriteDrawPixel(INT iXPos, INT iYPos, INT iPaletteNo) {
									this->MyLgfxSprite.drawPixel(iXPos, iYPos, iPaletteNo);
		};
		INLINE	VOID			SpriteDrawJpgFile(fs::FS &fs, CONST_STRING& Path, INT iX, INT iY) {
									this->MyLgfxSprite.drawJpgFile(fs, Path, iX, iY);
		};
		INLINE	VOID			SpriteDrawFastVLine(INT iX, INT iY, INT iHeight, INT iPaletteNo) {
									this->MyLgfxSprite.drawFastVLine(iX, iY, iHeight, iPaletteNo);
		};
		INLINE	VOID			SpriteDrawFastHLine(INT iX, INT iY, INT iWidth, INT iPaletteNo) {
									this->MyLgfxSprite.drawFastHLine(iX, iY, iWidth, iPaletteNo);
		};
		INLINE	VOID			SpriteDrawNumber(LONG lDrawNumber, INT iX, INT iY) {
									this->MyLgfxSprite.drawNumber(lDrawNumber, iX, iY);
		};
		INLINE	VOID			SpriteDrawFloat(FLOAT fDrawNumber, INT iColum, INT iX, INT iY) {
									this->MyLgfxSprite.drawFloat(fDrawNumber, iColum, iX, iY);
		};
		INLINE	VOID			SpriteDrawLine(INT iX1, INT iY1, INT iX2, INT iY2, INT iPaletteNo) {
									this->MyLgfxSprite.drawLine(iX1, iY1, iX2, iY2, iPaletteNo);
		};
		INLINE	VOID			SpriteDrawRect(INT iX, INT iY, INT iWidth, INT iHeight, INT iPaletteNo) {
									this->MyLgfxSprite.drawRect(iX, iY, iWidth, iHeight, iPaletteNo);
		};
		INLINE	VOID			SpriteDrawArc(INT iX, INT iY, INT iR, INT iR1, INT iStartVect, INT iEndVect, INT iPaletteNo) {
									this->MyLgfxSprite.drawArc(iX, iY, iR, iR1, iStartVect, iEndVect, iPaletteNo);
		};
		INLINE	VOID			SpriteFillArc(INT iX, INT iY, INT iR, INT iR1, INT iStartVect, INT iEndVect, INT iPaletteNo) {
									this->MyLgfxSprite.fillArc(iX, iY, iR, iR1, iStartVect, iEndVect, iPaletteNo);
		};
		INLINE	VOID			SpriteFillRect(INT iX, INT iY, INT iWidth, INT iHeight, INT iPaletteNo) {
									this->MyLgfxSprite.fillRect(iX, iY, iWidth, iHeight, iPaletteNo);
		};
		INLINE	VOID			SpriteDrawCircle(INT iX, INT iY, INT iR, INT iPaletteNo) {
									this->MyLgfxSprite.drawCircle(iX, iY, iR, iPaletteNo);
		};
		INLINE	VOID			SpriteFillCircle(INT iX, INT iY, INT iR, INT iPaletteNo) {
									this->MyLgfxSprite.fillCircle(iX, iY, iR, iPaletteNo);
		};
		INLINE	VOID			SpriteDrawEllipse(INT iX, INT iY, INT iRX, INT iRY, INT iPaletteNo) {
									this->MyLgfxSprite.drawEllipse(iX, iY, iRX, iRY, iPaletteNo);
		};
		INLINE	VOID			SpriteFillEllipse(INT iX, INT iY, INT iRX, INT iRY, INT iPaletteNo) {
									this->MyLgfxSprite.fillEllipse(iX, iY, iRX, iRY, iPaletteNo);
		};
		INLINE	VOID			SpriteFillTriangle(INT iX1, INT iY1, INT iX2, INT iY2, INT iX3, INT iY3, INT iPaletteNo) {
									this->MyLgfxSprite.fillTriangle(iX1, iY1, iX2, iY2, iX3, iY3, iPaletteNo);
		};
};
#endif
/*-- End Of File --*/
