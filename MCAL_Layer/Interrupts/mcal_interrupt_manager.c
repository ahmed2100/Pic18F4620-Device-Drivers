/* 
 * File:   mcal_interrupt_manager.c
 * Author: ahmed
 *
 * Created on August 4, 2022, 10:48 AM
 */

#include "mcal_interrupt_manager.h"

static volatile struct rbx_flags_t
{
    uint8_t rb4_flag : 1 ;
    uint8_t rb5_flag : 1 ;
    uint8_t rb6_flag : 1 ;
    uint8_t rb7_flag : 1 ;
};

struct rbx_flags_t flags = 
{
    
    .rb4_flag = 1,
    .rb5_flag = 1,
    .rb6_flag = 1,
    .rb7_flag = 1    
};


 
   


#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE
void __interrupt() InterruptManagerHigh(void)
{
    /* INT0 */
    if( (INT_ENABLE == INTCONbits.INT0IE)  && (INT_OCCUR == INTCONbits.INT0IF) )
    {
        INT0_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == INTCON3bits.INT2IE)  && (INT_OCCUR == INTCON3bits.INT2IF) )
    {
        INT2_ISR();
    }
    else { /* Do Nothing */}
 
    
    if( (INT_ENABLE == INTCONbits.TMR0IE)  && (INT_OCCUR == INTCONbits.TMR0IF) )
    {
        TMR0_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == PIE1bits.TMR2IE)  && (INT_OCCUR == PIR1bits.TMR2IF) )
    {
        TMR2_ISR();
    }
    else { /* Do Nothing */}
    
    
    if( (INT_ENABLE == PIE2bits.TMR3IE)  && (INT_OCCUR == PIR2bits.TMR3IF) )
    {
        TMR3_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == PIE2bits.CCP2IE)  && (INT_OCCUR == PIR2bits.CCP2IF) )
    {
        CCP2_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == PIE1bits.RCIE)  && (INT_OCCUR == PIR1bits.RCIF) )
    {
        EUSART_Rx_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == PIE1bits.TXIE)  && (INT_OCCUR == PIR1bits.TXIF) )
    {
        EUSART_Tx_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == PIE2bits.BCLIE)  && (INT_OCCUR == PIR2bits.BCLIF) )
    {
        MSSP_I2C_BC_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == PIE1bits.SSPIE)  && (INT_OCCUR == PIR1bits.SSPIF) )
    {
        MSSP_I2C_ISR();
    }
    else { /* Do Nothing */}
    

}

void __interrupt(low_priority) InterruptManagerLow(void)
{
    if( (INT_ENABLE == INTCON3bits.INT1IE)  && (INT_OCCUR == INTCON3bits.INT1IF) )
    {
        INT1_ISR();
    }
    else { /* Do Nothing */}
    
     /********************************* RBx **********************************************************/
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_HIGH == (PORTBbits.RB4)) && (INT_HIGH == flags.rb4_flag) )
    {
        flags.rb4_flag = INT_LOW ;
        RB4_ISR(INT_HIGH);
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_LOW == (PORTBbits.RB4)) && (INT_LOW == flags.rb4_flag) )
    {
        flags.rb4_flag = INT_HIGH ;
        RB4_ISR(INT_LOW);
    }
    else { /* Do Nothing */}
    /*___________________________________________________________________________________________________*/
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_HIGH == (PORTBbits.RB5)) && (INT_HIGH == flags.rb5_flag) )
    {
        flags.rb5_flag = INT_LOW ;
        RB5_ISR(INT_HIGH);
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_LOW == (PORTBbits.RB5)) && (INT_LOW == flags.rb5_flag) )
    {
        flags.rb5_flag = INT_HIGH ;
        RB5_ISR(INT_LOW);
    }
    else { /* Do Nothing */}
    /*___________________________________________________________________________________________________*/
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_HIGH == (PORTBbits.RB6)) && (INT_HIGH == flags.rb6_flag) )
    {
        flags.rb6_flag = INT_LOW ;
        RB6_ISR(INT_HIGH);
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_LOW == (PORTBbits.RB6)) && (INT_LOW == flags.rb6_flag) )
    {
        flags.rb6_flag = INT_HIGH ;
        RB6_ISR(INT_LOW);
    }
    else { /* Do Nothing */}
    /*___________________________________________________________________________________________________*/
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_HIGH == (PORTBbits.RB7)) && (INT_HIGH == flags.rb7_flag) )
    {
        flags.rb7_flag = INT_LOW ;
        RB7_ISR(INT_HIGH);
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_LOW == (PORTBbits.RB7)) && (INT_LOW == flags.rb7_flag) )
    {
        flags.rb7_flag = INT_HIGH ;
        RB7_ISR(INT_LOW);
    }
    else { /* Do Nothing */}
    /*___________________________________________________________________________________________________*/
  
    
    /************************************** PERIPHERALS *****************************************************/
    if( (INT_ENABLE == PIE1bits.ADIE)  && (INT_OCCUR == PIR1bits.ADIF) )
    {
        ADC_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == PIE1bits.TMR1IE)  && (INT_OCCUR == PIR1bits.TMR1IF) )
    {
        TMR1_ISR();
    }
    else { /* Do Nothing */}
    
