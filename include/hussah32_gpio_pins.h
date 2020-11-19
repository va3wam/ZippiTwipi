/*************************************************************************************************************************************
 * @file esp32_wrover_pins.h
 * @author va3wam
 * @brief Include file that defines the GPIO pins on an Espressif ESP-Wrover module 
 * @version 0.0.1
 * @copyright Copyright (c) 2020
 * Version YYYY-MM-DD Description
 * ------- ---------- ----------------------------------------------------------------------------------------------------------------
 * 0.0.1   2020-10-23 File created. 
 *************************************************************************************************************************************
 * # Table of Espressif ESP-Wrover Module Pin assignments
 * This table detials the 38 pins on the ESP32-Wrover (PCB) module. These definitions can be mapped to pins on various development 
 * boards based on this module. The peripheral pins defined in this file can be understood using this table.
 * | Phy |            |      |                                                                                                       |
 * | Pin | Name       | Type | Function                                                                                              |
 * |:---:|:----------:|:----:|:------------------------------------------------------------------------------------------------------|
 * |  1  | GND        | P    | GND |
 * |  2  | 3.3V       | P    | 3.3 Volts power supply |
 * |  3  | ENABLE     | I    | Module-enable signal. Active high. |
 * |  4  | SENSOR_VP  | I    | GPIO36, ADC1_CH0, RTC_GPIO0 |
 * |  5  | SENSOR_VN  | I    | GPIO39, ADC1_CH3, RTC_GPIO3 |
 * |  6  | IO34       | I    | GPIO34, ADC1_CH6, RTC_GPIO4 |
 * |  7  | IO35       | I    | GPIO35, ADC1_CH7, RTC_GPIO5 |
 * |  8  | IO32       | I/O  | GPIO32, XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4,TOUCH9, RTC_GPIO9 |
 * |  9  | IO33       | I/O  | GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output),ADC1_CH5, TOUCH8, RTC_GPIO8 |
 * | 10  | IO25       | I/O  | GPIO25, DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0 |
 * | 11  | IO26       | I/O  | GPIO26, DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1 |
 * | 12  | IO27       | I/O  | GPIO27, ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV |
 * | 13  | IO14       | I/O  | GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK,HS2_CLK, SD_CLK, EMAC_TXD2 |
 * | 14  | IO12       | I/O  | GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ,HS2_DATA2, SD_DATA2, EMAC_TXD3 |
 * | 15  | GND        | P    | Ground |
 * | 16  | IO13       | I/O  | GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID,HS2_DATA3, SD_DATA3, EMAC_RX_ER |
 * | 17  | SHD/SD2    | I/O  | GPIO9, SD_DATA2, SPIHD, HS1_DATA2, U1RXD |
 * | 18  | SWP/SD3    | I/O  | GPIO10, SD_DATA3, SPIWP, HS1_DATA3, U1TXD |
 * | 19  | SCS/CMD    | I/O  | GPIO11, SD_CMD, SPICS0, HS1_CMD, U1RTS |
 * | 20  | SCK/CLK    | I/O  | GPIO6, SD_CLK, SPICLK, HS1_CLK, U1CTS |
 * | 21  | SDO/SD0    | I/O  | GPIO7, SD_DATA0, SPIQ, HS1_DATA0, U2RTS |
 * | 22  | SDI/SD1    | I/O  | GPIO8, SD_DATA1, SPID, HS1_DATA1, U2CTS |
 * | 23  | IO15       | I/O  | GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13,HS2_CMD, SD_CMD, EMAC_RXD3 |
 * | 24  | IO2        | I/O  | GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0,SD_DATA0 |
 * | 25  | IO0        | I/O  | GPIO0, ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1,EMAC_TX_CLK |
 * | 26  | IO4        | I/O  | GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1,SD_DATA1, EMAC_TX_ER |
 * | 27  | NC1        | -    | - |
 * | 28  | NC2        | -    | - |
 * | 29  | IO5        | I/O  | GPIO5, VSPICS0, HS1_DATA6, EMAC_RX_CLK |
 * | 30  | IO18       | I/O  | GPIO18, VSPICLK, HS1_DATA7 |
 * | 31  | IO19       | I/O  | GPIO19, VSPIQ, U0CTS, EMAC_TXD0 |
 * | 32  | NC         | -    | - |
 * | 33  | IO21       | I/O  | GPIO21, VSPIHD, EMAC_TX_EN |
 * | 34  | RXD0       | I/O  | GPIO3, U0RXD, CLK_OUT2 |
 * | 35  | TXD0       | I/O  | GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2 |
 * | 36  | IO22       | I/O  | GPIO22, VSPIWP, U0RTS, EMAC_TXD1 |
 * | 37  | IO23       | I/O  | GPIO23, VSPID, HS1_STROBE |
 * | 38  | GND        | P    | Ground |
 *************************************************************************************************************************************/
#define ENABLE 3
#define GPIO0 25
#define GPIO2 24
#define GPIO4 26
#define GPIO5 29
#define GPIO6 20
#define GPIO7 21
#define GPIO8 22
#define GPIO9 17
#define GPIO10 18
#define GPIO11 19
#define GPIO12 14
#define GPIO13 16
#define GPIO14 13
#define GPIO15 23
#define GPIO18 30
#define GPIO19 31
#define GPIO21 33
#define GPIO22 36
#define GPIO23 37
#define GPIO25 10
#define GPIO26 11
#define GPIO27 12
#define GPIO32 8
#define GPIO33 9
#define GPIO34 6 
#define GPIO35 7
#define GPIO36 4 
#define GPIO39 5