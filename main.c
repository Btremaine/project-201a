/**
 * Project201 for Onanon Inc.
 *7 Date July 20, 2018
 * Developed by Brian Tremaine, TremaineConsultingGroup.com
 * btremaine@gmail.com
 * 
 * processor: PIC 12F1571
*/

#include "mcc_generated_files/mcc.h"
#include "labs.h"

void checkButtonS1(void);
//void checkButtonS2(void);
void checkButtonS3(void);
void initializeLED(void);
void runStateMachine(void);
void updatePWMvalue(void);
void SaveStateToFlash(void);
void SetStateFromFlash(void);

/*
                    Main application
 * 
 *    1. LED out on RA5 using PWM1 output
 *    2. Switch S1 on RA2 input, bump LED intensity down, weak PU
 *    3. Switch S3 on RA4 input, bump LED intensity up, weak PU
 *    4. PWM2 out on RA5, inverted
 *    5. phase 1 board, SW S2 not functional
 * TO-DO
 *    1. Code functions as designed for S1 and S3. <complete>
 *    2. add logic to test SW2 to update EEP memory if pressed > 2 seconds ---- PH2 NEW PCBA
 *
 */
struct PWM_t LED ;

void main(void)
{
     // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    // ---- 8/1/2018 reverse operation of S1 and S3. S1 is down, S3 is up
    
    PWM1_Start();
    initializeLED();
    TMR1_StopTimer();
    
    //__delay_ms(10);

    while (1) {          
        checkButtonS1();  
        checkButtonS3();
        if (switch1Event && !EEP_Flag) {
            runStateMachine();
            updatePWMvalue();
        } else if (switch3Event && !EEP_Flag) {  
            runStateMachine();
            updatePWMvalue();
        }
        if(EEP_Flag){
            EEP_Flag = false;
            SaveStateToFlash();          
        }
    }
}
void checkButtonS1(void){
    // button for power down 10%
    if(btn1State == NOT_PRESSED){
        if(SWITCH_S1_PORT == LOW){  
            __delay_ms(50);
            btn1State = PRESSED;
            // to-do: start timer
            TMR1_StartTimer();
        }
    }else if(SWITCH_S1_PORT == HIGH){
            btn1State = NOT_PRESSED;
            switch1Event = 1; 
            TMR1_StopTimer();
    }
}

/*
void checkButtonS2(void){
    // button for power on/off and save to flash
    if(btn2State == NOT_PRESSED){
        if(SWITCH_S2_PORT == LOW){  
            __delay_ms(50);
            btn2State = PRESSED;
            // to-do: start timer
            TMR1_StartTimer();
        }
    }else if(SWITCH_S2_PORT == HIGH){
            btn2State = NOT_PRESSED;
            
            switch2Event = 1;                                                   
    }
}
*/

void checkButtonS3(void){
    // button for power up 10%
    if(btn3State == NOT_PRESSED){
        if(SWITCH_S3_PORT == LOW){  
            __delay_ms(50);
            btn3State = PRESSED;
            // to-do: start timer
            TMR1_StartTimer();
        }
    }else if(SWITCH_S3_PORT == HIGH){
            btn3State = NOT_PRESSED;
            switch3Event = 1; 
            TMR1_StopTimer();
    }
}

void initializeLED(void){
    /* Check EEP memory and restore state, otherwise set to off.
       for now set to off state */
    SetStateFromFlash();
}

