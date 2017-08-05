//###########################################################################
//
// FILE:   F2837xD_SysCtrl.c
//
// TITLE:  F2837xD Device System Control Initialization & Support Functions.
//
// DESCRIPTION:
//
//         Example initialization of system resources.
//
//###########################################################################
// $TI Release: F2837xD Support Library v120 $
// $Release Date: Fri Aug 22 15:22:27 CDT 2014 $
//###########################################################################

#include "F2837xD_Device.h"     // Headerfile Include File
#include "F2837xD_Examples.h"   // Examples Include File

// Functions that will be run from RAM need to be assigned to
// a different section.  This section will then be mapped to a load and
// run address using the linker cmd file.
//
//  *IMPORTANT*
//  IF RUNNING FROM FLASH, PLEASE COPY OVER THE SECTION "ramfuncs"  FROM FLASH
//  TO RAM PRIOR TO CALLING InitSysCtrl(). THIS PREVENTS THE MCU FROM THROWING
//  AN EXCEPTION WHEN A CALL TO DELAY_US() IS MADE. 
//
#pragma CODE_SECTION(InitFlash, "ramfuncs");



void InitSysCtrl(void)
{
    // Disable the watchdog
    DisableDog();

#ifdef _FLASH
// Copy time critical code and Flash setup code to RAM
// This includes the following functions:  InitFlash();
// The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
// symbols are created by the linker. Refer to the device .cmd file.
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
    InitFlash();
#endif

    // *IMPORTANT*
    // The Device_cal function, which copies the ADC & oscillator calibration values
    // from TI reserved OTP into the appropriate trim registers, occurs automatically
    // in the Boot ROM. If the boot ROM code is bypassed during the debug process, the
    // following function MUST be called for the ADC and oscillators to function according
    // to specification. The clocks to the ADC MUST be enabled before calling this
    // function.
    // See the device data manual and/or the ADC Reference
    // Manual for more information.
#ifdef CPU1
    EALLOW;

    //enable pull-ups on unbonded IOs as soon as possible to reduce power consumption.
    //libreria en Gpio.c
    GPIO_EnableUnbondedIOPullups();
    //CPU System registers, page 282 manual pdf

    //PCLKCR13: Peripheral Clock Gating Registers--> ADC
    //1: Module clock is turned-on
    //0: Module clock is gated-off
	CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_D = 1;

    //check if device is trimmed, page 1164 manual pdf
	//empieza en 0x0005_D180 el ANALOG_SUBSYS_REGS
	//36h es el offset para ANAREFTRIMA-->36 offset para llegar a 5D1B6
	//0 is untrimmed, then trimmed
    if(*((Uint16 *)0x5D1B6) == 0x0000){
        //device is not trimmed, apply static calibration values
        AnalogSubsysRegs.ANAREFTRIMA.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMB.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMC.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMD.all = 31709;
    }

    //ADC -->gated off
	CpuSysRegs.PCLKCR13.bit.ADC_A = 0;
	CpuSysRegs.PCLKCR13.bit.ADC_B = 0;
	CpuSysRegs.PCLKCR13.bit.ADC_C = 0;
	CpuSysRegs.PCLKCR13.bit.ADC_D = 0;
    EDIS;

    //Hay que inicializar la fuente del reloj

    // Initialize the PLL control: PLLCR and CLKINDIV

   InitSysPll(XTAL_OSC,IMULT_20,FMULT_1,PLLCLK_BY_2);
	            //1,       20,     0,         1: 2
   //PLLSYSCLK = (fOSCCLK) * ((IMULT)+ (FMULT) )/ (PLLCLK_)

   // = [10MHz * (20+0)] / 2 = 200Mhz/2= 100MHz estamos a 100MHz

    //Turn on all peripherals, it can be modified to enable only few peripheral clocks
	InitPeripheralClocks();

#endif
}

