/* 
 * File:   mcal_interrupt_gen_config.h
 * Author: ahmed
 *
 * Created on August 4, 2022, 11:57 AM
 */

#ifndef MCAL_INTERRUPT_GEN_CONFIG_H
#define	MCAL_INTERRUPT_GEN_CONFIG_H

/* Includes */

/* Macro Defines */
#define FEATURE_ENABLE  1
#define FEATURE_DISABLE 0


#define INT_PRI_LEVELS_ENABLE                      FEATURE_ENABLE

#define INT_EXTERNAL_INTx_FEATURE_ENABLE           FEATURE_ENABLE
#define INT_EXTERNAL_ON_CHANGE_FEATURE_ENABLE      FEATURE_ENABLE
#define ADC_INT_ENABLE                             FEATURE_ENABLE
#define TIMER0_INT_ENABLE                          FEATURE_ENABLE
#define TIMER1_INT_ENABLE                          FEATURE_DISABLE
#define TIMER2_INT_ENABLE                          FEATURE_DISABLE
#define TIMER3_INT_ENABLE                          FEATURE_ENABLE
#define CCP2_INT_ENABLE                            FEATURE_ENABLE
#define EUSART_Tx_INT_ENABLE                       FEATURE_ENABLE
#define EUSART_Rx_INT_ENABLE                       FEATURE_ENABLE
#define MSSP_I2C_INT_ENABLE                        FEATURE_ENABLE
#define MSSP_I2C_BUS_COL_INT_ENABLE                FEATURE_DISABLE

/* Macro Functions */


#endif	/* MCAL_INTERRUPT_GEN_CONFIG_H */

