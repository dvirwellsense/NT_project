/*
 * conf_rtt.h
 *
 * Created: 1/4/2023 2:37:23 PM
 *  Author: DvirShavit
 */ 


#ifndef CONF_RTT_H_
#define CONF_RTT_H_

#define TICK  32768 //= 1sec //8192 //=250ms//1 or 2 is forbidden according to the datasheet. RTT
static bool print = false;
/**
 * \brief RTT configuration function.
 *
 * Configure the RTT to generate a ticks, which triggers the RTTINC
 * interrupt.
 */
//static uint32_t restart_rtt(void)
uint32_t restart_rtt(void)
{
	uint32_t ul_time;
	ul_time = rtt_read_timer_value(RTT);
	rtt_init(RTT, TICK);

	/* Enable RTT interrupt */
	NVIC_DisableIRQ(RTT_IRQn);
	NVIC_ClearPendingIRQ(RTT_IRQn);
	NVIC_SetPriority(RTT_IRQn, 1);
	NVIC_EnableIRQ(RTT_IRQn);
	rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
	return ul_time;
}

/**
 * \brief RTT configuration function.
 *
 * Configure the RTT to generate a (one second) tick, which triggers the RTTINC
 * interrupt.
 */
//static void start_rtt(void)
void start_rtt(void)
{
	///* Configure RTT for a 1 second tick interrupt */
	rtt_init(RTT, TICK);

	/* Enable RTT interrupt */
	NVIC_DisableIRQ(RTT_IRQn);
	NVIC_ClearPendingIRQ(RTT_IRQn);
	NVIC_SetPriority(RTT_IRQn, 1);
	NVIC_EnableIRQ(RTT_IRQn);
	rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
}

/**
 * \brief Interrupt handler for the RTT.
 *
 * Display the current time on the terminal.
 */
void RTT_Handler(void)
{
	//uint32_t ul_status, ul_time;
	///* Get RTT status */
	rtt_get_status(RTT);
	//ul_time = rtt_read_timer_value(RTT);
	//printf("time: %u", ul_time);
	print = true;
}


#endif /* CONF_RTT_H_ */