void InitPeripheralClocks()
{
	EALLOW;
//Se usa el reloj PERx.SYSCLK que es similar al SYSCLK
	//se controla con el registro PCLKCRx
	CpuSysRegs.PCLKCR0.bit.CLA1 = 1;
	CpuSysRegs.PCLKCR0.bit.DMA = 1;
	CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1;
	CpuSysRegs.PCLKCR0.bit.CPUTIMER1 = 1;
	CpuSysRegs.PCLKCR0.bit.CPUTIMER2 = 1;
	CpuSysRegs.PCLKCR0.bit.HRPWM = 1;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;

	//EMIF y EPWM tienen un diver /2 por default
	CpuSysRegs.PCLKCR1.bit.EMIF1 = 1;
	CpuSysRegs.PCLKCR1.bit.EMIF2 = 1;

	CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM6 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM9 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM11 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM12 = 1;

	CpuSysRegs.PCLKCR3.bit.ECAP1 = 1;
	CpuSysRegs.PCLKCR3.bit.ECAP2 = 1;
	CpuSysRegs.PCLKCR3.bit.ECAP3 = 1;
	CpuSysRegs.PCLKCR3.bit.ECAP4 = 1;
	CpuSysRegs.PCLKCR3.bit.ECAP5 = 1;
	CpuSysRegs.PCLKCR3.bit.ECAP6 = 1;

	CpuSysRegs.PCLKCR4.bit.EQEP1 = 1;
	CpuSysRegs.PCLKCR4.bit.EQEP2 = 1;
	CpuSysRegs.PCLKCR4.bit.EQEP3 = 1;

	CpuSysRegs.PCLKCR6.bit.SD1 = 1;
	CpuSysRegs.PCLKCR6.bit.SD2 = 1;

	CpuSysRegs.PCLKCR7.bit.SCI_A = 1;
	CpuSysRegs.PCLKCR7.bit.SCI_B = 1;
	CpuSysRegs.PCLKCR7.bit.SCI_C = 1;
	CpuSysRegs.PCLKCR7.bit.SCI_D = 1;

	CpuSysRegs.PCLKCR8.bit.SPI_A = 1;
	CpuSysRegs.PCLKCR8.bit.SPI_B = 1;
	CpuSysRegs.PCLKCR8.bit.SPI_C = 1;

	CpuSysRegs.PCLKCR9.bit.I2C_A = 1;
	CpuSysRegs.PCLKCR9.bit.I2C_B = 1;

	CpuSysRegs.PCLKCR10.bit.CAN_A = 1;
	CpuSysRegs.PCLKCR10.bit.CAN_B = 1;

	CpuSysRegs.PCLKCR11.bit.McBSP_A = 1;
	CpuSysRegs.PCLKCR11.bit.McBSP_B = 1;
	CpuSysRegs.PCLKCR11.bit.USB_A = 1;

	CpuSysRegs.PCLKCR12.bit.uPP_A = 1;

	CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_D = 1;

	CpuSysRegs.PCLKCR14.bit.CMPSS1 = 1;
	CpuSysRegs.PCLKCR14.bit.CMPSS2 = 1;
	CpuSysRegs.PCLKCR14.bit.CMPSS3 = 1;
	CpuSysRegs.PCLKCR14.bit.CMPSS4 = 1;
	CpuSysRegs.PCLKCR14.bit.CMPSS5 = 1;
	CpuSysRegs.PCLKCR14.bit.CMPSS6 = 1;
	CpuSysRegs.PCLKCR14.bit.CMPSS7 = 1;
	CpuSysRegs.PCLKCR14.bit.CMPSS8 = 1;

	CpuSysRegs.PCLKCR16.bit.DAC_A = 1;
	CpuSysRegs.PCLKCR16.bit.DAC_B = 1;
	CpuSysRegs.PCLKCR16.bit.DAC_C = 1;
	
	EDIS;
}

//---------------------------------------------------------------------------
// Example: InitFlash:
//---------------------------------------------------------------------------
// This function initializes the Flash Control registers

//                   CAUTION
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results

