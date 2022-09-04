/* 
 * File:   mcal_internal_interrupt.h
 * Author: ahmed
 *
 * Created on August 4, 2022, 10:46 AM
 */

#ifndef MCAL_INTERNAL_INTERRUPTS_H
#define	MCAL_INTERNAL_INTERRUPTS_H


/* Includes */
#include "mcal_interrupt_config.h"

/* Macro Defines */

/* Macro Functions */
#if ADC_INT_ENABLE==FEATURE_ENABLE
#define INT_ADC_ENABLE()                          (PIE1bits.ADIE = 1 )
#define INT_ADC_DISABLE()                         (PIE1bits.ADIE = 0 )

#define INT_ADC_CLEAR_FLAG()                      (PIR1bits.ADIF = 0)

#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_ADC_PRI_SET_HIGH()                    (IPR1bits.ADIP = 1)
#define INT_ADC_PRI_SET_LOW()                     (IPR1bits.ADIP = 0)
#endif
#endif
/* Timer0  */
#if TIMER0_INT_ENABLE==FEATURE_ENABLE
#define INT_TMR0_ENABLE()                          (INTCONbits.TMR0IE = 1 )
#define INT_TMR0_DISABLE()                         (INTCONbits.TMR0IE = 0 )

#define INT_TMR0_CLEAR_FLAG()                      (INTCONbits.TMR0IF = 0)

#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_TMR0_PRI_SET_HIGH()                    (INTCON2bits.TMR0IP = 1)
#define INT_TMR0_PRI_SET_LOW()                     (INTCON2bits.TMR0IP = 0)
#endif
#endif
/* Timer1  */
#if TIMER1_INT_ENABLE==FEATURE_ENABLE
#define INT_TMR1_ENABLE()                          (PIE1bits.TMR1IE = 1)
#define INT_TMR1_DISABLE()                         (PIE1bits.TMR1IE = 0)

#define INT_TMR1_CLEAR_FLAG()                      (PIR1bits.TMR1IF = 0)

#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_TMR1_PRI_SET_HIGH()                    (IPR1bits.TMR1IP = 1)
#define INT_TMR1_PRI_SET_LOW()                     (IPR1bits.TMR1IP = 0)
#endif
#endif
/* Timer2  */
#if TIMER2_INT_ENABLE==FEATURE_ENABLE
#define INT_TMR2_ENABLE()                          (PIE1bits.TMR2IE = 1)
#define INT_TMR2_DISABLE()                         (PIE1bits.TMR2IE = 0)

#define INT_TMR2_CLEAR_FLAG()                      (PIR1bits.TMR2IF = 0)

#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_TMR2_PRI_SET_HIGH()                    (IPR1bits.TMR2IP = 1)
#define INT_TMR2_PRI_SET_LOW()                     (IPR1bits.TMR2IP = 0)
#endif
#endif

/* Timer3  */
#if TIMER3_INT_ENABLE==FEATURE_ENABLE
#define INT_TMR3_ENABLE()                          (PIE2bits.TMR3IE = 1)
#define INT_TMR3_DISABLE()                         (PIE2bits.TMR3IE = 0)

#define INT_TMR3_CLEAR_FLAG()                      (PIR2bits.TMR3IF = 0)

#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_TMR3_PRI_SET_HIGH()                    (IPR2bits.TMR3IP = 1)
#define INT_TMR3_PRI_SET_LOW()                     (IPR2bits.TMR3IP = 0)
#endif
#endif

/* CCP2  */
#if CCP2_INT_ENABLE==FEATURE_ENABLE
#define INT_CCP2_ENABLE()                          (PIE2bits.CCP2IE = 1)
#define INT_CCP2_DISABLE()                         (PIE2bits.CCP2IE = 0)

#define INT_CCP2_CLEAR_FLAG()                      (PIR2bits.CCP2IF = 0)

#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_CCP2_PRI_SET_HIGH()                    (IPR2bits.CCP2IP = 1)
#define INT_CCP2_PRI_SET_LOW()                     (IPR2bits.CCP2IP = 0)
#endif
#endif

/* EUSART Tx  */
#if EUSART_Tx_INT_ENABLE==FEATURE_ENABLE
#define INT_EUSART_Tx_ENABLE()                        (PIE1bits.TXIE = 1)
#define INT_EUSART_Tx_DISABLE()                       (PIE1bits.TXIE = 0)


#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_EUSART_Tx_PRI_SET_HIGH()                  (IPR1bits.TXIP = 1)
#define INT_EUSART_Tx_PRI_SET_LOW()                   (IPR1bits.TXIP = 0)

#endif
#endif

/* EUSART Rx  */

#if EUSART_Rx_INT_ENABLE==FEATURE_ENABLE
#define INT_EUSART_Rx_ENABLE()                        (PIE1bits.RCIE = 1)
#define INT_EUSART_Rx_DISABLE()                       (PIE1bits.RCIE = 0)

#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_EUSART_Rx_PRI_SET_HIGH()                  (IPR1bits.RCIP = 1)
#define INT_EUSART_Rx_PRI_SET_LOW()                   (IPR1bits.RCIP = 0)

#endif
#endif

/* MSSP I2C  */

#if MSSP_I2C_INT_ENABLE==FEATURE_ENABLE
#define INT_MSSP_I2C_ENABLE()                        (PIE1bits.SSPIE = 1)
#define INT_MSSP_I2C_DISABLE()                       (PIE1bits.SSPIE = 0)

#define INT_MSSP_I2C_CLEAR_FLAG()                    (PIR1bits.SSPIF = 0)

#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_MSSP_I2C_PRI_SET_HIGH()                  (IPR1bits.SSPIP = 1)
#define INT_MSSP_I2C_PRI_SET_LOW()                   (IPR1bits.SSPIP = 0)

#endif
#endif
/* MSSP I2C BUS COL  */

#if MSSP_I2C_BUS_COL_INT_ENABLE==FEATURE_ENABLE
#define INT_MSSP_I2C_BUS_COL_ENABLE()                 (PIE2bits.BCLIE = 1)
#define INT_MSSP_I2C_BUS_COL_DISABLE()                (PIE2bits.BCLIE = 0)

#define INT_MSSP_I2C_BUS_COL_CLEAR_FLAG()             (PIR2bits.BCLIF = 0)


#if INT_PRI_LEVELS_ENABLE==FEATURE_ENABLE 
#define INT_MSSP_I2C_BUS_COL_PRI_SET_HIGH()           (IPR2bits.BCLIP = 1)
#define INT_MSSP_I2C_BUS_COL_PRI_SET_LOW()            (IPR2bits.BCLIP = 0)

#endif
#endif






/* User-Defined Data Type Declaration */

/* Software Interfaces (Prototypes) */


/* Global Shared Variables */

#endif	/* MCAL_INTERNAL_INTERRUPTS_H */

