/*
 * conf_matrix.h
 *
 * Created: 30/01/2023 11:15:04
 *  Author: DvirShavit
 */ 


#ifndef CONF_MATRIX_H_
#define CONF_MATRIX_H_
#include "conf_afec.h"
#define RESET	PIO_PD27_IDX
#define MEASURE	PIO_PB2_IDX	
#define START	PIO_PA10_IDX
#define SLEEP	PIO_PA0_IDX
#define LED		PIO_PB12_IDX

#define ROW1	PIO_PA15_IDX
#define ROW2    PIO_PD10_IDX
#define ROW3    PIO_PD30_IDX
#define ROW4    PIO_PA1_IDX
#define ROW5    PIO_PA17_IDX
#define ROW6    PIO_PD11_IDX
#define ROW7    PIO_PB0_IDX
#define ROW8    PIO_PA2_IDX
#define ROW9    PIO_PD23_IDX
#define ROW10	PIO_PD20_IDX
#define ROW11	PIO_PA19_IDX
#define ROW12	PIO_PD26_IDX
#define ROW13   PIO_PA8_IDX
#define ROW14   PIO_PA10_IDX
#define ROW15   PIO_PD24_IDX
#define ROW16   PIO_PD12_IDX
#define ROW17   PIO_PA18_IDX
#define ROW18   PIO_PD13_IDX
#define ROW19   PIO_PA20_IDX
#define ROW20   PIO_PD28_IDX
#define ROW21   PIO_PA7_IDX
#define ROW22   PIO_PA28_IDX
#define ROW23   PIO_PD17_IDX
#define ROW24   PIO_PA24_IDX
#define ROW25   PIO_PB1_IDX 
#define ROW26   PIO_PD14_IDX
#define ROW27   PIO_PA5_IDX
#define ROW28   PIO_PD16_IDX
#define ROW29   PIO_PA21_IDX
#define ROW30   PIO_PA9_IDX
#define ROW31   PIO_PA23_IDX
#define ROW32   PIO_PA26_IDX
#define ROW33   PIO_PD25_IDX
#define ROW34   PIO_PD18_IDX
#define ROW35   PIO_PA16_IDX
#define ROW36   PIO_PD19_IDX
#define ROW37   PIO_PA22_IDX
#define ROW38   PIO_PA30_IDX
#define ROW39   PIO_PB3_IDX
#define ROW40   PIO_PA25_IDX
#define ROW41   PIO_PD5_IDX
#define ROW42   PIO_PA27_IDX
#define ROW43   PIO_PD1_IDX
#define ROW44   PIO_PD15_IDX
#define ROW45   PIO_PB14_IDX
#define ROW46   PIO_PD22_IDX
#define ROW47   PIO_PD31_IDX
#define ROW48   PIO_PD29_IDX
#define ROW49   PIO_PD0_IDX
#define ROW50   PIO_PD9_IDX
#define ROW51   PIO_PD8_IDX
#define ROW52   PIO_PA31_IDX
#define ROW53   PIO_PA6_IDX
#define ROW54   PIO_PD2_IDX
#define ROW55   PIO_PD7_IDX
#define ROW56   PIO_PA29_IDX
#define ROW57   PIO_PD4_IDX
#define ROW58   PIO_PB13_IDX
#define ROW59   PIO_PD3_IDX
#define ROW60   PIO_PD6_IDX


//uint32_t rowValues[] = {ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7, ROW8, ROW9, ROW10,
	//ROW11, ROW12, ROW13, ROW14, ROW15, ROW16, ROW17, ROW18, ROW19, ROW20,
	//ROW21, ROW22, ROW23, ROW24, ROW25, ROW26, ROW27, ROW28, ROW29, ROW30,
	//ROW31, ROW32, ROW33, ROW34, ROW35, ROW36, ROW37, ROW38, ROW39, ROW40,
	//ROW41, ROW42, ROW43, ROW44, ROW45, ROW46, ROW47, ROW48, ROW49, ROW50,
	//ROW51, ROW52, ROW53, ROW54, ROW55, ROW56, ROW57, ROW58, ROW59, ROW60};
	
