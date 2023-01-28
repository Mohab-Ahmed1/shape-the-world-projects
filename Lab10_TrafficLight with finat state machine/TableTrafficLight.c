// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

//2.a FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void portf_int (void);   //initalize port f 
void portE_int (void);   //intalize port E
void portb_int (void); //intalize port b
void SysTick_Init(void); //intalize systic counter 
void SysTick_Wait(unsigned long time);  //delay for 1mili sec
void SysTick_Wait10ms(unsigned long time);  //delay for 10 msec 
char x=130;
//2.b struct,varibles &defines section
struct state{
  unsigned long out ;     // out element
 unsigned long wait;     // wait  element
  unsigned long next [8];   // index of the current state  element
};
typedef struct state stype;

#define gow 0
#define waitw 1
#define gos 2
#define waits 3
#define redboth 4
#define  redbothflash_off_1 5
#define  redbothflash_on_1 6
#define  redbothflash_off_2 7
#define  redbothflash_on_2 8
#define  redbothflash_off_3 9

stype fsm [10]={
	{0xc2,100,{gow,gow,waitw,waitw,waitw,waitw,waitw,waitw}},   //GOW STATE
	{0x142,50,{gos ,gos,gos,gos,redboth,redboth,redboth,redboth}},  //waitw state 
	{0x212,100,{gos,waits,gos,waits,waits,waits,waits,waits}},  // gos state 
	{0x222,50,{gow,gow,gow,gow,redboth,redboth,redboth,gow}}, //waits state
	{0x248,100,{redboth,redbothflash_off_1,redbothflash_off_1,redbothflash_off_1,redboth,redbothflash_off_1,redbothflash_off_1,redbothflash_off_1}}, //redboth state 
	{0x240,50,{redbothflash_on_1,redbothflash_on_1,redbothflash_on_1,redbothflash_on_1,redbothflash_on_1,redbothflash_on_1,redbothflash_on_1,redbothflash_on_1}},//red both flash off 1 state
	{0x242,50,{redbothflash_off_2,redbothflash_off_2,redbothflash_off_2,redbothflash_off_2,redbothflash_off_2,redbothflash_off_2,redbothflash_off_2,redbothflash_off_2}},//red both flash on 1 state 
	{0x240,50,{redbothflash_on_2,redbothflash_on_2,redbothflash_on_2,redbothflash_on_2,redbothflash_on_2,redbothflash_on_2,redbothflash_on_2,redbothflash_on_2}},//redbothflash_off 2 state
	{0x242,50,{redbothflash_off_3 ,redbothflash_off_3 ,redbothflash_off_3 ,redbothflash_off_3 ,redbothflash_off_3 ,redbothflash_off_3 ,redbothflash_off_3 ,redbothflash_off_3 }},//redflash on 2 state
	{0x240,50,{redboth,gow,gos,gow,redboth,gow,gos,gos}}//redboth flash off 3
};
unsigned char cstate=0;
unsigned long input;
//******4.code****************
int main(void){ 
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
  EnableInterrupts();
	portf_int();
	portb_int();
	portE_int();
	SysTick_Init();
	
	

  while(1){
GPIO_PORTF_DATA_R=fsm[cstate].out&0x0f;
GPIO_PORTB_DATA_R=fsm[cstate].out>>4;
SysTick_Wait10ms(fsm [cstate].wait);
input=GPIO_PORTE_DATA_R&0x07;
cstate=fsm[cstate].next[input];
  }
}

// ***** 3. Subroutines Section *****
void portf_int (void){
volatile unsigned long delay ;
SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay    
  GPIO_PORTF_CR_R = 0x0A;           // allow changes to PF1,pf3       
  GPIO_PORTF_AMSEL_R = 0x00;        // 2) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 3) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0A;          // 4)PF3,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 5) no alternate function      
  GPIO_PORTF_DEN_R = 0x0A;          // 6) enable digital pins PF1-PF3 
  GPIO_PORTF_DATA_R= 0X04;					//INTAL VALUES FOR PORT F AT GOW STATE	
}
void portE_int (void){
volatile unsigned long delay ;
SYSCTL_RCGC2_R |= 0x00000010;     // 1) Eclock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTE_CR_R = 0x07;           // allow changes to PE0-2      
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x00;          // 5) PE2,PE1,PE0 input
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
  //GPIO_PORTE_PUR_R = 0xff;          // enable pullup resistors on PE2,PE0       
  GPIO_PORTE_DEN_R = 0x07;          // 7) enable digital pins PE2-PE0 
}
void portb_int (void){
volatile unsigned long delay ;
SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTB_CR_R = 0x3F;           // allow changes to Pb5-0       
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x3f;          // 5) Pb5-0 output   
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function   
  GPIO_PORTB_DEN_R = 0x3f;          // 7) enable digital pins PB4-PB0 
	 GPIO_PORTB_DATA_R= 0X0C;					//INTAL VALUES FOR PORT B AT GOW STATE	
}

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}
void SysTick_Wait(unsigned long time){
  NVIC_ST_RELOAD_R = time-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}
void SysTick_Wait10ms(unsigned long time){
  int i;
  for(i=0; i<time; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}

