/*
 * conf_dacc.h
 *
 * Created: 02/03/2023 14:50:07
 *  Author: DvirShavit
 */ 


#ifndef CONF_DACC_H_
#define CONF_DACC_H_

/** The DAC Channel value */
#define DACC_CHANNEL 1
/** Analog control value */
#define DACC_ANALOG_CONTROL (DACC_ACR_IBCTLCH0(0x02) \
						  | DACC_ACR_IBCTLCH1(0x02) \
						  | DACC_ACR_IBCTLDACCORE(0x01))
/**
 * \brief Initialize DACC
 */
//static void Init_DACC(void)
void Init_DACC(void)
{
	/* Initialize DACC */
	/* Enable clock for DACC */
	pmc_enable_periph_clk(ID_DACC);
	/* Reset DACC registers */
	dacc_reset(DACC);
	/* External trigger mode disabled. DACC in free running mode. */
	dacc_disable_trigger(DACC);
	/* Half word transfer mode */
	dacc_set_transfer_mode(DACC, 0);
	/* Timing:
		 * max speed mode -    0 (disabled)
		 * startup time   - 0xf (960 dacc clocks)
		 */
	dacc_set_timing(DACC, 0, 0xf);	
	/* Disable TAG and select output channel DACC_CHANNEL */					  
	dacc_set_channel_selection(DACC, DACC_CHANNEL);
	/* Enable output channel DACC_CHANNEL */
	dacc_enable_channel(DACC, DACC_CHANNEL);
	/* Setup analog current */
	dacc_set_analog_control(DACC, DACC_ANALOG_CONTROL);
	/* Set DAC0 output*/
	dacc_write_conversion_data(DACC, 4095);
}

#endif /* CONF_DACC_H_ */