void InitFlash(void)
{
    EALLOW;

    //At reset bank and pump are in sleep
    //A Flash access will power up the bank and pump automatically
    //After a Flash access, bank and pump go to low power mode (configurable in FBFALLBACK/FPAC1 registers)-
    //if there is no further access to flash
    //Power up Flash bank and pump and this also sets the fall back mode of flash and pump as active
    Flash0CtrlRegs.FPAC1.bit.PMPPWR = 0x1;
    Flash0CtrlRegs.FBFALLBACK.bit.BNKPWR0 = 0x3;

    //Disable Cache and prefetch mechanism before changing wait states
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 0;
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 0;

    //Set waitstates according to frequency
    //                CAUTION
    //Minimum waitstates required for the flash operating
    //at a given CPU rate must be characterized by TI.
    //Refer to the datasheet for the latest information.
    #if CPU_FRQ_200MHZ
    Flash0CtrlRegs.FRDCNTL.bit.RWAIT = 0x3;
    #endif

    #if CPU_FRQ_150MHZ
    FlashCtrlRegs.FRDCNTL.bit.RWAIT = 0x2;
    #endif

    #if CPU_FRQ_120MHZ
    FlashCtrlRegs.FRDCNTL.bit.RWAIT = 0x2;
    #endif

    //Enable Cache and prefetch mechanism to improve performance
    //of code executed from Flash.
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 1;
    Flash0CtrlRegs.FRD_INTF_CTRL.bit.PREFETCH_EN = 1;

    //At reset, ECC is enabled
    //If it is disabled by application software and if application again wants to enable ECC
    Flash0EccRegs.ECC_ENABLE.bit.ENABLE = 0xA;

    EDIS;

    //Force a pipeline flush to ensure that the write to
    //the last register configured occurs before returning.

    __asm(" RPT #7 || NOP");

}

//---------------------------------------------------------------------------
// Example: ServiceDog:
//---------------------------------------------------------------------------
// This function resets the watchdog timer.
// Enable this function for using ServiceDog in the application

void ServiceDog(void)
{
    EALLOW;
    WdRegs.WDKEY.bit.WDKEY = 0x0055;
    WdRegs.WDKEY.bit.WDKEY = 0x00AA;
    EDIS;
}

//---------------------------------------------------------------------------
// Example: DisableDog:
//---------------------------------------------------------------------------
// This function disables the watchdog timer.

void DisableDog(void)
{
	/*WCDCR
	     * 15-8 RESERVED
	     * 7 RESERVED     *
	     * 6 WDDIS: Writing a 1 to this bit will disable the watchdog module. Writing a 0
	will enable the module.
	     * 5-3 WDCHK: The user must ALWAYS write 1,0,1 to these bits whenever a write to
	this register is performed. Writing any other value will cause an
	immediate reset to the core (if WD enabled).
	     * 2-0 WDPS:These bits configure the watchdog counter clock (WDCLK) rate
	relative to INTOSC1/512
	     * */

	volatile Uint16 temp;
    EALLOW;
    //Grab the clock config so we don't clobber it
    temp = WdRegs.WDCR.all & 0x0007;//0000 0000 0000 0111
    WdRegs.WDCR.all = 0x0068 | temp;
    //0000 0000 0110 1000 | x0000 0000 0000 0111
    //0000 0000 0 110 1 111
    //(2-0)WDPS -> 111 WDCLK = INTOSC1/512/64
    //(6-3)WDCHK-> 101
    //WDDIS-> 1 disable Watchdog
    EDIS;
}

//---------------------------------------------------------------------------
// Example: InitPll:
//---------------------------------------------------------------------------
// This function initializes the PLL registers.

//InitSysPll(XTAL_OSC,IMULT_20,FMULT_1,PLLCLK_BY_2);
			 //1,       20,     0,         1
//PLLSYSCLK = 10Mhz(OSCCLK) * 40 (IMULT) * 1 (FMULT) /  2 (PLLCLK_BY_2)
/*NMIWD
 * */
