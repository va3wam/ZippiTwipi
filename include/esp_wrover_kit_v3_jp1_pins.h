/*************************************************************************************************************************************
 * @file esp_wrover_kit_V3_jp1_pins.h
 * @author va3wam
 * @brief Include file that defines the GPIO pins on an Espressif ESP-Wrover-Kit V3 development board 
 * @version 0.0.1
 * @copyright Copyright (c) 2020
 * Version YYYY-MM-DD Description
 * ------- ---------- ----------------------------------------------------------------------------------------------------------------
 * 0.0.1   2020-10-23 File created. 
 *************************************************************************************************************************************
 * # Table of Espressif ESP-Wrover-Kit Pin assignments
 * This table details the main I/O pin section of the development. This section is made up of 14x2 male pins and is labelled 
 * JP1 on the silk screen. The physical numbering system makes most sense when viewing the board with the LCD face up, the USB port 
 * facing towards you, and the I/O pins in the upper right corner. Pins are numbered starting in the upper left corner of the JP1 I/O 
 * pin section. The first row has pin 1 (GND) on the left and pin 2 (3.3V) on the right. This pattern continues for all 28 I/O pins. 
 * | Phy | Label | GPIO | In,Out, |                                                                                                   |
 * | Pin |       | Pin  | Param   | Shared With          | Function                                                                   |
 * |:---:|:-----:|:----:|:-------:|:--------------------:|:---------------------------------------------------------------------------|
 * |  1  | GND   | -    | P       | N/A                  | GND |
 * |  2  | 3.3V  | -    | P       | N/A                  | 3.3 volts power supply |
 * |  3  | IO33  | -    | -       | NC/XTAL              | |
 * |  4  | IO32  | -    | -       | NC/XTAL              | |
 * |  5  | IO13  | -    | -       | JTAG, MicroSD        | |
 * |  6  | IO12  | -    | -       | JTAG, MicroSD        | |
 * |  7  | IO27  | -    | -       | Camera               | |
 * |  8  | IO14  | -    | -       | JTAG, MicroSD        | |
 * |  9  | IO25  | -    | -       | Camera, LCD          | |
 * | 10  | IO26  | -    | -       | Camera               | |
 * | 11  | IO34  | -    | -       | Camera               | |
 * | 12  | IO35  | -    | -       | Camera               | |
 * | 13  | IO36  | -    | -       | Camera               | |
 * | 14  | IO39  | -    | -       | Camera               | |
 * | 15  | IO23  | -    | -       | Camera, LCD          | |
 * | 16  | EN    | -    | I       | JTAG                 | ENABLE |
 * | 17  | IO21  | -    | -       | Camera, LCD, MicroSD | |
 * | 18  | IO22  | -    | -       | Camera, LCD          | |
 * | 19  | IO18  | -    | -       | Camera, LCD          | |
 * | 20  | IO19  | -    | -       | Camera, LCD          | |
 * | 21  | IO17  | -    | -       | PSRAM                | |
 * | 22  | IO5   | -    | -       | Camera, LCD          | |
 * | 23  | IO4   | -    | -       | LED, Camera, MicroSD | |
 * | 24  | IO16  | -    | -       | PSRAM                | |
 * | 25  | IO2   | -    | -       | LED, MicroSD         | |
 * | 26  | IO0   | -    | -       | Camera, LED, Boot    | |
 * | 27  | 5V    | -    | -       | N/A                  | |
 * | 28  | IO15  | -    | -       | JTAG, MicroSD        | |
 *************************************************************************************************************************************/
// Mapping of pins on esp32 Wrover module
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

// Mapping of I/O pins in section J1 of the ESP Wrover Kit V3 dev board
#define IO33 GPIO33 // XTAL_32K_N (32.768 kHz crystal oscillator output),ADC1_CH5, TOUCH8, RTC_GPIO8
#define IO32 GPIO32 // XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4,TOUCH9, RTC_GPIO9 
#define IO13 GPIO13 // ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID,HS2_DATA3, SD_DATA3, EMAC_RX_ER 
#define IO12 GPIO12 // ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ,HS2_DATA2, SD_DATA2, EMAC_TXD3 
#define IO27 GPIO27 // ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV
#define IO14 GPIO14 // ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK,HS2_CLK, SD_CLK, EMAC_TXD2
#define IO25 GPIO25 // DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0
#define IO26 GPIO26 // DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1
#define IO34 GPIO34 // ADC1_CH6, RTC_GPIO4
#define IO35 GPIO35 // ADC1_CH7, RTC_GPIO5
#define IO36 GPIO36 // ADC1_CH0, RTC_GPIO0
#define IO39 GPIO39 // ADC1_CH3, RTC_GPIO3 
#define IO23 GPIO23 // VSPID, HS1_STROBE
#define EN ENABLE  
#define IO21 GPIO21 // VSPIHD, EMAC_TX_EN 
#define IO22 GPIO22 // VSPIWP, U0RTS, EMAC_TXD1
#define IO18 GPIO18 // VSPICLK, HS1_DATA7 
#define IO19 GPIO19 // VSPIQ, U0CTS, EMAC_TXD0
#define IO17 NA // PSRAM on DEV board
#define IO5 GPIO5 // GPIO5, VSPICS0, HS1_DATA6, EMAC_RX_CLK
#define IO4 GPIO4 // GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1,SD_DATA1, EMAC_TX_ER 
#define IO16 NA // PSRAM on DEV board
#define IO2 GPIO2 // GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0,SD_DATA0
#define IO0 GPIO0 // GPIO0, ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1,EMAC_TX_CLK
#define IO15 GPIO15 // ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13,HS2_CMD, SD_CMD, EMAC_RXD3