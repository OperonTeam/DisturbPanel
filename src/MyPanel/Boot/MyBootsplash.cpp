//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/Boot/MyBootsplash.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MyBootsplash.h"

//--------------------------------------------------------------------------------------------------
// クラスインスタンスの初期処理
//--------------------------------------------------------------------------------------------------
_MyBootsplash::_MyBootsplash()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// クラスインスタンスの終了処理
//--------------------------------------------------------------------------------------------------
_MyBootsplash::~_MyBootsplash()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// メインパネル・起動ロゴを描画する
//	タスク化する前の状態で、Mainから呼び出されます。
//	---
//	＜要注意事項＞
//	起動時のオープンイングスプラッシュ描画では、メインパネル（右側）からサイドパネル（左側）に、
//	連続してアイコンイメージを連続移動させています。
//	実装が面倒なので（このメソッドだけ）、メイン・サイドのパネル毎に隠蔽している_LGFX_Spriteを共用
//	利用している都合上、書きっぷりが異なるので要注意。
//--------------------------------------------------------------------------------------------------
VOID				_MyBootsplash::DrawBootLogo(
_MyLGFX				*pMyLGFXMain,
_MyLGFX				*pMyLGFXSide)
{
	//----------------------------------------------------------------------------------------------
	// メイン・サイドパネルを初期化する
	//----------------------------------------------------------------------------------------------
	pMyLGFXMain->LcdFillScreen(TFT_BLACK);
	pMyLGFXSide->LcdFillScreen(TFT_BLACK);
//---
	//----------------------------------------------------------------------------------------------
	// オフ・スクリーン用描画情報を作成する（左側：サイドパネル／右側：メインパネル）
	//----------------------------------------------------------------------------------------------
	LGFX_Sprite		MySprite;
	MySprite.setColorDepth(8);
	MySprite.createSprite(MY_PANEL_MAX_WIDTH, MY_PANEL_MAX_HEIGHT);
	//---
	//----------------------------------------------------------------------------------------------
	// オフ・スクリーン用描画情報（パネル１枚分）を、右側パネルから左側パネルまで画像イメージをスクロール描画する
	//	スクロール完了時、右（メイン）パネルはブランク(TFT_BLACK)状態
	//----------------------------------------------------------------------------------------------
	MySprite.drawJpgFile(MYDEV_SD, MY_SIDEPANELBOOT_HAYABUSALOGO_PIC, 0, 0);
	for (INT iX = 640; iX >= 0; iX -= 6) {
		MySprite.pushSprite(pMyLGFXMain->GetLcdHandle(), iX - MY_PANEL_MAX_WIDTH,	0);		// +320 -> -320
		MySprite.pushSprite(pMyLGFXSide->GetLcdHandle(), iX,						0);		// +640 -> 0
	}
	//---
	MySprite.clearDisplay(TFT_BLACK);
	MySprite.drawJpgFile(MYDEV_SD, MY_MAINPANELBOOT_OWNERSBIKE_PIC, 0, 0);
	MySprite.pushSprite(pMyLGFXMain->GetLcdHandle(), 0, 0);
//
	delay(3000);
//
	//----------------------------------------------------------------------------------------------
	// 右（メイン）パネルに、ラインセス画像と車両メンテナンスのロゴ画像を描画する
	//----------------------------------------------------------------------------------------------
	MySprite.clearDisplay(TFT_BLACK);
	MySprite.drawJpgFile(MYDEV_SD, MY_MAINPANELBOOT_OWNERLICENSE_PIC, MY_MAINPANELBOOT_OWNERLICENSE_X, MY_MAINPANELBOOT_OWNERLICENSE_Y);
	MySprite.drawJpgFile(MYDEV_SD, MY_MAINPANELBOOT_SUZUKINWORLD_PIC, MY_MAINPANELBOOT_SUZUKINWORLD_X, MY_MAINPANELBOOT_SUZUKINWORLD_Y);
	MySprite.pushSprite(pMyLGFXMain->GetLcdHandle(), 0, 0);
	//---
	//----------------------------------------------------------------------------------------------
	// オフ・スクリーン用描画情報を解放する
	//----------------------------------------------------------------------------------------------
	MySprite.deleteSprite();
//---
	//----------------------------------------------------------------------------------------------
	// この状態で、パネル状態をホールドする
	//----------------------------------------------------------------------------------------------
	delay(3000);
}
/**/
//--------------------------------------------------------------------------------------------------
// サイドパネル・起動状態を描画する
//	この描画オブジェクトはあまり嬉しくないので、前面書き直しするけれど良いアイデアが無い。
//	所謂工業系デザイナーってどんな感じなんだろう？
// OperonTeamロゴ	　：160x47
//--------------------------------------------------------------------------------------------------
BOOL				_MyBootsplash::DrawBooting(
_MyLGFX				*pMyLGFXMain,
_MyLGFX				*pMyLGFXSide,
_MyDevice			*pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// サイドパネルを初期化する
	//----------------------------------------------------------------------------------------------
	pMyLGFXSide->LcdFillScreen(TFT_WHITE);
//---
	//----------------------------------------------------------------------------------------------
	// サイドパネル（上部）デバイス状態描画用のオフスクリーン・バッファ領域を作成する
	//----------------------------------------------------------------------------------------------
	pMyLGFXSide->SpriteCreate(MY_PANEL_MAX_WIDTH, MY_SIDEPANELBOOT_OPERONTEAM_Y, 4);
	pMyLGFXSide->SpriteClearDisplay(MY_PALETTE_BLACK);
//---
	//----------------------------------------------------------------------------------------------
	// サイドパネル（下部）製作者のクレジット画像描画用のオフスクリーン・バッファ領域を作成する
	//----------------------------------------------------------------------------------------------
	LGFX_Sprite		MySprite;
	MySprite.setColorDepth(8);
	MySprite.createSprite(MY_PANEL_MAX_WIDTH, MY_PANEL_MAX_HEIGHT - MY_SIDEPANELBOOT_OPERONTEAM_Y);
	MySprite.clearDisplay(MY_COLORMAP_WHITE);
//	MySprite.drawJpgFile(MYDEV_SD, MY_SIDEPANELBOOT_OPERONTEAM_PIC, 155, 0);
//---
	//----------------------------------------------------------------------------------------------
	// サイドパネル（動的）起動時（Ｂｏｏｔｓｐｌａｓｈ）描画ループ処理
	//----------------------------------------------------------------------------------------------
	for (INT iIndex = 0; iIndex < (360 * 2); iIndex += 2) {
		//------------------------------------------------------------------------------------------
		// サイドパネル（上部）デバイス状態を描画する
		//------------------------------------------------------------------------------------------
		this->DrawBootStatus(pMyLGFXSide, iIndex);

		//------------------------------------------------------------------------------------------
		// サイドパネル（下部）ProducedByイメージを描画する
		//------------------------------------------------------------------------------------------
		this->DrawProducedBy(&MySprite, pMyLGFXSide, iIndex);
	}
//---
	//----------------------------------------------------------------------------------------------
	// 描画用メモリを解放する
	//----------------------------------------------------------------------------------------------
	pMyLGFXSide->SpritePush(0, 0, MY_TRUE);
	MySprite.deleteSprite();

	//----------------------------------------------------------------------------------------------
	// この状態で、パネル状態をホールドする
	//----------------------------------------------------------------------------------------------
	delay(3000);

	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// サイドパネル・Ｂｏｏｔデバイス状態を描画する
//--------------------------------------------------------------------------------------------------
BOOL				_MyBootsplash::DrawBootStatus(
_MyLGFX				*pMyLGFXSide,
INT					iIndex)
{
	//----------------------------------------------------------------------------------------------
	// オフスクリーン・バッファ領域を作成する
	//----------------------------------------------------------------------------------------------
	String	strBuild = String(F("Build:")) + String(MyGlobalTool.MyConfigFile.dtJARVisBuild.timestamp(DateTime::TIMESTAMP_DATE));
	INT		iX = (MY_PANEL_MAX_WIDTH / 2);
	INT		iY = (MY_SIDEPANELBOOT_OPERONTEAM_Y / 2);
	INT		r = (MY_SIDEPANELBOOT_OPERONTEAM_Y / 2);
//---
	//----------------------------------------------------------------------------------------------
	// 動的描画オブジェクト：時計回りに回転する３つの円弧を描画する（１０５度ｘ３）
	//----------------------------------------------------------------------------------------------
	INT		iStartDeg = iIndex % 360;
	pMyLGFXSide->SpriteFillArc(iX, iY, r, r - 10, MyGlobalTool.Deg2Deg(iStartDeg),		MyGlobalTool.Deg2Deg(iStartDeg + 105),	MY_PALETTE_YELLOW);		iStartDeg += 120;	
	pMyLGFXSide->SpriteFillArc(iX, iY, r, r - 10, MyGlobalTool.Deg2Deg(iStartDeg - 15), MyGlobalTool.Deg2Deg(iStartDeg),		MY_PALETTE_BLACK);
	pMyLGFXSide->SpriteFillArc(iX, iY, r, r - 10, MyGlobalTool.Deg2Deg(iStartDeg),		MyGlobalTool.Deg2Deg(iStartDeg + 105),	MY_PALETTE_YELLOW);		iStartDeg += 120;
	pMyLGFXSide->SpriteFillArc(iX, iY, r, r - 10, MyGlobalTool.Deg2Deg(iStartDeg - 15), MyGlobalTool.Deg2Deg(iStartDeg),		MY_PALETTE_BLACK);
	pMyLGFXSide->SpriteFillArc(iX, iY, r, r - 10, MyGlobalTool.Deg2Deg(iStartDeg),		MyGlobalTool.Deg2Deg(iStartDeg + 105),	MY_PALETTE_YELLOW);		iStartDeg += 120;
	pMyLGFXSide->SpriteFillArc(iX, iY, r, r - 10, MyGlobalTool.Deg2Deg(iStartDeg - 15), MyGlobalTool.Deg2Deg(iStartDeg),		MY_PALETTE_BLACK);
	//
	pMyLGFXSide->SpriteFillCircle(iX, iY, r - 10, MY_PALETTE_BLACK);					//  静的描画オブジェクト：内周円（これを実行しないと綺麗に描画されない）
	pMyLGFXSide->SpriteFillCircle(iX, iY, r - 16, MY_PALETTE_YELLOW);					//  静的描画オブジェクト	
	pMyLGFXSide->SpriteFillCircle(iX, iY, r - 19, MY_PALETTE_BLACK);					//  静的描画オブジェクト
	//---
	//----------------------------------------------------------------------------------------------
	// 円周内の静的ラベル（暇があれば、画像にした方が綺麗なのだけれども、、、）
	//----------------------------------------------------------------------------------------------
	pMyLGFXSide->SpriteDrawFastHLine(iX - r + 26, iY - 26, 80, MY_PALETTE_LIGHTGRAY);	// ラベル装飾ライン（上部）
	pMyLGFXSide->SpriteDrawFastHLine(iX - r + 26, iY - 23, 70, MY_PALETTE_LIGHTGRAY);	// ラベル装飾ライン（上部）
	pMyLGFXSide->SpriteDrawFastHLine(iX - r + 26, iY - 20, 60, MY_PALETTE_LIGHTGRAY);	// ラベル装飾ライン（上部）
	//---
	pMyLGFXSide->SpriteDrawStringFormat(F("JARVis"), MY_PANEL_MAX_WIDTH / 2, MY_SIDEPANELBOOT_OPERONTEAM_Y / 2, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, 1.8, 1.4, textdatum_t::middle_center);
	pMyLGFXSide->SpriteDrawStringFormat(strBuild.c_str(), (MY_PANEL_MAX_WIDTH / 2) - 27, (MY_SIDEPANELBOOT_OPERONTEAM_Y / 2) + 10, MY_LCDFONT_eSPI4, MY_PALETTE_WHITE, 0.5, 0.7, textdatum_t::top_left);
	//---
	pMyLGFXSide->SpriteDrawFastHLine(iX +  5, iY + 30, 60, MY_PALETTE_LIGHTGRAY);		// ラベル装飾ライン（下部）
	pMyLGFXSide->SpriteDrawFastHLine(iX -  5, iY + 33, 70, MY_PALETTE_LIGHTGRAY);		// ラベル装飾ライン（下部）
	pMyLGFXSide->SpriteDrawFastHLine(iX - 15, iY + 36, 80, MY_PALETTE_LIGHTGRAY);		// ラベル装飾ライン（下部）
	//---
	//----------------------------------------------------------------------------------------------
	// デバイス・起動状態
	//----------------------------------------------------------------------------------------------
	pMyLGFXSide->SpriteDrawStringFormat(F("GPS"), 2,								2, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, 1.0, 1.0, textdatum_t::top_left);
	pMyLGFXSide->SpriteDrawStringFormat(F("RTC"), 2,						(30*1) +2, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, 1.0, 1.0, textdatum_t::top_left);
	pMyLGFXSide->SpriteDrawStringFormat(F("NET"), 2,						(30*5) +2, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, 1.0, 1.0, textdatum_t::top_left);
	//---
	pMyLGFXSide->SpriteDrawStringFormat(F("BME"), MY_PANEL_MAX_WIDTH - 2,	(30*1)+17, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, 1.0, 1.0, textdatum_t::top_right);
	pMyLGFXSide->SpriteDrawStringFormat(F("IMU"), MY_PANEL_MAX_WIDTH - 2,	(30*2)+17, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, 1.0, 1.0, textdatum_t::top_right);
	pMyLGFXSide->SpriteDrawStringFormat(F("PAJ"), MY_PANEL_MAX_WIDTH - 2,	(30*3)+17, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, 1.0, 1.0, textdatum_t::top_right);
	pMyLGFXSide->SpriteDrawStringFormat(F("AMG"), MY_PANEL_MAX_WIDTH - 2,	(30*4)+17, MY_LCDFONT_eSPI4, MY_PALETTE_YELLOW, 1.0, 1.0, textdatum_t::top_right);
	//---
	//----------------------------------------------------------------------------------------------
	// オフ・スクリーン用描画情報を実パネルに描画（但しメモリは解放しない）
	//----------------------------------------------------------------------------------------------
	pMyLGFXSide->SpritePush(0, 0, MY_FALSE);
	//---
}
/**/
//--------------------------------------------------------------------------------------------------
// サイドパネル・Ｐｒｏｄｕｃｅｄイメージを描画する
//--------------------------------------------------------------------------------------------------
VOID				_MyBootsplash::DrawProducedBy(
LGFX_Sprite			*pMySprite,
_MyLGFX				*pMyLGFXSide,
INT					iIndex)
{
	//----------------------------------------------------------------------------------------------
	// オフスクリーン・バッファ領域を作成する
	//----------------------------------------------------------------------------------------------
	if (iIndex == 0) {
		pMySprite->drawJpgFile(MYDEV_SD, MY_SIDEPANELBOOT_PRODUCEDBY_PIC, 0, 0);
	} else if (iIndex < 320) {
		pMySprite->pushSprite(pMyLGFXSide->GetLcdHandle(), iIndex - 155, MY_SIDEPANELBOOT_OPERONTEAM_Y);
	} else if (iIndex == 320) {
		pMySprite->drawJpgFile(MYDEV_SD, MY_SIDEPANELBOOT_OPERONTEAM_PIC, 155, 0);
	} else if (iIndex < 490) {
		pMySprite->pushSprite(pMyLGFXSide->GetLcdHandle(), 480 - iIndex, MY_SIDEPANELBOOT_OPERONTEAM_Y);
	}
}
/*-- End Of File --*/
