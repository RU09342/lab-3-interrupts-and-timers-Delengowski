// Author: Matt Delengowski
// Lab: 3
// Section: Timer A Blink
// Device: G2553
#include <msp430.h> 

volatile unsigned int LED_COUNTER;
volatile unsigned int frequency_count = 5;

int LED_frequency();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT0;                          // Set P1.0 to output direction -- LED
    P1DIR |= BIT6;                          // Set P1.6 to output direction -- LED

    P1OUT &= ~BIT0; //Toggle off P1.0 LED
    P1OUT &= ~BIT6; // Toggle off P1.6 LED

    P1DIR &= ~BIT3;                         // Set P1.3 to button input
    P1REN |= BIT3;                          // Enable Pull Up Resistor for pin1.3
    P1OUT |= BIT3;                          //SPECIFIED AS A PULLUP FOR P1.3

    TA0CCTL0 |= CCIE;                        // TACCR0 interrupt enabled
    TA0CCR0 |= LED_frequency(frequency_count);//Enter int value for LED_frequency for LED 1.0 to blink at that freq (starts at 1hz)
                                                        //LED 9.7 will blink at 1/5 of that frequency

    TA0CTL |= TASSEL_1 + MC_1 + TACLR;  //use ACLK in up mode, clear timer
    __enable_interrupt();

    for(;;)
    {   if(frequency_count > 1)
        {
            TA0CCR0 |= LED_frequency(frequency_count);//Enter int value for LED_frequency for LED 1.0 to blink at that freq (starts at 1hz)
                                                    //LED 9.7 will blink at 1/5 of that frequency
        }
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{


    P1OUT ^= BIT0;   //toggle P1.0 LED

    if(P1OUT & BIT0)     //Only increment when P1.0 is on
    {
        LED_COUNTER++;
    }

    if (LED_COUNTER == 5) //blink P1.6 at 1/5 the frequency of P1.0
    {
        P1OUT ^= BIT6;   //togggle P1.6 LED
        LED_COUNTER = 0;
    }

}


int LED_frequency(int frequency)
{
    int result;

    result = 32768/frequency;

    return result;
}