void runStateMachine(void){
    
    unsigned state = LED.state; 
    unsigned next_state = state;
    
    // run through state machine
    switch(state)
    {   
        case STATE0:   // disabled
            next_state = STATE0;
            if(switch3Event) {
                next_state = STATE1; 
            }
            break;
        case STATE1: 
            next_state = STATE1;
            if(switch3Event) {
                next_state = STATE2;
            }
            else if (switch1Event) {  // temporary for debug
                next_state = STATE0;
            }
            break;
        case STATE2:
            next_state = STATE2;
            if(switch3Event) {
                next_state = STATE3;
            }
            else if (switch1Event) {
                next_state = STATE1;
            }
            break;
        case STATE3:
            next_state = STATE3;
            if(switch3Event) {
                next_state = STATE4;
            }
            else if (switch1Event) {
                next_state = STATE2;
            }
            break;
        case STATE4:
            next_state = STATE4;
            if(switch3Event) {
                next_state = STATE5;
            }
            else if (switch1Event) {
                next_state = STATE3;
            }
            break;
        case STATE5:
            next_state = STATE5;
            if(switch3Event) {
                next_state = STATE6;
            }
            else if (switch1Event) {
                next_state = STATE4;
            }
            break;
        case STATE6:
            next_state = STATE6;
            if (switch3Event) {
                next_state = STATE7;
            }
            else if (switch1Event){
                next_state = STATE5;
            }
            break;  
       case STATE7:
            next_state = STATE7;
            if (switch3Event) {
                next_state = STATE8;
            }
            else if (switch1Event){
                next_state = STATE6;
            }
            break;  
       case STATE8:
            next_state = STATE8;
            if (switch3Event) {
                next_state = STATE9;
            }
            else if (switch1Event){
                next_state = STATE7;
            }
            break; 
       case STATE9:
            next_state = STATE9;
            if (switch3Event) {
                next_state = STATE10;
            }
            else if (switch1Event){
                next_state = STATE8;
            }
            break;
       case STATE10:
            next_state = STATE10;
            if (switch1Event){
                next_state = STATE9;
            }
            break;    

        default:
            next_state = STATE0;
    }

    state = next_state;

    // update structure
    LED.state = state ;
}
    
void updatePWMvalue(void) {
    
   unsigned state = LED.state;
   
   if (switch1Event | switch3Event) {
       switch (state) {
        case STATE0:
            PWM1_DutyCycleSet(DTC0);
            break;
        case STATE1:
            PWM1_DutyCycleSet(DTC10);
            break;
        case STATE2:
            PWM1_DutyCycleSet(DTC20);
            break;
        case STATE3:
            PWM1_DutyCycleSet(DTC30);
            break;
        case STATE4:
            PWM1_DutyCycleSet(DTC40);
            break;
        case STATE5:
            PWM1_DutyCycleSet(DTC50);
            break;
        case STATE6:
            PWM1_DutyCycleSet(DTC60);
            break; 
        case STATE7:
            PWM1_DutyCycleSet(DTC70);
            break;     
        case STATE8:
            PWM1_DutyCycleSet(DTC80);
            break;     
        case STATE9:
            PWM1_DutyCycleSet(DTC90);
            break;  
        case STATE10:
            PWM1_DutyCycleSet(DTC100);
            break;     
        default:
            PWM1_DutyCycleSet(DTC0);
       }
       
       PWM1_LoadBufferSet();
       
       switch1Event = 0;
       switch3Event = 0;
    }
}
void SaveStateToFlash(void)
{
    unsigned state = LED.state;
    
    uint16_t flashAddr = State_EEP_Addr;
    uint16_t ramBuf[ERASE_FLASH_BLOCKSIZE];
    uint16_t word ;
    uint16_t magic_byte = magic;
    
    word = (magic_byte << 8) | state&0xFF;
    
    FLASH_WriteWord( flashAddr, ramBuf, word );
    EEP_Flag = false;
}

void SetStateFromFlash(void)
{
    // to-do add code to make programmed state 'special'
    // then check before using
    uint16_t word;
    
    word = FLASH_ReadWord(State_EEP_Addr);
    // check magic
    if((word >> 8)== magic){
        EEP_State = word & 0xFF;
        if(EEP_State < STATE_LAST){
           LED.state = EEP_State;
           PWM1_DutyCycleSet(EEP_State);
        }
        else PWM1_DutyCycleSet(STATE0);
    }
    else PWM1_DutyCycleSet(STATE0);   
}


/**
 End of File
*/