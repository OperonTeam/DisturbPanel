//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyPanel/Boot/MyBootsplash.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYBOOTSPLASH_H
#define MYBOOTSPLASH_H

//--------------------------------------------------------------------------------------------------
// 外部参照・依存関係インクルード・ヘッダ情報
//--------------------------------------------------------------------------------------------------
#include "MyDevice.h"														// Device
#include "LGFX/MyLGFX.h"													// LovyanGFX Wrapper

//--------------------------------------------------------------------------------------------------
// 起動処理中に利用する画像イメージ
//--------------------------------------------------------------------------------------------------
// 【メインパネル】
#define	MY_MAINPANELBOOT_OWNERSBIKE_PIC		F("/AWSSetup/Logo/MyFavorite.jpg")		// OWNERS BIKE			：320x240
//
#define	MY_MAINPANELBOOT_OWNERLICENSE_X		0
#define	MY_MAINPANELBOOT_OWNERLICENSE_Y		0
#define	MY_MAINPANELBOOT_OWNERLICENSE_PIC	F("/AWSSetup/Logo/license_plate.jpg")	// ライセンスプレート	：320x168
//
#define	MY_MAINPANELBOOT_SUZUKINWORLD_X		0
#define	MY_MAINPANELBOOT_SUZUKINWORLD_Y		169
#define	MY_MAINPANELBOOT_SUZUKINWORLD_PIC	F("/AWSSetup/Logo/sw_logo_01.jpg")		// SUZUKIWORLDロゴ		：320x72
//
// 【サイドパネル】
#define	MY_SIDEPANELBOOT_OPERONTEAM_X		0
#define	MY_SIDEPANELBOOT_OPERONTEAM_Y		192
#define	MY_SIDEPANELBOOT_HAYABUSALOGO_PIC	F("/AWSSetup/Logo/hayabusa.jpg")		// HAYABUSAロゴ			：320x240
#define	MY_SIDEPANELBOOT_OPERONTEAM_PIC		F("/AWSSetup/Logo/OperonTeamLogo.jpg")	// OPERONTEAMロゴ		：165x47
#define	MY_SIDEPANELBOOT_PRODUCEDBY_PIC		F("/AWSSetup/Logo/ProducedBy.jpg")		// ProducedByロゴ		：155x47

//--------------------------------------------------------------------------------------------------
// 起動処理中イメージ・描画クラス
//--------------------------------------------------------------------------------------------------
class   _MyBootsplash {
	private:
		BOOL					DrawBootStatus(_MyLGFX *pMyLGFXMain, INT iIndex);
		VOID					DrawProducedBy(LGFX_Sprite *pMySprite, _MyLGFX *pMyLGFXSide, INT iIndex);
//
	public:
		//------------------------------------------------------------------------------------------
		// メインどころのメソッド
		//------------------------------------------------------------------------------------------
		_MyBootsplash();
		~_MyBootsplash();
//
		//------------------------------------------------------------------------------------------
		// パネル表示オブジェクト
		//	DrawBootLogo		:	BootSplash情報
		//	DrawDeviceStatus	:	デバイスBootSplash情報
		//------------------------------------------------------------------------------------------
		VOID					DrawBootLogo(_MyLGFX *pMyLGFXMain, _MyLGFX *pMyLGFXSide);
		BOOL					DrawBooting(_MyLGFX *pMyLGFXMain, _MyLGFX *pMyLGFXSide, _MyDevice *pMyDevice);
};
#endif
/*-- End Of File --*/
