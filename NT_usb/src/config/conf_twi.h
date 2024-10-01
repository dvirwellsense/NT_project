///*
 //* conf_twi.h
 //*
 //* Created: 12/12/2022 12:03:11 PM
 //*  Author: DvirShavit
 //*/ 
//
//
//#ifndef CONF_TWI_H_
//#define CONF_TWI_H_
//
///** TWI I2C related define */
//#define I2C_MODULE  TWI0
//#define I2C_MODULE_ID  ID_TWI0
//
///** Kit Data define */
//#define CONF_KIT_DATA_EXIST
//#define CONF_KIT_DATA_NUM 6
//
///** TWI Bus Clock 400kHz */
//#define TWI_CLK     400000
//
///** TWI interface address */
//#define SLAVE_ADDRESS 0x08; //0x28;
//
///** Request token of EDBG TWI information interface */
//#define READ_TOKEN		 0x05
//#define EXTEN_BOARDS_TOKEN 0xE1
//#define KIT_DATA_TOKEN     0xD2
//
///** Read buffer length */
//#define READ_BUFFER_LENGTH	4
//
///** Read buffer length */
//#define WRITE_BUFFER_LENGTH	4
//
///* Number of times to try to send packet if failed. */
//#define TIMEOUT 1000
//
///** I2C related structure */
//twi_options_t opt;
//twi_packet_t packet_rx;
//
///** Read buffer */
//uint8_t read_buffer[READ_BUFFER_LENGTH];
//
///** Write buffer */
//uint8_t Wtite_buffer[WRITE_BUFFER_LENGTH];
//
///**
 //* \brief Function for read from i2c command
 //*
 //*/
//static void read_i2c_command(uint8_t request_token)
//{
	////request_token = SLAVE_READ_TOKEN;
	//uint32_t timeout = 0;
	//uint8_t i;
//
	///** Get the extension boards info */
	//packet_rx.chip        = SLAVE_ADDRESS;
	//packet_rx.addr_length = 1;
	//packet_rx.addr[0]	  = request_token;
	//packet_rx.buffer      = read_buffer;
	//packet_rx.length      = READ_BUFFER_LENGTH;
	//while (twi_master_read(I2C_MODULE, &packet_rx) != TWI_SUCCESS) {
		///* Increment timeout counter and check if timed out. */
		//if (timeout++ == TIMEOUT) {
			//return;
		//}
	//}
	////printf("\r\n The data from the slave: \r\n");
	//for(i = 0; i<READ_BUFFER_LENGTH; i++)
	//{
		////printf("%02d ", read_buffer[i]);
		//read_buffer[i]=0;
	//}
	////printf("\r\n");
//}
//
///**
 //* \brief Function for write to i2c command
 //*
 //*/
//static void write_i2c_command(uint8_t request_token)
//{
	////request_token = SLAVE_READ_TOKEN;
	//uint32_t timeout = 0;
	//uint8_t i;
	////printf("inside\r\n");
	//for(i = 0; i<WRITE_BUFFER_LENGTH; i++)
	//{
		//Wtite_buffer[i] = i;
	//}
	///** Get the extension boards info */
	//packet_rx.chip        = SLAVE_ADDRESS;
	//packet_rx.addr_length = 1;
	//packet_rx.addr[0]	  = request_token;
	//packet_rx.buffer      = Wtite_buffer;
	//packet_rx.length      = WRITE_BUFFER_LENGTH;
	//while (twi_master_write(I2C_MODULE, &packet_rx) != TWI_SUCCESS) {
		///* Increment timeout counter and check if timed out. */
		//if (timeout++ == TIMEOUT) {
			//return;
		//}
	//}
//}
//
///**
 //* \brief Function for configuring I2C master module
 //*
 //* This function will configure the I2C master module with
 //* the SERCOM module to be used and pinmux settings
 //*/
//static void configure_i2c_master(void)
//{
	///* Enable the peripheral clock for TWI */
	//pmc_enable_periph_clk(I2C_MODULE_ID);
//
	///* Configure the options of TWI driver */
	//opt.master_clk = sysclk_get_cpu_hz();
	//opt.speed      = TWI_CLK;
//
	//if (twi_master_init(I2C_MODULE, &opt) != TWI_SUCCESS) {
		////puts("-E-\tTWI master initialization failed.\r");
		//opt.speed      = TWI_CLK;
		//while (1) {
			///* Capture error */
		//}
	//}
	//
	//NVIC_DisableIRQ(TWI0_IRQn);
	//NVIC_ClearPendingIRQ(TWI0_IRQn);
	//NVIC_SetPriority(TWI0_IRQn, 0);
	//NVIC_EnableIRQ(TWI0_IRQn);
//}
//
//#endif /* CONF_TWI_H_ */