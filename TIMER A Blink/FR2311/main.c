// Author: Matt Delengowski
// Lab: 3
// Section: Timer A Blink
// Device: FR2311
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
    P2DIR |= BIT0;                          // Set P2.0 to output direction -- LED

    P1DIR &= ~BIT1;                         // Set P1.1 to button input
    P1REN |= BIT1;                          // Enable Pull Up Resistor for pin1.1
    P1OUT |= BIT1;                          //SPECIFIED AS A PULLUP FOR P1.1
    P1IES |= BIT1;                          //Have flag set on High to Low
    P1IE |= BIT1;                           //enable interrupts for Pin 1.1
    P1IFG &= ~BIT1;                         //Clear Pin 1.1 flag so no immediate interrupt

    TB0CCTL0 |= CCIE;                        // TACCR0 interrupt enabled
    TB0CCR0 |= LED_frequency(frequency_count);//Enter int value for LED_frequency for LED 1.0 to blink at that freq (starts at 1hz)
                                                        //LED 9.7 will blink at 1/5 of that frequency

    TB0CTL |= TBSSEL_1 + MC_1;
    __enable_interrupt();

    for(;;)
    {   if(frequency_count > 1)
        {
            TB0CCR0 |= LED_frequency(frequency_count);//Enter int value for LED_frequency for LED 1.0 to blink at that freq (starts at 1hz)
                                                    //LED 2.0 will blink at 1/5 of that frequency
        }
    }
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0 (void)
{


    P1OUT ^= BIT0;

    if(P1OUT & BIT0)     //Only increment when P1.0 is on
    {
        LED_COUNTER++;
    }

    if (LED_COUNTER == 5) //blink P2.0 at 1/5 the frequency of P1.0
    {
        P2OUT ^= BIT0;
        LED_COUNTER = 0;
    }

}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)
{
    switch(P1IV)
    {
    case P1IV_P1IFG1:
        {
            frequency_count++;
            LED_COUNTER = 0;
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
