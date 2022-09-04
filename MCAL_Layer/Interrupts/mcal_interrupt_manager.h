/* 
 * File:   mcal_interrupt_manager.h
 * Author: ahmed
 *
 * Created on August 4, 2022, 10:48 AM
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H


/* Includes */
#include "mcal_interrupt_config.h"


/* Macro Defines */
#define INT_HIGH    0x01
#define INT_LOW     0x00

/* Macro Functions */

/* User-Defined Data Type Declaration */

/* Software Interfaces (Prototypes) */

void INT0_ISR(void);

void INT1_ISR(void);

void INT2_ISR(void);

void RB7_ISR(uint8_t handler_select);
void RB6_ISR(uint8_t handler_select);
void RB5_ISR(uint8_t handler_select);
void RB4_ISR(uint8_t handler_select);

void ADC_ISR(void);

void TMR0_ISR(void);
void TMR1_ISR(void);
void TMR2_ISR(void);
void TMR3_ISR(void);

void CCP2_ISR(void);

void EUSART_Tx_ISR(void);
void EUSART_Rx_ISR(void);

void MSSP_I2C_ISR(void);
void MSSP_I2C_BC_ISR(void);

/* Global Shared Variables */

#endif	/* MCAL_INTERRUPT_MANAGER_H */

