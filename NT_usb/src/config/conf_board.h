/**
 * \file
 *
 * \brief Board configuration.
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

/** Enable COM Port. */
#define CONF_BOARD_UART_CONSOLE

/** Enable USART port */
//#define CONF_BOARD_USART_RXD
//#define CONF_BOARD_USART_TXD
//#define CONF_BOARD_ADM3312_EN

#define VER (5)

#define STRING_EOL    "\r"
#define STRING_HEADER "-- ADC + PWM + SPI + I2C + Interrupt Example --\r\n" \
"-- "BOARD_NAME" --\r\n" \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

#define STRING_COMPILED "-- Compiled: "__DATE__" "__TIME__" --\r\n"

#define MENU_HEADER "\n\rChoose option:\n\r" \
"-- A: All Matrix Mode--\n\r"  \
"-- V: Change All Mode--\n\r"\
"-- U: Just Columns Mode--\n\r"\
"-- S: Single Pixel Mode--\n\r"\
"-- C: Single reference cap Mode--\n\r"\
"-- R: Change RESET Mode--\n\r"\
"-- L: Close Switch Mode--\n\r"\
"-- P: Open Switch Mode--\n\r"\
"-- F: Change PWM frequency--\n\r"\
"-- X: Release Switch Control--\n\r"\
"-- D: Micro Details--\n\r"

///** Usart Hw ID used by the console (UART0). */
//#define CONSOLE_UART_ID          ID_UART0

#define CONF_BOARD_PWM_LED1

/** SPI MACRO definition */
#define CONF_BOARD_SPI

/** SPI slave select MACRO definition */
#define CONF_BOARD_SPI_NPCS0

/** Configure TWI0 pins */
#define CONF_BOARD_TWI0

#endif /* CONF_BOARD_H_INCLUDED */
