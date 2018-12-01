/**
  Lab Header File

  @Company
    Microchip Technology Inc.

  @File Name
    labs.h

  @Summary
    This is the header file for the variables declarations and function prototypes

  @Description
    This header file provides all the variables and function prototypes used in this project.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65
        Device            :  PIC16F1619
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.45
        MPLAB             :  MPLAB X v4.15
 */

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef LABS_H
#define	LABS_H

#include <xc.h>
#include <stdint.h>
    

/**
  Section: Macro Declarations
 */
#define PRESSED             1
#define NOT_PRESSED         0
#define RUNNING             1
#define NOT_RUNNING         0
#define LAST                16
#define LEDs_SetLow()       do { LATA = 0; LATCbits.LATC5 = 0; } while(0)

#define INTERRUPT_TMR0InterruptEnable()         do { TMR0IE = 1; } while(0)
#define INTERRUPT_TMR0InterruptDisable()        do { TMR0IE = 0; } while(0)

/**
  Duty cycle defines
 */
#define DTC0                   0
#define DTC10                480
#define DTC20                960
#define DTC30               1440
#define DTC40               1920
#define DTC50               2400
#define DTC60               2880
#define DTC70               3360
#define DTC80               3840
#define DTC90               4320
#define DTC100              4800


/**
  Section: Variable Definitions
 */
uint8_t labNumber = 0;
bool switch1Event = false;
bool switch2Event = false;       // not used in first version
bool switch3Event = false;
    
uint8_t labState = NOT_RUNNING;
uint8_t btn1State = NOT_PRESSED;
uint8_t btn2State = NOT_PRESSED;  // not used in first version
uint8_t btn3State = NOT_PRESSED; 
 
enum { STATE0=0, STATE1, STATE2, STATE3, STATE4, STATE5, STATE6,
         STATE7, STATE8, STATE9, STATE10, STATE_LAST};   // PWM state machine
    
struct PWM_t
{
    unsigned char state;                    // state machine state #
    unsigned char ONOFF;                    // PWM on/off state
    unsigned char Enable;                   // PWM enable/disable
    int value;                              // PWM value
};

bool EEP_Flag = false;
uint16_t State_EEP_Addr = 0x0380;           // first word of HEF
uint16_t EEP_State = 0;
#define magic 0x3C

#endif	/* LABS_H */
/**
 End of File
 */
 
 
 