//Page 261 manual pdf
void InitSysPll(Uint16 clock_source, Uint16 imult, Uint16 fmult, Uint16 divsel)
{
	Uint16 temp_divsel;

	/*CLKSRCCTL1: ya ,origen del reloj
	 * 	 En CLKSRCCTL1 reg->
	 *  OSCCLKSRCSEL: Oscillator Clock Source Select Bit:
	 *  00 = INTOSC2 (default on reset)
		01 = External Oscillator (XTAL)
		10 = INTOSC1
		11 = reserved (default to INTOSC1)
	 *
	 *CLKSRCCTL2:reloj can
	 *CLKSRCCTL3:XCLKOUTSEL
	 *SYSPLLCTL1:decide si se omite el pll, es al final
	 *SYSPLLMULT:fraccional multipliers
	 	   * IMULT:SYSPLL Integer Multiplier, 0000001 Integer Multiplier = 1
	 	   * 	For 0000000 Fout = Fref (PLLBYPASS) Integer Multiplier = 1
				0000001 Integer Multiplier = 1
				0000010 Integer Multiplier = 2
				0000011 Integer Multiplier = 3
				...
				1111111 Integer Multipler = 127
	   	   * FMULT: SYSPLL Fractional Multiplier:page 270
				00 Fractional Multiplier = 0
				01 Fractional Multiplier = 0.25
				10 Fractional Multiplier = 0.5
				11 Fractional Multiplier = 0.75
	 *SYSPLLSTS: status register
	 *SYSCLKDIVSEL:System Clock Divider Select register
	 *		PLLSYSCLKDIV:PLLSYSCLK Divide Select: This bit selects the divider setting for the
		  PLLSYSCLK, 000000 = /1, 000001 = /2,111111 = /126
	*PERCLKDIVSEL: Peripheral Clock Divider Selet register,para perifericos como PWM,EMIFx
	 * */


	if((clock_source == ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)   &&
	  (imult		 == ClkCfgRegs.SYSPLLMULT.bit.IMULT) 	      &&
	  (fmult 	     == ClkCfgRegs.SYSPLLMULT.bit.FMULT)          &&
	  (divsel        == ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV))
    {
	    //everything is set as required, so just return
	    return;
	}
//ver pag 108
    // Make sure the PLL is not running in limp mode
	//MCDCR: Missing Clock Detect Control Register ->

	//MCLKSTS: Missing Clock Status Bit; 1: missing, 0: ok
    if (ClkCfgRegs.MCDCR.bit.MCLKSTS != 0)
    {// fallo
       EALLOW;
       // OSCCLKSRC2 failure detected. PLL running in limp mode.
       // Re-enable missing clock logic.
       //MCLKCLR: Missing Clock Clear Bit:1 to clear and reset
       ClkCfgRegs.MCDCR.bit.MCLKCLR = 1;
       EDIS;
       // Replace this line with a call to an appropriate
       // SystemShutdown(); function.
      __asm("        ESTOP0");     // Uncomment for debugging purposes
    }

    //no opera en limp mode, el reloj funciona bien

	if(clock_source != ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL)
		//verifica si el valor ya configurado es igual al que le pasamos como parametro
	{
		switch (clock_source)
		{
			case INT_OSC1:
				SysIntOsc1Sel();
				break;

			case INT_OSC2:
				SysIntOsc2Sel();
				break;

			case XTAL_OSC:
				SysXtalOscSel();
				break;
		}
	}

	 // first modify the PLL multipliers
	 if(imult != ClkCfgRegs.SYSPLLMULT.bit.IMULT || fmult != ClkCfgRegs.SYSPLLMULT.bit.FMULT)
	 {
		   EALLOW;
		   //modfiy dividers to maximum to reduce the inrush current
		   ClkCfgRegs.SYSPLLMULT.bit.IMULT = imult;		//Setting integer multiplier
		   ClkCfgRegs.SYSPLLMULT.bit.FMULT = fmult;      //Setting fractional multiplier

		   // ***********+ PROBAR SI ES EN REALIDAD NECESARIO ESCRIBIR ESTO ***********C
		   ClkCfgRegs.SYSPLLCTL1.bit.PLLEN = 1;		//Enable SYSPLL=1, 0: power off
		   EDIS;

		   //Wait for the SYSPLL lock
		   while(ClkCfgRegs.SYSPLLSTS.bit.LOCKS != 1) //1: not yet locked, 0:locked
		   {
		        // Uncomment to service the watchdog
		        // ServiceDog();

		   }
	 }

	 //increase the freq. of operation in steps to avoid any VDD fluctuations
	 temp_divsel = divsel+3;
	 if(temp_divsel > PLLCLK_BY_126)
	 {
		 temp_divsel = PLLCLK_BY_126; //63, pero el factor de división se multiplica por 2
	 }

	 while(ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV != divsel)
	 {
		 EALLOW;
		 ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = temp_divsel;
		 EDIS;
		 temp_divsel = temp_divsel - 1;

		 if(ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV != divsel)
		 {
			 DELAY_US(15L);
		 }
	 }

  //Enable PLLSYSCLK is fed from system PLL clock
   EALLOW;
   ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 1;
   EDIS;
}



