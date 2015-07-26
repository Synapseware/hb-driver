#include "hb-driver.h"


//-------------------------------------------------------
// Timer0 is used for 8-bit PWM output, clk/1, OC0A
void init_timer0(void)
{
    // Timer/Counter Control Register A
    TCCR0A  =   (1<<COM0A1)  |	// Toggle on OC0A
                (0<<COM0A0)  |	// ...
                (1<<WGM00)   |	// Fast PWM
                (1<<WGM00);		// Fast PWM

    // Timer/Counter Control Register B
    TCCR0B  =   (0<<WGM02)  |	// Fast PWM
                (0<<CS02)   |	// clk/1
                (1<<CS01)   |	// ...
                (1<<CS00);		// ...

    // Output Compare Register A
    OCR0A   =   0;

    // Output Compare Register B
    OCR0B   =   0xFF;

    // Set PWM OC0A as output, start low
    PWM_PORT &= ~(1<<PWM_OUTPUT);
    PWM_DDR	|=	(1<<PWM_OUTPUT);
}


//-------------------------------------------------------
// The ADC is hardwired to read from ADC7, free-running, self triggering
void init_adc(void)
{
    // ADC Multiplexer Selection Register
    ADMUX   =   (0<<REFS1)  |   // Analog reference
                (1<<REFS0)  |   // ...
                (1<<ADLAR)  |   // Left Adjust Result (so we can just read ADCH and be done)
                (0<<MUX4)   |   // Bits 4:0 – MUX4:0: Analog Channel Selection
                (0<<MUX3)   |   // ADC0
                (0<<MUX2)   |   // ADC0
                (0<<MUX1)   |   // ADC0
                (0<<MUX0);      // ADC0

    // ADC Control and Status Register A
    ADCSRA  =   (1<<ADEN)   |   // ADC Enable
                (0<<ADSC)   |   // Don't start a conversion
                (1<<ADATE)  |   // Enable ADC Auto Trigger Enable
                (1<<ADIE)   |   // ADC Interrupt Enable
                (1<<ADPS2)  |   // XTAL/128
                (1<<ADPS1)  |   // XTAL/128
                (1<<ADPS0);     // XTAL/128

    // ADC Control and Status Register B
    ADCSRB  =   (0<<MUX5)   |   // ADC0
                (0<<ADTS3)  |   // Free-running
                (0<<ADTS2)  |   // ...
                (0<<ADTS1)  |   // ...
                (0<<ADTS0);     // ...

    // Disable the digital input on the ADC input line
    DIDR0 |= (1<<ADC_ON);

    // Set the ADC pin as input
    ADC_DDR &= ~(1<<ADC_INPUT);}


//-------------------------------------------------------
// Prepare the chip
void setup(void)
{
	init_timer0();
	init_adc();

	power_adc_enable();
	power_timer0_enable();

	// start the first conversion!
	ADCSRA |= (1<<ADSC);

	// turn on the on-board LED
	LED_DDR |= (1<<LED_PIN);
	LED_PORT &= ~(1<<LED_PIN);

	sei();
}


//-------------------------------------------------------
// Main
int main(void)
{
	setup();

	while(1)
	{
		//no-op
	}

	return 0;
}


//-------------------------------------------------------
// ADC conversion complete.  ADLAR should be set
//volatile int data[4];
ISR(ADC_vect)
{
	uint8_t adc = ADCH;

	// save the diminishing average
	/*
	data[3] = data[2] >> 1;
	data[2] = data[1] >> 1;
	data[1] = data[0] >> 1;
	data[0] = adc;

	// take the average of the data
	int avg = (data[0] + data[1] + data[2] + data[3]) / 4;
	*/

	// set the PWM value to the new average
	OCR0A = adc & 0xFF;

	// diag
	if (OCR0A > 128)
	{
		LED_PORT |= (1<<LED_PIN);
	}
	else
	{
		LED_PORT &= ~(1<<LED_PIN);
	}
}
