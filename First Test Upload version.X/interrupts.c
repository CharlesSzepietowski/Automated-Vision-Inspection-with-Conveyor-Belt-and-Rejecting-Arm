/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif




#define BeltMotor PORTCbits.RC4 //ON OFF operations
#define Stop PORTBbits.RB1  //external interrupt input
#define Start PORTDbits.RD1  // normal pin Main while loop is checking state will read
#define ArmMotorForward PORTDbits.RD2  // output
#define ArmMotorReverse PORTDbits.RD3  // output
#define StartLEDGreen   PORTCbits.RC3  // sinking output
#define StopLEDRed      PORTAbits.RA6  // sinking output
#define VisionSystem    PORTBbits.RB2  // external interupt inuput
#define LaserBeam       PORTBbits.RB0  //external interupt  input



/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/
#define _XTAL_FREQ 4000000 
/* High-priority service */

extern unsigned char received_char;
extern unsigned char received_flag;
extern unsigned char rejects; //used for reject count and calculations
extern unsigned char good;
extern unsigned char totalamount;
char status=0;
char debounce(void);



#if defined(__XC) || defined(HI_TECH_C)
void __interrupt(high_priority) high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */
    
    if (INT1IF==1){  //logic for STOP button
                    
    ///DEbounce code
        status=debounce();
        
        if(status==1){// we will can put
            BeltMotor=0; //stop motor
            //Add If() for arm operation thus rehjection can be finished use normal delay function  and in if statment us OR logic for 
            if(ArmMotorForward==1||ArmMotorReverse==1){
            __delay_ms(5000); // cover both directions and extra
                
            ArmMotorForward=0;//arm motor left
            ArmMotorReverse=0; //arm motor right H-Bridge
            
            
            }
            StartLEDGreen=1;// GREEN LED STATUS ON is off now -sinking mode
            StopLEDRed=0;// RED LED STATUS STOP is lit now
            
            
            INT1IF=0;// reset flag 
        };
        
    
    }
    

  if (INT2IF==1) { //Jetson Nano -Vision System identification
      char status1;
      
      status1=debounce(); //used for project simulation due to not optimzed Jetson Code, Info is passed with every frame from camera currently.
      
            
      
      
        rejects++;
        
          while(TXIF==0){ /*page 301-The TXIF interrupt flag bit of the PIR1 register is set
        whenever the EUSART transmitter is enabled and no character is being held for transmission in the TXREG.*/
        }        
                
          TXREG='3';//passed information to bluetooth application 
          
          while(TXIF==0){ /*page 301-The TXIF interrupt flag bit of the PIR1 register is set
        whenever the EUSART transmitter is enabled and no character is being held for transmission in the TXREG.*/
        } 
          
        INT2IF=0;
    
         
        
        
        TMR0ON=1; //  Enable timer -to fast clock you have to adjust everything !!!!!
                  //Ejecting Arm operation govern by timer0
        
        // ARM HAS TO RETURN TO INITIAL POSITION
        
        
      }
      
     if (INT0IF==1){ //total part counter signal from laser beam
    
        totalamount++; // increase counter value for total amount
        
        
        
        TXREG='1';//passed information to bluetooth application 
         INT0IF=0;
    }
    
    
    ///Bluetooth Flag
    
      if (RCIF==1) // bit 5 RCIF: USART Receive Interrupt Flag bit- Only readable bit -PIR1 REGISTER -PAGE 98
                // RCIF is set when new char has been loaded into RCREG
    {
          received_flag=1;
        
         received_char=RCREG;
       
         
         
         
    }
    
    
    
    
      
    
      if (TMR0IF==1) //we can use it for arm operation reverse action and second timer for delay after detection of incorrect item so 
         {
        
      BeltMotor=0;// Conveyor belt stops before arm will go in forward direction
      /// H Bridge operations
      ArmMotorForward=1;
      __delay_ms(2000); //arm moves in forward direction for 2 sec
      ArmMotorForward=0;//
      
       __delay_ms(200);
      
       ArmMotorReverse=1;
       __delay_ms(2000);
       ArmMotorReverse=0;
      
       __delay_ms(200);
      
         
         
        TMR0ON=0; //  Disable timer
       
        
        TMR0IF=0;// clear timer overflow interrupt flag 
        TMR0=490; //VALUE IS WRONG preload value  for timer where 1 sec delay is required 1:16 prescaler NNOW IS 4MHz - 1MHz Timer clock  1MHz CPU clock  thus 250KHz Internal Timer Clock  
    
    BeltMotor=1;// Conveyor belt STARTS again after rejection process 
    }
    
    

           
    

#if 0
    
      /* TODO Add High Priority interrupt routine code here. */

      /* Determine which flag generated the interrupt */
      if(<Interrupt Flag 1>)
      {
          <Interrupt Flag 1=0>; /* Clear Interrupt Flag 1 */
      }
      else if (<Interrupt Flag 2>)
      {
          <Interrupt Flag 2=0>; /* Clear Interrupt Flag 2 */
      }
      else
      {
          /* Unhandled interrupts */
      }

#endif

}

/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void __interrupt(low_priority) low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 0

      /* TODO Add Low Priority interrupt routine code here. */

      /* Determine which flag generated the interrupt */
      if(<Interrupt Flag 1>)
      {
          <Interrupt Flag 1=0>; /* Clear Interrupt Flag 1 */
      }
      else if (<Interrupt Flag 2>)
      {
          <Interrupt Flag 2=0>; /* Clear Interrupt Flag 2 */
      }
      else
      {
          /* Unhandled interrupts */
      }

#endif

}




char debounce (void) {

    char state;
    char count;
    
    
    while(true){
    
        count=0;
        state=Stop;
        
        
        while(state==Stop) {
            
        count++;
        
        if (count==13){
            return state; 
        
        }
        __delay_ms(100);// switch dependend you have to check in data sheet
        }
    }


}



