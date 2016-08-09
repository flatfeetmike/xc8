/**********************************************************************
* File: txtest.c                                                      *
* Date: 08/09/2016                                                    *
* File Version: 1                                                     *
*                                                                     *
* Author: M10                                                         *
* Company:                                                            *
***********************************************************************
* Notes:                                                              *
*     Clock source = INTOSC, OSCCON set to 16 MHz HF                  *
*                                                                     *
*               PIC12F1822 pinout for this project                    *
*               ----------                                            *
* 3.3-5V -- Vdd |1  U   8| GND                                        *
*    RX --> RA5 |2      7| RA0/ICSPDAT                                *
*    TX <-- RA4 |3      6| RA1/ICSPCLK                                *
*   Vpp --> RA3 |4      5| RA2                                        *
*               ----------                                            *
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

// Definitions
#define _XTAL_FREQ  16000000    // this is used by the __delay_ms(xx) and __delay_us(xx) functions

void main(void) {
    unsigned char portValue;    // always use a variable to hold the value you want the port to assume

    // set up oscillator control register
    OSCCONbits.SPLLEN = 0;      // PLL is disabled
    OSCCONbits.IRCF = 0x0f;     //set OSCCON IRCF bits to select OSC frequency = 16 MHz HF
    OSCCONbits.SCS = 0x02;      //set the SCS bits to select internal oscillator block
    // OSCCON should be 0x7Ah now.

    // Port A access
    ANSELA = 0x0;               // set to digital I/O (not analog)
    TRISA = 0x0;                // set all port bits to be output

    while(1) {
        portValue = 0b00110111; // RA[0:5] except RA[3]
        LATA = portValue;       // write to port latch
        __delay_ms(100);

        portValue = 0b00000000;
        LATA = portValue;
        __delay_ms(900);
    }
    return;                     // we should never reach this
    // jumps back to reset vector
}
