// Author: Matt Delengowski
// Lab: 3
// Section: Button Interrupt
// Device: FR5994
#include <msp430.h> 

volatile unsigned int button_count;

void LEDSetup();
void ButtonSetup();
void TimerA0Setup();
void TimerA1Setup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                              // to activate previously configured port settings

    LEDSetup();
    ButtonSetup();
    TimerA0Setup();
    TimerA1Setup();

    __enable_interrupt();



    for(;;)
    {
        while(!(P5IN & BIT6))
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
    P1OUT ^= BIT1;
    if((P1OUT & BIT1)) //only increment when LED 1.1 turns on
    {
    button_count++;
    }
}

#pragma vector = PORT5_VECTOR
__interrupt void P5_ISR(void)
{
    switch(P5IV)
    {
    case P5IV_P5IFG6:
        {
            TA1CTL &= ~TASSEL_1;                   //disable TA1
            TA0CCR0 = 3277*button_count;          //slow down blink speed, by number of times LED 9.7 turns on
            TA1CTL |= TACLR;                      //Clear Timer A1
            button_count = 0;
            P1OUT &= ~BIT1;                       //Disable LED P1.1
            break;
        }
    case P5IV_P5IFG5:
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
    P1DIR |= BIT1;                          // Set P1.1 to output direction --- LED

    P1OUT &= ~BIT1;
}

void ButtonSetup()
{
    P5DIR &= ~BIT5;                         // Set P5.5 to button input
    P5REN |= BIT5;                          // Enable Pull Up Resistor for pin5.5
    P5OUT |= BIT5;                          //SPECIFIED AS A PULLUP FOR 5.5
    P5IES |= BIT5;                          //Have flag set on Low to High
    P5IE |= BIT5;                           //enable interrupts for Pin 5.5
    P5IFG &= ~BIT5;                         //Clear Pin 5.5 flag so no immediate interrupt

    P5DIR &= ~BIT6;                         // Set P5.6 to button input
    P5REN |= BIT6;                          // Enable Pull Up Resistor for pin5.6
    P5OUT |= BIT6;                          //SPECIFIED AS A PULLUP FOR 5.6
    P5IES &= ~BIT6;                          //Have flag set on High to Low
    P5IE |= BIT6;                           //enable interrupts for Pin 5.6
    P5IFG &= ~BIT6;                         //Clear Pin 5.6 flag so no immediate interrupt
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
