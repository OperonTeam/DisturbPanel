//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyDevice/Thermo/MyThermoMatrix.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyThermoMatrix.h"

//--------------------------------------------------------------------------------------------------
// クラス初期メソッド
//--------------------------------------------------------------------------------------------------
_MyThermoMatrix::_MyThermoMatrix()
{
	this->autoptr = 0;													// 過去情報保存用インデックス
	::memset(this->automin, 0x00, sizeof(this->automin));				// 過去Thermoマトリクス情報の最低値
	::memset(this->automax, 0x00, sizeof(this->automax));				// 過去Thermoマトリクス情報の最高値
	::memset(this->sensData, 0x00, sizeof(this->sensData));				// 現在Thermoマトリクス情報の複写版
	::memset(this->cMap, 0x00, sizeof(this->cMap));						// 現在Thermoマトリクス情報の演算後
}
/**/
//--------------------------------------------------------------------------------------------------
// クラス終了メソッド
//--------------------------------------------------------------------------------------------------
_MyThermoMatrix::~_MyThermoMatrix()
{
}
/**/
//--------------------------------------------------------------------------------------------------
// サーモ情報マトリクス情報の疑似拡張(8x8 -> 52x52)メソッド
//	AMG8833センサーから得られる生データは8x8ですが、このままビジュアル化しても寂しい限りです。
//	この情報をなんとか変換することで、巷で見かけるサーモ系のビジュアルに変換したい！と思っておりましたが、
//	Google先生が見つけてくれました。
//	本来はAMG8833 + M5STACK の組み合わせで、"のるLAB"さん頒布し、スイッチサイエンス社で販売される受託商品用
//	に採用されているソースを引用・改変しています。（サンプルソースが公開されています）
//	非常に厚かましいお願いにも関わらず、"のるLAB"さんに快諾頂き感謝の念に堪えません。(2022/06/17)
//	noru@fb3.so-net.ne.jp
//	https://drive.google.com/file/d/1jaGpVzVb6Ds5kiZZ9VfulSVY4krcmn0u/view
//--------------------------------------------------------------------------------------------------
FLOAT			_MyThermoMatrix::MakeExtendMatrixMap(
FLOAT			*pfRawBuff,
BOOL			bFaceFlag)
{
	//----------------------------------------------------------------------------------------------
	// 取得したサーモマトリクス(8x8)生データの極値（最低・最高値）を求めつつ、作業領域に複写する
	//----------------------------------------------------------------------------------------------
	FLOAT	fRetAverage = (FLOAT)0.0;
	FLOAT	fMinData = *pfRawBuff;
	FLOAT	fMaxData = fMinData;
	//---
	for (INT iY1 = 0; iY1 < THERMOMAP_RAWY; iY1++) {
		for (INT iX1 = 0; iX1 < THERMOMAP_RAWX; iX1++) {
			if (bFaceFlag) {
				this->sensData[7 - iX1][iY1]	= *pfRawBuff;						// Face
			} else {
				this->sensData[iX1][iY1]		= *pfRawBuff;						// Normal
			}
			fMinData = min(fMinData, *pfRawBuff);
			fMaxData = max(fMaxData, *pfRawBuff);
			fRetAverage += *pfRawBuff;
			pfRawBuff++;
		}
	}
	fRetAverage = fRetAverage / (THERMOMAP_RAWX * THERMOMAP_RAWX);

	//----------------------------------------------------------------------------------------------
	// 求めた極値（最低・最高値）情報を保存する
	//----------------------------------------------------------------------------------------------
	this->automin[this->autoptr] = (INT)fMinData;
	this->automax[this->autoptr] = (INT)fMaxData;
	this->autoptr++;
	if (this->autoptr >= IAUTOMEMN)		this->autoptr = 0;
//
	//----------------------------------------------------------------------------------------------
	// 過去10回分が退避されているテーブルから、極値（最低・最高値）それぞれの平均値を求める
	//----------------------------------------------------------------------------------------------
	INT		iMin = 0;
	INT		iMax = 0;
	for (INT iX1=0; iX1 < IAUTOMEMN; iX1++) {
		iMin += this->automin[iX1];		iMax += this->automax[iX1];
	}
	iMin = iMin / IAUTOMEMN - 1;	iMin = max(iMin, 0);
	iMax = iMax / IAUTOMEMN + 1;	FLOAT	fMin = (FLOAT)iMin;
	FLOAT	fScale = (FLOAT)(iMax - iMin);	fScale = max(fScale, (FLOAT)4.0);

	//----------------------------------------------------------------------------------------------
	// 疑似拡張後のドット・マトリクスの指定座標（Ｙ軸）に対してループ処理を実行する
	//----------------------------------------------------------------------------------------------
	for (INT iYa = IGRIDMAX1; iYa < IGRIDMAX2; iYa++) {
		FLOAT fY = (FLOAT)iYa / fDIVMATRIX - (FLOAT)0.4;
		INT	iMStart = (iYa - IDIVMATRIX - 1) / IDIVMATRIX;	iMStart = max(iMStart, 0);	iMStart = min(iMStart,	IGRIDMAX - 1);
		INT iMStop	= (iYa + IDIVMATRIX + 1) / IDIVMATRIX;	iMStop	= max(iMStop, 0);	iMStop	= min(iMStop,	IGRIDMAX - 1);
		//--
		//-----------------------------------------------------------------------------------------
		// 疑似拡張後のドット・マトリクスの指定座標（Ｘ軸）に対してループ処理を実行する
		//-----------------------------------------------------------------------------------------
		for (INT iXa = IGRIDMAX1; iXa < IGRIDMAX2; iXa++) {
			FLOAT fSum = (FLOAT)0;
			FLOAT fX = (FLOAT)iXa / (FLOAT)fDIVMATRIX - (FLOAT)0.4;
			INT iNStart = (iXa - IDIVMATRIX - 1) / IDIVMATRIX;	iNStart = max(iNStart, 0);	iNStart = min(iNStart,	IGRIDMAX - 1);	
			INT iNStop	= (iXa + IDIVMATRIX + 1) / IDIVMATRIX;	iNStop	= max(iNStop, 0);	iNStop	= min(iNStop,	IGRIDMAX - 1);
			//--
			//-------------------------------------------------------------------------------------
			// 疑似拡張後のドット・マトリクス座標に対して描画カラーを演算する
			//-------------------------------------------------------------------------------------
			for (INT iN = iNStart; iN <= iNStop; iN++) {
				FLOAT	fDiff = (FLOAT)iN - fX;
				fDiff = abs(fDiff);
				if (fDiff <= (FLOAT)1) {
					FLOAT	fFnX = (FLOAT)1 - fDiff;
					for (INT iM = iMStart; iM <= iMStop; iM++) {
						fDiff = (FLOAT)iM - fY;
						fDiff = abs(fDiff);
						if (fDiff <= (FLOAT)1) {
							FLOAT	fFnY = (FLOAT)1 - fDiff;
							fSum = fSum + (this->sensData[iN][iM] * fFnX * fFnY);
						}
					}
				}
			}

			//--------------------------------------------------------------------------------------
			// 疑似拡張サーモマトリクス情報（温度:fSum）をサーモ表示用のカラーパレット番号:iValに変換する
			//--------------------------------------------------------------------------------------
			INT iVal = (INT)((fSum - fMin) * MY_THERMO_COLOR_RANGE / fScale);
			iVal = max(iVal, 0);
			iVal = min(iVal, 15);

			//--------------------------------------------------------------------------------------
			// 生成したサーモ表示用のカラーパレット番号を、演算結果（表示用領域）に保存する
			//--------------------------------------------------------------------------------------
			this->cMap[iXa][iYa] = (CHAR)iVal;
		}
	}
	//----------------------------------------------------------------------------------------------
	// 入力センサー（生データ）の平均値を返却する
	//----------------------------------------------------------------------------------------------
	return	fRetAverage;
}
/*-- End Of File --*/
