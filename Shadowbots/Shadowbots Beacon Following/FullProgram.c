/*
 * File:   FullProgram.c
 * Author: Abdallah Muhammed
 * Created: 01/12/2022
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PWRT = ON
#define _XTAL_FREQ 10000000     // define clock frequency for __delay_10ms()
#define LED1 LATBbits.LATB2     //Define LED1
#define LED2 LATBbits.LATB3     //Define LED2
#define LED3 LATBbits.LATB4     //Define LED3
#define LED4 LATBbits.LATB5     //Define LED4
#define rightMotorA1 LATBbits.LATB0     //Right Motor A1
#define rightMotorA2 LATBbits.LATB1     //Right Motor A2
#define leftMotorA1 LATAbits.LATA4      //Right Motor A1
#define leftMotorA2 LATAbits.LATA5      //Right Motor A2

void startUp(void);
void flashLEDs(int del);        //Flashes all 4 LEDs
void wait10ms(int del);         //Generates a delay in multiples of 10ms
void findBeacon(void);          //Searches for the beacon
unsigned int readADCLeft(void);    //Read ADC for left sensor
unsigned int readADCRight(void);    //Read ADC for right sensor
void turn90(char direction, int del);
void goForward(int del);
void reverse(int del);
int isBeaconFound(void);
unsigned int setpoint_distance = 400;   //Distance set point


void main(void) {
    int encoderLeft;
    int encoderRight;
    unsigned char markspace = 100;
    TRISA = 0b11001111;      //Sets respective port A bits to input and output
    TRISB = 0b11000000;      //Sets respective port B bits to input and output
    TRISC = 0b11111001;      //Sets respective port C bits to input and output
    PR2 = 0b11111111 ;       //set period of PWM
    T2CON = 0b00000111 ;     //Timer 2(TMR2) on, Prescaler = 16
    CCP1CON = (0x0c);        //0x0c enables PWM module CCP1
    CCP2CON = (0x0c);        //0x0c enables PWM module CCP2
    ADCON1 = 0b00001101;  //Set voltage reference and port A0 as analogue input
    ADCON2 = 0b10000010; // Fosc/32, A/D result right justified
    CCPR1L = markspace;      //Load duty cycle into CCP1CON, PWM begins
    CCPR2L = markspace;      //Load duty cycle into CCP2CON, PWM begins
    LATB=0;             //Turn all Leds off
    
    startUp();               //Calls the start up sequence
    findBeacon();
    
    while(1){
        CCPR1L = markspace;      //Load duty cycle into CCP1CON, PWM begins
        CCPR2L = markspace;      //Load duty cycle into CCP2CON, PWM begins
        if (isBeaconFound() == 1){
            findBeacon();
            goForward(0);
            if((readADCLeft() >= setpoint_distance)){
                turn90('r', 85);
                LED4 = 1;
                findBeacon();
            }
            if((readADCRight() >= setpoint_distance)){
                turn90('l', 85);
                LED1 = 1;
                findBeacon();
            }
            LED4=0;
            LED1=0;
        }   
    }
}

void startUp(void){
    int i = 0;
    int inputRight = PORTAbits.RA3;
    int inputLeft  = PORTAbits.RA2;
    rightMotorA1 = 0;
    rightMotorA2 = 0;
    leftMotorA1 = 0;
    leftMotorA2 = 0;
    while(i < 3){
        i ++;
        flashLEDs(50);
        wait10ms(50);
    }
    if (inputLeft != 0 && inputRight != 0){
        while(inputRight != 0 || inputLeft != 0){
            inputRight = PORTAbits.RA3;
            inputLeft  = PORTAbits.RA2;
            rightMotorA1 = 0;
            rightMotorA2 = 1;
            leftMotorA1 = 1;
            leftMotorA2 = 0;
            LED1 = 1;
            LED4 = 1;
        }
    }
    LED1 = 0;
    LED4 = 0;
    rightMotorA1 = 0;
    rightMotorA2 = 0;
    leftMotorA1 = 0;
    leftMotorA2 = 0;
}

void flashLEDs(int del){
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    wait10ms(del);
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
}

void wait10ms(int del){     	 //delay function
	int c;
	for(c=0;c<del;c++)
    	__delay_ms(10);
	return;
} 

void findBeacon(void){
    int inputRight = PORTAbits.RA3;
    int inputLeft  = PORTAbits.RA2;
    if (inputRight == 0 && inputLeft != 0){
        while(inputRight != 0 || inputLeft != 0){
            inputRight = PORTAbits.RA3;
            inputLeft  = PORTAbits.RA2;
            rightMotorA1 = 1;
            rightMotorA2 = 0;
            leftMotorA1 = 0;
            leftMotorA2 = 1;
            LED4 = 1;
        }
    }
    else if (inputLeft == 0 && inputRight != 0){
        while(inputRight != 0 || inputLeft != 0){
            inputRight = PORTAbits.RA3;
            inputLeft  = PORTAbits.RA2;
            rightMotorA1 = 0;
            rightMotorA2 = 1;
            leftMotorA1 = 1;
            leftMotorA2 = 0;
            LED1 = 1;
        }
    }
    LED1 = 0;
    LED4 = 0;
    rightMotorA1 = 0;
    rightMotorA2 = 0;
    leftMotorA1 = 0;
    leftMotorA2 = 0;
}

unsigned int readADCLeft(void) {
    ADCON0 = 0b00000011; //select A/D channel AN0,start conversion
    while (ADCON0bits.GO); //do nothing while conversion in progress
    return ((ADRESH << 8) + ADRESL); //Combines high and low A/D bytes into one
}                                 // value and returns this A/D value 0-1023

unsigned int readADCRight(void) {
    ADCON0 = 0b0000111; //select A/D channel AN0,start conversion
    while (ADCON0bits.GO); //do nothing while conversion in progress
    return ((ADRESH << 8) + ADRESL); //Combines high and low A/D bytes into one
}                                 // value and returns this A/D value 0-1023

void turn90(char direction, int del){
    int encoderRight = PORTCbits.RC5;
    int encoderLeft  = PORTCbits.RC0;
    int sumR = 0;
    int sumL = 0;
    reverse(del - 40);
    if(direction == 'l'){
        rightMotorA1 = 0;
        rightMotorA2 = 1;
        leftMotorA1 = 1;
        leftMotorA2 = 0;
        while(sumR < 100)
        wait10ms(40);          //wait 1 second
        rightMotorA1 = 0;
        rightMotorA2 = 0;
        leftMotorA1 = 0;
        leftMotorA2 = 0;
        goForward(del);
        leftMotorA1 = 0;
        leftMotorA2 = 1;
        rightMotorA1 = 1;
        rightMotorA2 = 0;
        wait10ms(40);
        leftMotorA1 = 0;
        leftMotorA2 = 0;
        rightMotorA1 = 0;
        rightMotorA2 = 0;
    }
    if(direction == 'r'){
        leftMotorA1 = 0;
        leftMotorA2 = 1;
        rightMotorA1 = 1;
        rightMotorA2 = 0;
        wait10ms(40);
        leftMotorA1 = 0;
        leftMotorA2 = 0;
        rightMotorA1 = 0;
        rightMotorA2 = 0;
        goForward(del);
        rightMotorA1 = 0;
        rightMotorA2 = 1;
        leftMotorA1 = 1;
        leftMotorA2 = 0;
        wait10ms(40);          //wait 1 second
        rightMotorA1 = 0;
        rightMotorA2 = 0;
        leftMotorA1 = 0;
        leftMotorA2 = 0;
    }
    goForward(del);
}

void goForward(int del){
    if (del == 0){
        rightMotorA1 = 0;
        rightMotorA2 = 1;
        leftMotorA1 = 0;
        leftMotorA2 = 1;
    }
    else{
        rightMotorA1 = 0;
        rightMotorA2 = 1;
        leftMotorA1 = 0;
        leftMotorA2 = 1;
        wait10ms(del);          //wait 1 second
        rightMotorA1 = 0;
        rightMotorA2 = 0;
        leftMotorA1 = 0;
        leftMotorA2 = 0;
    }
}
void reverse(int del){
    if (del == 0){
        rightMotorA1 = 1;
        rightMotorA2 = 0;
        leftMotorA1 = 1;
        leftMotorA2 = 0;
    }
    else{
        rightMotorA1 = 1;
        rightMotorA2 = 0;
        leftMotorA1 = 1;
        leftMotorA2 = 0;
        wait10ms(del);          //wait 1 second
        rightMotorA1 = 0;
        rightMotorA2 = 0;
        leftMotorA1 = 0;
        leftMotorA2 = 0;
    }
    
}

int isBeaconFound(void){
    int inputRight = PORTAbits.RA3;
    int inputLeft  = PORTAbits.RA2;
    inputRight = PORTAbits.RA3;
    inputLeft  = PORTAbits.RA2;
    if(inputLeft == 0 || inputRight == 0){
        return 1;
    }
    else{
        return 0;
    }
}