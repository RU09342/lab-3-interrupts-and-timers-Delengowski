# Timer A Blink 
## Goal
To contorl the blinking of LEDs using timer modules and ISR as opposed to polling. The benefit of using the Timers is that they allow power savings by utilizing subroutines and low power modes. This power savings is the real beauty of the MSP430 line.
## Code
This lab was implemented using TimerA (except for the FR2311, which only has a TimerB) in up mode to compare against CCR0. When TA reaches the value of CCR0, LED1 is toggled (using XOR) if LED1 is on from this toggle, a variable gets incremented. Once this variable reaches the value of 5 (which would take TA hitting CCRO 10 times), LED2 is toggled. So, the second LED blinks at 1/10th the speed of the first. 

In some of my code, you will see an attempt to change the frequency of the LED by adjusting CCR0. This was done using a function call and button interrupts that incremented a variable when pressed. This variable was the arugment to the function. This didn't work however, I believe its because the boards cannot do division in real time (at least the way I implemented). It never seemed to work. Regardless, you can still pick the frequency by setting the value of "frequency _ count" variable at the top of the code. It will be 32768/(frequency _ count) as the value of CCR0. 


## original 
# TIMER A Blink
The TIMER peripherals can be used in many situations thanks to it flexibility in features. For this lab, you will be only scratching the surface as to what this peripheral can do. 

## Up, Down, Continuous 
There are a few different ways that the timer module can count. For starters, one of the easiest to initialize is Continuous counting where in the TIMER module will alert you when its own counting register overflows. Up mode allows you to utilize a Capture/Compare register to have the counter stop at a particular count and then start back over again. You can also set the TIMER to Up/Down mode where upon hitting a counter or the overflow, instead of setting the counter back to zero, it will count back down to zero. 

## Task
Using the TIMER module instead of a software loop, control the speed of two LEDS blinking on your development boards. Experiment with the different counting modes available as well as the effect of the pre-dividers. Why would you ever want to use a pre-divider? What about the Capture and Compare registers? Your code should include a function (if you want, place it in its own .c and .h files) which can convert a desired Hz into the proper values required to operate the TIMER modules.

### Extra Work
#### Thinking with HALs
So maybe up to this point you have noticed that your software is looking pretty damn similar to each other for each one of these boards. What if there was a way to abstract away all of the particulars for a processor and use the same functional C code for each board? Just for this simple problem, why don't you try and build a "config.h" file which using IFDEF statements can check to see what processor is on board and initialize particular registers based on that.

#### Low Power Timers
Since you should have already done a little with interrupts, why not build this system up using interrupts and when the processor is basically doing nothing other than burning clock cycles, drop it into a Low Power mode. Do a little research and figure out what some of these low power modes actually do to the processor, then try and use them in your code. If you really want to put your code to the test, using the MSP430FR5994 and the built in super cap, try and get your code to run for the longest amount of time only using that capacitor as your power source.
