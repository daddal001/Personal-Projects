
  /* File:   Complete.c
 * Author:  Saleh M Alsharideh, Malik Haider Ali,Abdallah Muhammed
 * Created: 08/03/2023
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#pragma config OSC = HS                 //High speed resonator
#pragma config WDT = OFF                //Watchdog timer off
#pragma config LVP = OFF                //Low voltage programming disabled
#pragma config PWRT = ON                //Power up timer on
#define _XTAL_FREQ 10000000             //Define clock frequency for __delay_10ms()
#define LED1 LATBbits.LATB2             //LED1
#define LED2 LATBbits.LATB3             //LED2
#define LED3 LATBbits.LATB4             //LED3
#define LED4 LATBbits.LATB5             //LED4
#define rightMotorA1 LATBbits.LATB0     //Right Motor A1
#define rightMotorA2 LATBbits.LATB1     //Right Motor A2
#define leftMotorA1 LATAbits.LATA4      //Left Motor A1
#define leftMotorA2 LATAbits.LATA5      //Left Motor A2

void I2C_Initialise(void);              //Initialise I2C
void I2C_checkbus_free(void);           //Wait until I2C bus is free
void I2C_Start(void);                   //Generate I2C start condition
void I2C_RepeatedStart(void);           //Generate I2C Repeat start condition
void I2C_Stop(void);                    //Generate I2C stop condition
void I2C_Write(unsigned char write);    //Generate I2C write condition
unsigned char I2C_Read(void);           //Generate I2C read condition
unsigned char GetLinesensor(void);      //Gets raw data from linesensor
void wait10ms(int del);                 //Generates a delay in multiples of 10ms
void configPWM(void);                   //Configure PWM
void goforward(void);                   //Turn both motors forward
unsigned int markspaceL;                //Mark space ratio for Left motor
unsigned int markspaceR;                //Mark space ratio for Right motor
int getAngle(void);                     // get angle 
void startup(void);                     // start up function turns the LEDs 5 time for 5 second
void stop(int delay);                   // stop 
unsigned int setpoint_distance = 400;   //Distance set point
unsigned int readADCLeft(void);         //Read ADC for left sensor
unsigned int readADCRight(void);        //Read ADC for right sensor

void main(void) 
{
    unsigned char linesensor;                   //Store raw data from sensor array
    unsigned char tempLinesensor = 0xFF;       //Store a copy of raw data from sensor array
    int angle;
    int error;          
    int u;
    int K = 30;
    int v;
    int v_l;
    int v_r;
    int lambda = 1;
    int loop = 0;
    
    ADCON2 = 0b10000010;
    TRISCbits.RC2 = 0; //set CCP1(pin13) to an output pin
    TRISCbits.RC1 = 0; //pin 12
    
    ADCON1 = 0b00001101;            //AN0,AN1 are analogue inputs,RA2 -RA5 are digital 
    TRISA = 0b11001111;             //Set PORTA pins
    TRISC = 0b00111001;             //Set PORTC pins
    TRISB = 0b11000000;      //configure Port B, RB0 to RB5 as outputs
    LATB = 0x00;                   //Turn All LEDs off
    I2C_Initialise();             //Initialise I2C Master 
    configPWM();                    //Configure PWM
    startup();
   
    while(1)
    {
                linesensor = GetLinesensor();

        if(readADCLeft() > 400 && readADCRight() > 400)    //          
        { 
                leftMotorA1 = 0;    //Left motor forward;
                leftMotorA2 = 0;    
                rightMotorA1 = 0;   //Right motor forward;
                rightMotorA2 = 0; 
        }
       else if(readADCLeft() >= 350 && readADCRight() >= 350)  
        {
            v = 50;
        }
        else if(readADCLeft() >= 300 && readADCRight() >= 300)  
        {
            v = 100;
        }
        else if(readADCLeft() >= 200 && readADCRight() >= 200)
        {
            v = 200;
        }
        else if(readADCLeft() >= 100 && readADCRight() >= 100)
        {
            v = 300;
        }
                
        else{
            v = 400;
        }
       
        angle = getAngle(); 
        error = 0 + angle;
        u = error*(K);
        v_l = v - lambda*(u);
        v_r = v + lambda*(u);
        
        markspaceL= v_l;   //Left motor speed 
        markspaceR= v_r;   //Right motor speed
        if (linesensor == 0x00 && tempLinesensor != linesensor)
        {
            loop++;
            if(loop == 2){         // change lane
                markspaceL= v_l;   //Left motor speed 
                markspaceR= v_r;   //Right motor speed
                wait10ms(10);      //wait 0.1 second
                leftMotorA1 = 0;    //Left motor forward;
                leftMotorA2 = 1;    
                rightMotorA1 = 1;   //Right motor forward;
                rightMotorA2 = 0;
                wait10ms(40);       //wait 0.4 second
                goforward();        // go forward
                wait10ms(35);      //wait 0.35 second
            }
            if(loop == 4){        //spin   
                stop(500);         //stop 5 second
                markspaceL= v_l;   //Left motor speed 
                markspaceR= v_r;   //Right motor speed
                leftMotorA1 = 0;    //Left motor forward;
                leftMotorA2 = 1;    
                rightMotorA1 = 1;   //Right motor forward;
                rightMotorA2 = 0;
                wait10ms(120);
            }
            if(loop == 5){         // change lane
                markspaceL= v_l;   //Left motor speed 
                markspaceR= v_r;   //Right motor speed
                wait10ms(10);      // wait 0.1 second
                leftMotorA1 = 0;    //Left motor forward;
                leftMotorA2 = 1;    
                rightMotorA1 = 1;   //Right motor forward;
                rightMotorA2 = 0;
                wait10ms(30);        //wait 0.3 second
                goforward();         // go forward
                wait10ms(25);       // wait 0.25 second
            }
            if(loop == 6){     //stop the robot and flash the LEDs for 5 second
                stop(0);       //stop 
                startup();     // flash the LED for 5 second
                wait10ms(50000); // wait 500 second
            }
        }
        else
        {
            goforward();     //Turn both motor forward
        }
        tempLinesensor = linesensor;
        I2C_Stop();
    }
}


unsigned char GetLinesensor(void)
{
    I2C_Start();                 //Send Start condition to slave
    I2C_Write(0x7C);             //Send 7 bit address + Write to slave
    I2C_Write(0x11);             //Write data, select RegdataA and send to slave
    I2C_RepeatedStart();         //Send repeat start condition
    I2C_Write(0x7D);             //Send 7 bit address + Read
    return(I2C_Read());             //Read  the IR sensors 
}

void I2C_Initialise(void)       //Initialise I2C
{
  SSPCON1 = 0b00101000;         //set to master mode, enable SDA and SCL pins
  SSPCON2 = 0;                  //reset control register 2
  SSPADD = 0x63;                //set baud rate to 100KHz
  SSPSTAT = 0;                  //reset status register
}

void I2C_checkbus_free(void)                    //Wait until I2C bus is free
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //wait until I2C bus is free
}

void I2C_Start(void)            //Generate I2C start condition
{
  I2C_checkbus_free();          //Test to see I2C bus is free
  SEN = 1;                      //Generate start condition,SSPCON2 bit 0 = 1
}

void I2C_RepeatedStart(void) //Generate I2C Repeat start condition
{
  I2C_checkbus_free();          //Test to see I2C bus is free
  RSEN = 1;                     //Generate repeat start, SSPCON2 bit1 = 1
}

void I2C_Stop(void)             //Generate I2C stop condition
{
  I2C_checkbus_free();          //Test to see I2C bus is free
  PEN = 1;                      // Generate stop condition,SSPCON2 bit2 = 1
}

void I2C_Write(unsigned char write) //Write to slave
{
  I2C_checkbus_free();                  //check I2C bus is free
  SSPBUF = write;                       //Send data to transmit buffer
}

unsigned char I2C_Read(void)    //Read from slave
{
  unsigned char temp;
  I2C_checkbus_free();          //Test to see I2C bus is free
  RCEN = 1;                     //enable receiver,SSPCON2 bit3 = 1
  I2C_checkbus_free();          //Test to see I2C bus is free
  temp = SSPBUF;                //Read slave
  I2C_checkbus_free();          //Test to see I2C bus is free
  ACKEN = 1;                    //Acknowledge
  return temp;                  //return sensor array data
}

void wait10ms(int del)          //delay function
{     
int c;
for(c=0;c<del;c++)
    __delay_ms(10);
return;
} 

void configPWM(void)            //Configures PWM
{   
    PR2 = 0b11111111 ;          //set period of PWM,610Hz
    T2CON = 0b00000111 ;        //Timer 2(TMR2)on, prescaler = 16 
    CCP1CON = 0b00001100;       //enable CCP1 PWM
    CCP2CON = 0b00001100;       //enable CCP2 PWM
    CCPR1L = 0;                 //turn left motor off
    CCPR2L = 0;                 //turn Right motor off
    return;
}

void goforward(void)    // go forward
{
    leftMotorA1 = 0;    //Left motor forward;
    leftMotorA2 = 1;    
    rightMotorA1 = 0;   //Right motor forward;
    rightMotorA2 = 1;
    CCP1CON = (0x0c)|((markspaceL&0x03)<<4);//0x0c enables PWM,then insert the 2 LSB
    CCPR1L = markspaceL>>2; //of markspaceL into CCP1CON and the higher 8 bits into
    CCP2CON = (0x0c)|((markspaceR&0x03)<<4); //CCPR1L.  Same as above but for 
    CCPR2L = markspaceR>>2;                  // CCP2CON and CCPR2L
    return;                 
}

int getAngle(void) //get angle
{
    unsigned char linesensor;
    linesensor = GetLinesensor();
    int angle = 0;
    switch (linesensor)
    {
        case 0b11111110:
            angle = 12;
            break;
        case 0b11111100:
            angle = 10;
            break;
        case 0b11111101:
            angle = 9;
            break;
        case 0b11111001:
            angle = 7;
            break;
        case 0b11111011:
            angle = 5;
            break;
        case 0b11110011:
            angle = 3;
            break;
        case 0b11110111:
            angle = 2;
            break;
        case 0b11100111:
            angle = 0;
            break;
        case 0b11101111:
            angle = -2;
            break;
        case 0b11001111:
            angle = -3;
            break;
        case 0b11011111:
            angle = -5;
            break;
        case 0b10011111:
            angle = -7;
            break;
        case 0b10111111:
            angle = -9;
            break;
        case 0b00111111:
            angle = -10;
            break;
        case 0b01111111:
            angle = -12;
            break;
       
    }
    return angle;
}
void startup(void) // start up function turns the LEDs 5 time for 5 second
{
    for(int i = 0; i < 5; i++){
        LED1 = 1; //turn LED1 on
        LED2 = 1;//turn LED2 on
        LED3 = 1;//turn LED3 on
        LED4 = 1;//turn LED4 on
        wait10ms(50); //wait 0.5 seconds
        LED1 = 0; //turn LED1 off
        LED2 = 0; //turn LED2 off
        LED3 = 0; //turn LED3 off
        LED4 = 0; //turn LED4 off
        wait10ms(50); //wait 0.5 seconds
    }
}
void stop(int delay) // stop function
{
    markspaceL= 300;   //Left motor speed 
    markspaceR= 300;   //Right motor speed
    leftMotorA1 = 1;    //Left motor forward;
    leftMotorA2 = 0;    
    rightMotorA1 = 1;   //Right motor forward;
    rightMotorA2 = 0;
    wait10ms(10);
    leftMotorA1 = 0;    //Left motor forward;
    leftMotorA2 = 0;    
    rightMotorA1 = 0;   //Right motor forward;
    rightMotorA2 = 0; 
    wait10ms(delay); 
}

unsigned int readADCLeft(void) // Function to read the left ADC
{
    ADCON0 = 0b00000011; //select A/D channel AN0,start conversion
    while (ADCON0bits.GO); //do nothing while conversion in progress
    return ((ADRESH << 8) + ADRESL); //Combines high and low A/D bytes into one
}                                 // value and returns this A/D value 0-1023

unsigned int readADCRight(void)   // Function to read the left ADC
{
    ADCON0 = 0b0000111; //select A/D channel AN0,start conversion
    while (ADCON0bits.GO); //do nothing while conversion in progress
    return ((ADRESH << 8) + ADRESL); //Combines high and low A/D bytes into one
}                                 // value and returns this A/D value 0-1023
