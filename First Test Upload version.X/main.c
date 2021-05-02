/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned char received_char=0;// varible defined for data transfger storage between two PICs  
unsigned char received_flag=0;
unsigned char rejects=0; //used for reject count and calculations
unsigned char good=0;
unsigned char totalamount=0;


char status=0;



/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

     #define _XTAL_FREQ 4000000

//#define DATA PORTB
//#define CS1 PORTCbits.RC0
//#define CS2 PORTCbits.RC1
//#define RS PORTCbits.RC2
//#define EN PORTCbits.RC3
//#define R_W PORTCbits.RC4


#define BeltMotor PORTCbits.RC4 //ON OFF operations
#define Start PORTDbits.RD1  // normal pin Main while loop is checking state will read
#define ArmMotorForward PORTDbits.RD2  // output
#define ArmMotorReverse PORTDbits.RD3  // output
#define StartLEDGreen   PORTCbits.RC3  // sinking output
#define StopLEDRed      PORTAbits.RA6  // sinking output



char debounceStart (void); 



void main(void)
{
    
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

   
    Start=1;
    ArmMotorForward=0;
    ArmMotorReverse=0;
    StopLEDRed=0;//sinking mode
    StartLEDGreen=1;
    BeltMotor=0;
    
   
    
    while(1)
    {
        
    
        if (received_flag==1) {
        
            
            
           while(TXIF==0){ /*page 301-The TXIF interrupt flag bit of the PIR1 register is set
        whenever the EUSART transmitter is enabled and no character is being held for transmission in the TXREG.*/
        } 
         
           if(received_char=='2') //STOP COmmanD
           {
            BeltMotor=0; //stop main motor
            ArmMotorReverse=0;//rejecting arm motor reverse
            ArmMotorForward=0; //arm motor right H-Bridge
            StartLEDGreen=1;// GREEN LED STATUS ON is off now -sinking mode
            StopLEDRed=0;// RED LED STATUS STOP is lit now
           received_char='0';//reset content to not used value
           }
           
           else if( received_char=='5')//START
           {
               
           //Code is not optimsed in terms of operation memory when stopped it starts regardless status of rejecting arm code is not optimised 
               
               BeltMotor=1; // Main Belt ON  - Represented by Red kd
               StopLEDRed=1;// RED LED OFF -is in sinking mode
               StartLEDGreen=0;//GREEN LED ON
               
               received_char=0;//reset content to not used value    
           }
           
           
           
           else { // RESET Command 
           
                rejects=0; //used for reject count and calculations
                good=0;
                totalamount=0;

            //this is for DEBUG test as well   
           TXREG=received_char;//resend commend to reset back to Bluetooth device
          
           
           }
           
            received_char='0';//reset content to not used value
           
          
           
        
        received_flag=0;// condition reset for next interrupt when new ASCI code will be received 
        
        
        
        
        }
        
      
        
        
        
        
        if (Start==1&&StartLEDGreen==1){
           
            
            status=debounceStart();//start main motor
            if(status==1){
            
            BeltMotor=1;
                
                StopLEDRed=1;// RED LED OFF -is in sinking mode
               StartLEDGreen=0;//GREEN LED ON
               
                TXREG='5';//Start send to Bluetooth device
            }
            }
            
        
       
    }

}


char debounceStart (void) {

    char state;
    char count;
    
    
    while(true){
    
        count=0;
        state=Start;
        
        
        while(state==Start) {
            
        count++;
        
        if (count==13){
            return state; 
        
        }
        __delay_ms(100);// switch dependend you have to check in data sheet
        }
    }


}