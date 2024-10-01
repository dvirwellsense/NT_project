///*
 //* conf_counter.h
 //*
 //* Created: 12/19/2022 11:25:06 AM
 //*  Author: DvirShavit
 //*/ 
//
//
//#ifndef CONF_COUNTER_H_
//#define CONF_COUNTER_H_
//
//#include "conf_matrix.h"
//
//#define TC             TC0
//#define TC_PERIPHERAL  0
//
//#define TC_CHANNEL 1
//#define ID_TC ID_TC1
//#define SAMPLINGRATE	16.6e-3
//
//static uint32_t first_measure, measure;
//// static float save_matrix[MATRIXLEN];
////static uint32_t save_matrix[MATRIXLEN];
//
////#include "stdio_serial.h"
//
//#define MEAS_C 1000
//static long count = 0;
//static uint8_t countAVERAGE = 0;
//static uint32_t counter[MEAS_C];
//static uint32_t counterAV = 0;
//static uint32_t counterAVERAGE = 0;
//static uint32_t counterMIN = 100000;
//static uint32_t counterMAX = 0;
//static uint32_t diff;
//static uint32_t temp;
//static uint32_t diffAVERAGE = 0;
//static float count_time = 0;
//static char str[20];
//
//void printTC()
//{	
	//for (uint8_t r = 0; r<ROWS; r++)
	//{
		////printf("\r\n");
		////printf("r%d,", r+1);
		//for (uint8_t c = 0; c<COLUMNS; c++)
		//{
			//temp = diff;
			////printf("%d,", save_matrix[r+c*(ROWS)]);
		//}
	//}
	////printf("\r\n");
//}
//
////void save_TC(float value, uint8_t place)
////{
	////save_matrix[place] = value;
////}
//
///**
 //* \brief Initialize TC
 //*/
////static void Init_TC(void)
//void Init_TC(void)
//{
	///* Configure the PMC to enable the TC module */
	//sysclk_enable_peripheral_clock(ID_TC);
	//tc_init(TC, TC_CHANNEL, TC_CMR_TCCLKS_TIMER_CLOCK1);
//}
//
//#endif /* CONF_COUNTER_H_ */