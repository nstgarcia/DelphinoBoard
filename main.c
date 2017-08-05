/*
 * main.c
 */
#include "F2837xD_Device.h"
#include "F2837xD_Examples.h"
/*#include "F2837xD_Gpio_defines.h"
#include "F2837xD_cputimervars.h"*/

extern void InitGpio(void);
extern void GPIO_SetupPinOptions(Uint16 pin, Uint16 output, Uint16 flags);
extern void GPIO_SetupPinMux(Uint16 pin, Uint16 cpu, Uint16 peripheral);
extern void InitSysCtrl(void);
extern void InitPieCtrl(void);
extern void  EnableInterrupts(void);
extern void GPIO_WritePin(Uint16 pin, Uint16 outVal);
extern void GPIO_Toggle(Uint16 pin);

void ConfigCpuTimer0(void);
void ConfigCpuTimer1(void);
void InitCpuTimers(void);
int y;
int x;

int main(void) {
	y=1;
	x=y;
	InitSysCtrl();

	InitGpio();

	InitCpuTimers();
	ConfigCpuTimer0();
	//ConfigCpuTimer1();

	InitPieCtrl();
	EnableInterrupts();


	//D10, D9 y D1 son los leds
		/*GPIO12 : D9 red
		 *GPIO13 : D10 blue
		 * */


	EALLOW;
	GPIO_SetupPinOptions(12,1,GPIO_PUSHPULL);
	GPIO_SetupPinOptions(13,1,GPIO_PUSHPULL);

	//Table 2. F28377S, gpio12, gpio13= mux value: 0 (3rd parameter)
	GPIO_SetupPinMux(12,GPIO_MUX_CPU1,0);
	GPIO_SetupPinMux(13,GPIO_MUX_CPU1,0);
	EDIS;

	//LED prende con un 0
	GPIO_WritePin(12,1);//GpioDataRegs.GPADAT.bit.GPIO12=1; //turn off led
	GPIO_WritePin(13,1);//GpioDataRegs.GPADAT.bit.GPIO13=0; //turn off led

	StartCpuTimer0();
	//StartCpuTimer1();

	//switch gpio: por el pin reset #69,XRSN

	while(1){


	}

}
