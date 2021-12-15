/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F18875
        Driver Version    :  2.00
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

#include "mcc_generated_files/mcc.h"
#include "I2C/i2c.h"
#include "LCD/lcd.h"
#include "stdio.h"

/*
                         Main application
 */

#define LUM_LVL_01   0xFF
#define LUM_LVL_12   0x1FE
#define LUM_LVL_23   0x2FD
#define START_WRITE_RING_BUFFER_ADDR 0xFF //TODO
#define START_READ_RING_BUFFER_ADDR 0xFF //TODO

void S1(void){
    if(IO_RB4_GetValue()==LOW && IO_RA6_GetValue()==HIGH){
        IO_RA6_SetLow();
    }
}

void timerInterrupt(void){
    if(IO_RA7_GetValue()==LOW){
        IO_RA7_SetHigh();
    }else{
        IO_RA7_SetLow();
    }
    adc_result_t value = ADCC_GetSingleConversion(channel_ANA0);
    if(value > LUM_LVL_23 || value < LUM_LVL_01){
        IO_RA4_SetHigh();
    }else{
        IO_RA4_SetLow();
    }
}

unsigned char readTC74 (void)
{
	unsigned char value;
do{
	IdleI2C();
	StartI2C(); IdleI2C();
    
	WriteI2C(0x9a | 0x00); IdleI2C();
	WriteI2C(0x01); IdleI2C();
	RestartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x01); IdleI2C();
	value = ReadI2C(); IdleI2C();
	NotAckI2C(); IdleI2C();
	StopI2C();
} while (!(value & 0x40));

	IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x00); IdleI2C();
	WriteI2C(0x00); IdleI2C();
	RestartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x01); IdleI2C();
	value = ReadI2C(); IdleI2C();
	NotAckI2C(); IdleI2C();
	StopI2C();

	return value;
}

uint8_t readEPROM(uint16_t dataAddr){
    uint16_t start = START_READ_RING_BUFFER_ADDR; 

    return 0x55;
}

void writeEPROM(uint16_t dataAddr,uint8_t data){
    uint16_t start = START_WRITE_RING_BUFFER_ADDR; 

    return 0x55;
}

uint8_t readRingBuffer(uint16_t dataAddr){
    return 0x55;
}

void writeRingBuffer(uint16_t dataAddr,uint8_t data){
    return 0x55;
}

void main(void)
{
    unsigned char c;
    char buf[17];
    uint16_t writeRingBufferAddr = START_WRITE_RING_BUFFER_ADDR; 
    uint16_t readRingBufferAddr = START_READ_RING_BUFFER_ADDR; 
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
    
    TMR1_SetInterruptHandler(timerInterrupt);
    
    IO_RA6_SetHigh();
     
    OpenI2C();
    //I2C_SCL = 1;
    //I2C_SDA = 1;
    //WPUC3 = 1;
    //WPUC4 = 1;
    LCDinit();
    
    uint16_t dataeeAddr = 0xF010;
    uint8_t dataeeData = 0x55;
    DATAEE_WriteByte(dataeeAddr,dataeeData);
    
    NOP();
    uint8_t FILIPE = DATAEE_ReadByte(dataeeAddr);
    NOP();
    
    while (1)
    {   
        S1();
        
        c = readTC74();
        LCDcmd(0x80);       //first line, first column
        while (LCDbusy());
        LCDstr("Temp");
        LCDpos(0,8);
        while (LCDbusy());
        LCDstr("STR-RTS");
        LCDcmd(0xc0);       // second line, first column
        sprintf(buf, "%02d C", c);
        while (LCDbusy());
        LCDstr(buf);
        __delay_ms(2000);
    }
}
/**
 End of File
*/