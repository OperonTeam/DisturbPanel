//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MySDWriter/MySDWriter.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"
#include "MyDevice.h"
#include "MySDWriter.h"

//--------------------------------------------------------------------------------------------------
// 本ファイル内（限定）で利用する領域を定義
//--------------------------------------------------------------------------------------------------
MY_PRIVATE	SHORT		sMyFDRYear		= (SHORT)0;								// ディレクトリ生成用
MY_PRIVATE	SHORT		sMyFDRMonth		= (SHORT)0;								//
MY_PRIVATE	SHORT		sMyFDRDay		= (SHORT)0;								//
MY_PRIVATE	SHORT		sMyFDRHour		= (SHORT)0;								//
MY_PRIVATE	SHORT		sMyFDR10Minute	= (SHORT)-1;							//
MY_PRIVATE	SHORT		sMyFDRMinute	= (SHORT)-1;							//　
MY_PRIVATE	SHORT		sMyFDRFlushMin	= (SHORT)-1;							// 停車時のFlush時刻（分）
/**/
//--------------------------------------------------------------------------------------------------
// 本ファイル内（限定）のSDFileクラスを定義
// open(), close()を繰り返すのはコストが高いので、open()したハンドルをキャッシュします。
//--------------------------------------------------------------------------------------------------
MY_PRIVATE	SDFile		MySDFile = SDFile();									// SDFile クラス
MY_PRIVATE	BOOL		bOpenedDevFlag = MY_FALSE;								// SD Dev オープン済みフラグ
MY_PRIVATE	BOOL		bOpenedFileFlag = MY_FALSE;								// SD Fileオープン済みフラグ
MY_PRIVATE	CHAR		szWorkBuff[256];										// 作業用領域
/**/
//--------------------------------------------------------------------------------------------------
// 本ファイル内（限定）のメソッドを定義
//--------------------------------------------------------------------------------------------------
MY_PRIVATE	BOOL		MySDWriterReOpenDev(VOID);
MY_PRIVATE	BOOL		MySDWriterReOpenFile(VOID);
MY_PRIVATE	BOOL		MySDWriterMKDirFile(CHAR *pszFilePath);
MY_PRIVATE	INT			MySDWriterQueType(VOID);
	typedef enum {
		FDRTYPE_CHANGE = 0,		/* 0: Change write file, flush cache */
		FDRTYPE_HEAD = 1,		/* 1: Write Head information to file */
		FDRTYPE_BODY = 2,		/* 2: Write Body information to file */	
	} MyRecorderType;

