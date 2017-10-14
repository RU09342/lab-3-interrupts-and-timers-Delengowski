// Author: Matt Delengowski
// Lab: 3
// Section: Button Delay
// Device: G2553
#include <msp430.h> 

volatile unsigned int button_count;
volatile unsigned int TimerA1_count;

void LEDSetup();
void ButtonSetup();
void TimerA0Setup();
void TimerA1Setup();

//NO BUTTON RESET FOR THIS BOARD, DUE TO IT HAVING A BUILT IN RESET BUTTON.

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // stop watchdog timer

    LEDSetup();
    ButtonSetup();
    TimerA0Setup();
    TimerA1Setup();

    __enable_interrupt();



    for(;;)
    {
        while(!(P1IN & BIT3))
        {
            if(TA1R == 0)
            {
                TA1CTL |= TASSEL_1 + MC_1 + TACLR;    //Pick ACLK & Set Counter to Up Mode
            }
        }
    }

}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0_ISR (void)
{
    P1OUT ^= BIT0;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void TimerA1_ISR (void)
{

    TimerA1_count++;
    if(TimerA1_count == 8)
    {
        TimerA1_count = 0; //Only one clock divider was avaible, so for TimerA1 must trigger 8 times before LED P1.6 toggles
        P1OUT ^= BIT6;

        if((P1OUT & BIT6)) //only increment when LED 1.6 turns on
        {
        button_count++;
        }
    }


}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)
{
    TA1CTL &= ~TASSEL_1;                   //disable TA1
    switch(button_count)                     //slow down blink speed, by number of times LED 9.7 turns on
    {
        case 1:
        {
            TA0CCR0 = 3277;
            break;
        }
        case 2:
        {
            TA0CCR0 = 6554;
            break;
        }
        case 3:
        {
            TA0CCR0 = 9831;
            break;
        }
        case 4:
        {
            TA0CCR0 = 13108;
            break;
        }
        case 5:
        {
            TA0CCR0 = 16385;
            break;
        }
        case 6:
        {
            TA0CCR0 = 19662;
            break;
        }
        case 7:
        {
            TA0CCR0 = 22939;
            break;
        }
        case 8:
        {
            TA0CCR0 = 26216;
            break;
        }
        case 9:
        {
            TA0CCR0 = 29493;
            break;
        }
        case 10:
        {
            TA0CCR0 = 32770;
            break;
        }
        case 11:
        {
            TA0CCR0 = 36047;
            break;
        }
    };
    TA1CTL |= TACLR;                      //Clear Timer A1
    button_count = 0;
    P1OUT &= ~BIT6;                       //Disable LED P1.6
}

void LEDSetup()
{
    P1DIR |= BIT0;                          // Set P1.0 to output direction -- LED
    P1DIR |= BIT6;                          // Set P1.6 to output direction --- LED

    P1OUT &= ~BIT6;
}

void ButtonSetup()
{
    P1DIR &= ~BIT3;                         // Set P1.3 to button input
    P1REN |= BIT3;                          // Enable Pull Up Resistor for pin1.3
    P1OUT |= BIT3;                          //SPECIFIED AS A PULLUP FOR P1.3
    P1IES &= ~BIT3;                          //Have flag set on Low to High
    P1IE |= BIT3;                           //enable interrupts for Pin 1.3
    P1IFG &= ~BIT3;                         //Clear Pin 1.3 flag so no immediate interrupt
}

void TimerA0Setup()
{
    TA0CCTL0 |= CCIE;                        // TA0CCR0 interrupt enabled
    TA0CCR0 = 3277;                          //Set value of TA0CCR0 for comparison
    TA0CTL |= TASSEL_1 + MC_1 + TACLR;              //Pick ACLK & Set Counter to Up Mode
}

void TimerA1Setup()
{
    TA1CCTL0 |= CCIE;                      // TA1CCR0 interrupt enabled
    TA1CCR0 = 512;                         //Set value of TA1CCR0 for comparison
    TA1CTL |= ID_3;                        //Divide clock by 8, ACLK will be chosen, so 4096 hz
    TA1CTL |= TACLR;                       //Clear Timer A1
}
