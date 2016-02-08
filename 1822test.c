/**********************************************************************
* File: 1822test.c                                                    *
* Date: 02/06/2016                                                    *
* File Version: 1                                                     *
*                                                                     *
* Author: M10                                                         *
* Company:                                                            *
***********************************************************************
* Notes:                                                              *
*     Clock source = internal 500 khz osc                             *
*                                                                     *
*              PIC12F1822 pinout for this project                     *
*              ----------                                             *
* 3.3-5V-- Vdd |1  U   8| GND                                         *
*          GP5 |2      7| GP0 --> IR LED output                       *
*          GP4 |3      6| GP1 <-- Button 1 input --> GND              *
*          GP3 |4      5| GP2 <-- Button 2 input --> GND              *
*              ----------                                             *
*                                                                     *
**********************************************************************/
// compile with: xc8 --chip=12f1822 example.c

#include <xc.h>

// PIC12F1822 Configuration Bit Settings
// 'C' source line config statements
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC	INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (OFF	MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (OFF	Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (OFF	Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (OFF	CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (OFF	Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (OFF	Fail-Safe Clock Monitor is disabled)
// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (OFF	Write protection off)
#pragma config PLLEN = OFF      // Phase Lock Loop enable (OFF	4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (ON	Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (LO	Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (OFF	High-voltage on MCLR/VPP must be used for programming)

void main(void) {
    unsigned char portValue;    // always use a variable to hold the value you want the port to assume

    // Port A access
    ANSELA = 0x0;               // set to digital I/O (not analog)
    TRISA = 0x0;                // set all port bits to be output

    while(1) {
        portValue = 0b00110111; // RA[0:5] except RA[3]
        LATA = portValue;       // write to port latch
        _delay(50000);          // compiler built-in function - see XC8 Compiler User's Guide
        portValue = 0b00000000;
        LATA = portValue;
        _delay(50000);          // instruction cycles - (50000/125000) sec for 500kHz clock
    }
    return;                     // we should never reach this
    // jumps back to reset vector
}
