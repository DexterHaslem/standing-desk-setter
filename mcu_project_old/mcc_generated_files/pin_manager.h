/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16LF18346
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RA0 aliases
#define IO_RA0_TRIS                 TRISAbits.TRISA0
#define IO_RA0_LAT                  LATAbits.LATA0
#define IO_RA0_PORT                 PORTAbits.RA0
#define IO_RA0_WPU                  WPUAbits.WPUA0
#define IO_RA0_OD                   ODCONAbits.ODCA0
#define IO_RA0_ANS                  ANSELAbits.ANSA0
#define IO_RA0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define IO_RA0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define IO_RA0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define IO_RA0_GetValue()           PORTAbits.RA0
#define IO_RA0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define IO_RA0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define IO_RA0_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define IO_RA0_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define IO_RA0_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define IO_RA0_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define IO_RA0_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define IO_RA0_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set IO_RA1 aliases
#define IO_RA1_TRIS                 TRISAbits.TRISA1
#define IO_RA1_LAT                  LATAbits.LATA1
#define IO_RA1_PORT                 PORTAbits.RA1
#define IO_RA1_WPU                  WPUAbits.WPUA1
#define IO_RA1_OD                   ODCONAbits.ODCA1
#define IO_RA1_ANS                  ANSELAbits.ANSA1
#define IO_RA1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define IO_RA1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define IO_RA1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define IO_RA1_GetValue()           PORTAbits.RA1
#define IO_RA1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define IO_RA1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define IO_RA1_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define IO_RA1_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define IO_RA1_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define IO_RA1_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define IO_RA1_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define IO_RA1_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set IO_RA2 aliases
#define IO_RA2_TRIS                 TRISAbits.TRISA2
#define IO_RA2_LAT                  LATAbits.LATA2
#define IO_RA2_PORT                 PORTAbits.RA2
#define IO_RA2_WPU                  WPUAbits.WPUA2
#define IO_RA2_OD                   ODCONAbits.ODCA2
#define IO_RA2_ANS                  ANSELAbits.ANSA2
#define IO_RA2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define IO_RA2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define IO_RA2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define IO_RA2_GetValue()           PORTAbits.RA2
#define IO_RA2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define IO_RA2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define IO_RA2_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define IO_RA2_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define IO_RA2_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define IO_RA2_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define IO_RA2_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define IO_RA2_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set IO_OUT_DOWN aliases
#define IO_OUT_DOWN_TRIS                 TRISAbits.TRISA4
#define IO_OUT_DOWN_LAT                  LATAbits.LATA4
#define IO_OUT_DOWN_PORT                 PORTAbits.RA4
#define IO_OUT_DOWN_WPU                  WPUAbits.WPUA4
#define IO_OUT_DOWN_OD                   ODCONAbits.ODCA4
#define IO_OUT_DOWN_ANS                  ANSELAbits.ANSA4
#define IO_OUT_DOWN_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define IO_OUT_DOWN_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define IO_OUT_DOWN_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define IO_OUT_DOWN_GetValue()           PORTAbits.RA4
#define IO_OUT_DOWN_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define IO_OUT_DOWN_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define IO_OUT_DOWN_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define IO_OUT_DOWN_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define IO_OUT_DOWN_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define IO_OUT_DOWN_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define IO_OUT_DOWN_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define IO_OUT_DOWN_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set IO_OUT_UP aliases
#define IO_OUT_UP_TRIS                 TRISAbits.TRISA5
#define IO_OUT_UP_LAT                  LATAbits.LATA5
#define IO_OUT_UP_PORT                 PORTAbits.RA5
#define IO_OUT_UP_WPU                  WPUAbits.WPUA5
#define IO_OUT_UP_OD                   ODCONAbits.ODCA5
#define IO_OUT_UP_ANS                  ANSELAbits.ANSA5
#define IO_OUT_UP_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define IO_OUT_UP_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define IO_OUT_UP_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define IO_OUT_UP_GetValue()           PORTAbits.RA5
#define IO_OUT_UP_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define IO_OUT_UP_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define IO_OUT_UP_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define IO_OUT_UP_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define IO_OUT_UP_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define IO_OUT_UP_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define IO_OUT_UP_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define IO_OUT_UP_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set IO_STATUS_LED aliases
#define IO_STATUS_LED_TRIS                 TRISBbits.TRISB4
#define IO_STATUS_LED_LAT                  LATBbits.LATB4
#define IO_STATUS_LED_PORT                 PORTBbits.RB4
#define IO_STATUS_LED_WPU                  WPUBbits.WPUB4
#define IO_STATUS_LED_OD                   ODCONBbits.ODCB4
#define IO_STATUS_LED_ANS                  ANSELBbits.ANSB4
#define IO_STATUS_LED_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define IO_STATUS_LED_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define IO_STATUS_LED_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define IO_STATUS_LED_GetValue()           PORTBbits.RB4
#define IO_STATUS_LED_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define IO_STATUS_LED_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define IO_STATUS_LED_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define IO_STATUS_LED_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define IO_STATUS_LED_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define IO_STATUS_LED_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define IO_STATUS_LED_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define IO_STATUS_LED_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set IO_OUT_VSHTDN aliases
#define IO_OUT_VSHTDN_TRIS                 TRISBbits.TRISB7
#define IO_OUT_VSHTDN_LAT                  LATBbits.LATB7
#define IO_OUT_VSHTDN_PORT                 PORTBbits.RB7
#define IO_OUT_VSHTDN_WPU                  WPUBbits.WPUB7
#define IO_OUT_VSHTDN_OD                   ODCONBbits.ODCB7
#define IO_OUT_VSHTDN_ANS                  ANSELBbits.ANSB7
#define IO_OUT_VSHTDN_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define IO_OUT_VSHTDN_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define IO_OUT_VSHTDN_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define IO_OUT_VSHTDN_GetValue()           PORTBbits.RB7
#define IO_OUT_VSHTDN_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define IO_OUT_VSHTDN_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define IO_OUT_VSHTDN_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define IO_OUT_VSHTDN_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define IO_OUT_VSHTDN_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define IO_OUT_VSHTDN_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define IO_OUT_VSHTDN_SetAnalogMode()      do { ANSELBbits.ANSB7 = 1; } while(0)
#define IO_OUT_VSHTDN_SetDigitalMode()     do { ANSELBbits.ANSB7 = 0; } while(0)