//    if( (INT_ENABLE == PIE1bits.TMR2IE)  && (INT_OCCUR == PIR1bits.TMR2IF) )
//    {
//        TMR2_ISR();
//    }
//    else { /* Do Nothing */}
    

}
#else

void __interrupt() InterruptManager(void)
{
    
    if( (INT_ENABLE == INTCONbits.INT0IE)  && (INT_OCCUR == INTCONbits.INT0IF) )
    {
        INT0_ISR();
    }
    else { /* Do Nothing */}
    /* INT1 */
    if( (INT_ENABLE == INTCON3bits.INT1IE)  && (INT_OCCUR == INTCON3bits.INT1IF) )
    {
        INT1_ISR();

    }
    else { /* Do Nothing */}
    /* INT2 */
    if( (INT_ENABLE == INTCON3bits.INT2IE)  && (INT_OCCUR == INTCON3bits.INT2IF) )
    {
        INT2_ISR();
    }
    else { /* Do Nothing */}
    
    /********************************* RBx **********************************************************/
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_HIGH == (PORTBbits.RB4)) && (INT_HIGH == flags.rb4_flag) )
    {
        flags.rb4_flag = INT_LOW ;
        RB4_ISR(INT_HIGH);
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_LOW == (PORTBbits.RB4)) && (INT_LOW == flags.rb4_flag) )
    {
        flags.rb4_flag = INT_HIGH ;
        RB4_ISR(INT_LOW);
    }
    else { /* Do Nothing */}
    /*___________________________________________________________________________________________________*/
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_HIGH == (PORTBbits.RB5)) && (INT_HIGH == flags.rb5_flag) )
    {
        flags.rb5_flag = INT_LOW ;
        RB5_ISR(INT_HIGH);
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_LOW == (PORTBbits.RB5)) && (INT_LOW == flags.rb5_flag) )
    {
        flags.rb5_flag = INT_HIGH ;
        RB5_ISR(INT_LOW);
    }
    else { /* Do Nothing */}
    /*___________________________________________________________________________________________________*/
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_HIGH == (PORTBbits.RB6)) && (INT_HIGH == flags.rb6_flag) )
    {
        flags.rb6_flag = INT_LOW ;
        RB6_ISR(INT_HIGH);
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_LOW == (PORTBbits.RB6)) && (INT_LOW == flags.rb6_flag) )
    {
        flags.rb6_flag = INT_HIGH ;
        RB6_ISR(INT_LOW);
    }
    else { /* Do Nothing */}
    /*___________________________________________________________________________________________________*/
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_HIGH == (PORTBbits.RB7)) && (INT_HIGH == flags.rb7_flag) )
    {
        flags.rb7_flag = INT_LOW ;
        RB7_ISR(INT_HIGH);
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == INTCONbits.RBIE)  && (INT_OCCUR == INTCONbits.RBIF) && 
            (INT_LOW == (PORTBbits.RB7)) && (INT_LOW == flags.rb7_flag) )
    {
        flags.rb7_flag = INT_HIGH ;
        RB7_ISR(INT_LOW);
    }
    else { /* Do Nothing */}
    /*___________________________________________________________________________________________________*/
    
    /************************************** PERIPHERALS *****************************************************/
    
    /***************************************** ADC **********************************************************/
    if( (INT_ENABLE == PIE1bits.ADIE)  && (INT_OCCUR == PIR1bits.ADIF) )
    {
        ADC_ISR();
    }
    else { /* Do Nothing */}

    if( (INT_ENABLE == INTCONbits.TMR0IE)  && (INT_OCCUR == INTCONbits.TMR0IF) )
    {
        TMR0_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == PIE1bits.TMR1IE)  && (INT_OCCUR == PIR1bits.TMR1IF) )
    {
        TMR1_ISR();
    }
    else { /* Do Nothing */}
    
    
    
    /***************************************** EUSART **********************************************************/

     if( (INT_ENABLE == PIE1bits.RCIE)  && (INT_OCCUR == PIR1bits.RCIF) )
    {
        EUSART_Rx_ISR();
    }
    else { /* Do Nothing */}
    
    if( (INT_ENABLE == PIE1bits.TXIE)  && (INT_OCCUR == PIR1bits.TXIF) )
    {
        EUSART_Tx_ISR();
    }
    else { /* Do Nothing */}
}
#endif