/*
Simplified BSD License (FreeBSD License)
Copyright (c) 2017, Jonathan Valvano, All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// Author:      Mohd A. Zainol, Reza Nejabati
// Date:        10/10/2018
// Chip:        MSP432P401R LaunchPad Development Kit (MSP-EXP432P401R) for TI-RSLK
// File:        Part1_Interrupt_bumpsw_motor.h
// Function:    Part 1 of ERTS, uses interrupt for bump switches to control the motor

#include <stdint.h>
#include "msp.h"

// Check these if you really need to include these libraries in your code

#include "./inc/Clock.h"
#include "./inc/SysTick.h"
#include "./inc/CortexM.h"
#include "./inc/motor.h"


// Color    LED(s) Port2
// dark     ---    0
// blue     --B    0x04
// green    -G-    0x02
// sky blue -GB    0x06
// red      R--    0x01
// pink     R-B    0x05
// yellow   RG-    0x03
// white    RGB    0x07
#define RED       0x01
#define GREEN     0x02
#define YELLOW    0x03
#define BLUE      0x04
#define PINK      0x05
#define SKYBLUE   0x06
#define WHITE     0x07


// Initialize Bump sensors using interrupt
// Make six from Port 4 input pins
// Activate interface pull-up
// The pins are P4.7, 4.6, 4.5, 4.3, 4.2, 4.0
void BumpEdgeTrigger_Init(void){
    P4->SEL0 &= ~0xED;
    P4->SEL1 &= ~0xED;      // configure as GPIO
    P4->DIR &= ~0xED;       // make in
    P4->REN |= 0xED;        // enable pull resistors
    P4->OUT |= 0xED;        // pull-up
    P4->IES |= 0xED;        // falling edge event
    P4->IFG &= ~0xED;       // clear flag
    P4->IE |= 0xED;         // arm the interrupt
    // priority 2 on port4
    NVIC->IP[9] = (NVIC->IP[9]&0xFF00FFFF)|0x00400000;
    // enable interrupt 38 in NVIC on port4
    NVIC->ISER[1] = 0x00000040;
}

volatile int mode = 2; //initialise a global variable that is read to determine
                           //operating mode of the device.
                           //0 for free-motion, 1 for autonomous
int go = 1;

void PORT1_IRQHandler(void){
    Port2_Output(0);        // turn off the coloured LED
    switch(P1 -> IV){

    //interrupt case vectors = 2 * (pin_number + 1) in hexadecimal format
        case 0x04: //Switch 1 pressed, autonomous mode
            if (mode == 1){
                mode = 2;
            } else {
                mode = 1; //autonomous
                go = 1;
            }
        break;

        case 0x0A: //Switch 2 pressed, free-motion mode
            if (mode ==0){
                mode = 2;
            } else {
                Port2_Output(0);
                Port2_Output(WHITE);
                mode = 0; //free-motion
                go = 1;
            }
        break;

        default: //Break if neither occurs

        break;
    }
}



// Uses P4IV IRQ handler to solve critical section/race
void PORT4_IRQHandler(void){

    uint8_t status;

    // The movement for coloured LED
    // WHITE:   Forward
    // BLUE:    Turn right
    // YELLOW:  Turn left
    // GREEN:   Backward
    Port2_Output(0);        // turn off the coloured LED

    // Interrupt Vector of Port4
      status = P4->IV;      // 2*(n+1) where n is highest priority
      switch(status){

        case 0x02: // Bump switch 1
            if (mode == 0){
                // Change the coloured LED into green (backward)
                Port2_Output(GREEN);
                // Move backward at 500 duty for 200ms
                Motor_BackwardSimple(500, 200);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                // Change the coloured LED into yellow (turn left)
                Port2_Output(YELLOW);
                // Make a left turn at 500 duty for 100ms
                Motor_LeftSimple(500, 100);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                Port2_Output(WHITE);
            } else {

                mode = 2;
            }

          break;
        case 0x06: // Bump switch 2
            if (mode == 0) {
                // Change the coloured LED into green (backward)
                Port2_Output(GREEN);
                // Move backward at 500 duty for 200ms
                Motor_BackwardSimple(500, 200);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                // Change the coloured LED into yellow (turn left)
                Port2_Output(YELLOW);
                // Make a left turn at 500 duty for 200ms
                Motor_LeftSimple(500, 200);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                Port2_Output(WHITE);
            } else {

                mode = 2;
            }
          break;
        case 0x08: // Bump switch 3
            if (mode == 0) {
                // Change the coloured LED into green (backward)
                Port2_Output(GREEN);
                // Move backward at 500 duty for 200ms
                Motor_BackwardSimple(500, 200);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                // Change the coloured LED into yellow (turn left)
                Port2_Output(YELLOW);
                // Make a left turn at 500 duty for 300ms
                Motor_LeftSimple(500, 300);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                Port2_Output(WHITE);
            } else {

                mode = 2;
            }
          break;
        case 0x0C: // Bump switch 4
            if (mode == 0) {
                // Change the coloured LED into green (backward)
                Port2_Output(GREEN);
                // Move backward at 500 duty for 200ms
                Motor_BackwardSimple(500, 200);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                // Change the coloured LED into blue (turn right)
                Port2_Output(BLUE);
                // Make a right turn at 500 duty for 300ms
                Motor_RightSimple(500, 300);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                Port2_Output(WHITE);
            } else {

                mode = 2;
            }
          break;
        case 0x0E: // Bump switch 5
            if (mode == 0) {
                // Change the coloured LED into green (backward)
                Port2_Output(GREEN);
                // Move backward at 500 duty for 200ms
                Motor_BackwardSimple(500, 200);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                // Change the coloured LED into blue (turn right)
                Port2_Output(BLUE);
                // Make a right turn at 500 duty for 200ms
                Motor_RightSimple(500, 200);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                Port2_Output(WHITE);
            } else {

                mode = 2;
            }
          break;
        case 0x10: // Bump switch 6
            if (mode == 0) {
                // Change the coloured LED into green (backward)
                Port2_Output(GREEN);
                // Move backward at 500 duty for 200ms
                Motor_BackwardSimple(500, 200);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                // Change the coloured LED into blue (turn right)
                Port2_Output(BLUE);
                // Make a right turn at 500 duty for 100ms
                Motor_RightSimple(500, 100);
                // turn off the coloured LED
                Port2_Output(0);
                // Stop for 1000ms
                Motor_StopSimple(10);
                Port2_Output(WHITE);
            } else {

                mode = 2;
            }
          break;

        case 0xED: // none of the switches are pressed
          break;
      }

      P4->IFG &= ~0xED; // clear flag
}

// Read current state of 6 switches
// Returns a 6-bit positive logic result (0 to 63)
// bit 5 Bump5
// bit 4 Bump4
// bit 3 Bump3
// bit 2 Bump2
// bit 1 Bump1
// bit 0 Bump0
uint8_t Bump_Read_Input(void){
  return (P4->IN&0xED); // read P4.7, 4.6, 4.5, 4.3, 4.2, 4.0 inputs
}

// Function: checkbumpswitch(uint8_t status)
// Description: this is an alternative way that you can use,
//              in which it uses polling method that comes from main function.
//              However it is important to note that:
//              1) the polling method is only useful for small program
//              2) the input mask in switch case (for polling method) is DIFFERENT from the
//                 Nested Vectored Interrupt Controller (NVIC) which used in interrupt method.
void checkbumpswitch(uint8_t status)
{
    switch(status){

      //case 0x02: // Bump switch 1 (for interrupt vector)
        case 0x6D: // Bump 1
            if (mode == 0){
                            // Change the coloured LED into green (backward)
                            Port2_Output(GREEN);
                            // Move backward at 500 duty for 200ms
                            Motor_BackwardSimple(500, 200);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            // Change the coloured LED into yellow (turn left)
                            Port2_Output(YELLOW);
                            // Make a left turn at 500 duty for 100ms
                            Motor_LeftSimple(500, 100);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            Port2_Output(WHITE);
                        } else {

                            mode = 2;
                        }
        break;

      //case 0x06: // Bump switch 2 (for interrupt vector)
        case 0xAD: // Bump 2
            if (mode == 0) {
                            // Change the coloured LED into green (backward)
                            Port2_Output(GREEN);
                            // Move backward at 500 duty for 200ms
                            Motor_BackwardSimple(500, 200);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            // Change the coloured LED into yellow (turn left)
                            Port2_Output(YELLOW);
                            // Make a left turn at 500 duty for 200ms
                            Motor_LeftSimple(500, 200);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            Port2_Output(WHITE);
                        } else {

                            mode = 2;
                        }
        break;

      //case 0x08: // Bump switch 3 (for interrupt vector)
        case 0xCD: // Bump 3
            if (mode == 0) {
                            // Change the coloured LED into green (backward)
                            Port2_Output(GREEN);
                            // Move backward at 500 duty for 200ms
                            Motor_BackwardSimple(500, 200);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            // Change the coloured LED into yellow (turn left)
                            Port2_Output(YELLOW);
                            // Make a left turn at 500 duty for 300ms
                            Motor_LeftSimple(500, 300);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            Port2_Output(WHITE);
                        } else {

                            mode = 2;
                        }
        break;

      //case 0x0C: // Bump switch 4 (for interrupt vector)
        case 0xE5: // Bump 4
            if (mode == 0) {
                            // Change the coloured LED into green (backward)
                            Port2_Output(GREEN);
                            // Move backward at 500 duty for 200ms
                            Motor_BackwardSimple(500, 200);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            // Change the coloured LED into blue (turn right)
                            Port2_Output(BLUE);
                            // Make a right turn at 500 duty for 300ms
                            Motor_RightSimple(500, 300);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            Port2_Output(WHITE);
                        } else {

                            mode = 2;
                        }
        break;

      //case 0x0E: // Bump switch 5 (for interrupt vector)
        case 0xE9: // Bump 5
            if (mode == 0) {
                            // Change the coloured LED into green (backward)
                            Port2_Output(GREEN);
                            // Move backward at 500 duty for 200ms
                            Motor_BackwardSimple(500, 200);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            // Change the coloured LED into blue (turn right)
                            Port2_Output(BLUE);
                            // Make a right turn at 500 duty for 200ms
                            Motor_RightSimple(500, 200);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            Port2_Output(WHITE);
                        } else {

                            mode = 2;
                        }
        break;
      //case 0x10: // Bump switch 6 (for interrupt vector)
        case 0xEC: // Bump 6
            if (mode == 0) {
                            // Change the coloured LED into green (backward)
                            Port2_Output(GREEN);
                            // Move backward at 500 duty for 200ms
                            Motor_BackwardSimple(500, 200);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            // Change the coloured LED into blue (turn right)
                            Port2_Output(BLUE);
                            // Make a right turn at 500 duty for 100ms
                            Motor_RightSimple(500, 100);
                            // turn off the coloured LED
                            Port2_Output(0);
                            // Stop for 1000ms
                            Motor_StopSimple(10);
                            Port2_Output(WHITE);
                        } else {

                            mode = 2;
                        }
        break;
      case 0xED: // neither switch pressed

        break;
    }
}

void Port1_Init(void){
  P1->SEL0 &= ~0x01;
  P1->SEL1 &= ~0x01;        // configure P1.0 as GPIO
  P1->DIR |= 0x01;          //make P1.0 out, the built-in LED1
}

void Port2_Init(void){
    P2->SEL0 &= ~0xC7;
    P2->SEL1 &= ~0xC7;        // configure P2.2 P2.1 P2.0, and P2.6 P2.7 as GPIO
    P2->DIR |= 0x07;          // make P2.2-P2.0 out
    P2->DS |= 0x07;           // activate increased drive strength
    P2->OUT &= ~0x07;         // all LEDs off
    P2->DIR |= 0xC0;          // Direction of the motor
}

void Port2_Output(uint8_t data){
    // built-in red LED connected to P2.0
    // built-in green LED connected to P2.1
    // built-in blue LED connected to P2.2
    // write three outputs bits of P2
    P2->OUT = (P2->OUT&0xF8)|data;
}

void Switch_Init(void){
    // negative logic built-in Button 1 connected to P1.1
    // negative logic built-in Button 2 connected to P1.4
    P1->SEL0 &= ~0x12;
    P1->SEL1 &= ~0x12;      // configure P1.4 and P1.1 as GPIO
    P1->DIR &= ~0x12;       // make P1.4 and P1.1 in
    P1->REN |= 0x12;        // enable pull resistors on P1.4 and P1.1
    P1->OUT |= 0x12;        // P1.4 and P1.1 are pull-up, so the switches go high so long as they aren't pressed

    //IES register set to 1 for falling and 0 for rising edges
    P1 -> IES |= 0x12; //P1.4 and P1.1 falling edge event (flag raised when switches are pressed)

    //Precautionary clear to ensure interrupts occur only after initialisation and no flags are raised when power is supplied
    P1 -> IFG &= ~0x12; //clear flags 4 and 1

    //IE is the arming register, 1 is armed and 0 is unarmed
    P1 -> IE |= 0x12; //arm interrupt for pins 1 and 4

    //Set priority bits to a 010 for 2, 001 for 1 etc. (priority bits are 31 30 29)
    NVIC -> IP[8] = (NVIC -> IP[8]&0x00FFFFFF) | 0x20000000;

    //35 - 32 because each register is only 32 bits wide, so IRQ 35 is actually bit 3
    //This code only sets bit 3 and doesn't clear the rest of the bits (weird
    //but just how it works)
    NVIC -> ISER[1] = 0x00000008; //enable interrupt 35

}

// bit-banded addresses, positive logic
#define SW1IN ((*((volatile uint8_t *)(0x42098004)))^1)
#define SW2IN ((*((volatile uint8_t *)(0x42098010)))^1) // input: switch SW2
#define REDLED (*((volatile uint8_t *)(0x42098040)))    // output: red LED

void PredefinedRoute(void){
    Port2_Output(0);
    // Change the coloured LED into white
    Port2_Output(WHITE);
    // Move forward at 500 duty for 1000ms
    Motor_ForwardSimple(500, 1000);
    // turn off the coloured LED
    Port2_Output(0);
    // Stop for 1000ms
    Motor_StopSimple(10);
    // Change the coloured LED into blue (turn right)
    Port2_Output(BLUE);
    // Make a right turn at 500 duty for 100ms
    Motor_RightSimple(500, 300);
    // turn off the coloured LED
    Port2_Output(0);
    // Stop for 1000ms
    Motor_StopSimple(10);
    //LED white
    Port2_Output(WHITE);
    //forward 500 duty 500ms
    Motor_ForwardSimple(500, 500);
    //turn off LED
    Port2_Output(0);
    Motor_StopSimple(10);
    //LED yellow
    Port2_Output(YELLOW);
    //Turn left
    Motor_LeftSimple(500,300);
    Port2_Output(0);
    Motor_StopSimple(10);


}

int main(void){

  uint8_t status;

  Clock_Init48MHz();        // Initialise clock with 48MHz frequency
  Switch_Init();            // Initialise switches
  SysTick_Init();           // Initialise SysTick timer
  Port1_Init();             // Initialise P1.1 and P1.4 built-in buttons

  BumpEdgeTrigger_Init();   // Initialise bump switches using edge interrupt

  Port2_Init();             // Initialise P2.2-P2.0 built-in LEDs

  Motor_InitSimple();       // Initialise DC Motor
  Motor_StopSimple(100);    // Stop the motor on initial state


  EnableInterrupts();       // Clear the I bit
  WaitForInterrupt();


  while(go) {
      __no_operation();

      if (mode == 0) {
          DisableInterrupts();

          while(!SW1IN){
              Port2_Output(WHITE);
              Motor_ForwardSimple(500, 1);

                status = Bump_Read_Input();
                    if (status == 0x6D || status == 0xAD || status == 0xCD || status == 0xE5 || status == 0xE9 || status == 0xEC) {
                        checkbumpswitch(status);
                    }
          }

          mode = 1;




      } else if (mode == 1) {
          EnableInterrupts();       // Clear the I bit
          WaitForInterrupt();
          //Predefined route
          PredefinedRoute();
      } else {
          DisableInterrupts();
          status = Bump_Read_Input();
                              if (status == 0x6D || status == 0xAD || status == 0xCD || status == 0xE5 || status == 0xE9 || status == 0xEC) {
                                  checkbumpswitch(status);
                              }
          EnableInterrupts();       // Clear the I bit
          WaitForInterrupt();
          Motor_StopSimple(10);
      }
  }



  // Run forever

  /*uint8_t status;
  while(1){
    // This section is used for Example 2 (section 5.8.2)



        status = Bump_Read_Input();
        if (status == 0x6D || status == 0xAD || status == 0xCD || status == 0xE5 || status == 0xE9 || status == 0xEC) {
            checkbumpswitch(status);
        }*/



  }
