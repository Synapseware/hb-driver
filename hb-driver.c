#include "hb-driver.h"


volatile int	_adc_result = 0;

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
    TCCR0B  =   (1<<WGM02)  |	// Fast PWM
                (0<<CS02)   |	// clk/1
                (0<<CS01)   |	// ...
                (1<<CS00);		// ...

    // Output Compare Register A
    OCR0A   =   127;
    
    // Output Compare Register B
    OCR0B   =   127;

    // Timer/Counter Interrupt Mask Register
    TIMSK0  =   0;

    // Set PWM OC0A as output
    PWM_PORT &= ~(1<<PWM_OUTPUT);
    PWM_DDR	|=	(1<<PWM_OUTPUT);
}


//-------------------------------------------------------
void init_adc(void)
{
    // ADC Multiplexer Selection Register
    ADMUX   =   (0<<REFS1)  |   // 5v ref
                (0<<REFS0)  |   // ...
                (1<<ADLAR)  |   // Left Adjust Result (so we can just read ADCH and be done)
                (0<<MUX4)   |   // Bits 4:0 – MUX4:0: Analog Channel Selection
                (0<<MUX3)   |   // ADC7
                (1<<MUX2)   |   // ADC7
                (1<<MUX1)   |   // ADC7
                (1<<MUX0);      // ADC7

    // ADC Control and Status Register A
    ADCSRA  =   (1<<ADEN)   |   // Bit 7 – ADEN: ADC Enable
                (0<<ADSC)   |   // Bit 6 – ADSC: ADC Start Conversion
                (0<<ADATE)  |   // Bit 5 – ADATE: ADC Auto Trigger Enable
                (0<<ADIF)   |   // Bit 4 – ADIF: ADC Interrupt Flag
                (1<<ADIE)   |   // Bit 3 – ADdIE: ADC Interrupt Enable
                (1<<ADPS2)  |   // XTAL/128
                (1<<ADPS1)  |   // XTAL/128
                (1<<ADPS0);     // XTAL/128

    // ADC Control and Status Register B
    ADCSRB  =   (0<<ADHSM)  |   // Bit 7 – ADHSM: ADC High Speed Mode
                (0<<ACME)   |   // Bit 6 – ACME: Analog Comparator Mux Enable
                (0<<MUX5)   |   // ADC7
                (0<<ADTS3)  |   // T/C1 - Compare Match B
                (1<<ADTS2)  |   // T/C1 - Compare Match B
                (0<<ADTS1)  |   // T/C1 - Compare Match B
                (1<<ADTS0);     // T/C1 - Compare Match B
    
    // Digital Input Disable Register 0
    // Bit 7:4, 1:0 – ADC1:0 Digital Input Disable
    DIDR0   =   (1<<ADC7D);     // Bit 0

    // Set the ADC pin as input
    ADC_DDR &= ~(1<<ADC_INPUT);
}


//-------------------------------------------------------
void setup(void)
{
	init_timer0();
	init_adc();

	sei();
}


//-------------------------------------------------------
int main(void)
{
	setup();

	while(1)
	{

	}

	return 0;
}


//-------------------------------------------------------
// ADC conversion complete.  ADLAR should be set
ISR(ADC_vect)
{
	// just set OCR0A to the ADCH result.
	OCR0A = ADCH;
}
