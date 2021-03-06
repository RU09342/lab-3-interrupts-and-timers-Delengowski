// Author: Matt Delengowski
// Lab: 3
// Section: Button Delay
// Device: F5529
#include <msp430.h> 

volatile unsigned int button_count;

void LEDSetup();
void ButtonSetup();
void TimerA0Setup();
void TimerA1Setup();

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
        while(!(P2IN & BIT1))
        {
            if(TA1R == 0)
            {
                TA1CTL |= TASSEL_1 + MC_1 + TACLR;    //Pick ACLK & Set Counter to Up Mode
            }
        }

        //if(P1IN & BIT1)
        {
          //  TA1CTL &= ~TASSEL_1;
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
    P4OUT ^= BIT7;
    if((P4OUT & BIT7)) //only increment when LED 4.7 turns on
    {
    button_count++;
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void P2_ISR(void)
{
    switch(P2IV)
    {
    case P2IV_P2IFG1:
        {
            TA1CTL &= ~TASSEL_1;                   //disable TA1
            TA0CCR0 = 3277*button_count;          //slow down blink speed, by number of times LED 9.7 turns on
            TA1CTL |= TACLR;                      //Clear Timer A1
            button_count = 0;
            P4OUT &= ~BIT7;                       //Disable LED P4.7
            break;
        }
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)
{
    switch(P1IV)
    {
        case P1IV_P1IFG1:
        {
            TA0CCR0 = 3277;                       //reset blink speed
            button_count = 0;                     //clear button_count
            break;
        }
    }
}

void LEDSetup()
{
    P1DIR |= BIT0;                          // Set P1.0 to output direction -- LED
    P4DIR |= BIT7;                          // Set P4.7 to output direction --- LED

    P4OUT &= ~BIT7;
}

void ButtonSetup()
{
    P2DIR &= ~BIT1;                         // Set P2.1 to button input
    P2REN |= BIT1;                          // Enable Pull Up Resistor for pin2.1
    P2OUT |= BIT1;                          //SPECIFIED AS A PULLUP FOR P2.1
    P2IES &= ~BIT1;                          //Have flag set on Low to High
    P2IE |= BIT1;                           //enable interrupts for Pin 2.1
    P2IFG &= ~BIT1;                         //Clear Pin 2.1 flag so no immediate interrupt

    P1DIR &= ~BIT1;                         // Set P1.1 to button input
    P1REN |= BIT1;                          // Enable Pull Up Resistor for pin1.1
    P1OUT |= BIT1;                          //SPECIFIED AS A PULLUP FOR P1.1
    P1IES |= BIT1;                          //Have flag set on High to Low
    P1IE |= BIT1;                           //enable interrupts for Pin 1.1
    P1IFG &= ~BIT1;                         //Clear Pin 1.1 flag so no immediate interrupt
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
    TA1EX0 |= TAIDEX_7;                    //Divide ACLK further by 8 to 512 hz
    TA1CTL |= TACLR;                       //Clear Timer A1
}