//---------------------------------------------------------------------------
// Example: InitPll2:
//---------------------------------------------------------------------------
// This function initializes the PLL2 registers.

void InitAuxPll(Uint16 clock_source, Uint16 imult, Uint16 fmult, Uint16 divsel)
{
	Uint16 temp_divsel;

	if((clock_source == ClkCfgRegs.CLKSRCCTL2.bit.AUXOSCCLKSRCSEL)   &&
	  (imult		 == ClkCfgRegs.AUXPLLMULT.bit.IMULT) 	      &&
	  (fmult 	     == ClkCfgRegs.AUXPLLMULT.bit.FMULT)          &&
	  (divsel        == ClkCfgRegs.AUXCLKDIVSEL.bit.AUXPLLDIV))
    {
	    //everything is set as required, so just return
	    return;
	}

	switch (clock_source)
	{

		case INT_OSC2:
			AuxIntOsc2Sel();
			break;

		case XTAL_OSC:
			AuxXtalOscSel();
			break;

		case AUXCLKIN:
			AuxAuxClkSel();
			break;

	}

   // Change the SYSPLL Integer Multiplier (or) SYSPLL Fractional Multiplier
   if(ClkCfgRegs.AUXPLLMULT.bit.IMULT != imult || ClkCfgRegs.AUXPLLMULT.bit.FMULT !=fmult)
   {
	   EALLOW;
	   ClkCfgRegs.AUXCLKDIVSEL.bit.AUXPLLDIV = AUXPLLRAWCLK_BY_8;
	   ClkCfgRegs.AUXPLLMULT.bit.IMULT = imult;		//Setting integer multiplier
	   ClkCfgRegs.AUXPLLMULT.bit.FMULT = fmult;     //Setting fractional multiplier
	   ClkCfgRegs.AUXPLLCTL1.bit.PLLEN = 1;			//Enable AUXPLL
	   EDIS;

	   //Wait for the AUXPLL lock
	   while(ClkCfgRegs.AUXPLLSTS.bit.LOCKS != 1)
	   {
	        // Uncomment to service the watchdog
	        // ServiceDog();
	   }
   }

	 //increase the freq. of operation in steps to avoid any VDD fluctuations
	 temp_divsel = AUXPLLRAWCLK_BY_8;
	 while(ClkCfgRegs.AUXCLKDIVSEL.bit.AUXPLLDIV != divsel)
	 {
		 EALLOW;
		 ClkCfgRegs.AUXCLKDIVSEL.bit.AUXPLLDIV = temp_divsel - 1;
		 EDIS;

		 temp_divsel = temp_divsel - 1;
		 if(ClkCfgRegs.AUXCLKDIVSEL.bit.AUXPLLDIV != divsel)
		 {
			 DELAY_US(15L);
		 }
	 }

   EALLOW;
   ClkCfgRegs.AUXPLLCTL1.bit.PLLCLKEN = 1;			//Enable AUXPLLCLK is fed from AUX PLL
   EDIS;
}

//---------------------------------------------------------------------------
// Example: CsmUnlock:
//---------------------------------------------------------------------------
// This function unlocks the CSM. User must replace 0xFFFF's with current
// password for the DSP. Returns 1 if unlock is successful.

