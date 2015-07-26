#ifndef __HB_DRIVER_H__
#define __HB_DRIVER_H__

#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <inttypes.h>


// ADC input on PF7 (Port A0 on Arduino board)
#define	ADC_INPUT			PF7
#define ADC_PORT			PORTF
#define ADC_DDR				DDRF

// PWM output on PB7 (Port D11 - PWM8/16 on Arduino board)
#define PWM_OUTPUT			PB7
#define PWM_PORT			PORTB
#define PWM_DDR				DDRB

# endif