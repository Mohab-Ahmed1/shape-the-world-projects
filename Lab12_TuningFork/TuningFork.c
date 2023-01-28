// TuningFork.c Lab 12
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to create a squarewave at 440Hz.  
// There is a positive logic switch connected to PA3, PB3, or PE3.
// There is an output on PA2, PB2, or PE2. The output is 
//   connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|     
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */


#include "TExaS.h"
#include "..//tm4c123gh6pm.h"
// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode
// needed variables
unsigned long delay ;
int click=0;
int audio =0;
unsigned long	In = 0;
// input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void){ 
//pin intalize
 SYSCTL_RCGCGPIO_R |=0x00000001;  //activate the clock for port A
	delay= SYSCTL_RCGCGPIO_R; 		  //wait for the clock to connect 
	GPIO_PORTA_CR_R= 0x0c;					//allow change to pA2,PA3
	GPIO_PORTA_DIR_R =0x04;       //make the pa3 input and PA2 output
	GPIO_PORTA_DR8R_R |=0x08;       //enable 8ma on pA3
	GPIO_PORTA_DEN_R |= 0x0c;				//make PA3 and PA2 digital pins
	GPIO_PORTA_PDR_R |= 0x08 ;      //put apull up resistor on PA3 bec it is active low switch
	GPIO_PORTA_AMSEL_R &=0x00;      //disable analog fucntions 
	GPIO_PORTA_PCTL_R=0x00;					//clear pctil bits
	GPIO_PORTA_DATA_R =0x00;     //clear the output at pA3
	GPIO_PORTA_AFSEL_R=0x00;				//no alternative function
//systick intrupt
	NVIC_ST_CTRL_R =0 ;							//Disable  systic during setup
  NVIC_ST_RELOAD_R=	90908;      //reload value for 1136 micro secand
	NVIC_ST_CURRENT_R=0;          //any write to clear current register
	NVIC_SYS_PRI3_R =(NVIC_SYS_PRI3_R&0x00ffffff)|0x40000000;//priority 2
NVIC_ST_CTRL_R= 0x00000007;			//enable with core clock and intrupt
}

// called at 880 Hz
void SysTick_Handler(void){
		In=GPIO_PORTA_DATA_R & 0x08;
		
	if (In==0x08&&click==0){
			click=1;
		}else if (In==0x00&&click==1) {
					click=2;
		}else if (In==0x08&&click==2){
						click=3;
		}else if (In==0x00&&click==3) {
						click=0;
		}
		if (click==1||click==2){
	GPIO_PORTA_DATA_R ^= 0x04;
		}else {GPIO_PORTA_DATA_R &= ~0x04;}

}
	
int main(void){// activate grader and set system clock to 80 MHz
 TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2,ScopeOn); 
  Sound_Init();         
 EnableInterrupts();   // enable after all initialization are done

  while(1){
    // main program is free to perform other tasks
    // do not use WaitForInterrupt() here, it may cause the TExaS to crash

	
		}
  }

