
//STEP 4

/*system spec
 two task
 counter
 two alarms
 set counter to interrupt (low level ISR1) then generate event for other task


 *Some lines are omitted here that will be already present
 *
 */

//libraries
#include "ee.h"
#include "ee_irq.h"

//Low level hardware specific definitions
#define __USE_LEDS__
#define __USE_BUTTONS__
#include "board/axiom_mpc5674fxmb/inc/ee_board.h"
#include "com/ee_com.h"
#include "com/com/inc/ee_cominit.h"
#include "com/com/inc/ee_api.h"



volatile unsigned int ERROR_FLAG = 0;

//add task by declaring
DeclareTask( SenderTask);
DeclareTask( Task2);
//add events by declaring
DeclareEvent( TimerEvent1);
DeclareEvent( msgIn);

//add messages by declaring?
//DeclareMessage( msgSendCount); //?
//DeclareMessage( msgReceiveCount);

/* some prototypes... */
//Advice only: define all prototypes of functions created in the project
//two functions and two global variables
//two function
void mydelay(unsigned long int del); //to form a loop creating a delay
void led_blink(unsigned char theled); //to change led configuration: blinking or not blinking

//global variables to set global status of led
volatile int led_status = 0; //command compiler to not optimize the variable
volatile int myErrorCounter; //to count number of errors to check the error hook working
/**/

/* just a dummy delay */
void mydelay(unsigned long int del) { //for loop stupid. busy wait
	volatile long int i; //compiler may remove because loop and no return of value  so volatile.
	for (i = 0; i < del; i++) {
		//empty
	}
}
/**/

/* Added for a finest debugger (run-time) tuning */
int tunable_delay = 50000;
/**/

/* sets and resets a led configuration passed as parameter, leaving the other
 * bits unchanged...
 */
void led_blink(unsigned char theled) { //Exam: not asked to write this function just implement?
	/*DisableAllInterrupts(); //process gets control and work without interruption. Scheduler stopped. if single core.
	led_status |= theled; //ledStatus = ledStatus bitwiseor theled unsafe but in C works. The desired led set to 1.
	EE_leds(led_status); //targetBoard EE,
	EnableAllInterrupts(); //disable enable similar to synchronization lock and unlock for the system
	mydelay((long int) tunable_delay);*/

	DisableAllInterrupts();
	led_status ^= theled; //flip the desired led status. bitwise AND NOT
	EE_leds(led_status);
	EnableAllInterrupts();

}

//Exam: asked to fill the function
/* BUTTON_0 interrupts activate Task2. */
static void Buttons_Interrupt(void) { //ISR1 not needed in OIL but still used service routine hardware specific.
//	set alarmtask2 :> run task2*/ //Exam comment to be filled with code
	led_blink(0x01);
	EE_buttons_clear_ISRflag(BUTTON_0); //exam: filled.
}

static void Counter_Interrupt(void) {
	CounterTick(myCounter);

	EE_e200z7_setup_decrementer(2000); //target board specific
}

void ErrorHook(StatusType Error) { //executed when system catches error
	myErrorCounter++; //increment global counter //just in case although not executed due to infinite loop
	ERROR_FLAG = Error; // global variable set to error
	led_blink(0xFF);
}

/*
 * The StartupHook in this case is used to initialize the Button and timer
 * interrupts
 */
void StartupHook(void) { //runs at boot . can be done at initial part in main. hook more elegant
	EE_buttons_init(BUTTON_0, 3); //low level function. implementation board specific

	//in datasheet of ERIKA
	EE_e200z7_register_ISR(46 + 16, Buttons_Interrupt, 1); //initialize interrupts
	EE_e200z7_register_ISR(10, Counter_Interrupt, 0); //in datasheet of Erika
	EE_e200z7_setup_decrementer(2000); ////in datasheet of Erika

}

TASK( SenderTask) {
	//led_blink(0b10000000); //Basic tasks
	//TerminateTask();	//Basic Tasks


	//Extended Task
	EventMaskType mask1;
	int count=0;
	int increment=0;
	while(1){
		WaitEvent(TimerEvent1); //wait any event? No. why specify event? Ans: event we want to wait. more than one event with bitwise |
				{
					GetEvent(SenderTask, &mask1); //to check which event received
					if (mask1 & TimerEvent1) {
						/*
						if (increment){
							count= (count+1)%10;

							increment = 0;
						} else {
							increment = 1;
						}*/
						count=(count+1)%10;
						SendMessage(msgSendCount, &count);
						ClearEvent(TimerEvent1);

					}
				}
	};
	TerminateTask(); //safety
}

TASK( Task2) {
	int count;
	ReceiveMessage(msgReceiveCount, &count);
	//if (count<5) {
		led_blink(0b10000000);
	//};
	TerminateTask();
}

int main(void) {
	EE_leds_init();
	//StartCom();
	StartOS(OSDEFAULTAPPMODE);
	EE_e200z7_enableIRQ();

	for (;;) {
	}
	return 0;

}

//some errors are false flags






