// Author: Matt Delengowski
// Lab: 3
// Section: Timer A Blink
// Device: FR5994
#include <msp430.h> 

volatile unsigned int LED_COUNTER;
volatile unsigned int frequency_count = 5;

int LED_frequency();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= BIT0;                          // Set P1.0 to output direction -- LED
    P1DIR |= BIT1;                          // Set P1.1 to output direction -- LED

    P5DIR &= ~BIT6;                         // Set P5.6 to button input
    P5REN |= BIT6;                          // Enable Pull Up Resistor for pin5.6
    P5OUT |= BIT6;                          //SPECIFIED AS A PULLUP FOR P5.6
    P5IES |= BIT6;                          //Have flag set on High to Low
    P5IE |= BIT6;                           //enable interrupts for Pin 5.6
    P5IFG &= ~BIT6;                         //Clear Pin 5.6 flag so no immediate interrupt

    P5DIR &= ~BIT5;                         // Set P5.5 to button input
    P5REN |= BIT5;                          // Enable Pull Up Resistor for pin5.5
    P5OUT |= BIT5;                          //SPECIFIED AS A PULLUP FOR P5.5
    P5IES |= BIT5;                          //Have flag set on High to Low
    P5IE |= BIT5;                           //enable interrupts for Pin 5.5
    P5IFG &= ~BIT5;                         //Clear Pin 5.5 flag so no immediate interrupt

    TA0CCTL0 |= CCIE;                        // TACCR0 interrupt enabled
    TA0CCR0 |= LED_frequency(frequency_count);//Enter int value for LED_frequency for LED 1.0 to blink at that freq (starts at 1hz)
                                                        //LED 1.1 will blink at 1/5 of that frequency

    TA0CTL |= TASSEL_1 + MC_1;
    __enable_interrupt();

    for(;;)
    {   if(frequency_count > 1)
        {
            TA0CCR0 |= LED_frequency(frequency_count);//Enter int value for LED_frequency for LED 1.0 to blink at that freq (starts at 1hz)
                                                    //LED 1.1 will blink at 1/5 of that frequency
        }
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{


    P1OUT ^= BIT0;

    if(P1OUT & BIT0)     //Only increment when P1.0 is on
    {
        LED_COUNTER++;
    }

    if (LED_COUNTER == 5) //blink P9.7 at 1/5 the frequency of P1.0
    {
        P1OUT ^= BIT1;
        LED_COUNTER = 0;
    }

}

#pragma vector = PORT5_VECTOR
__interrupt void P5_ISR(void)
{
    switch(P5IV)
    {
    case P5IV_P5IFG6:
        {
            frequency_count++;
            LED_COUNTER = 0;
            break;
        }
    case P5IV_P5IFG5:
        {
            frequency_count = 1;                       //reset blink frequency to 1hz
            break;
        }
    }
}

int LED_frequency(int frequency)
{
    int result;

    result = 32768/frequency;

    return result;
}
