/*
 * conf_spi.h
 *
 * Created: 11/27/2022 11:08:11 AM
 *  Author: DvirShavit
 */ 
/**
 * \defgroup spi_example_pin_defs
 *  - <b> SAM4E-EK  --  Pin number </b>
 *  - VCC -- VCC
 *  - NPCS0(PA11) -- EXT1.9
 *  - MISO(PA12)  -- EXT1.17
 *  - MOSI(PA13)  -- EXT1.16
 *  - SPCK(PA14)  -- EXT1.18
 *  - GND -- GND
 */

#ifndef CONF_SPI_H_
#define CONF_SPI_H_

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
	#endif
	/**INDENT-ON**/
	/// @endcond

	#define SPI_Handler     SPI_Handler
	#define SPI_IRQn        SPI_IRQn

	/** Spi Hw ID . */
	#define SPI_ID          ID_SPI

	/** SPI base address for SPI master mode*/
	#define SPI_MASTER_BASE      SPI
	
	/* Chip select. */
	#define SPI_CHIP_SEL 0
	#define SPI_CHIP_PCS spi_get_pcs(SPI_CHIP_SEL)

	/* Clock polarity. */
	#define SPI_CLK_POLARITY 0

	/* Clock phase. */
	#define SPI_CLK_PHASE 1

	/* Delay before SPCK. */
	#define SPI_DLYBS 0x40

	/* Delay between consecutive transfers. */
	#define SPI_DLYBCT 0x10

	/* SPI example commands for this example. */
	/* slave test state, begin to return RC_RDY. */
	#define CMD_TEST     0b00000101

	/* Slave data state, begin to return last data block. */
	#define CMD_DATA     0x29380000

	/* Slave status state, begin to return RC_RDY + RC_STATUS. */
	#define CMD_STATUS   0x68390384

	/* Slave idle state, begin to return RC_SYN. */
	#define CMD_END      0x68390484

	/* General return value. */
	#define RC_SYN       0x55AA55AA

	/* Ready status. */
	#define RC_RDY       0x12345678

	/* Slave data mask. */
	#define CMD_DATA_MSK 0xFFFF0000

	/* Slave data block mask. */
	#define DATA_BLOCK_MSK 0x0000FFFF

	/* Number of commands logged in status. */
	#define NB_STATUS_CMD   20

	/* Number of SPI clock configurations. */
	#define NUM_SPCK_CONFIGURATIONS 4

	/* SPI Communicate buffer size. */
	#define COMM_BUFFER_SIZE   64
	
	/* Status block. */
	struct status_block_t {
		/** Number of data blocks. */
		uint32_t ul_total_block_number;
		/** Number of SPI commands (including data blocks). */
		uint32_t ul_total_command_number;
		/** Command list. */
		uint32_t ul_cmd_list[NB_STATUS_CMD];
	};
	
	enum AmpLevels {
		kAmp1 = 0,
		kAmp2 = 1,
		kAmp4 = 2,
		kAmp8 = 3,
		kAmp16 = 4,
		kAmp32 = 5,
		kAmp64 = 6,
		kAmp128 = 7,

		kNumLevels = 8
	};
	
	enum Channels {
		kAcSourceChannel = 0,
		kSignalChannel = 1
	};
	
	static uint16_t ch0_amps[kNumLevels][2];
	static uint16_t ch1_amps[kNumLevels][2];
	static uint16_t read_amps[2];
	static uint8_t result;
	
	/* SPI clock setting (Hz). */
	static uint32_t gs_ul_spi_clock = 1000000; //10000;
	static uint32_t gs_ul_cv;
	
	uint8_t _currentAmp;
	uint8_t _currentChannel;
	
	/**
	* \brief Initialize SPI as master.
	 */
	static void spi_master_initialize(void)
	{
		//puts("-I- Initialize SPI as master\r");
		/* Configure an SPI peripheral. */
		spi_enable_clock(SPI_MASTER_BASE);
		spi_disable(SPI_MASTER_BASE);
		spi_reset(SPI_MASTER_BASE);
		spi_set_lastxfer(SPI_MASTER_BASE);
		spi_set_master_mode(SPI_MASTER_BASE);
		spi_disable_mode_fault_detect(SPI_MASTER_BASE);
		spi_set_peripheral_chip_select_value(SPI_MASTER_BASE, SPI_CHIP_PCS);
		spi_set_clock_polarity(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_POLARITY);
		spi_set_clock_phase(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_PHASE);
		spi_set_bits_per_transfer(SPI_MASTER_BASE, SPI_CHIP_SEL,
				SPI_CSR_BITS_8_BIT);
		spi_set_baudrate_div(SPI_MASTER_BASE, SPI_CHIP_SEL,
				(
		sysclk_get_peripheral_hz()
				/ gs_ul_spi_clock));
		spi_set_transfer_delay(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_DLYBS,
				SPI_DLYBCT);
		spi_enable(SPI_MASTER_BASE);
	}
	
	uint8_t pga_Read_Amp(void)
	{
		uint8_t data;
		//uint8_t uc_pcs;
		//static uint16_t data0;
		//static uint16_t data1;
		//
		//spi_write(SPI_MASTER_BASE, read_amps[0],0,0);
		//spi_write(SPI_MASTER_BASE, read_amps[1],0,0);
		//while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);
		//spi_read(SPI_MASTER_BASE, &data0, &uc_pcs);
		//spi_read(SPI_MASTER_BASE, &data1, &uc_pcs);
		//printf("data from pga: 0x%x \n\r", data0);
		//printf("data from pga: 0x%x \n\r", data1);
		data = _currentAmp;
		return data;
	}

	void pga_Change_Amp(uint8_t level, uint8_t ch)
	{
		if (ch == kAcSourceChannel)
		{   //Channel 0 is selected
			spi_write(SPI_MASTER_BASE, ch0_amps[level][0],0,0);
			spi_write(SPI_MASTER_BASE, ch0_amps[level][1],0,0);
			//spi_write(SPI_MASTER_BASE, 0b10101010,0,0);
			//spi_write(SPI_MASTER_BASE, 0b00000000,0,0);
		}
		else if (ch == kSignalChannel)
		{	//Channel 1 is selected
			spi_write(SPI_MASTER_BASE, ch1_amps[level][0],0,0);
			spi_write(SPI_MASTER_BASE, ch1_amps[level][1],0,0);
		}
		_currentAmp = level;
		_currentChannel = ch;
	}
	
	static void InitPGA(void)
{
	//Setting all values - these values are sent to the PGA according to the ch and amp
	//The exact explanation of the values can be found in the data sheet in ti.com (PGA112)
	ch0_amps[0][0] = 0b00101010;
	ch0_amps[0][1] = 0b00000000;
	ch0_amps[1][0] = 0b00101010;
	ch0_amps[1][1] = 0b00010000;
	ch0_amps[2][0] = 0b00101010;
	ch0_amps[2][1] = 0b00100000;
	ch0_amps[3][0] = 0b00101010;
	ch0_amps[3][1] = 0b00110000;
	ch0_amps[4][0] = 0b00101010;
	ch0_amps[4][1] = 0b01000000;
	ch0_amps[5][0] = 0b00101010;
	ch0_amps[5][1] = 0b01010000;
	ch0_amps[6][0] = 0b00101010;
	ch0_amps[6][1] = 0b01100000;
	ch0_amps[7][0] = 0b00101010;
	ch0_amps[7][1] = 0b01110000;	

	ch1_amps[0][0] = 0b00101010;
	ch1_amps[0][1] = 0b00000001;
	ch1_amps[1][0] = 0b00101010;
	ch1_amps[1][1] = 0b00010001;
	ch1_amps[2][0] = 0b00101010;
	ch1_amps[2][1] = 0b00100001;
	ch1_amps[3][0] = 0b00101010;
	ch1_amps[3][1] = 0b00110001;
	ch1_amps[4][0] = 0b00101010;
	ch1_amps[4][1] = 0b01000001;
	ch1_amps[5][0] = 0b00101010;
	ch1_amps[5][1] = 0b01010001;
	ch1_amps[6][0] = 0b00101010;
	ch1_amps[6][1] = 0b01100001;
	ch1_amps[7][0] = 0b00101010;
	ch1_amps[7][1] = 0b01110001;
	
	read_amps[0] = 0b01101010;
	read_amps[1] = 0b00000000;

	// kSignalChannel = using with channel 1
	pga_Change_Amp(kAmp1, kSignalChannel); //kAcSourceChannel
		
}