MY_PRIVATE	_MyDEV_STAT	MySDWriterFDR(_MyDevice *pMyDevice, INT iWriteRecordNum);
MY_PRIVATE	BOOL		MySDWriterHead(_MyDevice *pMyDevice);
MY_PRIVATE	BOOL		MySDWriterBody(_MyFDRBodyRec *pMyFDRBodyRec);
/**/
//--------------------------------------------------------------------------------------------------
// P R I V A T E -  F U N C T I O N
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// ＳＤデバイス上にディレクトリを作成する
//--------------------------------------------------------------------------------------------------
MY_PRIVATE	BOOL	MySDWriterMKDirFile(CHAR *pszFilePath)
{
	//----------------------------------------------------------------------------------------------
	// 依頼されたディレクトリが存在するか？
	//----------------------------------------------------------------------------------------------
	if (!MYDEV_SD.exists(pszFilePath)) {
		//------------------------------------------------------------------------------------------
		// 依頼されたディレクトリが存在しない場合、ディレクトリを生成する。
		//------------------------------------------------------------------------------------------
		if(!MYDEV_SD.mkdir(pszFilePath)) {
			//--------------------------------------------------------------------------------------
			// エラーが発生した場合
			//--------------------------------------------------------------------------------------
			MY_TRACE("mkdir(%s)=Err", pszFilePath);
			return	MY_FALSE;
		}
	}
	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// ログ情報記録用のＳＤデバイスをオープンする
//--------------------------------------------------------------------------------------------------
MY_PRIVATE	BOOL	MySDWriterReOpenDev(VOID)
{
	//----------------------------------------------------------------------------------------------
	// 上位レイヤ（ファイル）がオープン中の場合、上位レイヤを初期化します。
	//----------------------------------------------------------------------------------------------
	if (bOpenedFileFlag == MY_TRUE) {
		MySDFile.close();													// ＳＤファイル・終了
		bOpenedFileFlag = MY_FALSE;
	}

	//----------------------------------------------------------------------------------------------
	// 下位レイヤ（ＳＤ）がオープン中の場合、下位レイヤを初期化します
	//----------------------------------------------------------------------------------------------
	if (bOpenedDevFlag == MY_TRUE) {
		MYDEV_SD.end();														// ＳＤデバイス・終了
	}
//
	//----------------------------------------------------------------------------------------------
	// ＳＤデバイス状態：「初期値」の場合、下位レイヤを開始する。
	//----------------------------------------------------------------------------------------------
	bOpenedDevFlag = MY_TRUE;
	if (! MYDEV_SD.begin(SDCARD_SS_PIN, SDCARD_SPI, (INT)4000000)) {	// ＳＤデバイス・開始
		//------------------------------------------------------------------------------------------
		// 実機でテストした結果、SD.begin()がエラーを返した場合でもSD.end()が必要です！！
		//------------------------------------------------------------------------------------------
		return	MY_FALSE;
	} else {
		//------------------------------------------------------------------------------------------
		// ＳＤデバイスの初期化が完了した場合、ＳＤメディアのマウントを確認する
		//------------------------------------------------------------------------------------------
		if ((MYDEV_SD.cardType() == CARD_NONE)||(MYDEV_SD.cardType() == CARD_UNKNOWN)) {
			//--------------------------------------------------------------------------------------
			// ＦＤＲ情報が記録可能なＳＤメディアがマウントされていない場合は、エラー
			//--------------------------------------------------------------------------------------
			return	MY_FALSE;
		}
	}
	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// ログ情報記録用のデータファイルをオープンする（Minuteは、10分単位）
//	保存パス名は、/AWSLog/[Year]/[Month]/[Day]/[Hour]/[Year][Month][Day][Hour][*Minute].log]
//--------------------------------------------------------------------------------------------------
MY_PRIVATE	BOOL	MySDWriterReOpenFile(VOID)
{
	//----------------------------------------------------------------------------------------------
	// ＦＤＲファイルをクローズします
	//----------------------------------------------------------------------------------------------
	if (bOpenedFileFlag == MY_TRUE) {
		MySDFile.close();												// ＳＤファイル・終了
		bOpenedFileFlag = MY_FALSE;
	}
//---
	//----------------------------------------------------------------------------------------------
	// ＦＤＲファイルを保存するディレクトリを作成します。（現仕様では１回／１０分程度で深追いしません）
	// 末端のディレクトリが生成出来た場合は、正常終了です
	//----------------------------------------------------------------------------------------------
	BOOL	bRet;
	sprintf(szWorkBuff, "%s", MY_FDRREC_FILEPATH);				bRet = MySDWriterMKDirFile(szWorkBuff);
	sprintf(szWorkBuff, "%s/%04d", szWorkBuff, sMyFDRYear);		bRet = MySDWriterMKDirFile(szWorkBuff);
	sprintf(szWorkBuff, "%s/%02d", szWorkBuff, sMyFDRMonth);	bRet = MySDWriterMKDirFile(szWorkBuff);
	sprintf(szWorkBuff, "%s/%02d", szWorkBuff, sMyFDRDay);		bRet = MySDWriterMKDirFile(szWorkBuff);
	sprintf(szWorkBuff, "%s/%02d", szWorkBuff, sMyFDRHour);		bRet = MySDWriterMKDirFile(szWorkBuff);
	if (bRet != MY_TRUE) {
		//------------------------------------------------------------------------------------------
		// 親ディレクトリから生成している為、（恐らく）末端のディレクトリが生成出来たか？で判断可能
		// ディレクトリが生成出来なかった場合：SDカード交換中？
		//------------------------------------------------------------------------------------------
		MY_TRACE("ディレクトリ生成異常[%s]", szWorkBuff);
		return	MY_FALSE;
	}
//---
	//----------------------------------------------------------------------------------------------
	// ＦＤＲファイル名称を組み立てます。　[/AWSLog/2021/12/16/11/22/202112161120.log]
	//----------------------------------------------------------------------------------------------
	sprintf(szWorkBuff, "%s/%04d/%02d/%02d/%02d/%04d%02d%02d%02d%02d.log",
											MY_FDRREC_FILEPATH,
											sMyFDRYear,						// Directory
											sMyFDRMonth,
											sMyFDRDay,
											sMyFDRHour,
											sMyFDRYear,						// FileName
											sMyFDRMonth,
											sMyFDRDay,
											sMyFDRHour,
											sMyFDR10Minute);
	//
	//------------------------------------------------------------------------------------------
	// 前段で生成したＳＤファイル名のファイルを追記型でオープンする
	//------------------------------------------------------------------------------------------
	MySDFile = (SDFile)MYDEV_SD.open(szWorkBuff, FILE_APPEND);
	if (!MySDFile)	{
		MY_TRACE("ファイル生成異常 [%s]", szWorkBuff);
		return	MY_FALSE;
	}
	//------------------------------------------------------------------------------------------
	// ファイルのオープンカウンタを初期化（オープン済み）します。
	//------------------------------------------------------------------------------------------
	bOpenedFileFlag = MY_TRUE;												// Set Opened Flag
	return	MY_TRUE;
}
//---
//--------------------------------------------------------------------------------------------------
// ＦＤＲ書き込み処理種類を判断します。
// ログ情報記録用のデータファイルをオープンする
//	保存パス名は、/AWSLog/[Year]/[Month]/[Day]/[Hour]/
//--------------------------------------------------------------------------------------------------
MY_PRIVATE	INT			MySDWriterQueType(_MyFDRBodyRec *pMyFDRBodyRec)
{
	//----------------------------------------------------------------------------------------------
	// 前回処理したＲＴＣ日時(Year, Month, Day, Hour)と比較する（もし何かが異なっていたら）
	// GPS信号がオンライン化されるまで、GPS日時情報は初期値（2000/01/01)が通知される
	//----------------------------------------------------------------------------------------------
//
	//----------------------------------------------------------------------------------------------
	// １０分単位で、ＦＤＲ記録ファイル名の切り替えが必要か？判断します。
	//----------------------------------------------------------------------------------------------
	BOOL		bRet = MY_FALSE;
	DateTime	dtFDRNow = pMyFDRBodyRec->GetGPSDateTime();
	MY_TRACE("%04d/%02d/%02d %02d:%02d:%02d",dtFDRNow.year(),dtFDRNow.month(),dtFDRNow.day(),dtFDRNow.hour(),dtFDRNow.minute(),dtFDRNow.second());
	bRet += MyGlobalTool.IfSetDifferent((SHORT)dtFDRNow.year(),	&sMyFDRYear);
	bRet += MyGlobalTool.IfSetDifferent((SHORT)dtFDRNow.month(),&sMyFDRMonth);
	bRet += MyGlobalTool.IfSetDifferent((SHORT)dtFDRNow.day(),	&sMyFDRDay);
	bRet += MyGlobalTool.IfSetDifferent((SHORT)dtFDRNow.hour(),	&sMyFDRHour);
	bRet += MyGlobalTool.IfSetDifferent((SHORT)((dtFDRNow.minute() / 10) * 10), &sMyFDR10Minute);
	if (bRet == MY_TRUE) {
		return	MyRecorderType::FDRTYPE_CHANGE;		// 0: Change write file, flush cache
	}
//
	//----------------------------------------------------------------------------------------------
	// １分単位で、ＦＤＲ記録（ヘッダ）情報の記録を行います。
	//----------------------------------------------------------------------------------------------
	bRet = MyGlobalTool.IfSetDifferent((SHORT)dtFDRNow.minute(), &sMyFDRMinute);
	if (bRet == MY_TRUE) {
		return	MyRecorderType::FDRTYPE_HEAD;		// 1: Write Head information to file
	}
//
	//----------------------------------------------------------------------------------------------
	// 停車様態（移動速度＝０）を検知した場合、ＦＤＲ記録フラッシュを行う。
	//----------------------------------------------------------------------------------------------
	if ((INT)pMyFDRBodyRec->GetGPSSpeed() < 1) {
		//------------------------------------------------------------------------------------------
		// 記録フラッシュを行った場合、その後１分間は記録フラッシュ処理を行わない
		//------------------------------------------------------------------------------------------
		bRet = MyGlobalTool.IfSetDifferent((SHORT)dtFDRNow.minute(), &sMyFDRFlushMin);
		if (bRet == MY_TRUE) {
			return	MyRecorderType::FDRTYPE_CHANGE;	// 0: Change write file, flush cache
		}
	}
// 
	//----------------------------------------------------------------------------------------------
	// 通常は、ＦＤＲ記録（ボディ）情報の記録を行います。
	//----------------------------------------------------------------------------------------------
	return	MyRecorderType::FDRTYPE_BODY;			// 2: Write Body information to file
}
/**/
//--------------------------------------------------------------------------------------------------
// FDRHeadRec情報をＳＤデバイスに書き込む
//--------------------------------------------------------------------------------------------------
MY_PRIVATE BOOL		MySDWriterHead(_MyDevice *pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// ＲＴＣを利用して、管理情報をＳＤファイル・ストリーム形式にデータ変換する。
	//----------------------------------------------------------------------------------------------
	DateTime	dtAiMaiNow = pMyDevice->MyFDRBodyRecNow.GetGPSDateTime();
	pMyDevice->MyFDRHeadRecNow.ConvFDRStreamBuff(dtAiMaiNow, szWorkBuff);

	//----------------------------------------------------------------------------------------------
	// ＳＤファイルに追記書込みを行う
	//----------------------------------------------------------------------------------------------
	size_t	writeByte = strlen(szWorkBuff);
	size_t	sizeWriteByte = MySDFile.write(szWorkBuff, writeByte);
	MY_TRACE("MyRec : 管理情報書き込み = %d, Size = %d/%d", MySDFile.getWriteError(), writeByte, sizeWriteByte);
	MY_HEXDUMP(szWorkBuff, sizeWriteByte, "HeadWrite");
	if (sizeWriteByte != writeByte) {
		//------------------------------------------------------------------------------------------
		// ＳＤファイルへの書き込みで異常が発生（ＳＤカードが取り外された可能性が高い）
		//------------------------------------------------------------------------------------------
		MY_TRACE("管理情報書き込み異常");
		return	MY_FALSE;
	}
	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// FDRBodyRec情報をＳＤデバイスに書き込む
// 処理対象のレコードのみを記録するのでは効率が悪い為、未書き込み情報が存在する場合に書き込みを行う。
// 但し、書き込みを行う最大件数を
// →処理オーバーヘッドが大きいだけ。
//--------------------------------------------------------------------------------------------------
MY_PRIVATE BOOL		MySDWriterBody(_MyFDRBodyRec *pMyFDRBodyRec)
{
	//------------------------------------------------------------------------------------------
	// 取得したＦＤＲバッファメモリを、ＳＤファイル・ストリーム形式にデータ変換する
	//------------------------------------------------------------------------------------------
	pMyFDRBodyRec->ConvFDRStreamBuff(szWorkBuff);

	//------------------------------------------------------------------------------------------
	// ＳＤファイルに追記書込みを行う
	//------------------------------------------------------------------------------------------
	size_t	writeByte = strlen(szWorkBuff);
	size_t	sizeWriteByte = MySDFile.write(szWorkBuff, writeByte);
	MY_TRACE("SEQ[%ld]:FDRRec情報書き込み = %d, Size = %d/%d", pMyFDRBodyRec->GetRecordSeq(), MySDFile.getWriteError(), writeByte, sizeWriteByte);
	MY_HEXDUMP(szWorkBuff, sizeWriteByte, "BodyWrite");
	if (sizeWriteByte != writeByte) {
		//--------------------------------------------------------------------------------------
		// ＳＤファイルへの書き込みで異常が発生（ＳＤカードが取り外された可能性が高い）
		// デバイス管理クラスにタスク状態を通知する(MY_DEVSTAT_FAIL)
		//--------------------------------------------------------------------------------------
		MY_TRACE("MyRec[%ld]:FDRRec情報書き込み異常[%d/%d] = %d", pMyFDRBodyRec->GetRecordSeq(), writeByte, sizeWriteByte);
		return	MY_FALSE;
	}
	return	MY_TRUE;
}
/**/
//--------------------------------------------------------------------------------------------------
// FDRBodyRec情報をＳＤデバイスに書き込む
// 処理対象のレコードのみを記録するのでは効率が悪い為、未書き込み情報が存在する場合に書き込みを行う。
// 但し、書き込みを行う最大件数を
// →処理オーバーヘッドが大きいだけ。

//--------------------------------------------------------------------------------------------------
MY_PRIVATE _MyDEV_STAT	MySDWriterFDR(_MyDevice *pMyDevice, INT iWriteRecordNum)
{
	//----------------------------------------------------------------------------------------------
	// ＳＤデバイス・ファイルへの操作は必要無く、ＦＤＲ書き込み可能な状態な場合
	//----------------------------------------------------------------------------------------------
	_MyDEV_STAT		MyDevInfo = _MyDEV_STAT::MY_DEVSTAT_OPEN;
	_MyFDRBodyRec	MyFDRBodyRec;
	INT		iLoopCount = 0;
	while(pMyDevice->FDRRingBuffGetFirst(&MyFDRBodyRec)) {
		//------------------------------------------------------------------------------------------
		// ＦＤＲ未書き込み情報が存在する場合、書き込み処理種類を判断します。
		//------------------------------------------------------------------------------------------
		switch(MySDWriterQueType(&MyFDRBodyRec)) {
			case	MyRecorderType::FDRTYPE_CHANGE:						// 0: Change write file, flush cache
				//----------------------------------------------------------------------------------
				// ★FDR管理ファイルの切り替えを行う（ＳＤ・デバイス状態を変更する）
				//----------------------------------------------------------------------------------
				MyDevInfo = _MyDEV_STAT::MY_DEVSTAT_SETUP;				// FDR書き込みファイルの切り替えをセット
				break;
			case	MyRecorderType::FDRTYPE_HEAD:						// 1: Write Head information to file
				//----------------------------------------------------------------------------------
				// ★FDR管理レコード（FDRHeadRec情報）をＳＤファイルに書き込む
				//----------------------------------------------------------------------------------
				if (MySDWriterHead(pMyDevice) != MY_TRUE) {
					MyDevInfo = _MyDEV_STAT::MY_DEVSTAT_FAIL;			// デバイス異常状態に遷移
					break;
				} // 以下の行にbreakが無いのは意図的です。
			case	MyRecorderType::FDRTYPE_BODY:						// 2: Write Body information to file
				//----------------------------------------------------------------------------------
				// ★FDR通常レコード（FDRBodyRec情報）をＳＤファイルに書き込む
				//----------------------------------------------------------------------------------
				if (MySDWriterBody(&MyFDRBodyRec) != MY_TRUE) {
					MyDevInfo = _MyDEV_STAT::MY_DEVSTAT_FAIL;			// デバイス異常状態に遷移
				} else {
					//------------------------------------------------------------------------------
					// ＦＤＲバッファに「書込みが完了」を通知する
					//------------------------------------------------------------------------------
					pMyDevice->FDRRingBuffShift();
				}
				break;
		}
		//---
		iLoopCount += 1;
		if ((iLoopCount > iWriteRecordNum)||(MyDevInfo != _MyDEV_STAT::MY_DEVSTAT_OPEN)) {
			//--------------------------------------------------------------------------------------
			// 連続書き込みレコード件数を超えた場合、もしくはタスク状態が_MyDEV_STAT::MY_DEVSTAT_OPENと異なる
			//--------------------------------------------------------------------------------------
			break;
		}
	}
	return	MyDevInfo;
}
/**/
//--------------------------------------------------------------------------------------------------
// G L O B A L  -  F U N C T I O N
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// ＦＤＲ情報をＳＤデバイスに記録します。
// ＳＤデバイス、ＳＤファイルのオープン、ＦＤＲの書き込み処理にはそれぞれ一定以上の時間が必要な為、
// 一気に処理するのでは無く、何度かに分割して処理する様にしています。
// ディレクトリの作成も分割するか、悩みどころ、、、。
//--------------------------------------------------------------------------------------------------
BOOL				MySDWriter(_MyDevice *pMyDevice)
{
	//----------------------------------------------------------------------------------------------
	// ＳＤデバイスを正常（書き込み可能）な状態に遷移させる
	//----------------------------------------------------------------------------------------------
	_MyDEV_STAT	MyDevInfo = pMyDevice->GetDeviceInfo(_MyDEV_ID::MY_DEVID_FDR);
	switch(MyDevInfo) {
		case	_MyDEV_STAT::MY_DEVSTAT_NONE:								// デバイス未接続
		case	_MyDEV_STAT::MY_DEVSTAT_FAIL:								// 接続異常
		case	_MyDEV_STAT::MY_DEVSTAT_LOST:								// データ欠落（＃未使用のステータス＃）
			//--------------------------------------------------------------------------------------
			// 【下位レイヤ】の初期化処理を行う　→正常終了した場合は、上位レイヤ処理に遷移する。
			//--------------------------------------------------------------------------------------
			if (MySDWriterReOpenDev())			MyDevInfo = _MyDEV_STAT::MY_DEVSTAT_SETUP;
			break;
		case	_MyDEV_STAT::MY_DEVSTAT_SETUP:								// セットアップ
			//--------------------------------------------------------------------------------------
			// 【上位レイヤ】の初期化処理を行う　→正常終了した場合は、ＳＤ・デバイスの初期処理完了！
			//--------------------------------------------------------------------------------------
			if (MySDWriterReOpenFile())			MyDevInfo = _MyDEV_STAT::MY_DEVSTAT_OPEN;
			else								MyDevInfo = _MyDEV_STAT::MY_DEVSTAT_FAIL;
			break;
		case	_MyDEV_STAT::MY_DEVSTAT_OPEN:								// デバイス接続済み
			//----------------------------------------------------------------------------------------------
			// ＳＤデバイスにＦＤＲ情報を記録する
			//----------------------------------------------------------------------------------------------
			MyDevInfo = MySDWriterFDR(pMyDevice, MY_FDRREC_MAXRECORDERNUM);
			break;
	}
//---
	//----------------------------------------------------------------------------------------------
	// デバイス管理クラスにタスク状態を通知する
	//----------------------------------------------------------------------------------------------
	pMyDevice->SetDeviceInfo(_MyDEV_ID::MY_DEVID_FDR, MyDevInfo);

	//----------------------------------------------------------------------------------------------
	// ＳＤ記録ファイルのオープン状態を返す
	//----------------------------------------------------------------------------------------------
	return	bOpenedFileFlag;
}
/*-- End Of File --*/
