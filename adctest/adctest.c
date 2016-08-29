/**********************************************************************
* File: adctest.c                                                     *
* Date: 08/24/2016                                                    *
* File Version: 1                                                     *
*                                                                     *
* Author: M10                                                         *
* Company:                                                            *
***********************************************************************
* Notes:                                                              *
*       Clock source = INTOSC, OSCCON set to 16 MHz HF                 *
*                                                                     *
*       PIC12F1822 pinout for this project                            *
*                           ----------                                *
*             3.3-5V -- Vdd |1  U   8| GND                            *
*                RX --> RA5 |2      7| RA0/ICSPDAT                    *
*                TX <-- RA4 |3      6| RA1/ICSPCLK                    *
*               Vpp --> RA3 |4      5| RA2 <-- Sensor                 *
*                           ----------                                *
* compile with:                                                       *
* $ xc8 --chip=12f1822 example.c                                      *
**********************************************************************/

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
#define _XTAL_FREQ 16000000      // this is used by the __delay_ms(xx) and __delay_us(xx) functions

//**********************************************************************************
// This subroutine does the ADC conversion and returns the 10 bit result
//**********************************************************************************
unsigned int Read_ADC_Value(void)
{
    unsigned int ADCValue;
    
    ADCON0bits.GO = 1;      // start conversion
    while(ADCON0bits.GO);   // wait for conversion to finish
    ADCValue = ADRESH << 8; // get the 2 msbs of the result and rotate 8 bits to the left
    ADCValue = ADCValue + ADRESL;   // now add the low 8 bits of the resut into our return variable
    return (ADCValue);      // return the 10bit result in a single variable
}

//**********************************************************************************
//*****************   main routine   ***********************************************
//**********************************************************************************
void main(void) {
    unsigned char portValue;    // always use a variable to hold the value you want the port to assume
    unsigned char ch[4];
    unsigned int AnalogValue;       // used to store ADC result after capture

    // set up oscillator control register
    OSCCONbits.SPLLEN = 0;      // PLL is disabled (POR default)
    OSCCONbits.IRCF   = 0b1111; // set OSCCON IRCF bits to select OSC frequency = 16 MHz HF
    OSCCONbits.SCS    = 0b10;   // select internal oscillator block regardless of FOSC

    // set up port A I/O pins
    TRISAbits.TRISA0 = 0;	// RA0 = nc
    TRISAbits.TRISA1 = 0;	// RA1 = nc
    TRISAbits.TRISA2 = 1;	// RA2 = Analog Voltage In
    TRISAbits.TRISA3 = 0;	// RA3 = nc (MCLR)
    TRISAbits.TRISA4 = 0;	// RA4 = nc
    TRISAbits.TRISA5 = 0;	// RA5 = nc

    DACCON0bits.DACEN = 0;  // turn DAC off
    ANSELAbits.ANSA2 = 1;	// Select RA2 as analog input pin for potentiometer input
    ADCON0bits.CHS = 0x02;	// select RA2 analog channel
    ADCON0bits.ADON = 1;    // ADC is on
    ADCON1bits.ADCS = 0x01;	// select ADC conversion clock select as Fosc/8
    ADCON1bits.ADFM = 1;    // results are right justified
 
    // select alternative pin functions
    RXDTSEL = 1;                // select RA5 as rx port
    TXCKSEL = 1;                // select RA4 as tx port

    // setup serial port (8bit, no parity)
    RCSTA = 0b10010000;         // (*SPEN RX9  SREN *CREN  ADEN   FERR OERR RX9D)
    TXSTA = 0b00100100;         // ( CSRC TX9 *TXEN  SYNC  SENDB *BRGH TRMT TX9D)
    // set baudrate to 9600 bps
    // baudrate = fosc / [16 (n + 1)]
    SPBRG = 103;                 // 9615 actual (0.16% error) 

    __delay_ms(500);

    while(1) {
        AnalogValue = Read_ADC_Value();
        
        ch[0] = AnalogValue % 10 + '0';   // 10^0 digit
        AnalogValue = AnalogValue / 10;
        ch[1] = AnalogValue % 10 + '0';   // 10^1 digit
        AnalogValue = AnalogValue / 10;
        ch[2] = AnalogValue % 10 + '0';   // 10^2 digit
        AnalogValue = AnalogValue / 10;
        ch[3] = AnalogValue % 10 + '0';   // 10^3 digit
        
        while(TXIF == 0);           // block until tx buffer clears
        TXREG = ch[3];              // send the octet
        while(TXIF == 0);
        TXREG = ch[2];
        while(TXIF == 0);
        TXREG = ch[1];
        while(TXIF == 0);
        TXREG = ch[0];
        while(TXIF == 0);
        TXREG = '\r';
        while(TXIF == 0);
        TXREG = '\n';

        __delay_ms(200);
    }
    return;                     // we should never reach this
    // jumps back to reset vector
}
