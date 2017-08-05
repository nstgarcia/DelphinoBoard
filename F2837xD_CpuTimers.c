//###########################################################################
// FILE:    F2837xD_CpuTimers.c
// TITLE:    CPU 32-bit Timers Initialization & Support Functions.
// NOTES:
//###########################################################################
// $TI Release: F2837xD Support Library v120 $
// $Release Date: Fri Aug 22 15:22:27 CDT 2014 $
//###########################################################################

#include "F2837xD_Device.h"     // Headerfile Include File
#include "F2837xD_Examples.h"   // Examples Include File

struct CPUTIMER_VARS CpuTimer0;
struct CPUTIMER_VARS CpuTimer1;
struct CPUTIMER_VARS CpuTimer2;

//---------------------------------------------------------------------------
// InitCpuTimers:
//---------------------------------------------------------------------------
// This function initializes all three CPU timers to a known state.
void InitCpuTimers(void)
{
    // CPU Timer 0
    // Initialize address pointers to respective timer registers:
	//El CpuTimer0Regs es la variable global que hace referencia a los registros del Timer 0.
    CpuTimer0.RegsAddr = &CpuTimer0Regs;
    // Make sure timer is stopped:
    CpuTimer0Regs.TCR.bit.TSS = 1;    // tenemos disponible la funcion    StopCpuTimer0();

    CpuTimer0Regs.TCR.bit.TIF=1; //clears the flag

    // Initialize timer period to maximum:
    CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
    //es lo mismo que CpuTimer0.RegsAddr->PRD.all=0xFFFFFFFF;

    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
    //Debe ser 0, porque la fórmula es (TDDRH:TDDR +1)
    //TPR: tiene TDDR 0-7 y PSC 8-15
    CpuTimer0Regs.TPR.all  = 0;

    //TPRH: tiene TDDRH 0-7, y PSCH 8-15
    CpuTimer0Regs.TPRH.all = 0;


    // Reload all counter register with period value:
    CpuTimer0Regs.TCR.bit.TRB = 1; //recargar los regs con el mismo valor

    CpuTimer0Regs.TCR.bit.TIE=0;

    // Make sure timers are stopped:
    CpuTimer1Regs.TCR.bit.TSS = 1;
    CpuTimer2Regs.TCR.bit.TSS = 1;

    // Initialize timer period to maximum:
    CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
    CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;

    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
    CpuTimer1Regs.TPR.all  = 0;
    CpuTimer1Regs.TPRH.all = 0;
    CpuTimer2Regs.TPR.all  = 0;
    CpuTimer2Regs.TPRH.all = 0;

    CpuTimer1Regs.TCR.bit.TIF=1; //clears the flag
    CpuTimer2Regs.TCR.bit.TIF=1; //clears the flag


    // Reload all counter register with period value:
    CpuTimer1Regs.TCR.bit.TRB = 1;
    CpuTimer2Regs.TCR.bit.TRB = 1;

    // Reset interrupt counters: //ni se usan
    //CpuTimer1.InterruptCount = 0;
    //CpuTimer2.InterruptCount = 0;
    CpuTimer1Regs.TCR.bit.TIE=0;
    CpuTimer2Regs.TCR.bit.TIE=0;
}

//---------------------------------------------------------------------------
// ConfigCpuTimer:
//---------------------------------------------------------------------------
// This function initializes the selected timer to the period specified
// by the "Freq" and "Period" parameters. The "Freq" is entered as "MHz"
// and the period in "uSeconds". The timer is held in the stopped state
// after configuration.

