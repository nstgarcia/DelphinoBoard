//###########################################################################
//
// FILE:	F2837xD_PieCtrl.c
//
// TITLE:	F2837xD Device PIE Control Register Initialization Functions.
//
//###########################################################################
// $TI Release: F2837xD Support Library v120 $
// $Release Date: Fri Aug 22 15:22:27 CDT 2014 $
//###########################################################################

#include "F2837xD_Device.h"     // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"   // F2837xD Examples Include File

//---------------------------------------------------------------------------
// InitPieCtrl: 
//---------------------------------------------------------------------------
// This function initializes the PIE control registers to a known state.
//
void InitPieCtrl(void)
{
    // Disable Interrupts at the CPU level:
    //deshabilita las interrupciones globalmente
	//se deshabilita con 1, se habilita con 0
	DINT; //es lo mismo que SETC INTM

    // Disable the PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;


	InitPieVectTable(); //copia la tabla que contiene los ISR,en la estructura que
						//tiene las definiciones de la tabla de vectores


	// Clear all PIEIER registers:
    //se deshabilita con un cero
	PieCtrlRegs.PIEIER1.all = 0x0000;
	PieCtrlRegs.PIEIER2.all = 0x0000;
	PieCtrlRegs.PIEIER3.all = 0x0000;
	PieCtrlRegs.PIEIER4.all = 0x0000;
	PieCtrlRegs.PIEIER5.all = 0x0000;
	PieCtrlRegs.PIEIER6.all = 0x0000;
	PieCtrlRegs.PIEIER7.all = 0x0000;
	PieCtrlRegs.PIEIER8.all = 0x0000;
	PieCtrlRegs.PIEIER9.all = 0x0000;
	PieCtrlRegs.PIEIER10.all = 0x0000;
	PieCtrlRegs.PIEIER11.all = 0x0000;
	PieCtrlRegs.PIEIER12.all = 0x0000;

	// Clear all PIEIFR registers:
	//Se limpian con un cero

	PieCtrlRegs.PIEIFR2.all = 0x0000;
	PieCtrlRegs.PIEIFR3.all = 0x0000;
	PieCtrlRegs.PIEIFR4.all = 0x0000;
	PieCtrlRegs.PIEIFR5.all = 0x0000;
	PieCtrlRegs.PIEIFR6.all = 0x0000;
	PieCtrlRegs.PIEIFR7.all = 0x0000;
	PieCtrlRegs.PIEIFR8.all = 0x0000;
	PieCtrlRegs.PIEIFR9.all = 0x0000;
	PieCtrlRegs.PIEIFR10.all = 0x0000;
	PieCtrlRegs.PIEIFR11.all = 0x0000;
	PieCtrlRegs.PIEIFR12.all = 0x0000;
}	

//---------------------------------------------------------------------------
// EnableInterrupts: 
//---------------------------------------------------------------------------
// This function enables the PIE module and CPU __interrupts
//
void EnableInterrupts()
{

///Etapa del periferico

	CpuTimer0Regs.TCR.bit.TIE=1;			// 0 = Disable/ 1 = Enable Timer

////etapa del PIE

	//habilitar el PIE
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
	PieCtrlRegs.PIEIER1.bit.INTx7=1; //timer 0

///etapa del CPU

		IER|=0x1001;

//interrupciones globales

		EINT; //es para el ST1.INTM, es lo mismo que CLEAR INTM
}

//===========================================================================
// End of file.
//===========================================================================
