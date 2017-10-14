// Author: Matt Delengowski
// Lab: 3
// Section: Button Interrupt
// Device: FR2311
#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

     P1DIR |= BIT0;                          // Set P1.0 LED to output direction
     P2DIR |= BIT0;                          // Set P2.0 LED to output direction

     P1DIR &= ~BIT1;                         // Set P1.1 to button input
     P1REN |= BIT1;                          // Enable Pull Up Resistor for pin1.1
     P1OUT |= BIT1;                          //SPECIFIED AS A PULLUP FOR P1.1
     P1IES |= BIT1;                          //Have flag set on High to Low
     P1IE |= BIT1;                           //enable interrupts for Pin 1.1
     P1IFG &= ~BIT1;                         //Clear Pin 1.1 flag so no immediate interrupt

     __enable_interrupt();




            for(;;) {
                volatile unsigned int i;            // volatile to prevent optimization

                P2OUT &= ~BIT0;
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

    while(!(BIT1 & P1IN))
    {
    P1OUT &= ~BIT0;                     //Turn P1.0 LED OFF
    P2OUT ^= BIT0;                      // Toggle P2.0 LED
    j = 10000;
    do j--;
    while(j != 0);
    }

    P1IFG &= ~BIT1; // Clear Pin 1.3 Flag

}