#define STATUS_FAIL          0
#define STATUS_SUCCESS       1

Uint16 CsmUnlock()
{
    volatile Uint16 temp;

    // Load the key registers with the current password. The 0xFFFF's are dummy
    // passwords.  User should replace them with the correct password for the DSP.

    EALLOW;
//    CsmRegs.KEY0 = 0xFFFF;
//    CsmRegs.KEY1 = 0xFFFF;
//    CsmRegs.KEY2 = 0xFFFF;
//    CsmRegs.KEY3 = 0xFFFF;
//    CsmRegs.KEY4 = 0xFFFF;
//    CsmRegs.KEY5 = 0xFFFF;
//    CsmRegs.KEY6 = 0xFFFF;
//    CsmRegs.KEY7 = 0xFFFF;

    DcsmZ1Regs.Z1_CSMKEY0 = 0xFFFFFFFF;
    DcsmZ1Regs.Z1_CSMKEY1 = 0xFFFFFFFF;
    DcsmZ1Regs.Z1_CSMKEY2 = 0xFFFFFFFF;
    DcsmZ1Regs.Z1_CSMKEY3  = 0xFFFFFFFF;

    DcsmZ2Regs.Z2_CSMKEY0 = 0xFFFFFFFF;
    DcsmZ2Regs.Z2_CSMKEY1 = 0xFFFFFFFF;
    DcsmZ2Regs.Z2_CSMKEY2 = 0xFFFFFFFF;
    DcsmZ2Regs.Z2_CSMKEY3  = 0xFFFFFFFF;
    EDIS;

    // Perform a dummy read of the password locations
    // if they match the key values, the CSM will unlock

//    temp = CsmPwl.PSWD0;
//    temp = CsmPwl.PSWD1;
//    temp = CsmPwl.PSWD2;
//    temp = CsmPwl.PSWD3;
//    temp = CsmPwl.PSWD4;
//    temp = CsmPwl.PSWD5;
//    temp = CsmPwl.PSWD6;
//    temp = CsmPwl.PSWD7;

    // If the CSM unlocked, return succes, otherwise return
    // failure.
//    if (CsmRegs.CSMSCR.bit.SECURE == 0) return STATUS_SUCCESS;
//    else return STATUS_FAIL;

    return 0;

}

//---------------------------------------------------------------------------
// Example: SysIntOsc1Sel:
//---------------------------------------------------------------------------
// This function switches to Internal Oscillator 1 and turns off all other clock
// sources to minimize power consumption

void SysIntOsc1Sel (void) {
    EALLOW;
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 2; // Clk Src = INTOSC1
    EDIS;
}

//---------------------------------------------------------------------------
// Example: SysIntOsc2Sel:
//---------------------------------------------------------------------------
// This function switches to Internal oscillator 2 from External Oscillator
// and turns off all other clock sources to minimize power consumption
	// NOTE: If there is no external clock connection, when switching from
	//       INTOSC1 to INTOSC2, EXTOSC and XLCKIN must be turned OFF prior
//       to switching to internal oscillator 1

void SysIntOsc2Sel (void) {

    EALLOW;
    //pag 264 manual pdf
    ClkCfgRegs.CLKSRCCTL1.bit.INTOSC2OFF=0;     // Turn on INTOSC2
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 0; // Clk Src = INTOSC2
    EDIS;

}

//---------------------------------------------------------------------------
// Example: SysXtalOscSel:
//---------------------------------------------------------------------------
// This function switches to External CRYSTAL oscillator and turns off all other clock
// sources to minimize power consumption. This option may not be available on all
// device packages

void SysXtalOscSel (void)  {

    EALLOW;
    //pag 264
    ClkCfgRegs.CLKSRCCTL1.bit.XTALOFF=0;        // Turn on XTALOSC
    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 1; // Clk Src = XTAL
    EDIS;

}