uint8_t GetCurrentAmp(void)
{
	return _currentAmp;
}

uint8_t GetCurrentChannel(void)
{
	return _currentChannel;
}

uint8_t GetAmpRatio(void)
{
	switch (_currentAmp) {
	case kAmp1:
		return 1;
		break;
	case kAmp2:
		return 2;
		break;
	case kAmp4:
		return 4;
		break;
	case kAmp8:
		return 8;
		break;
	case kAmp16:
		return 16;
		break;
	case kAmp32:
		return 32;
		break;
	case kAmp64:
		return 64;
		break;
	case kAmp128:  //maximum amplification, cannot increase
		return 128;
		break;
	case kNumLevels:
		return -1;
		break;
	}
	return 0;
}

bool IncreaseAmplification(void)
{
	switch (_currentAmp) {
	case kAmp1:
		pga_Change_Amp(kAmp2,_currentChannel);
		break;
	case kAmp2:
		pga_Change_Amp(kAmp4,_currentChannel);
		break;
	case kAmp4:
		pga_Change_Amp(kAmp8,_currentChannel);
		break;
	case kAmp8:
		pga_Change_Amp(kAmp16,_currentChannel);
		break;
	case kAmp16:
		pga_Change_Amp(kAmp32,_currentChannel);
		break;
	case kAmp32:
		pga_Change_Amp(kAmp64,_currentChannel);
		break;
	case kAmp64:
		pga_Change_Amp(kAmp128,_currentChannel);
		break;
	case kAmp128:  //maximum amplification, cannot increase
		//puts("Trying to increase amplification when its the max amplification");
		return false;
		break;
	default:
		//puts("Unexpected amplification");
		break;
	}
	return true;
}