// get/set IO_RC0 aliases
#define IO_RC0_TRIS                 TRISCbits.TRISC0
#define IO_RC0_LAT                  LATCbits.LATC0
#define IO_RC0_PORT                 PORTCbits.RC0
#define IO_RC0_WPU                  WPUCbits.WPUC0
#define IO_RC0_OD                   ODCONCbits.ODCC0
#define IO_RC0_ANS                  ANSELCbits.ANSC0
#define IO_RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define IO_RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define IO_RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define IO_RC0_GetValue()           PORTCbits.RC0
#define IO_RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define IO_RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define IO_RC0_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define IO_RC0_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define IO_RC0_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define IO_RC0_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define IO_RC0_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define IO_RC0_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set IO_RC1 aliases
#define IO_RC1_TRIS                 TRISCbits.TRISC1
#define IO_RC1_LAT                  LATCbits.LATC1
#define IO_RC1_PORT                 PORTCbits.RC1
#define IO_RC1_WPU                  WPUCbits.WPUC1
#define IO_RC1_OD                   ODCONCbits.ODCC1
#define IO_RC1_ANS                  ANSELCbits.ANSC1
#define IO_RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define IO_RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define IO_RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define IO_RC1_GetValue()           PORTCbits.RC1
#define IO_RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define IO_RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define IO_RC1_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define IO_RC1_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define IO_RC1_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define IO_RC1_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define IO_RC1_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define IO_RC1_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set IO_RC2 aliases
#define IO_RC2_TRIS                 TRISCbits.TRISC2
#define IO_RC2_LAT                  LATCbits.LATC2
#define IO_RC2_PORT                 PORTCbits.RC2
#define IO_RC2_WPU                  WPUCbits.WPUC2
#define IO_RC2_OD                   ODCONCbits.ODCC2
#define IO_RC2_ANS                  ANSELCbits.ANSC2
#define IO_RC2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define IO_RC2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define IO_RC2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define IO_RC2_GetValue()           PORTCbits.RC2
#define IO_RC2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define IO_RC2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define IO_RC2_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define IO_RC2_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define IO_RC2_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define IO_RC2_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define IO_RC2_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define IO_RC2_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set IO_IN_VINT aliases
#define IO_IN_VINT_TRIS                 TRISCbits.TRISC3
#define IO_IN_VINT_LAT                  LATCbits.LATC3
#define IO_IN_VINT_PORT                 PORTCbits.RC3
#define IO_IN_VINT_WPU                  WPUCbits.WPUC3
#define IO_IN_VINT_OD                   ODCONCbits.ODCC3
#define IO_IN_VINT_ANS                  ANSELCbits.ANSC3
#define IO_IN_VINT_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define IO_IN_VINT_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define IO_IN_VINT_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define IO_IN_VINT_GetValue()           PORTCbits.RC3
#define IO_IN_VINT_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define IO_IN_VINT_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define IO_IN_VINT_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define IO_IN_VINT_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define IO_IN_VINT_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define IO_IN_VINT_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define IO_IN_VINT_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define IO_IN_VINT_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set IO_IN_BTN2 aliases
#define IO_IN_BTN2_TRIS                 TRISCbits.TRISC4
#define IO_IN_BTN2_LAT                  LATCbits.LATC4
#define IO_IN_BTN2_PORT                 PORTCbits.RC4
#define IO_IN_BTN2_WPU                  WPUCbits.WPUC4
#define IO_IN_BTN2_OD                   ODCONCbits.ODCC4
#define IO_IN_BTN2_ANS                  ANSELCbits.ANSC4
#define IO_IN_BTN2_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define IO_IN_BTN2_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define IO_IN_BTN2_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define IO_IN_BTN2_GetValue()           PORTCbits.RC4
#define IO_IN_BTN2_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define IO_IN_BTN2_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define IO_IN_BTN2_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define IO_IN_BTN2_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define IO_IN_BTN2_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define IO_IN_BTN2_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define IO_IN_BTN2_SetAnalogMode()      do { ANSELCbits.ANSC4 = 1; } while(0)
#define IO_IN_BTN2_SetDigitalMode()     do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set IO_IN_BTN1 aliases
#define IO_IN_BTN1_TRIS                 TRISCbits.TRISC5
#define IO_IN_BTN1_LAT                  LATCbits.LATC5
#define IO_IN_BTN1_PORT                 PORTCbits.RC5
#define IO_IN_BTN1_WPU                  WPUCbits.WPUC5
#define IO_IN_BTN1_OD                   ODCONCbits.ODCC5
#define IO_IN_BTN1_ANS                  ANSELCbits.ANSC5
#define IO_IN_BTN1_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define IO_IN_BTN1_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define IO_IN_BTN1_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define IO_IN_BTN1_GetValue()           PORTCbits.RC5
#define IO_IN_BTN1_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define IO_IN_BTN1_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define IO_IN_BTN1_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define IO_IN_BTN1_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define IO_IN_BTN1_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define IO_IN_BTN1_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define IO_IN_BTN1_SetAnalogMode()      do { ANSELCbits.ANSC5 = 1; } while(0)
#define IO_IN_BTN1_SetDigitalMode()     do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF3 pin functionality
 * @Example
    IOCCF3_ISR();
 */