//---------------------------------------------------------------------------
// Example: AuxIntOsc2Sel:
//---------------------------------------------------------------------------
// This function switches to Internal oscillator 2 from External Oscillator
// and turns off all other clock sources to minimize power consumption
// NOTE: If there is no external clock connection, when switching from
//       INTOSC1 to INTOSC2, EXTOSC and XLCKIN must be turned OFF prior
//       to switching to internal oscillator 1
//from XTAL to INTOSC2
void AuxIntOsc2Sel (void) {

    EALLOW;
    ClkCfgRegs.CLKSRCCTL1.bit.INTOSC2OFF=0;     // Turn on INTOSC2, 1: off
    ClkCfgRegs.CLKSRCCTL2.bit.AUXOSCCLKSRCSEL = 0; // Clk Src = INTOSC2
    EDIS;

}

//---------------------------------------------------------------------------
// Example: AuxXtalOscSel:
//---------------------------------------------------------------------------
// This function switches to External CRYSTAL oscillator and turns off all other clock
// sources to minimize power consumption. This option may not be available on all
// device packages
//to XTAL from others
//page 267
void AuxXtalOscSel (void)  {

    EALLOW;
    /*CLKSRCCTL2 reg ->
    AUXOSCCLKSRCSEL: Oscillator Clock Source Select Bit: This bit selects the source for
    AUXOSCCLK
    	00 = INTOSC2 (default on reset)
		01 = External Oscillator (XTAL)
		10 = AUXCLKIN (from GPIO)
		11 = Reserved
    */
    ClkCfgRegs.CLKSRCCTL1.bit.XTALOFF=0;        // Turn on XTALOSC, 1: off
    ClkCfgRegs.CLKSRCCTL2.bit.AUXOSCCLKSRCSEL = 1; // Clk Src = XTAL
    EDIS;

}

//---------------------------------------------------------------------------
// Example: AuxAUXCLKOscSel:
//---------------------------------------------------------------------------
// This function switches to AUXCLKIN from GPIO and turns off all other clock
// sources to minimize power consumption. This option may not be available on all
// device packages

void AuxAuxClkSel (void)  {

    EALLOW;
    ClkCfgRegs.CLKSRCCTL2.bit.AUXOSCCLKSRCSEL = 2; // Clk Src = AUXCLKIN
    EDIS;

}


//Enter IDLE mode (single CPU)
/*LPMCR: Low Power Mode Control Register
 * set the low power mode for the device. Takes effect when CPU executes
 * the IDLE instruction (when IDLE instruction is out of EXE Phase of the Pipeline)
 * 	00: IDLE Mode
	01: STANDBY Mode
	10: HALT Mode
	11: HIB Mode
 * */
void IDLE()
{
	EALLOW;
	CpuSysRegs.LPMCR.bit.LPM = LPM_IDLE;
	EDIS;
	asm(" IDLE");
}

//Enter STANDBY mode (single CPU)
void STANDBY()
{
	EALLOW;
	CpuSysRegs.LPMCR.bit.LPM = LPM_STANDBY;
	EDIS;
	asm(" IDLE");
}

//Enter HALT mode (dual CPU). Puts CPU2 in STANDBY mode first.
void HALT()
{
	#if defined(CPU2)
		IDLE();
	#elif defined(CPU1)
		EALLOW;
		CpuSysRegs.LPMCR.bit.LPM = LPM_HALT;
		EDIS;
		while (DevCfgRegs.LPMSTAT.bit.CPU2LPMSTAT == 0x0) {;}
		asm(" IDLE");
	#endif
}

//Enter HIB mode (dual CPU). Puts CPU2 in STANDBY first. Alternately,
//CPU2 may be in reset.
void HIB()
{
	#if defined(CPU2)
		STANDBY();
	#elif defined(CPU1)
		EALLOW;
		CpuSysRegs.LPMCR.bit.LPM = LPM_HIB;
		EDIS;
		while (DevCfgRegs.LPMSTAT.bit.CPU2LPMSTAT == 0x0 && DevCfgRegs.RSTSTAT.bit.CPU2RES == 1) {;}
		asm(" IDLE");
	#endif
}
