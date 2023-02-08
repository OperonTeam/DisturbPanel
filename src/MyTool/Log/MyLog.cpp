//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : src/MyTool/Log/MyLog.cpp
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#include "MyBase.h"

//--------------------------------------------------------------------------------------------------
// ログ出力ターゲットソースファイル名
// ログを出力するターゲット判断は、外付けファイルの方が良いのだけれどもね
//--------------------------------------------------------------------------------------------------
#if	defined(MYLOG_ENABLE_LOG) || defined(MYLOG_ENABLE_TRACE) || defined(MYLOG_ENABLE_HEXDUMP)
String	strDebugTarget[] = {
//	F("src\\Main.cpp"),
//	F("src\\MyAPTest\\MyAPTest.cpp"),
//	F("src\\MyDevice\\Button\\MyButton.cpp"),
//	F("src\\MyDevice\\FDRRec\\MyFDRBodyRec.cpp"),
//	F("src\\MyDevice\\FDRRec\\MyFDRHeadRec.cpp"),
//	F("src\\MyDevice\\MyDevice.cpp"),
//	F("src\\MyDevice\\Thermo\\MyThermoMatrix.cpp"),
//	F("src\\MyJARVis\\Analyze\\MyJARVisAnalyze.cpp"),
//	F("src\\MyJARVis\\Geolocation\\MyGeolocation.cpp"),
//	F("src\\MyJARVis\\Message\\MyMessage.cpp"),
//	F("src\\MyJARVis\\MyJARVis.cpp"),
//	F("src\\MyJARVis\\PastRec\\MyPastRec.cpp"),
//	F("src\\MyPanel\\Boot\\MyBootsplash.cpp"),
//	F("src\\MyPanel\\LGFX\\MyLGFX.cpp"),
//	F("src\\MyPanel\\MyPanel.cpp"),
//	F("src\\MyPanel\\PanelMain\\MyPanelMain.cpp"),
//	F("src\\MyPanel\\PanelSide\\LowArea\\MyPanelSideLowMove.cpp"),
//	F("src\\MyPanel\\PanelSide\\LowArea\\MyPanelSideLowStop.cpp"),
//	F("src\\MyPanel\\PanelSide\\MyPanelSide.cpp"),
//	F("src\\MyPanel\\PanelSide\\TopArea\\MyPanelSideTop.cpp"),
//	F("src\\MyPanel\\Utility\\MyDrawScroll.cpp"),
//	F("src\\MySDWriter\\MySDWriter.cpp"),
//	F("src\\MyTask\\MyBLE\\MyBLE.cpp"),
//	F("src\\MyTask\\MyGPS\\MyGPS.cpp"),
//	F("src\\MyTask\\MyI2C\\AMG8833\\MyAMG8833.cpp"),
//	F("src\\MyTask\\MyI2C\\BME280\\MyBME280.cpp"),
//	F("src\\MyTask\\MyI2C\\MPU9250\\MyMPU9250.cpp"),
//	F("src\\MyTask\\MyI2C\\MyI2C.cpp"),
//	F("src\\MyTask\\MyI2C\\MyI2CBase.cpp"),
//	F("src\\MyTask\\MyI2C\\PAJ7620\\MyPAJ7620.cpp"),
//	F("src\\MyTask\\MyNET\\MyNET.cpp"),
//	F("src\\MyTask\\MyTask.cpp"),
//	F("src\\MyTool\\Com\\MyComTimer.cpp"),
//	F("src\\MyTool\\GPSAnalyze\\MyGPSAnalyze.cpp"),
//	F("src\\MyTool\\Log\\MyLog.cpp"),
//	F("src\\MyTool\\MyTool.cpp"),
//	F("src\\MyTool\\Config\\MyConfigFile.cpp"),
	F("")
};

//--------------------------------------------------------------------------------------------------
// ログ出力ターゲット判断関数
//--------------------------------------------------------------------------------------------------
BOOL		IsDebugTarget(CONST_CHAR *pszFile)
{
	for (INT iIndex = 0; strDebugTarget[iIndex].length() != 0; iIndex++) {
		//------------------------------------------------------------------------------------------
		// 入力パラメータ(__FILE__)にターゲット文字列が含まれている場合は、MY_TRUE
		//------------------------------------------------------------------------------------------
		if (strstr(pszFile, strDebugTarget[iIndex].c_str()))	return	MY_TRUE;
	}
	return	MY_FALSE;
}
/**/
//--------------------------------------------------------------------------------------------------
// ログ情報開始
//--------------------------------------------------------------------------------------------------
VOID		MyLogBegin(CONST_CHAR *pszDev)
{
	if (!pszDev) {
		//------------------------------------------------------------------------------------------
		// 指示が無い場合は、標準シリアルポートに出力する
		//------------------------------------------------------------------------------------------
		MYDEV_Debug.begin(115200);
		for (INT iCount = 0; iCount < 10; iCount++) {
			if (MYDEV_Debug)			break;	
			delay(1000);
		}
	}
}
/**/
//--------------------------------------------------------------------------------------------------
// ログ情報出力関数
//--------------------------------------------------------------------------------------------------
VOID		MyLog(CONST_CHAR *pszFMT, ...)
{
	CHAR	szBuff[512] = { 0 };
	va_list vaArgs;
	va_start(vaArgs, pszFMT);
	INT		r = vsnprintf(szBuff, sizeof(szBuff), pszFMT, vaArgs);
	va_end(vaArgs);
	if (r > 0) {
		MYDEV_Debug.write(szBuff);
	}
}
/**/
//--------------------------------------------------------------------------------------------------
// HEXDUMP出力関数
//--------------------------------------------------------------------------------------------------
VOID		MyHexDump(
VOID		*pvPnt,
size_t		dumpSize,
CONST_CHAR	*pszTitle)
{
	CHAR	*pcPnt;
	UINT	i = 0;
	CHAR	szHEXBuff[64];
	CHAR	szASCBuff[64];
	MyLog("DUMP [%s]/Size=%d Start\n", pszTitle, dumpSize);
	pcPnt = (CHAR *)pvPnt;
	::memset(szHEXBuff, 0x00, sizeof(szHEXBuff));
	::memset(szASCBuff, 0x00, sizeof(szASCBuff));

	for (i = 0; i < dumpSize; i++ ) {
		if (pcPnt == NULL) {
			MyLog("Null pointer internal Error\n");		return;
		}
		sprintf(szHEXBuff, "%s%02X ", szHEXBuff, *pcPnt);
		if ((*pcPnt>=0x20)&&(*pcPnt<=0x7e))	sprintf(szASCBuff, "%s%c", szASCBuff, *pcPnt);
		else								sprintf(szASCBuff, "%s%c", szASCBuff, '@');
		if ((!((i-15)%16)) && (i>0)){
			MyLog("%04X : %s | %s\n", i-15, szHEXBuff, szASCBuff);
			::memset(szHEXBuff, 0x00, sizeof(szHEXBuff));
			::memset(szASCBuff, 0x00, sizeof(szASCBuff));
		}			
		pcPnt++;
	}
	if (strlen(szHEXBuff)) {
		MyLog("%04X : %-48s | %s\n", i-15, szHEXBuff, szASCBuff);
	}
	MyLog("DUMP END\n");
}
#endif
/*-- End Of File --*/