uint32_t rowValues[] = {ROW60, ROW1, ROW59, ROW2, ROW58, ROW3, ROW57, ROW4, ROW56, ROW5,
	ROW55, ROW6, ROW54, ROW7, ROW53, ROW8, ROW52, ROW9, ROW51, ROW10,
	ROW50, ROW11, ROW49, ROW12, ROW48, ROW13, ROW47, ROW14, ROW46, ROW15,
	ROW45, ROW16, ROW44, ROW17, ROW43, ROW18, ROW42, ROW19, ROW41, ROW20,
	ROW40, ROW21, ROW39, ROW22, ROW38, ROW23, ROW37, ROW24, ROW36, ROW25,
	ROW35, ROW26, ROW34, ROW27, ROW33, ROW28, ROW32, ROW29, ROW31, ROW30};

static uint8_t refCapC, refCapR;
static bool	first;
static bool debug = false; 
static bool sleeping = false;

void pin_sleep_handler();

void interrupt_configure(void)
{
	//start trigger
	ioport_init();
	ioport_set_pin_dir(RESET, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(MEASURE, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(START, IOPORT_DIR_OUTPUT);
	Matrix *p_matrix = MATRIX;
	p_matrix->CCFG_SYSIO |= CCFG_SYSIO_SYSIO12;
	ioport_set_pin_dir(LED, IOPORT_DIR_OUTPUT);
	
	////rows interrupt
	ioport_set_pin_dir(ROW1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW3, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW4, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW5, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW6, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW7, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW8, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW9, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW10, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW11, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW12, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW13, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW14, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW15, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW16, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW17, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW18, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW19, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW20, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW21, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW22, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW23, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW24, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW25, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW26, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW27, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW28, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW29, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW30, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW31, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW32, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW33, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW34, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW35, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW36, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW37, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW38, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW39, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW40, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW41, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW42, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW43, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW44, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW45, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW46, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW47, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW48, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW49, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW50, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW51, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW52, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW53, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW54, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW55, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW56, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW57, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW58, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW59, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ROW60, IOPORT_DIR_OUTPUT);
}

void pin_sleep_handler(const uint32_t id, const uint32_t index)
{
	//NVIC_ClearPendingIRQ(PIOA_IRQn);
	if ((id == ID_PIOA) && (index == PIO_PA0))
	{
		if (pio_get(PIOA, PIO_TYPE_PIO_INPUT, PIO_PA0))
		{
			printUSB("\r\nStart sleep mode\r\n");
			spi_master_open_col(254);
			delay_ms(1000);
			sleeping = true;
			sleepmgr_init();
			sleepmgr_lock_mode(4);
			sleepmgr_enter_sleep();
		}
		else
		{
			printUSB("\r\nEnd sleep mode\r\n");
			//irq_initialize_vectors();
			//cpu_irq_enable();
			//sysclk_init();
			//board_init();
//
			//ui_init();
			//ui_powerdown();
			//udc_start();
			//start_rtt();
			//ioport_set_pin_level(START, true);
			rstc_start_software_reset(RSTC);
			//printUSB("Wake up mode\r\n");
			sleeping = false;
			//NVIC_SystemReset();
		}
	}
}

void close_all_Rows()
{
	ioport_set_pin_level(ROW1, false);
	ioport_set_pin_level(ROW2, false);
	ioport_set_pin_level(ROW3, false);
	ioport_set_pin_level(ROW4, false);
	ioport_set_pin_level(ROW5, false);
	ioport_set_pin_level(ROW6, false);
	ioport_set_pin_level(ROW7, false);
	ioport_set_pin_level(ROW8, false);
	ioport_set_pin_level(ROW9, false);
	ioport_set_pin_level(ROW10, false);
	ioport_set_pin_level(ROW11, false);
	ioport_set_pin_level(ROW12, false);
	ioport_set_pin_level(ROW13, false);
	ioport_set_pin_level(ROW14, false);
	ioport_set_pin_level(ROW15, false);
	ioport_set_pin_level(ROW16, false);
	ioport_set_pin_level(ROW17, false);
	ioport_set_pin_level(ROW18, false);
	ioport_set_pin_level(ROW19, false);
	ioport_set_pin_level(ROW20, false);
	ioport_set_pin_level(ROW21, false);
	ioport_set_pin_level(ROW22, false);
	ioport_set_pin_level(ROW23, false);
	ioport_set_pin_level(ROW24, false);
	ioport_set_pin_level(ROW25, false);
	ioport_set_pin_level(ROW26, false);
	ioport_set_pin_level(ROW27, false);
	ioport_set_pin_level(ROW28, false);
	ioport_set_pin_level(ROW29, false);
	ioport_set_pin_level(ROW30, false);
	ioport_set_pin_level(ROW31, false);
	ioport_set_pin_level(ROW32, false);
	ioport_set_pin_level(ROW33, false);
	ioport_set_pin_level(ROW34, false);
	ioport_set_pin_level(ROW35, false);
	ioport_set_pin_level(ROW36, false);
	ioport_set_pin_level(ROW37, false);
	ioport_set_pin_level(ROW38, false);
	ioport_set_pin_level(ROW39, false);
	ioport_set_pin_level(ROW40, false);
	ioport_set_pin_level(ROW41, false);
	ioport_set_pin_level(ROW42, false);
	ioport_set_pin_level(ROW43, false);
	ioport_set_pin_level(ROW44, false);
	ioport_set_pin_level(ROW45, false);
	ioport_set_pin_level(ROW46, false);
	ioport_set_pin_level(ROW47, false);
	ioport_set_pin_level(ROW48, false);
	ioport_set_pin_level(ROW49, false);
	ioport_set_pin_level(ROW50, false);
	ioport_set_pin_level(ROW51, false);
	ioport_set_pin_level(ROW52, false);
	ioport_set_pin_level(ROW53, false);
	ioport_set_pin_level(ROW54, false);
	ioport_set_pin_level(ROW55, false);
	ioport_set_pin_level(ROW56, false);
	ioport_set_pin_level(ROW57, false);
	ioport_set_pin_level(ROW58, false);
	ioport_set_pin_level(ROW59, false);
	ioport_set_pin_level(ROW60, false);	
}

void open_all_Rows()
{
	ioport_set_pin_level(ROW1, true);
	ioport_set_pin_level(ROW2, true);
	ioport_set_pin_level(ROW3, true);
	ioport_set_pin_level(ROW4, true);
	ioport_set_pin_level(ROW5, true);
	ioport_set_pin_level(ROW6, true);
	ioport_set_pin_level(ROW7, true);
	ioport_set_pin_level(ROW8, true);
	ioport_set_pin_level(ROW9, true);
	ioport_set_pin_level(ROW10, true);
	ioport_set_pin_level(ROW11, true);
	ioport_set_pin_level(ROW12, true);
	ioport_set_pin_level(ROW13, true);
	ioport_set_pin_level(ROW14, true);
	ioport_set_pin_level(ROW15, true);
	ioport_set_pin_level(ROW16, true);
	ioport_set_pin_level(ROW17, true);
	ioport_set_pin_level(ROW18, true);
	ioport_set_pin_level(ROW19, true);
	ioport_set_pin_level(ROW20, true);
	ioport_set_pin_level(ROW21, true);
	ioport_set_pin_level(ROW22, true);
	ioport_set_pin_level(ROW23, true);
	ioport_set_pin_level(ROW24, true);
	ioport_set_pin_level(ROW25, true);
	ioport_set_pin_level(ROW26, true);
	ioport_set_pin_level(ROW27, true);
	ioport_set_pin_level(ROW28, true);
	ioport_set_pin_level(ROW29, true);
	ioport_set_pin_level(ROW30, true);
	ioport_set_pin_level(ROW31, true);
	ioport_set_pin_level(ROW32, true);
	ioport_set_pin_level(ROW33, true);
	ioport_set_pin_level(ROW34, true);
	ioport_set_pin_level(ROW35, true);
	ioport_set_pin_level(ROW36, true);
	ioport_set_pin_level(ROW37, true);
	ioport_set_pin_level(ROW38, true);
	ioport_set_pin_level(ROW39, true);
	ioport_set_pin_level(ROW40, true);
	ioport_set_pin_level(ROW41, true);
	ioport_set_pin_level(ROW42, true);
	ioport_set_pin_level(ROW43, true);
	ioport_set_pin_level(ROW44, true);
	ioport_set_pin_level(ROW45, true);
	ioport_set_pin_level(ROW46, true);
	ioport_set_pin_level(ROW47, true);
	ioport_set_pin_level(ROW48, true);
	ioport_set_pin_level(ROW49, true);
	ioport_set_pin_level(ROW50, true);
	ioport_set_pin_level(ROW51, true);
	ioport_set_pin_level(ROW52, true);
	ioport_set_pin_level(ROW53, true);
	ioport_set_pin_level(ROW54, true);
	ioport_set_pin_level(ROW55, true);
	ioport_set_pin_level(ROW56, true);
	ioport_set_pin_level(ROW57, true);
	ioport_set_pin_level(ROW58, true);
	ioport_set_pin_level(ROW59, true);
	ioport_set_pin_level(ROW60, true);
}


void matrixHandler(uint8_t row, uint8_t col)
{
	ioport_set_pin_level(rowValues[row], true);
	spi_master_open_col(col);
	//spi_master_open_col(message);
	//printUSB_4V(message);
}

void matrixHandlerOpen(uint8_t row, uint8_t col)
{
	spi_master_open_col(col+OPEN);
	//spi_master_open_col(message);
	//printUSB_4V(message);
	//printUSB_4V(col+OPEN);
	//printUSB("\r\n");
	ioport_set_pin_level(rowValues[row], true);
}

void matrixHandlerClose(uint8_t row, uint8_t col)
{
	spi_master_open_col(col+CLOSE);
	//spi_master_open_col(message);
	//printUSB_4V(message);
	//printUSB_4V(col+CLOSE);
	//printUSB("\r\n");
	ioport_set_pin_level(rowValues[row], false);
}

void matrixHandlerRef(uint8_t ref)
{
	spi_master_open_col(ref+26);
	//spi_master_open_col(message);
	//printUSB_4V(message);
}

void matrixHandlerRowOpen(uint8_t ref)
{
	ioport_set_pin_level(rowValues[ref], true);
}

void matrixHandlerRowClose(uint8_t ref)
{
	ioport_set_pin_level(rowValues[ref], false);
}

void matrixHandlerColOpen(uint8_t ref)
{
	//spi_master_open_col(ref+26+OPEN);
	spi_master_open_col(ref+OPEN);
	//spi_master_open_col(message);
	//printUSB_4V(message);
}

void matrixHandlerColClose(uint8_t ref)
{
	//spi_master_open_col(ref+26+CLOSE);
	spi_master_open_col(ref+CLOSE);
	//spi_master_open_col(message);
	//printUSB_4V(message);
}

void close_all_Cols()
{
	spi_master_open_col(10);
	//spi_master_open_col(message);
	//printUSB_4V(message);
}

void open_all_Cols()
{
	for (int i=0;i<COLUMNS+REF;i++)
	{
		spi_master_open_col(i+OPEN);
		delay_ms(100);
	}
	//spi_master_open_col(message);
	//printUSB_4V(message);
}

void resetHandler(bool change_to)
{
	ioport_set_pin_level(RESET, change_to);
}

#endif /* CONF_MATRIX_H_ */