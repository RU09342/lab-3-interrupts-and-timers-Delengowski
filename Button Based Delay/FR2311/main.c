// Author: Matt Delengowski
// Lab: 3
// Section: Button Delay
// Device: FR2311
#include <msp430.h> 

volatile unsigned int button_count;

void LEDSetup();
void ButtonSetup();
void TimerB0Setup();
void TimerB1Setup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                              // to activate previously configured port settings

    LEDSetup();
    ButtonSetup();
    TimerB0Setup();
    TimerB1Setup();

    __enable_interrupt();



    for(;;)
    {
        while(!(P1IN & BIT1))
        {
            if(TB1R == 0)
            {
                TB1CTL |= TBSSEL_1 + MC_1 + TBCLR;    //Pick ACLK & Set Counter to Up Mode
            }
        }

        //if(P1IN & BIT1)
        {
          //  TA1CTL &= ~TASSEL_1;
        }
    }

}


#pragma vector = TIMER0_B0_VECTOR
__interrupt void TimerB0_ISR (void)
{
    P1OUT ^= BIT0;
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void TimerB1_ISR (void)
{
    P2OUT ^= BIT0;
    if((P2OUT & BIT0)) //only increment when LED 9.7 turns on
    {
    button_count++;
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)
{
    switch(P1IV)
    {
    case P1IV_P1IFG1:
        {
            TB1CTL &= ~TBSSEL_1;                   //disable TA1
            TB0CCR0 = 3277*button_count;          //slow down blink speed, by number of times LED 9.7 turns on
            TB1CTL |= TBCLR;                      //Clear Timer A1
            button_count = 0;
            P2OUT &= ~BIT0;                       //Disable LED P2.0
            break;
        }
    }
}

void LEDSetup()
{
    P1DIR |= BIT0;                          // Set P1.0 to output direction -- LED
    P2DIR |= BIT0;                          // Set P2.0 to output direction --- LED

    P2OUT &= ~BIT0;
}

void ButtonSetup()
{
    P1DIR &= ~BIT1;                         // Set P1.1 to button input
    P1REN |= BIT1;                          // Enable Pull Up Resistor for pin1.1
    P1OUT |= BIT1;                          //SPECIFIED AS A PULLUP FOR P1.1
    P1IES &= ~BIT1;                          //Have flag set on Low to High
    P1IE |= BIT1;                           //enable interrupts for Pin 1.1
    P1IFG &= ~BIT1;                         //Clear Pin 1.1 flag so no immediate interrupt
}

void TimerB0Setup()
{
    TB0CCTL0 |= CCIE;                        // TA0CCR0 interrupt enabled
    TB0CCR0 = 3277;                          //Set value of TA0CCR0 for comparison
    TB0CTL |= TBSSEL_1 + MC_1 + TBCLR;              //Pick ACLK & Set Counter to Up Mode
}

void TimerB1Setup()
{
    TB1CCTL0 |= CCIE;                      // TA1CCR0 interrupt enabled
    TB1CCR0 = 512;                         //Set value of TA1CCR0 for comparison
    TB1CTL |= ID_3;                        //Divide clock by 8, ACLK will be chosen, so 4096 hz
    TB1EX0 |= TBIDEX_7;                    //Divide ACLK further by 8 to 512 hz
    TB1CTL |= TBCLR;                       //Clear Timer A1
}
