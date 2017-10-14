// Author: Matt Delengowski
// Lab: 3
// Section: Button Interrupt
// Device: G2553
#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

     P1DIR |= BIT0;                          // Set P1.0 LED to output direction
     P1DIR |= BIT6;                          // Set P1.6 LED to output direction

     P1DIR &= ~BIT3;                         // Set P1.3 to button input
     P1REN |= BIT3;                          // Enable Pull Up Resistor for pin1.3
     P1OUT |= BIT3;                          //SPECIFIED AS A PULLUP FOR P1.3
     P1IES |= BIT3;                          //Have flag set on High to Low
     P1IE |= BIT3;                           //enable interrupts for Pin 1.3
     P1IFG &= ~BIT3;                         //Clear Pin 1.3 flag so no immediate interrupt

     __enable_interrupt();




            for(;;) {
                volatile unsigned int i;            // volatile to prevent optimization

                P1OUT &= ~BIT6;
                P1OUT ^= BIT0;                      // Toggle P1.0 using exclusive-OR
                i = 10000;                          // SW Delay
                do i--;
                while(i != 0);


            }
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)
{
    volatile unsigned int j;

    while(!(BIT3 & P1IN))
    {
    P1OUT &= ~BIT0;                     //Turn P1.0 LED OFF
    P1OUT ^= BIT6;                      // Toggle P1.6 LED
    j = 10000;
    do j--;
    while(j != 0);
    }

    P1IFG &= ~BIT3; // Clear Pin 1.3 Flag

}
