#include <stdint.h>
#include "msp.h"
#include "SysTick.h"

extern volatile int mode;

void Motor_InitSimple(void){
    // initialise P1.6 and P1.7 and make them outputs
    // Returns right away
    P1->SEL0 &= ~0xC0;
    P1->SEL1 &= ~0xC0;        // configure as GPIO
    P1->DIR |= 0xC0;          // Motor Forward which is in Module 12.2
    P1->OUT |= 0xC0;          // P1.4 and P1.1 are pull-up

    P3->SEL0 &= ~0xC0;
    P3->SEL1 &= ~0xC0;        // configure as GPIO
    P3->DIR |= 0xC0;          // Motor Forward which is in Module 12.2
    P3->OUT |= 0xC0;          // P1.4 and P1.1 are pull-up
}

void Motor_StopSimple(uint32_t time_ms){
// Stops both motors, puts driver to sleep
// Returns right away
  P1->OUT &= ~0xC0;   // off
  P2->OUT &= ~0xC0;   // off
  SysTick_Wait10ms(time_ms); // wait for multiple of 10ms from SysTick
}

void Motor_ForwardSimple(uint16_t duty, uint32_t time_ms){

    uint32_t i; // this i is used for the 'for loop' in section mtr_pwm_loop
    uint16_t L;

    // The PWM has high (H) and low (L) cycle.
    L = 1000-duty; // PWM using H and L

    P1 -> DIR |= ~0xC0; //initialise ports in the forward direction
    P2 -> DIR |= ~0xC0;
    P1 -> OUT &= ~0xC0; //initialise motors as forward


    for (i = 0; i <= time_ms; i++){
        if (mode == 1 || mode == 0) {
            P2 -> OUT |= 0xC0; //turn on both motors

            SysTick_Wait1us(1);
            SysTick_Wait1us(duty); //wait here for on-time (equal to duty)

            P2 -> OUT &= ~0xC0; //turn off both motors

            SysTick_Wait1us(1);
            SysTick_Wait1us(L); //wait here for off-time (equal to L = 1 - duty)
        } else {
            break;
        }
    }
}

void Motor_BackwardSimple(uint16_t duty, uint32_t time_ms){

    uint32_t i; // this i is used for the 'for loop' in section mtr_pwm_loop
    uint16_t L;

    // The PWM has high (H) and low (L) cycle.
    L = 1000-duty; // PWM using H and L

    P1 -> DIR |= 0xC0; //initialise ports in the forward direction
    P2 -> DIR |= ~0xC0;
    P1 -> OUT |= 0xC0; //initialise motors as backward

    for (i = 0; i <= time_ms; i++){
        if (mode == 0 || mode == 1){
            P2 -> OUT |= 0xC0; //turn on both motors

            SysTick_Wait1us(1);
            SysTick_Wait1us(duty); //wait here for on-time (equal to duty)

            P2 -> OUT &= ~0xC0; //turn off both motors

            SysTick_Wait1us(1);
            SysTick_Wait1us(L); //wait here for off-time (equal to L = 1 - duty)
        } else {
            break;
        }
    }
}

void Motor_LeftSimple(uint16_t duty, uint32_t time_ms){

    uint32_t i; // this i is used for the 'for loop' in section mtr_pwm_loop
    uint16_t L;

    // The PWM has high (H) and low (L) cycle.
    L = 1000-duty; // PWM using H and L

    P1 -> DIR |= ~0xC0; //initialise ports in the forward direction
    P2 -> DIR |= ~0xC0;

    for (i = 0; i <= time_ms; i++){
        if (mode == 1 || mode == 0) {
            P1 -> OUT |= 0x80; // 1000 0000
            P2 -> OUT |= 0xC0; //turn on both motors


            SysTick_Wait1us(1);
            SysTick_Wait1us(duty); //wait here for on-time (equal to duty)

            P2 -> OUT &= ~0xC0; //turn off both motors

            SysTick_Wait1us(1);
            SysTick_Wait1us(L); //wait here for off-time (equal to L = 1 - duty)

        } else {
            break;
        }
    }
}


void Motor_RightSimple(uint16_t duty, uint32_t time_ms){

    uint32_t i; // this i is used for the 'for loop' in section mtr_pwm_loop
    uint16_t L;

    // The PWM has high (H) and low (L) cycle.
    L = 1000-duty; // PWM using H and L

    P1 -> DIR |= ~0xC0; //initialise ports in the forward direction
    P2 -> DIR |= ~0xC0;

    for (i = 0; i <= time_ms; i++){
        if (mode == 1 || mode == 0){
            P1 -> OUT |= 0x40; // 0100 0000
            P2 -> OUT |= 0xC0; //turn on both motors

            SysTick_Wait1us(1);
            SysTick_Wait1us(duty); //wait here for on-time (equal to duty)

            P2 -> OUT &= ~0xC0; //turn off both motors

            SysTick_Wait1us(1);
            SysTick_Wait1us(L); //wait here for off-time (equal to L = 1 - duty)
        } else {
            break;
        }
    }
}