bool DecreaseAmplification(void)
{
	switch (_currentAmp) {
	case kAmp1:  //minimum amplification, cannot increase
		//puts("Trying to decrease amplification when its the min amplification");
		return false;
		break;
	case kAmp2:
		pga_Change_Amp(kAmp1,_currentChannel);
		break;
	case kAmp4:
		pga_Change_Amp(kAmp2,_currentChannel);
		break;
	case kAmp8:
		pga_Change_Amp(kAmp4,_currentChannel);
		break;
	case kAmp16:
		pga_Change_Amp(kAmp8,_currentChannel);
		break;
	case kAmp32:
		pga_Change_Amp(kAmp16,_currentChannel);
		break;
	case kAmp64:
		pga_Change_Amp(kAmp32,_currentChannel);
		break;
	case kAmp128:
		pga_Change_Amp(kAmp64,_currentChannel);
		break;
	default:
		//puts("Unexpected amplification");
		break;
	}
	return true;
}

/**
 * \brief Start SPI transfer test.
 */
static void spi_master_go(void)
{
	uint32_t cmd;

	cmd = CMD_TEST;
	//puts("-> Master sending CMD_TEST... \r");
	spi_write(SPI_MASTER_BASE, 0b01010000,0,0);
	while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);
	spi_write(SPI_MASTER_BASE, 0b01111000,0,0);
	//spi_master_transfer(&cmd, sizeof(cmd));
	if (cmd == RC_RDY) {
		//puts("   <- Slave response RC_SYN, RC_RDY \r");
		return;
	}
	if (cmd != RC_SYN) {
		//printf("-E- Response unexpected: 0x%x \n\r", (unsigned)cmd);
		return;
	}
}

static void spi_master_open_col(uint8_t col)
{
	spi_write(SPI_MASTER_BASE,col,0,0);
	while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);
}

static void spi_master_test_write(void)
{
	uint32_t cmd;
	cmd = CMD_TEST;
	spi_write(SPI_MASTER_BASE,cmd,0,0); //1
	//spi_write(SPI_MASTER_BASE,cmd,0,0); //1
	while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);
	//spi_write(SPI_MASTER_BASE,0b00000010,0,0); //2
	//while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);
}

static void spi_master_test_read(void)
{
	uint8_t uc_pcs;
	static uint16_t data;
	spi_write(SPI_MASTER_BASE, 0b00000001, 0, 0);
	/* Wait transfer done. */
	while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);
	spi_read(SPI_MASTER_BASE, &data, &uc_pcs);
	//printf("data: %d\r\n", data);
}	

	/// @cond 0
	/**INDENT-OFF**/
	#ifdef __cplusplus
}


#endif
/**INDENT-ON**/
/// @endcond

#endif /* CONF_SPI_H_ */