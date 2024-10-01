/**
 * \file
 *
 * \brief CDC Application Main functions
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
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

#include <asf.h>
#include "conf_usb.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
//#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_spi.h"
#include "conf_pwm.h"
#include "conf_afec.h"
//#include "conf_twi.h"
#include "conf_flash.h"
//#include "conf_counter.h"
#include "conf_rtt.h"
#include "conf_matrix.h"
//#include "conf_dacc.h"

#define STRING_VERSION "-- Version: 7 (new order without sleep) --\r\n"

static volatile bool main_b_cdc_enable = false;
static bool all_matrix = true;
static bool default_message = false;
static bool just_ref = false;
static bool just_col = false;
static bool npn_true = true;
static bool print_opt = false;
static bool print_mat = false;
static bool reset_pixel = true;
static bool control = false;
static bool freq = false;
static bool choose = false;
static bool choose_m = false;
static bool open_s = true;
static bool switch_row = true;
static bool measure = true;
static bool update_version = true;
static uint32_t newFreq = 0;
static uint8_t digit = 0;
static uint8_t new_freq[6] = {1,0,0,0,0,0};
static int powerOf10 = 1;
int mat = 0;
int del = 4;

int first_number, second_number;

/**
 * \brief Display main menu.
 */
//static void display_menu(void)
//void display_menu(void)
//{
	////puts(MENU_HEADER);
//}
//
////static void display_matrix(void)
void display_matrix(void)
{
	usb_printf(MENU_HEADER);
}
//
////static void display_pixels(void)
void display_pixels(void)
{
	usb_printf("Choose ref cap:\n\r" \
	"--1:ref1 5nF\n\r"\
	"--2:ref2 10nF\n\r"\
	"--3:ref3 15nF\n\r");
}

static volatile bool reset_interrupt_triggered = false;

void RSTC_Handler(void)
{
	/* Clear the interrupt. */
	rstc_get_status(RSTC);

	reset_interrupt_triggered = true;
}