void IOCCF3_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF3 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF3 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF3_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF3_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF3 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF3_SetInterruptHandler() method.
    This handler is called every time the IOCCF3 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF3_SetInterruptHandler(IOCCF3_InterruptHandler);

*/
extern void (*IOCCF3_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF3 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF3_SetInterruptHandler() method.
    This handler is called every time the IOCCF3 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF3_SetInterruptHandler(IOCCF3_DefaultInterruptHandler);

*/
void IOCCF3_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF4 pin functionality
 * @Example
    IOCCF4_ISR();
 */
void IOCCF4_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF4 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF4 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF4_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF4_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF4 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF4_SetInterruptHandler() method.
    This handler is called every time the IOCCF4 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF4_SetInterruptHandler(IOCCF4_InterruptHandler);

*/
extern void (*IOCCF4_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF4 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF4_SetInterruptHandler() method.
    This handler is called every time the IOCCF4 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF4_SetInterruptHandler(IOCCF4_DefaultInterruptHandler);

*/
void IOCCF4_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF5 pin functionality
 * @Example
    IOCCF5_ISR();
 */
void IOCCF5_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF5 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF5 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF5_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF5_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF5 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF5_SetInterruptHandler() method.
    This handler is called every time the IOCCF5 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF5_SetInterruptHandler(IOCCF5_InterruptHandler);

*/
extern void (*IOCCF5_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF5 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF5_SetInterruptHandler() method.
    This handler is called every time the IOCCF5 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF5_SetInterruptHandler(IOCCF5_DefaultInterruptHandler);

*/
void IOCCF5_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/