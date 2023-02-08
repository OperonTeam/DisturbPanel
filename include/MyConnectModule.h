//--------------------------------------------------------------------------------------------------
//  Project : Disturbing Information
//  Edition : WioTerminal with Extend multi LCD-Monitor
//  File path : include/MyConnectModule.h
//  Copyright 2019, 2020, 2021, 2022, 2023 - OperonTeam
//--------------------------------------------------------------------------------------------------
#ifndef MYCONNECTMODULE_H
#define MYCONNECTMODULE_H

//--------------------------------------------------------------------------------------------------
// モジュール組込みセンサーチップの接続について
//---
//	WioTerminal -> (variant.h)
//	+-----------+-----------+-------------------+-------+---------------+-----------+
//	|Sensor Chip|Slave Addr	|Addr change		|I2C BUS|WioTerminal pin|PowerInput	|
//	+-----------+-----------+-------------------+-------+---------------+-----------+
//	|PAJ7620U2	|0x73		|SOLID				|Wire	|				|5.5v		|
//	+-----------+-----------+-------------------+-------+---------------+-----------+
//	|AMG8833	|[68h]or69h	|Address			|Wire1	|PIN_WIRE1_SDA	|3.3v(±0.3v)|
//	|BME280		|[76h]or77h	|Address/WireChange	|Wire1	|PIN_WIRE1_SCL	|1.8v - 3.3v|
//	|MPU9250	|68hor[69h]	|Address/WireChange	|Wire1	|				|2.4v - 3.6v|	_WHOAMI_DEFAULT_VALUE
//	|AK8963		|			|					|		|				|			|
//	+-----------+-----------+-------------------+-------+				+-----------+
//	|LIS3DH		|4Fh(79ul)	|Internal			|Wire1	|				|But,Not Use|
//	+-----------+-----------+-------------------+-------+---------------+-----------+
//---
//--------------------------------------------------------------------------------------------------
// I2C On Wire
//--------------------------------------------------------------------------------------------------
#define	MYI2C_PAJ7620_ADDRESS			0x73						// Gesture
//
//--------------------------------------------------------------------------------------------------
// I2C On Wire1
//--------------------------------------------------------------------------------------------------
#define MYI2C_AMG8833_ADDRESS			0x68						// Thermo
#define	MYI2C_BME280_ADDRESS			0x76						// Weather
#define MYI2C_MPU9250_ADDRESS			0x69						// IMU
/*
*/
//--------------------------------------------------------------------------------------------------
// SPI On Wire : ST7789液晶パネルが2枚購入出来たこともあり、こいつを利用します。
// 液晶パネルへのアクセスはArduinoGFXでは無く、(*)LovyanGFXを利用しています（とても便利です！）
// (*)https://github.com/lovyan03/LovyanGFX
//---
// 以下、LovyanGFXへのパラメータとして定義します。
//--------------------------------------------------------------------------------------------------
#define MYSPI_PANEL_SERCOM_INDEX		5							// 使用するSPIのSERCOM番号を設定します。
#define MYSPI_PANEL_SERCOM_CLKSRC		0							// SERCOMのクロックソースを設定します。
#define MYSPI_PANEL_SERCOM_CLKFREQ		F_CPU						// Seeeduino環境ではF_CPU定数でCPUクロック値が利用できます。
																	// SAMD51でのピン番号の設定はArduino向けの番号ではなく、
																	// SAMD51のポート+ピン番号で設定します。
#define MYSPI_PANEL_SPI_MISO			0x0100						// SPIのMISOのピン番号を設定します。
#define	MYSPI_PANEL_SPI_MOSI			0x0102						// SPIのMOSIのピン番号を設定します。
#define	MYSPI_PANEL_SPI_SCLK			0x0103						// SPIのSCLKのピン番号を設定します。 PORTA=0x000 / PORTB=0x100 / PORTC=0x200 / PORTD=0x300…
#define	MYSPI_PANEL_SPI_DC				0x0104
#define	MYSPI_PANEL_PAD_MOSI			SPI_PAD_0_SCK_1				// SPIで使用するTX Padを設定します。
#define	MYSPI_PANEL_PAD_MISO			SERCOM_RX_PAD_2				// PAD_SPI0_RX;	// SPIで使用するRX Padを設定します。
//
#define	MYSPI_PANEL_FREQ_WRITE			60000000
#define	MYSPI_PANEL_FREQ_FILL			75000000
#define	MYSPI_PANEL_FREQ_READ			20000000
//
#define	MYSPI_PANEL_MEMORY_HEIGHT		320
#define	MYSPI_PANEL_REVERSE_INVERT		true
#define	MYSPI_PANEL_A_ROTATION			3
#define	MYSPI_PANEL_B_ROTATION			1
//
//-- Panel A is Main Panel
#define	MYSPI_PANEL_A_SPI_CS			0x0107
#define	MYSPI_PANEL_A_GPIO_RST			0x0106
#define	MYSPI_PANEL_BACKLIGHT			0x0006
//
//-- Panel B is Side Panel
#define	MYSPI_PANEL_B_SPI_CS			0x0101
#define	MYSPI_PANEL_B_GPIO_RST			0x0105
//
#endif
/*-- End Of File --*/
