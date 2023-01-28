// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****
volatile unsigned long sw1 ;

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
void systm_intaliz( void );
volatile unsigned long sw_read(void);
void Delay1ms(unsigned long msec);
void led_toggle(void);
void led_on(void);

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  
	
  EnableInterrupts();           // enable interrupts for the grader
	 systm_intaliz();            //system intalize pE1 is output ,pE0 is input ,pe1 is always on 
  while(1){
	sw1 = sw_read();
		if (sw1==0x00){
				led_on();
			
		}else {
	
			led_toggle();
			Delay1ms (100);
		}
		
  }
  
}

//this function responsiple for intalizing the system as pE1 in an output ,pE0 is an input,and the pE1 is always on 
void systm_intaliz( void ){
	volatile unsigned long delay;
	
SYSCTL_RCGC2_R |= 0x00000010;      // 1) E clock
  delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize     
  GPIO_PORTE_AMSEL_R &= 0x00;        // 2) disable analog function
  GPIO_PORTE_PCTL_R &= 0x00000000;   // 3) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R &= ~0x01;         // 4.1) PE0 input,
  GPIO_PORTE_DIR_R |= 0x02;          // 4.2) PE1 output  
  GPIO_PORTE_AFSEL_R &= 0x00;        // 5) no alternate function
 // GPIO_PORTE_PDR_R |= 0x01;          // 6) enable pulldown resistor on PE0      
  GPIO_PORTE_DEN_R |= 0x03;          // 7) enable digital pins PE0-pE1
	GPIO_PORTE_DATA_R |=0x02;						//MAKE pE1 always output logic 1 

}

// THIS FUNCTION WILL READ THE DATA ON PIN PE0
//output : PE0 data
volatile unsigned long sw_read(void){
//volatile unsigned long read;
	//read=GPIO_PORTE_DATA_R &0x01;
//	Delay1ms(10);
return(GPIO_PORTE_DATA_R &0x01);
}


// THIS FUNCTION WILL PROVIDE A DELAY OF 1msec
//input: the amunt of msec to delay 
void Delay1ms(unsigned long msec){
// write this function
unsigned long count;
  while(msec > 0 ) { // repeat while still 1msec to delay
    count = 16000 ;
    while (count > 0) {
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
}
	}
//this function will make the PE1 is logic 1
	void led_on(void){
	
		GPIO_PORTE_DATA_R |=0x02;
	
	}
	
	void led_toggle(void ){
	
	GPIO_PORTE_DATA_R^=0x02;
	}