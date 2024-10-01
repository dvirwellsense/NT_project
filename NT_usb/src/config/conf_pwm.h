/*
 * conf_pwm.h
 *
 * Created: 12/5/2022 3:38:25 PM
 *  Author: DvirShavit
 */ 


#ifndef CONF_PWM_H_
#define CONF_PWM_H_

#include "conf_afec.h"

/** Reference voltage for AFEC in mv. */
#define VOLT_REF        (3300)

/** PWM frequency in Hz */
#define DEFAULT_FREQUENCY      100000//Hz
/** Min frequency */
#define MIN_FREQUENCY   5
/** Max frequency */
#define MAX_FREQUENCY   400000
/** Invalid value */
#define VAL_INVALID     0xFFFFFFFF
/** Period value of PWM output waveform */
#define PERIOD_VALUE       50
/** Initial duty cycle value */
#define INIT_DUTY_VALUE    25

/** PWM channel instance */
pwm_channel_t pwm_channel;

static void setFrequency(uint32_t newFreq)
{
	/* Disable PWM channels for LEDs */
	pwm_channel_disable(PWM, PIN_PWM_LED1_CHANNEL);
	/* Set PWM clock A as DEFAULT_FREQUENCY*PERIOD_VALUE (clock B is not used) */
	pwm_clock_t clock_setting = {
		.ul_clka = newFreq * PERIOD_VALUE,
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	afec_sample_data.value = 0;
	afec_sample_data.is_done = false;
	
	pwm_init(PWM, &clock_setting);

	/* Initialize PWM channel */
	/* Period is left-aligned */
	pwm_channel.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	pwm_channel.polarity = PWM_HIGH;
	/* Use PWM clock A as source clock */
	pwm_channel.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	pwm_channel.ul_period = PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	pwm_channel.ul_duty = INIT_DUTY_VALUE;
	pwm_channel.channel = PIN_PWM_LED1_CHANNEL;

	pwm_channel_init(PWM, &pwm_channel);
	pwm_channel_enable_interrupt(PWM, PIN_PWM_LED1_CHANNEL, 0);
	pwm_channel_enable(PWM, PIN_PWM_LED1_CHANNEL);
}

static void InitPWM()
{
	pmc_enable_periph_clk(ID_PWM);
	NVIC_DisableIRQ(PWM_IRQn);
	NVIC_ClearPendingIRQ(PWM_IRQn);
	NVIC_SetPriority(PWM_IRQn, 1);
	NVIC_EnableIRQ(PWM_IRQn);
	setFrequency(DEFAULT_FREQUENCY);
}

void PWM_Handler(void)
{
	uint32_t events = pwm_channel_get_interrupt_status(PWM);
	/* Interrupt on PIN_PWM_LED1_CHANNEL */
	if ((events & (1 << PIN_PWM_LED1_CHANNEL)) == (1 << PIN_PWM_LED1_CHANNEL)) 
	{
		pwm_channel.channel = PIN_PWM_LED1_CHANNEL;
	}
}

#endif /* CONF_PWM_H_ */