static void set_matrix(void)
{	
	if (!print_opt)
	{
		rtt_disable_interrupt(RTT, RTT_MR_RTTINCIEN);
		display_matrix();
		print_opt = true;
	}
	else if (freq)
	{
		if (47<mat & mat<58)
		{
			new_freq[digit] = mat-'0';
			digit++;
		}
		if (digit == 6)
		{
			newFreq = 0;
			powerOf10 = 100000;
			for(int i=0;i<6;i++)
			{
				newFreq += new_freq[i] * powerOf10;
				powerOf10 /= 10;
			}
			setFrequency(newFreq);
			freq = false;
			rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
		}
	}
	else if (choose)
	{
		if (47<mat & mat<58)
		{
			if(first)
			{
				first_number=(mat-'0')*10;
				first = false;
			}
			else
			{
				first = true;
				second_number = mat-'0';
				if(switch_row)
				{
					refCapR = first_number+second_number-1;
					matrixHandlerRowOpen(refCapR);
					
					switch_row = false;
					usb_printf("\r\nChoose C between 01-30\r\n");
				}
				else
				{
					refCapC = first_number+second_number-1;
					matrixHandlerColOpen(refCapC);
					usb_printf("\r\nOpen pixel: R%d C%d\r\n",refCapR+1,refCapC+1);
					rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
					print_mat = false;
					print_opt = false;
					choose = false;
				}
			}
		}
	}
	else if(control)
	{
		if(mat == 'x' | mat == 'X')
		{
			print_mat = false;
			print_opt = false;
			control = false;
			rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
			usb_printf("return to measuring\r\n");
		}
		else if(mat == 'p' | mat == 'P')
		{
			usb_printf("Choose C-col, R-row, S-reset:\r\n");
			open_s = true;
			first = true;
			print_mat = true;
			control = true;
		}
		else if(mat == 'l' | mat == 'L')
		{
			usb_printf("Choose C-col, R-row, S-reset:\r\n");
			open_s = false;
			first = true;
			print_mat = true;
			control = true;
		}
		else if(mat == 'c' | mat == 'C')
		{
			usb_printf("\r\nChoose number between 01-33\r\n");
			switch_row = false;
		}
		else if(mat == 'r' | mat == 'R')
		{
			usb_printf("\r\nChoose number between 01-60\r\n");
			switch_row = true;
		}
		else if(mat == 's' | mat == 'S')
		{
			if(open_s){
				usb_printf("Reset ON\r\n");
				resetHandler(true);}
			else{
				usb_printf("Reset OFF\r\n");
				resetHandler(false);}
			usb_printf("Type X to exit, L to close or P to open\r\n");
		}
		else if (47<mat & mat<58)
		{
			if(first)
			{
				first_number=(mat-'0')*10;
				first = false;
			}
			else
			{
				first = true;
				second_number = mat-'0';
				refCapC = first_number + second_number -1;
				if(switch_row)
				{
					if(open_s)
					{
						if(refCapC!=255)
						{
							usb_printf("\r\nSwitch R%d ON\r\n",refCapC+1);
							matrixHandlerRowOpen(refCapC);
						}
						else 
						{
							usb_printf("\r\nOpen all Rows");
							open_all_Rows();
						}
						}
					else
					{
						if(refCapC!=255)
						{
							usb_printf("\r\nSwitch R%d OFF\r\n",refCapC+1);
							matrixHandlerRowClose(refCapC);
						}
						else
						{
							usb_printf("\r\nClose all Rows");
							close_all_Rows();
						}
						}	
				}
				else
				{
					if(open_s)
					{
							if(refCapC!=255)
							{
								usb_printf("\r\nSwitch C%d ON\r\n",refCapC+1);
								matrixHandlerColOpen(refCapC);
							}
							else
							{
								usb_printf("\r\nOpen all Columns");
								open_all_Cols();
							}
						}
					else{
							if(refCapC!=255)
							{
								usb_printf("\r\nSwitch C%d OFF\r\n",refCapC+1);
								matrixHandlerColClose(refCapC);
							}
							else
							{
								usb_printf("\r\nClose all Columns");
								close_all_Cols();
							}
						}
				}
				usb_printf("Type X to exit, L to close or P to open\r\n");
			}
		}
		
	}
	else if(!print_mat)
	{
		switch (mat)
		{
			case 'a':
			case 'A':
				all_matrix = true;
				print_opt = false;
				just_ref = false;
				just_col = false;
				usb_printf("All mat\r\n");
				rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
				break;
			case 'u':
			case 'U':
				all_matrix = true;
				print_opt = false;
				just_ref = true;
				just_col = true;
				usb_printf("Just columns\r\n");
				rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
				break;
			case 'b':
			case 'B':
				usb_printf("Waiting for the flash file\r\n");
				rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
				testFlash();
					//for (int i =0; i<buffer_length;i++)
					//{
						//page_buffer[i] = mat+i;
					//}
					////page_buffer[index_buffer] = mat;
					////index_buffer++;
					//print_page_buffer();
					//WriteFlash();
				break;				
			case 'c':
			case 'C':
				all_matrix = false;
				just_ref = false;
				usb_printf("Single reference cap\r\n");
				print_mat = true;
				display_pixels();
				break;
			case 's':
			case 'S':
				all_matrix = false;
				first = true;
				choose = true;
				just_ref = true;
				switch_row = true;
				usb_printf("Single pixel cap\r\n");
				usb_printf("Choose R between 01-60\r\n");
				print_mat = true;
				break;
			case 'l':
			case 'L':
				usb_printf("Choose C-col, R-row, S-reset to close:\r\n");
				open_s = false;
				first = true;
				print_mat = true;
				control = true;
				break;
			case 'f':
			case 'F':
				usb_printf("Choose Frequency between 5 - 400000:\r\n");
				digit = 0;
				print_mat = true;
				freq = true;
				break;
			case 'p':
			case 'P':
				usb_printf("Choose C-col, R-row, S-reset to open:\r\n");
				open_s = true;
				first = true;
				print_mat = true;
				control = true;
				break;
			case 'd':
			case 'D':
				print_mat = false;
				print_opt = false;
				control = false;
				usb_printf(STRING_COMPILED);
				usb_printf(STRING_VERSION);
				delay_s(3);
				rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
				break;
			case 'v':
			case 'V':
				just_ref = !just_ref;
				print_opt = false;
				rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
				if(just_ref)
				{
					usb_printf("Specific cap mode\r\n");
				}
				else
				{
					usb_printf("All Matrix cap mode\r\n");
				}
				break;	
			case 'r':
			case 'R':
				reset_pixel = !reset_pixel;
				print_opt = false;
				rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
				if(reset_pixel)
				{
					usb_printf("reset the cap\r\n");
				}
				else
				{
					usb_printf("Not reset the cap\r\n");
				}
				break;
			case 'x':
			case 'X':
				print_mat = false;
				usb_printf(STRING_HEADER);
				rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
				break;		
			default:
				break;
		}
	}
	else if(48<mat & mat<53)
	{
		refCapC = mat-'0'-1;
		refCapR = 0;
		print_opt = false;
		rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
		print_mat = false;
		//spi_master_open_col(COLUMNS+REF);
		close_all_Cols();
		close_all_Rows();
		resetHandler(npn_true);
		//ioport_set_pin_level(RESET, npn_true);
	}
	else
	{
		print_opt = false;
		rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
		print_mat = false;
		usb_printf("out range\r\n");
	}
}