void ConfigCpuTimer(struct CPUTIMER_VARS *Timer)
//void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	//El SYSCLK está configurado a 100MHz

	/*The counter decrements once every (TPR[TDDRH:TDDR]+1) SYSCLKOUT cycles, where
		TDDRH:TDDR is the timer divider.
	 * */

    /*La formula para calcular el numero de ciclos a usar es:
     *
     *#ciclos = (Tiempo_deseado)/(Tiempo_periodo * prescalador)
     * Si estoy a 100MHz, tengo un Tiempo_periodo =10x10^-9s, 10ns
     * Si el Tiempo_deseado=500ms , 500x10^-3s
     * Y si usamos prescalador= 5000 = 0x1388
     * el #ciclos= 10000 ->0x2710
*/

    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    //Quiero conseguir retardo de 500ms, usaré 9999+1 en prescaler

    CpuTimer0Regs.PRD.all= 0x186A;
    CpuTimer0Regs.TPR.bit.TDDR=0x3F;
   	CpuTimer0Regs.TPRH.bit.TDDRH=0x1F;
  /*
    CpuTimer0Regs.PRD.all= 20000;
    CpuTimer0Regs.TPR.bit.TDDR=;
    CpuTimer0Regs.TPRH.bit.TDDRH=;
*/
   	CpuTimer0Regs.TCR.bit.TSS = 1;				// 1 = Stop timer, 0 = Start/Restart
   	CpuTimer0Regs.TCR.bit.TRB = 1;				// 1 = reload timer
   	CpuTimer0Regs.TCR.bit.SOFT=0;
   	CpuTimer0Regs.TCR.bit.FREE=0;			// Timer Free Run Disabled, hard stop
    CpuTimer0Regs.TCR.bit.TIE=1;			// 0 = Disable/ 1 = Enable Timer

    //habilitar la interrupcion del timer en PIE e IER
        //PieCtrlRegs.PIEIER1.bit.INTx7=1;

}


void ConfigCpuTimer0(void)
//void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	//El SYSCLK está configurado a 100MHz

	/*The counter decrements once every (TPR[TDDRH:TDDR]+1) SYSCLKOUT cycles, where
		TDDRH:TDDR is the timer divider.
*/
	CpuTimer0Regs.PRD.all= 0x186A;
	CpuTimer0Regs.TPR.bit.TDDR=0x3F;
	CpuTimer0Regs.TPRH.bit.TDDRH=0x1F;
   	CpuTimer0Regs.TCR.bit.TSS = 1;				// 1 = Stop timer, 0 = Start/Restart
   	CpuTimer0Regs.TCR.bit.TRB = 1;				// 1 = reload timer
   	CpuTimer0Regs.TCR.bit.SOFT=0;
   	CpuTimer0Regs.TCR.bit.FREE=0;			// Timer Free Run Disabled, hard stop
    CpuTimer0Regs.TCR.bit.TIE=1;			// 0 = Disable/ 1 = Enable Timer

}


void ConfigCpuTimer1(void)
//void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	//El SYSCLK está configurado a 100MHz

	/*The counter decrements once every (TPR[TDDRH:TDDR]+1) SYSCLKOUT cycles, where
		TDDRH:TDDR is the timer divider.
*/
	CpuTimer1Regs.PRD.all= 0x186A;
	CpuTimer1Regs.TPR.bit.TDDR=0x3F;
	CpuTimer1Regs.TPRH.bit.TDDRH=0x1F;
   	CpuTimer1Regs.TCR.bit.TSS = 1;				// 1 = Stop timer, 0 = Start/Restart
   	CpuTimer1Regs.TCR.bit.TRB = 1;				// 1 = reload timer
   	CpuTimer1Regs.TCR.bit.SOFT=0;
   	CpuTimer1Regs.TCR.bit.FREE=0;			// Timer Free Run Disabled, hard stop
    CpuTimer1Regs.TCR.bit.TIE=1;			// 0 = Disable/ 1 = Enable Timer

}


void ConfigCpuTimer2(void)
//void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	//El SYSCLK está configurado a 100MHz

	/*The counter decrements once every (TPR[TDDRH:TDDR]+1) SYSCLKOUT cycles, where
		TDDRH:TDDR is the timer divider.
*/
	CpuTimer2Regs.PRD.all= 0x186A;
	CpuTimer2Regs.TPR.bit.TDDR=0x3F;
	CpuTimer2Regs.TPRH.bit.TDDRH=0x1F;
   	CpuTimer2Regs.TCR.bit.TSS = 1;				// 1 = Stop timer, 0 = Start/Restart
   	CpuTimer2Regs.TCR.bit.TRB = 1;				// 1 = reload timer
   	CpuTimer2Regs.TCR.bit.SOFT=0;
   	CpuTimer2Regs.TCR.bit.FREE=0;			// Timer Free Run Disabled, hard stop
    CpuTimer2Regs.TCR.bit.TIE=0;			// 0 = Disable/ 1 = Enable Timer

}