int main(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();

	// Initialize the sleep manager
	sleepmgr_init();

	sysclk_init();
	board_init();

	//// Start USB stack to authorize VBus monitoring
	udc_start();
	//
	///* Configure the spi master module */
	spi_master_initialize();
	///* start real time timer  */
	start_rtt();
	//
	///* Configure the I2C master module */
	////configure_i2c_master();
	//
	///* Configure input interrupt */
	interrupt_configure();
	//
	///* Initial the pga */
	////InitPGA();
	
	/* Initial PWM peripheral clock */
	InitPWM();
	
	/* Initial ADC (AFEC) */
	InitADC();
	
	/* Initialize DACC */
	//Init_DACC();
	
	/* Initial TC  */
	//Init_TC();

	rstc_disable_user_reset(RSTC);
	NVIC_DisableIRQ(RSTC_IRQn);
	rstc_enable_user_reset_interrupt(RSTC);
	NVIC_ClearPendingIRQ(RSTC_IRQn);
	NVIC_SetPriority(RSTC_IRQn, 0);
	NVIC_EnableIRQ(RSTC_IRQn);
	
	refCapC = 0;
	close_all_Rows();
	//matrixHandler(R1C1);
	matrixHandler(0,0);
	delay_ms(10);
	//startCounter();
	resetHandler(true);
	//ioport_set_pin_level(RESET, npn_true);
	
	uint8_t uc_key;
	ioport_set_pin_level(START, true);
	
	while (true)
	{	
		//if(sleeping)
		//{
			//sleepmgr_init();
			//sleepmgr_lock_mode(4);
			//sleepmgr_enter_sleep();
		//}
		if(print)
		{
			ioport_set_pin_level(LED, true);
			afec_disable_interrupt(AFEC1, AFEC_INTERRUPT_ALL);
			send_json_like_message();
			printUSB("\r\n");
			printUSB("\r\n");
			//usb_printf("\r\n");
			//printADC(refCapR, refCapC, all_matrix, just_ref);
			print = false;
			afec_enable_interrupt(AFEC1, AFEC_INTERRUPT_DATA_READY);
			measure = true;
			ioport_set_pin_level(LED, false);
			restart_rtt();
		}
		if (measure)
		{			
			measure = false;
			if(all_matrix)
			{
				if(just_ref)
				{
					if (just_col)
					{
						close_all_Cols();
						close_all_Rows();
						delay_ms(1);
						for(uint8_t f = 0; f< COLUMNS+REF; f++)
						{
							counter_adcAV[0][f] = 0;
							counter_adcMIN[0][f] = 100000;
							counter_adcMAX[0][f] = 0;
							//spi_master_open_col(f+26);
							//matrixHandlerRef(f);
							matrixHandlerColOpen(f);
							//delay_us(50);
							resetHandler(false);
							//ioport_set_pin_level(RESET, !npn_true);
							for(uint8_t s = 0; s<MEAS; s++)
							{
								afec_start_software_conversion(AFEC1);
								//printUSB_S("");
								delay_us(del);
								while(!afec_sample_data.is_done){}
								save_ADC(0, f, s, afec_sample_data.value);
							}
							matrixHandlerColClose(f);
							resetHandler(true);
							//ioport_set_pin_level(RESET, npn_true);
							//spi_master_open_col(COLUMNS+REF);
							//close_all();
							delay_us(50);
						}
					}
					else
					{
						close_all_Cols();
						close_all_Rows();
						delay_ms(1);
						for(uint8_t f = COLUMNS; f< COLUMNS+REF; f++)
						{
							counter_adcAV[0][f] = 0;
							counter_adcMIN[0][f] = 100000;
							counter_adcMAX[0][f] = 0;
							//spi_master_open_col(f+26);
							//matrixHandlerRef(f);
							matrixHandlerColOpen(f);
							//delay_us(50);
							resetHandler(false);
							//ioport_set_pin_level(RESET, !npn_true);
							for(uint8_t s = 0; s<MEAS; s++)
							{
								afec_start_software_conversion(AFEC1);
								//printUSB_S("");
								delay_us(del);
								while(!afec_sample_data.is_done){}
								save_ADC(0, f, s, afec_sample_data.value);
							}
							matrixHandlerColClose(f);
							resetHandler(true);
							//ioport_set_pin_level(RESET, npn_true);
							//spi_master_open_col(COLUMNS+REF);
							//close_all();
							delay_us(50);
						}
					}
				}
				else
				{
					close_all_Cols();
					close_all_Rows();
					delay_ms(1);
					//ioport_set_pin_level(START, true);
					for(uint8_t r = 0; r< ROWS; r++)
					{
						matrixHandlerRowOpen(r);
						for(uint8_t c = 0; c<COLUMNS; c++)
						{
							delay_us(50); //delay_us(20);
							matrixHandlerColOpen(c);
							counter_adcAV[r][c] = 0;
							counter_adcMIN[r][c] = 100000;
							counter_adcMAX[r][c] = 0;
							//delay_us(50);
							//while(ioport_get_pin_level(START) == IOPORT_PIN_LEVEL_LOW){}
							resetHandler(false);
							//ioport_set_pin_level(RESET, !npn_true);
							for(uint8_t s = 0; s<MEAS; s++)
							{
								afec_start_software_conversion(AFEC1);
								//printUSB_S("");
								delay_us(del);
								while(!afec_sample_data.is_done){}
								save_ADC(r, c, s, afec_sample_data.value);
							}
							matrixHandlerColClose(c);
							resetHandler(true);
							//ioport_set_pin_level(RESET, npn_true);
							//matrixHandlerRef(c+CLOSE);
							//matrixHandlerRef(REF);
							//spi_master_open_col(COLUMNS+REF);
							//close_all();
							delay_us(50);
						}
						//matrixHandlerRowClose(r);
						close_all_Rows();
						for(uint8_t f = COLUMNS; f<REF+COLUMNS; f++)
						{
							counter_adcAV[r][f] = 0;
							counter_adcMIN[r][f] = 100000;
							counter_adcMAX[r][f] = 0;
							//spi_master_open_col(f+26);
							//matrixHandlerRef(f);
							matrixHandlerColOpen(f);
							//delay_us(50);
							resetHandler(false);
							//ioport_set_pin_level(RESET, !npn_true);
							for(uint8_t s = 0; s<MEAS; s++)
							{
								afec_start_software_conversion(AFEC1);
								//printUSB_S("");
								delay_us(del);
								while(!afec_sample_data.is_done){}
								save_ADC(r, f, s, afec_sample_data.value);
							}
							matrixHandlerColClose(f);
							resetHandler(true);
							//ioport_set_pin_level(RESET, npn_true);
							//spi_master_open_col(COLUMNS+REF);
							//close_all();
							delay_us(50);
						}
						delay_us(100);
						//delay_ms(1);
					}
				}
			}
			else
			{
				if(just_ref)
				{
					usb_printf("one pixel- r: %d c: %d\r\n",refCapR+1,refCapC+1);
					close_all_Cols();
					close_all_Rows();
					delay_ms(1);
					delay_ms(1);
					counter_adcAV[refCapR][refCapC] = 0;
					counter_adcMIN[refCapR][refCapC] = 100000;
					counter_adcMAX[refCapR][refCapC] = 0;
					//spi_master_open_col(refCapC+26);
					matrixHandlerOpen(refCapR, refCapC);
					//delay_ms(1);
					//delay_us(50);
					resetHandler(false);
					//ioport_set_pin_level(RESET, !npn_true);
					for(uint8_t s = 0; s<MEAS; s++)
					{
						afec_start_software_conversion(AFEC1);
						//printUSB_S("");
						delay_us(del);
						while(!afec_sample_data.is_done){}
						save_ADC(refCapR, refCapC, s, afec_sample_data.value);
					}
					if(reset_pixel)
					{
						matrixHandlerClose(refCapR, refCapC);
						resetHandler(true);
						delay_ms(1);
						//ioport_set_pin_level(RESET, npn_true);
						//spi_master_open_col(COLUMNS+REF);
						//close_all();
					}
					//delay_ms(500);
					delay_us(50);
				}
				else
				{
					close_all_Cols();
					close_all_Rows();
					delay_ms(1);
					delay_ms(1);
					matrixHandlerColOpen(COLUMNS + refCapC);
					counter_adcAV[0][COLUMNS + refCapC] = 0;
					counter_adcMIN[0][COLUMNS + refCapC] = 100000;
					counter_adcMAX[0][COLUMNS + refCapC] = 0;
					//spi_master_open_col(refCapC+26);
					
					//delay_ms(1);
					//delay_us(50);
					//delay_ms(1);
					resetHandler(false);
					//ioport_set_pin_level(RESET, !npn_true);
					for(uint8_t s = 0; s<MEAS; s++)
					{
						afec_start_software_conversion(AFEC1);
						//printUSB_S("");
						delay_us(del);
						while(!afec_sample_data.is_done){}
						save_ADC(0, COLUMNS + refCapC, s, afec_sample_data.value);
					}
					if(reset_pixel)
					{
						matrixHandlerColClose(COLUMNS + refCapC);
						resetHandler(true);
						delay_ms(1);
						//ioport_set_pin_level(RESET, npn_true);
						//spi_master_open_col(COLUMNS+REF);
						//close_all();
					}
					//delay_ms(500);
					delay_us(50);
				}
				
			}
		}

		if (udi_cdc_is_rx_ready()) {
			mat = udi_cdc_getc();
			udi_cdc_putc(mat);
			//page_buffer[index_buffer] = mat;
			//index_buffer++;
			//udi_cdc_putc(mat);
			}	
		if (mat !=0)
		{
			afec_disable_interrupt(AFEC1, AFEC_INTERRUPT_ALL);
			rtt_disable_interrupt(RTT, RTT_MR_RTTINCIEN);
			set_matrix();
			afec_enable_interrupt(AFEC1, AFEC_INTERRUPT_DATA_READY);
			mat = 0;
		}
	}
}
